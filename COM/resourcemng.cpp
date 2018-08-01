/***************************************************************************
 */
/*!       \file	 resourcemng.cpp 
 *       \brief  Implementation of resource manager
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:27:36 $
 *    $Revision: 2.0 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: resourcemng.cpp,v $
 * Revision 2.0  2010/01/22 11:27:36  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.4  2004/04/13 11:01:24  dpfeuffer
 * cosmetics
 *
 * Revision 1.3  2003/06/05 12:01:21  dschmidt
 * intermediate checkin
 *
 * Revision 1.2  2003/06/05 09:33:53  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:36:22  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include "resourcemng.h"
#include "wizdebug.h"

//----------------------------------------------------------------------
//! creates a ResourceMng region
ResourceRegion::ResourceRegion( 
	ulong _start, 
	ulong _size, 
	HwComponent *_owner )
{
	start = _start;
	size = _size;
	owner = _owner;
}

//----------------------------------------------------------------------

//! creates a resource manager
/*!
  \param _resName resource name
*/

ResourceMng::ResourceMng( const QString &_resName )
{
	resName = _resName;
	nextAlloc = 0;
}


//! add a region to list of available regions
/*!
  \param start start of region
  \param size size of region
  \throw ResourceMngAlreadyAvail if region is already in free list
*/
void 
ResourceMng::addRegion( ulong start, ulong size )
{
	insertToAvail( start, size );
}

ulong 
ResourceMng::align( ulong start, ulong alignment )
{
	if( start & (alignment-1))
		start = (start & ~(alignment-1)) + alignment;
	return start;
}

//! inserts a region into the list of available regions
/*!
  \param start start of region
  \param size size of region
  
  \throw ResourceMngAlreadyAvail if the area is already in free list
*/
void
ResourceMng::insertToAvail( ulong start, ulong size )
{
	ResourceRegion *reg = lstAvail.first();
	ResourceRegion *newReg;
	uint idx=0;

	// check if the area is not already in free list
	while( reg ){
		if( (start < reg->start+reg->size) &&
			(start+size > reg->start ))
			throw ResourceMngAlreadyAvail( resName, start );
		reg = lstAvail.next();
	}

	// ok, now add it to the free list; combine with already existing areas
    // if possible
	reg = lstAvail.first();

	while( reg ){
		if( start + size == reg->start ){
			// new area ends at existing region's start
			reg->start = start;
			reg->size += size;
			return;
		}
		else if( reg->start + reg->size == start ){
			// existing area ends at start
			reg->size += size;

			// check if the new end is exactly the start of the next region
			ResourceRegion *reg2 = lstAvail.next();
			if( reg2 && reg2->start == reg->start + reg->size ){
				// yes, combine the two regions to one
				reg->size += reg2->size;
				lstAvail.remove( reg2 );
				delete reg2;
			}
			return;
		}
		else if( reg->start > start ){
			// existing region is behind new area
			newReg = new ResourceRegion( start, size, 0 );
			lstAvail.insert( idx, newReg );
			return;
		}
		reg = lstAvail.next();
		idx++;
	}
	// new area is at end of regions
	newReg = new ResourceRegion( start, size, 0 );
	lstAvail.append( newReg );
}	

// attempt to allocate area beginning at offset in free region 
ResourceMng::AllocResult
ResourceMng::allocInRegion( 
	ResourceRegion *reg, 
	ulong offset,
	ulong size, 
	ulong alignment,
	ulong &start,
	HwComponent *caller)
{
    // get aligned start of region
	ulong s = align(reg->start+offset, alignment); 
	ResourceRegion *allocReg;
		
	if( s+size <= reg->start+reg->size ){
		// region can hold requested area
			
		// create a new region to store into the allocated list
		allocReg = new ResourceRegion( s, size, caller );
		lstAlloc.append( allocReg );

		// remove area from available regions
		lstAvail.remove( reg );

		if( s+size < reg->start+reg->size )
			insertToAvail( s+size, (reg->start+reg->size) - (s+size));
		if( s > reg->start )
			insertToAvail( reg->start, s - reg->start);
		
		delete reg;
		start = s;
		return AllocResOk;
	}
	else
		return AllocResFail;
}

//! allocate region of size \a size aligned at \a alignment
/*!
  attempts to allocate it at \a _nextAlloc if possible. If not possible,
  tries to allocate any other free region that matches \a alignment.

  \return AllocResOk if region could be allocated (\a start contains the 
  start of region)\n
  AllocResFail if resource could not be allocated
*/
  
ResourceMng::AllocResult
ResourceMng::alloc( ulong size, ulong alignment, ulong &start, 
					HwComponent *caller, ulong _nextAlloc )
{
	setNextAlloc( _nextAlloc );
	return alloc( size, alignment, start, caller );	
}

//! allocate region of size \a size aligned at \a alignment
/*!
  Tries to allocate any free region that matches \a alignment.

  \return AllocResOk if region could be allocated (\a start contains the 
  start of region)\n
  AllocResFail if resource could not be allocated
*/
  
ResourceMng::AllocResult
ResourceMng::alloc( ulong size, ulong alignment, ulong &start, 
					HwComponent *caller )
{
	ResourceRegion *reg = lstAvail.first();
	ulong s;

	// find resource that contains <nextAlloc>
	while( reg ){
		if( (reg->start <= nextAlloc) && (reg->start + reg->size > nextAlloc)){
			if( allocInRegion( reg, nextAlloc-reg->start,
							   size, alignment, s, caller ) == AllocResOk ){
				nextAlloc = s + size;
				start = s;
				return AllocResOk;
			}
			else
				break;
		}
		reg = lstAvail.next();
	}
	
	// could not allocate at nextAlloc, try all available regions
	reg = lstAvail.first();

	while( reg ){
		if( allocInRegion( reg, 0, size, alignment, s, caller ) == 
			AllocResOk ){

			nextAlloc = s + size;
			start = s;
			return AllocResOk;
		}

		reg = lstAvail.next();
	}
		
	return AllocResFail;
}

//! free area beginning at start (size is maintained internally)
/*!
  \throw ResourceMngAlreadyAvail if the area is already in free list
  \throw ResourceMngNotAlloced if area was not allocated
  \throw ResourceMngNotOwned if area was not owned by \a caller
*/
void
ResourceMng::free( ulong start, HwComponent *caller )
{
	ResourceRegion *reg = lstAlloc.first();
	ulong rstart, rsize;

	while( reg ){
		if( reg->start == start ){
			if( reg->owner == caller ){
				lstAlloc.remove( reg );
				rsize = reg->size;
				rstart = reg->start;
				delete reg;

				insertToAvail( rstart, rsize );
				return;
			}
			else {
				// region not owned
				throw ResourceMngNotOwned( resName, start );
			}
		}
		reg = lstAlloc.next();
	}
	// not found in allocated list
	throw ResourceMngNotAlloced( resName, start );	
}

//! free all resources owned by \a caller
void 
ResourceMng::freeAllOfOwner( HwComponent *caller )
{
	Q3PtrListIterator<ResourceRegion> it(lstAlloc);
	ResourceRegion *reg, *regnext;
	
	// need to perform this tricky way, since we remove entries from list
	regnext = it.current();

	while( (reg = regnext) ){
		++it;
		regnext = it.current();

		wDebug(("chk: %lx", reg->start ));
		if( reg->owner == caller ){
			wDebug(("free: %lx", reg->start ));
			free( reg->start, caller );
		}
	}
}

//! check if requested region is available
/*!
  \return \c Avail if region available\n
  \c Unavail if not\n
  \c InUse and \a *ownerP set to the region's owner if area was already 
  allocated
*/
ResourceMng::IsAvailResult
ResourceMng::isAvail( ulong start, ulong size, HwComponent **ownerP )
{
	ResourceRegion *reg;

	*ownerP = 0;

	// check if area is in allocated list
	reg = lstAlloc.first();

	while( reg ){
		// check if requested area overlaps allocated area
		if( (start < reg->start+reg->size) &&
			(start+size > reg->start )){

			*ownerP = reg->owner;
			return InUse;
		}
		reg = lstAlloc.next();
	}

	// now we know that the area is not already allocated.
	// Check if it is in free list
	reg = lstAvail.first();
	
	while( reg ){
		if( start >= reg->start && start+size <= reg->start+reg->size )
			return Avail;	// ok, found region
		reg = lstAvail.next();
	}
	
	return Unavail;
}


//! dump resource using wDebug()
void 
ResourceMng::dump()
{
	ResourceRegion *reg;
	int i;

	wDebug((" === ResourceMng %s ===", (const char *)resName ));
	wDebug((" Next alloc 0x%08lx", nextAlloc ));
	reg = lstAvail.first();
	if( reg ){ 
		wDebug(( " Available" ));
	}

	i=0;
	while( reg ){
		wDebug(("  Region %d 0x%08lx 0x%08lx %p", i, reg->start, 
			   reg->size, reg->owner ));
		reg = lstAvail.next();
		i++;
	}

	reg = lstAlloc.first();
	if( reg ){
		wDebug(( " Allocated" ));
	}

	i=0;
	while( reg ){
		wDebug(("  Region %d 0x%08lx 0x%08lx %p", i, reg->start, 
			   reg->size, reg->owner ));
		reg = lstAlloc.next();
		i++;
	}
	
}


