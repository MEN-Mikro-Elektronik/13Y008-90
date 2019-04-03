/***************************************************************************
 */
/*!       \file	 namemng.cpp 
 *       \brief  Implementation of device name manager
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:26:44 $
 *    $Revision: 2.0 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: namemng.cpp,v $
 * Revision 2.0  2010/01/22 11:26:44  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.5  2005/03/09 13:49:24  ub
 * cosmetics
 *
 * Revision 1.4  2004/06/08 17:25:38  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.3  2003/06/05 12:01:10  dschmidt
 * intermediate checkin
 *
 * Revision 1.2  2003/06/05 09:33:38  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:36:10  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include "namemng.h"
#include "wizdebug.h"

//! creates a device name manager
/*!
  \param _mngName manager name
*/
NameMng::NameMng( const QString &_mngName )
{
	mngName = _mngName;
	lstNames.setAutoDelete(true);
}

//! attempt to allocate a name. 
/*!
  \param name the name to allocate
  \param _caller pointer to caller's class

  \return true if allocation successfull or false if \a name is already 
  allocated
*/
bool
NameMng::alloc( const QString &name, void *_caller )
{
	NameMngEntry *newName;

	if( !isAvail( name ))
		return false;

	newName = new NameMngEntry(name.lower(),_caller);
	lstNames.append( newName );

	return true;
}

//! free \a name
/*!
  \throw NameMng::NotAllocated if \a name not allocated 
  \throw NameMng::NotOwner if \a name is not owned by \a caller
*/
void
NameMng::free( const QString &name, void *_caller )
{
    NameMngEntry *p = lstNames.first();

	while(p){
		if( p->name == name.lower() ){
			if( p->caller == _caller ){
                wDebug(("  freeing %s", p->name.ascii() ));
				lstNames.remove(p);
				return;
			}
			else 
				throw NotOwner(mngName,name.lower());
		}
		p = lstNames.next();
	}
	throw NotAllocated(mngName,name);
}


//! free all names owned by \a caller
void 
NameMng::freeAllOfOwner( void *caller )
{
	Q3PtrListIterator<NameMngEntry> it(lstNames);
	NameMngEntry *p, *pnext;

	pnext = it.current();

	while( (p = pnext) ){
		++it;
		pnext = it.current();

		if( p->caller == caller )
			free( p->name, caller );
	}
}

//! test if \a name is available
/*!
   \return true if name is available, false if already allocated
*/
bool
NameMng::isAvail( const QString &name )
{
	NameMngEntry *p = lstNames.first();

	while(p){
		if( p->name == name.lower() ){
			return false;
		}
		p = lstNames.next();
	}
	return true;
}

//! suggest an available name for \a baseName
/*!
  e.g. if \a baseName is "a201", suggest() tries "a201_1", "a201_2" until 
  it finds an available name

  \return an available name according to \a baseName
*/
QString
NameMng::suggest( const QString &baseName )
{
	QString name, bn;
	int idx = 1;
	bool addUnderscore=true;

	bn = baseName.lower();

	// start idx from 0 for devices appended with "__idx0"
	// do not add underscore after device_name
	if ( bn.contains("__idx0") ){
		bn.remove("__idx0");
		addUnderscore=false;
		idx=0;
	}

	while(1){
		if ( addUnderscore )
			name = QString("%1_%2").arg(bn).arg(idx);
		else
			name = QString("%1%2").arg(bn).arg(idx);

		if( isAvail( name ))
			return name;
		idx++;
	}
}	

//! dump the NameMng database via wDebug()
void
NameMng::dump()
{
	NameMngEntry *p = lstNames.first();

	while(p){
		wDebug((" <%s> %p", (const char *)p->name, p->caller));
		p = lstNames.next();
	}
}










