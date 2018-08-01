/***************************************************************************
 */
/*!       \file  osos9.cpp
 *       \brief  Implementation of class OsOs9
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2011/09/26 08:42:34 $
 *    $Revision: 2.1 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: osos9.cpp,v $
 * Revision 2.1  2011/09/26 08:42:34  dpfeuffer
 * R: SettingAllCpus called also for mdiswizvx by mistake
 * M: SettingAllCpus renamed to SettingAllCpusOs9
 *
 * Revision 2.0  2010/01/22 11:27:03  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.4  2009/08/17 13:52:08  CRuff
 * R: List of available CPUs not updated correctly when CPU was changed
 * M: a) attach to signal sigCpuChanged(), emitted by MdiswizView
 *    b) added new slot menCpuChanged() to update the list of cpu cores after
 *       signal was received
 *
 * Revision 1.3  2009/04/29 11:44:23  CRuff
 * R: make Kernel modules OSS and DBG configurable
 * M: added handling for new moduleType SwModuleType::CoreLib to methods
 *    OsOs9Configuration::saveMakefile() and
 *    OsOs9Configuration::swModuleMfString()
 *
 * Revision 1.2  2006/12/20 12:02:44  cs
 * fixed:
 *   - take MWOS from environment when not defined in Makefile
 *
 * Revision 1.1  2004/07/22 12:06:44  kp
 * Initial (alpha) Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

/*
  Definitions:
  - MWOS is read from Registry and used as default
  - Selected MWOS is stored in Makefile.
  - If environment MWIS exists, it is used

  Initial Dialog
  - MWOS selection
  - MDIS_PROJ_DIR selection
  - Select Cpu Board
    - Set "CPU" dependent on Cpu
	- If Cpu allows different CPU types, ask for them

  ALL_DBGS will be set to dbg and nodbg

  ToDo:
  - Select CPU:
    - Don't show non 68k/pppc boards
	- No default CPU
	- AllCPUs - only when more than one selection
	-

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
#include <stdlib.h>

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
#include "osos9.h"
#include "cfgmakefile.h"
#include "swmodulestab.h"
#include "osos9.h"
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

// pixmaps
#include "images/buildinstall.xpm"
#include "images/build.xpm"
#include "images/rebuild.xpm"

extern QWidget *G_mainWindow; //!< used as anchor for dialog
extern QApplication *G_qApp;  //!< main application

// strings
static const QString mdiswizOs9Version("os9-0");
static const QString sysDescPlainName("system");
static const QString sysDescName( sysDescPlainName + ".dsc");
static const QString makeName("Makefile");
static const QString packageDbPath("SRC/ETC/MEN_PACKAGE_DESC");
static QString envMenMwos; //!< value of env. var MWOS
static const char MWOS[] = "MWOS";


//! Mapping of Os9 style CPU types and CpuDevice constants
class Os9CpuTypeMap : public TypeStrMap<CpuDevice::CpuCore> {
public:
	Os9CpuTypeMap() {
		mmap[CpuDevice::Ppc4xx]			=
			mmap[CpuDevice::Ppc603]		=
			mmap[CpuDevice::Ppc603e]	=
			mmap[CpuDevice::Ppc604]		=
			mmap[CpuDevice::Ppc740]		=
			mmap[CpuDevice::Ppc750]		=
			mmap[CpuDevice::Ppc74xx]	=
			mmap[CpuDevice::Ppc8xx]		= "pppc";

		mmap[CpuDevice::M68000]			= "k68k";
		mmap[CpuDevice::M68010]			= "k68k";
		mmap[CpuDevice::M68020]			=
			mmap[CpuDevice::M68030]		=
			mmap[CpuDevice::M68060]		= "k020";
		mmap[CpuDevice::MCpu32]			= "kc32";
	};
};


//---------------------------------------------------------------------
// Static functions


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
//! Determine possible CpuTypes of CpuDevice
/*!
  Makes a QStringList that holds all (Os9 style) CPU types
  that are possible on the given \a cpu
*/
static QStringList os9CpuTypes( CpuDevice *cpu )
{
	QStringList os9Types;
	Os9CpuTypeMap typeMap;
	typedef Q3ValueList<CpuDevice::CpuCore> CpuList;
	CpuList::const_iterator it;

	for ( it = cpu->getLstCpuCores().begin();
		  it != cpu->getLstCpuCores().end();
		  ++it ){

		QString os9Cpu = typeMap.keyToStr( *it );
		wDebug(("os9Cpu=%s", os9Cpu.latin1() ));

		// check if mapped string already in list
		if( os9Types.contains( os9Cpu ) == 0 )
			os9Types += os9Cpu;
	}
	return os9Types;
}

//---------------------------------------------------------------------
// Settings



class SettingMenMwos : public DirectorySetting {
public:

	SettingMenMwos() :
		DirectorySetting( MWOS,
						  DirectorySetting::Dir ) {
	}

	virtual Setting *clone(){
		SettingMenMwos *setting = new SettingMenMwos();
		copy( setting );
		return setting;
	}

	virtual bool validate( const QVariant &newVal, QString &errMsg ){
		QString os9Dir = lst->expandedValueOf( newVal.toString() );
		bool rv = true;

		QFileInfo fi1( os9Dir + "/MAKETMPL/MEN/MDIS2004/rules.mak" );

		if( !fi1.exists() ){
			errMsg += "\"" + os9Dir +
				"\" does not look like a OS9 MDIS directory (" +
				fi1.filePath() + " not found)\n";
			rv = false;
		}
		return rv;
	}

protected:
	QString infoText() {
		return "The directory where you have installed the MDIS system "
			"package for Os9";
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



class SettingAllCpusOs9 : public MultipleChoiseSetting {
public:
	SettingAllCpusOs9() : MultipleChoiseSetting( "ALL_CPUS" ) {
	choises << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7"
			<< "2" << "3" << "4" << "5" << "6" << "7"; // ???
	}



	virtual Setting *clone(){
		SettingAllCpusOs9 *setting = new SettingAllCpusOs9();
		copy( setting );
		return setting;
	}

protected:
	QString infoText() {
		return "Defines the CPU";
	}
	QString labelText() {
		return "CPU Type";
	}
	QString wizPageTitle() {
		return "Select CPU Type";
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
class OsOs9InitialDialog : public QDialog {
public:
	//! creates the add component dialog
	OsOs9InitialDialog(QWidget *parent,
						 const char *name=0,
						 bool modal=TRUE,
						 Qt::WFlags f=0) : QDialog(parent,name,modal,f)
	{
		Q3VBoxLayout* vb = new Q3VBoxLayout(this,10);
		QPushButton *but;
		QLabel *lab;

		lab = new QLabel( "<b>Welcome to the MDIS Configuration Wizard "
						  "for OS-9!</b>",
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
//! creates OsOs9 object
/*!
*/
OsOs9::OsOs9() : TargetOs( OsFactory::Os9 )
{
	// check if environment variable MWOS is present
	envMenMwos = getenv( MWOS );
	wDebug(("ENV Os9 MWOS=%s", envMenMwos.latin1() ));
	if( envMenMwos.isEmpty() )
		envMenMwos = "c:\\MWOS";
}

Configuration *
OsOs9::createConfiguration()
{
	return new OsOs9Configuration;
}

QString
OsOs9::pkgDbPath()
{
	return envMenMwos + "/" + packageDbPath;
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
OsOs9::startup( int argc, char **argv )
{
	extern MdiswizView *G_mdiswiz;
	bool success = false;
	QString errMsg;
	QString mdisDir = QDir::currentDirPath();
	Os9CpuTypeMap typeMap;

	// remove unused SW module types
	G_swModTypes->types.remove( SwModuleTypes::Other );
	// patch OS-9 specific paths
	G_swModTypes->types[ SwModuleTypes::LlDriver ].natPath = "SRC/IO/MDIS_LL";
	G_swModTypes->types[ SwModuleTypes::BbDriver ].natPath = "SRC/IO/BBIS_BB";
	G_swModTypes->types[ SwModuleTypes::CbDriver ].natPath = "SRC/IO/CBIS";
	G_swModTypes->types[ SwModuleTypes::LlTool ].natPath = "SRC/IO/MDIS_LL";
	G_swModTypes->types[ SwModuleTypes::UserLib ].natPath = "SRC/LIBSRC";
	G_swModTypes->types[ SwModuleTypes::CoreLib ].natPath = "SRC/LIBSRC";
	G_swModTypes->types[ SwModuleTypes::ComTool ].natPath = "SRC/TOOLS";

	// must create a temporary configuration, it is immediately
	// destroyed by MdiswizView::loadConfig() or newConfig()
	OsOs9Configuration *newCfg = new OsOs9Configuration;;
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

	wDebug(("OsOs9::startup: try load mdisDir=%s", mdisDir.latin1() ));


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
		OsOs9InitialDialog dlg(G_mainWindow);
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
OsOs9::driverName( QString plainName, Device *dev )
{
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
	case Device::Cbis:
		name += "cb_"; break;
	default:
		return QString::null;
	}

	name += plainName;
	return name.lower();
}

QString
OsOs9::mdiswizVersion()
{
	return MdiswizView::Version() + "-" + mdiswizOs9Version;
}

QString
OsOs9::helpEnv()
{
	return " MWOS - path to MWOS root where MDIS is installed, e.g. c:\\MWOS";
}

QString
OsOs9::helpPath()
{
	return envMenMwos + "/MDIS_DOCS/mdis.html"; //???
}

bool
OsOs9::installDlg()
{
	Installer installer( envMenMwos );

	return installer.guiInstall( G_mainWindow );
}

bool
OsOs9::batchInstall( QString pkgFile, QString &errMsg  )
{
	Installer installer( envMenMwos );

	return installer.batchInstall( pkgFile, errMsg );
}
//----------------------------------------------------------------------

//! creates the OsOs9Configuration object
OsOs9Configuration::OsOs9Configuration() :
	Configuration()
{
	// create CPU type map


	// create the settings
	lstSettings.append( settingMenMwos = new SettingMenMwos() );
	lstSettings.append( settingMdisProjDir = new SettingMdisProjDir() );
	lstSettings.append( settingDbgMake = new Setting("V"));
	lstSettings.append( settingAllCpus = new SettingAllCpusOs9());
	lstSettings.connectSettingsToList();

	settingMdisProjDir->setPersist( false );

	// set setting defaults
	settingMenMwos->setValue( envMenMwos );
	settingMdisProjDir->setValue("mymdisproj");

	settingDbgMake->setPersist( true );
	settingDbgMake->setValue( "" );
	QString info = "Makefile debug output disable (1) or "
	               "empty to enable";
	settingDbgMake->setInfoText( &info );


	// set the configuration name when MDIS_PROJ_DIR is changed
	connect( settingMdisProjDir, SIGNAL( valueChanged( const QVariant & )),
			 this, SLOT( mdisProjDirChanged( const QVariant & )));

	// parse packagedb when MWOS changes
	connect( settingMenMwos, SIGNAL( valueChanged( const QVariant & )),
			 this, SLOT( menMwosChanged( const QVariant & )));

	// update properties when CPU is changed
	extern MdiswizView *G_mdiswiz;
    connect( G_mdiswiz, SIGNAL( sigCpuChanged( void )),
             this, SLOT( menCpuChanged( void )));
}

Configuration *
OsOs9Configuration::create()
{
	return new OsOs9Configuration();
}

bool
OsOs9Configuration::save(QString &errMsg)
{
	bool rv1, rv2;

	if( cfgName == QString::null )
		return saveAs( errMsg );

	wDebug(("OsOs9Configuration::save %s", cfgName.latin1() ));

	// create directory if necessary
	QDir cfgDir(cfgName);

	if( !cfgDir.exists() )
	{
		wDebug(("OsOs9Configuration::save: newDir"));

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

	return rv1 && rv2;
}

bool
OsOs9Configuration::saveAs(QString &errMsg)
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
OsOs9Configuration::load( QString &errMsg, const QString &_cfgName )
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
	wDebug(("OsOs9Configuration::load: cfgName=%s", cfgName.latin1()));
	// change into the loaded project
	if( QDir::setCurrent( cfgName ) == false ){
		errMsg += QString("Cannot change into directory ") + cfgName +
			"\n";
		rv = false;
	}

	// set MWOS + reparse package DB
	wDebug(("OsOs9Configuration::load: set Mwos/reparse package DB" ));
	QString os9Dir = loadMakefileKeyVal( errMsg, (QString)MWOS );
	if( os9Dir != QString::null )
	{
		// reparse from Makefile MWOS
		menMwosChanged( os9Dir );
	}
	else
	{
		// try to repase package DB from env
		menMwosChanged( envMenMwos );
	}

	// load descriptor file
	wDebug(("OsOs9Configuration::load: load desc" ));
	if( rv == true ){
		rv = loadDesc( sysDescName, errMsg );
	}

	if( rv == true )
	{
		// after loading DESC we know the possible CPU types
		// and can set the possible CPU core list

		Os9CpuTypeMap typeMap;
		CpuDevice *root;

		root = getRootDev();
		wDebug(("OsOs9Configuration::load: map CPU TYPES"));

		settingAllCpus->setChoises(
			os9CpuTypes( root ),
			QStringList(),
			typeMap.keyToStr( root->getCpuCoreDefault() ) );
	}

    // load Makefile
	if( rv == true ){
		setAllSwModulesDisabled();
		rv = loadMakefile( errMsg );
	}


	return rv;
}

QString
OsOs9Configuration::selectConfigName( const QString &action,
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

	wDebug(("OsOs9::selectConfigName dirName=%s", (const char *)dirName ));
	return dirName;
}

bool
OsOs9Configuration::loadMakefile( QString &errMsg )
{
	QString filePath = (cfgName + "/" + makeName);

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

	for( tup=lst->first(); tup; tup=lst->next() ){
		wDebug(("loadMakefile: tup=%s", (const char *)tup->key ));

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

					// setting known, change it's value
					// can validate it to update the error messages
					setting->setValue( tup->val[0] );
					setting->validate( tup->val[0], errMsg );

					if( setting == settingDbgOs9 )
						gotSettingDbg = true;
				}
				else {
					// unknown variable
					if( tup->key != MWOS
					    && tup->key != "ALL_DESC"
					    && tup->key != "MDBG"
					    && tup->key != "MDIS_EXTRA_DEFS"
					  )
				    {
						errMsg += QString("Unknown makefile variable %1\n").
							arg(tup->key);
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


QString OsOs9Configuration::loadMakefileKeyVal( QString &errMsg, QString key )
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
		return false;
	}

	// process the scanned tuples
	MkFileTuple *tup;

	for( tup=lst->first(); tup; tup=lst->next() )
	{
		wDebug(("loadMakefile: tup=%s", (const char *)tup->key ));

		if( tup->key != MWOS )
		{
			wDebug(("loadMakefile: OS9 Dir found" ));
		}
	}

	return( env );
}



//
// save all settings and sw modules to the Makefile
//
bool
OsOs9Configuration::saveMakefile( QString &errMsg )
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
		"MDIS project makefile" <<
		"Generated by mdiswiz";
	tupList.append(tup);

	// include Windriver template
	tup = new MkFileTuple;
	tup->key = "$(WIND_BASE)/target/h/make/defs.$(WIND_HOST_TYPE)";
	tup->tag = MkFileTuple::Include;
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
				tup->val = QStringList(setting->value().toString());
				tup->tag = MkFileTuple::Assignment;
			}
			else {
				tup->val = QStringList(setting->key());
				tup->tag = MkFileTuple::Comment;
			}
			tupList.append(tup);

		}
	}

	// include corecomponents
	tup = new MkFileTuple;
	tup->key = "$(MWOS)/BUILD/MDIS/TPL/corecomp.mak";
	tup->tag = MkFileTuple::Include;
	tupList.append(tup);

	// now build the sw module lists
	tupList.append( mkSwModuleTups( SwModuleTypes::LlDriver ));
	tupList.append( mkSwModuleTups( SwModuleTypes::BbDriver ));
	tupList.append( mkSwModuleTups( SwModuleTypes::UserLib ));
	tupList.append( mkSwModuleTups( SwModuleTypes::CoreLib ));
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
	tup->key = "$(MWOS)/BUILD/MDIS/TPL/rules.mak";
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
OsOs9Configuration::mkSwModuleTups( int modType )
{
	SwModule *mod;
	MkFileTuple *tup = new MkFileTuple;
	QStringList lst;

	tup->key = swModuleMfString(modType);
	tup->tag = MkFileTuple::Assignment;

	for( mod=lstSwModules.first(); mod; mod=lstSwModules.next()){
		if( mod->modType() == modType && mod->isEnabled()) {
			lst += mod->makefilePath();
		}
	}
	tup->val = lst;
	return tup;
}

//! convert \a Sw module type code to a string as used in Makefile
QString
OsOs9Configuration::swModuleMfString( int key )
{
	switch( key ){
	case SwModuleTypes::LlDriver: 	return "ALL_LL_DRIVERS";
	case SwModuleTypes::BbDriver: 	return "ALL_BB_DRIVERS";
	case SwModuleTypes::UserLib: 	return "ALL_USR_LIBS";
	case SwModuleTypes::CoreLib:	return "ALL_CORE1";
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
OsOs9Configuration::enableSwMods( int modType, MkFileTuple *tup,
									QString &errMsg )
{

	for ( QStringList::Iterator it = tup->val.begin(); it != tup->val.end();
		  ++it ) {

		SwModule *mod = lstSwModules.findMake( modType, *it );
		if( mod )
			mod->setEnabled( true );
		else
			errMsg += QString("No such SW module with makefile %1 "
							  "(ignored)\n").arg(*it);
	}
}

bool
OsOs9Configuration::settingsDlg()
{
	CfgSwModuleTab *modTab;
	OsOs9Tab *osTab;

	wDebug(("OsOs9::settingsDlg"));
	SettingsDlg dlg( G_mainWindow, this );
	osTab = new OsOs9Tab( this, G_mainWindow, 0);
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
OsOs9Configuration::newProjectWizard( QWidget *parent,
										const char *name)
{
	OsOs9NewProjectWizard wizard( this, parent, name );

	return wizard.exec();
}


//! called whenever \c MDIS_PROJ_DIR is changed
/*!
  sets the configurations name to the new value
  \param newVal new value of MDIS_PROJ_DIR
*/
void
OsOs9Configuration::mdisProjDirChanged( const QVariant &newVal )
{
	QString projDir = newVal.toString();

	QString expVal = lstSettings.expandedValueOf( newVal.toString());
	wDebug(( "MdisProjDirChanged %s", (const char *)expVal ));

	cfgName = expVal;
}

//! called whenever \c MWOS is changed
/*!
  sets value of envMenMwos
  reparses the package data base
  \param newVal new value of MWOS
*/
void
OsOs9Configuration::menMwosChanged( const QVariant &newVal )
{
	extern MdisModelFactory *G_mdisMods;

	wDebug(("menMwosChanged"));

	if( newVal.toString() != QString::null )
	{
		if( envMenMwos != newVal.toString() )
		{
			envMenMwos = newVal.toString();
			wDebug(("menMwosChanged %s", envMenMwos.latin1()));
		}

		// parse the model data base (ignore errors)
		PackageDb pkgdb( targetOs()->pkgDbPath() );
		pkgdb.parse( G_mdisMods );
	}
}

void
OsOs9Configuration::menCpuChanged( )
{
	wDebug(("menCpuChanged"));

	// refresh the list of cpu cores
	CpuDevice *root = getRootDev();
	Os9CpuTypeMap typeMap;

	settingAllCpus->setChoises( os9CpuTypes( root ),
		                        QStringList(),
		                        typeMap.keyToStr( root->getCpuCoreDefault() ) );
}

bool
OsOs9Configuration::validateSwModule(
	SwModule *mod,
	QString &errMsg,
	bool disableIfInvalid )
{
	int rv = true;

	// no check if module is not enabled
	if( !mod->isEnabled() )
		return rv;

	if( G_swModTypes->types.contains( mod->modType() )){

		// check if the swmodule's makefile exists ??? settingMenMwos
		QString makeName = envMenMwos + "/" +
			G_swModTypes->types[mod->modType()].natPath + "/" +
			mod->makefilePath();

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
		}
	}
	else {
		wDebug(("*** validateSwModule: unknown module type 0x%x",
			   mod->modType()));
	}

	return rv;
}

bool
OsOs9Configuration::cpuOkForCfg( CpuDevice *cpu )
{
	return ( cpu->getArch() == CpuDevice::Ppc ||
			 cpu->getArch() == CpuDevice::M68k  );
}

//----------------------------------------------------------------------
// New project wizard

//! create the new project wizard for Os9
/*!
  execute with the exec() method
*/
OsOs9NewProjectWizard::OsOs9NewProjectWizard(
	OsOs9Configuration *_cfg, QWidget *parent, const char *name) :
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
OsOs9NewProjectWizard::~OsOs9NewProjectWizard()
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
OsOs9NewProjectWizard::next()
{
	if( validateSetting() == false )
		return;

	if( currentPage() == cpuWdg ){
		//???
		wDebug(("OsOs9NewProjectWizard::next cpuWdg\n"));
	}

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
OsOs9NewProjectWizard::done( int r )
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
OsOs9NewProjectWizard::validateSetting()
{
	Setting *setting;

	wDebug(("OsOs9NewProjectWizard::validateSetting"));

	// call slotApply() of the currently displayed setting
	if( currentPage())
	{
		wDebug(("OsOs9NewProjectWizard::validateSetting: currentPage"));

		for( setting = cfg->lstSettings.first();
			 setting;
			 setting = cfg->lstSettings.next() )
		{
			if( setting->currentWidget() == currentPage() )
			{
				break;
			}
		}

		// not all pages belong to a setting, so its not an error
		// if setting is 0 here

		if( setting )
		{
			QString errMsg;
			wDebug(("OsOs9NewProjectWizard::validateSetting: %s",
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
	}
	return true;
}

//! reimplemented from QWizard
/*!
*/
void
OsOs9NewProjectWizard::showPage( QWidget* page )
{
	extern CpuModelFactory *G_cpuMods;
	Os9CpuTypeMap typeMap;

	// if this is the CPU selection page,
	// make a list of CPU boards matching kernel arch ???
	if( page == cpuWdg )
	{
		wDebug(("OsOs9NewProjectWizard::showPage: about to show cpuWdg\n"));

		cpuWdg->clearItems();

		for( CpuDevice *dev=(CpuDevice *)G_cpuMods->first(); dev;
			 dev=(CpuDevice *)G_cpuMods->next() )
		{

			if( cfg->cpuOkForCfg( dev ) )
				cpuWdg->addItem( dev->getModelName(), dev->getDescription() );
		}
	}
	else if( page == allCpusWdg )
	{
		CpuDevice *dev;
		// build list of suitable CPU types...
		wDebug(("OsOs9NewProjectWizard::showPage: about to show allCpusWdg\n"));
		setAppropriate( allCpusWdg, false ); // ???

		dev = (CpuDevice *)G_cpuMods->find( cpuWdg->result() );

		WIZ_ASSERT(dev);
		wDebug(("OsOs9NewProjectWizard::showPage: select CPU Type\n"));
		cfg->settingAllCpus->setChoises(
			os9CpuTypes( dev ),
			QStringList(),
			typeMap.keyToStr( dev->getCpuCoreDefault() ) );
	}

	Q3Wizard::showPage(page);

	if( finishButton() )
		finishButton()->setEnabled( true );
}

//----------------------------------------------------------------------
//! creates the OsOs9Tab ??? completely identical to OsLinux
OsOs9Tab::OsOs9Tab( OsOs9Configuration *_cfg, QWidget *parent,
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
OsOs9Tab::~OsOs9Tab()
{
	delete savedSettings;
}

//! called when apply or Ok button pressed in dialog
/*!
  calls the Setting::slotApply function of all underlying settings.
*/
void
OsOs9Tab::slotApply()
{
	wDebug(("OsOs9Tab::slotApply"));
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
OsOs9Tab::slotCancel()
{
	wDebug(("OsOs9Tab::slotCancel"));

	// restore original settings
	cfg->lstSettings.takeValues( savedSettings );
}


//----------------------------------------------------------------------
//! sets up the toolbar and project menu
void
Os9Mdiswiz::setupProjectActions()
{
	QAction *a;

	MdiswizView::setupProjectActions();

	// add os9 specific entries

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

void Os9Mdiswiz::doBuild( const QString &title, const QStringList &args )
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

		connect( buildDlg, SIGNAL(finished()),
				 this, SLOT(slotBuildFinished()));
	}
}

//! build project
void Os9Mdiswiz::slotBuild()
{
	QStringList args;

	args << "make";

	doBuild( "Building Project (make)", args );
}

//! rebuild project
void Os9Mdiswiz::slotRebuild()
{
	QStringList args;

	args << "sh" << "-c" << "make clean; make"; //??? make clean

	doBuild( "Rebuilding Project (make clean; make)", args );
}

//! called when build process has been exited
void Os9Mdiswiz::slotBuildFinished()
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
				QMessageBox::NoButton);
		}
	}
}



