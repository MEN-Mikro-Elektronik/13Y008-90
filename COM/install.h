/***************************************************************************
 */
/*!       \file  install.h
 *       \brief  Package installer header
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:26:20 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: install.h,v $
 * Revision 2.0  2010/01/22 11:26:20  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.3  2004/01/09 08:19:59  kp
 * cosmetics
 *
 * Revision 1.2  2003/06/05 09:33:26  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/06/03 10:16:11  kp
 * intermediate checkin
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef INSTALL_H
#define INSTALL_H

#include <qstring.h>
#include <q3process.h>
#include <qlist.h>

#include <Q3PtrList>

// -----------------------------------------------------------------
//! Installer action class
/*!
  Each InstallerAction object describes the action to take for
  each file. A list of actions is setup by Installer::pkgAnalyze
*/
class InstallerAction {
 public:
	enum Action {
		NormalCopy, 			//!< copy src->dst
		ReplaceDest, 			//!< rename dest, to dst.bak, copy src->dst
		DestNewer,				//!< copy src->dst.bak
		MkDir					//!< create directory \em dstFile
	};	
	QString srcFile;			//!< path to source file
	QString dstFile;			//!< path to destination file
	Action action;				//!< action to perform 
};

// -----------------------------------------------------------------
//! class that holds a list of InstallerAction objects
/*!
*/
class InstallerActionList : public Q3PtrList<InstallerAction> {
public:
	InstallerActionList();
	void add( InstallerAction *act );
	int countActions( InstallerAction::Action action );
};


// -----------------------------------------------------------------
//! Package installer class
/*!
  This class provides all methods to install a driver package
*/
class Installer : public QObject
{
	Q_OBJECT
public:
	Installer( const QString &installRoot );
	virtual ~Installer();
	bool guiInstall( QWidget *parent );
	bool batchInstall( QString pkgFile, QString &errMsg );
protected:
	virtual bool pkgExtract( const QString &pkgFile, QString &errMsg );
	virtual bool pkgAnalyze( QString &errMsg );
	virtual bool cleanDir( const QString &dirPath );
	virtual QString packageDlg( QWidget *parent );
	virtual bool analyzeDir( 
		const QString &srcRoot, 
		const QString &dstRoot, 
		const QString &relPath,
		QString &errMsg	);
	virtual bool installOkDlg( QWidget *parent );
	virtual bool doInstallAction( InstallerAction *act, QString &errMsg );
	virtual bool doCopy( const QString &srcFileName, 
						 const QString &dstFileName, 
						 QString &errMsg);
	virtual bool doRename( const QString &srcFileName, 
						   const QString &dstFileName, 
						   QString &errMsg);
	virtual bool doMkdir( const QString &newDir, QString &errMsg);

	QString _pkgFile;					//!< ZIP file path
	QString _installRoot;				//!< MDIS tree absolute path
	QString _tmpDir;					//!< temp dir for extraction
	InstallerActionList _actionList;	//!< list of file copy actions

	Q3Process *proc;						//!< external program
	QString procOutput;					//!< external program output

	

private slots:	
    void slotReadStdout();
    void slotReadStderr();
};


#endif

