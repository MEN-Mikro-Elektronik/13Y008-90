/***************************************************************************
 */
/*!       \file  targetos.h
 *       \brief  target operating system abstraction
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:27:54 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: targetos.h,v $
 * Revision 2.0  2010/01/22 11:27:54  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.5  2004/07/14 11:20:21  ufranke
 * added
 *  - helpEnv() environment help description
 *
 * Revision 1.4  2004/01/09 08:20:34  kp
 * intermediate checkin
 *
 * Revision 1.3  2003/06/03 10:16:56  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/05/14 13:46:34  kp
 * + pkgDbPath
 *
 * Revision 1.1  2003/05/02 14:36:33  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef TARGETOS_H
#define TARGETOS_H

#include <qstring.h>
#include "osfactory.h"

class Configuration;
class Device;

//! target operating system class (abstract)
/*!
  It contains interface methods that need to be specifically implemented
  for each target OS.

  The Configuration object holds a reference to the TargetOs object.
*/  
class TargetOs {
public:
	//! creates the TargetOs object 
	/*!
	  \param osType the type of OS
	*/
	TargetOs( OsFactory::OsType osType ) { _osType = osType; }

	//! destructs TargetOs
	virtual ~TargetOs() {}

	//! perform initial, OS-specific init
	/*!
	  This routine should perform the following steps if necessary:
	  - Init the package data base
	  - Bring up any initial dialogs/wizards
	  \return true if ok, false if fatal error (abort mdiswiz)
	*/
	virtual bool startup( int argc, char **argv ) = 0;

	//! Driver Package installer dialog
	virtual bool installDlg() = 0;

	//! Driver Package batch install
	virtual bool batchInstall( QString pkgFile, QString &errMsg  ) = 0;

	//! Get path to package database files
	virtual QString pkgDbPath() = 0;

	//! convert plain driver name to OS-specific full name
	/*!
	  \return converted string or QString::null if n/a
	*/
	virtual QString driverName( QString plainName, Device *dev ) = 0;

	//! get mdiswiz version
	/*!
	  subclasses shall consult MdiswizView::Version() and append their own
	  version.
	  \return string containing mdiswiz version
	*/
	virtual QString mdiswizVersion() = 0;

	//! get mdiswiz environment description
	/*!
	  \return string containing environment description
	*/
	virtual QString helpEnv() = 0;

	//! get path for helpfiles
	/*!
	  \return path of main page of help files
	*/
	virtual QString helpPath() = 0;

	//! create OS specific configuration object
	virtual Configuration *createConfiguration() = 0;

	//! get OS type
	OsFactory::OsType osType() { return _osType; }

private:
	OsFactory::OsType _osType;
};

#endif 






