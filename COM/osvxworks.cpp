/***************************************************************************
 */
/*!       \file  osvxworks.cpp
 *       \brief  Implementation of class OsVxworks
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2018/06/13 14:20:15 $
 *    $Revision: 2.17 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: osvxworks.cpp,v $
 * Revision 2.17  2018/06/13 14:20:15  ts
 * R: updated VxWorks version to 4.4, added different flags required to new processors (CC10 etc).
 *
 * Revision 2.16  2016/04/29 10:38:22  ts
 * R: added explanation to tool chain that it should match the BSPs TOOL setting
 *
 * Revision 2.15  2016/04/28 18:01:33  ts
 * R: tool chains for CPUs with e500v2 core were not selectable
 * M: added 2 additional toolchains for PPC boards
 *
 * Revision 2.14  2014/08/22 15:56:12  dpfeuffer
 * R: ARM CPU support
 * M: mapping for IMX6 added
 *
 * Revision 2.13  2014/07/18 15:12:00  dpfeuffer
 * R: erroneous PMC support, missing ComExpress support, general maintenance
 * M: intermediate check-in during development
 *
 * Revision 2.12  2013/03/04 13:02:50  dpfeuffer
 * R: A21 support
 * M: PpcQorIQ CPU mapping added
 *
 * Revision 2.11  2013/01/21 10:29:01  ts
 * R: typo in help text
 * M: corrected wether to whether
 *
 * Revision 2.10  2011/09/26 08:42:31  dpfeuffer
 * R:1. SettingAllCpus from osos9.cpp called by mistake
 *   2. CPU selection not clear for the user
 * M:1. SettingAllCpus renamed to SettingAllCpusVx
 *   2. CPU selection description improved
 *
 * Revision 2.9  2011/03/31 17:32:14  CRuff
 * R: 1. warn user if installed MDIS system package and selected cpu do not match
 * M: 1a) check pci domain compatibility in Configuration::save(), load() and
 *        in slot menCpuChanged()
 *    1b) added methods mdisSupportsPciDomains() and checkPciDomains to OS
 *        specific configuration class
 *
 * Revision 2.8  2011/02/16 17:04:07  CRuff
 * R: new revision
 * M: changed revision string
 *
 * Revision 2.7  2010/10/22 11:52:45  CRuff
 * R: release of new version
 * M: increased version string
 *
 * Revision 2.6  2010/10/20 13:33:08  CRuff
 * R: 1. project settings were not restored correctly after project load
 *    2. cosmetics
 * M: 1. in loadMakefile(): save setting value if setting known
 *    2. in load(): do not use format string for error messages, but QString
 *       operator +
 *
 * Revision 2.5  2010/06/21 10:55:38  CRuff
 * R: release of new version
 * M: increase version number
 *
 * Revision 2.4  2010/06/18 13:17:04  CRuff
 * R: Makefile with option ALL_CORE_LIBS = no not read correctly
 * M: in loadMakefile: changed key.isEmpty to !key.isEmpty at ALL_CORE_LIBS check
 *
 * Revision 2.3  2010/06/16 11:49:27  CRuff
 * R: avoid possibility of application crash
 * M: loadMakefile: verify that string is not empty before accessing char 0
 *
 * Revision 2.2  2010/06/02 17:09:18  CRuff
 * R: support for new processor types
 * M: added Atom and Ppc512x core types to VxCpuTypeMap
 *
 * Revision 2.1  2010/05/19 16:10:10  CRuff
 * R: Qt 4.6.0 does not allow to access empty String Lists
 * M: in loadMakefile(), add checks if StringList is empty before accessing it
 *
 * Revision 2.0  2010/01/22 11:26:59  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.19  2009/08/17 13:33:48  CRuff
 * R: List of available CPUs not updated correctly when CPU was changed
 * M: a) attach to signal sigCpuChanged(), emitted by MdiswizView
 *    b) added new slot menCpuChanged() to update the list of cpu cores after
 *       signal was received
 *
 * Revision 1.18  2009/05/13 11:25:34  CRuff
 * R:1. Core lib 'oss' needs to pull in additional makefile
 *   2. Symbol name ALL_CORE in generated Makefile causes cyclic dependency
 *   3. Compiler flag for UP/SMP setting needs definition tag '-D'
 * M:1a) OsVxWorksConfiguration::OsVxWorksConfiguration(): added makefile
 *      'library_pci.mak' to swModule 'oss'
 *   1b) extended mkSwModulesTups(), validateSwModule() to handle sw modules
 *       with multiple makefiles
 *   2. Rename symbol ALL_CORE to ALL_CORE_LIBS in
 *      OsVxWorksConfiguration::loadMakefile(), ::swModuleMfString()
 *   3. added option '-D' for UP/SMP setting in loadMakefile() and saveMakefile()
 *
 * Revision 1.17  2009/04/30 21:23:48  CRuff
 * R: release MDIZWIZ
 * M: increased version number
 *
 * Revision 1.16  2009/04/30 17:29:59  CRuff
 * R: make Kernel modules OSS and DBG configurable
 * M: set value for CoreLib flag ALL_CORE to 'no' if no module is selected
 *
 * Revision 1.15  2009/04/30 11:13:04  CRuff
 * R: The software modules selection in the project settings dialog for usr_oss
 *    and usr_utl has no effect
 * M: Removed the selection option for those modules from the project settings
 *    dialog
 *
 * Revision 1.14  2009/04/27 17:49:25  CRuff
 * R: 1. New project settings option UP / SMP for VxWorks
 *    2. Provide CPU selection possibility for Intel CPUs
 *    3. make Kernel modules OSS and DBG configurable
 * M: 1. a) added strings UP_ONLY and SMP_ONLY (compiler flags)
 *       b) added new setting class SettingSuppProcArch
 *       c) create SettingSuppProcArch instance in
 *          OsVxworksConfiguration::OsVxworksConfiguration()
 *       d) added handling for new architecture setting in
 *          OsVxworksConfiguration::loadMakefile() and
 *          OsVxworksConfiguration::saveMakefile()
 *    2. added handling for different Intel CPUs in VxCpuTypeMap::VxCpuTypeMap()
 *    3. added handling for new SwModuleTypes::CoreLib in
 *       OsVxworksConfiguration::saveMakefile() and
 *       OsVxworksConfiguration::swModuleMfString()
 *
 * Revision 1.13  2008/02/06 12:46:59  cs
 * R: 1. Cancel button in build window did not work
 *    2. creation of new project: subdirs w. same name were created on each save
 * M: 1. add own slotCancel function
 *       which calls kill when program not finished after 5s
 *    2. use absolute names as default new project name
 *       def dir for new projects now in BUILD/MDIS/..
 *
 * Revision 1.12  2006/12/20 11:57:12  cs
 * fixed:
 *    - don't include USR_OSS and USR_UTL in Makefile
 *    - take MEN_VX_DIR from environment when not defined in Makefile
 *
 * Revision 1.11  2006/11/30 11:50:44  cs
 * added:
 *   + Makefile: + ADDED_CFLAGS
 *   + backup old configuration files to *.bak on load of existing project
 * fixed:
 *   - don't display CPU extension boards in CPU selection lists
 * raised version
 *
 * Revision 1.10  2006/07/27 13:43:27  ub
 * Added: Crusoe processor
 *
 * Revision 1.9  2005/12/08 15:58:09  UFranke
 * added
 *  + Makefile support for VxWorks 6.1
 *
 * Revision 1.8  2005/12/05 09:35:09  UFranke
 * cosmetics
 *
 * Revision 1.7  2005/07/15 15:47:15  ub
 * added mapping for PentiumM and PPC E500
 *
 * Revision 1.6  2004/07/30 11:49:14  ufranke
 * added
 *  - Makefile debug flag MDBG
 *  - Compiler flag DEBUG_FLAGS
 * changed
 *  - at new configuration for a CPU with PCI
 *    PCI support is enabled
 *
 * Revision 1.5  2004/07/27 14:17:31  ufranke
 * changed
 *  - native tool path for MDIS build compatibility
 *
 * Revision 1.4  2004/07/21 15:07:23  ufranke
 * changed
 *  - hide G_cfg
 * added
 *  - default native SW modules
 *
 * Revision 1.3  2004/07/15 10:17:15  ufranke
 * no official version
 *
 * Revision 1.2  2004/01/09 08:20:14  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/06/11 12:21:43  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

/*
  Definitions:
  - MEN_VX_DIR is stored in Makefile.
  - setting dialog for MEN_VX_DIR
  - If environment MEN_VX_DIR exists, it is used
  - Old Makefiles will still work if they are on same level as TPL
  - NT descgen binary which ignores _WIZ_ keys

  Initial Dialog
  - MEN_VX_DIR
  - MDIS_PROJ_DIR
  - TOOL
  - ALL_DBGS
  - Select Cpu
    - Set "CPU" dependent on Cpu
	- If Cpu allows different CPU types, ask for them
    - Set "PCI_SUPPORT" dependent on CPU (no dialog)
*/
/*
  TBD:

  Build system:
  - ALL_NATIVE_DRIVERS, ALL_NATIVE_TOOLS, ALL_NATIVE_LIBS

  Auto build usrMdis.c
  Ask for BSP?

  CpuDevice:
   -add Info for PciTranslationOffset? or use new sysBusToAddr(PCI)?
   -Info for sysIntEnable etc...

*/
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
#include <qmap.h>
#include <qtimer.h>
#include <stdlib.h>
#include <qstring.h>

#include <Q3HBoxLayout>
#include <Q3ValueList>
#include <QLabel>
#include <QPixmap>
#include <Q3PopupMenu>
#include <Q3VBoxLayout>
#include <Q3PtrList>

#include "wizdebug.h"
#include "configuration.h"
#include "hwcomponent.h"
#include "osvxworks.h"
#include "cfgmakefile.h"
#include "swmodulestab.h"
#include "osvxworks.h"
#include "packagedb.h"
#include "setting.h"
#include "vmsgbox.h"
#include "util.h"
#include "install.h"
#include "modelfactory.h"
#include "mdiswizdialogs.h"
#include "mdiswiz.h"
#include "execprogram.h"
#include "typestr.h"
#include "esmcarrier.h"

// pixmaps
#include "images/build.xpm"
#include "images/rebuild.xpm"

extern QWidget *G_mainWindow; //!< used as anchor for dialog
extern QApplication *G_qApp;  //!< main application

// strings
static const QString mdiswizVxworksVersion("vxworks-4.4");
static const QString sysDescPlainName("system");
static const QString sysDescName( sysDescPlainName + ".dsc");
static const QString makeName("Makefile");
static const QString packageDbPath("PACKAGE_DESC");
static QString envMenVxDir; //!< value of env. var MEN_VX_DIR
static const char MEN_VX_DIR[] = "MEN_VX_DIR";
static const QString MDBG = "MDBG"; //!< makefile debug switch
static const QString DEBUG_FLAGS = "DEBUG_FLAGS"; //!< compiler switch i.e. -gdwarf
static const QString ADDED_CFLAGS = "ADDED_CFLAGS"; //!< compiler switch i.e. -O0
static const QString ADDED_CFLAGS_UP_SMP = "ADDED_CFLAGS: UP/SMP"; //UP/SMP system
static const QString UP_ONLY = "_UP_COMPATIBLE_ONLY"; //!< compiler setting: uniprocessor system
static const QString SMP_ONLY = "_SMP_COMPATIBLE_ONLY"; //!< compiler setting: multiprocessor system
static const char TGT_DIR[] = "TGT_DIR";
static const char MEN_VXWORKS_ENV_VER[] = "MEN_VXWORKS_ENV_VER";
static const QString sysProjectNameDef("BUILD/MDIS/myMdisProj");


//! Mapping of VxWorks style CPU types and CpuDevice constants
class VxCpuTypeMap : public TypeStrMap<CpuDevice::CpuCore> {
public:
	VxCpuTypeMap() {
		mmap[CpuDevice::I386] 		= "I80386";
		mmap[CpuDevice::I486] 		= "I80486";
		mmap[CpuDevice::Pentium]	=
		mmap[CpuDevice::Crusoe]		=
		mmap[CpuDevice::Atom]		=
		mmap[CpuDevice::Athlon]		=
		mmap[CpuDevice::Duron]		= "PENTIUM";
		mmap[CpuDevice::Pentium2]	= "PENTIUM2";
		mmap[CpuDevice::Pentium3]	= "PENTIUM3";
		mmap[CpuDevice::PentiumM]	=
		mmap[CpuDevice::Pentium4]	= "PENTIUM4";
		mmap[CpuDevice::Ppc4xx]		= "PPC403";
		mmap[CpuDevice::Ppc512x]	= "PPC512X";
		mmap[CpuDevice::Ppc603]		= "PPC32";
		mmap[CpuDevice::Ppc603e]	= "PPC32";
		mmap[CpuDevice::Ppc604]		=
		mmap[CpuDevice::Ppc740]		=
		mmap[CpuDevice::Ppc750]		=
		mmap[CpuDevice::Ppc74xx]	= "PPC604";
		mmap[CpuDevice::Ppc8xx]		= "PPC860";
		mmap[CpuDevice::PpcE500]	= "PPC32";
		mmap[CpuDevice::PpcQorIQ]	= "PPC32";
		mmap[CpuDevice::M68000]		= "MC68000";
		mmap[CpuDevice::M68010]		= "MC68010";
		mmap[CpuDevice::M68020]		=
		mmap[CpuDevice::M68030]		= "MC68020";
		mmap[CpuDevice::M68060]		= "MC68060";
		mmap[CpuDevice::MCpu32]		= "CPU32";
		mmap[CpuDevice::IMX6]		= "ARMARCH7";
	};
};


//---------------------------------------------------------------------
// Static functions


//----------------------------------------------------------------------
//! Check if directory is ok to store MDIS configuration
/*!
??? duplicated with Linux
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
//! Determine possible CpuTypes of CpuDevice
/*!
  Makes a QStringList that holds all (VxWorks style) CPU types
  that are possible on the given \a cpu
*/
static QStringList vxCpuTypes( CpuDevice *cpu )
{
	QStringList vxTypes;
	VxCpuTypeMap typeMap;
	typedef Q3ValueList<CpuDevice::CpuCore> CpuList;
	CpuList::const_iterator it;

	for ( it = cpu->getLstCpuCores().begin();
		  it != cpu->getLstCpuCores().end();
		  ++it )
	{

		QString vxCpu = typeMap.keyToStr( *it );
		wDebug(("vxCpu=%s", vxCpu.latin1() ));

		// check if mapped string already in list
		if( vxTypes.contains( vxCpu ) == 0 )
			vxTypes += vxCpu;
	}
	return vxTypes;
}

//----------------------------------------------------------------------
//! Check if the installed MDIS system package supports pci domains
/*!
  checks if file mdis_version.h is found and contains
  #define OS_HAS_PCI_DOMAINS

  \returns true if file is found and contains define, false otherwise
*/
bool 
OsVxworksConfiguration::mdisSupportsPciDomains( )
{
	QFileInfo mdisVersFile( envMenVxDir + QString("/INCLUDE/NATIVE/MEN/mdis_version.h"));
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
OsVxworksConfiguration::checkPciDomains()
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



class SettingMenVxDir : public DirectorySetting {
public:

	SettingMenVxDir() :
		DirectorySetting( MEN_VX_DIR,
						  DirectorySetting::Dir ) {
	}

	virtual Setting *clone(){
		SettingMenVxDir *setting = new SettingMenVxDir();
		copy( setting );
		return setting;
	}

	virtual bool validate( const QVariant &newVal, QString &errMsg ){
		QString vxDir = lst->expandedValueOf( newVal.toString() );
		bool rv = true;

		QFileInfo fi1( vxDir + "/BUILD/MDIS/TPL/corecomp.mak" );

		if( !fi1.exists() ){
			errMsg += "\"" + vxDir +
				"\" does not look like a VxWorks MDIS directory (" +
				fi1.filePath() + " not found)\n";
			rv = false;
		}
		return rv;
	}

protected:
	QString infoText() {
		return "The directory where you have installed the MDIS system "
			"package for VxWorks, e.g. /MEN/VXWORKS\nnote: FORWARD SLASHES '/' needed";
	}
	QString labelText() {
		return "MDIS system package install directory";
	}
	QString wizPageTitle() {
		return "Select the MDIS system package install directory";
	}
};

class SettingMdisProjDir : public DirectorySetting {
public:

	SettingMdisProjDir() :
		DirectorySetting( "MDIS_PROJ_DIR",
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
		QString expVal = lst->expandedValueOf( newVal.toString() );
		bool rv;

		if( (rv = isGoodMdisProjDir( expVal, errMsg )) == false )
			errMsg = key() + ": " + errMsg;

		return rv;
	}

protected:
	QString infoText() {
		return "This directory is used to store the configuration that you "
			"configure with mdiswiz. The Makefile and descriptor file "
			"\"system.dsc\" will be saved to this directory";
	}
	QString labelText() {
		return "MDIS project directory";
	}
	QString wizPageTitle() {
		return "Select the project directory to store MDIS configuration";
	}
};


class SettingDbgVx : public MultipleChoiseSetting {
public:
	enum Choises { No=0, Yes };

	SettingDbgVx() : MultipleChoiseSetting( "ALL_DBGS" ) {
		choises << "no" << "yes";

		choisesHelp <<
			"Do not include debug strings. Run at full speed." <<

			"Include debug strings. Modules run slower.";

		_value = QVariant(choises[Yes]);
	}

	virtual Setting *clone(){
		SettingDbgVx *setting = new SettingDbgVx();
		copy( setting );
		return setting;
	}

protected:
	QString infoText() {
		return "Defines whether to build and install the release (no) or "
		"debug (yes) versions of the modules. The debug version "
		"of the modules issue debug messages using DBG_Write for "
 	    "trouble shooting. The message display function is DBG_Show().";
	}
	QString labelText() {
		return "Build mode for MDIS modules";
	}
	QString wizPageTitle() {
		return "Select build mode for MDIS modules";
	}
};

class SettingToolChain : public MultipleChoiseSetting {
public:
	enum Choises { Gnu=0, Diab };
	SettingToolChain() : MultipleChoiseSetting( "TOOL" ) {
		choises << "gnu" << "e500v2gnu" << "diab" << "e500v2diab"  ;

		choisesHelp << "GNU toolchain" << "GNU toolchain (e500v2 cores)" << "Diab toolchain" << "diab toolchain (e500v2 cores)";

		_value = QVariant(choises[Gnu]);

	}

	virtual Setting *clone(){
		SettingToolChain *setting = new SettingToolChain();
		copy( setting );
		return setting;
	}

protected:
	QString infoText() {
		return "Defines compiler toolchain to build the MDIS modules. This should match the TOOL setting used in your VxWorks BSP. ";
	}
	QString labelText() {
		return "Toolchain";
	}
	QString wizPageTitle() {
		return "Select Toolchain";
	}
};

class SettingSuppProcArch : public MultipleChoiseSetting {
public:
	enum Choises { UP=0, SMP };

	SettingSuppProcArch() : MultipleChoiseSetting( ADDED_CFLAGS_UP_SMP ) {
		choises << UP_ONLY << SMP_ONLY;

		choisesHelp <<
			"UP System" <<
			"SMP System";

		_value = QVariant(choises[UP]);
	}

	virtual Setting *clone(){
		SettingSuppProcArch *setting = new SettingSuppProcArch();
		copy( setting );
		return setting;
	}

protected:
	QString infoText() {
		return "Defines the system type: \n"
			   "uniprocessor (UP) or a symmetrical multi-processor (SMP) system";
	}
	QString labelText() {
		return "";
	}
	QString wizPageTitle() {
		return "Select processor system type";
	}

};

class SettingAllCpusVx : public MultipleChoiseSetting {
public:
	SettingAllCpusVx() : MultipleChoiseSetting( "ALL_CPUS" ) {
	choises << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7"
			<< "2" << "3" << "4" << "5" << "6" << "7";
	}



	virtual Setting *clone(){
		SettingAllCpusVx *setting = new SettingAllCpusVx();
		copy( setting );
		return setting;
	}

protected:
	QString infoText() {
		return "Please choose the CPU type appropriate to your CPU Board. "
			"This will determine the right compiler switch.\n"
			"Note: If you are using an older VxWorks version, "
			"the determined compiler switch may not be supported. "
			"In this case you have to select a lower CPU type.";
	}
	QString labelText() {
		return "CPU Type";
	}
	QString wizPageTitle() {
		return "Select CPU Type";
	}

};

// -----------------------------------------------------------------
//! Vxworks specific build exec
class OsVxworksExecProgram : public ExecProgram {
public:
	OsVxworksExecProgram( const QString & caption,
						  const QString & text,
						  QWidget * parent = 0,
						  const char * name = 0,
						  bool modal = false,
						  Qt::WFlags f = 0) :
		ExecProgram( caption, text, parent, name, modal, f)
	{};

private slots:
	virtual void slotCancel(){
		proc->tryTerminate();
	    QTimer::singleShot( 5000, proc, SLOT( kill() ) );
		finishReason = QString( "canceled" );
	};
};


//----------------------------------------------------------------------
//! initial action dialog
/*!
  Initial dialog to let user decide what to do
  (open existing/create new project).

  Bring The dialog with exec(). If it returns \c QDialog::Accepted,
  you can retrieve the choosen action by calling the action() method.

*/
class OsVxworksInitialDialog : public QDialog {
public:
	//! creates the add component dialog
	OsVxworksInitialDialog(QWidget *parent,
						 const char *name=0,
						 bool modal=TRUE,
						 Qt::WFlags f=0) : QDialog(parent,name,modal,f)
	{
		Q3VBoxLayout* vb = new Q3VBoxLayout(this,10);
		QPushButton *but;
		QLabel *lab;

		lab = new QLabel( "<b>Welcome to the MDIS Configuration Wizard "
						  "for VxWorks!</b>",
						  this );
		vb->addWidget( lab );

		lab = new QLabel(
			"This tool allows you to configure MDIS devices easily.\n\n"
			"Please choose your initial action:", this );
		vb->addWidget( lab );

	    buttonGroup = new Q3ButtonGroup( 1, Qt::Horizontal,
										QString::null, this );

		new QRadioButton( "Create new project", buttonGroup );
		new QRadioButton( "Open existing project", buttonGroup );
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
	  \return 0=create new, 1=open existing
	*/
	int action() { return buttonGroup->id( buttonGroup->selected()); }

private:
	Q3ButtonGroup *buttonGroup;
};


//----------------------------------------------------------------------
//! creates OsVxworks object
/*!
*/
OsVxworks::OsVxworks() : TargetOs( OsFactory::Vxworks )
{
	// check if environment variable MEN_VX_DIR is present
	envMenVxDir = getenv( MEN_VX_DIR );
	envMenVxDir.replace( '\\' , '/' );
	wDebug(("OsVxworks MEN_VX_DIR=%s", envMenVxDir.latin1() ));

}

Configuration *
OsVxworks::createConfiguration()
{
	return new OsVxworksConfiguration;
}

QString
OsVxworks::pkgDbPath()
{
	return envMenVxDir + "/" + packageDbPath;
}

//! perform initial, OS-specific init
/*!
  This routine performs the following steps:
  - Init the package data base
  - Checks if a config name has been specified on command line. If so,
    open this configuration.
  - Checks if it was started from a directory containg a MDIS project. If so,
    open this configuration.
  - Bring up the initial dialogs

  \return true if ok, false if fatal error (abort mdiswiz)
*/
bool
OsVxworks::startup( int argc, char **argv )
{
	extern MdiswizView *G_mdiswiz;
	bool success = false;
	QString errMsg;
	QString mdisDir = QDir::currentDirPath();

	wDebug(("OsVxworks::startup()" ));
	// remove unused SW module types
	G_swModTypes->types.remove( SwModuleTypes::Other );
	G_swModTypes->types.remove( SwModuleTypes::CbDriver );

	// patch VxWorks specific paths
	G_swModTypes->types[ SwModuleTypes::NativeTool ].natPath = "TOOLS";

	// must create a temporary configuration, it is immediately
	// destroyed by MdiswizView::loadConfig() or newConfig()
	OsVxworksConfiguration *newCfg = new OsVxworksConfiguration;;
	MAIN_SetCurrentCfgDeleteLast( newCfg );

	// check if project dir given on command line
	if( argc > 1 )
	{
		if( argv[1][0] == '/' )
		{
			/* absolute path given */
			mdisDir = argv[1];
		}
		else
		{
			/* relative path given */
			mdisDir = mdisDir + "/" + QString(argv[1]);
		}
	}

	wDebug(("OsVxworks::startup: try load mdisDir=%s", mdisDir.latin1() ));


	// check if started from a MDIS project dir
	QFileInfo sysdesc( mdisDir + "/" + sysDescName);
	QFileInfo makefile( mdisDir + "/" + makeName);

	if( sysdesc.exists() && makefile.exists() )
	{
		success =  G_mdiswiz->loadConfig( mdisDir );
		goto CLEANUP;
	}

	{
		// bring up initial dialog (create new/open existing)
		OsVxworksInitialDialog dlg(G_mainWindow);
		if( dlg.exec() == QDialog::Accepted )
		{

			if( dlg.action() == 0 )
			{ // create new
				success = G_mdiswiz->newConfig();
			}
			else
			{ // open existing

				// load an existing config (ask user for a directory)
				success = G_mdiswiz->loadConfig(0);
			}
		}
	}


CLEANUP:

	return success;
}


QString
OsVxworks::driverName( QString plainName, Device *dev )
{
	// ??? check!
	QString name;

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
		return QString::null;
	}

	name += plainName + ".o";
	return name.lower();
}

QString
OsVxworks::mdiswizVersion()
{
	return MdiswizView::Version() + "-" + mdiswizVxworksVersion + " Build " + __DATE__ + " " + __TIME__;
}

QString
OsVxworks::helpEnv()
{
	return " MEN_VX_DIR - path to MDIS VXWORKS ie ~/work/VXWORKS";
}

QString
OsVxworks::helpPath()
{
	return envMenVxDir + "/MDIS_DOCS/mdis.html"; //???
}

bool
OsVxworks::installDlg()
{
	Installer installer( envMenVxDir );

	return installer.guiInstall( G_mainWindow );
}

bool
OsVxworks::batchInstall( QString pkgFile, QString &errMsg  )
{
	Installer installer( envMenVxDir );

	return installer.batchInstall( pkgFile, errMsg );
}
//----------------------------------------------------------------------

//! creates the OsVxworksConfiguration object
OsVxworksConfiguration::OsVxworksConfiguration() :
	Configuration()
{
	wDebug(("OsVxworksConfiguration::OsVxworksConfiguration()"));

	// create the settings
	lstSettings.append( settingMenVxDir = new SettingMenVxDir() );
	lstSettings.append( settingMdisProjDir = new SettingMdisProjDir() );
	lstSettings.append( settingToolChain = new SettingToolChain() );
	lstSettings.append( settingAddedCflags = new StringSetting(ADDED_CFLAGS));
	lstSettings.append( settingSuppProcArch = new SettingSuppProcArch() );
	lstSettings.append( settingDbgVx = new SettingDbgVx() );
	lstSettings.append( settingDbgFlagsCompiler = new Setting(DEBUG_FLAGS) );
	lstSettings.append( settingPciSupport = new Setting("PCI_SUPPORT"));
	lstSettings.append( settingDbgMake = new Setting( MDBG ));
	lstSettings.append( settingAllCpus = new SettingAllCpusVx());
	lstSettings.connectSettingsToList();

	settingMdisProjDir->setPersist( false );
	settingPciSupport->setEnabled( Setting::Hide );

	// set setting defaults
	settingMenVxDir->setValue( envMenVxDir );
	settingMdisProjDir->setValue( envMenVxDir + "/" +  sysProjectNameDef );
	settingToolChain->setValue( "gnu" );
	settingDbgVx->setValue("yes");
	settingSuppProcArch->setValue("UP");

	settingDbgMake->setPersist( true );
	settingDbgMake->setValue( "@" );
	QString info = "Makefile debug output disable (@) or "
	               "empty to enable";
	settingDbgMake->setInfoText( &info );


	settingAddedCflags->setPersist( true );
	settingAddedCflags->setValue( "" );
	info = "Additional CFLAGS e.g. for multicore CPUs: -D_WRS_CONFIG_SMP -D_SMP_COMPATIBLE_ONLY\n"
		   "or -D_UP_COMPATIBLE_ONLY for singlecore processors.";
	settingAddedCflags->setInfoText( &info );

	settingPciSupport->setPersist( true );
	settingPciSupport->setValue( "no" );
	info = "CPU has PCIbus yes or no";
	settingPciSupport->setInfoText( &info );
	wDebug(("OsVxworksConfiguration::OsVxworksConfiguration disable PCI support"));


	settingDbgFlagsCompiler->setPersist( true );
	settingDbgFlagsCompiler->setValue( "-Wall" );
	info = "Compiler Switches for Debug Informations i.e. -g or -gdwarf\n"
		   "or -E for preprocessor -S for assembler output";
	settingDbgFlagsCompiler->setInfoText( &info );


	// set the configuration name when MDIS_PROJ_DIR is changed
	connect( settingMdisProjDir, SIGNAL( valueChanged( const QVariant & )),
			 this, SLOT( mdisProjDirChanged( const QVariant & )));

	// parse packagedb when MEN_VX_DIR changes
	connect( settingMenVxDir, SIGNAL( valueChanged( const QVariant & )),
			 this, SLOT( menVxDirChanged( const QVariant & )));

	// update properties when CPU is changed
	extern MdiswizView *G_mdiswiz;
    connect( G_mdiswiz, SIGNAL( sigCpuChanged( void )),
             this, SLOT( menCpuChanged( void )));

	SwModule *swMod;

	/* prepare default list    NATIVE TOOLS */
	wDebug(("add default NATIVE TOOLS to lstSwModules"));
	SwModuleTypes::ModType type = SwModuleTypes::NativeTool;

	swMod = new SwModule( type, "pciScanner", "NATIVE/PCISCANNER/program.mak",
						 "Get's the CPU to Device PCIbus Path Information" );
	swMod->setEnabled( false ); /*default - for CPU's without PCIbus */

	addSwMod( swMod );

	// add additional library to sw module OSS
	SwModuleList* lstSwMods = getLstSwModules();
	SwModule* oss_lib = lstSwMods->find( SwModuleTypes::CoreLib, "oss" );
	oss_lib->addMakefilePath( QString( "OSS/library_pci.mak" ) );

}

Configuration *
OsVxworksConfiguration::create()
{
	wDebug(("OsVxworksConfiguration::create()"));
	Configuration *cfg;

	cfg = new OsVxworksConfiguration();

	return cfg;
}


bool
OsVxworksConfiguration::save(QString &errMsg)
{
	bool rv1, rv2;

	if( cfgName == QString::null )
		return saveAs( errMsg );

	wDebug(("OsVxworksConfiguration::save %s", cfgName.latin1() ));

	// create directory if necessary
	QDir cfgDir(cfgName);

	if( !cfgDir.exists() )
	{
		wDebug(("OsVxworksConfiguration::save: newDir"));

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
	if(rv1 && rv2)
	{
		wDebug(("OsVxworksConfiguration::save: setCurrent()"));

	 	if (QDir::setCurrent( cfgName ) == false)
	 	{
			errMsg = "Cannot change into directory" + cfgName + "\n";
			rv1 = false;
		}
	}

	// QMessageBox::about(0, 0,cfgDir.absPath() + "    " + cfgName );

	/* check if pci domains are needed and supported */
	checkPciDomains();
		
	return rv1 && rv2;
}

bool
OsVxworksConfiguration::saveAs(QString &errMsg)
{
	QString newCfgName;

	if( (newCfgName = selectConfigName(
		"Choose Directory to save project to", false, errMsg ))
		!= QString::null) {

		// user selected a configuration name
		settingMdisProjDir->setValue( newCfgName );
		return save( errMsg );
	}
	return false;
}

bool
OsVxworksConfiguration::load( QString &errMsg, const QString &_cfgName )
{
	bool rv = true;
	QString newCfgName;

	// open an existing configuration
	if( _cfgName.isEmpty() ){
		// let user choose a configuration name
		if( (newCfgName = selectConfigName( "Open project", true, errMsg )) ==
			QString::null )
			return false;
	}
	else
		newCfgName = _cfgName;

	settingMdisProjDir->setValue( newCfgName );
	wDebug(("OsVxworksConfiguration::load: cfgName=%s", cfgName.latin1()));

	// change into the loaded project
	if( QDir::setCurrent( cfgName ) == false ){
		errMsg += QString("Cannot change into directory ") + cfgName +
			"\n";
		rv = false;
	}

	// save old files to *.bak
	wDebug(("OsVxworksConfiguration::load: backup %s and %s",
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
	// set MEN_VX_DIR + reparse package DB
	wDebug(("OsVxworksConfiguration::load: set VxDir/reparse package DB" ));
	QString vxDir = loadMakefileKeyVal( errMsg, (QString)MEN_VX_DIR );
	if( vxDir != QString::null )
	{
		// reparse from Makefile MEN_VX_DIR
		menVxDirChanged( vxDir );
	}
	else
	{
		// try to repase package DB from env
		menVxDirChanged( envMenVxDir );
	}

	// load descriptor file
	wDebug(("OsVxworksConfiguration::load: load desc" ));
	if( rv == true ){
		rv = loadDesc( sysDescName, errMsg );
	}

	if( rv == true )
	{
		// after loading DESC we know the possible CPU types
		// and can set the possible CPU core list

		VxCpuTypeMap typeMap;
		CpuDevice *root;

		root = getRootDev();
		wDebug(("OsVxworksConfiguration::load: map VxWorks CPU TYPES"));

		settingAllCpus->setChoises( vxCpuTypes( root ),
		                            QStringList(),
		                          	typeMap.keyToStr( root->getCpuCoreDefault() ) );

		BusInterface *busIf;

		for( busIf=(BusInterface *)root->firstChild();
			 busIf;
			 busIf=(BusInterface *)root->nextChild())
		{
			wDebug(("OsVxworksConfiguration::load: busIf %s", busIf->getInstName().latin1() ));
			if( busIf->getIsPci() )
			{
				settingPciSupport->setValue( "yes" );
				wDebug(("OsVxworksConfiguration::load: enable PCI support"));
			}
		}
	}


	// load Makefile
	if( rv == true )
	{
		setAllSwModulesDisabled();
		rv = loadMakefile( errMsg );
	}

	/* check if pci domains are needed and supported */
	checkPciDomains();

	return rv;
}

QString
OsVxworksConfiguration::selectConfigName( const QString &action,
										bool mustExist, QString &errMsg )
{
    Q3FileDialog fd( G_mainWindow, "file dialog", TRUE );
	QString dirName = QString::null;

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
				dirName = QString::null;
			}
		}
	}

	wDebug(("OsVxworks::selectConfigName dirName=%s", (const char *)dirName ));
	return dirName;
}

bool
OsVxworksConfiguration::loadMakefile( QString &errMsg )
{
	QString filePath = (cfgName + "/" + makeName);
	QStringList list;
	QString fullAddedCflags;
	int i;
	// Read in Makefile and convert it into MkFileTuples
	CfgMakefile mkFile( filePath );

	Q3PtrList<MkFileTuple> *lst;

	wDebug(("loadMakefile %s", filePath.latin1() ));

	try {
		lst = mkFile.read();
	}
	catch( CfgMakefile::Except &e ){
		errMsg += e.getMsg();
		return false;
	}

	bool gotSettingDbg = false;

	// process the scanned tuples
	MkFileTuple *tup;
	wDebug(( "================= loadMakefile: processing scanned tuples ============================"));
	for( tup=lst->first(); tup; tup=lst->next() )
	{
		wDebug(("loadMakefile: tup=%s", (const char *)tup->key ));

		if ( ( tup->key == "ALL_CORE_LIBS" ) &&
			 ( !(tup->val.isEmpty()) && (tup->val[0] == "no") ) )
		{
			tup->val.clear();
		}

		if( (tup->tag & MkFileTuple::TypeMask) == MkFileTuple::Assignment )
		{
			bool isSwModule=false;
			SwModuleTypes::TypeMap::Iterator it;

			// check if this tuple describes a list of sw modules
			for( it = G_swModTypes->types.begin();
				 it != G_swModTypes->types.end(); ++it )
			{

				if( tup->key == swModuleMfString( it.key() ) )
				{
					enableSwMods( it.key(), tup, errMsg );
					isSwModule=true;
				}
			}

			if( !isSwModule )
			{
				Setting *setting;

				if( (setting = lstSettings.find( tup->key )))
				{
					if (!tup->val.isEmpty() )
					{
						if( tup->key == MEN_VX_DIR )
						{
							QString vxDir = tup->val[0];
							vxDir.replace( '\\', '/' );
							tup->val[0] = vxDir;
						}

						if ( tup->key == ADDED_CFLAGS ) {
							if ( tup->val[0].contains(UP_ONLY) || tup->val[0].contains(SMP_ONLY) )
							{
								tup->val[0] = tup->val[0].mid(2); // cut the "-D"
								setting = lstSettings.find( ADDED_CFLAGS_UP_SMP );

							}
						}

						// ts: Makefile has 2 ADDED_CFLAGS lines, one with only -D_UP/SMP_COMPATIBLE_ONLY.
						//     If 'this' ADDED_CFLAGS entry is the other entry read whole line
						if (( tup->key == ADDED_CFLAGS ) && (!( tup->val[0].contains(UP_ONLY) || tup->val[0].contains(SMP_ONLY))))
						{
							wDebug((" ---- this is the other long CFLAGS line!" ));
							list = tup->val;
							fullAddedCflags = list.join(" ");
							wDebug(("fullAddedCflags = %s", (const char *)fullAddedCflags ));
							wDebug(("OSVX: setValue=%s", (const char *)fullAddedCflags ));
							setting->setValue( fullAddedCflags );

						} else {

							wDebug(("OSVX: setValue=%s", (const char *)tup->val[0] ));
							setting->setValue( tup->val[0] );

							if ( ( tup->key == "ALL_CORE_LIBS" ) &&	( tup->val[0] == "no" ) )
							{
								setting->setValue( QString(QString::null) );
							}

							setting->validate( tup->val[0], errMsg );
						}
					}

					if( setting == settingDbgVx )
						gotSettingDbg = true;
				}
				else
				{
					// unknown variable
					if( tup->key != MEN_VX_DIR
					    && tup->key != "ALL_DESC"
					    && tup->key != MDBG
					    && tup->key != DEBUG_FLAGS
					    && tup->key != ADDED_CFLAGS
					    && tup->key != "MDIS_EXTRA_DEFS"
					    && tup->key != TGT_DIR
					    && tup->key != MEN_VXWORKS_ENV_VER
					  )
				    {
						errMsg += QString("Unknown makefile variable %1\n").
							arg(tup->key);
					}
					else
					{
						const char* str = "";
						if( !tup->val.isEmpty() )
							str = (const char *)tup->val[0]; 
						wDebug(("OSVX: setValue=%s", str ));
					}
				}
			}
		}
	}
	wDebug(( "================= loadMakefile: processing scanned tuples finished ======================"));
	// minimal sanity check...
	if( !gotSettingDbg ){
		errMsg += "This is not an mdiswiz Makefile (ALL_DBGS not found)\n";
		return false;
	}

	return true;
}


QString OsVxworksConfiguration::loadMakefileKeyVal( QString &errMsg, QString key )
{
	QString env = QString::null;


	QString filePath = (cfgName + "/" + makeName);

	// Read in Makefile and convert it into MkFileTuples
	CfgMakefile mkFile( filePath );

	Q3PtrList<MkFileTuple> *lst;

	wDebug(("loadMakefileKeyVal: %s key=%s",
	        filePath.latin1(), key.latin1() ));

	try
	{
		lst = mkFile.read();
	}
	catch( CfgMakefile::Except &e )
	{
		errMsg += e.getMsg();
		return QString::null;
	}

	// process the scanned tuples
	MkFileTuple *tup;

	for( tup=lst->first(); tup; tup=lst->next() )
	{
		if( tup->key == key )
		{
			wDebug(("loadMakefileKeyVal: key %s is %s", key.latin1(), tup->val[0].latin1() ));
			env= tup->val[0];
		}
	}

	return( env );
}



//
// save all settings and sw modules to the Makefile
//
bool
OsVxworksConfiguration::saveMakefile( QString &errMsg )
{
	Setting *setting;
	Q3PtrList<MkFileTuple> tupList;
	MkFileTuple *tup;

	wDebug(("OsVxworksConfiguration::saveMakefile"));

	// Build a list of tuples
	tupList.setAutoDelete(true);

	// file header
	tup = new MkFileTuple;
	tup->tag = MkFileTuple::Comment;
	tup->val <<
		"MDIS project makefile" <<
		"Generated by mdiswiz" << targetOs()->mdiswizVersion().latin1();
	tupList.append(tup);

	// generate:
	// ifeq ($(WIND_TOOLS),)
	// 	   export MEN_VXWORKS_ENV_VER :=
	// else
	// 	   export TGT_DIR := $(WIND_BASE)/target
	// 	   export MEN_VXWORKS_ENV_VER := VXWORKS_6_0
	// endif
	tup = new MkFileTuple;
	tup->tag = MkFileTuple::Comment;
	tup->val << "Check VxWorks Develpoment Environment Version";
	tupList.append(tup);

	tup = new MkFileTuple;
	tup->tag = MkFileTuple::Ifeq;
	tup->key = "$(WIND_TOOLS),";
	/*tup->val << " ";*/
	tupList.append(tup);

	tup = new MkFileTuple;
	tup->tag = MkFileTuple::Assignment | MkFileTuple::Export | MkFileTuple::HardAssign;
	tup->key = "MEN_VXWORKS_ENV_VER";
	tupList.append(tup);

	tup = new MkFileTuple;
	tup->tag = MkFileTuple::Else;
	tupList.append(tup);

	tup = new MkFileTuple;
	tup->tag = MkFileTuple::Assignment | MkFileTuple::Export | MkFileTuple::HardAssign;
	tup->key = TGT_DIR;
	tup->val = QStringList("$(WIND_BASE)/target");
	tupList.append(tup);

	tup = new MkFileTuple;
	tup->tag = MkFileTuple::Assignment | MkFileTuple::Export | MkFileTuple::HardAssign;
	tup->key = MEN_VXWORKS_ENV_VER;
	tup->val = QStringList("VXWORKS_6_0");
	tupList.append(tup);

	tup = new MkFileTuple;
	tup->tag = MkFileTuple::Endif;
	tupList.append(tup);


	// include Windriver template
	tup = new MkFileTuple;
	tup->key = "$(WIND_BASE)/target/h/make/defs.$(WIND_HOST_TYPE)";
	tup->tag = MkFileTuple::Include;
	tupList.append(tup);

	QStringList* usedKeyList = new QStringList();
	for( setting = lstSettings.first(); setting; setting=lstSettings.next() ){

		if( setting->persist() )
		{

			if( setting->infoText() != QString::null )
			{
				tup = new MkFileTuple;

				// add a comment for this setting
				tup->val = QStringList::split( "\n", Util::breakString(
					setting->infoText(), 60));
				tup->tag = MkFileTuple::Comment;
				tupList.append(tup);
			}

			tup = new MkFileTuple;
			if( setting->value().toString() != QString::null )
			{

				if ( setting->key() == ADDED_CFLAGS_UP_SMP )
				{
					// special handling because this needs to be added to ADDED_CFLAGS key
					tup->key = ADDED_CFLAGS;
					tup->val = QStringList(QString("-D") + setting->value().toString());
					tup->tag = ( MkFileTuple::Assignment | MkFileTuple::ExtendAssign );
				}
				else
				{
					// assignment
				tup->key = setting->key();
				tup->val = QStringList(setting->value().toString());
				tup->tag = MkFileTuple::Assignment;
				}

				if( setting->key() == MDBG ) // export prefix
					tup->tag |= MkFileTuple::Export;
			}
			else
			{
				tup->val = QStringList(setting->key());
				tup->tag = MkFileTuple::Comment;
			}
			tupList.append(tup);
			usedKeyList->append(tup->key);
		}
	}

	// cleanup
	usedKeyList->clear();
	delete usedKeyList;

	// include corecomponents
	tup = new MkFileTuple;
	tup->key = "$(MEN_VX_DIR)/BUILD/MDIS/TPL/corecomp.mak";
	tup->tag = MkFileTuple::Include;
	tupList.append(tup);

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

	// file trailer
	tup = new MkFileTuple;
	tup->tag = MkFileTuple::Assignment;
	tup->key = "ALL_DESC";
	tup->val = QStringList(sysDescPlainName);
	tupList.append(tup);


	tup = new MkFileTuple;
	tup->key = "$(MEN_VX_DIR)/BUILD/MDIS/TPL/rules.mak";
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


//
// make a MkFileTuple for all SwModules in lstSwModules matching the
// type modType
//
MkFileTuple *
OsVxworksConfiguration::mkSwModuleTups( int modType )
{
	SwModule *mod;
	MkFileTuple *tup = new MkFileTuple;
	QStringList lst;

	tup->key = swModuleMfString(modType);
	tup->tag = MkFileTuple::Assignment;

	for( mod=lstSwModules.first(); mod; mod=lstSwModules.next()){
		if( mod->modType() == modType && mod->isEnabled()) {
			// USR_OSS and USR_UTL are already considered in makefile
			// templates (CORE_COMP(_RTP)). When included here, the
			// attempt to compile them for user space will fail
			if( mod->modType() != SwModuleTypes::UserLib ||
				( mod->modName() != QString("usr_oss") &&
				  mod->modName() != QString("usr_utl") ) )
			{
				const QStringList* makefileList = mod->makefilePath_all();
				for (int i=0; i<makefileList->size(); i++) {
					lst += makefileList->at(i);
				}
			}
		}
	}
	tup->val = lst;
	return tup;
}

//! convert \a Sw module type code to a string as used in Makefile
QString
OsVxworksConfiguration::swModuleMfString( int key )
{
	switch( key ){
	case SwModuleTypes::LlDriver: 	return "ALL_LL_DRIVERS";
	case SwModuleTypes::BbDriver: 	return "ALL_BB_DRIVERS";
	case SwModuleTypes::UserLib: 	return "ALL_USR_LIBS";
	case SwModuleTypes::CoreLib: 	return "ALL_CORE_LIBS";
	case SwModuleTypes::LlTool: 	return "ALL_LL_TOOLS";
	case SwModuleTypes::ComTool: 	return "ALL_COM_TOOLS";
	case SwModuleTypes::NativeDriver:return "ALL_NATIVE_DRIVERS";
	case SwModuleTypes::NativeTool:  return "ALL_NATIVE_TOOLS";
	case SwModuleTypes::NativeLib:   return "ALL_NATIVE_LIBS";
	default: 					return QString::null;
	}
}

//
// lookup all makefilePaths contained in <tup> within the current
// configuration's swModule list. If a SwModule exists with a matching
// module type and makefile path, set the state of this module to enabled
// If the makefilePath is unknown, add a warning to <errMsg>
void
OsVxworksConfiguration::enableSwMods( int modType, MkFileTuple *tup,
									QString &errMsg )
{

	for ( QStringList::Iterator it = tup->val.begin();
		  it != tup->val.end();
		  ++it
		)
	{

		SwModule *mod = lstSwModules.findMake( modType, *it );

		if( mod )
			mod->setEnabled( true );
		else
			errMsg += QString("No such SW module with makefile %1 "
							  "(ignored)\n").arg(*it);
	}
}

bool
OsVxworksConfiguration::settingsDlg()
{
	CfgSwModuleTab *modTab;
	OsVxworksTab *osTab;

	wDebug(("OsVxworks::settingsDlg"));
	SettingsDlg dlg( G_mainWindow, this );
	osTab = new OsVxworksTab( this, G_mainWindow, 0);
	dlg.addTab( osTab, "Project Settings");
	dlg.connect( &dlg, SIGNAL(applyButtonPressed()), osTab,
				 SLOT(slotApply()));
	dlg.connect( &dlg, SIGNAL(cancelButtonPressed()), osTab,
				 SLOT(slotCancel()));

	// remove User libs
	SwModule *sw;
	sw= lstSwModules.find( SwModuleTypes::UserLib, "usr_oss" );
	if ( sw )
		lstSwModules.remove(sw);
	sw= lstSwModules.find( SwModuleTypes::UserLib, "usr_utl" );
	if ( sw )
		lstSwModules.remove(sw);

	modTab = new CfgSwModuleTab( G_mainWindow, 0,
								 this );

	dlg.addTab( modTab, "Software Modules");
	dlg.connect( &dlg, SIGNAL(applyButtonPressed()), modTab,
				 SLOT(slotApply()));
	dlg.setButtons();
	return (dlg.exec() == QDialog::Accepted );
}

bool
OsVxworksConfiguration::newProjectWizard( QWidget *parent,
										const char *name)
{
	OsVxworksNewProjectWizard wizard( this, parent, name );

	return wizard.exec();
}


//! called whenever \c MDIS_PROJ_DIR is changed
/*!
  sets the configurations name to the new value
  \param newVal new value of MDIS_PROJ_DIR
*/
void
OsVxworksConfiguration::mdisProjDirChanged( const QVariant &newVal )
{
	QString projDir = newVal.toString();

	QString expVal = lstSettings.expandedValueOf( newVal.toString());
	wDebug(( "MdisProjDirChanged %s", (const char *)expVal ));

	cfgName = expVal;
}

//! called whenever \c MEN_VX_DIR is changed
/*!
  sets value of envMenVxDir
  reparses the package data base
  \param newVal new value of MEN_VX_DIR
*/
void
OsVxworksConfiguration::menVxDirChanged( const QVariant &newVal )
{
	extern MdisModelFactory *G_mdisMods;

	wDebug(("menVxDirChanged"));

	if( newVal.toString() != QString::null )
	{
		if( envMenVxDir != newVal.toString() )
		{
			envMenVxDir = newVal.toString();
			envMenVxDir.replace( '\\' ,'/' );
			wDebug(("menVxDirChanged %s", envMenVxDir.latin1()));
		}

		// parse the model data base (ignore errors)
		PackageDb pkgdb( lstSettings.expandedValueOf( targetOs()->pkgDbPath() ) );
		pkgdb.parse( G_mdisMods );
	}
}

void OsVxworksConfiguration::menCpuChanged( )
{
	wDebug(("menCpuChanged"));

	// refresh the list of cpu cores
	CpuDevice *root = getRootDev();
	VxCpuTypeMap typeMap;

	settingAllCpus->setChoises( vxCpuTypes( root ),
		                        QStringList(),
		                        typeMap.keyToStr( root->getCpuCoreDefault() ) );
	
	// check if pci Domains are supported
	checkPciDomains();
}

bool
OsVxworksConfiguration::validateSwModule(
	SwModule *mod,
	QString &errMsg,
	bool disableIfInvalid )
{
	int rv = true;

	// no check if module is not enabled
	if( !mod->isEnabled() )
		return rv;

	if( G_swModTypes->types.contains( mod->modType() )){

		// check if the swmodule's makefile exists ??? settingMenVxDir
		const QStringList* makeNameList = mod->makefilePath_all();

		for (int i=0; i<makeNameList->size(); i++)
		{
			QString makeName = lstSettings.expandedValueOf( envMenVxDir + "/" +
			G_swModTypes->types[mod->modType()].natPath + "/" +
			makeNameList->at(i) );

			QFileInfo fi( makeName );

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

//----------------------------------------------------------------------
// New project wizard

//! create the new project wizard for Vxworks
/*!
  execute with the exec() method
*/
OsVxworksNewProjectWizard::OsVxworksNewProjectWizard(
	OsVxworksConfiguration *_cfg, QWidget *parent, const char *name) :
	Q3Wizard( parent, name, TRUE )
{
	QWidget *wdg;
	Setting *setting;
	QString title;

	resize( 400, 300 ); // make it big. looks better
	cfg = _cfg;

	// make a copy of the current settings
	// these will be restored when user cancels wizard
	savedSettings = cfg->lstSettings.clone();

	int numPages=0;
	allCpusWdg = 0;

	// build one page for each setting
	for( setting = cfg->lstSettings.first();
		 setting;
		 setting= cfg->lstSettings.next() ){

		wdg = setting->wizardPage( this, &title );
		if( wdg ){
			wDebug((" add page %p (%s)", wdg, (const char *)setting->key()));
			addPage( wdg, title );
			numPages++;
			setHelpEnabled( wdg, false );

			if( setting == cfg->settingAllCpus )
				allCpusWdg = wdg;
		}
	}

	// Add page to select CPU board:
	cpuWdg = new AddComponentWidget(this, 0,
		"Please select your target CPU board from the list below:");

	insertPage( cpuWdg, "Select a CPU board", numPages-1 );
	setHelpEnabled( cpuWdg, false );
}

//! destructs the project wizard
OsVxworksNewProjectWizard::~OsVxworksNewProjectWizard()
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
OsVxworksNewProjectWizard::next()
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
OsVxworksNewProjectWizard::done( int r )
{
	extern CpuModelFactory *G_cpuMods;

	if( r == QDialog::Accepted ){
		if( validateSetting() == false )
			return;

		// Add cpu device to configuration
		QString cpuName = cpuWdg->result();
		if( cpuName != QString::null ){
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
OsVxworksNewProjectWizard::validateSetting()
{
	Setting *setting;

	wDebug(("OsVxworksNewProjectWizard::validateSetting"));

	// call slotApply() of the currently displayed setting
	if( currentPage())
	{
		wDebug(("OsVxworksNewProjectWizard::validateSetting: currentPage"));

		for( setting = cfg->lstSettings.first();
			 setting;
			 setting = cfg->lstSettings.next() )
		{
		wDebug(("OsVxworksNewProjectWizard::validateSetting: currentPage: for"));

			if( setting->currentWidget() == currentPage() )
			{
				wDebug(("OsVxworksNewProjectWizard::validateSetting: currentPage: for: break"));
				break;
			}
		}

		// not all pages belong to a setting, so its not an error
		// if setting is 0 here

		if( setting )
		{
			QString errMsg;
			wDebug(("OsVxworksNewProjectWizard::validateSetting: %s",
				   (const char *)setting->key()));
			setting->slotApply( errMsg );

			if( !errMsg.isEmpty() )
			{
				QMessageBox::critical( this,
									   cfg->msgBoxCaption(),
									   errMsg,
									   QMessageBox::Ok,
									   QMessageBox::NoButton);
				return false;
			}
		}
		else
		{
			wDebug(("OsVxworksNewProjectWizard::validateSetting: no setting"));
		}
	}
	return true;
}

//! reimplemented from QWizard
/*!
*/
void
OsVxworksNewProjectWizard::showPage( QWidget* page )
{
	extern CpuModelFactory *G_cpuMods;
	VxCpuTypeMap typeMap;

	// if this is the CPU selection page,
	// make a list of CPU boards matching kernel arch ???
	if( page == cpuWdg )
	{
		wDebug(("OsVxworksNewProjectWizard::showPage: about to show cpuWdg\n"));

		cpuWdg->clearItems();

        // add CPU devices to list
		for( CpuDevice *dev=(CpuDevice *)G_cpuMods->first(); dev;
			 dev=(CpuDevice *)G_cpuMods->next() )
		{

			// sort out CPU extension boards / ESM carriers
			if( cfg->cpuOkForCfg( dev ) )
			{
				cpuWdg->addItem( dev->getModelName(), dev->getDescription() );
			}
		}
	}
	else if( page == allCpusWdg )
	{
		CpuDevice *dev;
		// build list of suitable CPU types...
		wDebug(("OsVxworksNewProjectWizard::showPage: about to show allCpusWdg\n"));
		dev = (CpuDevice *)G_cpuMods->find( cpuWdg->result() );

		WIZ_ASSERT(dev);
		wDebug(("OsVxworksNewProjectWizard::showPage: select CPU Type\n"));
		cfg->settingAllCpus->setChoises( vxCpuTypes( dev ),
		                                 QStringList(),
		                                 typeMap.keyToStr( dev->getCpuCoreDefault() )  );


		BusInterface *busIf;

		for( busIf=(BusInterface *)dev->firstChild();
			 busIf;
			 busIf=(BusInterface *)dev->nextChild())
		{
			wDebug(("OsVxworksConfiguration::load: busIf %s", busIf->getInstName().latin1() ));
			if( busIf->getIsPci() )
			{
				cfg->settingPciSupport->setValue( "yes" );
				wDebug(("OsVxworksNewProjectWizard::showPage: enable PCI support"));
			}
		}


	}
	else
	{
		wDebug(("OsVxworksNewProjectWizard::showPage: nothing\n"));
	}

	wDebug(("OsVxworksNewProjectWizard::showPage: showPage\n"));
	Q3Wizard::showPage(page);

	wDebug(("OsVxworksNewProjectWizard::showPage: finishButton\n"));

	if( finishButton() )
		finishButton()->setEnabled( true );
}

//----------------------------------------------------------------------
//! creates the OsVxworksTab
OsVxworksTab::OsVxworksTab( OsVxworksConfiguration *_cfg, QWidget *parent,
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
OsVxworksTab::~OsVxworksTab()
{
	delete savedSettings;
}

//! called when apply or Ok button pressed in dialog
/*!
  calls the Setting::slotApply function of all underlying settings.
*/
void
OsVxworksTab::slotApply()
{
	wDebug(("OsVxworksTab::slotApply"));
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
OsVxworksTab::slotCancel()
{
	wDebug(("OsVxworksTab::slotCancel"));

	// restore original settings
	cfg->lstSettings.takeValues( savedSettings );
}


//----------------------------------------------------------------------
//! sets up the toolbar and project menu
void
VxworksMdiswiz::setupProjectActions()
{
	QAction *a;

	MdiswizView::setupProjectActions();

	// add vxworks specific entries

	// generate build menu
	Q3PopupMenu *buildMenu = new Q3PopupMenu( this );
	menu->insertItem( "&Build", buildMenu, -1, 1 );

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
}

void VxworksMdiswiz::doBuild( const QString &title, const QStringList &args )
{
	if( !buildDlg ){
		// make sure configuration saved
		slotSaveConfig();

		// make sure to be in correct directory (CONFIG DIR)
		buildDlg = new OsVxworksExecProgram(
			MAIN_GetCurrentCfg()->msgBoxCaption(),
			title,
			this,
			"buildDlg",
			false);
		buildDlg->show();

		buildDlg->start( args );

		connect( buildDlg, SIGNAL(finished()),
				 this, SLOT(slotBuildFinished()));
	}
}

//! build project
void VxworksMdiswiz::slotBuild()
{
	QStringList args;

	args << "make";

	doBuild( "Building Project (make)", args );
}

//! rebuild project
void VxworksMdiswiz::slotRebuild()
{
	QStringList args;

	args << "sh" << "-c" << "make clean; make"; //??? make clean

	doBuild( "Rebuilding Project (make clean; make)", args );
}

//! called when build process has been exited
void VxworksMdiswiz::slotBuildFinished()
{
	if( buildDlg ){
		int status = buildDlg->status();
		delete buildDlg;
		buildDlg = 0;

		if( status == 0 ){
			QMessageBox::information(
				this,
				MAIN_GetCurrentCfg()->msgBoxCaption(),
				"Build completed with success.\n"
				"The MDIS object binary can now be loaded or linked to your BSP. See MDIS user guide!", //???
				QMessageBox::Ok,
				QMessageBox::NoButton);
		}
	}
}



