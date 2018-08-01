/***************************************************************************
 */
/*!       \file  packagedb.cpp
 *       \brief  Parser for XML model files
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:27:14 $
 *    $Revision: 2.0 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: packagedb.cpp,v $
 * Revision 2.0  2010/01/22 11:27:14  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.6  2008/10/27 12:30:27  channoyer
 * R:1. Program name should be "MDIS Wizard" when displayed to the user
 *   2. The screen position of the show packages dialog box was not relative
 *      to the main window
 * M:1. Call setCaption with G_progName constant
 *   2. Change parent to G_mdiswiz
 *
 * Revision 1.5  2004/07/21 15:01:37  ufranke
 * changed
 *  - currentCfg() by MAIN_GetCurrentCfg()
 *
 * Revision 1.4  2004/07/16 12:41:43  ufranke
 * cosmetics
 *
 * Revision 1.3  2003/06/05 09:33:45  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/06/03 10:16:31  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:36:16  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#include <q3progressdialog.h>
#include <qapplication.h>
#include <qdialog.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qstringlist.h>
#include <qdialog.h>
#include <q3vbox.h>
#include <qpushbutton.h>
#include <q3listview.h>

#include <Q3HBoxLayout>
#include <Q3VBoxLayout>

#include "wizdebug.h"
#include "packagedb.h"
#include "packagefile.h"
#include "configuration.h"
#include "modelfactory.h"
#include "vmsgbox.h"


extern QWidget *G_mainWindow; 	//!< used as anchor for dialog/progress bar
extern QApplication *G_qApp;	//!< main application
extern const QString	G_progName;     //!< main application name

//----------------------------------------------------------------------

//! constructs a PackageDb object
/*!
  This sets up the PackageDb object for parsing the directory specified
  in \a dirName.
*/
PackageDb::PackageDb( const QString &dirName ) :
	dir( dirName, "*.xml *.XML" )
{
}

//! parse all model XML files in the directory
/*!
  Parses all XML files in the directory specified to the PackageDb::PackageDb()
  constructor. All models found are added to \a factory.

  While parsing, this method brings up a QProgressBar. After parsing it shows
  all warnings and errors in a VMsgBox.

  \return false if the directory does not exist or no XML files found,
  otherwise true even if not a single model has been found.
*/
bool PackageDb::parse( ModelFactory *factory )
{
	QStringList files = dir.entryList();	// get all files in directory
	QString file, errMsg;
	Q3ProgressDialog progress( "Parsing " + dir.path(), 0, files.count(),
							  G_mainWindow, "progress", TRUE );

	wDebug(("PackageDb::parse %s", dir.dirName().latin1()  ));

	//progress.setMinimumDuration( 0 );
	int fileCount = 0;
	if( files.isEmpty() )
		return false;


    for ( QStringList::Iterator it = files.begin(); it != files.end(); ++it ) {

		progress.setProgress( fileCount++ );
		G_qApp->processEvents();

		PackageFile pkgFile( dir.filePath( *it ));
		QString lErrMsg;

		if( pkgFile.parse( lErrMsg ) ) {
			pkgFile.toModelFactory( factory, lErrMsg );
			errMsg += lErrMsg;
		}
		else
			errMsg += lErrMsg;

    }
	progress.setProgress( fileCount );
	G_qApp->processEvents();

	if( !errMsg.isEmpty() )
		VMsgBox::warningOk( MAIN_GetCurrentCfg()->msgBoxCaption(),
							"Warnings while parsing " + dir.path(),
							errMsg,
							G_mainWindow );
	return true;
}




//! Show all installed packages
/*!
  Opens a QListView dialog to show all currently installed packages.
  Silently ignores packages failed to parse...
*/
void PackageDb::showPackages()
{
	extern MdiswizView *G_mdiswiz;
	QStringList files = dir.entryList();	// get all files in directory
	QString lErrMsg;
	QDialog *dlg = new QDialog( (QWidget *) G_mdiswiz, "dlg", true );
	Q3ListView *table = new Q3ListView( dlg );

	Q3VBoxLayout* vb = new Q3VBoxLayout(dlg,10);
	QPushButton *but;

	dlg->setCaption(G_progName);
	vb->addWidget( table);

	Q3HBoxLayout *hb = new Q3HBoxLayout();
	vb->addLayout( hb );
	hb->addStretch(1);

	but = new QPushButton("OK", dlg);
	hb->addWidget( but );
	hb->addStretch(1);
	QObject::connect( but, SIGNAL(clicked()), dlg, SLOT(accept()) );

	table->addColumn( "Package Name" );
	table->addColumn( "Revision" );
	table->addColumn( "Date" );
	table->addColumn( "Description" );

    for ( QStringList::Iterator it = files.begin(); it != files.end(); ++it ) {
		PackageFile pkgFile( dir.filePath( *it ));

		if( pkgFile.parse( lErrMsg ) ) {
			QString name = pkgFile.pkgTag("name");
			QString desc = pkgFile.pkgTag("description");
			QString date = pkgFile.pkgTag("date");
			QString rev  = pkgFile.pkgTag("revision");

			new Q3ListViewItem( table, name, rev, date, desc );
		}
	}

	dlg->exec();
	delete dlg;
}

