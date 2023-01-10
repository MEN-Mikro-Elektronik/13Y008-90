/***************************************************************************
 */
/*!       \file  main.cpp 
 *       \brief  The root of everything
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2011/09/26 08:42:22 $
 *    $Revision: 2.3 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: main.cpp,v $
 * Revision 2.3  2011/09/26 08:42:22  dpfeuffer
 * R:1. Welcome dialog not focused to mdiswit application window
 *      (may appear at different display)
 *   2. after cancelling during project creation, do not close the application
 *      but show the main window
 * M:1. G_mainWindow updated after XxxMdiswiz() call
 *   2. implemented
 *
 * Revision 2.2  2011/03/31 16:58:25  CRuff
 * R: cosmetics
 * M: removed debug prints and outcommented code
 *
 * Revision 2.1  2010/06/16 12:01:04  CRuff
 * R: not linkable with libc-2.4.so and new fontconfig libs
 * M: added dummy implementation of __isoc99_sscanf (should never be called)
 *
 * Revision 2.0  2010/01/22 11:26:28  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.22  2009/04/28 15:27:25  dpfeuffer
 * R: -pkg2inf option does not support MDIS5
 * M: -pkg2inf5 option for MDIS5 added
 *
 * Revision 1.21  2009/04/01 15:52:16  CRuff
 * R: 1.Chameleon V2 support
 * M: 1.extended function dumpModel(): also support AutoIdChamV2 type
 *
 * Revision 1.20  2008/10/23 18:03:17  channoyer
 * R:1. The Linux host version of MDIS Wizard don't display the application icon
 *      in top left
 *   2. Program name should be "MDIS Wizard" when displayed to the user
 * M:1.a) Include xpm version of the 16x16 mdiswiz.ico
 *     b) Display the MDIS Wizard icon when the application start
 *   2. Create and use a global constant: G_progName
 *
 * Revision 1.19  2008/07/31 16:24:26  gvarlet
 * R: Debug printf was not removed before initial CI
 * M: Debug printf removed
 *
 * Revision 1.18  2008/07/23 11:49:09  gvarlet
 * R:1. No support for QNX
 *   2. helpEnv printing duplicated on command line help
 * M:1. Add support for QNX
 *   2. Remove duplicated output on helpEnv
 *
 * Revision 1.17  2005/07/15 15:46:59  ub
 * cosmetics
 *
 * Revision 1.16  2004/10/07 16:09:27  dpfeuffer
 * G_cfg must be visible for mdiswiz_lib
 *
 * Revision 1.15  2004/07/22 12:05:22  kp
 * added OS9
 *
 * Revision 1.14  2004/07/21 15:04:51  ufranke
 * changed
 *  - hide G_cfg
 *
 * Revision 1.13  2004/07/14 11:22:52  ufranke
 * added
 *  - environment help
 *
 * Revision 1.12  2004/06/08 17:25:25  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.11  2004/05/24 08:50:28  dpfeuffer
 * dumpModel(): case DeviceAutoId::AutoIdCham added
 *
 * Revision 1.10  2004/04/13 15:04:12  dpfeuffer
 * cosmetics
 *
 * Revision 1.9  2004/04/13 11:00:41  dpfeuffer
 * cosmetics
 *
 * Revision 1.8  2004/01/09 08:20:02  kp
 * run for VxWorks target if program name contains "vx"
 * support for -pkgauto -install -xmldump
 *
 * Revision 1.7  2003/06/06 13:32:16  kp
 * added usage
 *
 * Revision 1.6  2003/06/05 12:01:01  dschmidt
 * intermediate checkin
 *
 * Revision 1.5  2003/06/05 09:51:23  dschmidt
 * -pkg2inf option added
 *
 * Revision 1.4  2003/06/05 09:33:30  kp
 * intermediate checkin
 *
 * Revision 1.3  2003/06/03 10:16:12  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/05/14 13:43:52  kp
 * misc
 *
 * Revision 1.1  2003/05/02 14:34:48  dschmidt
 * Initial Revision
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

/*! \mainpage
  
  \section whatis What is MDISWIZ

  This is mdiswiz, the MDIS configuration wizard. This Qt program should
  allow users to interactively create offline MDIS configurations using
  a graphical user interface.

  The user can select a CPU board, carrier boards and MDIS devices. The
  resulting configuration can then be saved to a file (typically called
  \em system.dsc) which will the contain all the MDIS descriptors that had
  to be previously created manualy by the user.

*/

#include <qapplication.h>
#include <stdio.h>
#include <stdlib.h>

#include <QPixmap>

#include "mdiswiz.h"
#include "wizdebug.h"
#include "modelfactory.h"
#include "configuration.h"
#include "osfactory.h"
#include "targetos.h"
#include "vmsgbox.h"
#include "packagedb.h"
#include "images/mdiswiz.xpm"

extern int pkg2Dsc( int argc, char **argv );
extern int pkg2Inf( int argc, char **argv );
extern int pkg2Inf5( int argc, char **argv );
extern int pkgAuto( int argc, char **argv );
extern int xmlDump( int argc, char **argv );
static int doBatchInstall( QString pkgFile );
static void dumpModels();

#ifndef WIN32
extern "C" int __isoc99_sscanf (const char *s, const char *format, ...);
#endif

// Global objects
const QString G_progName = "MDIS Wizard";	//!< main application name
#ifndef WIN_MDISWIZ_LIB
	static CpuModelFactory		_G_cpuMods;		//!< CPU model factory object
	static BbisModelFactory		_G_bbisMods;	//!< BBIS model factory object
	static MdisModelFactory		_G_mdisMods;	//!< MDIS model factory object
	static SwModuleTypes		_G_swModTypes;	//!< SW module types object
#endif /* WIN_MDISWIZ_LIB */

// Global pointers
QApplication		*G_qApp;		//!< main application
QWidget				*G_mainWindow;	//!< main window of mdiswiz
MdiswizView			*G_mdiswiz;		//!< same as G_mainWindow, but MdiswizView type
TargetOs			*G_os;			//!< target OS object

static Configuration *G_Oldcfg;	//!< old configuration

#ifndef WIN_MDISWIZ_LIB
	CpuModelFactory		*G_cpuMods = &_G_cpuMods;
	BbisModelFactory	*G_bbisMods = &_G_bbisMods;
	MdisModelFactory	*G_mdisMods = &_G_mdisMods;
	SwModuleTypes		*G_swModTypes = &_G_swModTypes;

	static Configuration *G_cfg;	//!< current configuration
#else
	CpuModelFactory		*G_cpuMods;		//!< CPU model factory
	BbisModelFactory	*G_bbisMods;	//!< BBIS model factory
	MdisModelFactory	*G_mdisMods;	//!< MDIS model factory
	SwModuleTypes		*G_swModTypes;	//!< SW module types

	Configuration *G_cfg;	//!< current configuration
#endif

bool G_menInternal;				//!< set when MDISWIZ_INTERNAL env variable set                        

//! obtain pointer to currently active configuration (null if none)
Configuration *MAIN_GetCurrentCfg( void )
{
	return G_cfg;
}

//! obtain pointer to old configuration (null if none)
Configuration *MAIN_GetOldCfg( void)
{
	return G_Oldcfg;
}

//! delete old configuration
void MAIN_DeleteOldCfg( void)
{
	if( G_Oldcfg )
		delete G_Oldcfg;
}

//! deletes the old configuration and sets up the new
void MAIN_SetCurrentCfgDeleteLast( Configuration *cfg )
{
	wDebug(("MAIN_SetCurrentCfgDeleteLast() old 0x%08x new 0x%08x", (int)G_cfg, (int)cfg ));

	if( G_cfg )
	{
		// delete old cfg
		delete G_cfg;
	}

	G_cfg = cfg;
	G_cfg->setTargetOs( G_os );	
}

//! store the old configuration and sets up the new
void MAIN_SetCurrentCfgStoreLast( Configuration *cfg )
{
	wDebug(("MAIN_SetCurrentCfgStoreLast() old 0x%08x new 0x%08x", (int)G_cfg, (int)cfg ));

	// save old cfg
	G_Oldcfg = G_cfg;
	G_cfg = cfg;
	G_cfg->setTargetOs( G_os );	
}

#ifndef WIN_MDISWIZ_LIB
static void usage( char *arg0 )
{
	const QString progName = arg0;
	const QString helpText = 
        "Usage:\n" +
        progName + " [<projectdir>]        Normal, GUI mode\n" +
        progName + " -pkg2dsc <args>...    Generate .dsc files\n" +
        progName + " -pkg2inf <args>...    Generate Windows MDIS4 .inf files\n" +
        progName + " -pkg2inf5 <args>...   Generate Windows MDIS5 .inf files\n" +
        progName + " -dumpmodels           Dump all known models and exit\n" +
        progName + " -auto <args>...       Autobuild configuration\n" +
        progName + " -xmldump <args>...    Dump XML file as directory tree\n";

	printf( "%s %s   ", arg0, G_os->mdiswizVersion().latin1() );
	printf( "Qt version %s\n", qVersion() );
	printf( "%s\n", helpText.latin1() );
	printf( "Environment : %s\n", G_os->helpEnv().latin1() );
}

//! where all begins...
int main( int argc, char **argv )
{
	int rv=0;
	wDebug(("main"));
	// check if we're in internal mode (internal Sw modules must be present)
	G_menInternal = (getenv("MDISWIZ_INTERNAL") != NULL);

	if( QString(argv[0]).contains("vx", false) )
		// Target = vxWorks			
		G_os		 = OsFactory::create( OsFactory::Vxworks );
	else if( QString(argv[0]).contains("qnx", false) )
       // Target = QNX
	   G_os		 = OsFactory::create( OsFactory::Qnx ); 
	else if( QString(argv[0]).contains("os9", false) )
		G_os		 = OsFactory::create( OsFactory::Os9 );
	else  {
		// Linux 
		wDebug(("start"));
		// Set default locale in Linux
		putenv("LC_ALL=C");
		G_os		 = OsFactory::create( OsFactory::Linux );
		wDebug(("start2"));
	}
	G_mainWindow = G_mdiswiz;
	if( QString(argv[1]) == "-pkg2dsc" ){
		rv = pkg2Dsc( argc-2, argv+2 );
		goto CLEANUP;
	}
	if( QString(argv[1]) == "-pkg2inf" ){
		rv = pkg2Inf( argc-2, argv+2 );
		goto CLEANUP;
	}
	if( QString(argv[1]) == "-pkg2inf5" ){
		rv = pkg2Inf5( argc-2, argv+2 );
		goto CLEANUP;
	}
	if( QString(argv[1]) == "-dumpmodels" ){
		QApplication a( argc, argv );
		dumpModels();
		goto CLEANUP;
	}
	if( QString(argv[1]) == "-auto" ){
		rv = pkgAuto( argc-2, argv+2 );
		goto CLEANUP;
	}
	if( QString(argv[1]) == "-install" ){
		rv = doBatchInstall( QString(argv[2]) );
		goto CLEANUP;
	}
	if( QString(argv[1]) == "-xmldump" ){
		rv = xmlDump( argc-2, argv+2 );
		goto CLEANUP;
	}
	if( QString(argv[1]).startsWith("-")){
		rv = 1;
		usage( argv[0] );
		goto CLEANUP;
	}
	{
		QApplication a( argc, argv );
		try {
			G_qApp = &a;
		    
			switch(G_os->osType()){
			case OsFactory::Vxworks: 
				G_mdiswiz = new VxworksMdiswiz; break;
			case OsFactory::Os9: 
				G_mdiswiz = new Os9Mdiswiz; break;
			case OsFactory::Qnx:
				G_mdiswiz = new QnxMdiswiz; break;
			default:
				G_mdiswiz = new LinuxMdiswiz; break;
			}
			a.setMainWidget( G_mdiswiz );
			G_mainWindow = G_mdiswiz;
			G_mdiswiz->setIcon(QPixmap( mdiswiz_xpm ));
			G_mdiswiz->show();
			// note: this would close the main window after cancelling during project creation
			//		if( G_os->startup( argc, argv ) == true )
			//			rv = a.exec();
			G_os->startup( argc, argv );
			rv = a.exec();
		}
		
		catch( WizExcept( &e )){
			VMsgBox::criticalOk( G_progName, "Internal exception", 
								 e.getMsg(), G_mdiswiz );
		}
		catch (...) {
			VMsgBox::criticalOk( G_progName, "Unknown Internal exception", 
								 "", G_mdiswiz );
		}
		
	}

CLEANUP:

	return rv;
}

#ifndef WIN32
// added empty method to avoid undefined reference error in glibc
int
__isoc99_sscanf (const char *s, const char *format, ...)
{
    printf("MDIS Wizard: forbidden function called. Functionality might be incomplete.\n");
    return 0;
}
#endif

static int doBatchInstall( QString pkgFile )
{
	QString errMsg;
	if( G_os->batchInstall( pkgFile, errMsg ) == false ){
		fprintf(stderr,"*** Install failed: %s\n", errMsg.latin1() );
		return 1;
	}
	printf("Installation of %s successful.\n", pkgFile.latin1());
	return 0;
}

static void dumpModel( Device *dev )
{
	DeviceAutoId *ai;

	printf(" hw=%s mod=%s ven=%s drv=%s devtpl=%s",
		   dev->getHwName().latin1(),
		   dev->getModelName().latin1(),
		   dev->getVendorName().latin1(),
		   dev->getDriverName().latin1(),
		   dev->getDeviceNameTpl().latin1()
		);

	if( dev->type() == Device::Mdis  )
		printf(" busif=%s", 
			   dev->busIfTypeMap.keyToStr(
				   ((MdisDevice *)dev)->getDevIfType()).latin1());
	
	printf("\n");
			   
	if( (ai = dev->autoId() )){

		switch( ai->getType() ){

		case DeviceAutoId::AutoIdMmod:
			DeviceMmodAutoId *mai;
			WIZ_DYNAMIC_CAST( ai, mai, DeviceMmodAutoId *);
			printf("  M-mod autoid=0x%lx\n", mai->id );
			break;

		case DeviceAutoId::AutoIdPci:
			DevicePciAutoId *pai;
			WIZ_DYNAMIC_CAST( ai, pai, DevicePciAutoId *);
			printf("  PCI autoid=0x%lx 0x%lx 0x%lx 0x%lx\n", 
				   pai->pciVendorId,
				   pai->pciDeviceId,
				   pai->pciSubvenId,
				   pai->pciSubsysId);
				   
			break;

		case DeviceAutoId::AutoIdCham: // fallthrough
		case DeviceAutoId::AutoIdChamV2:
			DeviceChamAutoId *cai;
			WIZ_DYNAMIC_CAST( ai, cai, DeviceChamAutoId *);
			printf("  Chameleon autoid=0x%lx\n", cai->deviceId );
			break;

		default:
			printf("  unknown autoid\n");
		}
	}
}

static void dumpModels()
{
	Device *dev;

	// parse the model data base (ignore errors)
	printf("pkg=%s\n", G_os->pkgDbPath().latin1());
	PackageDb pkgdb( G_os->pkgDbPath() );
	pkgdb.parse( G_mdisMods );

	printf("CPU models:\n");
	for( dev = G_cpuMods->first(); dev; dev = G_cpuMods->next()){
		dumpModel( dev );
	}

	printf("\nBBIS models:\n");
	for( dev = G_bbisMods->first(); dev; dev = G_bbisMods->next()){
		dumpModel( dev );
	}
	
	printf("\nMDIS models:\n");
	for( dev = G_mdisMods->first(); dev; dev = G_mdisMods->next()){
		dumpModel( dev );
	}
	

}
#endif // WIN_MDISWIZ_LIB


