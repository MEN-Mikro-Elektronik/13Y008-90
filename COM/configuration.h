/***************************************************************************
 */
/*!       \file  configuration.h
 *       \brief  Configuration class
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2011/03/31 16:35:27 $
 *    $Revision: 2.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: configuration.h,v $
 * Revision 2.1  2011/03/31 16:35:27  CRuff
 * R: 1. pci domains are supported in MDIS now
 * M: 1a) added member and getter / setter for flag if pci domains are supported
 *        by currently installed system package
 *    1b) added member and getter / setter for flag if current cpu uses several
 *        pci domains
 *        by currently installed system package
 *
 * Revision 2.0  2010/01/22 11:25:36  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.5  2008/10/23 18:02:58  channoyer
 * R: Program name should be "MDIS Wizard" when displayed to the user
 * M: Use of the G_progName constant
 *
 * Revision 1.4  2004/07/21 14:57:53  ufranke
 * added
 *  + MAIN_GetOldCfg(), MAIN_DeleteOldCfg()
 * renamed
 *  - setCurrentCfg(), currentCfg() to MAIN_Set/Get...
 *
 * Revision 1.3  2004/06/08 17:25:09  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.2  2003/06/03 10:16:02  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:34:28  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:19:52  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <qstring.h>
#include <qlist.h>
#include "descriptor.h"
#include "namemng.h"
#include "swmodule.h"
#include "setting.h"

class Device;
class CpuDevice;
class BusInterface;
class HwComponent;
class TargetOs;

extern const QString	G_progName;     //!< main application name

// -----------------------------------------------------------------
//! Configuration 
/*!
  An instance of this class is used as the root object within 
  mdiswiz. It holds pointers to or instances of the following objects:

  - root device
  - target OS
  - device name manager
  - list of software modules

  The global function currentCfg() can be used to get a pointer to the
  current Configuration object, while setCurrentCfg() can be used to set
  a new configuration.

  \remark All SwModule objects are owned by the Configuration
  object. They are automatically deleted when the Configuration object
  is deleted. The same is true for the root device (and all it's
  children)
  
*/
class Configuration : public QObject
{
public:
	Configuration();
	//! destructs Configuration object
	virtual ~Configuration();

	NameMng nameMng;						//!< device name manager

    //! get root device (CPU) 
	CpuDevice *getRootDev() {				
		return root;
	}

	virtual Configuration *create();

	// add a new device to the device hierarchy
	bool addDevice( HwComponent *parentComp, Device *dev, 
					QString &errMsg );
	// remove device from device hierarchy
	void removeDevice( Device *dev );

	//! return title to be used in message boxes
	QString msgBoxCaption(){ return QString(G_progName); }; 
	
	virtual bool save(QString &errMsg);			// save configuration
	virtual bool saveAs(QString &errMsg);		// save configuration as
	virtual bool load( QString &errMsg, const QString &_cfgName=0 ); // load 
	virtual bool settingsDlg();					// bring up project cfg dialog
	virtual bool newProjectWizard( QWidget *parent = 0, 
								   const char *name = 0);
	virtual bool validateSwModules( QString &errMsg, bool disableInvalidMods,
									bool showMsgBox );

	virtual bool validateSwModule( SwModule *mod, QString &errMsg, 
								   bool disableIfInvalid );
	
	//! supply TargetOs pointer 
	void setTargetOs( TargetOs *_os ) { os = _os; }

	//! get TargetOs pointer
	TargetOs *targetOs() { return os; }

	//! get direct pointer to list of SwModule objects
	SwModuleList *getLstSwModules() { return &lstSwModules; }

	//! get name of configuration
	QString configName() { return cfgName; }

	//! test if CPU device would be ok for configuration
	virtual bool cpuOkForCfg( CpuDevice *cpu ) {
		UTIL_UNREF_PARAM(cpu);
		return true;
	}

	//! debug SW module list
	void dbgSwModuleList( void );

	//! set flag: pci domains are supported by installed MDIS package
	void setPciDomainsSupported( bool supportpciDomains ) {
		mdisSupportsPciDomains = supportpciDomains;
	}

	//! read flag: are pci domains supported by installed MDIS package ?
	bool arePciDomainsSupported(  ) {
		return mdisSupportsPciDomains;
	}

	//! set flag: current CPU has several pci domains
	void setPciDomainsNeeded( bool needpciDomains ) {
		cpuUsesPciDomains = needpciDomains;
	}

	//! read flag: does current CPU have several pci domains ?
	bool arePciDomainsNeeded( ) {
		return cpuUsesPciDomains;
	}

protected:	
	void addSwMods( HwComponent *comp );
	void removeSwMods( HwComponent *comp );
	virtual void addSwMod( SwModule *swMod );
	virtual void removeSwMod( SwModule *swMod );

	virtual QString selectConfigName( const QString &action, bool mustExist,
									  QString &errMsg); 

	// descriptor file handling
	bool saveDesc(const QString &systemDescName,
				  QString &errMsg);
	bool loadDesc(const QString &systemDescName,
				  QString &errMsg ); 

	bool loadDevice( 
		CpuDevice **rootDevP,
		DescriptorEntryDirectory *devDesc,
		QString &errMsg);
	bool findDescKey( 
		DescriptorEntryDirectory *devDesc,
		DescriptorFactory::KeyType key,
		QString &errMsg,
		ulong *valueP,
		bool deleteKey=true);
	bool findDescKey( 
		DescriptorEntryDirectory *devDesc,
		DescriptorFactory::KeyType key,
		QString &errMsg,
		QString *valueP,
		bool deleteKey=true);
	bool findBusIf( 
		DescriptorEntryDirectory *devDesc,
		QString &errMsg,
		BusInterface **busIfP,
		CpuDevice *rootDev);

	bool applyDefaultDevProps( HwComponent *comp, QString &errMsg );
	void setAllSwModulesDisabled();

	// attributes
	QString cfgName;						//!< name of configuration
	CpuDevice *root;						//!< root device of configuration

	TargetOs *os;							//!< the OS class

	SwModuleList lstSwModules;				//!< list of Sw modules
	SettingList lstSettings;				//!< list of Settings

	bool mdisSupportsPciDomains;            //!< flag for pciDomain Support
	bool cpuUsesPciDomains;
};

// obtain pointer to currently active configuration
extern Configuration *MAIN_GetCurrentCfg( void );
extern Configuration *MAIN_GetOldCfg( void );
extern void MAIN_DeleteOldCfg( void );
extern void MAIN_SetCurrentCfgDeleteLast( Configuration *cfg );
extern void MAIN_SetCurrentCfgStoreLast( Configuration *cfg );

#endif



