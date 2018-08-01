/***************************************************************************
 */
/*!       \file  hwcomponent.h
 *       \brief  Representation of hardware components and devices
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2016/02/20 16:27:23 $
 *    $Revision: 2.9 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: hwcomponent.h,v $
 * Revision 2.9  2016/02/20 16:27:23  ts
 * R: BoxPCs based on SC24 need 2 SMBusses in MDIS wizard
 * M: 1. added member nrSmbBusses to class CpuDeviceSmb
 *    2. added parameter int nrSmbBusses to every constructor of CPUs with SMB
 *       controllers: 2 for SC24 (AMD based) Box PCs, 1 for every other CPU
 *
 * Revision 2.8  2014/08/22 15:56:04  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.7  2014/07/18 15:11:50  dpfeuffer
 * R: erroneous PMC support, missing ComExpress support, general maintenance
 * M: intermediate check-in during development
 *
 * Revision 2.6  2013/03/04 13:02:45  dpfeuffer
 * R: A21 support
 * M: PpcQorIQ CPU added
 *
 * Revision 2.5  2011/09/26 08:42:20  dpfeuffer
 * R:1. no MEN HW with PC104+ PCI (it is PCI-104)
 *   2. new bus interfaces required: StdPciE, CpciSer
 * M:1. Pc104PlusPci replaced with Pci104
 *   2. added StdPciE, CpciSer
 *
 * Revision 2.4  2011/03/31 16:45:32  CRuff
 * R: 1. support of pci domains
 * M: 1a) class CpuDevice: added flag _hasPciDomains and getter method
 *    1b) class CpuExtDevice: added member and setter for pci domain number
 *    1c) class PciBusInterface: added declaration of virtual method addChild
 *        (will be overridden)
 *
 * Revision 2.3  2010/07/05 17:59:03  CRuff
 * R: implementation of PCI Domains
 * M: 1. added new parameter pciDomain to PciBusInterface constructors
 *    2. added new method getPciDomain()
 *    3. hold pci domain as member (needed for expandable bus interfaces)
 *
 * Revision 2.2  2010/06/02 16:58:25  CRuff
 * R: 1. support new ESM connectors
 *    2. support new processor types
 * M: 1. added new BusIfTypes ESMexpress and ESMini
 *    2. added new cpu cores Atom and Ppc512x
 *
 * Revision 2.1  2010/05/19 15:05:33  CRuff
 * R: 1. class name CpuSmbDevice is misleading
 *    2. need to enable multiple inheritance of class DeviceProperties
 *    3. need new functionality to activate onboard smb controllers
 * M: 1. rename class CpuSmbDevice to CpuDeviceSmb
 *    2. make inheritage of DeviceProperties virtual for DevicePropertiesWithSmb
 *    3a) define new slot activateOnboardSmb
 *    3b) add new members for smbBbis handling to class CpuDeviceSmb
 *
 * Revision 2.0  2010/01/22 11:26:15  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.21  2009/12/07 09:54:43  CRuff
 * R: 1. Smb not recognized as a valid bus interface
 *    2. Smb devices need special device properties
 *    3. differentiation between cpu devices with/without SMB controller needed
 * M: 1. move BusInterface Smb before MaxBusIf in enum BusIfType
 *    2. Implement property class DevicePropertiesWithSmb for boards with Smb
 *       controller (derived from DeviceProperties)
 *    3. implemented new class CpuDeviceSmb
 *
 * Revision 1.20  2009/09/03 10:46:33  CRuff
 * R: 1. preparation for SMB support
 *    2. allow to add native driver as a std sw module
 * M: 1a) added new BusIfType Smb
 *    1b) added class definition for new class SMBusInterface
 *    2. added flag 'isNative' to signature of  makeStdDriverSwModule;
 *       default = false;
 *
 * Revision 1.19  2009/08/21 14:50:48  CRuff
 * R: set address mode capability of BBisSlot (needed for A203N, A08/A24)
 * M: added BbisSlot::setMmodACap() to set the address mode capability
 *    of a BBIS slot
 *
 * Revision 1.18  2009/05/13 11:39:55  CRuff
 * R: 1. Copied CpuDevice objects lack information
 *    2. new device type needed to distinguish between MdisGeneric and MdisCham
 *       devices
 * M: 1a) make Device::copy() method virtual
 *    1b) add CpuDevice::copy() method
 *    2. added new device type Device::MdisCham
 *
 * Revision 1.17  2009/04/27 17:30:41  CRuff
 * R: Provide CPU selection possibility for Intel CPUs
 * M: added new CPU Core type Pentium2
 *
 * Revision 1.16  2009/04/01 15:23:20  CRuff
 * R: 1.Chameleon V2 support
 *    2.cosmetics
 * M: 1.a) extend AutoIdType with new type AutoIdChamV2
 *      b) added new class DeviceChamV2AutoId
 *    2.a) added empty virtual destructors in AutoId classes to avoid warnings
 *
 * Revision 1.15  2006/11/30 11:27:40  cs
 * added:
 *   + BusIfType: Xmc
 *   + HwComponent::insChild(), ::replChild() members
 *
 * Revision 1.14  2005/07/15 15:46:57  ub
 * added CpuCore::PentiumM
 *
 * Revision 1.13  2005/03/09 13:48:30  ub
 * new bus type "ChameleonBus"
 *
 * Revision 1.12  2004/10/07 11:13:27  dpfeuffer
 * cosmetics
 *
 * Revision 1.11  2004/07/30 11:44:00  ufranke
 * added
 *  - isPci()
 *
 * Revision 1.10  2004/07/16 12:34:50  ufranke
 * changed
 *  - cpuCoreGetDefaults() no longer virtual
 *
 * Revision 1.9  2004/06/25 13:40:57  ufranke
 * added
 *  + getCpuCoreDefault()
 *
 * Revision 1.8  2004/06/08 17:25:21  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.7  2004/05/28 14:23:58  dpfeuffer
 * Device::getProp() added
 *
 * Revision 1.6  2004/05/24 08:48:43  dpfeuffer
 * - class DeviceAutoId: AutoIdCham added
 * - class DeviceChamAutoId implemented
 *
 * Revision 1.5  2004/04/13 11:00:39  dpfeuffer
 * some changes for mdiswiz_lib.lib
 *
 * Revision 1.4  2004/01/09 08:19:55  kp
 * added BbSlotspec
 *
 * Revision 1.3  2003/06/11 12:21:30  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/06/03 10:16:08  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:34:45  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:20:10  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef HWCOMPONENT_H
#define HWCOMPONENT_H

#include <qobject.h>
#include <qstring.h>
#include <qlist.h>
#include <q3valuelist.h>
#include <q3memarray.h>

#include <Q3MemArray>
#include <Q3PtrList>

#include "swmodule.h"
#include "typestr.h"

class HwComponent;
class MdiswizView;
class QWidget;
class Q3ListViewItem;
class ResourceMng;
class DevPropertiesDlg;
class DescriptorEntryDirectory;
class BbisSlot;
class MdisDevice;
class ComponentViewItem;
class BusInterface;
class BbisDevice;

// for windows
#ifdef WIN_MDISWIZ_LIB
	#define MDEV_PROP_PARAMS		void
	#include "Windows.h"
#endif

// -----------------------------------------------------------------
//! HwComponent is the base class for all devices and interfaces
/*!HwComponent is an abstract representation of a piece of hardware.
It can have a number of children. E.g. a CPU may have a number of bus
interfaces. A bus interface may have a number of boards etc.
*/
class HwComponent : public QObject
{
 public:
	HwComponent();
	virtual ~HwComponent();

	virtual QString getInstName();		// get instance name of component
	// set instance name of component
	virtual void setInstName( const QString &name );

	HwComponent *getParent();			// get/set parent
	void setParent( HwComponent *_parent );

	// add child to list of children (and set child's parent)
	virtual void addChild( HwComponent *newComp );

	// insert child into list of children
	virtual void insChild( uint i, HwComponent *newComp );

	// replace child in list of children
	virtual void replChild( uint i, HwComponent *newComp );

	// remove child from list of children
	virtual bool removeChild( HwComponent *comp );

	// two methods to scan the list of children
	HwComponent *firstChild();			// get first child
	HwComponent *nextChild(); 			// get next child

	Q3PtrList<HwComponent> &getChildren();	// get direct list access to
	                                    // children

	QString &getDescription();			// get description of component
	void setDescription( const QString &_description );

	virtual QString getMainProperties(); // get main properties in a string

	void setEndianSwapping( bool isSwapping );
	virtual bool isEndianSwapping(); 	// Byteorder swapped by that
	                                    // with respect to CPU?

	// build descriptor
	virtual void createDesc( DescriptorEntryDirectory *parentDesc );
	virtual void createSpecialDesc( DescriptorEntryDirectory *parentDesc );

	// find child by instance name (recursive)
	HwComponent *findChild( const QString &_instName );

    // find child by instance pointer (recursive)
	HwComponent *findChild( HwComponent *dstComp );

	//! electrical bus interfaces
	enum BusIfType { Vme, StdPci, StdPciE, Cpci, CpciSer, Mmod, PcMip, Pmc, Xmc, LocalBus, PciOrCham,
					 Pci104, ChameleonBus, Smb, ESMexpress, ESMini, ComExpr,
					 MaxBusIf, InvalidBusIf=-1 };
	//! class to map bus interfaces and their strings
	class BusIfTypeMap: public TypeStrMap<BusIfType> {
	public:	BusIfTypeMap();
	};
	static BusIfTypeMap busIfTypeMap; //!< instantiation of BusIfTypeMap

	//! M-module data width capabilites
	enum MmodDataCaps { MmodDInvalid=-1, MmodD16=1, MmodD32=2 };
	//! class to map M-module addr caps and their strings
	class MmodDataMap: public TypeStrMap<MmodDataCaps> {
	public:	MmodDataMap();
	};
	static MmodDataMap mmodDataMap;	//!< instantiation of MmodDataMap

	//! M-module addressing capabilites
	enum MmodAddrCaps { MmodAInvalid=-1, MmodA08=1, MmodA24=2 };
	//! class to map M-module data width and their strings
	class MmodAddrMap: public TypeStrMap<MmodAddrCaps> {
	public:	MmodAddrMap();
	};
	static MmodAddrMap mmodAddrMap;	//!< instantiation of MmodAddrMap

 protected:
	QString myName;						//!< instance name of component
	QString description;				//!< short description of component
	HwComponent *parent;				//!< parent that hosts this component
	Q3PtrList<HwComponent> lstChildren;		//!< children of that component
	bool swapping;						//!< component swaps byteorder
};

//! class to keep track of debug level settings of a device
class DebugProperties {
public:
	//! defines the modules to be debugged
	enum Module {
		Driver, BbisKernel, MdisKernel, Oss, Desc, Mbuf, _Max, Invalid=-1
	};
	//! defines bit masks for debug levels
	enum Level {
		Int=0x80000000, Normal=0x40000000, Error=0x8000,
		Lev3=0x4, Lev2=0x2, Lev1=0x1
	};

	DebugProperties();
	void setModules( ulong modMask );
	//! get the debug setting modules used by that device
	ulong getModules() { return modules; }
	bool hasModule( Module mod );
	//! tell if there is something to select
	bool hasModules() { return modules!=0; }
	void setModuleLevel( Module mod, ulong level );
	ulong moduleLevel( Module mod );

	//! class to map debug module types and their strings
	/*! (values displayed to user) */
	class ModuleMap: public TypeStrMap<Module> {
	public:	ModuleMap();
	};
	static ModuleMap moduleMap; //!< instantiation of ModuleMap

	//! class to map debug module types and their strings
	/*! (as used for descriptor keys) */
	class ModuleDescMap: public TypeStrMap<Module> {
	public:	ModuleDescMap();
	};
	static ModuleDescMap moduleDescMap; //!< instantiation of ModuleDescMap

private:
	//! contains the debug settings for each module
	ulong dbgSetting[_Max];
	//! modules used by this device
	ulong modules;
};

// -----------------------------------------------------------------
//! class containing all parameters of a device
// Derived by models
class DeviceProperties {
public:
    //! create device properties
	DeviceProperties() {};
    //! destroy device properties
	virtual ~DeviceProperties() {} ;
	QString deviceName;		//!< holds device name of device
	DebugProperties dbgSettings; //!< holds debug settings of device
};

// -----------------------------------------------------------------
//! class containing all parameters of a device
// Derived by models
class DevicePropertiesWithSmb : virtual public DeviceProperties {
public:
	//! create device properties
	DevicePropertiesWithSmb() {
		obSmbControllerActive = false;
		chamSmbControllersActive = false;
		numChamSmbControllers = 0;
	}

    //! destroy device properties
	virtual ~DevicePropertiesWithSmb() {} ;

	bool obSmbControllerActive;		//!< are the onboard smb controllers activated?
	bool chamSmbControllersActive; 	//!< are chameleon z001 controllers activated?
	int numChamSmbControllers;		//!< number of used z001 controllers
};

// -----------------------------------------------------------------
//! class to describe auto-identification of Devices.
class DeviceAutoId {
public:
	//! return value of getType
	enum AutoIdType {
		AutoIdNone,				//!< not auto-identifyable
		AutoIdMmod,				//!< M-module
		AutoIdPci,				//!< PCI device
		AutoIdCham, 			//!< Chameleon device
		AutoIdChamV2			//!< ChameleonV2 device
	};
	DeviceAutoId() { refCount=0; };

	virtual ~DeviceAutoId(){};

    //! get type of auto-id-info
	virtual AutoIdType getType() { return AutoIdNone; }
	int refCount;				//!< keep track of refernences to this object
};

// -----------------------------------------------------------------
//! class to describe auto-identification of M-modules
class DeviceMmodAutoId : public DeviceAutoId {
public:
	//! create an instance of DeviceMmodAutoId class
	/*!
	  \param _id M-module-id word, 32 bit: MSW: magic word, LSW: module number
	*/
	DeviceMmodAutoId( ulong _id ) { id = _id; }
	virtual ~DeviceMmodAutoId(){};
	virtual AutoIdType getType() { return AutoIdMmod; }
	//! MSW: mmodule magic, LSW: m-module number
	ulong id;
};

// -----------------------------------------------------------------
//! class to describe auto-identification of PCI devices
class DevicePciAutoId : public DeviceAutoId {
public:
	DevicePciAutoId() {
		pciVendorId = pciDeviceId = pciSubvenId = pciSubsysId = 0xFFFFFFFF;
	}
	virtual AutoIdType getType() { return AutoIdPci; }
	virtual ~DevicePciAutoId(){};
	//! PCI ids: if set to 0xFFFFFFFF, means any device ID
	ulong pciVendorId, pciDeviceId, pciSubvenId, pciSubsysId;
};

// -----------------------------------------------------------------
//! class to describe auto-identification of Chameleon devices
class DeviceChamAutoId : public DeviceAutoId {
public:
	//! create an instance of DeviceChamAutoId class
	/*!
	  \param _id Chameleon-id word, 32 bit: LSB: chameleon unit module code
	*/
	DeviceChamAutoId( ulong _id) {
		deviceId = _id;
	}
	virtual ~DeviceChamAutoId() {};

	virtual AutoIdType getType() { return AutoIdCham; }

	//! Device identification information
	ulong deviceId;
};

// -----------------------------------------------------------------
//! class to describe auto-identification of Chameleon devices (V2)
class DeviceChamV2AutoId : public DeviceChamAutoId {
public:
	//! create an instance of DeviceChamV2AutoId class
	/*!
	  \param _id Chameleon-id word, 32 bit: LSB: chameleon unit module code
	*/
	DeviceChamV2AutoId( const ulong _chamv2id,
						const ulong _modId=0 )
						: DeviceChamAutoId( _modId)
	{
		deviceId = _chamv2id;
		originalChamId = _modId;
		chamv2grpList = new Q3ValueList<ulong>();
	}

	//! Destructor for class DeviceChamV2AutoId
	virtual ~DeviceChamV2AutoId()
	{
		if (chamv2grpList)
		{
			chamv2grpList->clear();
			delete chamv2grpList;
			chamv2grpList = NULL;
		}
	}

	virtual AutoIdType getType() { return AutoIdChamV2; };

	//! original chameleon id (optional for cham v2 devices)
	ulong originalChamId;

	//! List of attached group devices (if applicable)
	Q3ValueList<ulong>* chamv2grpList;
};

// -----------------------------------------------------------------
//! represents a device (Cpu, carrier board, mdis device)
// Derived by CpuDevice, BbisDevice, MdisDevice
class Device : public HwComponent
{
	Q_OBJECT
public:
	//! return value of applyProperties()
	enum ApplyResult {
		ApplyPropOk, 			//!< all resources allocates as requested
		ApplyPropAlternate, 	//!< ok, but alternate resources allocated
		ApplyPropFail			//!< failed, no resources
	};

	Device( const QString &_hwName, const QString &_driverName,
			bool isRemoveable );
	virtual ~Device();

    //! create another instance of the same device
	/*!
	  if \a withSubDevs is true create the device with all
	  built-in subdevices (e.g. built-in BBIS devs)
	*/
	virtual Device *create(bool withSubDevs=true)=0;
	virtual void copy( const Device *org );

	//! return value of type()
	enum Type { Mdis, MdisCham, Bbis, Cbis };

	//! get the type of device, see Device::Type
	virtual Type type() = 0;

	// set instance name of component
	virtual void setInstName( const QString &name );

	//! tell wether user is allowed to change instance name of device
	virtual bool isInstNameChangeAllowed() {
		return instNameChangeAllowed;
	}
	//! set wether user is allowed to change instance name of device
	virtual void setInstNameChangeAllowed( bool allowed ){
	    instNameChangeAllowed = allowed;
	}

	virtual bool isRemoveable();		// device can be removed?
    // get driver name
	virtual QString getDriverName( bool fullName=false, bool withSw=true);
	virtual void setDriverName( const QString &_driverName );

	virtual QString getDeviceNameTpl();	// get device name template

	QString getModelName();				// get model name
	//! set model name of device. \sa getModelName()
    void setModelName( const QString &_modelName ) {
		modelName = _modelName;
	}

	//! get hardware vendor name
	QString getVendorName() { return vendorName; }
	//! set hardware vendor name
	void setVendorName( const QString &_vendorName) {
		vendorName = _vendorName;
	}

	//! get hardware name
	QString getHwName() { return hwName; }
	//! set hardware name. Also sets model name to the same string.
	void setHwName( const QString &_hwName) {
		hwName = modelName = _hwName;
	}

	QString getVendorModelName();
	QString getVendorHwName();

	virtual BusIfType getBusIfType() { return ifType; };

	DeviceAutoId *autoId();
	void setAutoId( DeviceAutoId *_newAutoId );

	virtual ComponentViewItem *createGui(MdiswizView *wizView,
										 Q3ListViewItem *parent);
	virtual bool propertiesDialog( QWidget *parent ); // show properties dialog

// for windows
#if ((defined WIN_MDISWIZ_LIB) || (defined WIN_CLASS_INSTALLER))

	bool propertiesDialog4Win( void *ppParamsContext );  // show properties dialog

	bool getDescParamsFromReg(
		HKEY						parentKeyH,
		char						*keyName,
		DescriptorEntryDirectory	*currentDir );

	bool enumRegValues(
		HKEY						paramsKeyH,
		DescriptorEntryDirectory	*currentDir );

	bool setDescParamsInReg( HKEY	parentKeyH );

	bool setRegValues(
		HKEY						dirKeyH,
		DescriptorEntryDirectory	*descEntryDir,
		bool						*entryExist );

	void showDialog( void );

#endif // WIN32

	virtual ApplyResult applyProperties( DeviceProperties *newProp,
										 QString &errMsg );
	virtual ApplyResult applyProperties( DescriptorEntryDirectory *devDesc,
										 QString &errMsg );
	virtual void freeResources();

	//! get list of SwModule used by that device
	/*!
	  \return the SwModule list used by that device. This must
	  not be modified by caller and is still owned by the device
	*/
	virtual SwModuleList *enumSwModules() { return lstSwModules; }

	//! set list of SwModule used by that device
	/*!
	  this is mainly used by ModMdisGeneric from the PackageFile class
	*/
	virtual void setSwModules( SwModuleList *lst ){
		if( lstSwModules ) delete lstSwModules;
		lstSwModules = lst;
	}

	DeviceProperties* getProp() {
		return prop;
	}

protected:
	virtual DevPropertiesDlg *createPropDlg( QWidget *parent );
	virtual DevPropertiesDlg *createPropDlg4Win( QWidget *parent );
	virtual DeviceProperties *createProperties();
	virtual void copyProperties( const DeviceProperties *from,
								 DeviceProperties *to );
	SwModule *makeStdDriverSwModule( const QString &baseDir,
									 const QString &variant,
									 bool ignoreSwapping=false,
									 const QString &descriptionOverride=0,
									 bool isNative=false);

	void createDebugDesc( DescriptorEntryDirectory *parentDesc );

	QString modelName;					//!< name of model (not instance)
	QString driverName;					//!< name of non-swapped driver
	QString vendorName;					//!< name of HW vendor
	QString hwName;						//!< name of hardware (without variant)
	bool removeable;					//!< is device removeable
	BusIfType ifType;					//!< upper bus interface type
	bool resourcesAllocated;			//!< resources have been allocated
	DeviceProperties *prop;				//!< current device properties
	DeviceProperties *orgProp; 			//!< original device properties
	DeviceProperties *modProp;			//!< modified device properties
	DevPropertiesDlg *dialog;			//!< current properties dialog
	SwModuleList *lstSwModules;			/*!< software modules of that device
										  class */
	DeviceAutoId *_autoId;				//!< auto id info of device */
	bool instNameChangeAllowed;			//!< instance name change allowed
private:
	bool propertiesDlgApplied;

protected slots:
    void slotPropertiesApply();

};

// -----------------------------------------------------------------
//! representation of a CPU board

class CpuDevice : public Device
{
public:
	CpuDevice(const QString &_hwName, const QString &_driverName,
			  bool isRemoveable);

	virtual Type type() { return Cbis; }
	// create another instance
	virtual Device *create(bool withSubDevs=true);

	virtual ComponentViewItem *createGui(MdiswizView *wizView,
										 Q3ListViewItem *parent);
	// build descriptor
	virtual void createDesc( DescriptorEntryDirectory *parentDesc );

	virtual SwModuleList *enumSwModules();

	static SwModuleList *enumSwModulesForExternalPackage( QString modelName );

	//! CPU architecture
	enum Arch { UnsetArch, X86, Ppc, M68k, Mips, Arm };

	//! get CPU architecture
	/*! must be reimplemented in subclasses */
	virtual Arch getArch() { return UnsetArch; }

	//! Cpu core types
	enum CpuCore {
		UnsetCpuCore = -1,
		// X86
		I386, I486, Pentium, Pentium2, Pentium3, Pentium4, PentiumM, Duron, Athlon,
        Crusoe, Atom,
		// PPC
		Ppc4xx, Ppc512x, Ppc603, Ppc603e, Ppc604, Ppc740, Ppc750, Ppc74xx, Ppc8xx,
		PpcE500, PpcQorIQ,
		// 68K
		M68000, M68010, M68020, M68030, M68040, M68060, MCpu32,
		// Arm
		IMX6
		// Mips???
	};
	//! get possible CPU core types
	/*! lists all possible CPU cores possible for that CPU board */
	const Q3ValueList<CpuCore> &getLstCpuCores() { return lstCpuCores; }

	//! get core default
	CpuCore getCpuCoreDefault( void );

	virtual void copy( const CpuDevice *org );

	//! read flag: does this cpu have several pci host controllers?
	bool hasPciDomains () {
		return this->_hasPciDomains;
	}

protected:
	Q3ValueList<CpuCore> lstCpuCores; /**< list of CPU cores. to be initialized
									   by subclasses */

	bool _hasPciDomains;

};

// -----------------------------------------------------------------
//! representation of a CPU extension/carrier board

class CpuExtDevice : public CpuDevice
{
public:
	CpuExtDevice(const QString &_hwName, bool isRemoveable, BusIfType _ifType);
	virtual SwModuleList *enumSwModules();

	//! set pci domain of the cpu extension board pci bus
	void setPciDomain( int pciDom ) {
		this->pciDomain = pciDom;
	}

protected:
	//! pci domain of this cpu extension board pci bus
	int pciDomain;
};

// -----------------------------------------------------------------
//! representation of a CPU board with SMBus controller(s)

class CpuDeviceSmb: public CpuDevice
{
	Q_OBJECT
public:
	CpuDeviceSmb(const QString &_hwName, const QString &_driverName, bool isRemoveable, bool hasObSmb, int nrSmbBusses );

	virtual DevPropertiesDlg *createPropDlg( QWidget *parent );
	virtual DeviceProperties *createProperties();
	virtual void copyProperties( const DeviceProperties *from,
								 DeviceProperties *to );

		
	// flag to decide whether chameleon smb controllers are possible for this device
	bool hasObSmb;
	bool obSmbActive;
	int nrSmbBusses;
	
	// onboard smb controllers
	BusInterface *smbusIf;
	QList<BbisDevice*> smbBbisList;
	
	
protected slots:
	void activateOnboardSmb( bool active, MdiswizView* wizview );
	
};

// -----------------------------------------------------------------
//! representation of a BBIS device (carrier board for MDIS devices)

class BbisDevice : public Device
{
public:
	BbisDevice( const QString &_hwName, const QString &_driverName,
				bool isRemoveable, BusIfType _ifType );

	Type type() { return Bbis; }
	// create another instance
	virtual Device *create(bool withSubDevs=true);
	virtual SwModuleList *enumSwModules();

	BbisSlot *bbSlot( int slotNum );

	virtual ComponentViewItem *createGui(MdiswizView *wizView,
										 Q3ListViewItem *parent);
	// build descriptor
	virtual void createDesc( DescriptorEntryDirectory *parentDesc );
};

// -----------------------------------------------------------------
//! Specification of a BBIS slot on a specific carrier board
/*!
  Internal representation of the packagedesc model's \em bbslot
  tag.
  Used for onboard devices to specify on which bbis slots the device
  can be connected
*/
class BbSlotSpec {
public:

	//! create a BbSlotSpec object
	/*
	  \param bbModel	name of bbis model
	  \param bbInstance	name of bbis instance
	  \param slotNo		the slot number to which the device can be
	                    connected. If -1, can be connected to any slot
						of \a bbModel

	  \a bbModel and \a bbInstance are mutually exlusive. Only one
	  should be named. The other should be passed as QString::null
	*/
	BbSlotSpec( const QString &bbModel,
				const QString &bbInstance,
				int slotNo ) {
		_bbModel = bbModel;
		_bbInstance = bbInstance;
		_slotNo = slotNo;
	}

	QString _bbModel;	//!< name of bbis model
	QString _bbInstance;//!< name of bbis instance
	int _slotNo;		/**< the slot number to which the device can be
						   connected. If -1, can be connected to any slot
						   of \a bbModel */
};

// -----------------------------------------------------------------
//! List of BbSlotSpec objects

class BbSlotSpecList : public Q3PtrList<BbSlotSpec> {
public:
	//! create an empty BbSlotSpec list
	/*! items are owned by list */
	BbSlotSpecList() {
		setAutoDelete(true);
	}
	void add( BbSlotSpec *spec );
	void add( BbSlotSpecList *lst );
};

// -----------------------------------------------------------------
//! representation of a MDIS device

class MdisDevice : public Device
{
public:
	MdisDevice( const QString &_hwName, const QString &_driverName,
				bool isRemoveable, BusIfType _ifType );

	virtual Type type() { return Mdis; }
	// create another instance
	virtual Device *create(bool withSubDevs=true);

	void copy( const MdisDevice *org );

	BusIfType getDevIfType(); // get device interface

	//! return list of BbSlotSpec objects.
	/*!
	  returns 0 if no list has been created. (means device
	  can be connected according to getDevIfType() )
	*/
	BbSlotSpecList *getBbSlotSpecList() {
		return bbSlotSpecList;
	}

	//! add bbSlotSpec to list of of BbSlotSpec objects
	/*! added object must not be destroyed by caller! */
	void addBbSlotSpec( BbSlotSpec *spec ) {
		if( ! bbSlotSpecList )
			bbSlotSpecList = new BbSlotSpecList;
		bbSlotSpecList->add( spec );
	}

	//! add bbSlotSpecList to list of of BbSlotSpec objects
	/*! added object must not be destroyed by caller! */
	void addBbSlotSpec( BbSlotSpecList *lst ) {
		if( ! bbSlotSpecList )
			bbSlotSpecList = new BbSlotSpecList;
		bbSlotSpecList->add( lst );
	}

	//! get M-Module data capabilty requirements
	MmodDataCaps mmodDataRequirements() { return mmodDReq; }
	//! set M-Module data capabilty requirements
	void setMmodDataRequirements( MmodDataCaps caps ) { mmodDReq = caps; }

	//! get M-Module address capabilty requirements
	MmodAddrCaps mmodAddrRequirements() { return mmodAReq; }
	//! set M-Module address capabilty requirements
	void setMmodAddrRequirements( MmodAddrCaps caps ) { mmodAReq = caps; }

	virtual ComponentViewItem *createGui(MdiswizView *wizView,
										 Q3ListViewItem *parent);

	// build descriptor
	virtual void createDesc( DescriptorEntryDirectory *parentDesc );

	virtual QString getMainProperties(); // get main properties in a string

	//! get number of subdevices of this MDIS device
	/*!
	  a value of 1 means no subdevices.
	  To be reimplented by subclasses
	*/
	virtual int numSubDevs() { return 1; }

	//! create a subdevice of that device (not the main device)
	/*!
	  Must be reimplemented by devices that consist of more that one subdev.
	  \param subDevNo the subdevice number 1..n
	*/
	virtual Device *createSubDev( int subDevNo ) { subDevNo=subDevNo; return 0; }
	//! set wether device is a true MDIS device or a native driver
	void setNativeDevice( bool isNative ){
		_nativeDevice = isNative;
	}
	//! tell wether device is a true MDIS device or a native driver
	bool isNativeDevice() { return _nativeDevice; };

protected:
	BusIfType ifType;	//!< upper interface of MDIS device
	MmodDataCaps mmodDReq; //!< M-Module data cap. requirements
	MmodAddrCaps mmodAReq; //!< M-Module addr cap. requirements
	bool _nativeDevice;	   //!< is not an MDIS device, but uses MDIS
	BbSlotSpecList *bbSlotSpecList; //!< list of BbSlotSpec objects. Can be 0
};

// -----------------------------------------------------------------
//! representation of a Bus Interface
/*!
a BusInterface is a container to connect BBIS devices. For example, a CPU
may have a VME BusInterface, a CompactPCI BusInterface...
*/
class BusInterface : public HwComponent
{
 public:
	BusInterface( BusIfType _ifType, bool _isExpandeable = false );

	// check if BBIS device can be connected
	bool canConnect( Device *bbisDev );

	// check if user can attach more BBISes
	bool isExpandeable();

	// get the bus interface number (of the CPU)
	int getBusIfNum();

    //! get interface type of the BusInterface
	BusIfType getBusIfType(){return ifType;}

    //! get info if it is a PCI serial interface
	bool getIsPciE( void ){
		if( (ifType == CpciSer) ||
			(ifType == StdPciE) ||
			(ifType == Xmc) ||
			(ifType == ESMexpress) ||
			(ifType == ComExpr) )
			return TRUE;
		else
			return false;
	}

	void setIsPci( bool _isPci )
	{
		isPci = _isPci;
	}

	bool getIsPci( void )
	{
		return( isPci );
	}

	virtual QString getMainProperties(); // get main properties in a string

 protected:
	BusIfType ifType;		//!< type of that bus interface
	bool      isPci;
	bool expandeable;		//!< true if more BBIS devices can be attached
};

// -----------------------------------------------------------------
//! representation of a Vme Bus Interface

class VmeBusInterface : public BusInterface {
public:
	VmeBusInterface();
	virtual ~VmeBusInterface();

	virtual ulong supportedIrqLevels();

	// resources for VME A16, A24, A32 space memory regions
	// they exist always, even if the businterface doesn't support it
	ResourceMng *vmeA16Res; //!< resource manager for VME A16 space
	ResourceMng *vmeA24Res;	//!< resource manager for VME A24 space
	ResourceMng *vmeA32Res;	//!< resource manager for VME A32 space

	//! resource for VME IRQ vectors
	ResourceMng *vmeIrqVectorRes;
};

// -----------------------------------------------------------------
//! representation of a SMBus Interface

class SMBusInterface : public BusInterface {
public:
	SMBusInterface();
	virtual ~SMBusInterface() {}

};

// -----------------------------------------------------------------
//! representation of a PCI Bus Interface
/*!
may be standard PCI, CompactPCI etc.
*/

class PciBusInterface : public BusInterface {
public:
	PciBusInterface( BusIfType _ifType,
					 int _minSlot=-1, int _maxSlot=-1, int _minSlotDevId=-1,
					 const Q3MemArray<uchar> *_basePciPath=0,
					 bool isExpanable=true, int pciDomain=0);
	PciBusInterface( BusIfType _ifType,
					 int _minSlot, int _maxSlot,
					 const Q3MemArray<uchar> *_basePciPath,
					 const Q3MemArray<uchar> *_directIdTbl,
					 bool isExpanable=true, int pciDomain=0);

	virtual ~PciBusInterface();

	// get PCI bus path
	virtual bool hasPciBusPath();
	virtual bool pciBusPath( int slotNo, Q3MemArray<uchar> *pciPath );
	virtual int busPathToSlot( const Q3MemArray<uchar> &pciPath );
	
	int getPciDomainNo() { return pciDomainNo; };

	// add child to list of children (override to add pci Domain to child)
	virtual void addChild( HwComponent *newComp );
	int getNbrOfFreeSlots();
	int getNbrOfSlots();
	void setPciDomainNoUnknown( void ){
		pciDomainNoUnknown = true;
	}
	bool getPciDomainNoUnknown( void ){
		return pciDomainNoUnknown;
	}

	//! resource for used slots
	ResourceMng *pciSlots;
protected:
	int minSlot;	//!< first slot number
	int maxSlot;	//!< last slot number
	int minSlotDevId;	//!< PCI device ID of minSlot
	Q3MemArray <uchar> basePciPath;	//!< PCI path to that bus
	Q3MemArray <uchar> directIdTbl; //!< don't autonumber, use the IDs from table
	int pciDomainNo;
	bool pciDomainNoUnknown;	//! true indicates that the domain number is not known 
private:
	void init( int _minSlot, int _maxSlot, int _minSlotDevId,
			   const Q3MemArray<uchar> *_basePciPath,
			   const Q3MemArray<uchar> *_directIdTbl);
			   
};

// -----------------------------------------------------------------
//! representation of a BBIS slot
/*!
a BBIS device will have one or more BBIS slots. Each slot is asigned
an arbitrary number.

For PCI carriers, it can hold additionally the PCI device number of that
slot.
*/

class BbisSlot : public HwComponent
{
public:
	BbisSlot( BusIfType _ifType, int _slotNum,
			  bool _bbSlotSpecRequired=false, const QString &_slotName = 0);
	BbisSlot( BusIfType _ifType, int _slotNum, int _pciDevNo,
			  bool _bbSlotSpecRequired=false, const QString &_slotName = 0);
	BbisSlot( int _slotNum, MmodDataCaps dcap, MmodAddrCaps acap,
			  bool _bbSlotSpecRequired=false, const QString &_slotName = 0 );

	// check if MDIS device can be connected
	bool canConnect( MdisDevice *mdisDev );

	int getSlotNum();
	int getPciDevNo();

    //! get slot interface type
	BusIfType getIfType() { return ifType; }

	QString getSlotName();
	virtual QString getInstName () { return getSlotName(); }

	void setSlotName( const QString &slotName );

	void setBbSlotSpecRequired( bool _bbSlotSpecRequired );

	void setMmodACap( MmodAddrCaps addrCap ) { mmodACap = addrCap; }

protected:
	BusIfType ifType;	//!< interface type of that slot
	int slotNum;		//!< MDIS slot number
	int pciDevNo;		//!< PCI device number of slot
	MmodDataCaps mmodDCap; //!< M-module slot data capabilities
	MmodAddrCaps mmodACap; //!< M-module slot address capabilities
	bool bbSlotSpecRequired; /*!< true to tell canConnect() to check
							   mdisDevs that have bbSlotSpecList */
	QString slotName;	//!< if non-0 overwrites default slot name
private:
	void init( BusIfType _ifType, int _slotNum, int _pciDevNo,
			   MmodDataCaps dcap, MmodAddrCaps acap,
			   bool _bbSlotSpecRequired=false, const QString &_slotName = 0 );
};


#endif





