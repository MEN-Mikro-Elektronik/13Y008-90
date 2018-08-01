/***************************************************************************
 */
/*!       \file  pkgauto.cpp 
 *       \brief  
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/05/19 16:16:37 $
 *    $Revision: 2.1 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: pkgauto.cpp,v $
 * Revision 2.1  2010/05/19 16:16:37  CRuff
 * R: pkgAuto returns error with no reason
 * M: initialize errMsg with empty string in pkgAuto()
 *
 * Revision 2.0  2010/01/22 11:27:28  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.2  2004/07/21 15:02:48  ufranke
 * changed
 *  - setCurrentCfg() by MAIN_SetCurrentCfgDeleteLast()
 *
 * Revision 1.1  2004/01/09 08:20:23  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#include <qapplication.h>
#include <qdatetime.h>
#include "wizdebug.h"
#include "modelfactory.h"
#include "configuration.h"
#include "targetos.h"
#include "swmodule.h"
#include <stdio.h>

#include "packagefile.h"
#include "descfile.h"

extern TargetOs *G_os;					//!< target OS object

static void usage()
{
	const QString helpText = 
		"Usage: mdiswiz -auto mdisprojdir pkgfile [pkgfile...] "
		"Function: Used to automatically generate MDIS projects for\n"
		"automatic driver build validation\n"
		" mdisprojdir absolute path of MDIS project\n"
		" pkgfile     filename of package XML file without path\n";

	printf( "%s\n", helpText.latin1() );
}

//! PKGAUTO converter
/*
  Usage: mdiswiz -auto pkgdesc mdisprojdir

  Used to automatically generate MDIS projects for automatic driver
  build validation.

  This function takes 
  - reads in an existing MDIS project. 
  - parses the given \a pkgdesc file
  - creates a dummy CPU/BBIS
  - for each model in \a pkgdesc file
    - create two devices: One swapped, one non-swapped
  - checks if all .mak files are present
  - save MDIS project
  
  The existing MDIS project Makefile must not include any driver.mak files.
  The system.dsc file shall only contain the CPU device (arbitrary)
  
*/

int pkgAuto( int argc, char **argv )
{

	int rv=1;

	if( argc < 2 ){
		usage();
		return 1;
	}
	
	QString pkgName;
	QString mdisProjDir(argv[0]);


	try {

		Configuration *cfg = G_os->createConfiguration();
		MAIN_SetCurrentCfgDeleteLast( cfg );
		QString errMsg = "";
		bool ok = FALSE;

		// Try to load existing config
		if( cfg->load( errMsg, mdisProjDir ) == false ){
			fprintf(stderr,"*** Can't load configuration %s: %s\n", 
					mdisProjDir.latin1(), errMsg.latin1() );
			goto ABORT;			
		}
		
		// delete all devices of configuration
		cfg->removeDevice( cfg->getRootDev() );

		// Create Dummy CPU/BBIS devs
		CpuDevice *cpuDev = new CpuDevice( "DUMMY_CPU", "NONE", true);
		BusInterface *busIf = new BusInterface( HwComponent::LocalBus, 
												true );
		BbisDevice *bbDev = new BbisDevice( "DUMMY_BB", "NONE", 
											true, HwComponent::LocalBus);
		bbDev->setInstName("DUMMY_BB");

		ok = cfg->addDevice( 0, cpuDev, errMsg );
		cpuDev->addChild( busIf );
		if( ok )
			ok = cfg->addDevice( busIf, bbDev, errMsg );
		if( !ok ){
			fprintf(stderr,"*** Can't add dummy devices: %s\n", 
					errMsg.latin1() );
			goto ABORT;
		}
		
		for( int ac=1; ac<argc; ac++ ){
			ModelFactory mdisMods;
			pkgName = argv[ac];

			printf("pkgAuto pkg=%s proj=%s\n",  pkgName.latin1(), 
				   mdisProjDir.latin1());

			QString pkgPath = G_os->pkgDbPath() + "/" + pkgName;
			PackageFile pkgFile(pkgPath);

			errMsg = QString::null;



			ok = false;
			// parse XML file
			if( pkgFile.parse( errMsg ) == TRUE ){
				//pkgFile.dump();
				if( pkgFile.toModelFactory( &mdisMods, errMsg ) != 0 )
					ok = TRUE;
			}
			if( !ok ){
				fprintf(stderr,"*** Parsing package file failed: %s\n", 
						errMsg.latin1() );
				goto ABORT;
			}
			else if( errMsg != QString::null ){
				fprintf(stderr,"*** WARNINGS while parsing package file: %s\n",
						errMsg.latin1() );
			}

			Device *modDev;

			for( modDev = mdisMods.first(); modDev; modDev = mdisMods.next()){

				printf(" Model %s\n", modDev->getModelName().latin1() );

				for( int i=0; i<2; i++ ){
					BbisSlot *bbSlot = new BbisSlot( 
						((MdisDevice *)modDev)->getDevIfType(), 0 );

					bbSlot->setEndianSwapping( (i==1) );
					bbDev->addChild( bbSlot );

					// create new device
					// Note: only one is created even for devs with subdevs
					Device *dev = modDev->create(false);

					dev->setInstName( MAIN_GetCurrentCfg()->nameMng.suggest(
										  dev->getDeviceNameTpl()));

					ok = cfg->addDevice( bbSlot, dev, errMsg );
					if( !ok ){
						fprintf(stderr,"*** Can't add device: %s\n", 
								errMsg.latin1() );
						goto ABORT;
					}

				}
			}
		}
		
		// Validate SW modules in configuration
		if( cfg->validateSwModules( errMsg, true, false ) == false ){
			fprintf(stderr,"WARNING: %s\n", 
					errMsg.latin1() );
		}
		errMsg = QString::null;
			
		// save config
		if( cfg->save( errMsg ) == false ){
			fprintf(stderr,"*** Can't save configuration: %s\n", 
					errMsg.latin1() );
			goto ABORT;			
		}
		
		rv = 0;
	}
	catch( WizExcept( &e )){
		fprintf(stderr,"*** Internal exception: %s\n",e.getMsg().latin1());
	}
	catch (...) {
		fprintf(stderr,"*** Unknown Internal exception\n" );
	}
 ABORT:
	return rv;
}

