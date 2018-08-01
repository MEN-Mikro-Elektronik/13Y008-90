/***************************************************************************
 */
/*!       \file  osqnx.cpp
 *       \brief  Implementation of class OsQnx
 *      \author
 *		  $Date: 2011/04/12 09:18:38 $
 *    $Revision: 2.6 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: osqnx.cpp,v $
 * Revision 2.6  2011/04/12 09:18:38  CRuff
 * R: 1. warn user if installed MDIS system package and selected cpu do not match
 * M: 1a) check pci domain compatibility in Configuration::save(), load() and
 *        in slot menCpuChanged()
 *    1b) added methods mdisSupportsPciDomains() and checkPciDomains to OS
 *        specific configuration class
 *
 * Revision 2.5  2011/02/16 17:04:04  CRuff
 * R: new revision
 * M: changed revision string
 *
 * Revision 2.4  2010/10/22 11:52:43  CRuff
 * R: release of new version
 * M: increased version string
 *
 * Revision 2.3  2010/10/22 10:54:26  CRuff
 * R. 1. cosmetics
 * M: 1. in load(): do not use format string for error messages, but QString
 *       operator +
 *
 * Revision 2.2  2010/06/21 10:55:22  CRuff
 * R: release of new version
 * M: increase version number
 *
 * Revision 2.1  2010/06/16 11:45:15  CRuff
 * R: 1. Qt 4.6.0 does not allow to access empty String Lists
 *    2. bug in QStringList handling
 * M: 1. in loadMakefile(), add checks if StringList is empty before accessing it
 *    2. validateSwModule: dot not dereference StringList element, is returned
 *       as reference
 *
 * Revision 2.0  2010/01/22 11:27:11  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.14  2009/06/30 08:22:10  CRuff
 * R: changes of revision 1.12 not necessary
 * M: undo changes of revison 1.12
 *
 * Revision 1.13  2009/06/26 09:36:35  channoyer
 * R: PACKAGE_DESC directory should be in root directory of packages
 * M: Remove /src/ directory in drivers DB path
 *
 * Revision 1.12  2009/05/13 11:33:23  CRuff
 * R: 1. Core lib 'oss' needs to pull in additional makefile for VxWorks
 *    2. Symbol name ALL_CORE in generated Makefile causes cyclic dependency
 * M: 1. extended mkSwModulesTups(), validateSwModule() to handle sw modules
 *       with multiple makefiles
 *    2. Rename symbol ALL_CORE to ALL_CORE_LIBS in
 *       OsLinuxConfiguration::loadMakefile(), ::swModuleMfString()
 *
 * Revision 1.11  2009/04/30 21:23:50  CRuff
 * R: release MDIZWIZ
 * M: increased version number
 *
 * Revision 1.10  2009/04/30 20:45:12  CRuff
 * R: no configurability for kernel libs oss and dbg for qnx in this version
 * M: removed oss and dbg selection possibility from project settings dialog
 *
 * Revision 1.9  2009/04/30 17:29:48  CRuff
 * R: make Kernel modules OSS and DBG configurable
 * M: set value for CoreLib flag ALL_CORE to 'no' if no module is selected
 *
 * Revision 1.8  2009/04/30 11:12:48  CRuff
 * R: The software modules selection in the project settings dialog for usr_oss
 *    and usr_utl has no effect
 * M: Removed the selection option for those modules from the project settings
 *    dialog
 *
 * Revision 1.7  2009/04/29 11:45:28  CRuff
 * R: make Kernel modules OSS and DBG configurable
 * M: added handling for new moduleType SwModuleType::CoreLib to methods
 *    OsQnxConfiguration::saveMakefile() and
 *    OsQnxConfiguration::swModuleMfString()
 *
 * Revision 1.6  2008/11/06 10:09:55  ch
 * R: QNX: Missing ARCH directory for the target files
 * M: QNX: During Makefile generation place the ARCH variable before QNX_TARGET_DIR
 *
 * Revision 1.5  2008/10/27 12:30:08  channoyer
 * R:1. Remaining wrong box names
 *   2. The screen position of some dialog box was not relative to the main window
 * M:1. Call setCaption with G_progName constant
 *   2. Change parent to G_mdiswiz
 *
 * Revision 1.4  2008/10/23 18:04:18  channoyer
 * R:1. Some of the configuration windows are not the QNX specific but from Linux
 *   2. The Makefile parameter QNX_TARGET_DIR should be modifiable in the wizard
 *   3. The screen position of some dialog box was not relative to the main window
 *   4. Multiple Next/Back on the CPU page duplicate the CPU list
 *   5. Program name should be "MDIS Wizard" when displayed to the user
 *   6. Langage and cosmetic
 * M:1. Change the name of the some class to QNX specific name
 *   2. Add and use the class SettingQnxTargetDir
 *   3. Change parent to G_mdiswiz
 *   4. Call clearItems when create the CPU list
 *   5. Use of the G_progName constant
 *   6. Language update of text displayed to the user
 *
 * Revision 1.3  2008/07/31 16:24:36  gvarlet
 * R: Path to reach the TOOLS/NATIVE dir was not set
 * M: Added path the TOOLS/NATIVE dir
 *
 * Revision 1.2  2008/07/31 14:36:02  dpfeuffer
 *  R:1. Windows compiler warning: 'swMod' : unreferenced local variable
 *  M:1. Unused 'swMod' removed
 *
 * Revision 1.1  2008/07/23 11:49:47  gvarlet
 * Initial Revision
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
/*
  Definitions:
  - MEN_QNX_DIR is stored in Makefile.
  - setting dialog for MEN_QNX_DIR
  - If environment MEN_QNX_DIR exists, it is used
  - NT descgen binary which ignores _WIZ_ keys

  Initial Dialog
  - MEN_QNX_DIR
  - MDIS_PROJ_DIR
  - ALL_DBGS
  - Select Achitecture via CPU choice
    - Set "ARCH" dependent on Achitecture ( x86 - ppcbe )
*/
/*
  TBD:

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

#include <Q3HBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <Q3PopupMenu>
#include <Q3VBoxLayout>
#include <Q3PtrList>

#include "configuration.h"
#include "hwcomponent.h"
#include "osqnx.h"
#include "cfgmakefile.h"
#include "swmodulestab.h"
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
#include "wizdebug.h"

// pixmaps
#include "images/build.xpm"
#include "images/rebuild.xpm"

extern QWidget         *G_mainWindow; //!< used as anchor for dialog
extern QApplication    *G_qApp;       //!< main application

extern const QString	G_progName;     //!< main application name

// strings
static const QString	mdiswizQnxVersion("qnx-3.2");
static const QString	sysDescPlainName("system");
static const QString	sysDescName( sysDescPlainName + ".dsc");
static const QString	makeName("Makefile");
static const QString	packageDbPath("PACKAGE_DESC");
static 		 QString	envMenQnxDir;
static const char 		MEN_QNX_DIR[] = "MEN_QNX_DIR";
static const QString	sysProjectNameDef("/src/BUILD/MDIS/myMdisProject");


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

				errMsg +=	"\"" + dir + "\" is an existing,"
							" non-empty directory!\n";
				return false;
			}
		}
		else {
			errMsg +=	"\"" + dir + "\" is an existing"
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
OsQnxConfiguration::mdisSupportsPciDomains( )
{
	QFileInfo mdisVersFile( envMenQnxDir + QString("/src/INCLUDE/NATIVE/MEN/mdis_version.h"));
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
OsQnxConfiguration::checkPciDomains()
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

// -----------------------------------------------------------------
//! QNX directory setting class
class SettingMenQnxDir : public DirectorySetting
{
public:
	SettingMenQnxDir( void ) : DirectorySetting(
		MEN_QNX_DIR,
		DirectorySetting::Dir )
	{
	}

	virtual Setting *clone( void )
	{
		SettingMenQnxDir *setting = new SettingMenQnxDir();
		copy( setting );
		return setting;
	}

	virtual bool validate( const QVariant &newVal, QString &errMsg )
	{
		QString qnxDir = lst->expandedValueOf( newVal.toString() );
		bool 	rv = true;

		QFileInfo fi1( qnxDir + "/src/BUILD/MDIS/TPL/rules.mak" );

		if( !fi1.exists() ){
			errMsg += "\"" + qnxDir +
				"\" does not look like a QNX MDIS directory (" +
				fi1.filePath() + " not found)\n";
			rv = false;
		}
		return rv;
	}

protected:
	QString infoText( void )
	{
		return
			"The directory where you have installed the MDIS system "
			"package for QNX, e.g. /MEN/QNX.\n"
			"Note: FORWARD SLASHES '/' needed.";
	}

	QString labelText( void )
	{
		return "MDIS system package install directory";
	}

	QString wizPageTitle( void )
	{
		return "Select the MDIS system package install directory";
	}

};

// -----------------------------------------------------------------
//! QNX directory setting class
class SettingQnxTargetDir : public DirectorySetting
{
public:
	SettingQnxTargetDir( void ) : DirectorySetting(
		"QNX_TARGET_DIR",
		DirectorySetting::Dir )
	{
	}

	virtual Setting *clone( void )
	{
		SettingQnxTargetDir *setting = new SettingQnxTargetDir();
		copy( setting );
		return setting;
	}

	virtual bool validate( const QVariant &newVal, QString &errMsg )
	{
		QString qnxDir = lst->expandedValueOf( newVal.toString() );
		bool 	rv = true;

		QFileInfo fi1( qnxDir );

		if( !fi1.exists() || !fi1.isDir() ){
			errMsg += "\"" + qnxDir +
				"\" does not look like a QNX target directory (" +
				fi1.filePath() + " not found)\n";
			rv = false;
		}
		return rv;
	}

protected:
	QString infoText( void )
	{
		return
			"This directory is used to install the binaries for the target.\n"
			"e.g.: $(QNX_TARGET)/$(ARCH) if you want to install them directly "
			"on your QNX target tree.";
	}

	QString labelText( void )
	{
		return "QNX target directory";
	}

	QString wizPageTitle( void )
	{
		return "Select the QNX target directory";
	}

};

// -----------------------------------------------------------------
//! MDIS directory setting class
class SettingMdisQnxProjDir : public DirectorySetting
{
public:
	SettingMdisQnxProjDir( void ) : DirectorySetting(
	"MDIS_PROJ_DIR",
	DirectorySetting::Dir )
	{
	}

	virtual Setting *clone( void )
	{
		SettingMdisQnxProjDir *setting = new SettingMdisQnxProjDir();
		copy( setting );
		return setting;
	}

	// dont't show in Setting dialog
	virtual QWidget *settingsTabWidget( QWidget *parent )
	{
		UTIL_UNREF_PARAM(parent);
		return 0;
	}

	virtual bool validate( const QVariant &newVal, QString &errMsg )
	{
		QString expVal = lst->expandedValueOf( newVal.toString() );
		bool rv;

		if(( rv = isGoodMdisProjDir( expVal, errMsg ) ) == false )
			errMsg = key() + ": " + errMsg;

		return rv;
	}

protected:
	QString infoText( void )
	{
		return
			"This directory is used to store the configuration that you "
			"create using MDIS Wizard.\n"
			"The Makefile and descriptor file \"system.dsc\" will be saved to "
			"this directory.\n"
			"The modules and binaries will be compiled into this directory "
			"before they are installed.";
	}

	QString labelText( void )
	{
		return "MDIS project directory";
	}

	QString wizPageTitle( void )
	{
		return "Select the project directory to store the MDIS configuration";
	}

};

// -----------------------------------------------------------------
//! Choose either a debug nor a non-debug building
class SettingDbgQnx : public MultipleChoiseSetting {
public:
	enum Choises { No=0, Yes };

	SettingDbgQnx() : MultipleChoiseSetting( "ALL_DBGS" ) {
		choises << "nodbg" << "dbg";
		choisesHelp <<
			"Do not include debug strings. Run at full speed." <<
			"Include debug strings. Modules run slower.";

		_value = QVariant( choises[Yes] );
	}

	virtual Setting *clone(){
		SettingDbgQnx *setting = new SettingDbgQnx();
		copy( setting );
		return setting;
	}

protected:
	QString infoText() {
		return
			"Define whether to build and install the release (nodbg) or "
			"debug (dbg) versions of the modules. The debug version "
			"of the modules issue debug messages for "
			"troubleshooting. The message display command is sloginfo.";
	}
	QString labelText() {
		return "Build mode for MDIS modules";
	}
	QString wizPageTitle() {
		return "Select build mode for MDIS modules";
	}
};

// -----------------------------------------------------------------
//! Choose either a static nor a shared library build mode
class SettingQnxLibMode : public MultipleChoiseSetting {
public:
	enum Choises { Static=0, Shared };

	SettingQnxLibMode() : MultipleChoiseSetting( "LIB_MODE" ) {
		choises << "static" << "shared";
		choisesHelp <<
			"make static libs / link programs with static libs." <<
			"make shared libs / link programs for use with shared libs.";

		_value = QVariant( choises[Shared] );
	}

	virtual Setting *clone(){
		SettingQnxLibMode *setting = new SettingQnxLibMode();
		copy( setting );
		return setting;
	}

protected:
	QString infoText() {
		return
			"Define whether to build/use static or shared user state libraries. "
			"This affects only ALL_*_TOOLS and the libraries "
			"specified by ALL_USR_LIBS.";
	}
	QString labelText() {
		return "Library build mode for MDIS Libraries";
	}
	QString wizPageTitle() {
		return "Select build mode for MDIS Libraries";
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
class OsQnxInitialDialog : public QDialog
{
public:
	//! creates the add component dialog
	OsQnxInitialDialog(
		QWidget *parent,
		const char *name=0,
		bool modal=TRUE,
		Qt::WFlags f=0) : QDialog( parent, name, modal, f )
	{
		// set the widgets
		Q3VBoxLayout *vb = new Q3VBoxLayout( this, 10 );
		QPushButton *but;
		QLabel 		*lab;

		lab = new QLabel(
			"<b>Welcome to the " + G_progName + " for QNX!</b>",
			this );
		vb->addWidget( lab );

		lab = new QLabel(
			"This tool allows you to easily configure MDIS devices.\n\n"
			"Please choose your initial action:", this );
		vb->addWidget( lab );

	    buttonGroup = new Q3ButtonGroup(
			1, Qt::Horizontal,
			QString::null,
			this );
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
	int action( void )
	{
		return buttonGroup->id( buttonGroup->selected());
	}

private:
	Q3ButtonGroup *buttonGroup;

};

//----------------------------------------------------------------------
//! creates OsQnx object
/*!
*/
OsQnx::OsQnx( void ) : TargetOs( OsFactory::Qnx )
{
	// check if environment variable MEN_QNX_DIR is present
	envMenQnxDir = getenv( MEN_QNX_DIR );
	envMenQnxDir.replace( '\\' , '/' );
	wDebug(( "OsQnx MEN_QNX_DIR=%s", envMenQnxDir.latin1() ));

}

bool OsQnx::batchInstall( QString pkgFile, QString &errMsg  )
{
	Installer installer( envMenQnxDir );
	return installer.batchInstall( pkgFile, errMsg );
}

//! Creates a new configuration
Configuration* OsQnx::createConfiguration( void )
{
	wDebug(("OsQnx::createConfiguration"));
	return new OsQnxConfiguration;
}

QString OsQnx::driverName( QString plainName, Device *dev )
{
	// ??? check!
	QString name;

	wDebug(( "OsQnx::driverName" ));

	switch( dev->type() ){
		case Device::Mdis:
			MdisDevice *mdev;
			WIZ_DYNAMIC_CAST( dev, mdev, MdisDevice *);
			if( !mdev->isNativeDevice() )
				name += "";
			break;

		case Device::Bbis:
			name += "";
			break;

		default:
			wDebug(("OsQnx::driverName : Not a MDIS driver" ));
			return QString::null;
	}

	name += plainName + "";
	return name.lower();
}

//! Return the MEN_QNX_DIR in cmd line help.
QString OsQnx::helpEnv( void )
{
	return "MEN_QNX_DIR - path to MDIS QNX ie ~/work/QNX";
}

//! Return the documentation path in cmd line help.
QString OsQnx::helpPath( void )
{
	return envMenQnxDir + "/src/MDIS_DOCS/mdis.html"; //???
}

//! Enable the driver installation in the wizard
bool OsQnx::installDlg( void )
{
	extern MdiswizView *G_mdiswiz;

	Installer installer( envMenQnxDir + "/src" );
	return installer.guiInstall( G_mdiswiz );
}

//! Return the wizard version
QString OsQnx::mdiswizVersion( void )
{
	return MdiswizView::Version() + "-" + mdiswizQnxVersion;
}

//! Set the drivers DB path
QString OsQnx::pkgDbPath( void )
{
	return envMenQnxDir + "/src/" + packageDbPath;
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
bool OsQnx::startup( int argc, char **argv )
{
	extern	MdiswizView *G_mdiswiz;
	bool	success = false;
	QString	errMsg;
	QString	mdisDir = QDir::currentDirPath();

	wDebug(( "OsQnx::startup()" ));
	// remove unused SW module types
	G_swModTypes->types.remove( SwModuleTypes::Other );
	G_swModTypes->types.remove( SwModuleTypes::CbDriver );

	// patch QNX specific paths
	G_swModTypes->types[ SwModuleTypes::NativeTool ].natPath = "TOOLS";

	// must create a temporary configuration, it is immediately
	// destroyed by MdiswizView::loadConfig() or newConfig()
	OsQnxConfiguration *newCfg = new OsQnxConfiguration;
	MAIN_SetCurrentCfgDeleteLast( newCfg );

	// check if project dir given on command line
	if( argc > 1 ){
		if( argv[1][0] == '/' ){
			/* absolute path given */
			mdisDir = argv[1];
		}
		else{
			/* relative path given */
			mdisDir = mdisDir + "/" + QString(argv[1]);
		}
	}

	wDebug(( "OsQnx::startup: try load mdisDir=%s", mdisDir.latin1() ));

	// check if started from a MDIS project dir
	QFileInfo sysdesc( mdisDir + "/" + sysDescName );
	QFileInfo makefile( mdisDir + "/" + makeName );

	if( sysdesc.exists() && makefile.exists() )
	{
		success =  G_mdiswiz->loadConfig( mdisDir );
		goto CLEANUP;
	}

	{
		// bring up initial dialog (create new/open existing)
		OsQnxInitialDialog dlg(G_mdiswiz);
        dlg.setCaption(G_progName);
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

//----------------------------------------------------------------------
//! creates the OsQnxConfiguration object
OsQnxConfiguration::OsQnxConfiguration( void ) : Configuration()
{
	wDebug(("OsQnxConfiguration::OsQnxConfiguration()"));

	// create the settings
	lstSettings.append( settingMenQnxDir 	= new SettingMenQnxDir() );
	lstSettings.append( settingMdisProjDir	= new SettingMdisQnxProjDir() );
	lstSettings.append( settingCpuArch		= new Setting( "ARCH" ) );
	lstSettings.append( settingTargetDir 	= new SettingQnxTargetDir() );
	lstSettings.append( settingBinDir		= new Setting( "BIN_INSTALL_DIR" ));
	lstSettings.append( settingLibDir		= new Setting( "LIB_INSTALL_DIR" ));
	lstSettings.append( settingStaticLibDir	= new Setting(
													"STATIC_LIB_INSTALL_DIR" ));
	lstSettings.append( settingDescDir		= new Setting(
													"DESC_INSTALL_DIR" ));
	lstSettings.append( settingLibMode		= new SettingQnxLibMode() );
	lstSettings.append( settingVmeSupport	= new Setting( "USE_VME_SERVER" ));
	lstSettings.append( settingDbgQnx		= new SettingDbgQnx() );
	lstSettings.connectSettingsToList();

	// Set properties for hidden and persistant settings
	settingMdisProjDir->setPersist( false );
	settingVmeSupport->setEnabled( Setting::Hide );
	settingVmeSupport->setPersist( true );
	settingLibDir->setEnabled( Setting::Hide );
	settingLibDir->setPersist( true );
	settingCpuArch->setEnabled( Setting::Hide );
	settingCpuArch->setPersist( true );
	settingStaticLibDir->setEnabled( Setting::Hide );
	settingStaticLibDir->setPersist( true );
	settingDescDir->setEnabled( Setting::Hide );
	settingDescDir->setPersist( true );

	// set default directories
	settingMenQnxDir->setValue( envMenQnxDir );

	settingMdisProjDir->setValue( "$(MEN_QNX_DIR)"  +  sysProjectNameDef );

	QString info =
		"define the directory in which the libraries (static/shared)\n"
		"are to be installed\n"
		"Usually this is the target's /usr/local/lib/mdis directory";
	settingLibDir->setInfoText( &info );
	settingLibDir->setValue( "$(QNX_TARGET_DIR)/usr/lib/mdis" );

	info =
		"define the directory in which the executable binaries\n"
		"are to be installed\n"
		"Usually this is the target's /usr/bin\n"
		"or /usr/local/bin directory\n";
	settingBinDir->setInfoText( &info );
	settingBinDir->setValue( "$(QNX_TARGET_DIR)/usr/bin" );

	settingTargetDir->setValue( "$(MEN_QNX_DIR)/qnx6/$(ARCH)" );

	settingStaticLibDir->setValue( "$(LIB_INSTALL_DIR)" );

	settingDescDir->setValue( "$(QNX_TARGET_DIR)/etc/mdis" );

	// set default dbg value
	settingDbgQnx->setValue("dbg");

	// set default VME support
	info = "CPU has VMEbus yes or no";
	settingVmeSupport->setInfoText( &info );
	setVmeSupport( "no" );
	wDebug(( "OsQnxConfiguration::OsQnxConfiguration disable VME support" ));

	// Set default achitechture
	info =
		"Processor architecture to create binaries for ";
	settingCpuArch->setInfoText( &info );
	settingCpuArch->setValue( "x86" );

	// set the configuration name when MDIS_PROJ_DIR is changed
	connect( settingMdisProjDir, SIGNAL( valueChanged( const QVariant & )),
			 this, SLOT( mdisProjDirChanged( const QVariant & )));

	// parse packagedb when MEN_QNX_DIR changes
	connect( settingMenQnxDir, SIGNAL( valueChanged( const QVariant & )),
			 this, SLOT( menQnxDirChanged( const QVariant & )));

	// update properties when CPU is changed
	extern MdiswizView *G_mdiswiz;
	connect( G_mdiswiz, SIGNAL( sigCpuChanged( void )),
			 this, SLOT( menCpuChanged( void )));
}

//! Create the configuration
Configuration *OsQnxConfiguration::create( void )
{
    wDebug(("OsQnxConfiguration::create()"));

	Configuration *cfg;
	cfg = new OsQnxConfiguration();

	return cfg;
}

//
// lookup all makefilePaths contained in <tup> within the current
// configuration's swModule list. If a SwModule exists with a matching
// module type and makefile path, set the state of this module to enabled
// If the makefilePath is unknown, add a warning to <errMsg>
void OsQnxConfiguration::enableSwMods(
	int modType,
	MkFileTuple *tup,
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

bool OsQnxConfiguration::load( QString &errMsg, const QString &_cfgName )
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
	wDebug(("OsQnxConfiguration::load: cfgName=%s", cfgName.latin1()));

	// change into the loaded project
	if( QDir::setCurrent( cfgName ) == false ){
		errMsg += QString("Cannot change into directory ") + cfgName +
			"\n";
		rv = false;
	}

	// save old files to *.bak
	wDebug(("OsQnxConfiguration::load: backup %s and %s",
			sysDescName.latin1(), makeName.latin1()));
	{
		QFile file;

		file.setName( cfgName  + "/" + sysDescName + ".bak" );
		// try to remove old backup file
		if( file.exists() && !file.remove() ) {
			errMsg +=
				QString("Cannot remove old backup file ") + file.name() + "\n\n";
			rv = false;
		} else {
			Util::fileCopy( QString( cfgName  + "/" + sysDescName ),
							QString( file.name() ),
							errMsg );
		}

		file.setName( cfgName  + "/" + makeName + ".bak" );
		// try to remove old backup file
		if( file.exists() && !file.remove() ) {
			errMsg +=
				QString("Cannot remove old backup file ") + file.name() + "\n\n";
			rv = false;
		} else {
			Util::fileCopy( QString( cfgName  + "/" + makeName ),
							QString( file.name() ),
							errMsg );
		}
	}
	// set MEN_QNX_DIR + reparse package DB
	wDebug(("OsQnxConfiguration::load: set QnxDir/reparse package DB" ));
	QString qnxDir =
		loadMakefileKeyVal( errMsg, (QString)MEN_QNX_DIR ) ;
	if( qnxDir != QString::null )
	{
		// reparse from Makefile $(MEN_QNX_DIR) dir
		menQnxDirChanged( qnxDir );
	}
	else
	{
		// try to repase package DB from env
		menQnxDirChanged( envMenQnxDir );
	}

	// load descriptor file
	wDebug(("OsQnxConfiguration::load: load desc" ));
	if( rv == true ){
		rv = loadDesc( sysDescName, errMsg );
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

bool OsQnxConfiguration::loadMakefile( QString &errMsg )
{
	QString filePath = ( cfgName + "/" + makeName );

	// Read in Makefile and convert it into MkFileTuples
	CfgMakefile mkFile( filePath );

	Q3PtrList<MkFileTuple> *lst;

	wDebug(( "loadMakefile %s", filePath.latin1() ));

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

	for( tup=lst->first(); tup; tup=lst->next() )
	{
		wDebug(("loadMakefile: tup=%s", (const char *)tup->key ));

		if ( ( tup->key == "ALL_CORE_LIBS" ) &&
			 ( !tup->val.isEmpty() && (tup->val[0] == "no") ) )
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
						if( tup->key == MEN_QNX_DIR )
						{
							QString qnxDir = tup->val[0];
							qnxDir.replace( '\\', '/' );
							tup->val = QStringList(qnxDir);
						}

						// setting known, change it's value
						// can validate it to update the error messages
						wDebug(("OSQNX: setting->setValue=%s", (const char *)tup->val[0] ));
						setting->setValue( tup->val[0] );
						setting->validate( tup->val[0], errMsg );
	
						if( setting == settingDbgQnx )
							gotSettingDbg = true;
					}
				}
				else
				{
					// unknown variable
					if( tup->key != "QNX_SRC_DIR" &&
						tup->key != "USE_VME_SERVER" &&
						tup->key != "ALL_DESC" )
				    {
						errMsg += QString("Unknown makefile variable %1\n").
							arg(tup->key);
					}
					else
					{
						const char* str = "";
						if ( !tup->val.isEmpty() )
							str = (const char*)tup->val[0];
						wDebug(( "OSQNX: setValue=%s", str ));
					}
				}
			}
		}
	}

	// minimal sanity check...
	if( !gotSettingDbg ){
		errMsg += "This is not an mdiswiz Makefile (ALL_DBGS not found)\n";
		return false;
	}

	return true;
}

QString OsQnxConfiguration::loadMakefileKeyVal( QString &errMsg, QString key )
{
	QString env = QString::null;


	QString filePath = (cfgName + "/" + makeName);

	// Read in Makefile and convert it into MkFileTuples
	CfgMakefile mkFile( filePath );

	Q3PtrList<MkFileTuple> *lst;

	wDebug((
		"loadMakefileKeyVal: %s key=%s",
		filePath.latin1(),
		key.latin1() ));

	try
	{
		lst = mkFile.read();
	}
	catch( CfgMakefile::Except &e )
	{
		errMsg += e.getMsg();
		return false;
	}

	// process the scanned tuples
	MkFileTuple *tup;

	for( tup=lst->first(); tup; tup=lst->next() )
	{
		if( tup->key == key )
		{
			wDebug((
				"loadMakefileKeyVal: key %s is %s",
				key.latin1(),
				tup->val[0].latin1() ));
			env= tup->val[0];
		}
	}

	return( env );
}

//! called whenever \c MEN_QNX_DIR is changed
/*!
  sets value of envMenQnxDir
  reparses the package data base
  \param newVal new value of MEN_QNX_DIR
*/
void OsQnxConfiguration::menQnxDirChanged( const QVariant &newVal )
{
	extern MdisModelFactory *G_mdisMods;

	wDebug(("menQnxDirChanged"));

	if( newVal.toString() != QString::null ){
		if( envMenQnxDir != newVal.toString() ){
			envMenQnxDir = newVal.toString();
			envMenQnxDir.replace( '\\' ,'/' );
			wDebug(("menQnxDirChanged %s", envMenQnxDir.latin1()));
		}

		// parse the model data base (ignore errors)
		PackageDb pkgdb(
			lstSettings.expandedValueOf( targetOs()->pkgDbPath() ));
		pkgdb.parse( G_mdisMods );
	}
}

//
// make a MkFileTuple for all SwModules in lstSwModules matching the
// type modType
//
MkFileTuple *OsQnxConfiguration::mkSwModuleTups( int modType )
{
	SwModule *mod;
	MkFileTuple *tup = new MkFileTuple;
	QStringList lst;

	tup->key = swModuleMfString(modType);
	tup->tag = MkFileTuple::Assignment | MkFileTuple::HardAssign;

	for( mod=lstSwModules.first(); mod; mod=lstSwModules.next())
	{
		if( mod->modType() == modType && mod->isEnabled())
		{
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
	lst.append( "\\" );
	tup->val = lst;
	return tup;
}

void OsQnxConfiguration::mdisProjDirChanged( const QVariant &newVal )
{
	QString projDir = newVal.toString();

	QString expVal = lstSettings.expandedValueOf( newVal.toString());
	wDebug(( "MdisProjDirChanged %s", (const char *)expVal ));

	cfgName = expVal;
}

bool OsQnxConfiguration::newProjectWizard( QWidget *parent, const char *name )
{
	wDebug(("OsQnxConfiguration::newProjectWizard()"));
	OsQnxNewProjectWizard wizard( this, parent, name );
	return wizard.exec();
}

bool OsQnxConfiguration::save( QString &errMsg )
{
	bool rv1, rv2;

	wDebug(( "OsQnxConfiguration::save cfgName = %s", (const char*) cfgName ));

	if( cfgName == QString::null )
		return saveAs( errMsg );

	wDebug(( "OsQnxConfiguration::save %s" , cfgName.latin1() ));

	// create directory if necessary
	QDir cfgDir( cfgName );

	if( !cfgDir.exists() ){
		wDebug(( "OsQnxConfiguration::save: newDir" ));
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
	//rv2 = true;

	// change into dir
	if( rv1 && rv2 )
	{
		wDebug(( "OsQnxConfiguration::save: setCurrent()" ));

	 	if ( QDir::setCurrent( cfgName ) == false )
	 	{
			errMsg = "Cannot change into directory" + cfgName + "\n";
			rv1 = false;
		}
	}

	/* check if pci domains are needed and supported */
	checkPciDomains();

	return rv1 && rv2;
}

bool OsQnxConfiguration::saveAs( QString &errMsg )
{
	QString newCfgName;

	wDebug((
		"OsQnxConfiguration::saveAs cfgName = %s",
		(const char*) cfgName ));

	if(( newCfgName = selectConfigName(
			"Choose Directory to save project to", false, errMsg ))
			!= QString::null ) {

		// user selected a configuration name
		settingMdisProjDir->setValue( newCfgName );
		wDebug((
			"OsQnxConfiguration::saveAs cfgName = %s",
			(const char*) cfgName ));

		return save( errMsg );
	}
	return false;
}

//
// save all settings and sw modules to the Makefile
//
bool
OsQnxConfiguration::saveMakefile( QString &errMsg )
{
	Setting *setting;
	Q3PtrList<MkFileTuple> tupList;
	MkFileTuple *tup, *tupSrc;

	wDebug(("OsQnxConfiguration::saveMakefile"));

	// Build a list of tuples
	tupList.setAutoDelete(true);

	// file header
	tup = new MkFileTuple;
	tup->tag = MkFileTuple::Comment;
	tup->val <<
		"MDIS project makefile" <<
		"Generated by mdiswiz" << targetOs()->mdiswizVersion().latin1();
	tupList.append(tup);

	tup = new MkFileTuple;
	tup->tag = MkFileTuple::Comment;
	tup->val << "Check QNX Develpoment Environment Version";
	tupList.append(tup);

	for( setting = lstSettings.first(); setting; setting=lstSettings.next() )
	{

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
				// assignment
				tup->key = setting->key();
				tup->val = QStringList(setting->value().toString());
				tup->tag = MkFileTuple::Assignment | MkFileTuple::HardAssign;
			}
			else
			{
				tup->val = QStringList(setting->key());
				tup->tag = MkFileTuple::Comment;
			}
			tupList.append(tup);

		}

		// if assigns the MEN_QNX_DIR key, set the src dir
		if( setting->key() == QString("MEN_QNX_DIR") )
		{
			tupSrc = new MkFileTuple;
			tupSrc->tag = MkFileTuple::Ifndef;
			tupSrc->key = "QNX_SRC_DIR";
			tupList.append(tupSrc);

			tupSrc = new MkFileTuple;
			tupSrc->tag = MkFileTuple::Assignment | MkFileTuple::HardAssign;
			tupSrc->key = "	QNX_SRC_DIR";
			tupSrc->val = QStringList("$(MEN_QNX_DIR)/src");
			tupList.append(tupSrc);

			tupSrc = new MkFileTuple;
			tupSrc->tag = MkFileTuple::Endif;
			tupList.append(tupSrc);
		}

	}

	// set the system.dsc file as descriptor
	tup = new MkFileTuple;
	tup->tag = MkFileTuple::Assignment | MkFileTuple::HardAssign;
	tup->key = "ALL_DESC";
	tup->val = QStringList(sysDescPlainName);
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
	tup->key = "$(MEN_QNX_DIR)/src/BUILD/MDIS/TPL/rules.mak";
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

QString OsQnxConfiguration::selectConfigName(
	const QString &action,
	bool mustExist, QString &errMsg )
{
	extern	MdiswizView *G_mdiswiz;
    Q3FileDialog fd( G_mdiswiz, "file dialog", TRUE );
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

	wDebug(("OsQnxworks::selectConfigName dirName=%s", (const char *)dirName ));
	return dirName;
}

bool OsQnxConfiguration::settingsDlg( void )
{
	extern MdiswizView *G_mdiswiz;
	CfgSwModuleTab	*modTab;
	OsQnxTab 		*osTab;

	wDebug(( "OsQnxConfiguration::settingsDlg" ));

	SettingsDlg dlg( G_mdiswiz, this );
	dlg.setCaption(G_progName);
	osTab = new OsQnxTab( this, G_mdiswiz, 0);
	dlg.addTab( osTab, "Project Settings");
	dlg.connect( &dlg, SIGNAL(applyButtonPressed()),
				 osTab, SLOT(slotApply()));
	dlg.connect( &dlg, SIGNAL(cancelButtonPressed()),
				 osTab, SLOT(slotCancel()));

	// remove User libs
	SwModule *sw;
	sw= lstSwModules.find( SwModuleTypes::UserLib, "usr_oss" );
	if ( sw )
		lstSwModules.remove(sw);
	sw= lstSwModules.find( SwModuleTypes::UserLib, "usr_utl" );
	if ( sw )
		lstSwModules.remove(sw);
	// remove Core Libs
	sw= lstSwModules.find( SwModuleTypes::CoreLib, "oss" );
	if ( sw )
		lstSwModules.remove(sw);
	sw= lstSwModules.find( SwModuleTypes::CoreLib, "dbg" );
	if ( sw )
		lstSwModules.remove(sw);

	modTab = new CfgSwModuleTab( G_mdiswiz, 0, this );
	dlg.addTab( modTab, "Software Modules");
	dlg.connect( &dlg, SIGNAL(applyButtonPressed()),
				 modTab, SLOT(slotApply()));
	dlg.setButtons();
	return ( dlg.exec() == QDialog::Accepted );
}

//! convert \a Sw module type code to a string as used in Makefile
QString OsQnxConfiguration::swModuleMfString( int key )
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

bool OsQnxConfiguration::validateSwModule(
	SwModule *mod,
	QString &errMsg,
	bool disableIfInvalid )
{
	wDebug(( "OsQnxConfiguration::validateSwModule" ));
	int rv = true;

	// no check if module is not enabled
	if( !mod->isEnabled() )
		return rv;

	if( G_swModTypes->types.contains( mod->modType() )){

		// check if the swmodule's makefile exists ??? settingMenQnxDir
		const QStringList* makeNameList = mod->makefilePath_all();

		for ( int i=0; i<makeNameList->size(); i++ )
		{
			QString makeName = lstSettings.expandedValueOf(
				envMenQnxDir + "/src/" +
				G_swModTypes->types[mod->modType()].natPath +
				"/" +
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
		wDebug((
			"*** validateSwModule: unknown module type 0x%x",
			mod->modType()));
	}

	return rv;
}

void OsQnxConfiguration::menCpuChanged( )
{
	wDebug(("menCpuChanged"));

	// check if pci Domains are supported
	checkPciDomains();
}

//----------------------------------------------------------------------
//! wizard for new projects
OsQnxNewProjectWizard::OsQnxNewProjectWizard(
	OsQnxConfiguration	*_cfg,
	QWidget 			*parent,
	const char			*name ) :
	Q3Wizard( parent, name, TRUE )
{
	QWidget *wdg;
	Setting *setting;
	QString title;

	resize( 400, 300 ); // make it big. looks better
	cfg = _cfg;

    setCaption(G_progName);

	// make a copy of the current settings
	// these will be restored when user cancels wizard
	savedSettings = cfg->lstSettings.clone();

	int numPages = 0;

	// build one page for each setting
	for( setting = cfg->lstSettings.first();
		 setting;
		 setting= cfg->lstSettings.next() ){

		wdg = setting->wizardPage( this, &title );
		if( wdg ){
			wDebug(( " add page %p (%s)", wdg, (const char *)setting->key() ));
			addPage( wdg, title );
			numPages++;
			setHelpEnabled( wdg, false );
		}
	}

	// Add page to select CPU board:
	cpuWdg = new AddComponentWidget(this, 0,
			 "Please select your target architecture from the list below:");
	insertPage( cpuWdg, "Select an architecture", numPages-1 );
	setHelpEnabled( cpuWdg, false );
}

//! Called when user pressed "finish" or "cancel" button.
void OsQnxNewProjectWizard::done( int r )
{
    extern CpuModelFactory *G_cpuMods;

    wDebug(( "OsQnxNewProjectWizard::done" ));

    if( r == QDialog::Accepted ){
		if( validateSetting() == false )
			return;

		// Add cpu device to configuration
		QString cpuName = cpuWdg->result();
		if( cpuName != QString::null ){
            Device *dev, *newDev;
			CpuDevice *cpuDev;
			dev = G_cpuMods->find( cpuName );
			QString errMsg;

			WIZ_ASSERT( dev );

			// create an instance of template device
			newDev = dev->create();

			// Set CPU Architechture
			cpuDev = (CpuDevice*)newDev;
			cfg->setCpuArch( cpuDev->getArch() );

			// Define if the CPU has a VME bus and set support for it
			HwComponent *child;
			for( child = cpuDev->firstChild();
				 child;
				 child = cpuDev->nextChild() )
			{
				if( child->getInstName().latin1() == QString("VME") ||
					child->getInstName().latin1() == QString("VMEbus") )
					cfg->setVmeSupport( "yes" );
			}

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
	else if( r == QDialog::Rejected ){
		// restore original settings
		cfg->lstSettings.takeValues( savedSettings );
	}

    Q3Wizard::done(r);
}

//! Called when user pressed "next" button.
void OsQnxNewProjectWizard::next( void )
{
	if( validateSetting() == false )
		return;
	Q3Wizard::next();
}

OsQnxNewProjectWizard::~OsQnxNewProjectWizard()
{
	delete savedSettings;
}

void OsQnxNewProjectWizard::showPage( QWidget* page )
{
    extern CpuModelFactory *G_cpuMods;

	wDebug(("OsQnxNewProjectWizard::showPage"));

	if ( page == cpuWdg )	{
        // add CPU devices to list
		cpuWdg->clearItems();

		for( CpuDevice *dev=(CpuDevice *)G_cpuMods->first(); dev;
			 dev=(CpuDevice *)G_cpuMods->next() ){
			// sort out CPU extension boards / ESM carriers
			if( cfg->cpuOkForCfg( dev ) ){
				cpuWdg->addItem( dev->getModelName(), dev->getDescription() );
			}
		}
    }

	wDebug(( "OsQnxNewProjectWizard::showPage: showPage\n" ));
	Q3Wizard::showPage( page );

	wDebug(("OsQnxNewProjectWizard::showPage: finishButton\n"));

	if( finishButton() )
		finishButton()->setEnabled( true );
}

bool OsQnxNewProjectWizard::validateSetting( void )
{
	Setting *setting;

	wDebug(( "OsQnxNewProjectWizard::validateSetting" ));

	// call slotApply() of the currently displayed setting
	if( currentPage()){
		wDebug(("OsQnxNewProjectWizard::validateSetting: currentPage"));

		for( setting = cfg->lstSettings.first();
			 setting;
			 setting = cfg->lstSettings.next() )
		{
		wDebug(("OsQnxNewProjectWizard::validateSetting: currentPage: for"));

			if( setting->currentWidget() == currentPage() )
			{
				wDebug(("OsQnxNewProjectWizard::validateSetting: currentPage: for: break"));
				break;
			}
		}

		// not all pages belong to a setting, so its not an error
		// if setting is 0 here

		if( setting )
		{
			QString errMsg;
			wDebug(("OsQnxNewProjectWizard::validateSetting: %s",
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
			wDebug(("OsQnxNewProjectWizard::validateSetting: no setting"));
		}
	}
	return true;
}

//----------------------------------------------------------------------
//! sets up the toolbar and project menu
void QnxMdiswiz::setupProjectActions( void )
{
	QAction *action;

	MdiswizView::setupProjectActions();
	wDebug(( "QnxMdiswiz::setupProjectActions()" ));

	// add QNX specific entries
	// generate build menu
	Q3PopupMenu *buildMenu = new Q3PopupMenu( this );
	menu->insertItem( "&Build", buildMenu, -1, 1 );
	action = new QAction( QPixmap(build_xpm),
					 	  "&Build", 0, this, "build" );
	action->setObjectName("Build Project");
	connect( action, SIGNAL( activated() ) , this, SLOT( slotBuild() ));
	action->addTo( buildMenu );
	action->addTo( tb );

	action = new QAction( QPixmap(rebuild_xpm),
					 	  "&Rebuild", 0, this, "rebuild" );
	action->setObjectName("Rebuild Project");
	connect( action, SIGNAL( activated() ) , this, SLOT( slotRebuild() ));
	action->addTo( tb );
	action->addTo( buildMenu );
}

void QnxMdiswiz::doBuild( const QString &title, const QStringList &args )
{

	if( !buildDlg ){
		// make sure configuration saved
		slotSaveConfig();
		// make sure to be in correct directory (CONFIG DIR)
		buildDlg = new ExecProgram(
			MAIN_GetCurrentCfg()->msgBoxCaption(),
			title,
			this,
			"buildDlg",
			false );
		buildDlg->setCaption(G_progName);
		buildDlg->show();
		buildDlg->start( args );
		connect( buildDlg, SIGNAL(finished()),
				 this, SLOT(slotBuildFinished()));
	}
}

//! build project
void QnxMdiswiz::slotBuild( void )
{
	QStringList args;

	args << "make";
	doBuild( "Building Project (make)", args );
}

//! Change CPU device
void QnxMdiswiz::slotChangeCpu( void )
{
	// Do the changing of CPU
	MdiswizView::slotChangeCpu();

	// Get new CPU device and config
	Configuration *cfg = MAIN_GetCurrentCfg();
	CpuDevice *cpuDev;

	cpuDev = MAIN_GetCurrentCfg()->getRootDev();
	OsQnxConfiguration *qnxCfg;
	qnxCfg = (OsQnxConfiguration*)cfg;

	// Set the architechture
	qnxCfg->setCpuArch( cpuDev->getArch() );

	// Define if the CPU has a VME bus and set support for it
	HwComponent *child;
	for( child = cpuDev->firstChild();
		 child;
		 child = cpuDev->nextChild() )
	{
		if( child->getInstName().latin1() == QString("VME") ||
			child->getInstName().latin1() == QString("VMEbus") )
		{
			qnxCfg->setVmeSupport( "yes" );
			break;
		}
		else
			qnxCfg->setVmeSupport( "no" );
	}
}

//! rebuild project
void QnxMdiswiz::slotRebuild()
{
	QStringList args;

	args << "sh" << "-c" << "make clean; make"; //??? make clean
	doBuild( "Rebuilding Project (make clean; make)", args );
}

//! called when build process has been exited
void QnxMdiswiz::slotBuildFinished( void )
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
				"The modules can now be installed. See MDIS user guide!", //???
				QMessageBox::Ok,
				QMessageBox::NoButton );
		}
	}
}

//----------------------------------------------------------------------
//! Sets up the tabs settings

OsQnxTab::OsQnxTab(
	OsQnxConfiguration *_cfg,
	QWidget *parent,
	const char *name ) :
	QWidget( parent, name )
{
	Q3VBoxLayout *vb = new Q3VBoxLayout(this,10);
	Setting 	*setting;
	QWidget 	*wdg;

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
	vb->addStretch( 1 );
}

OsQnxTab::~OsQnxTab( void )
{
	delete savedSettings;
}

void OsQnxTab::slotApply( void )
{
	wDebug(( "OsQnxTab::slotApply" ));
	QString errMsg;

	cfg->lstSettings.applyAll( errMsg );
	if( !errMsg.isEmpty() ){
		VMsgBox::criticalOk(
			cfg->msgBoxCaption(),
			"Some settings have invalid values.\n"
			"Please correct them!",
			errMsg,
			this );
	}
}

void OsQnxTab::slotCancel( void )
{
	wDebug(( "OsQnxTab::slotCancel" ));
	// restore original settings
	cfg->lstSettings.takeValues( savedSettings );
}




