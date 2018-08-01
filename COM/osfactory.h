/***************************************************************************
 */
/*!       \file  osfactory.h
 *       \brief  Instantiation of different target OSes
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:26:51 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: osfactory.h,v $
 * Revision 2.0  2010/01/22 11:26:51  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.2  2005/11/30 15:23:31  dpfeuffer
 * added NT4 (used for APB to suppress NT driver build if notos=NT4)
 *
 * Revision 1.1  2004/01/09 08:20:09  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef OSFACTORY_H
#define OSFACTORY_H

#include <qstring.h>

class TargetOs;

//! target operating system factory
/*!
  This class can be used to create() TargetOs objects by specifying
  their OS name or type.

  It also provides functions to map the OS names to the OsType codes and
  vice versa.
*/  
class OsFactory {
public:
	//! Operating system types
	enum OsType {
		NoOs, Linux, Vxworks, Os9, Win, Qnx, Nt4
	};

	static TargetOs *create( QString osName );
	static TargetOs *create( OsType osType );
	static QString typeToStr( OsType osType );
	static OsType strToType( const QString &osName );
};

#endif 






