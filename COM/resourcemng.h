/***************************************************************************
 */
/*!       \file  resourcemng.h
 *       \brief  resource manager
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:27:38 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: resourcemng.h,v $
 * Revision 2.0  2010/01/22 11:27:38  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.1  2003/05/02 14:36:24  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef RESOURCEMNG_H
#define RESOURCEMNG_H

//----------------------------------------------------------------------
// Resource management

#include <qlist.h>
//Added by qt3to4:
#include <Q3PtrList>
#include "wizexcept.h"

class HwComponent;

//! a resource region
class ResourceRegion {
public:
	ResourceRegion( ulong start, ulong size, HwComponent *owner); 
	ulong start;	//!< start of region
	ulong size;		//!< size of region
	HwComponent *owner;	//!< hwcomponent that owns region
};

//----------------------------------------------------------------------

//! resource manager
/*!
  This class can be used to keep track of available resources of a certain
  type. For example, it can be used to keep track of the available address
  regions within a VME address space or to keep track of available CompactPCI
  slot.

  Currently, the region start and size parameters are limited to unsigned
  32 bit integers.

  A resource may have initially zero or more available regions, these regions
  have to be created by addRegion().

  A region can be requested by HwComponent subclasses using alloc() functions.

  When the region is no more used, it has to be released using free() or
  freeAllOfOwner().
*/
  
class ResourceMng {
public:
	//! return value of alloc()
	enum AllocResult { AllocResOk, AllocResFail };
	//! return value of isAvail()
	enum IsAvailResult { Avail, Unavail, InUse };
	
	ResourceMng( const QString &_resName );
	void addRegion( ulong start, ulong size );
	AllocResult alloc( ulong size, ulong alignment, ulong &start, 
					   HwComponent *caller );
	AllocResult alloc( ulong size, ulong alignment, ulong &start, 
					   HwComponent *caller, ulong _nextAlloc );
	void free( ulong start, HwComponent *caller);
	void freeAllOfOwner( HwComponent *caller );
	IsAvailResult isAvail( ulong start, ulong size, HwComponent **ownerP );

	//! set the region start that is tried first on next alloc()
	void setNextAlloc(ulong next) { nextAlloc=next; }
	void dump();
private:
	ulong align( ulong start, ulong alignment );
	AllocResult allocInRegion( ResourceRegion *reg, ulong offset, ulong size,
						 ulong alignment, ulong &start, HwComponent *caller);
	void insertToAvail( ulong start, ulong size );
	QString resName;	// name of resource
	Q3PtrList<ResourceRegion> lstAvail;	// available regions
	Q3PtrList<ResourceRegion> lstAlloc;	// allocated regions
	ulong nextAlloc;	
};


//----------------------------------------------------------------------
//! base class for exceptions thrown by ResourceMng
class ResourceMngExcept : public WizExcept {
public:
};

//! exception when attempting to free an already available region
class ResourceMngAlreadyAvail : public ResourceMngExcept {
public:
	//! creates ResourceMngAlreadyAvail exception
	ResourceMngAlreadyAvail( const QString &mngName, ulong start ) {
		msg.sprintf("Resource Manager %s Free exception: "
					"Resource at 0x%lx already available",
					(const char *)mngName, start );
	}
};

//! exception when attempting to free a non allocated region
class ResourceMngNotAlloced : public ResourceMngExcept {
public:
	//! creates ResourceMngNotAlloced exception
	ResourceMngNotAlloced( const QString &mngName, ulong start ) {
		msg.sprintf("Resource Manager %s Free exception: "
					"Resource at 0x%lx not allocated",
					(const char *)mngName, start );
	}
};

//! exception when attempting to free a region not owned by caller
class ResourceMngNotOwned : public ResourceMngExcept {
public:
	//! creates ResourceMngNotOwned exception
	ResourceMngNotOwned( const QString &mngName, ulong start ) {
		msg.sprintf("Resource Manager %s Free exception: "
					"Resource at 0x%lx not owned",
					(const char *)mngName, start );
	}
};

#endif

