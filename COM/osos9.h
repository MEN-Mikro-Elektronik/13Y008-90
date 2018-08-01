/***************************************************************************
 */
/*!       \file  osos9.h
 *       \brief  target OS Os9
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/07/12 10:06:42 $
 *    $Revision: 2.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: osos9.h,v $
 * Revision 2.1  2010/07/12 10:06:42  dpfeuffer
 * R: access violation in mdiswiz_lib.lib under Windows with MDIS_CLINST
 * M: QComboBox replaced with Q3ComboBox
 *
 * Revision 2.0  2010/01/22 11:27:01  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.2  2009/08/17 13:50:57  CRuff
 * R: List of available CPUs not updated correctly when CPU was changed
 * M: declare new slot menCpuChanged() (called when signal received from
 *    MdiswizView
 *
 * Revision 1.1  2004/07/22 12:06:50  kp
 * Initial (alpha) Revision
 *
 * Revision 1.3  2004/07/15 10:17:18  ufranke
 * no official version
 *
 * Revision 1.2  2004/01/09 08:20:16  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/06/11 12:21:44  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef OSOS9_H
#define OSOS9_H

#include <q3wizard.h>
#include <qstringlist.h>
#include <q3vbox.h>
#include <q3combobox.h>
#include <qlabel.h>
#include <qlist.h>
#include <qmap.h>

#include <Q3Frame>

#include "setting.h"
#include "targetos.h"
#include "swmodule.h"
#include "configuration.h"
#include "hwcomponent.h"

class MkFileTuple;
class Q3Frame;
class Q3VBox;
class Q3ComboBox;
class QLineEdit;
class OsOs9NewProjectWizard;
class OsOs9Tab;
class QVariant;
class AddComponentWidget;

//! class to implement specific configurations for Os9
/*!
*/
class OsOs9 : public TargetOs {
public:
	OsOs9();

	bool startup( int argc, char **argv );
	QString pkgDbPath();
	QString driverName( QString plainName, Device *dev );
	QString mdiswizVersion();
	QString helpEnv();
	QString helpPath();
	bool installDlg();
	bool batchInstall( QString pkgFile, QString &errMsg  );
	Configuration *createConfiguration();

private:
};

//----------------------------------------------------------------------
//! Configuration class for Os9
/*!
  Extends Configuration
*/
class OsOs9Configuration : public Configuration {
	Q_OBJECT
	friend class OsOs9NewProjectWizard;
	friend class OsOs9Tab;
	friend class OsOs9;
public:
	OsOs9Configuration();
	//! destructs OsOs9Configuration object
	virtual ~OsOs9Configuration() {};
	virtual Configuration *create();

	virtual bool save(QString &errMsg);			// save configuration
	virtual bool saveAs(QString &errMsg);		// save configuration as
	virtual bool load( QString &errMsg, const QString &_cfgName=0 );
	virtual bool settingsDlg();					// bring up project cfg dialog
	virtual bool newProjectWizard( QWidget *parent = 0, 
								   const char *name = 0);
	virtual bool validateSwModule( SwModule *mod, QString &errMsg, 
								   bool disableInvalidMods );
	virtual bool cpuOkForCfg( CpuDevice *cpu );

protected:
	virtual QString selectConfigName( const QString &action, bool mustExist, 
									  QString &errMsg ); 
private:
	bool loadMakefile( QString &errMsg );
	QString loadMakefileKeyVal( QString &errMsg, QString key );
	bool saveMakefile( QString &errMsg );
	void enableSwMods( int modType, MkFileTuple *tup, QString &errMsg );
	MkFileTuple *mkSwModuleTups( int modType );
	QString swModuleMfString( int key );

	MultipleChoiseSetting *settingAllCpus, *settingDbgOs9;
	DirectorySetting *settingMenMwos, *settingMdisProjDir;
	Setting *settingPciSupport; 
	Setting *settingDbgMake; 

private slots:
	void mdisProjDirChanged( const QVariant &newVal );
	void menMwosChanged( const QVariant &newVal );
	void menCpuChanged();
};
	

//! wizard for new projects

class OsOs9NewProjectWizard : public Q3Wizard {
	Q_OBJECT

public:
	OsOs9NewProjectWizard( OsOs9Configuration *_cfg, 
							   QWidget *parent = 0, 
							   const char *name = 0 );
	~OsOs9NewProjectWizard();
	void showPage(QWidget* page);
public slots:
    virtual void done(int r);
    virtual void next();
private:
	bool validateSetting();
	OsOs9Configuration *cfg;
	SettingList *savedSettings;
	AddComponentWidget *cpuWdg;
	QWidget *allCpusWdg;
};

// -----------------------------------------------------------------
//! Os9 specific configuration settings dialog tab
/*!
  It is added to the SettingsDlg dialog.
*/
class OsOs9Tab : public QWidget {
	Q_OBJECT

public:
	OsOs9Tab( OsOs9Configuration *_cfg, 
				  QWidget *parent, const char *name);
	~OsOs9Tab();
public slots:
    void slotApply(); 
    void slotCancel(); 

protected slots:

private:
    OsOs9Configuration *cfg;
	SettingList *savedSettings;
};


#endif




