/***************************************************************************
 */
/*!       \file  oslinux.h
 *       \brief  target OS Linux
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2016/04/28 18:00:26 $
 *    $Revision: 2.3 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: oslinux.h,v $
 * Revision 2.3  2016/04/28 18:00:26  ts
 * R: men_mdis_kernel.ko seeks /dev/mdis major number now automatically
 * M: MDIS major number member removed
 *
 * Revision 2.2  2011/03/31 17:19:08  CRuff
 * R: 1. warn user if installed MDIS system package and selected cpu do not match
 * M: 1a) added method checkPciDomains() to show warning dialog
 *    1b) added method mdisSupportsPciDomains() to check if PCI Domains are
 *        supported by installed MDIS system package
 *    1c) added slot menCpuChanged() to also check pci domain compatibility when
 *        the cpu is changed
 *
 * Revision 2.1  2010/07/12 10:06:37  dpfeuffer
 * R: access violation in mdiswiz_lib.lib under Windows with MDIS_CLINST
 * M: QComboBox replaced with Q3ComboBox
 *
 * Revision 2.0  2010/01/22 11:26:56  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.11  2008/02/06 11:49:49  cs
 * R: running mdiswiz for Linux under Windows:
 *     - cygwin/ELinOS requires Linux like paths
 *     - QT requires Windows specific paths.
 * M: pass paths either host or shell specific
 *
 * Revision 1.10  2006/11/08 13:58:25  cs
 * added:
 *   + public method elinosRootEnv() to class OsLinuxConfiguration
 *
 * Revision 1.9  2005/07/15 15:47:13  ub
 * cosmetics
 *
 * Revision 1.8  2004/12/02 10:19:38  kp
 * added swModOkForRtai
 *
 * Revision 1.7  2004/07/16 12:30:49  kp
 * - added Environment help text
 *
 * Revision 1.6  2004/07/14 11:23:44  ufranke
 * added
 *  - environment help
 *
 * Revision 1.5  2004/06/08 17:25:44  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.4  2004/01/09 08:20:12  kp
 * added batchInstall
 *
 * Revision 1.3  2003/06/03 10:16:29  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/05/14 13:44:55  kp
 * misc startup changes
 *
 * Revision 1.1  2003/05/02 14:36:14  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef OSLINUX_H
#define OSLINUX_H

#include <q3wizard.h>
#include <qstringlist.h>
#include <q3vbox.h>
#include <q3combobox.h>
#include <qlabel.h>
#include <qlist.h>
#include <qlineedit.h>
#include <qmap.h>

#include <Q3Frame>

#include "wizdebug.h"
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
class OsLinuxDirectorySetting;
class OsLinuxNewProjectWizard;
class OsLinuxTab;
class QVariant;
class AddComponentWidget;

//! class to implement specific configurations for (standard) Linux
/*!
*/
class OsLinux : public TargetOs {
public:
	OsLinux();

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
//! Configuration class for Linux
/*!
  Extends Configuration
*/
class OsLinuxConfiguration : public Configuration {
	Q_OBJECT
	friend class OsLinuxNewProjectWizard;
	friend class OsLinuxTab;
	friend class OsLinux;
public:
	OsLinuxConfiguration();
	//! destructs OsLinuxConfiguration object
	virtual ~OsLinuxConfiguration() {};
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

	static bool isAutoConfOptionSet( const QString linKernelDir,
									 const QString &option );
	static bool isElinosDir( const QString &dir );
	static bool sourceElinosSh( const QString &dir );
	static QString elinosProjectEnv();
	static QString elinosRootEnv();
	bool elinosCopyFixedFiles();
	static QString hostSpecPath( const QString &path);
	static QString shellSpecPath( const QString &path);

protected:
    //! this extends standard SW module types with RTAI modules
	enum LinuxSwModType {
		RtaiUserLib=0x1000, RtaiLlTool=0x2000, RtaiComTool=0x4000
	};

	void addSwMod( SwModule *swMod );
	void removeSwMod( SwModule *swMod );

	virtual QString selectConfigName( const QString &action, bool mustExist,
									  QString &errMsg );
private:
	bool loadMakefile( QString &errMsg );
	bool saveMakefile( QString &errMsg );
	void enableSwMods( int modType, MkFileTuple *tup, QString &errMsg );
	MkFileTuple *mkSwModuleTups( int modType );
	QString swModuleMfString( int key );
	bool swModOkForRtai( SwModule *swMod );

	Setting *settingLinuxVersion;
	MultipleChoiseSetting *settingCdk, *settingDbg, *settingLibMode,
		*settingRtaiSupport;
	OsLinuxDirectorySetting *settingLinKernelDir, *settingRtaiDir,
		*settingLinUsrIncDir,
		*settingMdisProjDir,    *settingElinosProjDir,
		*settingTargetTree,     *settingModsInstallDir,
		*settingLibInstallDir,  *settingStaticLibInstallDir,
		*settingDescInstallDir,	*settingDevNodeInstallDir,
		*settingBinInstallDir;

	CpuDevice::Arch kernelArch;

	void checkPciDomains( );
	bool mdisSupportsPciDomains( );

private slots:
    void cdkChanged(int index );
	void linKernelDirChanged( const QVariant &newVal );
	void mdisProjDirChanged( const QVariant &newVal );
	void elinosProjDirChanged( const QVariant &newVal );
	void rtaiSupportChanged( const QVariant &newVal );
	void libModeChanged( const QVariant &newVal );
	void menCpuChanged( );
};


//! wizard for new projects
class OsLinuxNewProjectWizard : public Q3Wizard {
	Q_OBJECT

public:
	OsLinuxNewProjectWizard( OsLinuxConfiguration *_cfg, QWidget *parent = 0,
							 const char *name = 0 );
	~OsLinuxNewProjectWizard();
	void showPage(QWidget* page);
public slots:
    virtual void done(int r);
    virtual void next();
private:
	bool validateSetting();
	OsLinuxConfiguration *cfg;
	SettingList *savedSettings;
	AddComponentWidget *cpuWdg;
};

// -----------------------------------------------------------------
//! Linux specific configuration settings dialog tab
/*!
  It is added to the SettingsDlg dialog.
*/
class OsLinuxTab : public QWidget {
	Q_OBJECT

public:
	OsLinuxTab( OsLinuxConfiguration *_cfg, QWidget *parent, const char *name);
	~OsLinuxTab();
public slots:
    void slotApply();
    void slotCancel();

protected slots:

private:
    OsLinuxConfiguration *cfg;
	SettingList *savedSettings;
};

// ------------------------------------------------------------------
//! Linux specific directory setting class
/*!
  The setValue and slotApply method is overridden
*/
class OsLinuxDirectorySetting : public DirectorySetting {
	Q_OBJECT
	public:
		OsLinuxDirectorySetting( QString key, int mode ):
			DirectorySetting( key, mode ) {};

		virtual void setValue( const QVariant &newVal )
		{
			wDebug(("OsLinuxDirectorySetting::setValue: new = %s\n", newVal.toString().latin1()));
			QVariant actVal = QVariant( OsLinuxConfiguration::hostSpecPath(newVal.toString()) );
			if( frm )
				edit->setText( actVal.toString() );

			Setting::setValue( actVal );
		}
	public slots:
		virtual void slotApply( QString &errMsg ){
			if( !frm )
				return;

			QVariant value = QVariant( OsLinuxConfiguration::hostSpecPath( edit->text() ) );
			validate( value, errMsg );
			setValue( value );
		};
};

#endif




