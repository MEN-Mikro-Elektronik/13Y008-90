/***************************************************************************
 */
/*!       \file  pkg2dsc.cpp 
 *       \brief  XML to DSC converter
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:27:23 $
 *    $Revision: 2.0 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: pkg2dsc.cpp,v $
 * Revision 2.0  2010/01/22 11:27:23  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.8  2004/08/31 12:46:51  kp
 * don't attempt to change BBIS instance name if BBIS doesn't allow this
 *
 * Revision 1.7  2004/07/21 15:02:21  ufranke
 * changed
 *  - setCurrentCfg() by MAIN_SetCurrentCfgDeleteLast()
 *
 * Revision 1.6  2004/06/08 17:25:46  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.5  2004/05/11 12:03:46  cs
 * DEBUG_LEVEL(_xxx) are set to inactive (not printed) for *_min.dsc files
 *
 * Revision 1.4  2003/06/06 13:32:27  kp
 * help changed
 *
 * Revision 1.3  2003/06/05 09:33:49  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/06/03 10:16:40  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/14 13:46:44  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#include <qapplication.h>
#include <qdatetime.h>
#include <stdio.h>

#include "wizdebug.h"
#include "modelfactory.h"
#include "configuration.h"
#include "oslinux.h"
#include "packagefile.h"
#include "descfile.h"

// Imports
extern CpuModelFactory *G_cpuMods;		//!< CPU model factory
extern BbisModelFactory *G_bbisMods;	//!< BBIS model factory
extern MdisModelFactory *G_mdisMods;	//!< MDIS model factory

static void usage()
{
	const QString helpText = 
"Usage: mdiswiz -pkg2dsc pkgfile cpu brd model descfile [slot [devname]]\n"
"Function: Convert package XML files to old-style .dsc files\n"
" pkgfile     filename of package XML file without path\n"
" cpu         hardware name of CPU board (consult mdiswiz -dumpmodels)\n"
" brd         hardware name of carrier board (consult mdiswiz -dumpmodels)\n"
" model       hardware name of MDIS device (consult mdiswiz -dumpmodels)\n"
" descfile    filename of created files. mdiswiz will append e.g. \"_max.dsc\"\n"
" slot        slot on carrier board (hex). If missing, use first available slot\n"
" devname     instance name. If missing, use <descfile>_1 (upper case)\n"
"\n"
"Example:\n"
" mdiswiz -pkg2dsc 13m06706.xml A12b A201 M58\n"
" mdiswiz -pkg2dsc 13m06706.xml A12b A12  M58 m58_sw\n";

	printf( "%s\n", helpText.latin1() );
}

//! PKG2DSC converter
int pkg2Dsc( int argc, char **argv )
{
	int rv=0;

	if( argc < 5 ){
		usage();
		return 1;
	}

	QString pkgName(argv[0]);
	QString cpuName(argv[1]);
	QString brdName(argv[2]);
	QString modelName(argv[3]);
	QString descName(argv[4]);
	int slotNum = -1; 
	QString devName(descName.upper() + "_1");

	if( argc >= 6 )
		slotNum = QString(argv[5]).toInt(0,16);
	if( argc >= 7 )
		devName = argv[6];

	printf("pkg=%s cpu=%s brd=%s model=%s desc=%s slot=0x%x devname=%s\n",
		   pkgName.latin1(), 
		   cpuName.latin1(),
		   brdName.latin1(),
		   modelName.latin1(),
		   descName.latin1(),
		   slotNum,
		   devName.latin1()
		);

	try {
		Configuration *cfg = new Configuration;
		MAIN_SetCurrentCfgDeleteLast( cfg );

		QString pkgPath = cfg->targetOs()->pkgDbPath() + "/" + pkgName;
		QString errMsg;
		PackageFile pkgFile(pkgPath);
		bool ok = FALSE;


		// parse XML file
		if( pkgFile.parse( errMsg ) == TRUE ){
			//pkgFile.dump();
			if( pkgFile.toModelFactory( G_mdisMods, errMsg ) != 0 )
				ok = TRUE;
		}
		if( !ok ){
			printf("Parsing package file failed: %s\n", errMsg.latin1() );
			goto ABORT;
		}
		else if( errMsg != QString::null ){
			printf("WARNINGS while parsing package file: %s\n", 
				   errMsg.latin1() );
		}
		
		// Create CPU device
		Device *cpu = G_cpuMods->find( cpuName );
		if( !cpu ){
			printf("Can't find CPU model %s\n", cpuName.latin1() );
			goto ABORT;
		}
		cpu = cpu->create(false);
					
		cfg->addDevice( NULL, cpu, errMsg );

		// Create BBIS device
		Device *_bbDev = G_bbisMods->find( brdName );
		if( !_bbDev ){
			printf("Can't find carrier board %s\n", brdName.latin1() );
			goto ABORT;
		}
		BbisDevice *bbDev = dynamic_cast<BbisDevice *>(_bbDev->create(false));

		// Attach BBIS device to first match interface
		{
			BusInterface *busIf;

			for( busIf=(BusInterface *)cpu->firstChild(); busIf; 
				 busIf=(BusInterface *)cpu->nextChild()){

				if( busIf->canConnect( bbDev) )
					break;
			}
			if( !busIf ){
				printf("Can't find matching busif\n" );
				goto ABORT;
			}

			//printf("Using busif %s\n", busIf->getInstName().latin1());
			if( bbDev->isInstNameChangeAllowed() )
				bbDev->setInstName( bbDev->getDeviceNameTpl() + "_1" );

			// add A201 to busif	
			cfg->addDevice( busIf, bbDev, errMsg );
		}
		

		// find specified model
		Device *modDev;

		for( modDev = G_mdisMods->first(); modDev; modDev = G_mdisMods->next()){
			if( modelName == modDev->getModelName() )
				break;
		}

		if( !modDev ){
			printf("Can't find model %s\n", modelName.latin1() );
			goto ABORT;
		}


		// find slot
		BbisSlot *bbSlot;

		if( slotNum == -1 ){
			for( bbSlot = (BbisSlot *)bbDev->firstChild(); bbSlot;
				 bbSlot = (BbisSlot *)bbDev->nextChild()){
				if( bbSlot->canConnect( (MdisDevice *)modDev ))
					break;
			}
			if( !bbSlot ){
				printf("No matching bbSlot found\n");
				goto ABORT;	
			}				
		}
		else {
			bbSlot = bbDev->bbSlot( slotNum );
			WIZ_ASSERT( bbSlot );
		}

		if( !bbSlot->canConnect( (MdisDevice *)modDev )){
			printf("Can't connect device to bbSlot\n");
			goto ABORT;
		}

		// generate min/max descriptor
		for( int i=0; i<2; i++ ){			
			int subDevNo = 0;
			Device *dev;
			DescriptorEntryDirectory descTmp( "_TMP_" );
			QString descFileName = descName + 
				( i==0 ? "_min" : "_max" ) + ".dsc";
			DescFile descFile(descFileName);

			// Generate file header text
			QString headerText;

			headerText.sprintf("#************************** MDIS4 device descriptor *************************\n"
			                   "#\n# %s: Descriptor for %s\n"
			                   "# Automatically generated by mdiswiz %s from %s\n# %s\n"
			                   "#\n#****************************************************************************\n\n",
					descFileName.latin1(), 
					modDev->getModelName().latin1(),
					cfg->targetOs()->mdiswizVersion().latin1(),
					pkgName.latin1(),
					QDate::currentDate().toString(Qt::ISODate).
					latin1() );
			
			dev = modDev->create(false);

			while( subDevNo < ((MdisDevice *)dev)->numSubDevs() ){
			
				WIZ_ASSERT(dev);

				if( ((MdisDevice *)dev)->numSubDevs() > 1 )
					dev->setInstName( devName + QChar('a'+subDevNo));
				else
					dev->setInstName( devName );

				WIZ_ASSERT( cfg->addDevice( bbSlot, dev, errMsg ));

				dev->createDesc( &descTmp );

				if( ++subDevNo < ((MdisDevice *)dev)->numSubDevs() )
					dev = ((MdisDevice *)dev)->createSubDev( subDevNo );
				else
					break;
			}

			// remove descriptor entries beginning with "_WIZ"
			// activate all descriptor entries for max descr.
			DescriptorEntry *de, *eNext;

			eNext = descTmp.firstChild();

			while( (de=eNext) ){
				if( i==1 ) de->setActive( true );
				else {
					if(strstr(de->getKey(), "DEBUG_LEVEL"))
						de->setActive( false );
				}

				eNext=de->nextSibling();

				if( de->getKey().startsWith("_WIZ" ))
					de->getParent()->removeChild(de);
			}

			descFile.write( &descTmp, headerText );

			cfg->removeDevice( dev );
		}
		
		
		printf("done\n=================================================\n");
	}
	catch( WizExcept( &e )){
	    printf("*** Internal exception: %s\n", e.getMsg().latin1() );
	}
	catch (...) {
	    printf("*** Unknown Internal exception\n" );
	}
 ABORT:
	return rv;
}


