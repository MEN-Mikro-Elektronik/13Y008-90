/***************************************************************************
 */
/*!       \file  osqnx.h
 *       \brief  target OS QNX
 *      \author	 
 *		  $Date: 2011/04/12 09:19:31 $
 *    $Revision: 2.2 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: osqnx.h,v $
 * Revision 2.2  2011/04/12 09:19:31  CRuff
 * R: 1. warn user if installed MDIS system package and selected cpu do not match
 * M: 1a) added method checkPciDomains() to show warning dialog
 *    1b) added method mdisSupportsPciDomains() to check if PCI Domains are
 *        supported by installed MDIS system package
 *
 * Revision 2.1  2010/07/12 10:06:52  dpfeuffer
 * R: access violation in mdiswiz_lib.lib under Windows with MDIS_CLINST
 * M: QComboBox replaced with Q3ComboBox
 *
 * Revision 2.0  2010/01/22 11:27:09  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.2  2008/10/23 18:04:00  channoyer
 * Cosmetic in comments
 *
 * Revision 1.1  2008/07/23 11:49:40  gvarlet
 * Initial Revision
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef OSQNX_H
#define OSQNXS_H

#include <q3wizard.h>
#include <qstringlist.h>
#include <q3vbox.h>
#include <q3combobox.h>
#include <qlabel.h>
#include <qlist.h>

#include <Q3Frame>

#include "setting.h"
#include "targetos.h"
#include "swmodule.h"
#include "configuration.h"
#include "hwcomponent.h"

class AddComponentWidget;
class MkFileTuple;
class OsQnxNewProjectWizard;
class OsQnxTab;
class Q3Frame;
class Q3VBox;
class Q3ComboBox;
class QLineEdit;
class QVariant;

//----------------------------------------------------------------------
//! class to implement specific configurations for QNX
/*!
*/
class OsQnx : public TargetOs
{
public:
	OsQnx();
	
	bool 			batchInstall( QString pkgFile, QString &errMsg  );
	Configuration*	createConfiguration();
	QString 		driverName( QString plainName, Device *dev );
	QString 		helpEnv( void );
	QString 		helpPath( void );
	bool 			installDlg( void );
	QString 		mdiswizVersion( void );
	QString 		pkgDbPath( void );
	bool 			startup( int argc, char **argv );
	
private:

};

//----------------------------------------------------------------------
//! Configuration class for Qnx
/*!
	Extends Configuration
*/
class OsQnxConfiguration : public Configuration
{
	Q_OBJECT
	friend class OsQnxNewProjectWizard;
	friend class OsQnxTab;
	friend class OsQnx;
public:
	OsQnxConfiguration( void );
	//! destructs OsQnxConfiguration object
	virtual ~OsQnxConfiguration() {};
	
	Configuration			*create( void );
	virtual bool 			cpuOkForCfg( CpuDevice *cpu )
	{
		if( cpu->getArch() == CpuDevice::UnsetArch ){
			return( false );
		}
		return( true );
	};
	// Get the current CPU Arch
	virtual CpuDevice::Arch	getCpuArch( void ) { return cpuArch; }
	virtual bool 			load( QString &errMsg, const QString &_cfgName=0 );
	virtual bool 			newProjectWizard(
								QWidget *parent,
								const char *name );
	virtual bool 			save( QString &errMsg );
	virtual bool 			saveAs( QString &errMsg );
	virtual bool 			settingsDlg( void );
	// Set the current CPU Arch
	virtual void			setCpuArch( CpuDevice::Arch Arch )
	{
		cpuArch = Arch;
		if( cpuArch == CpuDevice::X86 )
			settingCpuArch->setValue( "x86" );
		else if( cpuArch == CpuDevice::Ppc )
			settingCpuArch->setValue( "ppcbe" );
		else
			settingCpuArch->setValue( "none" );
	};
	// Set the MEN PLDZ002 VME support for current CPU
	virtual void			setVmeSupport( QString VmeSupport )
	{
		if( VmeSupport == "yes" ||
			VmeSupport == "no" )
			settingVmeSupport->setValue( VmeSupport );
		else
			settingVmeSupport->setValue( "" );
	}
	virtual bool			validateSwModule(
								SwModule *mod,
								QString &errMsg,
								bool disableInvalidMods );

protected:
	virtual QString 		selectConfigName( 
								const QString &action,
								bool mustExist,
								QString &errMsg );
	
private:
	CpuDevice::Arch			cpuArch;
	MultipleChoiseSetting	*settingDbgQnx;
	MultipleChoiseSetting	*settingLibMode;
	DirectorySetting 		*settingMdisProjDir;
	DirectorySetting 		*settingMenQnxDir;
	Setting 				*settingBinDir;
	Setting 				*settingDescDir;
	Setting 				*settingCpuArch;
	Setting 				*settingLibDir;
	Setting 				*settingVmeSupport;
	Setting 				*settingStaticLibDir;
	Setting 				*settingTargetDir;
	
	bool					loadMakefile( QString &errMsg );
	QString 				loadMakefileKeyVal( QString &errMsg, QString key );
	bool 					saveMakefile( QString &errMsg );
	MkFileTuple 			*mkSwModuleTups( int modType );
	QString 				swModuleMfString( int key );
	void 					enableSwMods( 
								int modType,
								MkFileTuple *tup,
								QString &errMsg );

	void checkPciDomains( );
	bool mdisSupportsPciDomains( );

private slots:
	void menQnxDirChanged( const QVariant &newVal );
	void mdisProjDirChanged( const QVariant &newVal );
	void menCpuChanged( );
	
};

//----------------------------------------------------------------------
//! wizard for new projects
class OsQnxNewProjectWizard : public Q3Wizard
{
	Q_OBJECT

public:
	OsQnxNewProjectWizard(	OsQnxConfiguration	*_cfg,
						  	QWidget 			*parent = 0,
							const char			*name = 0 );
	~OsQnxNewProjectWizard( void );
	void showPage(QWidget* page);
	
public slots:
	virtual void done( int r );
    virtual void next( void );
	
private:
	OsQnxConfiguration	*cfg;
	AddComponentWidget	*cpuWdg;
	SettingList			*savedSettings;
	
	bool validateSetting( void );
	
};

//----------------------------------------------------------------------
//! Qnx settings dialog tab
class OsQnxTab : public QWidget
{
	Q_OBJECT

public:
	OsQnxTab(	OsQnxConfiguration *_cfg,
				QWidget *parent, const char *name);
	~OsQnxTab();
	
public slots:
    void slotApply( void );
    void slotCancel( void );

private:
    OsQnxConfiguration	*cfg;
	SettingList			*savedSettings;
};

#endif


