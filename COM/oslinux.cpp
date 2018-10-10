/***************************************************************************
 */
/*!       \file  oslinux.cpp
 *       \brief  Implementation of class OsLinux
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2018/06/13 14:18:06 $
 *    $Revision: 2.28 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: oslinux.cpp,v $
 * Revision 2.28  2018/06/13 14:18:06  ts
 * R: updated linux version to 13.0, added MDIS_MAJOR_NUMBER to ignored keys (now requested automatically)
 *
 * Revision 2.27  2016/04/28 18:02:43  ts
 * R: MDIS major number now chosen automatically by driver men_mdis_kernel.ko
 * M: removed tab with major number entry
 *
 * Revision 2.26  2016/02/20 16:27:27  ts
 * R: BoxPCs based on SC24 need 2 SMBusses in MDIS wizard
 * M: 1. added member nrSmbBusses to class CpuDeviceSmb
 *    2. added parameter int nrSmbBusses to every constructor of CPUs with SMB
 *       controllers: 2 for SC24 (AMD based) Box PCs, 1 for every other CPU
 *
 * Revision 2.25  2015/05/26 17:17:41  ts
 * R: gituser autocheckin: updated source
 *
 * Revision 2.24  2014/06/06 14:15:49  ts
 * R: typo "for for" in wizard tab for userstate applications
 * M: corrected text, updated version to linux-10.6
 *
 * Revision 2.23  2013/11/27 10:37:51  ts
 * R: usage for generating projekts with other environment for A21 unclear
 * M: added explicit "ELDK" in other environment
 *
 * Revision 2.22  2013/09/27 08:19:14  dpfeuffer
 * R: VC compiler warning: unreferenced formal parameter
 * M: OsLinuxConfiguration::enableSwMods: UTIL_UNREF_PARAM( errMsg ) added
 *
 * Revision 2.21  2013/05/17 18:47:19  ts
 * R: scan_system script failed with new cpu names for conductive cooled boards
 * M: model names F19P/F19C changed to F19P_F19C ('/' breaks sed commands)
 *
 * Revision 2.20  2013/01/23 18:47:23  ts
 * R: scanned configuration was visible after manual loading only
 * M: added automatic reloading and displaying of generated system.dsc
 *
 * Revision 2.19  2013/01/21 10:28:09  ts
 * R: manual system configuration was often difficult
 * M: added support for automatic system scan
 *
 * Revision 2.18  2012/12/20 13:19:07  ts
 * R: new cPCI card F223 was introduced
 * M: added support, change revision to linux-10.3
 *
 * Revision 2.17  2012/08/07 18:30:43  ts
 * R: Box Computer models were missing together with SC24
 * M: 1. expanded model name SC24 to SC24/BC2/BCxx
 *    2. increased wizard version to 10.2
 *
 * Revision 2.16  2012/05/23 15:38:34  ts
 * R: 1. Adding cPCI components to F14 CPU was not possible
 *    2. choosing cPCI slot for F210 was not possible
 * M: 1. changed constructor in modcpuf14.h
 *    2a. condition to select between cPciSerial and cPCI corrected
 *    2b. set mdiswiz Version to 10.1
 *
 * Revision 2.15  2012/05/14 11:17:59  ts
 * R: default major number 250 for /dev/mdis already in use on Ubuntu 12.04
 * M: changed default major number to 248
 *
 * Revision 2.14  2012/04/23 14:36:10  ts
 * R: new Models for G215, G20 and SC24 added
 * M: updated version to "-linux-10.0"
 *
 * Revision 2.13  2011/03/31 17:25:59  CRuff
 * R: 1. warn user if installed MDIS system package and selected cpu do not match
 *    2. also check pci domains when cpu is changed
 *    3. for ElinOS 5.0 projects, lin kernel dir check was still executed at
 *       project load
 *    4. Device Node Install Dir had no default value for ELinOS projects
 * M: 1. check pci domain compatibility in Configuration::save() and load()
 *    2a) connect to signal sigCpuChanged
 *    2b) in slot menCpuChanged(), check pci domain compatibility
 *    3. in loadMakefile(), only check lin kernel dir validity if the loaded
 *       Makefile is no ELinOS Makefile
 *    4. added default value for device node installation directory
 *
 * Revision 2.12  2011/02/16 17:03:55  CRuff
 * R: new revision
 * M: changed revision string
 *
 * Revision 2.11  2010/12/10 11:02:27  CRuff
 * R: 1. Kernel dir validation for ELinOs fails if force linking is not selected
 *    2. cosmetics
 * M: 1. read ELinOs version from ELINOS.sh and only check kernel dir if the
 *       version is < 5.0
 *    2. removed debugs in OsLinuxNewProjectWizard
 *
 * Revision 2.10  2010/10/22 11:52:40  CRuff
 * R: release of new version
 * M: increased version string
 *
 * Revision 2.9  2010/10/22 10:54:23  CRuff
 * R. 1. cosmetics
 * M: 1. in load(): do not use format string for error messages, but QString
 *       operator +
 *
 * Revision 2.8  2010/07/15 08:54:02  CRuff
 * R: new version
 * M: increase version number to 9.1 (patch version)
 *
 * Revision 2.7  2010/07/14 16:58:41  CRuff
 * R: autoconf string is sometimes empty
 * M: only try to open file if an autoconf path is set
 *
 * Revision 2.6  2010/07/14 15:29:12  CRuff
 * R: autoconf string is sometimes empty
 * M: set autoconfPath every time before its usage
 *
 * Revision 2.5  2010/07/07 16:42:27  CRuff
 * R: avoid possibility of application crash during loadMakefile
 * M: loadMakefile: verify that string is not empty before accessing char 0
 *
 * Revision 2.4  2010/07/01 11:12:53  CRuff
 * R: generated linux header autoconf.h was moved to directory include/generated
 *    with kernel version 2.6.33
 * M: 1. look for autoconf.h both in linux and in generated
 *    2. save path to autoconf.h in static QString and use variable instead of
 *       hard coded path
 *
 * Revision 2.3  2010/06/21 10:55:05  CRuff
 * R: release of new version
 * M: increase version number
 *
 * Revision 2.2  2010/06/08 11:15:00  CRuff
 * R: default major dev node number 252 is often taken on current installations
 * M: changed the default major number to 250
 *
 * Revision 2.1  2010/05/19 16:02:37  CRuff
 * R: 1. crashes with QT 4.6
 * M: 1a) saveMakefile(): also handle as StringList in case of simple string
 *        for non-Win32 case
 *    1b) validateSwModule(): do not dereference iterator
 *
 * Revision 2.0  2010/01/22 11:26:54  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.30  2009/08/21 09:17:19  CRuff
 * R: UTS_RELEASE has moved to utsrelease.h for kernels >= 2.6.18
 * M: linKernelDirChanged(): read utsrelease.h if version.h does not contain the
 *    UTS_RELEASE definition
 *
 * Revision 1.29  2009/05/15 15:04:37  dpfeuffer
 * R: mdiswiz_lib for Windows: error C2011: 'HKEY__' : 'struct' type redefinition
 * M: #include "Windows.h" removed
 *
 * Revision 1.28  2009/05/13 11:31:56  CRuff
 * R: 1. Core lib 'oss' needs to pull in additional makefile for VxWorks
 *    2. Symbol name ALL_CORE in generated Makefile causes cyclic dependency
 * M: 1. extended mkSwModulesTups(), validateSwModule() to handle sw modules
 *       with multiple makefiles
 *    2. Rename symbol ALL_CORE to ALL_CORE_LIBS in
 *       OsLinuxConfiguration::loadMakefile(), ::swModuleMfString()
 *
 * Revision 1.27  2009/04/30 21:23:46  CRuff
 * R: release MDIZWIZ
 * M: increased version number
 *
 * Revision 1.26  2009/04/30 19:34:33  CRuff
 * R: Makefile flag -DCONFIG_MEN_VME_KERNELIF has to be added also for VME4L_API
 * M: added flag to MDIS_EXTRA_DEFS if VME4L_API lib is included
 *
 * Revision 1.25  2009/04/30 17:28:59  CRuff
 * R: make Kernel modules OSS and DBG configurable
 * M: set value for CoreLib flag to 'no' if no module is selected
 *
 * Revision 1.24  2009/04/29 11:42:45  CRuff
 * R: Do not offer project option 'Selfhosted' if wizard runs on windows host
 * M: removed option 'Selfhosted' for windows host
 *
 * Revision 1.23  2009/04/27 17:33:54  CRuff
 * R: make Kernel modules OSS and DBG configurable
 * M: added handling for new moduleType SwModuleType::CoreLib to methods
 *    OsLinuxConfiguration::saveMakefile() and OsLinuxConfiguration::swModuleMfString()
 *
 * Revision 1.22  2008/02/06 15:57:32  cs
 * cosmetics to avoid warning on build
 *
 * Revision 1.21  2008/02/06 12:46:40  cs
 * R: running mdiswiz for Linux under Windows:
 *    - cygwin/ELinOS requires Linux like paths
 *    - QT requires Windows specific paths.
 * M: pass paths either host or shell specific, depending on use
 *
 * Revision 1.20  2006/11/30 11:50:42  cs
 * added:
 *   + backup old configuration files to *.bak on load of existing project
 * raised version
 *
 * Revision 1.19  2006/11/08 13:57:25  cs
 * changed:
 *   - validate ELinOS installation directorys only after sourcing ELINOS.sh
 *   - accept directories other than /opt/elinos (e.g. /opt/elinos-4.1)
 *
 * Revision 1.18  2006/09/26 15:44:21  ts
 * Added detection of Xenomai to enable RTAI yes/no selection
 *
 * Revision 1.17  2005/12/07 16:55:16  ub
 * cosmetics
 *
 * Revision 1.16  2005/07/15 15:47:11  ub
 * - wizard dialog now wider
 * - "make" call improved
 *
 * Revision 1.15  2005/01/31 15:15:49  ts
 * Revision number set to linux-5
 *
 * Revision 1.14  2005/01/18 17:25:44  ts
 * added seek for RTAI ADEOS in addition to CONFIG_RTHAL
 *
 * Revision 1.13  2004/12/02 10:19:36  kp
 * hide SW modules from RTAI that work under Linux User Mode only
 *
 * Revision 1.12  2004/07/28 11:15:22  ufranke
 * added
 *  - stdlib.h for Visual C++.net Standard
 *
 * Revision 1.11  2004/07/21 15:01:17  ufranke
 * changed
 *  - currentCfg() by MAIN_GetCurrentCfg()
 *
 * Revision 1.10  2004/07/16 12:30:46  kp
 * Version linux-3
 * - Disable LIN_USR_INC_DIR for Selfhosted mode
 * - Fixed problem with kdesu (must change to project dir)
 * - added Environment help text
 *
 * Revision 1.9  2004/06/08 17:25:42  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.8  2004/01/09 08:20:10  kp
 * added batchInstall
 *
 * Revision 1.7  2003/06/11 12:21:41  kp
 * intermediate checkin
 *
 * Revision 1.6  2003/06/06 13:32:24  kp
 * Elinos StaticLibInstallDir fixed
 *
 * Revision 1.5  2003/06/05 12:01:14  dschmidt
 * intermediate checkin
 *
 * Revision 1.4  2003/06/05 09:33:43  kp
 * intermediate checkin
 *
 * Revision 1.3  2003/06/03 10:16:27  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/05/14 13:44:51  kp
 * misc startup changes
 *
 * Revision 1.1  2003/05/02 14:36:13  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <qapplication.h>
#include <q3filedialog.h>
#include <qfileinfo.h>
#include <qwidget.h>
#include <qstringlist.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <q3process.h>
#include <q3buttongroup.h>
#include <qradiobutton.h>
#include <qmessagebox.h>
#include <qregexp.h>
#include <qdir.h>
#include <qaction.h>
#include <q3toolbar.h>
#include <qdatetime.h>

#include <QLabel>
#include <QPixmap>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <Q3PopupMenu>
#include <Q3PtrList>

#include "wizdebug.h"
#include "configuration.h"
#include "hwcomponent.h"
#include "oslinux.h"
#include "cfgmakefile.h"
#include "swmodulestab.h"
#include "oslinux.h"
#include "packagedb.h"
#include "setting.h"
#include "vmsgbox.h"
#include "util.h"
#include "install.h"
#include "modelfactory.h"
#include "mdiswizdialogs.h"
#include "mdiswiz.h"
#include "execprogram.h"
#include "stdlib.h"

// getcwd() prototype is declared in direct.h under Windows
#ifdef _WIN32
 #include <direct.h>
#else
# include <unistd.h>
# define _getcwd        getcwd
#endif

// pixmaps
#include "images/buildinstall.xpm"
#include "images/scanning.xpm"
#include "images/build.xpm"
#include "images/rebuild.xpm"

extern QWidget *G_mainWindow; //!< used as anchor for dialog
extern QApplication *G_qApp;  //!< main application

// strings
static const QString mdiswizLinuxVersion("linux-13.0");
static const QString sysDescPlainName("system");
static const QString sysDescName( sysDescPlainName + ".dsc");
static const QString makeName("Makefile");
static const QString defMenLinuxPath("/opt/menlinux");
static const QString elinosSubPath("src/mdis");
static const QString packageDbPath("PACKAGE_DESC");
static void setAutoConfDir( QString kernelDir );
static QString elinosRootPath;
static QString menLinuxPath; // typicall /opt/menlinux
static QString autoconfPath;


//---------------------------------------------------------------------
// Static functions

//----------------------------------------------------------------------
//! check if kernel dir is ok for MDIS
/*!
  checks for \c kernel.h, \c autoconf.h and check if CONFIG_MODULES is
  set.

  \return true if everything ok, else false and errMsg contains error msg
*/
static bool isGoodKernelDir( const QString &kDir, QString &errMsg )
{
	// check if it's really a linux kernel include dir...
	QFileInfo fi1( kDir + "/include/linux/kernel.h" );
	QFileInfo fi1lnk( fi1.filePath() + ".lnk" );

	if( !fi1.exists() && !fi1lnk.exists()){
		errMsg += "\"" + kDir +
			"\" does not look like a linux kernel directory (" +
			fi1.filePath() + " not found)\n";
		return false;
	}

	// check if kernel has been compiled...
	setAutoConfDir( kDir );
	
	if ( autoconfPath.isEmpty() ) {
		errMsg += "\" file autoconf.h not found in /include/linux or /include/generated. "
			"You must compile this kernel first. Come back later!\n";
		return false;
	} 
#if 0 // set by default in all new distributions
	// check if CONFIG_MODULES has been defined
	if( !OsLinuxConfiguration::isAutoConfOptionSet(
		kDir, "CONFIG_MODULES")){

		errMsg += "\"" + kDir + "\": this kernel is missing"
			" loadable module support.\nConfigure the kernel with"
			" CONFIG_MODULES set and come back!\n";
		return false;
	}
#endif 
	return true;
}

void setAutoConfDir( QString kDir )
{
	QFileInfo fi2( kDir + "/include/linux/autoconf.h" );
	
	if( fi2.exists() ){
		autoconfPath = kDir + "/include/linux/autoconf.h";
		wDebug(("autoconf1"));
	} else {
		// from kernel 2.6.33, autoconf.h is located in include/linux/generated
		QFileInfo fi3( kDir + "/include/generated/autoconf.h" );
		
		if ( fi3.exists() ) {
			autoconfPath = kDir + "/include/generated/autoconf.h";
			wDebug(("autoconf2"));
		} else {
			autoconfPath = "";
			wDebug(("autoconf empty"));
		}
	}
}

//----------------------------------------------------------------------
//! Get the architecture used in this kernel
/*!
  This is determined by looking at the symbolic link include/asm

  \param kDir kernel directory
  \returns one of the CPU architectures or UnsetArch if unknown
*/
static CpuDevice::Arch getArch( const QString &kDir )
{
	CpuDevice::Arch arch = CpuDevice::UnsetArch;
	QFileInfo fi( kDir + "/include/asm" );

	if( fi.exists() ){
		QString lnk = fi.readLink();

		wDebug(("getArch: %s", (const char *)lnk));

		if( lnk == "asm-i386" )
			arch = CpuDevice::X86;
		else if( lnk == "asm-ppc" )
			arch = CpuDevice::Ppc;
		else if( lnk == "asm-arm" )
			arch = CpuDevice::Arm;
		else if( lnk == "asm-m68k" )
			arch = CpuDevice::M68k;
	}
	return arch;
}

//----------------------------------------------------------------------
//! Check if directory is ok to store MDIS configuration
/*!
  checks if specified \a dir is empty

  \param dir directory to check
  \returns true if it's an empty directory (or it doesn't exist), false
  if not and errMsg contains newline separated error messages
*/
static bool isGoodMdisProjDir( const QString &dir, QString &errMsg )
{
	QFileInfo fi( dir );

	// check if directory exists. If so, check if it is empty
	if( fi.exists() ){
		if( fi.isDir() ){
			QDir di(dir);

			if( di.count() >2 ){ // ignore . and ..

				errMsg += "\"" + dir + "\" is an existing,"
					" non-empty directory!\n";
				return false;
			}
		}
		else {
			errMsg += "\"" + dir + "\" is an existing,"
				" file (not a directory)!\n";
			return false;
		}
	}
	return true;
}

//----------------------------------------------------------------------
//! Check if the installed MDIS system package supports pci domains
/*!
  checks if file mdis_version.h is found and contains 
  #define OS_HAS_PCI_DOMAINS

  \returns true if file is found and contains define, false otherwise
*/
bool
OsLinuxConfiguration::mdisSupportsPciDomains( )
{
	QFileInfo mdisVersFile( menLinuxPath + QString("/INCLUDE/NATIVE/MEN/mdis_version.h"));
	// check if native include file mdis_version.h exists
	if( mdisVersFile.exists() ){
		// check content of the file
		QFile file(mdisVersFile.absolutePath() + QString("/mdis_version.h"));
		if (file.open(IO_ReadOnly)) {
			QTextStream stream (&file);
			QString line;
			while ( !stream.atEnd() ) {
				line = stream.readLine();
				if ( line.contains("HAS_PCI_DOMAINS")) {
					return true;
				}
			}
		}
	}
	return false;
}

//----------------------------------------------------------------------
//! Check if pci domains are needed by selected cpu and supported by MDIS
/*!
  shows warning message if current CPU has several pci domains and the
  installed MDIS system package does not contain mdis_version.h with 
  #define OS_HAS_PCI_DOMAINS set
*/
void
OsLinuxConfiguration::checkPciDomains()
{
	Configuration* currConfig;
	bool domainsNeeded;
	bool domainsSupported;

	// check if pci Domains are supported
	currConfig = MAIN_GetCurrentCfg();
	currConfig->setPciDomainsSupported( mdisSupportsPciDomains( ) );
	domainsNeeded = currConfig->arePciDomainsNeeded();
	domainsSupported = currConfig->arePciDomainsSupported();
	wDebug(("domains needed: %s, domains supported: %s\n", domainsNeeded ? "true" : "false",
			domainsSupported ? "true" : "false" ));
	if (currConfig->arePciDomainsNeeded() &&
		!currConfig->arePciDomainsSupported() ) {

		VMsgBox::criticalOk(
				"MDIS system package too old for this CPU",
				"Out-dated MDIS system package may lead to wrong device access for this CPU",
				"Please update MDIS system package to avoid inconsistencies",
				G_mainWindow );
	}
}
//---------------------------------------------------------------------
// Settings


class SettingCdk : public MultipleChoiseSetting {
public:
	enum Choises { Selfhosted=0, Elinos, Other };

	SettingCdk() : MultipleChoiseSetting( "WIZ_CDK" ) {
		
#ifndef _WIN32
		choises << "Selfhosted";
#endif 
		choises << "ELinOS" << "Other";
		
		choisesHelp <<
			"The development host is the same as your target system. Uses "
			"the GNU compiler tool chain." <<

			"Cross compilation with SYSGO's embedded Linux distribution" <<

			"Other environment, e.g. ELDK";

		_value = QVariant(choises[Selfhosted]);
	}

	// dont't show in Setting dialog
	virtual QWidget *settingsTabWidget( QWidget *parent ){
		UTIL_UNREF_PARAM(parent);
		return 0;
	}

	virtual Setting *clone(){
		SettingCdk *setting = new SettingCdk();
		copy( setting );
		return setting;
	}

	void slotApply( QString &errMsg ){
		validate( value(), errMsg );
	}

	virtual bool validate( const QVariant &newVal, QString &errMsg ){
		UTIL_UNREF_PARAM(newVal);
		UTIL_UNREF_PARAM(errMsg);
		return true;
	}

protected:
	QString infoText() {
		return "You need to select the development environment so that MDIS "
			"modules are compiled with the correct tool chain";
	}
	QString labelText() {
		return "Development Environment";
	}
	QString wizPageTitle() {
		return "Select the development environment";
	}
};



class SettingLinKernelDir : public OsLinuxDirectorySetting {
public:

	SettingLinKernelDir() :
		OsLinuxDirectorySetting( "LIN_KERNEL_DIR",
						  DirectorySetting::Dir |
						  DirectorySetting::MustExist) {
	}

	virtual Setting *clone(){
		SettingLinKernelDir *setting = new SettingLinKernelDir();
		copy( setting );
		return setting;
	}

	virtual bool validate( const QVariant &newVal, QString &errMsg ){
		QString expVal = OsLinuxConfiguration::hostSpecPath( lst->expandedValueOf( newVal.toString() ) );

		// basic validation first
		if( DirectorySetting::validate( QVariant(expVal), errMsg ) == false )
			return false;

		if( isGoodKernelDir( expVal, errMsg ) == false ){
			errMsg = key() + ": " + errMsg;
			return false;
		}
		return true;
	}
protected:

	QString infoText() {
		return "The directory of the kernel tree used for your "
			"target's kernel. If you're doing selfhosted development, "
			"it's typically /usr/src/linux. This directory is "
			"used when building the kernel modules.";
	}
	QString labelText() {
		return "Kernel Directory";
	}
	QString wizPageTitle() {
		return "Select Target's Kernel Directory";
	}
};


class SettingRtaiDir : public OsLinuxDirectorySetting {
public:

	SettingRtaiDir() :
		OsLinuxDirectorySetting( "RTAI_DIR",
						  DirectorySetting::Dir |
						  DirectorySetting::MustExist) {
	}

	virtual Setting *clone(){
		SettingRtaiDir *setting = new SettingRtaiDir();
		copy( setting );
		return setting;
	}

	virtual bool validate( const QVariant &newVal, QString &errMsg ){
		QString expVal = OsLinuxConfiguration::hostSpecPath( lst->expandedValueOf( newVal.toString() ) );

		// basic validation first
		if( DirectorySetting::validate( QVariant(expVal), errMsg ) == false )
			return false;

		// check if it's really a RTAI include dir...
		QFileInfo fi1( expVal + "/include/rtai.h" );
		if( !fi1.exists() ){
		    errMsg = key() + ": \"" + expVal +
			  "\" does not look like a RTAI directory (" +
			  fi1.filePath() + " not found)\n";
		    return false;
		}
		return true;
	}
protected:

	QString infoText() {
	    return "The directory where you have installed the RTAI distribution"
			" via \"make install\"" ;
	}
	QString labelText() {
		return "RTAI Directory";
	}
	QString wizPageTitle() {
		return "Select RTAI Distribution Directory";
	}
};



class SettingMdisProjDir : public OsLinuxDirectorySetting {
public:

	SettingMdisProjDir() :
		OsLinuxDirectorySetting( "MDIS_PROJ_DIR",
						  DirectorySetting::Dir ) {
	}

	virtual Setting *clone(){
		SettingMdisProjDir *setting = new SettingMdisProjDir();
		copy( setting );
		return setting;
	}

	// dont't show in Setting dialog
	virtual QWidget *settingsTabWidget( QWidget *parent ){
		UTIL_UNREF_PARAM(parent);
		return 0;
	}

	virtual bool validate( const QVariant &newVal, QString &errMsg ){
		QString expVal = OsLinuxConfiguration::hostSpecPath( lst->expandedValueOf( newVal.toString() ));
		bool rv;

		if( (rv = isGoodMdisProjDir( expVal, errMsg )) == false )
			errMsg = key() + ": " + errMsg;

		return rv;
	}

protected:
	QString infoText() {
		return "This directory is used to store the configuration that you "
			"configure with mdiswiz. The modules and binaries will be "
			"compiled into this directory before they are installed.";
	}
	QString labelText() {
		return "MDIS project directory";
	}
	QString wizPageTitle() {
		return "Select the project directory to store MDIS configuration";
	}
};

class SettingElinosProjDir : public OsLinuxDirectorySetting {
public:

	SettingElinosProjDir() :
		OsLinuxDirectorySetting( "ELINOS_PROJ_DIR",
						  DirectorySetting::Dir|
						  DirectorySetting::MustExist ) {
	}

	virtual Setting *clone(){
		SettingElinosProjDir *setting = new SettingElinosProjDir();
		copy( setting );
		return setting;
	}

	virtual bool validate( const QVariant &newVal, QString &errMsg ){

		QString expVal = OsLinuxConfiguration::hostSpecPath( lst->expandedValueOf( newVal.toString() ) );
		QStringList lines;
		
		// basic validation first
		if( DirectorySetting::validate( QVariant(expVal), errMsg ) == false )
			return false;

		// check if it's really an ELINOS project dir
		QFileInfo fi1( expVal + "/ELINOS.sh" );
		if( !fi1.exists() ){
			errMsg += key() + ": \"" + expVal +
				"\" does not look like an ELinOS project directory (" +
				fi1.filePath() + " not found)\n";
			return false;
		}

		// check if this is ElinOs 5.0 or higher
		QRegExp rx("ELINOS_VERSION=\"(\\d)\\.(\\d)");
		int majVers = 0;
		Util::fileGrep( expVal + "/ELINOS.sh", rx, &lines ); // ELINOS_VERSION="5.0"
		if ( lines.count() == 1 ) {
			rx.indexIn(lines.at(0));
			majVers = rx.cap(1).toInt();
		} else {
			wDebug(("ELINOS_VERSION was found unexpectedly often in ELINOS.sh"));
		}
		
		// only check for kernel.h if we have ElinOS version < 5.0
		if ( majVers < 5 ) {					
			// check if kernel is ok for MDIS
			if( isGoodKernelDir( expVal + "/linux", errMsg ) == false ){
				errMsg = key() + ": " + errMsg;
				return false;
			}
		}
		return true;
	}

protected:
	QString infoText() {
		return "This is the directory you have created with "
			"elinos-cloneproject. The MDIS project will be created "
			"in this directory in subdirectory src/mdis.";
	}
	QString labelText() {
		return "ELinOS project directory";
	}
	QString wizPageTitle() {
		return "Select the ELinOS project directory";
	}
};


class SettingTargetTree : public OsLinuxDirectorySetting {
public:

	SettingTargetTree() :
		OsLinuxDirectorySetting( "TARGET_TREE",
						  DirectorySetting::Dir ) {
	}

	virtual Setting *clone(){
		SettingTargetTree *setting = new SettingTargetTree();
		copy( setting );
		return setting;
	}

	virtual bool validate( const QVariant &newVal, QString &errMsg ){

		QString expVal = OsLinuxConfiguration::hostSpecPath( lst->expandedValueOf( newVal.toString() ) );

		// basic validation
		if( DirectorySetting::validate( QVariant(expVal), errMsg ) == false )
			return false;

		return true;
	}
protected:
	QString infoText() {
		return "All binaries (modules, programs and libraries) will "
			"be installed under this directory.";
	}
	QString labelText() {
		return "Target root tree";
	}
	QString wizPageTitle() {
		return "Select your target's filesystem root directory";
	}
};


class SettingDbg : public MultipleChoiseSetting {
public:
	enum Choises { Nodbg=0, Dbg };

	SettingDbg() : MultipleChoiseSetting( "ALL_DBGS" ) {
		choises << "nodbg" << "dbg";

		choisesHelp <<
			"Do not include debug strings. Run at full speed." <<

			"Include debug strings. Modules run slower.";

		_value = QVariant(choises[Dbg]);
	}

	virtual Setting *clone(){
		SettingDbg *setting = new SettingDbg();
		copy( setting );
		return setting;
	}

protected:
	QString infoText() {
		return "Defines whether to build and install the release (nodbg) or "
		"debug (dbg) versions of the kernel modules. The debug version "
		"of the modules issue many debug messages using printk's for "
 	    "trouble shooting";
	}
	QString labelText() {
		return "Build mode for kernel modules";
	}
	QString wizPageTitle() {
		return "Select build mode for kernel modules";
	}
};

class SettingModsInstallDir : public OsLinuxDirectorySetting {
public:

	SettingModsInstallDir() :
		OsLinuxDirectorySetting( "MODS_INSTALL_DIR",
						  DirectorySetting::Dir ) {
	}

	virtual Setting *clone(){
		SettingModsInstallDir *setting = new SettingModsInstallDir();
		copy( setting );
		return setting;
	}
protected:
	QString infoText() {
		return "The directory in which the kernel modules are to be installed."
		" Usually this is the target's /lib/modules/$(LINUX_VERSION)/misc "
	    "directory.";
	}
	QString labelText() {
		return "Installation directory for kernel modules";
	}
	QString wizPageTitle() {
		return "Select installation directory for kernel modules";
	}
};

class SettingBinInstallDir : public OsLinuxDirectorySetting {
public:

	SettingBinInstallDir() :
		OsLinuxDirectorySetting( "BIN_INSTALL_DIR",
						  DirectorySetting::Dir ) {
	}

	virtual Setting *clone(){
		SettingBinInstallDir *setting = new SettingBinInstallDir();
		copy( setting );
		return setting;
	}
protected:
	QString infoText() {
		return "The directory in which the user state programs are to be "
			"installed. Often something like /usr/local/bin. "
			"(relative to the target's root tree)";
	}
	QString labelText() {
		return "Installation directory for user state applications";
	}
	QString wizPageTitle() {
		return labelText();
	}
};

class SettingLibInstallDir : public OsLinuxDirectorySetting {
public:

	SettingLibInstallDir() :
		OsLinuxDirectorySetting( "LIB_INSTALL_DIR",
						  DirectorySetting::Dir ) {
	}

	virtual Setting *clone(){
		SettingLibInstallDir *setting = new SettingLibInstallDir();
		copy( setting );
		return setting;
	}
protected:
	QString infoText() {
		return "The directory in which the shared (.so) "
			"user mode libraries are to "
			"be installed. Often something like /usr/local/lib. "
			"(relative to the target's root tree)";
	}
	QString labelText() {
		return "Installation directory for shared user mode libraries";
	}
	QString wizPageTitle() {
		return labelText();
	}
};

class SettingStaticLibInstallDir : public OsLinuxDirectorySetting {
public:

	SettingStaticLibInstallDir() :
		OsLinuxDirectorySetting( "STATIC_LIB_INSTALL_DIR",
						  DirectorySetting::Dir ) {
	}

	virtual Setting *clone(){
		SettingStaticLibInstallDir *setting = new SettingStaticLibInstallDir();
		copy( setting );
		return setting;
	}
protected:
	QString infoText() {
		return "The directory in which the static user mode libraries are to "
			"be installed. Often something like /usr/local/lib on "
			"development host. For cross compilation select a path "
			"relative to your cross compilers lib directory.";
	}
	QString labelText() {
		return "Installation directory for static user mode libraries";
	}
	QString wizPageTitle() {
		return labelText();
	}
};

class SettingLibMode : public MultipleChoiseSetting {
public:
	enum Choises { Static=0, Shared };

	SettingLibMode() : MultipleChoiseSetting( "LIB_MODE" ) {
		choises << "static" << "shared";
		choisesHelp <<
			"Build static libraries and link programs statically. "
			"No library installation required on target." <<

			"Build shared libraries. These must be installed on the target.";

		_value = QVariant(choises[Static]);
	}

	virtual Setting *clone(){
		SettingLibMode *setting = new SettingLibMode();
		copy( setting );
		return setting;
	}
protected:
	QString infoText() {
		return "Define whether to build/use static or shared user state "
			"libraries. "
			"In \"static\" mode, libraries are statically linked to programs. "
			"In \"shared\" mode, programs dynamically link to the libraries. "
			"\"shared\" mode makes programs smaller but requires installation "
			"of shared libraries on the target";
	}
	QString labelText() {
		return "Library mode";
	}
	QString wizPageTitle() {
		return labelText();
	}
};

class SettingLinUsrIncDir : public OsLinuxDirectorySetting {
public:

	SettingLinUsrIncDir() :
		OsLinuxDirectorySetting( "LIN_USR_INC_DIR",
						  DirectorySetting::Dir |
						  DirectorySetting::MustExist) {
	}

	virtual Setting *clone(){
		SettingLinUsrIncDir *setting = new SettingLinUsrIncDir();
		copy( setting );
		return setting;
	}
	virtual bool validate( const QVariant &newVal, QString &errMsg ){
		QString expVal = lst->expandedValueOf( newVal.toString() );

		// if empty, assume this is intended
		if( expVal == QString() || expVal == "")
			return true;

		// basic validation first
		if( DirectorySetting::validate( newVal, errMsg ) == false )
			return false;

		// check if it's really a include dir...
		QFileInfo fi1( expVal + "/stdio.h" );
		if( !fi1.exists() ){
			errMsg += key() + ": \"" + expVal +
				"\" does not look like a include dir (" +
				fi1.filePath() + " not found)\n";
			return false;
		}

		return true;
	}
protected:

	QString infoText() {
		return "The include directory used when building user mode "
			"libraries and "
			"applications. If you're doing selfhosted development, "
			"it's typically /usr/include. "
			"If you're doing cross development, select the include directory "
			"of your cross compiler. Leave it blank if your compiler doesn't "
			"need this setting.";
	}
	QString labelText() {
		return "User State Include Directory";
	}
	QString wizPageTitle() {
		return labelText();
	}
};


class SettingRtaiSupport : public MultipleChoiseSetting {
public:
	enum Choises { No=0, Yes };

	SettingRtaiSupport() : MultipleChoiseSetting( "MDIS_SUPPORT_RTAI" ) {
		choises << "no" << "yes";

		choisesHelp <<
			"MDIS RTAI support turned off." <<

			"Compile MDIS to support RTAI and standard Linux.";

		_value = QVariant(choises[Yes]);
	}

	virtual Setting *clone(){
		SettingRtaiSupport *setting = new SettingRtaiSupport();
		copy( setting );
		return setting;
	}

protected:
	QString infoText() {
		return "Defines whether to build MDIS to support RTAI. "
		"If enabled, MDIS modules support RTAI in addition to the "
		"standard Linux mode. Set it to \"yes\" if you want "
 	    "to access MDIS devices from RTAI applications";
	}
	QString labelText() {
		return "Enable MDIS RTAI support";
	}
	QString wizPageTitle() {
		return "Select MDIS RTAI support";
	}
};


//----------------------------------------------------------------------
//! initial action dialog
/*!
  Initial dialog to let user decide what to do
  (open existing/create new project).

  Bring The dialog with exec(). If it returns \c QDialog::Accepted,
  you can retrieve the choosen action by calling the action() method.

*/
class OsLinuxInitialDialog : public QDialog {
public:
	//! creates the add component dialog
	OsLinuxInitialDialog(QWidget *parent,
						 const char *name=0,
						 bool modal=TRUE,
						 Qt::WFlags f=0) : QDialog(parent,name,modal,f)
	{
		Q3VBoxLayout* vb = new Q3VBoxLayout(this,10);
		QPushButton *but;
		QLabel *lab;

		lab = new QLabel( "<b>Welcome to the MDIS Configuration Wizard!</b>",
						  this );
		vb->addWidget( lab );
		
		lab = new QLabel("This tool allows you to configure MDIS devices easily.\n", this );
		vb->addWidget( lab );

		lab = new QLabel(
			"<font color=\"red\">To generate an initial config (scan system) click Cancel and use button \"scan\" from the toolbar.\n</font>", this );
		vb->addWidget( lab );
		
		lab = new QLabel( "Please choose your initial action:", this );
		vb->addWidget( lab );

	    buttonGroup = new Q3ButtonGroup( 1, Qt::Horizontal,
										QString::null, this );

		new QRadioButton( "Create new project", buttonGroup );
		new QRadioButton( "Open existing project", buttonGroup );
		// new QRadioButton( "Scan system (Selfhosted project)", buttonGroup );

		buttonGroup->setButton(0);

		vb->addWidget( buttonGroup );

		Q3HBoxLayout *butHb = new Q3HBoxLayout();
		vb->addLayout( butHb );

		but = new QPushButton("OK", this );
		connect( but, SIGNAL(clicked()), this, SLOT(accept()));
		butHb->addStretch(1);
		butHb->addWidget( but );
		butHb->addStretch(1);
		but = new QPushButton("Cancel", this );
		butHb->addWidget( but );
		connect( but, SIGNAL(clicked()), this, SLOT(reject()));
		butHb->addStretch(1);
	}

	//! get the selected action
	/*!
	  \return 0=create new, 1=open existing, 2=scan system
	*/
	int action() { return buttonGroup->id( buttonGroup->selected()); }

private:
	Q3ButtonGroup *buttonGroup;
};


//----------------------------------------------------------------------
//! creates OsLinux object
/*!
*/
OsLinux::OsLinux() : TargetOs( OsFactory::Linux )
{
	// check if environment variable MEN_LIN_DIR is present
	menLinuxPath = getenv("MEN_LIN_DIR");
	if( menLinuxPath.isEmpty() )
		menLinuxPath = OsLinuxConfiguration::hostSpecPath(defMenLinuxPath);
}

Configuration *
OsLinux::createConfiguration()
{
	return new OsLinuxConfiguration;
}

QString
OsLinux::pkgDbPath()
{
	return OsLinuxConfiguration::hostSpecPath(menLinuxPath + "/" + packageDbPath);
}

//! perform initial, OS-specific init
/*!
  This routine performs the following steps:
  - Init the package data base
  - Checks if a
  - Checks if started from an ELINOS project or if ELINOS_PROJECT environment
    is set. If so attempt to open $ELINOS_PROJECT/src/mdis project.
  - Checks if it was started from a directory containg a MDIS project. If so,
    open this configuration.
  - Bring up the initial dialogs

  \return true if ok, false if fatal error (abort mdiswiz)
*/
bool
OsLinux::startup( int argc, char **argv )
{
	extern MdisModelFactory *G_mdisMods;
	extern MdiswizView *G_mdiswiz;
	bool success;
	QString errMsg;
	QString elinosDir, mdisDir = QDir::currentDirPath();
	QString elinosProject;

	// Add linux SW module paths and remove unused types
	G_swModTypes->types.remove( SwModuleTypes::Other );
	G_swModTypes->types.remove( SwModuleTypes::CbDriver );

	// must create a temporary configuration, it is immediately
	// destroyed by MdiswizView::loadConfig() or newConfig()
	OsLinuxConfiguration *newCfg = new OsLinuxConfiguration;;
	MAIN_SetCurrentCfgDeleteLast( newCfg );

	// parse the model data base (ignore errors)
	PackageDb pkgdb( pkgDbPath() );
	pkgdb.parse( G_mdisMods );

	// check if project dir given on command line
	if( argc > 1 )
		mdisDir = argv[1];

	// check if started from a ELINOS project dir
	elinosDir = mdisDir;
	if( OsLinuxConfiguration::isElinosDir( elinosDir ) )
		OsLinuxConfiguration::sourceElinosSh( elinosDir );
	else {
		elinosDir = mdisDir + "/..";
		if( OsLinuxConfiguration::isElinosDir( elinosDir ) )
			OsLinuxConfiguration::sourceElinosSh( elinosDir );
		else {
			elinosDir = mdisDir + "/../..";
			if( OsLinuxConfiguration::isElinosDir( elinosDir ) )
				OsLinuxConfiguration::sourceElinosSh( elinosDir );
		}
	}

	// check if environment variable ELINOS_PROJECT exists
	elinosProject = OsLinuxConfiguration::elinosProjectEnv();
	if( !elinosProject.isEmpty() ){
		wDebug(("Found ELINOS_PROJECT = %s", (const char *)elinosProject ));
		mdisDir = elinosProject + "/" + elinosSubPath;
		elinosRootPath = OsLinuxConfiguration::elinosRootEnv();
	}

	// check if started from a MDIS project dir
	QFileInfo sysdesc( mdisDir + "/" + sysDescName);
	QFileInfo makefile( mdisDir + "/" + makeName);

	if( sysdesc.exists() && makefile.exists() ){
		return G_mdiswiz->loadConfig( mdisDir );
	}

	// bring up initial dialog (create new/open existing)

	OsLinuxInitialDialog dlg(G_mainWindow);
	if( dlg.exec() == QDialog::Accepted ){
		switch ( dlg.action() ) {

		case 0: // create new
			success = G_mdiswiz->newConfig();
			if( !success )
				return false;
			break;
		case 1: // open existing config (ask user for a directory)
			success = G_mdiswiz->loadConfig(0);
			if( !success )
				return false;
			break;
		case 2: // scan the system and propose an initial system.dsc/Makefile
			break;
		default:
			return false;
		}
	}
	
	else
		return false; // users wants to leave

	return true;
}


QString
OsLinux::driverName( QString plainName, Device *dev )
{
	QString name = "men_";

	switch( dev->type() ){
	case Device::Mdis:
		MdisDevice *mdev;
		WIZ_DYNAMIC_CAST( dev, mdev, MdisDevice *);

		if( !mdev->isNativeDevice() )
			name += "ll_";
		break;
	case Device::Bbis:
		name += "bb_"; break;
	default:
		return QString();
	}

	name += plainName;
	return name.lower();
}

QString
OsLinux::mdiswizVersion()
{
	return MdiswizView::Version() + "-" + mdiswizLinuxVersion;
}

QString
OsLinux::helpPath()
{
	return menLinuxPath + "/MDIS_DOCS/mdis.html";
}

QString
OsLinux::helpEnv()
{
	return
		" MEN_LIN_DIR (optional): override /opt/menlinux\n"
		" ELINOS_PROJECT (optional): if set, start mdiswiz for that project"
		;
}

bool
OsLinux::installDlg()
{
	Installer installer( menLinuxPath );

	return installer.guiInstall( G_mainWindow );
}

bool
OsLinux::batchInstall( QString pkgFile, QString &errMsg )
{
	Installer installer( menLinuxPath );

	return installer.batchInstall( pkgFile, errMsg );
}

//----------------------------------------------------------------------

//! creates the OsLinuxConfiguration object
OsLinuxConfiguration::OsLinuxConfiguration() :
	Configuration()
{
	kernelArch = CpuDevice::UnsetArch;

	// create the settings
	lstSettings.append( settingCdk 					= new SettingCdk() );
	lstSettings.append( settingMdisProjDir 			= new SettingMdisProjDir() );
	lstSettings.append( settingElinosProjDir 		= new SettingElinosProjDir() );
	lstSettings.append( settingTargetTree 			= new SettingTargetTree() );
	lstSettings.append( settingLinKernelDir 		= new SettingLinKernelDir() );
	lstSettings.append( settingRtaiSupport 			= new SettingRtaiSupport() );
	lstSettings.append( settingRtaiDir 				= new SettingRtaiDir() );
	lstSettings.append( settingLinUsrIncDir 		= new SettingLinUsrIncDir() );
	lstSettings.append( settingLibMode 				= new SettingLibMode() );
	lstSettings.append( settingDbg 					= new SettingDbg() );
	lstSettings.append( settingModsInstallDir 		= new SettingModsInstallDir() );
	lstSettings.append( settingBinInstallDir 		= new SettingBinInstallDir() );
	lstSettings.append( settingLibInstallDir 		= new SettingLibInstallDir() );
	lstSettings.append( settingStaticLibInstallDir 	= new SettingStaticLibInstallDir());
	lstSettings.append( settingLinuxVersion 		= new Setting("LINUX_VERSION"));
	lstSettings.append( settingDescInstallDir 		= new OsLinuxDirectorySetting( "DESC_INSTALL_DIR",
						 													 	   DirectorySetting::Dir ));
	lstSettings.append( settingDevNodeInstallDir 	= new OsLinuxDirectorySetting( "DEVNODE_INSTALL_DIR",
						 														   DirectorySetting::Dir));
	lstSettings.connectSettingsToList();

	settingMdisProjDir->setPersist( false );
	settingElinosProjDir->setPersist( false );
	settingLinuxVersion->setPersist( false );

	// update LINUX_VERSION whenever LIN_KERNEL_DIR is changed
	connect( settingLinKernelDir, SIGNAL( valueChanged( const QVariant & )),
			 this, SLOT( linKernelDirChanged( const QVariant & )));

	// set defaults depending on CDK
	connect( settingCdk, SIGNAL( indexChanged(int)),
			 this, SLOT( cdkChanged(int)));

	// source ELINOS.sh when ELINOS_PROJECT_DIR changed
	connect( settingElinosProjDir, SIGNAL( valueChanged( const QVariant & )),
			 this, SLOT( elinosProjDirChanged( const QVariant & )));

	// set the configuration name when MDIS_PROJ_DIR is changed
	connect( settingMdisProjDir, SIGNAL( valueChanged( const QVariant & )),
			 this, SLOT( mdisProjDirChanged( const QVariant & )));

	// hide RTAI_DIR if MDIS_SUPPORT_RTAI is not defined
	connect( settingRtaiSupport, SIGNAL( valueChanged( const QVariant & )),
			 this, SLOT( rtaiSupportChanged( const QVariant & )));

	// select LIB_INSTALL_DIR or STATIC_LIB_INSTALL_DIR depending
	// on LIB_MODE
	connect( settingLibMode, SIGNAL( valueChanged( const QVariant & )),
			 this, SLOT( libModeChanged( const QVariant & )));

	// update properties when CPU is changed
	extern MdiswizView *G_mdiswiz;
	connect( G_mdiswiz, SIGNAL( sigCpuChanged( void )),
			 this, SLOT( menCpuChanged( void )));

	cdkChanged( settingCdk->index() );

}

Configuration *
OsLinuxConfiguration::create()
{
	return new OsLinuxConfiguration();
}

bool
OsLinuxConfiguration::save(QString &errMsg)
{
	bool rv1, rv2;

	if( cfgName.isEmpty() )
		return saveAs( errMsg );

	// prepare ELINOS project directory
	if( settingCdk->index() == SettingCdk::Elinos ){
		if( elinosCopyFixedFiles() == false ){
			errMsg = "Could not create fixed files for ELinOS project  \"" +
				cfgName + "\". Template files missing ?";
			return false;
		}
	}

	// create directory if necessary
	QDir cfgDir(cfgName);

	if( !cfgDir.exists() ){
		QDir newDir;
		if( newDir.mkdir( cfgName, true ) == false ){
			errMsg = "Could not create directory \"" + cfgName + "\"";
			return false;
		}
	}

	// save descriptor file
	rv1 = saveDesc( cfgName + "/" + sysDescName, errMsg );

	// save the Makefile
	rv2 = saveMakefile( errMsg );

	// change into dir
	if( (rv1 && rv2) && (QDir::setCurrent( cfgName ) == false) ){
		errMsg = "Cannot change into directory" + cfgName;
		rv1 = false;
	}
	
	/* check if pci domains are needed and supported */
	checkPciDomains();

	return rv1 && rv2;
}

bool
OsLinuxConfiguration::saveAs(QString &errMsg)
{
	QString newCfgName;

	newCfgName = selectConfigName("Choose Directory to save project to", false, errMsg );
	if( !newCfgName.isNull() ){
		// user selected a configuration name
		settingMdisProjDir->setValue( newCfgName );
		return save( errMsg );
	}
	return false;
}

bool
OsLinuxConfiguration::load( QString &errMsg, const QString &_cfgName )
{
	bool rv = true;
	QString newCfgName;

	// open an existing configuration
	if( _cfgName.isEmpty() ){
		// let user choose a configuration name
		newCfgName = selectConfigName( "Open project", true, errMsg );
		if( newCfgName.isEmpty() )
			return false;
	}
	else
		newCfgName = _cfgName;

	settingMdisProjDir->setValue( newCfgName );

	wDebug(("OsLinuxConfiguration::load: cfgName=%s", cfgName.latin1()));
	// change into the loaded project
	if( QDir::setCurrent( cfgName ) == false ){
		errMsg += QString("Cannot change into directory ") + cfgName +
			"\n";
		rv = false;
	}

	// save old files to *.bak
	wDebug(("OsLinuxConfiguration::load: backup %s and %s",
			sysDescName.latin1(), makeName.latin1()));
	{
		QFile f;

		f.setName( cfgName  + "/" + sysDescName + ".bak" );
		// try to remove old backup file
		if( f.exists() && !f.remove() ) {
			errMsg += QString("Cannot remove old backup file ") + f.name() + "\n\n";
			rv = false;
		} else {
			Util::fileCopy( QString( cfgName  + "/" + sysDescName ),
							QString( f.name() ),
							errMsg );
		}

		f.setName( cfgName  + "/" + makeName + ".bak" );
		// try to remove old backup file
		if( f.exists() && !f.remove() ) {
			errMsg += QString("Cannot remove old backup file ") + f.name() + "\n\n";
			rv = false;
		} else {
			Util::fileCopy( QString( cfgName  + "/" + makeName ),
							QString( f.name() ),
							errMsg );
		}
	}

	// load descriptor file
	if( rv == true ){
		rv = loadDesc( sysDescName, errMsg );
	}

	// during Makefile loading, cfgName may be modified (WIZ_CDK changed)
	// but I want to keep it. So save the current cfgName and restore
	// it afterwards
	QString orgCfgName = cfgName;

	// load Makefile
	if( rv == true ){
		setAllSwModulesDisabled();
		rv = loadMakefile( errMsg );
	}
	settingMdisProjDir->setValue( orgCfgName );

	/* check if pci domains are needed and supported */
	checkPciDomains();

	return rv;
}

QString
OsLinuxConfiguration::selectConfigName( const QString &action,
										bool mustExist, QString &errMsg )
{
    Q3FileDialog fd( G_mainWindow, "file dialog", TRUE );
	QString dirName = QString();

	fd.setCaption( action );

	if( mustExist ){
		fd.setMode( Q3FileDialog::ExistingFile );
		fd.setFilter( "MDIS wizard config files (system.dsc)" );

		if ( fd.exec() == QDialog::Accepted ){
			dirName = fd.selectedFile();
			dirName = dirName.section( '/', 0, -2 ); // get directory name
		}
	}
	else {
		fd.setMode( Q3FileDialog::DirectoryOnly );

		if ( fd.exec() == QDialog::Accepted ){
			dirName = fd.selectedFile();

			// check if selected dir is empty or non-existing
			if( isGoodMdisProjDir( dirName, errMsg ) == false ){
				dirName = QString();
			}
		}

	}

	wDebug(("OsLinux::selectConfigName dirName=%s", (const char *)dirName ));
	return dirName;
}

bool
OsLinuxConfiguration::loadMakefile( QString &errMsg )
{
	// Read in Makefile and convert it into MkFileTuples
	CfgMakefile mkFile( cfgName + "/" + makeName);

	Q3PtrList<MkFileTuple> *lst;
	bool elinosProj = false;

	try {
		lst = mkFile.read();
	}
	catch( CfgMakefile::Except &e ){
		errMsg += e.getMsg();
		return false;
	}

	bool gotSettingCdk = false;

	// process the scanned tuples
	MkFileTuple *tup;

	for( tup=lst->first(); tup; tup=lst->next() ){
		wDebug(("loadMakefile: tup=%s", (const char *)tup->key ));
		
		if ( ( tup->key == "ALL_CORE_LIBS" ) &&
			 ( !(tup->val.isEmpty()) && (tup->val[0] == "no") ) )
		{
			tup->val.clear();
		}
		if( (tup->tag & MkFileTuple::TypeMask) == MkFileTuple::Assignment ){
			bool isSwModule=false;
			SwModuleTypes::TypeMap::Iterator it;

			// check if this tuple describes a list of sw modules
			for( it = G_swModTypes->types.begin();
				 it != G_swModTypes->types.end(); ++it ){

				if( tup->key == swModuleMfString( it.key() ) ){
					enableSwMods( it.key(), tup, errMsg );
					isSwModule=true;
				}
			}

			if( !isSwModule ){
				Setting *setting;
				
				if( (setting = lstSettings.find( tup->key ))){
					
					if ( !tup->val.isEmpty() ) {
						// setting known, change it's value
						// can validate it to update the error messages
						setting->setValue( tup->val[0] );

						if ((tup->key != "LIN_KERNEL_DIR") || !elinosProj ) {
							setting->validate( tup->val[0], errMsg );
						}
					} else {
						setting->setValue( "" ); 
					}

					if( setting == settingCdk )
						gotSettingCdk = true;

					if ( tup->key == "WIZ_CDK" ) {
						if (tup->val[0] == "ELinOS" ) {
							elinosProj = true;
						}
					}
				}
				else {
					// unknown variable
					if( tup->key != "MEN_LIN_DIR" &&
						tup->key != "ALL_DESC" &&
						tup->key != "MDIS_MAJOR_NUMBER" &&
						tup->key != "MDIS_EXTRA_DEFS" ){
						errMsg += QString("Unknown makefile variable %1\n").
							arg(tup->key);
					}
				}
			}
		}
	}
	// check if all mantadory keys are present
	if( !gotSettingCdk ){
		errMsg += "This is not an mdiswiz Makefile (WIZ_CDK not found)\n";
		return false;
	}

	return true;
}


//
// save all settings and sw modules to the Makefile
//
bool
OsLinuxConfiguration::saveMakefile( QString &errMsg )
{
	Setting *setting;
	Q3PtrList<MkFileTuple> tupList;
	MkFileTuple *tup;

	// Build a list of tuples
	tupList.setAutoDelete(true);

	// file header
	tup = new MkFileTuple;
	tup->tag = MkFileTuple::Comment;
	tup->val <<
		"MDIS for Linux project makefile" <<
		QString("Generated by mdiswiz ") + targetOs()->mdiswizVersion() <<
		QDate::currentDate().toString(Qt::ISODate);
	tupList.append(tup);

	// generate:
	// ifndef MEN_LIN_DIR
	// MEN_LIN_DIR = /opt/menlinux
	// endif
	tup = new MkFileTuple;
	tup->tag = MkFileTuple::Ifndef;
	tup->key = "MEN_LIN_DIR";
	tupList.append(tup);

	tup = new MkFileTuple;
	tup->tag = MkFileTuple::Assignment;
	tup->key = "MEN_LIN_DIR";
	tup->val = QStringList(defMenLinuxPath);
	tupList.append(tup);

	tup = new MkFileTuple;
	tup->tag = MkFileTuple::Endif;
	tupList.append(tup);

	for( setting = lstSettings.first(); setting; setting=lstSettings.next() ){

		if( setting->persist() ){

			if( setting->infoText() != QString::null ){
				tup = new MkFileTuple;

				// add a comment for this setting

				tup->val = QStringList::split( "\n", Util::breakString(
					setting->infoText(), 60));
				tup->tag = MkFileTuple::Comment;

				tupList.append(tup);
			}

			tup = new MkFileTuple;
		    if( setting->value().toString() != QString::null ){
				// assignment
				tup->key = setting->key();
#ifdef _WIN32
				if( setting->key() == "LIN_KERNEL_DIR" 			||
					setting->key() == "MODS_INSTALL_DIR" 		||
					setting->key() == "BIN_INSTALL_DIR" 		||
					setting->key() == "LIB_INSTALL_DIR" 		||
					setting->key() == "STATIC_LIB_INSTALL_DIR" 	||
					setting->key() == "DESC_INSTALL_DIR" 		||
					setting->key() == "DEVNODE_INSTALL_DIR"){
					tup->val = QStringList(shellSpecPath(setting->value().toString()));
				} else
					tup->val = QStringList(setting->value().toString());
#else
				tup->val = setting->value().toStringList();
#endif
				tup->tag = MkFileTuple::Assignment;
			}
			else {
				tup->val = QStringList(setting->key());
				tup->tag = MkFileTuple::Comment;
			}
			tupList.append(tup);

		}
	}

	// check if VME4L core SW module is in configuration
	// if so, add CONFIG_MEN_VME_KERNELIF
	{
		SwModule *sw;
		if( (( sw= lstSwModules.find( SwModuleTypes::NativeDriver, "VME4L-CORE" ))
			&& sw->isEnabled() ) ||
		    (( sw= lstSwModules.find( SwModuleTypes::UserLib, "VME4L_API" ))
			&& sw->isEnabled()) )
		{

			tup = new MkFileTuple;
			tup->tag = MkFileTuple::Assignment;
			tup->key = "MDIS_EXTRA_DEFS";
			tup->val = QStringList("-DCONFIG_MEN_VME_KERNELIF");
			tupList.append(tup);
		}
	}


	// now build the sw module lists
	tupList.append( mkSwModuleTups( SwModuleTypes::LlDriver ));
	tupList.append( mkSwModuleTups( SwModuleTypes::BbDriver ));
	tupList.append( mkSwModuleTups( SwModuleTypes::UserLib ));
	MkFileTuple * coreTuple = mkSwModuleTups( SwModuleTypes::CoreLib );
	if ( coreTuple->val.empty() )
		coreTuple->val.append("no");
	tupList.append( coreTuple );
	//tupList.append( mkSwModuleTups( SwModuleTypes::CoreLib ));
	tupList.append( mkSwModuleTups( SwModuleTypes::LlTool ));
	tupList.append( mkSwModuleTups( SwModuleTypes::ComTool ));
	tupList.append( mkSwModuleTups( SwModuleTypes::NativeDriver ));
	tupList.append( mkSwModuleTups( SwModuleTypes::NativeLib ));
	tupList.append( mkSwModuleTups( SwModuleTypes::NativeTool ));

	if( settingRtaiSupport->index() == SettingRtaiSupport::Yes ){
		tupList.append( mkSwModuleTups( RtaiUserLib ));
		tupList.append( mkSwModuleTups( RtaiLlTool ));
		tupList.append( mkSwModuleTups( RtaiComTool ));
	}

	// file trailer
	tup = new MkFileTuple;
	tup->tag = MkFileTuple::Assignment;
	tup->key = "ALL_DESC";
	tup->val = QStringList(sysDescPlainName);
	tupList.append(tup);


	tup = new MkFileTuple;
	tup->key = "$(MEN_LIN_DIR)/BUILD/MDIS/TPL/rules.mak";
	tup->tag = MkFileTuple::Include;
	tupList.append(tup);

	CfgMakefile mkFile( cfgName + "/" + makeName);

	try {
		mkFile.write( &tupList );
	}
	catch( CfgMakefile::Except &e ){
		errMsg += e.getMsg();
		return false;
	}
	return true;
}

//----------------------------------------------------------------------
//! check if module can be used for RTAI
/*!
  We try to guess. If the SW module supports only Linux and no other OSes
  we assume that it is NOT ok for RTAI.

  \return true if ok for RTAI
*/
bool
OsLinuxConfiguration::swModOkForRtai( SwModule *swMod )
{
	if( swMod->supportsOs( OsFactory::Linux ) &&
		!swMod->supportsOs( OsFactory::Vxworks ))
		return false;
	return true;
}

void
OsLinuxConfiguration::addSwMod( SwModule *swMod )
{
	SwModule additionalMod(*swMod);
	bool addAdditional = false;

	wDebug(("OsLinuxConfiguration::addSwMod"));

	switch( swMod->modType() ){

	case SwModuleTypes::UserLib:
		additionalMod.setModType( RtaiUserLib );
		addAdditional = true;
		break;

	case SwModuleTypes::LlTool:
		additionalMod.setModType( RtaiLlTool );
		addAdditional = true;
		break;

	case SwModuleTypes::ComTool:
		additionalMod.setModType( RtaiComTool );
		addAdditional = true;
		break;
	default:
		break;
	}

	if( addAdditional && swModOkForRtai( swMod ))
		Configuration::addSwMod( &additionalMod );

	Configuration::addSwMod( swMod );
}

void
OsLinuxConfiguration::removeSwMod( SwModule *swMod )
{
	SwModule additionalMod(*swMod);
	bool rmAdditional = false;

	switch( swMod->modType() ){

	case SwModuleTypes::UserLib:
		additionalMod.setModType( RtaiUserLib );
		rmAdditional = true;
		break;

	case SwModuleTypes::LlTool:
		additionalMod.setModType( RtaiLlTool );
		rmAdditional = true;
		break;

	case SwModuleTypes::ComTool:
		additionalMod.setModType( RtaiComTool );
		rmAdditional = true;
		break;
	default:
		break;
	}

	if( rmAdditional && swModOkForRtai( swMod ))
		Configuration::removeSwMod( &additionalMod );

	Configuration::removeSwMod( swMod );
}

//
// make a MkFileTuple for all SwModules in lstSwModules matching the
// type modType
//
MkFileTuple *
OsLinuxConfiguration::mkSwModuleTups( int modType )
{
	SwModule *mod;
	MkFileTuple *tup = new MkFileTuple;
	QStringList lst;

	tup->key = swModuleMfString(modType);
	tup->tag = MkFileTuple::Assignment;

	for( mod=lstSwModules.first(); mod; mod=lstSwModules.next()){
		if( mod->modType() == modType && mod->isEnabled()) {
			const QStringList* makefileList = mod->makefilePath_all();
			for (int i=0; i<makefileList->size(); i++) {
				lst += makefileList->at(i);
			}
		}
	}
	tup->val = lst;
	return tup;
}

//! convert \a Sw module type code to a string as used in Makefile
QString
OsLinuxConfiguration::swModuleMfString( int key )
{
	switch( key ){
	case SwModuleTypes::LlDriver: 	return "ALL_LL_DRIVERS";
	case SwModuleTypes::BbDriver: 	return "ALL_BB_DRIVERS";
	case SwModuleTypes::UserLib: 	return "ALL_USR_LIBS";
	case SwModuleTypes::CoreLib:		return "ALL_CORE_LIBS";
	case SwModuleTypes::LlTool: 		return "ALL_LL_TOOLS";
	case SwModuleTypes::ComTool: 	return "ALL_COM_TOOLS";
	case SwModuleTypes::NativeDriver:return "ALL_NATIVE_DRIVERS";
	case SwModuleTypes::NativeTool:  return "ALL_NATIVE_TOOLS";
	case SwModuleTypes::NativeLib:   return "ALL_NATIVE_LIBS";
	case RtaiUserLib:			return "ALL_RTAIK_USR_LIBS";
	case RtaiLlTool:			return "ALL_RTAIK_LL_TOOLS";
	case RtaiComTool:			return "ALL_RTAIK_COM_TOOLS";
	default: 					return QString();
	}
}

//
// lookup all makefilePaths contained in <tup> within the current
// configuration's swModule list. If a SwModule exists with a matching
// module type and makefile path, set the state of this module to enabled
// If the makefilePath is unknown, add a warning to <errMsg>
void
OsLinuxConfiguration::enableSwMods( int modType, MkFileTuple *tup,
									QString &errMsg )
{
	UTIL_UNREF_PARAM( errMsg );

	for ( QStringList::Iterator it = tup->val.begin(); it != tup->val.end();
		  ++it ) {

		SwModule *mod = lstSwModules.findMake( modType, *it );
		if( mod )
			mod->setEnabled( true );
		else
			//wDebug(("No such SW module with makefile %1 (ignored)\n", arg(*it) ));
			;
	}
}

bool
OsLinuxConfiguration::settingsDlg()
{
	CfgSwModuleTab *modTab;
	OsLinuxTab *osTab;

	wDebug(("OsLinux::settingsDlg"));
	SettingsDlg dlg( G_mainWindow, this );
	osTab = new OsLinuxTab( this, G_mainWindow, 0);
	dlg.addTab( osTab, "Project Settings");
	dlg.connect( &dlg, SIGNAL(applyButtonPressed()), osTab,
				 SLOT(slotApply()));
	dlg.connect( &dlg, SIGNAL(cancelButtonPressed()), osTab,
				 SLOT(slotCancel()));


	modTab = new CfgSwModuleTab( G_mainWindow, 0,
								 this );

	dlg.addTab( modTab, "Software Modules");
	dlg.connect( &dlg, SIGNAL(applyButtonPressed()), modTab,
				 SLOT(slotApply()));
	dlg.setButtons();
	return (dlg.exec() == QDialog::Accepted );
}

bool
OsLinuxConfiguration::newProjectWizard( QWidget *parent,
										const char *name)
{
	OsLinuxNewProjectWizard wizard( this, parent, name );

	return wizard.exec();
}

//! set the matching value of all settings corresponding to the Cdk
/*!
  \param cdkIdx see SettingCdk::Choises
*/
void
OsLinuxConfiguration::cdkChanged( int cdkIdx )
{
	Setting *setting;
    char currentDir[256];
    QString unknownPath = "unknown_path";
	QString nullString;

	wDebug(("cdkChanged %d", cdkIdx ));

    if(  _getcwd( currentDir, sizeof(currentDir) ) )
        unknownPath = QString(currentDir);

	for( setting = lstSettings.first();
		 setting;
		 setting = lstSettings.next() )
	{
		setting->setEnabled( Setting::Enabled );
	}

	settingModsInstallDir->setEnabled(Setting::Disabled);
	settingElinosProjDir->setEnabled(Setting::Hide);

	switch( cdkIdx ){
	case SettingCdk::Selfhosted:
		settingMdisProjDir->	setValue( "$(HOME)/mymdisproj" );
		settingLinKernelDir->	setValue( "/usr/src/linux" );
		settingRtaiDir->		setValue( unknownPath );
		//settingLinUsrIncDir->	setValue( "/usr/include" );
		settingLinUsrIncDir->	setEnabled(Setting::Hide);
		settingModsInstallDir->	setValue("/lib/modules/$(LINUX_VERSION)/misc");
		settingBinInstallDir->	setValue( "/usr/local/bin");
		settingLibInstallDir->	setValue( "/usr/local/lib");
		settingStaticLibInstallDir->setValue( "/usr/local/lib");
		settingDbg->			setValue( "dbg");
		settingLibMode->		setValue( "shared");
		settingTargetTree->		setEnabled(Setting::Hide);
		settingDescInstallDir->	setValue( "/etc/mdis" );
		settingDevNodeInstallDir->setValue( "/dev" );

		break;
	case SettingCdk::Elinos:
		// in case user loads existing configuration, attempt to source
		// ELINOS.sh. This is called during loadMakefile after WIZ_CDK
		// has been read.
		{
			QString elinosProjDir = cfgName.section('/', 0, -3 ); //up two dirs
			sourceElinosSh( elinosProjDir );
		}

		// check if environment variable ELINOS_PREFIX is present
		elinosRootPath = elinosRootEnv();

		settingMdisProjDir->	setEnabled(Setting::Disabled);

		settingElinosProjDir->setValue( elinosProjectEnv().isEmpty() ?
			unknownPath : elinosProjectEnv() );
		settingElinosProjDir->setEnabled(Setting::Enabled);

		settingLinKernelDir->	setValue( "$(ELINOS_PROJECT)/linux" );
		settingLinKernelDir->	setEnabled(Setting::Disabled);
		settingRtaiDir->		setValue( nullString );
		settingRtaiDir->		setEnabled(Setting::Hide );
		settingLinUsrIncDir->	setValue( nullString );
		settingLinUsrIncDir->	setEnabled(Setting::Hide );
		settingModsInstallDir->	setValue( "$(ELINOS_PROJECT)/kernel.rootfs/lib/modules/$(LINUX_VERSION)/misc");
		settingBinInstallDir->	setValue( "$(ELINOS_PROJECT)/app.rootfs/usr/bin");
		settingLibInstallDir->	setValue( "$(ELINOS_PROJECT)/app.rootfs/usr/lib");
		settingDevNodeInstallDir->setValue( "$(ELINOS_PROJECT)/app.rootfs/dev" );
		settingStaticLibInstallDir->setValue( unknownPath );
		settingStaticLibInstallDir->setEnabled(Setting::Disabled);
		settingDbg->			setValue( "dbg");
		settingLibMode->		setValue( "shared");
		settingLibMode->		setEnabled(Setting::Disabled); // for now
		settingTargetTree->		setEnabled(Setting::Hide);
		settingDescInstallDir->	setValue( "$(ELINOS_PROJECT)/app.rootfs/etc/mdis" );
		break;
	default:
		settingMdisProjDir->	setValue( "$(HOME)/mymdisproj");
		settingLinKernelDir->	setValue( unknownPath );
		settingRtaiDir->		setValue( unknownPath );
		settingLinUsrIncDir->	setValue( unknownPath );
		settingModsInstallDir->	setValue( "$(TARGET_TREE)/lib/modules/$(LINUX_VERSION)/misc");
		settingBinInstallDir->	setValue( "$(TARGET_TREE)/usr/bin");
		settingLibInstallDir->	setValue( "$(TARGET_TREE)/usr/lib");
		settingStaticLibInstallDir->setValue( unknownPath );
		settingDbg->			setValue( "dbg");
		settingLibMode->		setValue( "static");
		settingTargetTree->		setValue( unknownPath );
		settingDescInstallDir->	setValue( "$(TARGET_TREE)/etc/mdis" );
		settingDevNodeInstallDir->setValue( "$(TARGET_TREE)/dev" );
		break;
	}

	QString info = "The directory in which the MDIS descriptors are to "
			"be installed. Often something like /etc/mdis."
			" (Relative to the targets root tree)";
	settingDescInstallDir->setInfoText( &info );

	info = "The directory in which the MDIS device nodes are to "
			"be installed. Often something like /dev."
			" (Relative to the targets root tree)";
	settingDevNodeInstallDir->setInfoText( &info );

}

//! called whenever \c LIN_KERNEL_DIR is changed
/*!
  Re-evaluates the \c LINUX_VERSION from \c linux/version.h
  and CONFIG_DEVFS_FS, CONFIG_RTHAL from \c linux/autoconf.h.

  Also checks the used architecture

  \param newVal new value of LIN_KERNEL_DIR
*/
void
OsLinuxConfiguration::linKernelDirChanged( const QVariant &newVal )
{
	QString incDir = newVal.toString();
	QStringList lines;
	wDebug(( "linKernelDirChanged %s", (const char *)incDir ));

	QString expVal = lstSettings.expandedValueOf( newVal.toString());

	// read version.h / utsrelease.h and extract value of UTS_RELEASE
	Util::fileGrep( expVal + "/include/linux/version.h",
					QRegExp("REL"), &lines );
					
	// for kernels >= 2.6.18, UTS_RELEASE was moved to utsrelease.h				
	if ( lines.count() == 0 ) {
		Util::fileGrep( expVal + "/include/linux/utsrelease.h",
					    QRegExp("REL"), &lines );
	}
	
	if( lines.count() == 1 ){
		QString line = lines[0];
		// break up words
		QStringList words = QStringList::split( QRegExp("[ \\t]+"), line );
		if( words.count() == 3 ){
			QString version = words[2];
			if( version.left(1) == "\"" && version.right(1) == "\"" )
				version = version.mid( 1, version.length()-2 );
			wDebug(("linKernelDirChanged version now %s",
				   (const char *)version));

			settingLinuxVersion->setValue( version );
		}
	}

	// set autoconf path
	setAutoConfDir( expVal );
	
	// check several options in autoconf.h
	bool hasConfigDevFsFs = isAutoConfOptionSet( expVal, "CONFIG_DEVFS_FS" );
	wDebug(("hasConfigDevFsFs=%d", hasConfigDevFsFs ));

	bool hasRtai = 0;

	wDebug(("hasRtai = %d", hasRtai ));

	// enable/disable RTAI_DIR/MDIS_SUPPORT_RTAI
	// widget depending on CONFIG_RTHAL _or_ the new ADEOS (RTAI3.x)
	if( hasRtai ){
		if( settingRtaiSupport->enabled() != Setting::Enabled ) {
			settingRtaiSupport->setEnabled( Setting::Enabled );
			settingRtaiSupport->setValue( "yes" );
		}
	}
	else {
		settingRtaiSupport->setEnabled( Setting::Disabled );
		settingRtaiSupport->setValue( "no" );
	}

	// get used CPU architecture
	kernelArch = getArch( expVal );

}

//! called whenever \c MDIS_PROJ_DIR is changed
/*!
  sets the configurations name to the new value
  \param newVal new value of MDIS_PROJ_DIR
*/
void
OsLinuxConfiguration::mdisProjDirChanged( const QVariant &newVal )
{
	QString projDir = newVal.toString();

	QString expVal = lstSettings.expandedValueOf( newVal.toString());
	wDebug(( "MdisProjDirChanged %s", (const char *)expVal ));

	cfgName = expVal;
}

//! called whenever \c ELINOS_PROJ_DIR is changed
/*!
  - sources ELINOS.sh
  - sets MDIS_PROJ_DIR to \c $ELINOS_PROJ_DIR/src/mdis

  \param newVal new value of ELINOS_PROJ_DIR
*/
void
OsLinuxConfiguration::elinosProjDirChanged( const QVariant &newVal )
{
	// ignore this if ELINOS_PROJ_DIR is not enabled
	if( settingElinosProjDir->enabled() != Setting::Enabled )
		return;

	QString expVal = hostSpecPath( lstSettings.expandedValueOf( newVal.toString() ));
	wDebug(( "ElinosProjDirChanged %s", (const char *)expVal ));

	settingMdisProjDir->setValue( expVal + "/" + elinosSubPath );
	sourceElinosSh( expVal );
	settingLinKernelDir->setValue( settingLinKernelDir->value() );

	elinosRootPath = elinosRootEnv();
}

//! called whenever \c MDIS_SUPPORT_RTAI is changed
/*!
  hides RTAI_DIR and RTAI Sw modules if MDIS_SUPPORT_RTAI is not set

  \param newVal new value of MDIS_SUPPORT_RTAI
*/
void
OsLinuxConfiguration::rtaiSupportChanged( const QVariant &newVal )
{
	UTIL_UNREF_PARAM(newVal);

	wDebug(("rtaiSupportChanged %d\n", settingRtaiSupport->index() ));
	if( settingRtaiSupport->index() == SettingRtaiSupport::Yes ){
		if( settingCdk->index() != SettingCdk::Elinos )
			settingRtaiDir->setEnabled( Setting::Enabled );

		G_swModTypes->types[OsLinuxConfiguration::RtaiUserLib] =
			SwModuleType( "RTAI Kernel Mode User Library",
						  "LIBSRC", "LIBSRC" );
		G_swModTypes->types[OsLinuxConfiguration::RtaiLlTool] =
			SwModuleType( "RTAI Kernel Mode Driver Specific Tool",
						  "DRIVERS/MDIS_LL", "DRIVERS/MDIS_LL" );
		G_swModTypes->types[OsLinuxConfiguration::RtaiComTool] =
			SwModuleType( "RTAI Kernel Mode Common Tool",
						  "TOOLS", "TOOLS" );
	}
	else {
		settingRtaiDir->setEnabled( Setting::Hide );
		settingRtaiDir->setValue( QString() );
		G_swModTypes->types.remove( OsLinuxConfiguration::RtaiUserLib );
		G_swModTypes->types.remove( OsLinuxConfiguration::RtaiLlTool );
		G_swModTypes->types.remove( OsLinuxConfiguration::RtaiComTool );
	}
}

//! called whenever \c LIB_MODE is changed
/*!
  selects either LIB_INSTALL_DIR or STATIC_LIB_INSTALL_DIR
*/
void
OsLinuxConfiguration::libModeChanged( const QVariant &newVal )
{
	UTIL_UNREF_PARAM(newVal);
	wDebug(("libModeChanged %d\n", settingLibMode->index() ));
	if( settingLibMode->index() == SettingLibMode::Static ){
		settingLibInstallDir->setEnabled( Setting::Disabled );
		settingStaticLibInstallDir->setEnabled( Setting::Enabled );
	}
	else {
		settingLibInstallDir->setEnabled( Setting::Enabled );
		settingStaticLibInstallDir->setEnabled( Setting::Disabled );
	}
}

void OsLinuxConfiguration::menCpuChanged( )
{
	wDebug(("menCpuChanged"));

	// check if pci Domains are supported
	checkPciDomains();
}

//! check if \a option is defined in \c include/linux/autoconf.h
/*!
  \return true if option is defined,
          false if option is undefined, not found or autoconf can't be
		  opened
*/
bool
OsLinuxConfiguration::isAutoConfOptionSet( const QString linKernelDir,
										   const QString &option )
{
	QStringList lines, words;
	QString line;
	
	if ( autoconfPath.isEmpty() ) {
		wDebug(("path is empty"));
		return false;
	}

	Util::fileGrep( autoconfPath, QRegExp( QString("^#.*")+option), &lines );

	if( lines.count() ){
		// split line according to whitespace
		words = QStringList::split( QRegExp("[ \\t]+"), lines[0] );
		if( words[0] == "#define" && words[1] == option )
			return true;
	}
	return false;
}

//! check if \a dir is an ELINOS project root directory
/*!
  Checks if ELINOS.sh can be found in \a dir
*/
bool
OsLinuxConfiguration::isElinosDir( const QString &dir )
{
	QFileInfo elinosSh( dir + "/ELINOS.sh");
	return elinosSh.exists();
}

//! Setup environment variables from ELINOS.sh
/*!
  This reads ELINOS.sh from \a dir and sets up environment variables
  accordingly. It also modifies the PATH environment variable like
  it is done in ELINOS.sh

  \param dir ELINOS project root directory
  \return false if ELINOS.sh could not be found\n
          true if ok
*/
bool
OsLinuxConfiguration::sourceElinosSh( const QString &dir )
{
#ifdef _WIN32 /* Windows host, execute ELINOS.bat */
	/* CS 05.10.2007 TBD */
	/* for some undefined reason setEnvironmentVariable() does not work
	 * therefore the MDIS wizard for Linux cross development under Windows
	 * may only be run after ELINOS.sh was sourced from the shell
	 * before starting the MDIS Wizard!!!
	 * Neither sourcing ELINOS.sh the old way, nor running ELINOS.bat did work */
  #if 0
	Q3Process *proc = new Q3Process();	/* external program */
	QStringList args;
	QString procOutput = QString::null;	/* external program output */
	QString batchFile = hostSpecPath( dir + "/ELINOS.bat" );

	wDebug(("sourceElinosSh in %s", (const char *)dir ));

	args << batchFile;
	proc->setArguments( args );

	if( ! proc->start() ){
		wDebug(( "sourceElinosSh: Can't run \"%s\"\n", (const char *)args[0] ));
	} else {
		wDebug(("sourceElinosSh: Executing %s\n", (const char *)args[0] ));
		while( proc->isRunning() ){
			if( G_qApp )
				G_qApp->processEvents();
		}

		if( !proc->normalExit() || proc->exitStatus() != 0 ){
			wDebug(("sourceElinosSh: failed to execute %s: exitStatus=%x\n", (const char *)args[0], proc->exitStatus() ));
			return false;
		}
	}

	#ifdef TEST_SETENVIRONMENTVARIABLE
		char *env;
		QString rv = QString::null;
		SetEnvironmentVariable( (const TCHAR *)"ELINOS_PROJECT",
								(const TCHAR *)"s:/work/LINUX_MDISWIZ/EL_PROJECTS/EM1/NFSroot");
		env = getenv("ELINOS_PROJECT");
		rv = env ? hostSpecPath(QString(env)) : QString();
		wDebug(("sourceElinosSh: ELINOS_PROJECT: %s\n", rv.latin1()));
	#endif /* TEST_SETENVIRONMENTVARIABLE */
  #endif
  UTIL_UNREF_PARAM(dir);
#else /* others,source ELINOS.sh */
	QStringList lines, words;

	wDebug(("sourceElinosSh in %s", (const char *)dir ));

	if( Util::fileGrep( dir + "/ELINOS.sh",
						QRegExp("^[A-Z_]*="), &lines ) == false )
		return false;

	for ( QStringList::Iterator it = lines.begin(); it != lines.end(); ++it ) {
		words = QStringList::split( "=", *it );

		if( words.count() == 2 ){
			QString var = words[0];
			QString val = words[1];

			// remove enclosing "
			if( val.startsWith("\"") && val.endsWith("\""))
				val = val.mid( 1, val.length()-2 );

			wDebug(("sourceElinosSh %s = %s", (const char *)var,
				   (const char *)val ));

			// set environment variable
			Util::setEnv( var, val );

			// add $(ELINOS_PREFIX)/bin and $(ELINOS_CDK)/bin to path
			if( (var == "ELINOS_PREFIX") || (var == "ELINOS_CDK") )
				Util::addPath( "PATH", val + "/bin" );
		}
    }
#endif
	return true;
}

//! get value of ELINOS_PROJECT environment variable
/*!
  \return value of $(ELINOS_PROJECT) or QString::null
 */
QString
OsLinuxConfiguration::elinosProjectEnv()
{
	char *env = getenv("ELINOS_PROJECT");
	QString rv = env ? hostSpecPath(QString(env)) : QString();
	wDebug(("elinosProjectEnv: %s\n", rv.latin1()));
	return rv;
}

//! get value of ELINOS_PREFIX environment variable
/*!
  \return value of $(ELINOS_PREFIX) or QString::null
 */
QString
OsLinuxConfiguration::elinosRootEnv()
{
	char *env = getenv("ELINOS_PREFIX");
	QString rv = env ? hostSpecPath(QString(env)) : QString();
	wDebug(("elinosRootEnv: %s\n", rv.latin1()));
	return rv;
}

//! copy fixed MDIS files for ELINOS project
/*!
  executes /opt/menlinux/BUILD/MDIS/TPL/mkElinosFiles to copy
  autonode.sh and src/mdis/Makefile.elinos into the elinos project directory

  The elinos project dir is taken from environment var $ELINOS_PROJECT ???

  Silently overwrites existing files

  \return true if ok, false if not
*/
bool
OsLinuxConfiguration::elinosCopyFixedFiles()
{
	QString elinosProjDir = settingElinosProjDir->value().asString();
	QStringList args;
	QString batchFile = hostSpecPath( menLinuxPath + "/BUILD/MDIS/TPL/mkElinosFiles" );
	WIZ_ASSERT( !elinosProjDir.isEmpty() );

	wDebug(("elinosCopyFixedFiles %s", (const char *)elinosProjDir ));

	args << "sh" << batchFile
		 << elinosProjDir;

	Q3Process proc( args );

	if( ! proc.start() ){
		wDebug(("*** elinosCopyFixedFiles cannot start"));
		return false;
	}

	while( proc.isRunning() ){
		G_qApp->processEvents();
	}

	if( !proc.normalExit() )
		return false;
	if( proc.exitStatus() != 0 )
		return false;
	return true;
}

bool
OsLinuxConfiguration::validateSwModule(
	SwModule *mod,
	QString &errMsg,
	bool disableIfInvalid )
{
	int rv = true;

	// no check if module is not enabled
	if( !mod->isEnabled() )
		return rv;

	if( G_swModTypes->types.contains( mod->modType() )){
		
		// check if the swmodule's makefile exists
		const QStringList* makeNameList = mod->makefilePath_all();
		
		for ( int i=0; i<makeNameList->size(); i++ )
		{
			QString makeName = hostSpecPath( menLinuxPath + "/" +
							  G_swModTypes->types[mod->modType()].natPath + "/" +
							  makeNameList->at(i) );
	
			QFileInfo fi(  makeName );
	
			wDebug(("validateSwModule %s", (const char *)makeName));
			if( ! fi.exists() ){
				errMsg += "Source code for \"" + mod->modName() +
					"\" not installed\n"
					"(" + makeName + ") doesn't exist.\n";
	
				// disable if invalid
				if( disableIfInvalid )
					mod->setEnabled( false );
				rv = false;
				break;
			}
		}
	}
	else {
		wDebug(("*** validateSwModule: unknown module type 0x%x",
			   mod->modType()));
	}

	return rv;
}

bool
OsLinuxConfiguration::cpuOkForCfg( CpuDevice *cpu )
{
	if( cpu->getArch() == CpuDevice::UnsetArch )
		return false;			// extension boards...

	// unknown arch in kernel, allow any CPU to attach
	if( kernelArch == CpuDevice::UnsetArch )
		return true;

	return (cpu->getArch() == kernelArch );
}

QString OsLinuxConfiguration::hostSpecPath( const QString &path )
{
	QString hsPath=QString::null;
#ifdef _WIN32 /* Windows host */
	if( ! path.isEmpty() ){
		if( path.find("/cygdrive/") >= 0 ) {        /* cygwin specific path */
			hsPath = path.section( '/', 2,2) + ":\\" + path.section('/',3);
		}else if( path.find('/') == 0 ){
			/* probably a path relative to the Cygwin root mount */
			Q3Process *proc = new Q3Process();	/* external program */
			QStringList args;
			QString procOutput;	/* external program output */

			args << "cygpath -m -p /";
			proc->setArguments( args );

			if( ! proc->start() ){
				wDebug(( "hostSpecPath: Can't run \"cygpath\"\n" ));
			} else {
				while( proc->isRunning() ){
					if( G_qApp )
						G_qApp->processEvents();
				}

				if( !proc->normalExit() || proc->exitStatus() != 0 ){
					wDebug(("hostSpecPath: failed to execute %s: exitStatus=%x\n", (const char *)args[0], proc->exitStatus() ));
					hsPath = "***ERROR** " + path;
				} else {
					if( proc->canReadLineStdout() ) {
						procOutput = proc->readLineStdout();
						hsPath = procOutput + path;
					} else {
						wDebug(("hostSpecPath: failed getting cygwin install dir; nothing at stdout\n", (const char *)args[0], proc->exitStatus() ));
						hsPath = "***ERROR*** " + path;
					}
				}
			}
		}else if( (path.find( QRegExp("[a-zA-Z]:\\") ) >= 0) ||
				  (path.find( '\\' ) >= 0) ) { /* windows specific path */
			hsPath = path;
		} else {
			hsPath = path;
		}
		hsPath.replace(QChar('/'), '\\');
	}
#else /* non Windows host */
	hsPath = path;
#endif /* _WIN32 */

	wDebug(("hostSpecPath \"%s\"; \"%s\"", (const char *)path, (const char *)hsPath));
	return hsPath;
}

QString OsLinuxConfiguration::shellSpecPath( const QString &path )
{
	QString ssPath;

#ifdef _WIN32 /* Windows host */
	char *env = getenv("CYGWIN");

	if( ! path.isEmpty() ){
		if( env ) { /* it's a cygwin shell */
			if( path.find( QRegExp("^[a-zA-Z]:") ) < 0 && 	/* drive letter */
				path.find("\\") < 0 )						/* no backslashes */
			{
				/* linux / cygwin specific path, leave as is */
				ssPath = path;
			}else if( path.find( QRegExp("^[a-zA-Z]:") ) >= 0 ){ /* absolute Windows specific path */
				ssPath = "/cygdrive/" + path;
				ssPath.remove(':');
			} else { /* leave as is so a error is thrown? */
				ssPath = path;
			}
			ssPath.replace(QChar('\\'), '/');
		} else {
			/* leave as is */
			ssPath = hostSpecPath(path);
		}
	}
#else /* non Windows host */
	ssPath = path;
#endif /* _WIN32 */

	wDebug(("shellSpecPath \"%s\" ==> \"%s\"", path.latin1(), ssPath.latin1()));
	return ssPath;
}
//----------------------------------------------------------------------
// New project wizard

//! create the new project wizard for Linux
/*!
  execute with the exec() method
*/
OsLinuxNewProjectWizard::OsLinuxNewProjectWizard(
	OsLinuxConfiguration *_cfg, QWidget *parent, const char *name) :
	Q3Wizard( parent, name, TRUE )
{
	QWidget *wdg;
	Setting *setting;
	QString title;

	resize( 450, 300 ); // make it big. looks better
	cfg = _cfg;

	// make a copy of the current settings
	// these will be restored when user cancels wizard
	savedSettings = cfg->lstSettings.clone();


	// build one page for each setting
	for( setting = cfg->lstSettings.first();
		 setting;
		 setting= cfg->lstSettings.next() ){

		wdg = setting->wizardPage( this, &title );
		if( wdg ){
			wDebug((" add page %p (%s)", wdg, (const char *)setting->key()));
			addPage( wdg, title );
			setHelpEnabled( wdg, false );
		}
	}

	// Add page to select CPU board:
	cpuWdg = new AddComponentWidget(this, 0,
		"Please select your target CPU board from the list below:");

	addPage( cpuWdg, "Select a CPU board" );
	setHelpEnabled( cpuWdg, false );

	// set CDK setting to ELinOS if we've found ELINOS_PROJECT
	if( !(OsLinuxConfiguration::elinosProjectEnv().isEmpty()) ) {
		cfg->settingCdk->setValue("ELinOS");
		elinosRootPath = QString( OsLinuxConfiguration::elinosRootEnv() );
	}

	// force the wizard page's "appropriate" property to be update
	cfg->cdkChanged( cfg->settingCdk->index() );
}

//! destructs the project wizard
OsLinuxNewProjectWizard::~OsLinuxNewProjectWizard()
{
	delete savedSettings;
}


//! Called when user pressed "next" button.
/*!
  Reimplemented from QWizard.

  This calls the slotApply() function of the current setting (if the
  current page belongs to a setting. If Setting::slotApply() returns
  an error message, it shows a message box and the current page cannot
  be left.

  Otherwise it behaves like QWizard::next
*/
void
OsLinuxNewProjectWizard::next()
{
	if( validateSetting() == false )
		return;

	Q3Wizard::next();
}

//! Called when user pressed "finish" or "cancel" button.
/*!
  Reimplemented from QWizard.

  This calls the slotApply() function of the current setting,
  i.e. the last page (if the current page belongs to a setting. If
  Setting::slotApply() returns an error message, it shows a message
  box and the current page cannot be left.

  If the user pressed "cancel", the original settings are restored.

  Otherwise it behaves like QWizard::done
*/
void
OsLinuxNewProjectWizard::done( int r )
{
	extern CpuModelFactory *G_cpuMods;

	if( r == QDialog::Accepted ){
		if( validateSetting() == false )
			return;

		// Add cpu device to configuration
		QString cpuName = cpuWdg->result();
		if( !cpuName.isEmpty() ){
			Device *dev, *newDev;
			dev = G_cpuMods->find( cpuName );
			QString errMsg;

			WIZ_ASSERT( dev );

			// create an instance of template device
			newDev = dev->create();

			if( cfg->addDevice( 0, newDev, errMsg ) == false ){

				VMsgBox::criticalOk(
					MAIN_GetCurrentCfg()->msgBoxCaption(),
					"Couldn't create CPU device",
					errMsg,
					this );
				return;
			}

		}
		else
			return; // no CPU selected
	}
	else if( r == QDialog::Rejected ) {
		// restore original settings
		cfg->lstSettings.takeValues( savedSettings );
	}
	Q3Wizard::done(r);
}

bool
OsLinuxNewProjectWizard::validateSetting()
{
	Setting *setting;

	// call slotApply() of the currently displayed setting
	if( currentPage()){
		for( setting = cfg->lstSettings.first();
			 setting;
			 setting = cfg->lstSettings.next() ){

			if( setting->currentWidget() == currentPage() ){
				break;
			}
		}

		// not all pages belong to a setting, so its not an error
		// if setting is 0 here

		if( setting ){
			QString errMsg;
			wDebug(("OsLinuxNewProjectWizard::validateSetting: %s",
				   (const char *)setting->key()));
			setting->slotApply( errMsg );

			if( errMsg.isEmpty() && (setting->key() == "ELINOS_PROJ_DIR") ) {
				//! In case of ELinOS, check if ELinOS is installed
				//  check here because earlier ELINOS.sh is not sourced yet
				QFileInfo fi( elinosRootPath );
				wDebug(("OsLinuxNewProjectWizard::validateSetting: %s",
						elinosRootPath.latin1() ));

				// check if directory exists. If so, check if it is empty
				if( elinosRootPath.isEmpty() || !fi.exists() ){
					// should never reach
					// should already fail in SettingElinosProjDir::validate()
					errMsg += setting->key() + ": ELinOS is not installed. (\"" +
							  elinosRootPath + "\" does not exist!)\n" +
							  " Maybe, ELinOS installed in other directory?\n";
				}
#ifdef _WIN32
			/* TBD: "WIZ_CDK:ELinOS: source ELINOS.sh does not work; source upfront" */
			} else if( errMsg.isEmpty() &&
					   (setting->key() == "WIZ_CDK") &&
					   (setting->value() == "ELinOS") &&
					   (OsLinuxConfiguration::elinosProjectEnv() == QString::null ) )
			{
				//! In case of ELinOS under Windows, check if ELinOS.sh is sourced,
				//  setting environment variables does not work yet!!!

				errMsg += setting->key() + ": ELinOS environment not set up!\n" +
						  "Please source ELINOS.sh or execute ELINOS.bat in shell you call mdiswiz.exe from\n";
#endif
			}

			if( !errMsg.isEmpty() ){
				QMessageBox::critical( this,
									   cfg->msgBoxCaption(),
									   errMsg,
									   QMessageBox::Ok,
									   Qt::NoButton);
				return false;
			}
		}
	}
	return true;
}

//! reimplemented from QWizard
/*!
*/
void
OsLinuxNewProjectWizard::showPage( QWidget* page )
{
	extern CpuModelFactory *G_cpuMods;

	// if this is the CPU selection page,
	// make a list of CPU boards matching kernel arch
	if( page == cpuWdg ){
		wDebug(("OsLinuxNewProjectWizard:: about to show cpuWdg\n"));

		cpuWdg->clearItems();

		for( CpuDevice *dev=(CpuDevice *)G_cpuMods->first(); dev;
			 dev=(CpuDevice *)G_cpuMods->next() ){
			if( cfg->cpuOkForCfg( dev ) ) {
				cpuWdg->addItem( dev->getModelName(), dev->getDescription() );
			}
		}
	}
	Q3Wizard::showPage(page);

	if( finishButton() )
		finishButton()->setEnabled( true );
}

//----------------------------------------------------------------------
//! creates the OsLinuxTab
OsLinuxTab::OsLinuxTab( OsLinuxConfiguration *_cfg, QWidget *parent,
						const char *name) :
	QWidget( parent, name )
{
	Q3VBoxLayout *vb = new Q3VBoxLayout(this,10);
	Setting *setting;
	QWidget *wdg;

	cfg = _cfg;

	// make a copy of the current settings
	// these will be restored when user cancels dialog
	savedSettings = cfg->lstSettings.clone();

	for( setting = cfg->lstSettings.first();
		 setting;
		 setting= cfg->lstSettings.next() ){

		if( setting->enabled() != Setting::Hide ){
			wdg = setting->settingsTabWidget( this );
			if( wdg )
				vb->addWidget( wdg );
		}
	}
	vb->addStretch(1);
}

//! destructs the settings tab widget
OsLinuxTab::~OsLinuxTab()
{
	delete savedSettings;
}

//! called when apply or Ok button pressed in dialog
/*!
  calls the Setting::slotApply function of all underlying settings.
*/
void
OsLinuxTab::slotApply()
{
	wDebug(("OsLinuxTab::slotApply"));
	QString errMsg;

	cfg->lstSettings.applyAll( errMsg );

	if( !errMsg.isEmpty() ){
		VMsgBox::criticalOk( cfg->msgBoxCaption(),
								 "Some settings have invalid values.\n"
								 "Please correct them!",
								 errMsg,
								 this );
	}

}

//! called when Cancel button pressed in dialog
/*!
  restores the saved settings
*/
void
OsLinuxTab::slotCancel()
{
	wDebug(("OsLinuxTab::slotCancel"));

	// restore original settings
	cfg->lstSettings.takeValues( savedSettings );
}


//----------------------------------------------------------------------
//! sets up the toolbar and project menu
void
LinuxMdiswiz::setupProjectActions()
{
	QAction *a;

	MdiswizView::setupProjectActions();

	// add linux specific entries

	// generate build menu
	Q3PopupMenu *buildMenu = new Q3PopupMenu( this );
	menu->insertItem( "&Build", buildMenu, -1, 1 );


	a = new QAction( QPixmap(scan_xpm), 
					 "&Scan", 0, this, "scan" );
	a->setObjectName("Scan System");
	connect( a, SIGNAL( activated() ) , this, SLOT( slotScan() ) );
	a->addTo( buildMenu );
	a->addTo( tb );

	a = new QAction( QPixmap(build_xpm),
					 "&Build", 0, this, "build" );
	a->setObjectName("Build Project");
	connect( a, SIGNAL( activated() ) , this, SLOT( slotBuild() ) );
	a->addTo( buildMenu );
	a->addTo( tb );

	a = new QAction( QPixmap(rebuild_xpm),
					 "&Rebuild", 0, this, "rebuild" );
	a->setObjectName("Rebuild Project");
	connect( a, SIGNAL( activated() ) , this, SLOT( slotRebuild() ) );
	a->addTo( tb );
	a->addTo( buildMenu );

	a = new QAction( QPixmap(buildinstall_xpm),
					 "&Install", 0, this, "install" );
	a->setObjectName("Install Project");
	connect( a, SIGNAL( activated() ) , this, SLOT( slotBuildInstall() ) );
	a->addTo( tb );
	a->addTo( buildMenu );
}

void LinuxMdiswiz::doBuild( const QString &title, const QStringList &args, int isScan )
{
	if( !buildDlg ){
		// make sure configuration saved
		slotSaveConfig();

		buildDlg = new ExecProgram(
			MAIN_GetCurrentCfg()->msgBoxCaption(),
			title,
			this,
			"buildDlg",
			false);
		buildDlg->show();

		buildDlg->start( args );

		if (isScan) {
			connect( buildDlg, SIGNAL(finished()),
					 this, SLOT(slotScanFinished()));			
		} else {
			connect( buildDlg, SIGNAL(finished()),
					 this, SLOT(slotBuildFinished()));
		}
	}
}

//! scan system
void LinuxMdiswiz::slotScan()
{
	QStringList args;
	//pass MEN_LIN_DIR to script
	args << "sh" << "-c" << menLinuxPath + "/scan_system.sh " + menLinuxPath ;
	doBuild( "Scanning system and generating example system.dsc", args, 1 );
}


//! build project
void LinuxMdiswiz::slotBuild()
{
	QStringList args;

	args << "make";

	doBuild( "Building Project (make)", args, 0 );
}

//! rebuild project
void LinuxMdiswiz::slotRebuild()
{
	QStringList args;

	args << "sh" << "-c" << "echo \"MEN_LIN_DIR=$MEN_LIN_DIR ELINOS_PROJECT=$ELINOS_PROJECT\"; make clean; make";

	doBuild( "Rebuilding Project (make clean; make)", args, 0 );
}

//! install project
void LinuxMdiswiz::slotBuildInstall()
{
	QStringList args;

#ifdef _WIN32
	/* no kdesu available/needed */
	args << "make install";
#else
	args << "sh" << "-c" << "echo \"MEN_LIN_DIR=$MEN_LIN_DIR ELINOS_PROJECT=$ELINOS_PROJECT\"; make install";
#endif
	doBuild( "Installing Project (make install)", args, 0 );
}


//! called when build process has been exited
void LinuxMdiswiz::slotScanFinished()
{
	bool success;
	Configuration *cfg;
	QString errMsg;
	QString currDirName = QDir::currentDirPath();
	if( buildDlg ){
		int status = buildDlg->status();
		delete buildDlg;
		buildDlg = 0;
		if( status == 0 ){
			QMessageBox::information(
				this, MAIN_GetCurrentCfg()->msgBoxCaption(),
				"Scan completed. Example system.dsc and Makefile generated.\n",
				QMessageBox::Ok, Qt::NoButton);
		}
		// auto load the generated system.dsc
		cfg = MAIN_GetCurrentCfg()->create();
		MAIN_SetCurrentCfgStoreLast( cfg );
		//settingMdisProjDir->setValue( cfg );
		success = MAIN_GetCurrentCfg()->load( errMsg, currDirName  );

		if( success )
		{
			Q3ListViewItem *it;
			// delete the old GUI items
			it = listView->firstChild();
			if( it )
				delete it;

			MAIN_DeleteOldCfg();
			if(	errMsg != QString::null )
			{
				VMsgBox::warningOk(
					MAIN_GetCurrentCfg()->msgBoxCaption(),
					QString("Warnings while loading project"),
					errMsg,	this );
			}
		}
		else
		{
			if( errMsg != QString::null )
			{
				// fatal errors while loading
				VMsgBox::criticalOk(
					MAIN_GetCurrentCfg()->msgBoxCaption(),
					QString("Could not load project"),
					errMsg,	this );
			} 
			// restore the old configuration (delete current)
			MAIN_SetCurrentCfgDeleteLast( MAIN_GetOldCfg() );
		}
		// create the new GUI representation
		if( MAIN_GetCurrentCfg()->getRootDev() )
			MAIN_GetCurrentCfg()->getRootDev()->createGui( this, 0 );

	}
} // funcEnd


//! called when build process has been exited
void LinuxMdiswiz::slotBuildFinished()
{
	if( buildDlg ){
		int status = buildDlg->status();
		delete buildDlg;
		buildDlg = 0;

		if( status == 0 ){
			QMessageBox::information(
				this,
				MAIN_GetCurrentCfg()->msgBoxCaption(),
				"Build completed successful, kernel modules can be loaded and used.\n"
				"Check MDIS- and driver documentation for details.",
				QMessageBox::Ok,
				Qt::NoButton);
		}
	}
}


