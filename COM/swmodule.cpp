/***************************************************************************
 */
/*!       \file  swmodule.cpp 
 *       \brief  Implementation of Sw modules
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:27:45 $
 *    $Revision: 2.0 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: swmodule.cpp,v $
 * Revision 2.0  2010/01/22 11:27:45  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.10  2009/05/13 11:54:30  CRuff
 * R: 1. Core lib 'oss' needs to pull in additional Makefile for VxWorks
 * M: 1. change method SwModuleList::findMake(): look for given makefile in all
 *       members of the makefilePath list
 *
 * Revision 1.9  2009/04/27 17:56:21  CRuff
 * R: make kernel modules OSS and DBG configurable
 * M: add handling for new SwModuleType CoreLib
 *
 * Revision 1.8  2006/03/07 10:55:15  DPfeuffer
 * - bug fixed in supportsOs() - has returned false if any OS in excludedOses
 *
 * Revision 1.7  2004/07/21 15:03:32  ufranke
 * cosmetics
 *
 * Revision 1.6  2004/01/09 08:20:30  kp
 * added supportsOs
 *
 * Revision 1.5  2003/06/05 12:01:27  dschmidt
 * intermediate checkin
 *
 * Revision 1.4  2003/06/05 09:33:59  kp
 * intermediate checkin
 *
 * Revision 1.3  2003/06/03 10:16:48  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/05/14 13:46:16  kp
 * fix
 *
 * Revision 1.1  2003/05/02 14:36:28  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include "swmodule.h"
#include "wizdebug.h"

//----------------------------------------------------------------------
//! tell wether SwModule supports OS \a osType
/*!
  \a osType shall be a OsFactory::OsType value
*/
bool 
SwModule::supportsOs( int osType )
{
	// if osType excluded, return false
	if( !excludedOses.isEmpty() && excludedOses.findIndex( osType ) != -1 )
		return false;

	// if osType not included, return false
	if( !includedOses.isEmpty() && includedOses.findIndex( osType ) == -1 )
		return false;

	// if both lists are empty, module supports any OS
	return true;
}

//----------------------------------------------------------------------
SwModuleTypes::SwModuleTypes()
{
	types[Other] 		= SwModuleType( "Other" );
	types[LlDriver]		= SwModuleType( "Low Level Driver", "DRIVERS/MDIS_LL");
	types[BbDriver] 	= SwModuleType( "BBIS Driver", "DRIVERS/BBIS"); 
	types[CbDriver] 	= SwModuleType( "CBIS Driver", "DRIVERS/CBIS"); 
	types[UserLib] 		= SwModuleType( "User Library", "LIBSRC"); 
	types[CoreLib]		= SwModuleType( "Core Library", "LIBSRC");
	types[LlTool] 		= SwModuleType( "Driver Specific Tool", 
										"DRIVERS/MDIS_LL"); 
	types[ComTool] 		= SwModuleType( "Common Tool", "TOOLS"); 
	types[NativeDriver] = SwModuleType( "Native Driver" ); 
	types[NativeLib] 	= SwModuleType( "Native Library" ); 
	types[NativeTool] 	= SwModuleType( "Native Tool" ); 	

	// set default native paths
	types[LlDriver].natPath = types[LlDriver].comPath;
	types[BbDriver].natPath = types[BbDriver].comPath;
	types[CbDriver].natPath = types[CbDriver].comPath;
	types[UserLib].natPath 	= types[UserLib].comPath;
	types[CoreLib].natPath 	= types[CoreLib].comPath;
	types[LlTool].natPath 	= types[LlTool].comPath;
	types[ComTool].natPath 	= types[ComTool].comPath;

}

//! find module type string \a str in available module types
/*!
  \return Iterator if found or end() if not.
*/
SwModuleTypes::TypeMap::Iterator 
SwModuleTypes::findString( const QString &str )
{
	TypeMap::Iterator it;

	for ( it = types.begin(); it != types.end(); ++it ){
		if( it.data().typeName == str )
			break;
	}
	return it;
}

//! find module type code of type string \a str in available module types
/*!
  \return type code or -1 if not found
*/
int
SwModuleTypes::findStringVal( const QString &str )
{
	TypeMap::Iterator it;

	for ( it = types.begin(); it != types.end(); ++it ){
		if( it.data().typeName == str )
			return it.key();
	}
	return -1;
}

//! map common to native path
/*
  loops through all defined SwModuleType objects and checks wether 
  beginning of \a comPath matches to any of the SwModuleType's comPath.

  \param comPath common path relative to root dir. (e.g. DRIVERS/MDIS_LL/M027)
  \returns the native path. If no SwModuleType matched, returns 
    \a comPath unchanged.
*/
QString
SwModuleTypes::mapComToNatPath( const QString &comPath )
{
	TypeMap::Iterator it;
	QString natPath = comPath;

	for ( it = types.begin(); it != types.end(); ++it ){
		if( (it.data().comPath != QString::null) &&
			comPath.startsWith( it.data().comPath )){
			natPath = it.data().comPath + 
				comPath.mid( it.data().comPath.length() );
			wDebug(("SwModuleTypes::mapComToNatPat t=%s cp=%s np=%s",
				   it.data().typeName.latin1(), comPath.latin1(),
				   natPath.latin1()));				   
		}
	}
	
	return natPath;
}

//----------------------------------------------------------------------

//----------------------------------------------------------------------

//! construct an empty SwModuleList
SwModuleList::SwModuleList()
{
	setAutoDelete(true);
}

//! add a SwModule to the list
/*!
  Adds \a mod to list. If \a overwrite is \c true, overwrites an existing
  entry with the same name and type.

  \return true if \a mod was added
*/
bool 
SwModuleList::add( SwModule *mod, bool overwrite )
{
	SwModule *old;

	if( (old = find( mod->modType(), mod->modName())) != 0 ){
		if( overwrite )
			remove( old );
		else
			return false;
	}
	append( mod );
	return true;
}
//! add another list to the list
/*!
  This makes a copy of each element of \a lst and adds it to this object.

  If an entry with the same name and type did already exist, it is replaced
  by the new SwModule.
*/
void 
SwModuleList::add( SwModuleList *lst )
{
	SwModule *mod, *newMod;

	for( mod=lst->first(); mod; mod=lst->next() ){
		newMod = new SwModule(*mod);
		add( newMod );
	}
}


//! find SwModule by type
/*!
  find the fist module in the list that has the same type as
  specified in the parameters.

  \return a pointer to the SwModule within the list if known,
  0 if unknown

*/
SwModule * 
SwModuleList::find( int type )
{
	SwModule *mod;

	for( mod=first(); mod; mod=next()){
		if( mod->modType() == type)
			return mod;
	}
	return 0;
}	

//! find SwModule by name and type
/*!
  find the module in the list that has the same name and type as
  specified in the parameters.

  \return a pointer to the SwModule within the list if known,
  0 if unknown

*/
SwModule * 
SwModuleList::find( int type, const QString &name )
{
	SwModule *mod;

	for( mod=first(); mod; mod=next()){
		if( mod->modName() == name && 
			mod->modType() == type)
			return mod;
	}
	return 0;
}	

//! find SwModule by name and makefilePath
/*!
  find the module in the list that has the same name and makefilePath as
  specified in the parameters.

  \return a pointer to the SwModule within the list if known,
  0 if unknown

*/
SwModule * 
SwModuleList::findMake( int type, const QString &makefilePath )
{
	SwModule *mod;

	wDebug(("SwModuleList::findMake type %d path %s", type, makefilePath.latin1() ));

	for( mod=first(); mod; mod=next())
	{
			wDebug(("SwModuleList::findMake type %d mod path %s path %s", type,
						mod->makefilePath().latin1(),
						makefilePath.latin1() ));
		if( mod->modType() == type && 
			mod->makefilePath_all()->contains(makefilePath) )
		{
			wDebug(("SwModuleList::findMake found"));
			// If internal sw module .mak already exists in makefile then
			// remove internal flag and use as non internal sw module
			if( mod->isInternal() )
			{
				mod->setInternal(false);
			}
			return mod;
		}
	}

	return 0;
}

//! construct an empty ComponentEosEolList
ComponentEosEolList::ComponentEosEolList()
{
	setAutoDelete(true);
};

//! add another component to the list
bool ComponentEosEolList::add(DepracatedComponent *component)
{
	append( component );
	return true;
};

//! find component in the list
DepracatedComponent*
ComponentEosEolList::find( int type, const QString &name )
{
	DepracatedComponent *comp;
	for( comp=first(); comp; comp=next()){
		if( comp->componentName() == name &&
			comp->componentType() == type)
			return comp;
	}
	return 0;
}
