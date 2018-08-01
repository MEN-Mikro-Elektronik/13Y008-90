/***************************************************************************
 */
/*!       \file  osvxworks.h
 *       \brief  target OS VxWorks
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2011/03/31 17:27:51 $
 *    $Revision: 2.2 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: osvxworks.h,v $
 * Revision 2.2  2011/03/31 17:27:51  CRuff
 * R: 1. warn user if installed MDIS system package and selected cpu do not match
 * M: 1a) added method checkPciDomains() to show warning dialog
 *    1b) added method mdisSupportsPciDomains() to check if PCI Domains are
 *        supported by installed MDIS system package
 *
 * Revision 2.1  2010/07/12 10:06:48  dpfeuffer
 * R: access violation in mdiswiz_lib.lib under Windows with MDIS_CLINST
 * M: QComboBox replaced with Q3ComboBox
 *
 * Revision 2.0  2010/01/22 11:27:06  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.7  2009/08/17 13:34:56  CRuff
 * R: List of available CPUs not updated correctly when CPU was changed
 * M: declare new slot menCpuChanged() (called when signal received from
 *    MdiswizView
 *
 * Revision 1.6  2009/04/27 17:52:51  CRuff
 * R: New project settings option UP / SMP for VxWorks
 * M: in OsVxworksConfiguration: added member
 *    MultipleChoiseSetting *settingSuppProcArch
 *
 * Revision 1.5  2006/11/30 11:27:51  cs
 * added cpuOkForCfg() member
 *
 * Revision 1.4  2004/07/30 11:46:38  ufranke
 * added
 *  - compiler flags DEBUG_FLAGS in Makefile
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
#ifndef OSVXWORKS_H
#define OSVXWORKS_H

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
class OsVxworksNewProjectWizard;
class OsVxworksTab;
class QVariant;
class AddComponentWidget;

//! class to implement specific configurations for Vxworks
/*!
*/
class OsVxworks : public TargetOs {
public:
	OsVxworks();

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
//! Configuration class for Vxworks
/*!
  Extends Configuration
*/
class OsVxworksConfiguration : public Configuration {
	Q_OBJECT
	friend class OsVxworksNewProjectWizard;
	friend class OsVxworksTab;
	friend class OsVxworks;
public:
	OsVxworksConfiguration();
	//! destructs OsVxworksConfiguration object
	virtual ~OsVxworksConfiguration() {};
	virtual Configuration *create();

	virtual bool save(QString &errMsg);			// save configuration
	virtual bool saveAs(QString &errMsg);		// save configuration as
	virtual bool load( QString &errMsg, const QString &_cfgName=0 );
	virtual bool settingsDlg();					// bring up project cfg dialog
	virtual bool newProjectWizard( QWidget *parent = 0,
								   const char *name = 0);
	virtual bool validateSwModule( SwModule *mod, QString &errMsg,
								   bool disableInvalidMods );
	virtual bool cpuOkForCfg( CpuDevice *cpu )
	{
		if( cpu->getArch() == CpuDevice::UnsetArch ){
			return( false );
		}
		return( true );
	}

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

	MultipleChoiseSetting *settingToolChain, *settingAllCpus, *settingDbgVx;
	MultipleChoiseSetting *settingSuppProcArch;
	DirectorySetting *settingMenVxDir, *settingMdisProjDir;
	Setting *settingAddedCflags;
	Setting *settingPciSupport;
	Setting *settingDbgFlagsCompiler;
	Setting *settingDbgMake;

	void checkPciDomains( );
	bool mdisSupportsPciDomains( );

private slots:
	void mdisProjDirChanged( const QVariant &newVal );
	void menVxDirChanged( const QVariant &newVal );
	void menCpuChanged( );
};


//! wizard for new projects

class OsVxworksNewProjectWizard : public Q3Wizard {
	Q_OBJECT

public:
	OsVxworksNewProjectWizard( OsVxworksConfiguration *_cfg,
							   QWidget *parent = 0,
							   const char *name = 0 );
	~OsVxworksNewProjectWizard();
	void showPage(QWidget* page);
public slots:
    virtual void done(int r);
    virtual void next();
private:
	bool validateSetting();
	OsVxworksConfiguration *cfg;
	SettingList *savedSettings;
	AddComponentWidget *cpuWdg;
	QWidget *allCpusWdg;
};

// -----------------------------------------------------------------
//! Vxworks specific configuration settings dialog tab
/*!
  It is added to the SettingsDlg dialog.
*/
class OsVxworksTab : public QWidget {
	Q_OBJECT

public:
	OsVxworksTab( OsVxworksConfiguration *_cfg,
				  QWidget *parent, const char *name);
	~OsVxworksTab();
public slots:
    void slotApply();
    void slotCancel();

protected slots:

private:
    OsVxworksConfiguration *cfg;
	SettingList *savedSettings;
};


#endif




