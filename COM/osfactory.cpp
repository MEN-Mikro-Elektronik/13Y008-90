/***************************************************************************
 */
/*!       \file  osfactory.cpp 
 *       \brief  Implementation of OsFactory
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:26:49 $
 *    $Revision: 2.0 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: osfactory.cpp,v $
 * Revision 2.0  2010/01/22 11:26:49  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.4  2008/07/23 11:49:33  gvarlet
 * R:1. No support for QNX
 * M:1. Add support for QNX
 *
 * Revision 1.3  2005/11/30 15:23:08  dpfeuffer
 * added NT4 (used for APB to suppress NT driver build if notos=NT4)
 *
 * Revision 1.2  2004/07/22 12:07:09  kp
 * added OS9
 *
 * Revision 1.1  2004/01/09 08:20:08  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#include "typestr.h"
#include "osfactory.h"
#include "oslinux.h"
#include "osvxworks.h"
#include "osqnx.h"
#include "osos9.h"

//----------------------------------------------------------------------
//! Mapping between Os type codes and OS names
/*!
  \remark the names used here must match the name passed to the 
  TargetOs() constructor by the subclassed OS objects in osxxx.cpp !
*/
class OsNameMap : public TypeStrMap<OsFactory::OsType> {
public:   
	OsNameMap() {
	  mmap[OsFactory::NoOs] 	= "None";
	  mmap[OsFactory::Linux] 	= "Linux";
	  mmap[OsFactory::Vxworks] 	= "VxWorks";
	  mmap[OsFactory::Os9]		= "OS-9";
	  mmap[OsFactory::Win]		= "Windows";
	  mmap[OsFactory::Qnx]		= "QNX";
	  mmap[OsFactory::Nt4]		= "NT4";
	}
};


//----------------------------------------------------------------------

//! create a TargetOs object based on \a osName
/*!
  \return pointer to new TargetOs object or 0 if name unknown
*/
TargetOs *
OsFactory::create( QString osName )
{
	OsNameMap nameMap;

	return create( nameMap.strToKey( osName ) );
}

//! create a TargetOs object based on \a osType
/*!
  \return pointer to new TargetOs object or 0 if \a osType unknown
*/
TargetOs *
OsFactory::create( OsType osType )
{
	TargetOs *os;

	switch( osType ){
	case Linux: 	os = new OsLinux(); break;
	case Vxworks: 	os = new OsVxworks(); break;
	case Os9: 		os = new OsOs9(); break;
	case Qnx:		os = new OsQnx(); break;
	
	//??? more types	
	default: 		os = 0; break;
	}
	return os;
}

//! get name string of OS based on \a osType
/*!
  \returns OS name string or 0 if \a osType unknown
*/
QString 
OsFactory::typeToStr( OsType osType )
{
	OsNameMap nameMap;

	return nameMap.keyToStr( osType );
}

//! get type code of OS based on \a osName
/*!
  \returns ostype code or -1 if unknown
*/
OsFactory::OsType 
OsFactory::strToType( const QString &osName )
{
	OsNameMap nameMap;

	return nameMap.strToKey( osName );
}

