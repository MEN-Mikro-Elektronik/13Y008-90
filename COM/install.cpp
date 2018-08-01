/***************************************************************************
 */
/*!       \file  install.cpp 
 *       \brief  Package installer
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:26:18 $
 *    $Revision: 2.0 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: install.cpp,v $
 * Revision 2.0  2010/01/22 11:26:18  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.8  2008/10/27 12:29:23  channoyer
 * R: Program name should be "MDIS Wizard" when displayed to the user
 * M: Call setCaption with G_progName constant
 *
 * Revision 1.7  2004/06/08 17:25:23  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.6  2004/01/09 12:28:50  dpfeuffer
 * VC compiler errors fixed
 *
 * Revision 1.5  2004/01/09 08:19:57  kp
 * added batchInstall
 *
 * Revision 1.4  2003/06/06 13:32:13  kp
 * utime rather _utime
 *
 * Revision 1.3  2003/06/05 12:00:59  dschmidt
 * intermediate checkin
 *
 * Revision 1.2  2003/06/05 09:33:24  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/06/03 10:16:10  kp
 * intermediate checkin
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#include <qapplication.h>
#include <q3filedialog.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qfileinfo.h>
#include <qdir.h>
#include <q3process.h>
#include <q3progressdialog.h>
#include <qmessagebox.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qstringlist.h>
#include <q3textedit.h>
#include <qpushbutton.h>
#include <q3listview.h>

#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <QDateTime>

#include "wizdebug.h"
#include "install.h"
#include "packagefile.h"
#include "swmodule.h"
#include "vmsgbox.h"
#include "util.h"

#ifndef WIN32
# include <sys/types.h>
# include <utime.h>
#else
# include <sys/utime.h>
# define utime _utime
# define utimbuf _utimbuf
#endif


extern QApplication *G_qApp;  //!< main application	
static const QString packageDescDir("PACKAGE_DESC");
extern const QString	G_progName;     //!< main application name

//----------------------------------------------------------------------

//! constructs the PackageInstaller object
/*!
  This sets up the PackageInstaller object.
*/
Installer::Installer( const QString &installRoot )
{	
	_installRoot = installRoot;
	_pkgFile = QString::null;
	_tmpDir  = "INSTALLTMP";	
	proc = 0;
}

//! destructs the PackageInstaller object
/*!
  Cleans up the temporary dir.
 */
Installer::~Installer()
{
	cleanDir( _tmpDir );
}

//! perform GUI based install of a package 
/*!
  - filedialog to select ZIP file
  - extract ZIP to temp. dir
  - analyze contents of temp. dir
  - dialog "Ok to install?"
    - show contained packages (all contained packagedesc XML files)
	- warn when files will be replaced with newer version or newer version
	  already exists
  - copy files to MDIS dir (rename older files)
  \return true if successful install, false on error or cancel
 */
bool
Installer::guiInstall( QWidget *parent )
{
	QString errMsg;
	QString caption = "Install";
	QDialog *dlg = new QDialog( parent, "dlg", true );
	bool ok;

	// ask for package file
	QString pkgFile = packageDlg( parent );
	if( pkgFile == QString::null )
		return false;

	// extract package
	Q3VBoxLayout *vb = new Q3VBoxLayout( dlg ,10);
	QLabel *lab = new QLabel( 
		QString("Extracting ") + pkgFile + " to temporary directory...", dlg);
	vb->addWidget( lab );

	dlg->setCaption(G_progName);
	dlg->show();
	ok = pkgExtract( pkgFile, errMsg );
	dlg->hide();

	if( ok == false ){
		VMsgBox::criticalOk( 
			caption,
			"Package extraction failed",
			errMsg,
			parent );
		goto ABORT;
	}

	// analyze package
	lab->setText("Analyzing package...");
	dlg->show();
	ok = pkgAnalyze( errMsg );
	dlg->hide();
	
	if( ok == false ){
		VMsgBox::criticalOk( 
			caption,
			"Package analysation failed",
			errMsg,
			parent );
		goto ABORT;
	}

	// get install confirmation from user
	if( (ok = installOkDlg( parent )) == false )
		goto ABORT;

	// now do the real installation
	{
		int actionCount=0;
		Q3ProgressDialog progress( "Installing " + _pkgFile, 0, 
								  _actionList.count(), 
								  parent, "progress", TRUE );

		for( InstallerAction *act = _actionList.first();
			 act && ok==true;
			 act = _actionList.next()){

			// process one action from actionlist
			ok = doInstallAction( act, errMsg );		

			progress.setProgress( actionCount++ );
			G_qApp->processEvents();
		}
	}

	// tell user about status
	if( ok == false ){
		VMsgBox::criticalOk( 
			caption,
			"Package installation failed",
			errMsg,
			parent );
		goto ABORT;
	}
	else {
		QMessageBox::information( 
			parent, 
			caption, 
			QString("Package installation completed successfully"));
	}
 ABORT:
	delete dlg;
	return ok;
}



//! show dialog to confirm installation
/*!
  - Shows which packages will be installed
  - Show files that will be replaced
  - Install/Cancel button
 */
bool
Installer::installOkDlg( QWidget *parent )
{
	QDir dir( Util::joinPath( _tmpDir, packageDescDir ), "*.xml *.XML" );
	QDialog *dlg = new QDialog( parent, "dlg", true );
	QStringList files = dir.entryList();	// get all files in directory
	Q3ListView *table = new Q3ListView( dlg );
	Q3TextEdit *textEdit;

	Q3VBoxLayout* vb = new Q3VBoxLayout(dlg,10);
	QPushButton *but;
	QLabel *lab;

	dlg->setCaption(G_progName);
	lab = new QLabel( QString("About to install ") + _pkgFile + ", which "
					  "contains the following package" + 
					  (files.count() > 1 ? "s" : "") + ":", dlg);
	vb->addWidget( lab );
	vb->addWidget( table);

	Q3HBoxLayout *hb = new Q3HBoxLayout();
	hb->addStretch(1);

	but = new QPushButton("Install", dlg);
	hb->addWidget( but );
	QObject::connect( but, SIGNAL(clicked()), dlg, SLOT(accept()) );

	but = new QPushButton("Cancel", dlg);
	hb->addWidget( but );
	hb->addStretch(1);
	QObject::connect( but, SIGNAL(clicked()), dlg, SLOT(reject()) );
	
	// build table to show packages contained in ZIP file
	table->addColumn( "Package Name" );
	table->addColumn( "Revision" );
	table->addColumn( "Date" );
	table->addColumn( "Description" );
	table->setMaximumHeight( 80 );

    for ( QStringList::Iterator it = files.begin(); it != files.end(); ++it ) {
		PackageFile pkgFile( dir.filePath( *it ));
		QString lErrMsg;

		if( pkgFile.parse( lErrMsg ) ) {
			QString name = pkgFile.pkgTag("name");
			QString desc = pkgFile.pkgTag("description");
			QString date = pkgFile.pkgTag("date");
			QString rev  = pkgFile.pkgTag("revision");
			
			new Q3ListViewItem( table, name, rev, date, desc );
		}
	}

	InstallerAction::Action action = InstallerAction::ReplaceDest;

	for( int i=0; i<2; i++ ){

		if( _actionList.countActions( action ) > 0 ){
			// Tell user which files will be replaced/renamed:
			if( action == InstallerAction::ReplaceDest )
				lab = new QLabel( 
					QString("The following files will be overwritten in ")
					+ _installRoot + "with more recent files from the package."
					" The original files will "
					"be renamed (.mdisbak)", dlg );
			else
				lab = new QLabel( 
					QString("The following files are more recent in ") 
					+ _installRoot + " than in the package.\n"
					"The files from the package will be named ("
					".mdisbak)", dlg );
			vb->addWidget( lab );
		
			textEdit = new Q3TextEdit( QString::null, QString::null, dlg );
			textEdit->setReadOnly( true );
			textEdit->setMaximumHeight( 80 );
			vb->addWidget( textEdit );

			for( InstallerAction *act = _actionList.first();
				 act;
				 act = _actionList.next()){

				if( act->action == action )
					textEdit->append( act->dstFile );
			}

		}
		action = InstallerAction::DestNewer;
	}
	vb->addLayout( hb );
	
	return dlg->exec();	
}

//! Perform batch installation 
/*!
  Unconditional installation.
  - extract ZIP to temp. dir
  - analyze contents of temp. dir
  - copy files to MDIS dir (rename older files)
  \return true if successful install, false on error, \a errMsg contains error
 */
bool
Installer::batchInstall( QString pkgFile, QString &errMsg )
{
	bool ok;
	InstallerAction *act;

	// extract package
	ok = pkgExtract( pkgFile, errMsg );
	if( !ok ) goto ABORT;

	// analyze package
	ok = pkgAnalyze( errMsg );
	if( !ok ) goto ABORT;
	
	// perform installation
	for( act = _actionList.first();
		 act && ok==true;
		 act = _actionList.next()){

		// process one action from actionlist
		ok = doInstallAction( act, errMsg );		
	}
 ABORT:
	return ok;
}

//! show file dialog to select ZIP file
/*!
  \return path to ZIP or 0 if user canceled
*/
QString
Installer::packageDlg( QWidget *parent )
{
	// ask for ZIP file
    Q3FileDialog fd( parent, "file dialog", TRUE );
	QString fileName = QString::null;

	fd.setCaption( "Select Package to Install" );

	fd.setMode( Q3FileDialog::ExistingFile );
	fd.setFilter( "Driver package files (*.zip *.ZIP)" );
		
	if ( fd.exec() == QDialog::Accepted )
		fileName = fd.selectedFile();

	return fileName;
}


//! extracts the package into a temp. dir 
/*!
  \a pkgFile is the path to a driver ZIP file.

  - Checks if ZIP file exists
  - Cleans and creates temp dir.
  - Runs external tool \b unzip on the package file into temp. dir

  \return true if everything ok or false if failed, errMsg contains errors
 */
bool
Installer::pkgExtract( const QString &pkgFile, QString &errMsg )
{
	QFileInfo fi( pkgFile );
	bool ok = true;

	// check if ZIP file exists
	if( !fi.isFile() || !fi.isReadable() ){
		errMsg = pkgFile + " doesn't exist!\n";
		return false;
	}

	_pkgFile = pkgFile;

	// clean temp dir
	if( cleanDir( _tmpDir ) == false ){
		errMsg += "Can't clean temp. dir " + _tmpDir + "\n";
		return false;
	}

	// create temp dir
	QDir dir(".");
	if( dir.mkdir( _tmpDir ) == false ){
		errMsg += "Can't create temp. dir " + _tmpDir + "\n";
		return false;
	}
	
	// run unzip in temp. dir
	QStringList args;
	QString cwd = QDir::currentDirPath();
	QDir::setCurrent( _tmpDir );

	procOutput = QString::null;

	args << "unzip" << "-ao" << pkgFile;
	//args << "sleep" << "5";
	proc = new Q3Process();
	proc->setArguments( args );

	connect( proc, SIGNAL(readyReadStdout()),
			 this, SLOT(slotReadStdout()));
	connect( proc, SIGNAL(readyReadStderr()),
			 this, SLOT(slotReadStderr()));

	if( ! proc->start() ){
		errMsg += "Can't run unzip tool\n";
		ok = false;
	}

	if( ok == true ){
		while( proc->isRunning() ){
			if( G_qApp )
				G_qApp->processEvents();
		}

		if( !proc->normalExit() )
			ok = false;
		if( proc->exitStatus() != 0 )
			ok = false;
	}
	QDir::setCurrent( cwd ); // restore current dir
	
	if( !ok )
		errMsg += procOutput;

	delete proc;
	proc = 0;
	return ok;
}

//! read the processes stdout
void 
Installer::slotReadStdout()
{
	procOutput += proc->readStdout();
}
	
//! read the processes stderr
void 
Installer::slotReadStderr()
{
	procOutput += proc->readStderr();
}

//! analyzes the extracted package
/*!
  pgkExtract() must have been run before!

  checks if 
  - at least one PACKAGE_DESC/xxx.xml exists
  - builds the Installer::_actionList (see what install would do)

  \return true if everything ok or false if failed

  \a errMsg may contain errors or warnings, independent of return value
 */
bool
Installer::pkgAnalyze( QString &errMsg )
{
	bool ok;

	// check if packagedesc file exists
	QDir dir( Util::joinPath( _tmpDir, packageDescDir ), "*.xml *.XML" );
	if( dir.count() == 0 ){
		errMsg += QString("Package ") + _pkgFile + " does not contain any "
			" package description XML files and cannot be installed.\n" 
			"This might be a pre MDIS4/2004 package.";
		return false;
	}

	// analyze temp. dir against installRoot recursively
	_actionList.clear();
	ok = analyzeDir( _tmpDir, _installRoot, QString::null, errMsg );

	return ok;
}

//! analyze directories
/*!
  Recursively analyzes directory in \a srcRoot against
  corresponding directory in MDIS tree (\a dstRoot) and builds the _actionList.
 
  This also performs any path mapping required from the common 
  directory structure in \a srcDir to the native (perhaps different)
  structure in \a dstDir.

  When it's called in the root dir (\a relPath is 0), it checks for
  \c history.txt and \c tree.txt files and will setup actions to 
  put them in the _installDir as \c history_xxx.txt and \c tree_xxx.txt.

  \param srcRoot	source dir root (temp. dir)
  \param dstRoot	destination dir root (=_installRoot)
  \param relPath	current path relative to \a srcRoot (common path)
  \param errMsg		receives all error message strings
  \return true if success, false if failed. Sets up Installer::_actionList
  
*/
bool
Installer::analyzeDir( 
	const QString &srcRoot, 
	const QString &dstRoot, 
	const QString &relPath,
	QString &errMsg	)
{
	QDir sDir( Util::joinPath( srcRoot, relPath ) );
	bool ok = true;

	wDebug(("analyzeDir rel=%s", sDir.path().latin1() ));

	// source dir must be existant
	if( !sDir.exists() ){
		errMsg += QString("Directory ") + sDir.path() + " doesn't exist!\n";
		return false;
	}

	// loop through source directory, see which files exist in destination
	// dir.
	
	QFileInfoList files = sDir.entryInfoList();

	// map common path to native path
	QString dstPath = G_swModTypes->mapComToNatPath( relPath );
	QDir dstDir( Util::joinPath( dstRoot, dstPath ));

	// create action to create directory
	if( !(dstDir.exists()) ){
		InstallerAction *act = new InstallerAction;
			
		act->dstFile = dstDir.path();
		act->action  = InstallerAction::MkDir;
		_actionList.add( act );
		wDebug(("action mkdir %s", act->dstFile.latin1()));
	}


	if ( !files.isEmpty() ) {
		const QFileInfo *fi;

		for( int j=0; j<files.size() && ok==true; j++ ){
			fi = &files.at(j);
		
			if ( fi->fileName() == "." || fi->fileName() == ".." )
				;

			else if( fi->isDir() ){
				// recurse into subdir
				ok = analyzeDir( srcRoot, dstRoot, 
								 Util::joinPath( relPath, fi->fileName() ),
								 errMsg);
			}
			else {
				// regular file
				QString dstFileName = fi->fileName();

				// build action (default to NormalCopy)
				InstallerAction *act = new InstallerAction;
			
				// check for special files in root dir
				if( relPath == QString::null && 
					(fi->fileName() == "history.txt" ||
					 fi->fileName() == "tree.txt") ||
					 fi->fileName() == "Copying" ){	  

					// copy them as file_pkg.txt
					QFileInfo fi2( dstFileName );
					QFileInfo fi3( _pkgFile );
					dstFileName = fi2.baseName() + "_" + fi3.baseName() +
						( (fi2.extension()).isEmpty() ? QString("") : 
						( QString(".") + fi2.extension() ));
				}

				act->srcFile = fi->filePath();
				act->dstFile = Util::joinPath( dstDir.path(), dstFileName );
				act->action	 = InstallerAction::NormalCopy;

				// if destination directory is existing, check
				// if a file with same name already exists

				wDebug(("check file %s %s", act->srcFile.latin1(), 
					   act->dstFile.latin1()));

				if( dstDir.exists() ){
					QFileInfo dstFi( dstDir, dstFileName );

					// check dates of files
					if( dstFi.exists() ){
						if( dstFi.lastModified() > fi->lastModified() ){
							act->action	 = InstallerAction::DestNewer;
							wDebug((" dst newer"));
						}
						else if( dstFi.lastModified() < fi->lastModified() ){
							act->action	 = InstallerAction::ReplaceDest;
							wDebug((" src newer"));
						}					
							
					}
				}
			
				_actionList.add( act );
			}
		}
	}
	return ok;
}

//! Remove all files/dirs from directory
/*!
  Recursively removes all files/dirs from directory \a dirPath.
  Finally removes directory \a dirPath
 
  \return true if success (or dir not existing), false if failed
*/
bool
Installer::cleanDir( const QString &dirPath )
{
	bool ok = true;

	{
		QDir dir( dirPath );

		wDebug(("cleanDir pwd=%s %s", QDir::currentDirPath().latin1(), 
			   dirPath.latin1() ));

		if( !dir.exists() )
			return ok;

		QFileInfoList files = dir.entryInfoList();

		if ( !files.isEmpty() ) {

			const QFileInfo *fi;

			for( int j=0; j<files.size(); j++ ){
				fi = &files.at(j);
            
				if ( fi->fileName() == "." || fi->fileName() == ".." )
					ok = true; 

				else if( fi->isDir() ){
					ok = cleanDir( Util::joinPath( dirPath, fi->fileName() ));
				}
				else {
					// file
					ok = QFile::remove( Util::joinPath( dirPath, 
														fi->fileName() ));
				}
				if( !ok )
					break;
			}
		}
	}
	// remove directory itsself
	QFileInfo fi( dirPath );
	QDir dir( fi.dirPath() );
	if( ok )
		ok = dir.rmdir( fi.fileName() );
	return ok;
}

//! perform the specified install action
/*!
  \return true if ok, false if not, errMsg may contain errors
*/
bool
Installer::doInstallAction( InstallerAction *act, QString &errMsg )
{
	bool ok = false;

	switch( act->action ){
	case InstallerAction::NormalCopy:
		ok = doCopy( act->srcFile, act->dstFile, errMsg );
		break;

	case InstallerAction::ReplaceDest:
		ok = doRename( act->dstFile, act->dstFile + ".mdisbak", errMsg );
		if( ok )
			ok = doCopy( act->srcFile, act->dstFile, errMsg );
		break;

	case InstallerAction::DestNewer:
		ok = doCopy( act->srcFile, act->dstFile + ".mdisbak", errMsg );
		break;

	case InstallerAction::MkDir:
		ok = doMkdir( act->dstFile, errMsg );
		break;
	}
	return ok;	
}

//! copy srcFileName to dstFileName
/*
  \a srcFileName and \a dstFileName must be paths (relative or absolute)
  Maintain file dates
*/
bool
Installer::doCopy( 
	const QString &srcFileName, 
	const QString &dstFileName, 
	QString &errMsg)
{
	QFile srcFile( srcFileName );
	QFile dstFile( dstFileName );
	QByteArray copyBuf( 4096 );
	Q_LONG nRead, nWritten = 0;
	bool ok = true;

	wDebug(("doCopy %s %s", srcFileName.latin1(), dstFileName.latin1() ));

	if( srcFile.open( QIODevice::Unbuffered | QIODevice::ReadOnly ) == false ){
		errMsg += QString("Can't open ") + srcFileName + " for reading\n";
		return false;
	}
	if( dstFile.open( QIODevice::Unbuffered | QIODevice::WriteOnly ) == false ){
		errMsg += QString("Can't open ") + dstFileName + " for writing\n";
		return false;
	}
	
	// copy files
	while( (nRead = srcFile.readBlock( copyBuf.data(), copyBuf.size() )) > 0 ){
		nWritten = dstFile.writeBlock( copyBuf, nRead );
		if( nWritten < 0 )
			break;
	}

	if( nRead < 0 ){
		errMsg += QString("Error reading ") + srcFileName + "\n";
		ok = false;
	}
	if( nWritten < 0 ){
		errMsg += QString("Error writing ") + dstFileName + "\n";
		ok = false;
	}
	srcFile.close();
	dstFile.close();


	// reset file dates
	{
		// get source time
		QFileInfo fi( srcFileName );
		QDateTime srcDate = fi.lastModified();

		wDebug((" srcDate: %s %d", 
			   srcDate.toString("yyyy-MM-dd hh:mm:ss").latin1(),
			   srcDate.toTime_t()));

		// set dest time
		{ 
			struct utimbuf ub;
			ub.actime = ub.modtime = srcDate.toTime_t();
			char dstF[200];
			strncpy( dstF, dstFileName, sizeof(dstF));

			if( utime( dstF, &ub ) < 0 ){
				errMsg += "Couldn't set date/time on %s\n", dstFileName + "\n";
				ok = false;
			}
		}
	}
	return ok;
}

//! rename srcFileName to dstFileName
/*
  \a srcFileName and \a dstFileName must be paths (relative or absolute)
*/
bool
Installer::doRename( 
	const QString &srcFileName, 
	const QString &dstFileName, 
	QString &errMsg)
{
	QDir dir;

	wDebug(("doRename %s %s", srcFileName.latin1(), dstFileName.latin1() ));

	if( dir.rename( srcFileName, dstFileName ) != true ){
		errMsg += QString("Error renaming ") + srcFileName;
		return false;
	}
	return true;	
}

//! make directory
/*
  \a newDir must be a path (relative or absolute)
*/
bool
Installer::doMkdir( 
	const QString &newDir, 
	QString &errMsg)
{
	QDir dir;

	wDebug(("doMkdir %s", newDir.latin1() ));
	if( dir.mkdir( newDir, true ) != true ){
		errMsg += QString("Error making directory ") + newDir;
		return false;
	}
	return true;	
}

//----------------------------------------------------------------------

//! create actionlist object
InstallerActionList::InstallerActionList()
{
	setAutoDelete(true);
}

//! add action \a act to action list
void
InstallerActionList::add( InstallerAction *act )
{
	append( act );
}

//! count number of actions in action list matching \a action
int 
InstallerActionList::countActions( InstallerAction::Action action )
{
	int count = 0;
	for( InstallerAction *act = first(); act; act = next()){

		if( act->action == action )
			count++;
	}
	return count;
}

