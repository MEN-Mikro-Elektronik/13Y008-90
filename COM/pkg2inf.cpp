/***************************************************************************
 */
/*!       \file  pkg2inf.cpp
 *       \brief  XML to INF converter
 *      \author	 Dieter.Schmidt@men.de
 *		  $Date: 2011/09/26 08:42:39 $
 *    $Revision: 2.6 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: pkg2inf.cpp,v $
 * Revision 2.6  2011/09/26 08:42:39  dpfeuffer
 * R: new bus interfaces required: StdPciE, CpciSer, Pci104
 * M: added StdPciE, CpciSer, Pci104
 *
 * Revision 2.5  2011/01/07 13:06:34  dpfeuffer
 * R: introduction of io mapped chameleon units
 * M: for io mapped chameleon units (modelname=*_IO):
 *    - MDIS5/pkg2inf5: add _IO to CHAM devicetype prefix
 *    - MDIS4/pkg2inf : add _IO to MDISDEV devicetype prefix
 *
 * Revision 2.4  2010/08/25 10:01:38  dpfeuffer
 * R: support to build lltest with APB5
 * M: sw module type Native Driver supported
 *
 * Revision 2.3  2010/05/26 09:53:36  dpfeuffer
 * R:1. SourceDisksNames section improvement from last checkin was buggy
 *   2. MDIS5/pkg2inf5: HWIDs must be changed due to Windows 7 installation
 *      problems
 *   3. MDIS5/pkg2inf5: mcvs date/revision info in inf header dangerous
 *      because inf signature
 * M:1. SourceDisksNames section improvement fixed
 *   2.a) MDIS5/pkg2inf5: ExcludeFromSelect removed
 *     b) MDIS5/pkg2inf5: Models without autoid in xml:
 *        using "XXX[_SW]&NO_AUTOID" instead of XXX[_SW]&ID_FFFFFFFF
 *   3. MDIS5/pkg2inf5: mcvs date/revision info in inf header replaced
 *      with creation date and pkg2inf.cpp revision
 *
 * Revision 2.2  2010/03/22 09:57:03  dpfeuffer
 * R: SourceDisksNames section improvement
 * M: text string now within ""
 *
 * Revision 2.1  2010/01/25 15:06:13  dpfeuffer
 * R: package-desc-files of SMB devices use now new <busif>SMBus</busif>
 * M: support of businterface SMBus implemented for MDIS4 and MDIS5
 *
 * Revision 2.0  2010/01/22 11:27:26  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.29  2009/08/17 09:35:39  dpfeuffer
 * R:1. require configurable KMDF versions
 *   2. inf appearance should be according MDIS5 BBIS inf
 *   3. output not consistent
 * M:1. using envs (MEN_KMDF_VER, MEN_KMDF_COINSTVER) instead of fix values
 *   2. cosmetics in inf generation
 *   3. output revised
 *
 * Revision 1.28  2009/07/29 14:15:33  dpfeuffer
 * R: cosmetics for generated inf files
 * M: cosmetics for generated inf files
 *
 * Revision 1.27  2009/07/29 12:36:13  dpfeuffer
 * R: article number changed from 13MDIS570 to 13MD05-70
 * M: new article number for MDIS5 System Package considered
 *
 * Revision 1.26  2009/07/17 13:38:15  dpfeuffer
 * R:1. -pkg2inf5: generated *_mf.inf doen't specify CatalogFile entry
 *   2. chameleon devices not handled due to new type MdisCham
 * M:1. -pkg2inf5: CatalogFile entry added
 *   2. chameleon devices of type MdisCham considered
 *
 * Revision 1.25  2009/04/30 11:09:29  CRuff
 * R: not compilable under Linux
 * M: added loop variable declaration in pkg2Inf()
 *
 * Revision 1.24  2009/04/30 10:30:46  dpfeuffer
 * R: header direct.h for mkdir() not available under linux
 * M: mkdir() replaced by QDir().mkdir()
 *
 * Revision 1.23  2009/04/28 15:46:48  dpfeuffer
 * R: -pkg2inf option does not support MDIS5
 * M: -pkg2inf5 option for MDIS5 implemented
 *
 * Revision 1.22  2009/04/07 11:42:25  dpfeuffer
 * R: 1. "_SW" was missing in "MDISDEV_<dev-hwname>[_SW]&ID_<dev-id>"
 *    2. no note to MDIS4 in inf header
 *    3. chameleon devs: V0 mod-code matches no longer to devId from chameleon BBIS
 * M: 1. createMdisdevId(): add "_SW" if swapped is set
 *    2. writeInfFileHeader(): MDIS4 string added
 *    3. writeInfDevList4Dev(): use AutoIdChamV2 or none id for chameleon devs
 *
 * Revision 1.21  2009/04/01 15:44:41  CRuff
 * R: 1.Chameleon V2 support
 * M: 1.extended function writeInfDevList4Dev: handle new DeviceAutoId type
 *      DeviceAutoIdChamV2
 *
 * Revision 1.20  2008/05/02 14:05:35  DPfeuffer
 * - supports PCI devices without auto-id
 *
 * Revision 1.19  2007/07/26 11:19:22  DPfeuffer
 * - new inf entry DriverPackageDisplayName added
 * - pkg2inf revision information added
 *
 * Revision 1.18  2007/07/24 16:20:12  DPfeuffer
 * - for PCI devices: generates now extra model entry with "PCI\VEN_<ven-id>..."
 *   (changed for target-installer inf-drv-pkg mode: uninstallation)
 *
 * Revision 1.17  2007/02/23 12:20:30  DPfeuffer
 * added:
 * - MDISDEV_<dev-modelname>[_SW] / MDISDEV_<dev-hwname>_<dev-hwname>[_SW]
 * - PCI_<dev-hwname>_<dev-hwname>
 *
 * Revision 1.16  2006/11/30 11:27:55  cs
 * - supports now BusIfType Xmc
 *
 * Revision 1.15  2006/03/07 10:55:13  DPfeuffer
 * - supports now any <busif>, implemented for ChameleonBus first used in 13z05106.xml
 *
 * Revision 1.14  2006/01/09 14:29:59  dpfeuffer
 * - device ID generation for PCI devices changed:
 * added [PCI_<dev-modelname>,]PCI_<dev-hwname> for PCI devices serviced by PCI BBIS
 * - control flag 'ExcludeFromSelect' added to hide devices from Add Hardware Wizard
 * (but it will be displayed by the Found New Hardware and Hardware Update wizards)
 *
 * Revision 1.13  2005/07/19 13:32:45  dpfeuffer
 * device ID generation for MDISDEV devices changed (generates no longer
 * blank "MDISDEV" because 13z00570.inf was matched for D203_TRIG devices)
 *
 * Revision 1.12  2004/10/07 11:13:29  dpfeuffer
 * - bugfix: internal attribute and notos tag was not fully considered
 * - *_mf.inf generation for subdevices implemented
 *
 * Revision 1.11  2004/07/14 11:19:20  ufranke
 * cosmetics
 *
 * Revision 1.10  2004/06/21 08:45:35  dpfeuffer
 * device ID generation changed
 *
 * Revision 1.9  2004/06/02 14:24:03  dpfeuffer
 * windows.h removed, type changed
 *
 * Revision 1.8  2004/05/28 14:32:38  dpfeuffer
 * cosmetics
 *
 * Revision 1.7  2004/05/28 14:25:21  dpfeuffer
 * [ControlFlags] section and "non-default descriptor entries" added
 *
 * Revision 1.6  2004/05/24 08:56:03  dpfeuffer
 * some changes
 *
 * Revision 1.5  2004/04/13 11:00:46  dpfeuffer
 * some changes
 *
 * Revision 1.4  2004/03/12 11:52:32  dpfeuffer
 * intermediate checkin
 *
 * Revision 1.2  2003/06/13 09:01:58  dschmidt
 * intermediate checkin
 *
 * Revision 1.1  2003/06/06 11:46:27  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#include <qapplication.h>
#include <qdatetime.h>
#include <qdir.h>
#include <stdlib.h>
#include <stdio.h>

#include "modelfactory.h"
#include "configuration.h"
#include "oslinux.h"
#include "packagefile.h"
#include "descfile.h"
#include "swmodule.h"
#include "devgenericprop.h"

// Enums
enum DevType {
	mdisdev,
	cham,
	smb
};

// Imports
extern const QString mdiswizVersion;

// Globals
static FILE	*G_fp = NULL;		// for inf
static FILE	*G_mfFp = NULL;		// for mf-inf
static QString G_mfInfPath;		// for mf-inf
static QString G_natPkgName;	// e.g. 13m03470
static QString G_xmlFileName;	// e.g. 13m03406.xml
static QString G_infRev;		// e.g. 2.1.0.0
static 	bool G_verb = FALSE;
static ModelFactory G_mods;		// empty model factory
								// MdisModelFactory G_mdisMods cannot be used
								// because it is not empty!
static bool G_mdis5 = FALSE;
static bool G_mdis5_amd64 = FALSE;
static QString G_mdis5_platform;
static char *G_mdis5_kmdfVer;			// MEN_KMDF_VER  (e.g. 1.7)
static char *G_mdis5_kmdfCoinstVer;		// MEN_KMDF_COINSTVER (e.g. 01007)
static char* G_pltfExt;

// Prototypes
static int writeInfFileHeader( FILE	*fp, bool mf );
static int writeInfVersionSect( PackageFile &pkgFile );
static int writeInfSrcSect( PackageFile &pkgFile );
static int writeInfDestSect( void );
static int writeInfClassInstSect( void );
static void writeInfDevListHeader( FILE *fp );
static int writeInfDevList( PackageFile &pkgFile );
static int writeInfDevList4Dev(
	Device		*modDev,
	int			nbrOfSubDevs,
	int			subDevNo,
	bool		swapped,
	QString		&modDevIds );
static void createDevtypeId( Device	*modDev, bool swapped, ulong id, QString &devId, enum DevType devType );
static int createChamAutoId( Device	*modDev, bool swapped, ulong id, QString &devId );
static QString getDevTypePrefix( Device	*modDev, enum DevType devType );
static int writeInfDevSect( void );
static int writeInfDevSect4Dev(
	Device						*modDev,
	int							nbrOfSubDevs,
	int							subDevNo,
	DescriptorEntryDirectory	*descEntryDir,
	bool						swapped );
static int writeInfNonDefDesc(
	QString						dir,
	DescriptorEntryDirectory	*descEntryDir );
static int writeInfCommonSect( void );
static int writeInfStringSect( void );
static bool swMod4WinUser( SwModule *swMod );
static int model4WinUser( Device *modDev, bool *forWin );
static int llDrvSupportsSwapping( Device *modDev, bool *swapp );
static int openMfInf( PackageFile &pkgFile );
static int writeMfInfDevSect4Dev(
	Device	*modDev,
	int		nbrOfSubDevs,
	bool	swapped );
static int closeMfInf( void );
static int writeInfDevList4Subdev(
	Device		*modDev,
	int			nbrOfSubDevs,	// 1=no subdevs
	int			subDevNo,		// zero based subdev index
	bool		swapped);

static void usage()
{
	const QString helpText =
"Usage: mdiswiz -pkg2inf <xml-path> <inf-name> <inf-rev> [-dir <inf-dir>][-v]\n"
"Function: Convert MEN package files (.xml) to MDIS4 Windows installation files (.inf).\n"
" <xml-path>      path to package XML file\n"
" <inf-name>      INF file name\n"
" <inf-rev>       INF revision (4-digit native fileset revision, e.g. 2.1.0.0)\n"
" -dir <inf-dir>  output directory for INF files\n"
" -v              verbose (prints extended information)\n"
"\n"
"Examples:\n"
" mdiswiz -pkg2inf 13m03406.xml 13m03470.inf 2.1.0.0 -v\n"
" mdiswiz -pkg2inf D:/WORK/NT/PACKAGE_DESC/COM/13m03406.xml 13m03470.inf 2.1.0.0 -dir D:/WORK/W2K/PACKAGE_DESC/NATIVE\n"
"\n"
"pkg2inf $Revision: 2.6 $\n";
	printf( "%s\n", helpText.latin1() );
}

static void usage_mdis5()
{
	const QString helpText =
"Usage: mdiswiz -pkg2inf5 <xml-path> <inf-name> <inf-rev> [-dir <inf-dir>][-v]\n"
"Function: Convert MEN package files (.xml) to MDIS5 Windows installation files (.inf).\n"
"          Different .inf files for the I386 and AMD64 platform will be created\n"
"          in the sub-dirs \"I386\" and \"AMD64\".\n"
" <xml-path>      path to package XML file\n"
" <inf-name>      INF file name\n"
" <inf-rev>       INF revision (4-digit native fileset revision, e.g. 2.1.0.0)\n"
" -dir <inf-dir>  output directory for INF files (without platform sub-dir I368/AMD)\n"
" -v              verbose (prints extended information)\n"
"\n"
"Examples:\n"
" mdiswiz -pkg2inf5 13m03406.xml 13m03470.inf 2.1.0.0 -v\n"
" mdiswiz -pkg2inf5 D:/WORK/WINDOWS/PACKAGE_DESC/COM/13m03406.xml 13m03470.inf 2.1.0.0 -dir D:/WORK/WINDOWS/PACKAGE_DESC/NATIVE\n"
"\n"
"pkg2inf $Revision: 2.6 $\n";
	printf( "%s\n", helpText.latin1() );
}

//! PKG2INF converter
int pkg2Inf( int argc, char **argv )
{
	int			err=1, optArgs=0;
	int			mandatoryArgs;
	QString		infPath;

	if( G_mdis5 )
		mandatoryArgs = 3;
	else
		mandatoryArgs = 3;

	//
	// get arguments
	//
	if( argc < mandatoryArgs ){
		G_mdis5 ? usage_mdis5() : usage();
		return 1;
	}

	QString xmlPath( argv[0] ); // absolute or relative file path (directory + name)
	QString infName( argv[1] );
	G_infRev = QString( argv[2] );

	// verify rev format
	if( G_infRev.count(".") != 3 ){
		printf("*** invalid inf-rev format: %s\n", G_infRev.latin1() );
		return 1;
	}

	QFileInfo xmlFileInfo = QFileInfo( xmlPath );
	QString xmlDirPath = xmlFileInfo.dirPath( TRUE );	// abs dir path (e.g. D:\WORK\NT\PACKAGE_DESC\COM)
	G_xmlFileName = xmlFileInfo.fileName();				// file name (e.g. 13m03406.xml)
	xmlPath = xmlFileInfo.absFilePath();				// abs file path (e.g. D:\WORK\NT\PACKAGE_DESC\COM\ 13m03406.xml)

	QString infDirPath = xmlDirPath;

	// optional params?
	for( int i=mandatoryArgs; i<argc; i++ ){

		QString arg( argv[i] );
		QString subPath;

		// inf directory
		if( arg == "-dir" ){
			if( i+1 > argc ){
				printf("*** <infdir> not specified\n" );
				goto CLEANUP;
			}
			infDirPath = QString( argv[i+1] );
			optArgs+=2;

			// create specified inf directory if necessary
			int index = 2; // jump over e.g. "D:"
			do{
				index = infDirPath.find( '/', index+1 );

				if( index != -1 ) // found
					subPath = infDirPath.left( index );
				else // last
					subPath = infDirPath;

				// for debug: printf("index=%d, %s\n", index, subPath.latin1());
				// sub-path not exists?
				if( !QDir().exists( subPath ) ){
					// create sub-dir
					if( !QDir().mkdir( subPath, TRUE ) ){
						printf("*** cannot create directory: %s\n", subPath.latin1() );
						goto CLEANUP;
					}
				}
			}while(index != -1 );
		}

		// verbose
		if( arg == "-v" ){
			G_verb = TRUE;
			optArgs++;
		}
	}

	if( argc < mandatoryArgs+optArgs ){
		usage();
		return 1;
	}

	printf("=== MDIS%d XML-to-INF Converter ===\n", G_mdis5 ? 5 : 4 );

	// MDIS5: switch to AMD64, add and create platform sub-dir
	if( G_mdis5 ){
		// get environment variable MEN_KMDF_VER
		G_mdis5_kmdfVer = getenv( "MEN_KMDF_VER" );
		if( !G_mdis5_kmdfVer ){
			printf("*** environment variable MEN_KMDF_VER missing\n" );
			return 1;
		}

		// get environment variable MEN_KMDF_COINSTVER
		G_mdis5_kmdfCoinstVer = getenv( "MEN_KMDF_COINSTVER" );
		if( !G_mdis5_kmdfCoinstVer ){
			printf("*** environment variable MEN_KMDF_COINSTVER missing\n" );
			return 1;
		}

		printf("using : MEN_KMDF_VER=%s, MEN_KMDF_COINSTVER=%s\n",
			G_mdis5_kmdfVer, G_mdis5_kmdfCoinstVer);
	}

	printf("input : XML-file %s\n", xmlPath.latin1() );

	try {
		QString errMsg;
		PackageFile pkgFile( xmlPath );
		QString modelName;

		// parse XML file
		if( pkgFile.parse( errMsg ) == FALSE ){
			printf("*** parsing package file failed: %s\n", errMsg.latin1() );
			goto CLEANUP;
		}

		// add it to model factory
		if( pkgFile.toModelFactory( &G_mods, errMsg ) == FALSE ){
			printf("*** adding package file to ModelFactory failed: %s\n", errMsg.latin1() );
			goto CLEANUP;
		}
		if( errMsg != QString::null ){
			printf("WARNINGS while parsing package file: %s\n", errMsg.latin1() );
		}

		// create native package name
		QFileInfo infFileInfo = QFileInfo( infName );
		G_natPkgName = infFileInfo.baseName();	// e.g. 13m03470

		// MDIS5: loop to create inf(s) for AMD64/I386
		int i_end = G_mdis5 ? 2 : 1;

		QString orgInfDirPath = infDirPath;

		for( int i=0; i<i_end; i++ ){

			G_pltfExt = "NTx86"; // MDIS4 and MDIS5 I386

			// MDIS5: switch to AMD64, add and create platform sub-dir
			if( G_mdis5 ){
				G_mdis5_amd64 = i ?  TRUE : FALSE;
				if( G_mdis5_amd64 ){
					G_mdis5_platform = "AMD64";
					G_pltfExt = "NTamd64"; // MDIS5 AMD64
				}
				else{
					G_mdis5_platform = "I386";
				}
				G_mdis5_platform = G_mdis5_amd64 ? "AMD64" : "I386";
				if( G_verb )
					printf("=== MDIS5: create inf(s) for %s ===\n", G_mdis5_platform.latin1());
				infDirPath = orgInfDirPath + "/" + G_mdis5_platform;
				QDir().mkdir( infDirPath, TRUE );
			}

			// create inf-file path
			infPath = infDirPath + "/" + infName;

			//
			// create mf-inf file path
			//
			G_mfInfPath = infDirPath + "/" + G_natPkgName + "_mf.inf";

			//
			// open inf-file for writing
			//
			if( (G_fp = fopen( infPath.latin1(), "w" )) == NULL ){
				printf("*** open file %s failed\n", infPath.latin1() );
				goto CLEANUP;
			}
			printf("output: INF-file %s\n", infPath.latin1() );

			//
			// write inf-file
			//
			if( writeInfFileHeader( G_fp, FALSE ) )
				goto CLEANUP;
			if( writeInfVersionSect( pkgFile ) )
				goto CLEANUP;
			if( writeInfSrcSect( pkgFile ) )
				goto CLEANUP;
			if( writeInfDestSect() )
				goto CLEANUP;
			if( writeInfClassInstSect() )
				goto CLEANUP;
			if( writeInfDevList( pkgFile ) )	// calls openMfInf
				goto CLEANUP;
			if( writeInfDevSect() )				// calls closeMfInf
				goto CLEANUP;
			if( writeInfCommonSect() )
				goto CLEANUP;
			if( writeInfStringSect() )
				goto CLEANUP;
		} // for

		if( G_verb )
			printf("============done============\n");
	}
	catch( WizExcept( &e )){
	    printf("*** internal exception: %s\n", e.getMsg().latin1() );
	}
	catch (...) {
	    printf("*** unknown Internal exception\n" );
	}

	// success
	err = 0;

 CLEANUP:

	// close inf-file
	if( G_fp )
		fclose( G_fp );

	// error?
	if( err ){

		// inf-file was opened?
		if( G_fp ){
			// remove inf-file
			if( !remove( infPath.latin1() ) )
				printf("INF-file %s removed due to error\n", infPath.latin1() );
		}

		if( G_mfFp ){
			// close mf inf-file
			closeMfInf();

			// remove mf inf-file
			if( !remove( G_mfInfPath.latin1() ) )
				printf("INF-file %s removed due to error\n", G_mfInfPath.latin1() );
		}
	}

	return err;
}

//! PKG2INF5 converter
int pkg2Inf5( int argc, char **argv )
{
	G_mdis5 = TRUE;
	return pkg2Inf( argc, argv );
}

//! write inf/mf-inf file-header
static int writeInfFileHeader( FILE *fp, bool mf )
{
	// MDIS5
	if( G_mdis5 ){
		fprintf( fp,
			"; identifier: MEN_INF_MDIS5\n"
			";\n"
			"; MDIS5 %s",
			G_mdis5_platform.latin1() );
	}
	// MDIS4
	else{
		fprintf( fp,
			"; identifier: MEN_INF_W2K\n"
			";\n"
			"; MDIS4" );
	}

	fprintf( fp,
		" %sINF file for %s\n"
		";\n"
		";    Author: Generated by mdiswiz V%s from %s\n"
		,
		mf ? "MF-" : "",
		G_natPkgName.latin1(),
		mdiswizVersion.latin1(),
		G_xmlFileName.latin1() );

	// MDIS5
	if( G_mdis5 ){
		static char revStr[] = "$Revision: 2.6 $";
		size_t revStrLen = strlen( revStr );
		char revNbrStr[] = "\0\0\0\0\0";
		strncpy( revNbrStr, revStr+11, revStrLen-13 );
		fprintf( fp,
			";      Date: %s (pkg2inf.cpp rev. %s)\n",
			QDate::currentDate().toString(Qt::ISODate).latin1(),
			revNbrStr );
	}
	// MDIS4
	else{
		fprintf( fp,
			";     $Date: 2011/09/26 08:42:39 $\n"
			"; $Revision: 2.6 $\n" );
	}

	fprintf( fp,
		";\n"
		";    > > > DO NOT EDIT THIS FILE! < < <\n"
		";\n"
		"; (c) Copyright %s by MEN Mikro Elektronik GmbH, Nuremberg, Germany\n"
		";\n"
		"\n"
		,
		QDate::currentDate().toString("yyyy").latin1() );

	return 0;
}

//! write inf version-section
static int writeInfVersionSect( PackageFile &pkgFile )
{
	fprintf( G_fp,
		"[Version]\n"
		"Signature=\"$Windows NT$\"\n");

	// MDIS5
	if( G_mdis5 )
		fprintf( G_fp,
			"Class=\"MDIS5 devices\"\n"
			"ClassGUID={C82FF067-7D62-4801-B5FC-D01C6F2E7696}\n");
	// MDIS4
	else
		fprintf( G_fp,
			"Class=\"MDIS devices\"\n"
			"ClassGUID={149F64DD-F1B6-4900-9D50-55F4AA284B24}\n");

	fprintf( G_fp,
		"Provider=%%MEN%%\n"
		"DriverPackageDisplayName=\"%s\"\n"
		"DriverVer=%s,%s\n"
		,
		pkgFile.pkgTag("description").latin1(),
		QDate::currentDate().toString("MM/dd/yyyy").latin1(), G_infRev.latin1()
		);

	// MDIS5
	if( G_mdis5 )
		fprintf( G_fp,
			"CatalogFile=%s.cat\n",
			G_natPkgName.latin1()
			);

	fprintf( G_fp,
		"\n");

	return 0;
}

//! write inf source-section
static int writeInfSrcSect( PackageFile &pkgFile )
{
	Device			*modDev;
	SwModuleList	*swModList;
	SwModule		*swMod;
	QStringList		drvObjList;
	QString			modName;

	if( G_verb )
		printf("write inf source-section..\n");

	// MDIS5
	if( G_mdis5 )
		fprintf( G_fp,
			"[SourceDisksNames]\n"
			"1=\"MEN %s - %s\"\n"
			"2=\"MEN 13MD05-70 - MDIS5 System Package for Windows\"\n"
			"\n"
			"[SourceDisksFiles]\n"
			"WdfCoInstaller%s.dll=2\n"
			"%s=1\n"
			,
			G_natPkgName.latin1(),
			pkgFile.pkgTag("description").latin1(),
			G_mdis5_kmdfCoinstVer,
			G_xmlFileName.latin1()
			);

	// MDIS4
	else
		fprintf( G_fp,
			"[SourceDisksNames.x86]\n"
			"1=\"MEN %s - %s\"\n"
			"2=\"MEN 13m00006 - MDIS4 System Package for Windows\"\n"
			"\n"
			"[SourceDisksFiles.x86]\n"
			"men_qt-mt.dll=2\n"
			"men_mdis_clinst.dll=2\n"
			"men_evlg.dll=2\n"
			"%s=1\n"
			,
			G_natPkgName.latin1(),
			pkgFile.pkgTag("description").latin1(),
			G_xmlFileName.latin1()
			);

		// no model in G_mods?
		if( !G_mods.first() ){
			printf("*** no model found\n");
			return -1;
		}

		// for all models
		for( modDev = G_mods.first();
			 modDev;
			 modDev = G_mods.next() ){

			// enumerate sw-module list
			swModList = modDev->enumSwModules();

			// get ll-drv (or native driver e.g. for lltest)
			if( !(swMod = swModList->find( SwModuleTypes::LlDriver )) )
				swMod = swModList->find( SwModuleTypes::NativeDriver );
			
			if( swMod ){
				// sw-module not for us?
				if( !swMod4WinUser( swMod ) )
					continue;

				modName = swMod->modName();

				if( G_verb )
					printf("- drv: %s\n", modName.latin1() );

				// drv-obj not in drvObjList?
				// Note: This is necessary to avoid duplicate drv-obj if
				//       a ll-drv is specified in the global <swmodulelist> for
				//       several <model> entries of the xml-file.
				if( drvObjList.find(modName) == drvObjList.end() ){

					drvObjList.append(modName);
					if( G_verb )
						printf("- drv: %s added to list\n", modName.latin1() );

					// swapped variant supported?
					if( swMod->isSwapCapable() ){
						modName.append("_sw");
						drvObjList.append(modName);
						if( G_verb )
							printf("- drv: %s added to list\n", modName.latin1() );
					}
				}

			}
		}// for

		// for all drv-obj
		for ( QStringList::Iterator it = drvObjList.begin();
			  it != drvObjList.end();
			  ++it ) {

			fprintf( G_fp, "men_%s.sys=1\n", (*it).latin1() );
		}

	return 0;
}

//! write inf destination-section
static int writeInfDestSect( void )
{
	fprintf( G_fp,
		"\n"
		"[DestinationDirs]\n"
		"DefaultDestDir=12   ; Drivers directory\n"
		"Xml_Files=17,MEN    ; INF file directory\\MEN (for XML)\n"
		);


	// MDIS5
	if( G_mdis5 )
		fprintf( G_fp,
			"WdfCoInst_Files=11  ; System directory\n"
			"\n"
			);
	// MDIS4
	else
		fprintf( G_fp,
			"ClassInst_Files=11  ; System directory\n"
			"\n"
			);

	return 0;
}

//! write inf class-installation-section
static int writeInfClassInstSect( void )
{
	fprintf( G_fp,
		";;\n"
		";; Class installation section\n"
		";;\n"
		"[ClassInstall32.%s]\n"
		"AddReg=AddClassReg\n",
		G_pltfExt
		);

	// MDIS4
	if( !G_mdis5 )
		fprintf( G_fp,
			"CopyFiles=ClassInst_Files\n"
			);

	fprintf( G_fp,
		"\n"
		"[AddClassReg]\n"
		"HKR,,,,\"MDIS%s devices\"\n"
		"HKR,,Icon,,100\n"
		"HKR,,Installer32,,\"men_mdis_clinst.dll,MdisClassInstaller\"\n"
		"\n",
		G_mdis5 ? "5" : ""
		);

	// MDIS4
	if( !G_mdis5 )
		fprintf( G_fp,
		"[ClassInst_Files]\n"
		"men_qt-mt.dll,,,%%COPYFLG%%\n"
		"men_mdis_clinst.dll,,,%%COPYFLG%%\n"
		"men_evlg.dll,,,%%COPYFLG%%\n"
		"\n"
		);
	return 0;
}

//! write inf/mf-inf device-list header
static void writeInfDevListHeader( FILE *fp )
{
	fprintf( fp,
		";;\n"
		";; Device list\n"
		";;\n"
		"[Manufacturer]\n"
		"%%MEN%%=MEN");

	// MDIS5
	if( G_mdis5 )
		fprintf( fp, ",%s\n", G_pltfExt );
	// MDIS4
	else
		fprintf( fp, "\n"	);

	fprintf( fp, "[MEN" );

	// MDIS5
	if( G_mdis5 )
		fprintf( fp, ".%s]\n", G_pltfExt );
	// MDIS4
	else
		fprintf( fp, "]\n" );
}


//! write inf device-list
//  Note: - if no subdevs: writes to inf only
//        - if subdevs: writes to inf and mf-inf
static int writeInfDevList( PackageFile &pkgFile )
{
	Device			*modDev;
	bool			swap, forWin;
	QStringList		devIdList;
	QStringList		subdevIdList;
	QStringList		_idList, _idSubList;
	bool			found;
	QString			modDevIds;

	if( G_verb )
		printf("write inf device-list..\n");

	writeInfDevListHeader( G_fp );

	// for all models
	for( modDev = G_mods.first();
		 modDev;
		 modDev = G_mods.next() ){

		modDevIds = "";

		// model not for us?
		if( model4WinUser( modDev, &forWin ) )
			return -1;
		if( !forWin )
			continue;

		// swapped variant supported?
		if( llDrvSupportsSwapping( modDev, &swap ) )
			return -1;

		// SUB-DEV
		int subDevNo = 0;
		DescriptorEntryDirectory descTmp( "_TMP_" );
		Device *dev = modDev->create(false);
		int nbrOfSubDevs = ((MdisDevice *)dev)->numSubDevs();	// 1=no subdevs

		// subdev?
		if( nbrOfSubDevs > 1){
			// first subdev => open mf-inf
			if( !G_mfFp )
				openMfInf( pkgFile );

			if( writeInfDevList4Subdev( modDev, nbrOfSubDevs, subDevNo, FALSE ) )
				return -1;
			if( swap )
				if( writeInfDevList4Subdev( modDev, nbrOfSubDevs, subDevNo, TRUE ) )
					return -1;
		}

		if( writeInfDevList4Dev( modDev, nbrOfSubDevs, subDevNo, FALSE, modDevIds ) )
			return -1;
		if( swap )
			if( writeInfDevList4Dev( modDev, nbrOfSubDevs, subDevNo, TRUE, modDevIds ) )
				return -1;

		_idSubList = QStringList::split( ",", modDevIds );

		// subdev?
		if( nbrOfSubDevs > 1)
			_idList = subdevIdList;
		else
			_idList = devIdList;

		// each id in _idSubList
		for( QStringList::Iterator subListIt = _idSubList.begin();
			 subListIt != _idSubList.end();
			 ++subListIt ) {

			if( G_verb )
				 printf(": subListIt=%s\n", QString(*subListIt).latin1());

			found = FALSE;	// id from _idSubList not in _idList

			// each id in _idList
			for( QStringList::Iterator listIt = _idList.begin();
				 listIt != _idList.end();
				 ++listIt ) {

				if( G_verb )
					 printf(": listIt=%s\n", QString(*listIt).latin1());

				// id from _idSubList already in _idList?
				if( QString( *subListIt ) == QString( *listIt ) ){
					found = TRUE;
					break;
				}
			} // for

			// append id from _idSubList to _idList
			if( !found ){
				_idList += QString( *subListIt );
			}

		} // for

		// subdev?
		if( nbrOfSubDevs > 1)
			subdevIdList = _idList;
		else
			devIdList = _idList;

	} // for

	QString allDevIds = devIdList.join ( "," );
	QString allSubdevIds = subdevIdList.join ( "," );

	if( G_verb ){
		printf("allDevIds = %s\n", allDevIds.latin1());
		printf("allSubdevIds = %s\n", allSubdevIds.latin1());
	}

	//
	// [ControlFlags] note:
	//
	// InteractiveInstall=<hwids>
	// Required if more than one model section match to a HWID.
	// Always added here.
	// If not specified, no model list would be displayed and
	// the first listed model would be installed.
	//
	// ExcludeFromSelect=*
	// Hide all models from the Add Hardware Wizard (but it will be
	// displayed by the Found New Hardware and Hardware Update wizards).
	//
	if( !allDevIds.isEmpty() ){

		// MDIS5
		if( G_mdis5 ){
			fprintf( G_fp,
				"\n"
				"[ControlFlags]\n"
				"ExcludeFromSelect=*\n");
		}
		// MDIS4
		else {
			fprintf( G_fp,
				"\n"
				"[ControlFlags]\n"
				"InteractiveInstall=%s\n"
				"ExcludeFromSelect=*\n"
				,allDevIds.latin1() );
		}
	}

	if( !allSubdevIds.isEmpty() ){

		// MDIS5
		if( G_mdis5 ){
			fprintf( G_mfFp,
				"\n"
				"[ControlFlags]\n"
				"ExcludeFromSelect=*\n");
		}
		// MDIS4
		else {
			fprintf( G_mfFp,
				"\n"
				"[ControlFlags]\n"
				"InteractiveInstall=%s\n"
				"ExcludeFromSelect=*\n"
				,allSubdevIds.latin1() );
		}
	}

	return 0;
}

//! write inf device-list for device
//  Note: - if no subdevs: writes to inf
//        - if subdevs: writes to mf-inf
static int writeInfDevList4Dev(
	Device		*modDev,
	int			nbrOfSubDevs,	// 1=no subdevs
	int			subDevNo,		// zero based subdev index
	bool		swapped,
	QString		&modDevIds )
{
	QString devId;
	QString pciId = "";
	FILE	*fp = G_fp;

	UTIL_UNREF_PARAM( subDevNo );

	// model name
	QString	qModName = modDev->getModelName();

	// subdev -> switch to mf-inf
	if( nbrOfSubDevs > 1 )
		fp = G_mfFp;

	if( swapped )
		qModName += "_SW";

	if( G_verb )
		printf("- model: %s ", qModName.latin1() );


	switch( modDev->type() ){
		case Device::Mdis:
			if( G_verb )
				printf("(MDIS device)\n");
			break;
		case Device::MdisCham:
			if( G_verb )
				printf("(Chameleon device)\n");
			break;
		default:
			if( G_verb )
				printf("skipping - no MDIS device!\n");
			return 0;
	}

	MdisDevice* modDevMdis = (MdisDevice*)modDev;

	//
	// build device ID
	//
	DeviceAutoId *autoId = modDev->autoId();

	// optional <bbslot> exists?
	BbSlotSpecList *bbSlotLst = modDevMdis->getBbSlotSpecList();
	if( bbSlotLst ){

		// autoId
		if( autoId ){

			// NOTE: We will not go here if autoid/mmoduleid tag is set
			//       together with bbslot/bbismodel tag.
			//       MDISWIZ prints the following WARNING and doesnt set autoId value:
			//       WARNINGS while parsing package file: *.xml: model * autoid: unsupported bustype or unsupported autoid

			switch( autoId->getType() ){

			// see note above
			// // M-Module
			// case DeviceAutoId::AutoIdMmod:
			//     DeviceMmodAutoId *mai;
			// 	   WIZ_DYNAMIC_CAST( autoId, mai, DeviceMmodAutoId *);
			// 	   createDevtypeId( modDev, swapped, mai->id, devId, mdisdev );
			// 	   break;

			// Chameleon device
			case DeviceAutoId::AutoIdCham:		// V0 module code
				// do not use V0 module code
				if( createChamAutoId( modDev, swapped, 0xFFFFFFFF, devId ) )
					return -1;
				break;
			case DeviceAutoId::AutoIdChamV2:	// V2 device Id
				DeviceChamAutoId *cai;
				WIZ_DYNAMIC_CAST( autoId, cai, DeviceChamAutoId *);
				if( createChamAutoId( modDev, swapped, cai->deviceId, devId ) )
					return -1;
				break;

			// unknown
			default:
				printf("*** unknown auto-id\n");
				return -1;
			}
		}
		// no autoId
		else {
			createDevtypeId( modDev, swapped, 0xFFFFFFFF, devId, mdisdev );
		}
	}
	// optional <busif> must exist
	else{

		QString idStr;
		HwComponent::BusIfType busIfType = modDevMdis->getDevIfType();
		switch( busIfType ){

		// PCI
		case HwComponent::StdPci:
		case HwComponent::StdPciE:
		case HwComponent::Cpci:
		case HwComponent::CpciSer:
		case HwComponent::PcMip:
		case HwComponent::Pmc:
		case HwComponent::Xmc:
		case HwComponent::Pci104:
		{
			// auto-id for PCI device?
			if( autoId ){
				DevicePciAutoId *aidPci = (DevicePciAutoId*)autoId;

				// "PCI\VEN_<ven-id>&DEV_<dev-id>[&SUBSYS_<subsys-id><subven-id>]
				idStr.sprintf("%04x", aidPci->pciVendorId);
				pciId = "PCI\\VEN_" + idStr;
				idStr.sprintf("%04x", aidPci->pciDeviceId);
				pciId += "&DEV_"     + idStr;
				// optional
				if( (aidPci->pciSubsysId != 0xFFFFFFFF) &&
					(aidPci->pciSubvenId != 0xFFFFFFFF) ){
					idStr.sprintf("%04x", aidPci->pciSubsysId);
					pciId += "&SUBSYS_" + idStr;
					idStr.sprintf("%04x", aidPci->pciSubvenId);
					pciId += idStr;
				}
			} else {
				if( G_verb )
					printf("*** WARNING: auto-id missing for PCI device\n");
			}

			// modelname specified?
			// PCI_<dev-modelname>
			if( modDev->getModelName() != modDev->getHwName() ){
				devId = "PCI_" + modDev->getModelName();
			}
			// modelname = hwname
			// PCI_<dev-hwname>_<dev-hwname>
			else {
				devId = "PCI_" + modDev->getHwName() + "_" + modDev->getHwName();
			}

			// always
			// PCI_<dev-hwname>
			devId += ",PCI_" + modDev->getHwName();

			break;
		}

		// MMOD
		case HwComponent::Mmod:
		{
			// auto-id provided?
			// "MMOD_<dev-hwname>[_SW]&ID_<dev-id>"
			// e.g. "MMOD_M36&ID_53460024"
			if( autoId ){
				DeviceMmodAutoId *mai = (DeviceMmodAutoId*)autoId;
				idStr.sprintf("%08x", mai->id);

				// MMOD_<dev-hwname>[_SW]&ID_<dev-id>
				devId = "MMOD_" + modDev->getHwName();
				if( swapped )
					devId += "_SW";
				devId += "&ID_" + idStr;
			}

			// MDIS5
			if( G_mdis5 ){
				if( !autoId ){
					// If no autoid is specified in xml, we have to create a
					// hwid that not match to any BK created hwid.
					// Note: We need at least one hwid for a model section in the inf.
					// "MMOD[_SW]&NO_AUTOID"
					// e.g. "MMOD&NO_AUTOID", "MMOD_SW&NO_AUTOID"
					devId += "MMOD";
					if( swapped )
						devId += "_SW";
					devId += "&NO_AUTOID";
				}
			}
			// MDIS4
			else {
				if( autoId )
					devId += ",";

				// for m-modules with auto-id:
				//  necessary if mod-id could not be read
				// for m-modules without auto-id:
				//  to match all m-module drivers
				// "MMOD[_SW]&ID_FFFFFFFF"
				// e.g. "MMOD&ID_FFFFFFFF", "MMOD_SW&ID_FFFFFFFF"
				devId += "MMOD";
				if( swapped )
					devId += "_SW";
				devId += "&ID_FFFFFFFF";
			}

			break;
		}

		// SMBus
		// Note: No auto-id available
		case HwComponent::Smb:
			createDevtypeId( modDev, swapped,0xFFFFFFFF, devId, smb );
			break;

		// other <busif>
		default:
			// autoId
			if( autoId ){
				switch( autoId->getType() ){

				// Chameleon device
				case DeviceAutoId::AutoIdCham:		// V0 module code
					// do not use V0 module code
					if( createChamAutoId( modDev, swapped, 0xFFFFFFFF, devId ) )
						return -1;
					break;
				case DeviceAutoId::AutoIdChamV2:	// V2 device Id
					DeviceChamAutoId *cai;
					WIZ_DYNAMIC_CAST( autoId, cai, DeviceChamAutoId *);
					if( createChamAutoId( modDev, swapped, cai->deviceId, devId ) )
						return -1;
					break;

				// unknown
				default:
					printf("*** unknown auto-id\n");
					return -1;
				}
			}
			// no autoId
			else {
				createDevtypeId( modDev, swapped,0xFFFFFFFF, devId, mdisdev );
			}
		}// switch
	}// else

	// first run adds MEN specific hwid
	// second run adds PCI\VEN_... hwid (for PCI devs)
	for( int n=0; n<2; n++ ){
		if( G_verb )
			printf("  - id: %s\n", devId.latin1() );

		fprintf( fp,
			"\"%s - %s"
			,
			qModName.latin1(),
			modDev->getDescription().latin1()
			);

		if( swapped )
			fprintf( fp, " - byte-swap access");

		fprintf( fp,
			"\"=*%s*, %s\n"
			,
			qModName.latin1(),
			devId.latin1()
			);

		modDevIds += devId + ',';

		// no pci device?
		if( pciId == "" ){
			break;	// leave loop
		}
		// pci device
		else{
			// add extra entry for pci device with pciId
			devId = pciId;
		}
	}

	return 0;
}

//! create device type specific hwid
static void createDevtypeId( Device	*modDev, bool swapped, ulong id, QString &devId, enum DevType devType )
{
	QString idStr;
	QString devtypePrefix;

	// get <dev-type>
	devtypePrefix = getDevTypePrefix( modDev, devType );

	//
	// auto-id provided?
	//

	// "<dev-type>_<dev-hwname>[_SW]&ID_<dev-id>"
	// e.g. "MDISDEV_BOROMIR&ID_00000008"
	if( id != 0xFFFFFFFF ){
		devId += devtypePrefix + "_" + modDev->getHwName();
		if( swapped )
			devId += "_SW";
		idStr.sprintf("%08x", id);
		devId += "&ID_" + idStr + ",";
	}

	//
	// always
	//

	// modelname specified?
	// "<dev-type>_<dev-modelname>[_SW]
	if( modDev->getModelName() != modDev->getHwName() ) {
		devId += devtypePrefix + "_" + modDev->getModelName();
		if( swapped )
			devId += "_SW";
	}
	// modelname = hwname
	// "<dev-type>_<dev-hwname>_<dev-hwname>[_SW]
	else {
		devId += devtypePrefix + "_" + modDev->getHwName() + "_" + modDev->getHwName();
		if( swapped )
			devId += "_SW";
	}

	// "<dev-type>_<dev-hwname>[_SW]
	// e.g. "MDISDEV_BOROMIR"
	devId += "," + devtypePrefix + "_" + modDev->getHwName();
	if( swapped )
		devId += "_SW";

	//
	// if auto-id is not provided:
	// (MDIS5: always provided)
	//

	// MDIS4
	if( !G_mdis5 ){
		// "<dev-type>[_SW]&ID_FFFFFFFF"
		// e.g. "MDISDEV&ID_FFFFFFFF", MDISDEV_SW&ID_FFFFFFFF"
		if( id == 0xFFFFFFFF ){
			devId += "," + devtypePrefix;
			if( swapped )
				devId += "_SW";
			devId += "&ID_FFFFFFFF";
		}
	}
}

//! create CHAM_... aouto id for chameleon devices
static int createChamAutoId( Device *modDev, bool swapped, ulong id, QString &devId )
{
	QString idStr;

	// MDIS5: check for chamv2id
   if( G_mdis5 && (id == 0xFFFFFFFF) ){
		printf("*** MDIS5: chamv2id must be provided in xml package-desc file!\n");
		return -1;
	}

	createDevtypeId( modDev, swapped, id, devId, cham );

	// do not use V0 module code
	if( id != 0xFFFFFFFF ){
		// MDIS4: "MDISDEV[_IO][_SW]&ID_<chamv2id>"
		// MDIS5: "CHAM[_IO][_SW]&ID_<chamv2id>"
		// e.g. "MDISDEV&ID_00000008"
		devId += "," + getDevTypePrefix( modDev, cham );
		if( swapped )
			devId += "_SW";
		idStr.sprintf("%08x", id);
		devId += "&ID_" + idStr;
	}
	return 0;
}

//! get device type specific prefix
// devType=mdisdev
//   - MDIS4: MDISDEV
//   - MDIS5: MDISDEV
// devType=cham
//   - MDIS4: MDISDEV[_IO]
//   - MDIS5: CHAM[_IO]
// devType=smb
//   - MDIS4: MDISDEV
//   - MDIS5: SMB
static QString getDevTypePrefix( Device	*modDev, enum DevType devType )
{
	QString devtypePrefix = "MDISDEV";
	QString ioTail = "";

	// modelname specified?
	if( modDev->getModelName() != modDev->getHwName() ) {
		// modelname has _IO suffix?
		if( modDev->getModelName().endsWith("_IO") ){
			// set _IO tail 
			ioTail = "_IO";
		}
	}

	// MDIS5:
	if( G_mdis5 ){
		switch( devType ){
			// CHAM[_IO]
			case cham:
				devtypePrefix = "CHAM";
				// consider _IO tail
				devtypePrefix += ioTail;
				break;
			// SMB
			case smb:
				devtypePrefix = "SMB";
				break;
		}
	}
	// MDIS4:
	else {
		switch( devType ){
			// MDISDEV[_IO]
			case cham:
				// consider _IO tail
				devtypePrefix += ioTail;
				break;
		}
	}

	return devtypePrefix;
}

//! write inf device-list for subdev
static int writeInfDevList4Subdev(
	Device		*modDev,
	int			nbrOfSubDevs,	// 1=no subdevs
	int			subDevNo,		// zero based subdev index
	bool		swapped)
{
	QString idStr;
	QString devId;

	UTIL_UNREF_PARAM( subDevNo );

	// model name
	QString	qModName = modDev->getModelName();
	if( swapped )
		qModName += "_SW";

	for( int n=0; n<nbrOfSubDevs; n++ ){
		QString qSubModName = qModName + QString("_SUBDEV_") + QChar('A'+n);

		fprintf( G_fp,
			"\"%s - %s"
			,
			qSubModName.latin1(),
			modDev->getDescription().latin1()
			);

		if( swapped )
			fprintf( G_fp, " - byte-swap access");

		fprintf( G_fp,
			"\"=*%s*,MF\\%s\n"
			,
			qSubModName.latin1(),
			qSubModName.latin1()
			);
	}

	return 0;
}

//! write inf device-section
static int writeInfDevSect( void )
{
	Device	*modDev;
	bool	swap, forWin;

	if( G_verb )
		printf("write inf device-section..\n");

	fprintf( G_fp, "\n");

	// for all models
	for( modDev = G_mods.first();
		 modDev;
		 modDev = G_mods.next() ){

		// model not for us?
		if( model4WinUser( modDev, &forWin ) )
			return -1;
		if( !forWin )
			continue;

		// SUB-DEV
		int subDevNo = 0;
		DescriptorEntryDirectory descTmp( "_TMP_" );
		Device *dev = modDev->create(false);
		int nbrOfSubDevs = ((MdisDevice *)dev)->numSubDevs();	// 1=no subdevs

		// swapped variant supported?
		if( llDrvSupportsSwapping( modDev, &swap ) )
			return -1;

		// subdev?
		if( nbrOfSubDevs > 1){
			if( writeMfInfDevSect4Dev( modDev, nbrOfSubDevs, FALSE ) )
				return -1;
			if( swap )
				if( writeMfInfDevSect4Dev( modDev, nbrOfSubDevs, TRUE ) )
					return -1;
		}

		while( subDevNo < nbrOfSubDevs ){

			WIZ_ASSERT(dev);

			dev->createSpecialDesc( &descTmp );

			if( writeInfDevSect4Dev( modDev, nbrOfSubDevs, subDevNo, &descTmp, FALSE ) )
				return -1;

			if( swap ){
				if( writeInfDevSect4Dev(modDev, nbrOfSubDevs, subDevNo, &descTmp, TRUE ) )
					return -1;
			}

			if( ++subDevNo < nbrOfSubDevs )
				dev = ((MdisDevice *)dev)->createSubDev( subDevNo );
		}
	}

	if( G_mfFp )
		closeMfInf();

	return 0;
}

//! write inf device-section for device
static int writeInfDevSect4Dev(
	Device						*modDev,
	int							nbrOfSubDevs,
	int							subDevNo,
	DescriptorEntryDirectory	*descEntryDir,
	bool						swapped )
{
	SwModuleList	*swModList;
	SwModule		*swMod;

	// model name
	QString	qModName = modDev->getModelName();
	if( swapped )
		qModName += "_SW";
	if( nbrOfSubDevs > 1 )
		qModName += QString("_SUBDEV_") + QChar('A'+subDevNo);
	const char *modName = qModName.latin1();

	if( G_verb )
		printf("- model: %s, ", modName );

	// enumerate sw-module list
	swModList = modDev->enumSwModules();

	// get ll-drv (or native driver e.g. for lltest)
	if( !(swMod = swModList->find( SwModuleTypes::LlDriver )) )
		swMod = swModList->find( SwModuleTypes::NativeDriver );
	
	if( !swMod ){
		printf("*** ll-drv or native driver missing\n");
		return -1;
	}

	// driver name
	QString	qDrvName = swMod->modName();
	if( swapped )
		qDrvName += "_sw";
	const char *drvName = qDrvName.latin1();

	if( G_verb )
		printf("drv: %s\n", drvName );

	fprintf( G_fp,
		";;\n"
		";; *%s* section\n"
		";;\n"
		"[*%s*]\n"
		"CopyFiles=Xml_Files,*%s*_Files\n"
		"\n"
		"[*%s*_Files]\n"
		"men_%s.sys\n"
		"\n"
		"[*%s*.Services]\n"
		"AddService=men_%s,0x00000002,*%s*_Service_Inst,EventLog_Inst\n"
		"\n"
		"[*%s*_Service_Inst]\n"
		"ServiceType=0x1\n"
		"StartType=0x3\n"
		"ErrorControl=0x1\n"
		"ServiceBinary=%%12%%\\men_%s.sys\n"
		"\n"
		"[*%s*.HW]\n"
		"AddReg=*%s*_HW_AddReg\n"
		"\n"
		"[*%s*_HW_AddReg]\n"
		"HKR,,PackDescFile,%%REG_SZ%%,\"%s\"\n"
		"HKR,,ModelName,%%REG_SZ%%,\"%s\"\n"	// without "_SW" suffix
		,
		modName,
		modName,
		modName,
		modName,
			drvName,
		modName,
			drvName,
		modName,
		modName,
			drvName,
		modName,
		modName,
		modName,
		G_xmlFileName.latin1(),
		modDev->getModelName().latin1()			// NOT modName with "_SW" suffix
		);

		QString dir = "Descriptor";
		if( writeInfNonDefDesc( dir, descEntryDir ) )
			return -1;

		fprintf( G_fp, "\n" );

	// MDIS5
	if( G_mdis5 ){
		fprintf( G_fp,
			";; WDF specifics\n"
			"[*%s*.CoInstallers]\n"
			"AddReg=WdfCoInst_AddReg\n"
			"CopyFiles=WdfCoInst_Files\n"
			"\n"
			"[*%s*.Wdf]\n"
			"KmdfService=men_%s, WdfLib\n"
			"\n",
			modName,
			modName,
			drvName
			);
	}

	return 0;
}

//! write non-default descriptor entries
static int writeInfNonDefDesc(
	QString						dir,
	DescriptorEntryDirectory	*descEntryDir )
{
	int					rv=-1;		// error
	static int			level=0;	// call level (for debugging)
	QString				regType;
	DescriptorEntry		*e;
	QString				currDir;
	QString				value;

	level++;	// 1=first level

	if( G_verb )
		printf("writeInfNonDefDesc(): level=%d, dir=%s\n", level, dir.latin1() );

	// handle all entries of current directory
	for( e=descEntryDir->firstChild(); e; e=descEntryDir->nextChild()){

		currDir = dir;

		// skip inactive parameter
		if( !e->isActive() )
			continue;

		value = e->valueToString();

		// map parameter type
		switch( e->getType() ){

		case DescriptorEntry::Uint32:
			regType = "%REG_DWORD%";
			break;

		case DescriptorEntry::String:
			regType = "%REG_SZ%";
			value = QString("\"") + value + QString("\"");
			break;

		case DescriptorEntry::Binary:
			regType = "%REG_BINARY%";
			break;

		case DescriptorEntry::Directory:
			regType = "DIR";

			// add sub-dir
			currDir += "\\";
			currDir += e->getKey();

			// recursive call to handle sub-params
			rv = writeInfNonDefDesc( currDir, (DescriptorEntryDirectory*)e );
			if( rv == -1 )
				goto CLEANUP;

			break;

		default:
			rv = -1;
			goto CLEANUP;

		} // switch

		// create value
		if( regType != "DIR" ){
			// reg-root,[subkey],[value-entry-name],[flags],[value]
			fprintf( G_fp, "HKR,%s,%s,%s,%s\n",
				currDir.latin1(),		// subkey (e.g. Descriptor\RD_BUF)
				e->getKey().latin1(),	// value-entry-name
				regType.latin1(),		// flags
				value.latin1() );		// value
		}

	} // for

	rv = 0;

CLEANUP:

	level--;
	return rv;
}

//! write inf common-section
static int writeInfCommonSect( void )
{
	fprintf( G_fp,
		";;\n"
		";; Common section\n"
		";;\n"
		"[Xml_Files]\n"
		"%s\n"
		"\n"
		";; Event logging\n"
		"[EventLog_Inst]\n"
		"AddReg=EventLog_AddReg\n"
		"\n"
		"[EventLog_AddReg]\n"
		"HKR,,EventMessageFile,%%REG_EXPAND_SZ%%,\""
		"%%%%SystemRoot%%%%\\System32\\IoLogMsg.dll;%%%%SystemRoot%%%%\\System32\\men_evlg.dll\"\n"
		"HKR,,TypesSupported,%%REG_DWORD%%,7\n"
		"\n"
		,
		G_xmlFileName.latin1()
		);

	// MDIS5
	if( G_mdis5 ){
		fprintf( G_fp,
			";; WDF specifics\n"
			"[WdfCoInst_AddReg]\n"
			"HKR,,CoInstallers32,0x00010000, \"WdfCoInstaller%s.dll,WdfCoInstaller\"\n"
			"[WdfCoInst_Files]\n"
			"WdfCoInstaller%s.dll\n"
			"\n"
			"[WdfLib]\n"
			"KmdfLibraryVersion=%s\n"
			"\n",
			G_mdis5_kmdfCoinstVer,
			G_mdis5_kmdfCoinstVer,
			G_mdis5_kmdfVer);
	}

	return 0;
}

//! write inf string-section
static int writeInfStringSect( void )
{
	fprintf( G_fp,
		";;\n"
		";; String section\n"
		";;\n"
		"[Strings]\n"
		"REG_DWORD=0x00010001\n"
		"REG_SZ=0x00000000\n"
		"REG_EXPAND_SZ=0x00020000\n"
		"REG_BINARY=0x00000001\n"
		"COPYFLG=0x00000060\n"
		"MEN=\"MEN Mikro Elektronik\"\n"
		);
	return 0;
}


//! checks if the specified sw-module supports Windows
//! and is not for internal usage
static bool swMod4WinUser( SwModule *swMod )
{
	// ll-drv not for Windows
	if( false == swMod->supportsOs( OsFactory::Win ) ){
		if( G_verb )
			printf("- drv %s not for Windows\n",
				swMod->modName().latin1() );
		return false;
	}

	// ll-drv for internal usage?
	if( true == swMod->isInternal() ){
		if( G_verb )
			printf("- drv %s for internal usage\n",
				swMod->modName().latin1() );
		return false;
	}

	return true;
}

//! checks if the ll-drv for the specified device supports Windows
//! and is not for internal usage
static int model4WinUser( Device *modDev, bool *forWin )
{
	SwModuleList	*swModList;
	SwModule		*swMod;

	// enumerate sw-module list
	swModList = modDev->enumSwModules();

	// get ll-drv (or native driver e.g. for lltest)
	if( !(swMod = swModList->find( SwModuleTypes::LlDriver )) )
		swMod = swModList->find( SwModuleTypes::NativeDriver );
	
	if( !swMod ){
		printf("*** ll-drv or native driver missing\n");
		return -1;
	}

	// sw-module not for us?
	*forWin = swMod4WinUser( swMod );

	return 0;
}

//! checks if the ll-drv for the specified device supports swapping
static int llDrvSupportsSwapping( Device *modDev, bool *swapp )
{
	SwModuleList	*swModList;
	SwModule		*swMod;

	// enumerate sw-module list
	swModList = modDev->enumSwModules();

	// get ll-drv (or native driver e.g. for lltest)
	if( !(swMod = swModList->find( SwModuleTypes::LlDriver )) )
		swMod = swModList->find( SwModuleTypes::NativeDriver );
	
	if( !swMod ){
		printf("*** ll-drv or native driver missing\n");
		return -1;
	}

	// swapped variant supported?
	*swapp = swMod->isSwapCapable();

	return 0;
}

//! open mf-inf
static int openMfInf( PackageFile &pkgFile )
{
	// open inf-file for writing
	if( (G_mfFp = fopen( G_mfInfPath.latin1(), "w" )) == NULL ){
		printf("*** open file %s failed\n", G_mfInfPath.latin1() );
		return 1;
	}
	printf("output: INF-file %s (MF-INF for sub-devs)\n", G_mfInfPath.latin1() );


	if( writeInfFileHeader( G_mfFp, TRUE ) )
		return 1;

	fprintf( G_mfFp,
		"[Version]\n"
		"Signature=\"$Windows NT$\"\n"
		"Class=\"MultiFunction\"\n"
		"ClassGUID={4d36e971-e325-11ce-bfc1-08002be10318}\n"
		"Provider=%%MEN%%\n"
		"DriverPackageDisplayName=\"%s (parent devices)\"\n"
		"DriverVer=%s,%s\n"
		,
		pkgFile.pkgTag("description").latin1(),
		QDate::currentDate().toString("MM/dd/yyyy").latin1(), G_infRev.latin1()
		);

		// MDIS5
	if( G_mdis5 )
		fprintf( G_mfFp,
			"CatalogFile=%s_mf.cat\n",
			G_natPkgName.latin1()
			);

	fprintf( G_mfFp,
		"\n");

	writeInfDevListHeader( G_mfFp );

	return 0;
}

//! write mf-inf device-section for device
static int writeMfInfDevSect4Dev(
	Device	*modDev,
	int		nbrOfSubDevs,
	bool	swapped )
{
	QString qModName = modDev->getModelName();
	bool	pci;

	// model name
	if( swapped )
		qModName += "_SW";
	const char *modName = qModName.latin1();

	if( G_verb )
		printf("- model: %s, ", modName );

	fprintf( G_mfFp,
		";;\n"
		";; *%s* section\n"
		";;\n"
		"[*%s*]\n"
		"Include = mf.inf\n"
		"Needs   = MFINSTALL.mf\n"
		"\n"
		"[*%s*.Services]\n"
		"Include = mf.inf\n"
		"Needs = MFINSTALL.mf.Services\n"
		"\n"
		"[*%s*.HW]\n"
		"AddReg=*%s*_HW_AddReg\n"
		"\n"
		"[*%s*_HW_AddReg]\n"
		,
		modName,
		modName,
		modName,
		modName,
		modName,
		modName);

	// determine device type
	HwComponent::BusIfType busIfType = ((MdisDevice*)modDev)->getDevIfType();
	switch( busIfType ){
		// PCI
		case HwComponent::StdPci:
		case HwComponent::StdPciE:
		case HwComponent::Cpci:
		case HwComponent::CpciSer:
		case HwComponent::PcMip:
		case HwComponent::Pmc:
		case HwComponent::Xmc:
		case HwComponent::Pci104:
			pci = TRUE;
			fprintf( G_mfFp, "; Note: We assume that the enumerated PCI device uses\n"
							 ";       one interrupt and one address resource.\n");
			break;
		// other (e.g. MMOD)
		default:
			pci = FALSE;
			fprintf( G_mfFp, "; Note: We assume that the enumerated device uses\n"
							 ";       one interrupt resource.\n");
	}

	fprintf( G_mfFp, ";       However, all actually existing resources must be assigned\n"
					 ";       to each subdevice!\n");

	// Note: The _SUBDEV_i index must be in reversed order
	//       bcause the "reversed installation procedure"
	//       of the Windows Hardware Wizard
	for( int n=0, r=nbrOfSubDevs-1;
		 n<nbrOfSubDevs;
		 n++, r-- ){

		QString qSubModName = qModName + QString("_SUBDEV_") + QChar('A'+r);

		fprintf( G_mfFp, "HKR, Child%04d, HardwareID,,MF\\%s\n",
			n, qSubModName.latin1());

		// pci-dev?
		if( pci )
			fprintf( G_mfFp, "HKR,Child%04d,ResourceMap,1,00,01    ; use res 00 (addr), 01 (irq)\n", n);
		// other dev
		else
			fprintf( G_mfFp, "HKR,Child%04d,ResourceMap,1,00       ; use res 00 (irq)\n", n);
	}

	fprintf( G_mfFp, "\n" );

	return 0;
}

//! close mf-inf
static int closeMfInf( void )
{
	fprintf( G_mfFp,
		";;\n"
		";; String section\n"
		";;\n"
		"[Strings]\n"
		"MEN=\"MEN Mikro Elektronik\"\n"
		);

	fclose( G_mfFp );
	G_mfFp = NULL;

	return 0;
}






















