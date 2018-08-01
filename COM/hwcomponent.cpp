/***************************************************************************
 */
/*!       \file  hwcomponent.cpp
 *       \brief  Representation of hardware components and devices
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2016/04/29 10:37:16 $
 *    $Revision: 2.10 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: hwcomponent.cpp,v $
 * Revision 2.10  2016/04/29 10:37:16  ts
 * R: under VxWorks a capital section name "CPU" gets redefined by CPU = PPC32, causing compile errors.
 * M: generate lower case CPU name
 *
 * Revision 2.9  2016/02/20 16:27:21  ts
 * R: BoxPCs based on SC24 need 2 SMBusses in MDIS wizard
 * M: 1. added member nrSmbBusses to class CpuDeviceSmb
 *    2. added parameter int nrSmbBusses to every constructor of CPUs with SMB
 *       controllers: 2 for SC24 (AMD based) Box PCs, 1 for every other CPU
 *
 * Revision 2.8  2014/12/17 07:48:45  dpfeuffer
 * R: customer requires upper case device names
 * M: Device::setInstName(): .lower() removed
 *
 * Revision 2.7  2014/08/22 15:56:02  dpfeuffer
 * R:1. inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 *   2. occupied PCIe slots not grayed out
 * M:1. PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *   2. PciBusInterface::getNbrOfFreeSlots() added for CpuDeviceViewItem::showPopup()
 *
 * Revision 2.6  2014/07/18 15:11:47  dpfeuffer
 * R: erroneous PMC support, missing ComExpress support, general maintenance
 * M: intermediate check-in during development
 *
 * Revision 2.5  2011/09/26 08:42:18  dpfeuffer
 * R:1. no MEN HW with PC104+ PCI (it is PCI-104)
 *   2. new bus interfaces required: StdPciE, CpciSer
 * M:1. Pc104PlusPci replaced with Pci104
 *   2. added StdPciE, CpciSer
 *
 * Revision 2.4  2011/03/31 16:53:16  CRuff
 * R: 1. support of pci domains
 *    2. ModBbisSmbPciGenProperties now inherit ModBbisPciMmodProperties
 * M: 1a) class CpuDevice constructor: initialize _hasPciDomains flag with false
 *    1c) override method addChild() for PciBusInterface: hand over the domain
 *        of the bus interface to added BBIS devices and grandchild interfaces
 *        (used for cpu extension boards with pci bus interfaces)
 *    2. changed pci device number access from pciDevNo to pciDeviceNo
 *
 * Revision 2.3  2010/07/05 17:57:37  CRuff
 * R: implementation of PCI Domains
 * M: handle new parameter pciDomain in PciBusInterface() constructors
 *
 * Revision 2.2  2010/06/02 16:59:23  CRuff
 * R: 1. support new ESM connectors
 * M: 1. added new ident strings for ESMexpress and ESMini to mmap
 *
 * Revision 2.1  2010/05/19 14:58:44  CRuff
 * R: 1. class name CpuSmbDevice is misleading
 *    2. members of CpuDeviceSmb are not initialized
 *    3. add functionality to activate onboard smb controllers for CpuDeviceSmb
 * M: 1. rename class CpuSmbDevice to CpuDeviceSmb
 *    2. initialize members in constructor
 *    3a) include header modbbissmbpcigen.h
 *    3b) implement method activateOnboardSmb and connect slot in createPropDlg
 *    3c) in BbisDevice::bbSlot, check if given child is really bbis slot (bbis
 *        can now have other children than bbis slots)
 *
 * Revision 2.0  2010/01/22 11:26:13  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.22  2009/12/07 09:57:40  CRuff
 * R: 1. naming map for BusInterface SMBus does not match specification
 *    2. differentiation between cpu devices with/without SMB controller needed
 * M: 1. rename BusInterface string from SMB to SMBus as specified in
 *       PackageDesc spec
 *    2. implemented new class CpuSmbDevice
 *
 * Revision 1.21  2009/09/03 10:43:18  CRuff
 * R: 1. preparation for SMB support
 *    2. allow to add native driver as a std sw module
 * M: 1a) added Smb Bus to BusIfTypeMap
 *    1b) added constructor for new class SMBusInterface
 *    2. added flag 'isNative' to signature of  makeStdDriverSwModule;
 *       default = false;
 *
 * Revision 1.20  2009/05/13 11:41:55  CRuff
 * R: 1. Copied CpuDevice objects lack information
 *    2. new device type needed to distinguish between MdisGeneric and MdisCham
 *       devices
 * M: 1a) added CpuDevice::copy() method
 *    2. handle new device type Device::MdisCham in makeStdDriverSwModule()
 *
 * Revision 1.19  2006/11/30 11:27:38  cs
 * added:
 *   + BusIfType: Xmc
 *   + BusIfTypeMap: Pmc, Xmc
 *   + HwComponent::insChild(), ::replChild() members
 * fixed:
 *   - BbisSlot::canConnect(): handle CHAMELEON devices differently
 *
 * Revision 1.18  2005/12/07 16:55:10  ub
 * cosmetics
 *
 * Revision 1.17  2005/03/09 13:48:28  ub
 * new bus type "ChameleonBus"
 *
 * Revision 1.16  2004/10/07 11:13:24  dpfeuffer
 * cosmetics
 *
 * Revision 1.15  2004/09/30 13:58:27  dpfeuffer
 * conflicts on merge solved
 *
 * Revision 1.14  2004/08/31 12:46:04  kp
 * bbisSlot:canConnect is no longer case sensitive when comparing instance/model names
 *
 * Revision 1.13  2004/07/30 11:45:12  ufranke
 * added
 *  - bus interface function isPci()
 *
 * Revision 1.12  2004/07/22 12:04:52  kp
 * small fix for VC6
 *
 * Revision 1.11  2004/07/21 15:00:33  ufranke
 * changed
 *  - currentCfg() by MAIN_GetCurrentCfg()
 *
 * Revision 1.10  2004/07/16 12:35:45  ufranke
 * added
 *  - cpuCoreGetDefaults()
 *
 * Revision 1.9  2004/06/25 13:41:40  ufranke
 * cosmetics
 *
 * Revision 1.8  2004/06/08 17:25:19  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.7  2004/04/13 11:00:37  dpfeuffer
 * Device::createPropDlg4Win(): Apply button removed
 *
 * Revision 1.6  2004/01/09 08:19:53  kp
 * removed "UNUSUABLE" handling
 * support BbSlotSpec
 *
 * Revision 1.5  2003/06/11 12:21:28  kp
 * intermediate checkin
 *
 * Revision 1.4  2003/06/05 12:00:57  dschmidt
 * intermediate checkin
 *
 * Revision 1.3  2003/06/05 09:33:22  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/06/03 10:16:06  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:34:44  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#include <qmessagebox.h>
#include <qmap.h>

#include <Q3MemArray>
#include <Q3PtrList>

#include "wizdebug.h"
#include "mdiswiz.h"
#include "hwcomponent.h"
#include "resourcemng.h"
#include "configuration.h"
#include "propertiesdlg.h"
#include "descriptor.h"
#include "targetos.h"
#include "swmodule.h"
#include "vmsgbox.h"
#include "modelfactory.h"
#include "util.h"

#include "modbbispcimmod.h"
#include "modbbissmbpcigen.h"
#include "esmcarrier.h"

#ifndef INT_MAX
	#define INT_MAX 2147483647 /* maximum (signed) int value */
#endif

// -----------------------------------------------------------------
// HwComponent

// statics
HwComponent::BusIfTypeMap HwComponent::busIfTypeMap;
HwComponent::MmodDataMap HwComponent::mmodDataMap;
HwComponent::MmodAddrMap HwComponent::mmodAddrMap;

//! creates an empty component
HwComponent::HwComponent()
{
    lstChildren.setAutoDelete( TRUE );
	myName = QString::null;
	description = QString::null;
	parent = 0;
	swapping = false;
}

//! destructs the component and all of it's children
HwComponent::~HwComponent()
{
	HwComponent *child;

	wDebug(("~HwComponent: %s", myName.latin1()));

	// tell all children they don't have parents anymore
	for( child=lstChildren.first(); child; child=lstChildren.next() )
		child->setParent( 0 );
}

//! get instance name of component
/*!
*/
QString
HwComponent::getInstName()
{
	return myName;
}

//! set instance name of component
void
HwComponent::setInstName( const QString &name )
{
	myName = name;
}

//! get the component's parent
/*!
\return parent component or 0 if it has no parent
*/
HwComponent *
HwComponent::getParent()
{
	return parent;
}

//! tell the component about its parents
void
HwComponent::setParent( HwComponent *_parent )
{
	parent = _parent;
}


//! add child to end of list of children
/*!
automatically set the child's parent entry
*/
void
HwComponent::addChild( HwComponent *newComp )
{
	lstChildren.append( newComp );
	newComp->setParent( this );
}

//! replace child in list of children
/*!
automatically set the child's parent entry
*/
void
HwComponent::replChild( uint i, HwComponent *newComp )
{
	lstChildren.replace( i, newComp );
	newComp->setParent( this );
}

//! insert child into list of children
/*!
automatically set the child's parent entry
*/
void
HwComponent::insChild( uint i, HwComponent *newComp )
{
	lstChildren.insert( i, newComp );
	newComp->setParent( this );
}

//! remove child from list of children
/*!
child is deleted automatically
\return true if child was in list
*/
bool
HwComponent::removeChild( HwComponent *comp )
{
	return lstChildren.remove( comp );
}

//! get first child of this component
/*!
Can be used to iterate through the list of children:
\code
for( child=comp->firstChild(); child; child=comp->nextChild() )
   ;
\endcode
\sa nextChild()
*/
HwComponent *
HwComponent::firstChild()
{
	return lstChildren.first();
}

//! get next child
/*!
\sa firstChild()
*/
HwComponent *
HwComponent::nextChild()
{
	return lstChildren.next();
}

//! get direct list access to children
/*!
\return a reference to the list of children
*/
Q3PtrList<HwComponent> &
HwComponent::getChildren()
{
	return lstChildren;
}

//! check if bus is swapped
/*!
\return true if data bus is swapped with respect to the CPU bus
i.e. indicates if we need to use the swapped driver variant
*/
bool
HwComponent::isEndianSwapping()
{
	if( parent ){
		if( swapping )
			return ! parent->isEndianSwapping();
		else
			return parent->isEndianSwapping();
	}
	else
		return swapping;
}

//! tell component if it swaps data bus
void
HwComponent::setEndianSwapping( bool isSwapping )
{
	swapping = isSwapping;
}

//! get short description of the component
QString &
HwComponent::getDescription()
{
	return description;
}

//! set short description of the component
void
HwComponent::setDescription( const QString &_description )
{
	description = _description;
}

//! get main properties in a string
/*!
\return a string that describes the main properties of that device

the return value is displayed in the "location" column of mdiswiz

to be overwritten by subclasses
*/
QString
HwComponent::getMainProperties()
{
	if( parent )
		return parent->getMainProperties();
	else
		return QString::null;
}

//! create descriptor of this device and all its children.
/*!
All entries will be created at the same level and will be added
to \a parentDesc children

HwComponent::createDesc() only calls the createDesc() function of its
children without creating a descriptor for its own object

Should be reimplemented in subclasses
*/
void
HwComponent::createDesc( DescriptorEntryDirectory *parentDesc )
{
	HwComponent *comp;

	for( comp=firstChild(); comp; comp=nextChild() )
		comp->createDesc( parentDesc );

}

//! add component's own descriptor entries
/*!
This is called by BbisDevice::createDesc() and MdisDevice::createDesc()
to fill device specific descriptor entries

Should be reimplemented in subclasses
*/
void
HwComponent::createSpecialDesc( DescriptorEntryDirectory *parentDesc )
{
	parentDesc = parentDesc;
}

//! find child by instance name (recursive)
/*!
\return ptr to hwComponent or 0 if not found
*/
HwComponent *
HwComponent::findChild( const QString &_instName )
{
	HwComponent *comp, *rv;

	if( getInstName() == _instName )
		return this;

	for( comp=firstChild(); comp; comp=nextChild() ){

		if( (rv = comp->findChild( _instName )) != 0 )
			return rv;
	}
	return 0;
}

//! find child by instance pointer (recursive)
/*!
\return ptr to hwComponent or 0 if not found
*/
HwComponent *
HwComponent::findChild( HwComponent *dstComp )
{
	HwComponent *comp, *rv;

	if( this == dstComp )
		return this;

	for( comp=firstChild(); comp; comp=nextChild() ){

		if( (rv = comp->findChild( dstComp )) != 0 )
			return rv;
	}
	return 0;
}

//! fills the strings for BusIfTypeMap
HwComponent::BusIfTypeMap::BusIfTypeMap()
{
	mmap[Vme] = "VME";
	mmap[StdPci] = "PCI";
	mmap[StdPciE] = "PCIe";
	mmap[Cpci] = "CompactPCI";
	mmap[CpciSer] = "CompactPCI Serial";
	mmap[Mmod] = "M-Module";
	mmap[PcMip] = "PC-MIP";
	mmap[Pmc] = "PMC";
	mmap[Xmc] = "XMC";
	mmap[LocalBus] = "Local Bus";
	mmap[PciOrCham] = "Mezzanine";
	mmap[Pci104] = "PCI-104";
	mmap[ChameleonBus] = "ChameleonBus";
	mmap[Smb] = "SMBus";
	mmap[ESMexpress] = "ESMexpress";
	mmap[ESMini] = "ESMini";
	mmap[ComExpr] = "COM Express";
	mmap[InvalidBusIf] = "Invalid BusIf";
}

//! fills the strings for MmodDataMap
HwComponent::MmodDataMap::MmodDataMap()
{
	mmap[HwComponent::MmodDInvalid] = "Invalid";
	mmap[HwComponent::MmodD16] = "D16";
	mmap[HwComponent::MmodD32] = "D32";
}

//! fills the strings for MmodAddrMap
HwComponent::MmodAddrMap::MmodAddrMap()
{
	mmap[HwComponent::MmodAInvalid] = "Invalid";
	mmap[HwComponent::MmodA08] = "A08";
	mmap[HwComponent::MmodA24] = "A24";
}



// -----------------------------------------------------------------
// DebugProperties

// statics
DebugProperties::ModuleMap DebugProperties::moduleMap;
DebugProperties::ModuleDescMap DebugProperties::moduleDescMap;

//! constructs a DebugProperties object
/*! all debug settings are set to "errors only" and module mask is cleared  */
DebugProperties::DebugProperties()
{
	modules = 0;

	for( int i=0; i<_Max; i++ )
		dbgSetting[i] = Int|Normal|Error;
}

//! set the used modules whose debug levels can be controlled
/*!
  \param modMask bit ORed value of Module flags, e.g.
  (1<<DebugProperties::Driver) | (1<<DebugProperties::Oss)
*/
void
DebugProperties::setModules( ulong modMask )
{
	modules = modMask;
}

//! test wether debug module is used by that device
bool
DebugProperties::hasModule( Module mod )
{
	return ( modules & (1L<<mod));
}

//! set the debug level for a specific module
void
DebugProperties::setModuleLevel( Module mod, ulong level )
{
	if( mod < _Max )
		dbgSetting[mod] = level;
}

//! get debug level for a specific module
ulong
DebugProperties::moduleLevel( Module mod )
{
	if( mod < _Max )
		return dbgSetting[mod];
	else
		return 0;
}

//! fills the strings for ModuleMap (values displayed to user)
DebugProperties::ModuleMap::ModuleMap()
{
	mmap[Driver] 		= "Driver";
	mmap[BbisKernel] 	= "BBIS Kernel";
	mmap[MdisKernel]	= "MDIS Kernel";
	mmap[Oss]			= "OSS Library";
	mmap[Desc]			= "DESC Library";
	mmap[Mbuf]			= "MBUF Library";
	mmap[Invalid]		= "Invalid";
}

//! fills the strings for ModuleDescMap (as used for descriptor keys)
DebugProperties::ModuleDescMap::ModuleDescMap()
{
	QString com = "DEBUG_LEVEL";

	mmap[Driver]		= com;
	mmap[BbisKernel]	= com + "_BK";
	mmap[MdisKernel]	= com + "_MK";
	mmap[Oss]			= com + "_OSS";
	mmap[Desc]			= com + "_DESC";
	mmap[Mbuf]			= com + "_MBUF";
}

// -----------------------------------------------------------------
// Device

//! constructs a device
/*!
creates a device with no children and no SW modules.

Default DeviceProperties are created, however subclasses that wish to
use their own properties must delete these default properties and
create their own.

vendorName is set to "MEN" by default.

 \param _hwName set as the Device's hwName \b and as default model name
 \param _driverName name of driver
 \param isRemoveable flags if device is removeable
*/
Device::Device( const QString &_hwName, const QString &_driverName,
				bool isRemoveable )
{
	hwName = modelName = _hwName;
	driverName = _driverName;
	vendorName = "MEN";
	removeable = isRemoveable;
	resourcesAllocated = false;
	prop = createProperties();
	lstSwModules = 0;
	_autoId = 0;
	ifType = InvalidBusIf;
	instNameChangeAllowed = true;
}

//! destroy device instance
Device::~Device()
{
	freeResources();
	if( prop ){
		delete prop;
	}
	if( lstSwModules )
		delete lstSwModules;

	if( _autoId ){
		if( --_autoId == 0 )
			delete _autoId;
	}
}

//! copy most important attributes of \a org to our object
void
Device::copy( const Device *org )
{
	modelName = org->modelName;
	driverName = org->driverName;
	vendorName = org->vendorName;
	hwName = org->hwName;
	removeable = org->removeable;
	instNameChangeAllowed = org->instNameChangeAllowed;
	ifType = org->ifType;

	description = org->description;

	if( (_autoId = org->_autoId) )
		_autoId->refCount++;	// reference only!

	if( org->lstSwModules ){
		SwModuleList *lstMods = new SwModuleList;

		// copy org. list to new list
		lstMods->add(org->lstSwModules);
		lstSwModules = lstMods;
	}
}


//! check if device can be removed
/*!
\return true if device can be removed (i.e. is not permanently soldered)
*/
bool
Device::isRemoveable()
{
	return removeable;
}

//! set instance name of component
/*!
sets both the member variable myName and deviceName of DeviceProperties
*/
void
Device::setInstName( const QString &name )
{
	myName = name; // keep upper/lower case
	if( prop )
		prop->deviceName = myName;
}

//! get the driver name
/*!  \return If \a fullName and \a withSw is true, the (OS-specific)
full filename of the driver, e.g. "men_ll_m35_sw" (Linux) or
"ll_m35_sw_dbg" (OS-9) \n
If \a fullName is false, the basename of the driver with possible
swapped variant.("m35_sw").  If \a withSw is also false, just the
plain name ("m35")

can be overwritten by subclasses
*/
QString
Device::getDriverName( bool fullName, bool withSw)
{
	QString rv;

	rv = driverName.upper();
	if( rv == QString::null )
		return rv;

	if( withSw )
		if( isEndianSwapping() )
			rv += "_SW";

	if( fullName )
		rv = MAIN_GetCurrentCfg()->targetOs()->driverName( rv, this );

	return rv;
}

//! set the driver name
void
Device::setDriverName( const QString &_driverName )
{
	driverName = _driverName;
}

//! get devicename template string
/*!
\return e.g. "a201" or "m65" to build the default device names

the default implementation just returns the plain driver name, i.e. calls
getDriverName()

can be overwritten by subclasses
*/
QString
Device::getDeviceNameTpl()
{
	return getDriverName(false);
}

//! get the model name
/*!
Model name is defined as hardware name and possible driver variant
\return \a modelName that has been passed to the constuctor
*/
QString
Device::getModelName()
{
	return modelName;
}

//! get concatenated vendor + model name
QString
Device::getVendorModelName()
{
	if( !vendorName.isEmpty() )
		return vendorName + " " + modelName;
	else
		return modelName;
}

//! get concatenated vendor + hardware name
QString
Device::getVendorHwName()
{
	if( !vendorName.isEmpty() )
		return vendorName + " " + hwName;
	else
		return hwName;
}

//! get auto ID info of device
/*
  \return pointer to the devices auto-id info or 0 if device has no auto-id
  Returned auto-id is owned by Device. Do not delete!
*/
DeviceAutoId *
Device::autoId()
{
	return _autoId;
}

//! set auto ID info of device
void
Device::setAutoId( DeviceAutoId *id )
{
	_autoId = id;
}

//! create the device's properties dialog
/*!
creates a tabbed dialog without actually showing it.

subclasses should reimplement this method with their own implementation
by adding special tabs.

\return a DevPropertiesDlg instance
*/
DevPropertiesDlg *
Device::createPropDlg( QWidget *parent )
{
	DevPropertiesDlg *dp;

	dp = new DevPropertiesDlg( parent, this );
	dp->setButtons();

	wDebug(("Device::createPropDlg dp=%p", dp ));
	return dp;
}

//! create the device's properties dialog for windows
/*!
creates a tabbed dialog without actually showing it.
same as createPropDlg() but suppresses the creation of the general tab
and uses no Apply button.

subclasses should reimplement this method with their own implementation
by adding special tabs.

\return a DevPropertiesDlg instance
*/
DevPropertiesDlg *
Device::createPropDlg4Win( QWidget *parent )
{
	DevPropertiesDlg *dp;

	dp = new DevPropertiesDlg( parent, this, FALSE );
	dp->setOkButton();
	dp->setCancelButton();

	wDebug(("Device::createPropDlg dp=%p", dp ));
	return dp;
}

//! create device properties object
/*!
subclasses should reimplement this function and create their own properties
class
\return a new object derived from DeviceProperities
*/
DeviceProperties *
Device::createProperties()
{
	return new DeviceProperties();
}

//! copy device properties
/*!
copy the properties contained in object \a from to the empty object \a to

subclasses implementing their own properties must reimplement this method
*/
void
Device::copyProperties(
	const DeviceProperties *from,
	DeviceProperties *to )
{
	*to = *from;
}


//! show the properties dialog
/*!
\return true if properties have been modified (or more exactly: if user pressed
Ok or has pressed Apply button in the middle of the dialog)
*/
bool
Device::propertiesDialog( QWidget *parent )
{
	QString errMsg;
	Device *thisDev = this;
	int rv;
	bool status;
	
	dialog = createPropDlg( parent );

	wDebug(("Device::propertiesDialog %s", (const char *)getInstName()));

	if( dialog == 0 ){
		wDebug(("Device::propertiesDialog: no dialog!"));
		return false;
	}
	propertiesDlgApplied = false;

	Q_CHECK_PTR( prop );

   	orgProp = createProperties(); 	// create temporary properties
	copyProperties( prop, orgProp );
   	modProp = createProperties(); 	// create temporary properties
	copyProperties( prop, modProp );
	wDebug(("prop=%p modProp=%p orgProp=%p\n", prop, modProp, orgProp ));

	dialog->setProperties( modProp ); 	// pass prop. to dialog

	connect( dialog, SIGNAL(apply()), thisDev,
			 SLOT(slotPropertiesApply()));

	rv = dialog->exec();			// show dialog
	wDebug(("Dialog exec: %d", rv ));

	// restore original properties if canceled
	status = propertiesDlgApplied;

	if( rv == DevPropertiesDlg::Cancel ){
		copyProperties( orgProp, modProp );
		slotPropertiesApply();
	}
	
	delete dialog;
	dialog = 0;
	delete orgProp;
	orgProp = 0;
	delete modProp;
	modProp = 0;

	return status;
}

//! called when Ok or Apply button pressed in properties dialog
void
Device::slotPropertiesApply()
{
	QString errMsg;

	// free resources currently contained in prop
	freeResources();
	propertiesDlgApplied = true;

	if( applyProperties( modProp, errMsg ) != ApplyPropFail ){
		if( !errMsg.isEmpty() ){
			VMsgBox::informationOk(
				MAIN_GetCurrentCfg()->msgBoxCaption(),
				"Warnings while allocating resources",
				errMsg,
				dialog );
		}
		if( dialog )
			dialog->setProperties( modProp ); 	// update dialog widgets

		// copy to original parameters, so that cancel will undo
		// only changes with respect to last apply
		copyProperties( prop, orgProp );
	}
	else {
		VMsgBox::criticalOk(
			MAIN_GetCurrentCfg()->msgBoxCaption(),
			"Fatal errors while allocating resources",
			errMsg,
			dialog );

		// device unusuable???
	}
}


//! register resources contained in \a newProp to the resource/name managers
/*!
If \a newProp is passed as 0, the device's current properties will be used

If alternate resources allocated, \a newProp is modified

To be overwritten by subclasses

Subclasses must call Device::applyProperties() as their last step

old resources have to be released using freeResources() before calling
applyProperties
\return Device::ApplyPropOk if all resources have been assigned as requested\n
        Device::ApplyPropAlternate if alternate resources assigned\n
        Device::ApplyPropFailed if no free resources

\a errMsg will contain a newline seperated string with warning and error
messages

Current device properties (prop) will be updated

Device::applyProperties() allocates the device name only
*/
Device::ApplyResult
Device::applyProperties( DeviceProperties *newProp, QString &errMsg )
{
	QString msg;
	ApplyResult ar;

	if( newProp == 0 )
		newProp = prop;
		
	// allocate name
	if( MAIN_GetCurrentCfg()->nameMng.isAvail( newProp->deviceName ) == false ){
		// name not available
		msg.sprintf("Cannot use deviceName %s\n",
					(const char *)newProp->deviceName);
		errMsg += msg;
		newProp->deviceName = MAIN_GetCurrentCfg()->nameMng.suggest( getDeviceNameTpl());

		msg.sprintf("Using default name %s\n",
					(const char *)newProp->deviceName );
		errMsg += msg;
		ar = ApplyPropAlternate;
	}
	else
		ar = ApplyPropOk;

	MAIN_GetCurrentCfg()->nameMng.alloc( newProp->deviceName, this );
	setInstName( newProp->deviceName );

	// update current properties
	if( newProp != prop )
	    copyProperties( newProp, prop );

	resourcesAllocated = true;

	wDebug(("Device:applyProperties::resources allocated"));
	return ar;
}

//! register resources contained in \a devDesc to the resource/name managers
/*!
same purpose as function above, but a descriptor is passed.

This function shall extract the device parameters from the descriptor
being passed, and allocate the resources.

This function is typically called from Configuration::load(). All entries
handled by configuration::load() are already removed from \a devDesc

In the same way, subclasses should call Device::applyProperties() at the
end, after removing all descriptor entries handled by the subclass.

This routine will also parse the DEBUG_LEVEL* settings and will set
the debug properties accordingly.

\a errMsg will contain warnings if an unknown descriptor entry is detected

see Device::ApplyResult for return values
*/
Device::ApplyResult
Device::applyProperties( DescriptorEntryDirectory *devDesc, QString &errMsg )
{
	DescriptorEntry *e, *eNext;

	if( !devDesc ) return ApplyPropFail;

	// set my device name
	setInstName( devDesc->getKey() );

	// scan for debug settings
	eNext=devDesc->firstChild();

	while( (e=eNext) ){
		DebugProperties::Module module;
		DescriptorEntryUint32 *eUint32;
		bool entryHandled = false;

		QString key = e->path().section("/",1);

		// check if it's a known debug setting key
		if( (module = prop->dbgSettings.moduleDescMap.strToKey( key )) !=
			DebugProperties::Invalid ){

			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			prop->dbgSettings.setModuleLevel( module, eUint32->getValue() );
			entryHandled = true;
		}

		eNext=e->nextSibling();
		if( entryHandled )
			e->getParent()->removeChild(e);
	}

	// report unknown descriptor entries to user, but don't treat this as
	// a fatal error

	for( e=devDesc->firstChild(); e; e=e->nextSibling() ){
		if( e->getType() != DescriptorEntry::Directory ){
			QString key = e->path().section("/",1);
			errMsg += QString("unknown descriptor entry %1\n").arg(key);
			break; // only report one descriptor entry
		}
	}

	return applyProperties( (DeviceProperties *)0, errMsg );
}

//! undo applyProperties(). i.e. unregister resources
/*!
subclasses must call Device::freeResources as their last step
*/
void
Device::freeResources()
{
	wDebug(("Device::freeResources %s", myName.latin1()));
	// free device name
	if( MAIN_GetCurrentCfg() )
		MAIN_GetCurrentCfg()->nameMng.freeAllOfOwner( this );
	resourcesAllocated = false;

    wDebug(( "Device::freeResources() successful" ));
}

//! create SwModule for device driver
/*!
creates a new SwModule object for the device driver of that device,
based on the parameters passed.

\a baseDir is the base directory of the driver tree relative to the OS specific
base directory for MDIS LL/BB/CB drivers; e.g. "M033"

\a variant, if non-zero specifies a driver variant and is appended to
the filename of the driver makefile; e.g. driver_<variant>.mak. \a
variant is automatically converted to lower case.

If the device swaps the byte order (isEndianSwapping() returns true), it also
makes a makefile name like \c driver_<variant>_sw.mak. This can be suppressed
by \a ignoreSwapping.

The description is set to a generic description e.g.  "Low Level
driver for \em modelName", unless \a descriptionOverride is specified

\return a new SwModule object
*/
SwModule *
Device::makeStdDriverSwModule(
	const QString &baseDir,
	const QString &variant,
	bool ignoreSwapping,
	const QString &descriptionOverride,
	bool isNative)
{
	int modType = SwModuleTypes::Other;
	QString makefPath, drvPrefix;
	QString description;

	switch( type() ){
	case Mdis:
	case MdisCham:
		modType = SwModuleTypes::LlDriver; drvPrefix = "Low Level";
		break;
	case Bbis:
		modType = SwModuleTypes::BbDriver; drvPrefix = "BBIS";
		break;
	case Cbis:
		modType = SwModuleTypes::CbDriver; drvPrefix = "CBIS";
		break;
	}
	
	QString comNatPath;
	if (isNative) {
		comNatPath = "NATIVE";
	} else {
		comNatPath = "COM";
	}
	
	makefPath = QString("%1/DRIVER/%2/driver").arg(baseDir).arg(comNatPath);

	if( variant != QString::null && variant != "" )
		makefPath += QString("_%1").arg(variant.lower());

	if( !ignoreSwapping && isEndianSwapping())
		makefPath += QString("_sw");

	makefPath += ".mak";

	description = drvPrefix + " driver for " + getModelName();
	if( !descriptionOverride.isEmpty() )
		description = descriptionOverride;

	return new SwModule( modType, getDriverName(), makefPath,
						 description);

}
//! create the debug descriptor entries for the device
/*!
  Should be called from the device's createDesc() method
*/
void
Device::createDebugDesc( DescriptorEntryDirectory *parentDesc )
{
	DescriptorFactory dFact;
	DebugProperties::Module mod;

	for( int module=DebugProperties::Driver; module<DebugProperties::_Max;
		 module++ ){

		mod = (DebugProperties::Module)module;

		if( prop->dbgSettings.hasModule( mod) ){
			parentDesc->addChild( dFact.create(
				DebugProperties::moduleDescMap.keyToStr( mod ),
				prop->dbgSettings.moduleLevel( mod)));
		}
	}
}


//! builds the graphical representation of a device
/*!
creates a ComponentViewItem for the device.

It also builds the graphical items for all known children
Must be reimplemented by subclasses.
*/
ComponentViewItem *
Device::createGui(MdiswizView *wizView,  Q3ListViewItem *parent)
{
	UTIL_UNREF_PARAM(wizView);
	UTIL_UNREF_PARAM(parent);

	return 0;
}

// -----------------------------------------------------------------
// CpuDevice

//! constructs a new CpuDevice
/*!
all parameters are passed unchanged to Device::Device()
*/
CpuDevice::CpuDevice(const QString &_hwName, const QString &_driverName,
					 bool isRemoveable) :
	Device( _hwName, _driverName, isRemoveable )
{
	setInstName( QString("cpu") );
	this->_hasPciDomains = false; // false by default
}

Device *CpuDevice::create( bool withSubDevs )
{
	CpuDevice *newDev;
	withSubDevs = withSubDevs;

	newDev = new CpuDevice( hwName, driverName, removeable );
	newDev->copy( this );
	return newDev;
}


//! get the CPU core default
/*!
	\return the core the first core in list or UnsetCpuCore
*/
CpuDevice::CpuCore
CpuDevice::getCpuCoreDefault( void )
{
	CpuCore core = UnsetCpuCore;

	if( lstCpuCores.count() > 0 )
		core = lstCpuCores.first();

	return core;
}


//! builds the graphical representation of the CPU device
/*!
creates a CpuDeviceViewItem for the CPU device. This will be the root
device of mdiswiz.

It also builds the graphical items for all known children (BBIS and MDIS devs)
*/
ComponentViewItem *
CpuDevice::createGui(MdiswizView *wizView,  Q3ListViewItem *parent)
{
	CpuDeviceViewItem *cv;
	Device *cDev;
	HwComponent *busIfHw, *devHw;

	wDebug(("*** CpuDevice::createGui FOR %s %s parent=%p", myName.latin1(),
            getVendorHwName().latin1(), parent));
	if( !parent )
		cv = new CpuDeviceViewItem( wizView, wizView->getListView(),
									getInstName() );
	else
		cv = new CpuDeviceViewItem( wizView, parent,
									getInstName() );

	WIZ_ASSERT(cv);

	// create GUI components for all BBIS children
	for( busIfHw=firstChild(); busIfHw; busIfHw=nextChild() ){
		
        wDebug(("   busIf: %s", busIfHw->getInstName().latin1() ));

		for( devHw=busIfHw->firstChild(); devHw; devHw=busIfHw->nextChild() ){
			WIZ_DYNAMIC_CAST( devHw, cDev, Device *);
            wDebug(("     child: %s", cDev->getInstName().latin1() ));
			cDev->createGui( wizView, cv );
		}
	}
	cv->setDevice( this );
	return cv;
}

//! create descriptor of this CPU device and all its children.
/*! see description of HwComponent::createDesc() */
void
CpuDevice::createDesc( DescriptorEntryDirectory *parentDesc )
{
	DescriptorEntryDirectory *myDesc =
		new DescriptorEntryDirectory( getInstName());
	DescriptorFactory dFact;
	HwComponent *parentComp;
	BusInterface *busIf;

	// create CPU device descriptor
	myDesc->addChild( dFact.create( dFact.keyStr(DescriptorFactory::DESC_TYPE),
									(ulong)0 ));
	myDesc->addChild( dFact.create( dFact.keyStr(DescriptorFactory::HW_TYPE),
									getDriverName() ));
	myDesc->addChild( dFact.create(
		dFact.keyStr(DescriptorFactory::_WIZ_MODEL),
		getModelName() ));

	if( getParent() ){
		// for CPU extensions
		WIZ_DYNAMIC_CAST( getParent(), busIf, BusInterface * );

		parentComp = busIf->getParent();
		WIZ_ASSERT( parentComp );

		myDesc->addChild( dFact.createWizBusIf( parentComp->getInstName(),
												busIf->getBusIfNum()));
	}

	// create debug setting entries
	createDebugDesc( myDesc );

	parentDesc->addChild( myDesc );

	// build desc of all children
	HwComponent::createDesc( parentDesc );
}


//! default function to create a single swModule (and no swapping variant)
SwModuleList *
CpuDevice::enumSwModules()
{
	if( lstSwModules == 0 ){
		lstSwModules = new SwModuleList;

		// always ignore byteswapping (to generate driver.mak)
		lstSwModules->add( makeStdDriverSwModule( driverName, "", true ));
	}
	return lstSwModules;
}

//! list SwModule for given external package
/*!
  Searches the G_mdisMods factory for the given \a modelName
  and provides the list of SwModules for that package.

  Used for e.g. VME driver of Linux
  To be called by the Cpu models enumSwModules() function.

*/
SwModuleList *
CpuDevice::enumSwModulesForExternalPackage( QString modelName )
{
	extern MdisModelFactory *G_mdisMods;
	Device *dev;



	if( (dev = G_mdisMods->find( modelName )) == 0 ){
		// if not found, append OS name
		modelName += "_" +
			OsFactory::typeToStr(MAIN_GetCurrentCfg()->targetOs()->osType() ).upper();
		dev = G_mdisMods->find( modelName );
	}

	wDebug(("enumSwModulesForExternalPackage: %s %x", modelName.latin1(), (int)dev));
	if( dev )
		return dev->enumSwModules();

	return 0;
}

//! copy most important attributes of \a org to our object
void
CpuDevice::copy( const CpuDevice *org )
{
	// copy the list of supported cpu cores
	if( org->lstCpuCores.size() != 0 ){
		for (int i=0; i<org->lstCpuCores.size(); i++)
		{
			lstCpuCores << *(org->lstCpuCores.at(i));
		}

	}
}

// -----------------------------------------------------------------
// CpuExtDevice

//! constructs a new CpuExtDevice
/*!
*/
CpuExtDevice::CpuExtDevice(const QString &_hwName, bool isRemoveable,
						   BusIfType _ifType) :
	CpuDevice( _hwName, _hwName, isRemoveable )
{
	ifType = _ifType;
}

//! default function to create an empty SwModuleList
SwModuleList *
CpuExtDevice::enumSwModules()
{
	return 0;
}


// -----------------------------------------------------------------
// CpuDeviceSmb

//! constructs a new CpuDeviceSmb
/*!
*/
CpuDeviceSmb::CpuDeviceSmb(const QString &_hwName, const QString &_driverName, bool isRemoveable, bool hasObSmb, int nrSmbBusses)
 : CpuDevice( _hwName, _driverName, isRemoveable )
{
	UTIL_UNREF_PARAM( _driverName );

	if( prop ) delete prop; // delete Device::prop
    prop = createProperties();
    setInstName("cpu");

    this->nrSmbBusses = nrSmbBusses;
    this->hasObSmb = hasObSmb;
    this->smbusIf = 0;
    this->obSmbActive = false;
}

DevPropertiesDlg*
CpuDeviceSmb::createPropDlg( QWidget *parent )
{
	wDebug(("CpuDeviceSmb::create prop dialog"));
	DevPropertiesDlg* dlg = Device::createPropDlg( parent );
	dlg->setProperties( prop );
	DevSmbTab* smbTab;
	MdiswizView* wizView;
	
	dlg->addTab( (smbTab = new DevSmbTab(false, parent, dlg)), "SMBus" );
	dlg->showPage( smbTab );
	
	// cast to MdiswizView
	WIZ_DYNAMIC_CAST( parent, wizView, MdiswizView* );
	
	// establish connection between dlg and hw models
	connect( smbTab, SIGNAL( activateOnboardSmb( bool, MdiswizView* )),	this, SLOT(activateOnboardSmb( bool, MdiswizView* )) );
	
	return dlg;
}

DeviceProperties*
CpuDeviceSmb::createProperties()
{
	wDebug(("CpuDeviceSmb::createProperties"));
	return new DevicePropertiesWithSmb();

}

void 
CpuDeviceSmb::copyProperties( 
	const DeviceProperties *from,
	DeviceProperties *to )
{
	const DevicePropertiesWithSmb *_from;
	DevicePropertiesWithSmb *_to;

	WIZ_DYNAMIC_CAST( to, _to, DevicePropertiesWithSmb * );
	WIZ_DYNAMIC_CAST( from, _from, const DevicePropertiesWithSmb * );

	*_to = *_from;
	wDebug(("DevicePropertiesWithSmb::copyProperties"));
}

void 
CpuDeviceSmb::activateOnboardSmb( bool active, MdiswizView *wizView )
{
	MdisWizListView *listView=0;
	ComponentViewItem *cvi=0;
	HwComponent* bbisDev=0;
	HwComponent* oldBbisDev=0;
	ModBbisSmbPciGen* smbBbis = 0;
	ModBbisSmbPciGenProperties* smbProp;
	ModBbisSmbPciGenProperties* cviSmbProp;
	int devNo=0, funcNo=0;
	int busPathLength =0;
	int numChildren=0;
	int i=0,j=0;
	QStringList childrenNamesList;

	if ( this->hasObSmb )
	{
		if ( active && (!this->obSmbActive) )
		{
			// add obSmbPciBBis
			this->obSmbActive = true;
			for (i=0; i< this->smbBbisList.size(); i++)
			{
				// first, remember all children before adding new one
				numChildren = wizView->activeItem()->childCount();
				childrenNamesList.clear();
				cvi = (ComponentViewItem*)wizView->activeItem()->firstChild();
				if (cvi)
					childrenNamesList.push_back(cvi->text(0));
				for (j=1; j<numChildren; j++)
				{

					cvi = (ComponentViewItem*)cvi->nextSibling();
					childrenNamesList.push_back(cvi->text(0));
				}
					
				// create device of same model
				wizView->activeItem()->attachDev( this->smbusIf, this->smbBbisList.at(i)->getModelName() );
				// has at least one child (just added by attach Dev)
				cvi = (ComponentViewItem*)wizView->activeItem()->firstChild();
				for (j=0; j<numChildren; j++)
				{
					wDebug(("text: %s", cvi->text(0).ascii()));
					if (childrenNamesList.contains(cvi->text(0))) {
						cvi = (ComponentViewItem*)cvi->nextSibling();
					}
					else {
						break; // name not found -> is our new child!
					}
				}
				
				// currently just 1 SMB controller (G2x CPUs) or 2 controller busses (Bx50x, Bx70x) used
				if (this->nrSmbBusses > 1)
				{
					wDebug((" adding 2nd bus:" ));
					// create device of same model
					wizView->activeItem()->attachDev( this->smbusIf, this->smbBbisList.at(i)->getModelName() );
					// has at least one child (just added by attach Dev)
					cvi = (ComponentViewItem*)wizView->activeItem()->firstChild();
					for (j=0; j<numChildren; j++)
					{
						wDebug(("text: %s", cvi->text(0).ascii()));
						if (childrenNamesList.contains(cvi->text(0)))
						{
							cvi = (ComponentViewItem*)cvi->nextSibling();
						}
						else {
							break; // name not found -> is our new child!
						}
					}
				}

				WIZ_ASSERT(cvi);
				WIZ_DYNAMIC_CAST(cvi->getDevice(), smbBbis, ModBbisSmbPciGen*);
				WIZ_DYNAMIC_CAST(this->smbBbisList.at(i)->getProp(), smbProp, ModBbisSmbPciGenProperties*);
				WIZ_DYNAMIC_CAST(smbBbis->getProp(), cviSmbProp, ModBbisSmbPciGenProperties*);

				// last level in bus path is the device number (smb controllers are non bridge devices)
				busPathLength = smbProp->pciBusPath.count();
				if (busPathLength > 0) {
					devNo = smbProp->pciBusPath.at(busPathLength-1);
					if (devNo > 0x1f) {
						funcNo = devNo >> 5;
						devNo &= 0x1f;
					}
					cviSmbProp->pciDevNo = devNo;
					cviSmbProp->pciFuncNo = funcNo;
			
					smbBbis->setPciBusPath(smbProp->pciBusPath);
					// last element was taken as dev no
					cviSmbProp->pciBusPath.resize( busPathLength-1 );
				}
			}
			
		}
		// "falling edge" -> SMB controller was active but shall be deleted
		else if ( !active && (this->obSmbActive) )
		{
			this->obSmbActive = false;
			WIZ_DYNAMIC_CAST(wizView->getListView(), listView, MdisWizListView*);
			
			// cleanup 
			bbisDev = this->smbusIf->firstChild();
				
			while (bbisDev)
			{
				cvi = listView->findItemByInstName( bbisDev->getInstName() );
				if (cvi) {
					delete cvi; // delete gui object
				}
				oldBbisDev = bbisDev; // remember
				bbisDev = this->smbusIf->nextChild();
				this->smbusIf->removeChild(oldBbisDev); // delete corresponding devices
			}
			
				
			wizView->slotCfgModified();
		}
	}
	wDebug(("leave"));
}

// -----------------------------------------------------------------
// BbisDevice

//! constructs a new BbisDevice (carrier board for MDIS devices)
/*!
all parameters are passed unchanged to Device::Device(), except \a ifType,
which must be set to the upper interface type of the board (bus interface)
*/
BbisDevice::BbisDevice( const QString &_hwName, const QString &_driverName,
						bool isRemoveable, BusIfType _ifType) :
	Device( _hwName, _driverName, isRemoveable )
{
	ifType = _ifType;
}

Device *BbisDevice::create( bool withSubDevs )
{
	BbisDevice *newDev;
	withSubDevs = withSubDevs;

	newDev = new BbisDevice( hwName, driverName, removeable, ifType );
	newDev->copy( this );
	return newDev;
}

//! default function to create a single swModule (and swapping variant)
SwModuleList *
BbisDevice::enumSwModules()
{
	if( lstSwModules == 0 ){
		lstSwModules = new SwModuleList;
		lstSwModules->add( makeStdDriverSwModule( driverName, QString::null ));
	}
	return lstSwModules;
}

//! get bbSlot component by slot number
/*!
\return pointer to the bbSlot or 0 if slot not existant
*/
BbisSlot *
BbisDevice::bbSlot( int slotNum )
{
	HwComponent *comp;
	BbisSlot *bbSlot;

	for( comp=firstChild(); comp; comp=nextChild() ){
		if (comp->getInstName().contains("BusIf"))
			continue; // this is no bbis slot! cannot cast
			
		WIZ_DYNAMIC_CAST( comp, bbSlot, BbisSlot *);

		if( bbSlot->getSlotNum() == slotNum )
			return bbSlot;
	}
	return 0;
}

//! builds the graphical representation of the BBIS device
/*!
creates a BbisDeviceViewItem for the BBIS device. This will be added to the
QListViewItem pointed to by \a parent.

It also builds the graphical items for all known children (MDIS devs)
*/
ComponentViewItem *
BbisDevice::createGui(MdiswizView *wizView, Q3ListViewItem *parent)
{
	BbisDeviceViewItem *bv;
	HwComponent *bbSlot, *devHw;
	Device *dev;

	bv = new BbisDeviceViewItem( wizView, parent, getInstName() );
	WIZ_ASSERT(bv);
	
	// create GUI components for all MDIS children
	for( bbSlot=firstChild(); bbSlot; bbSlot=nextChild() ){
		for( devHw=bbSlot->firstChild(); devHw; devHw=bbSlot->nextChild() ){
			WIZ_DYNAMIC_CAST( devHw, dev, Device *)
			dev->createGui( wizView, bv );
		}
	}
	bv->setDevice( this );
	return bv;
}

//! create descriptor of this BBIS device and all its children.
/*! see description of HwComponent::createDesc() */
void
BbisDevice::createDesc( DescriptorEntryDirectory *parentDesc )
{
	DescriptorEntryDirectory *myDesc =
		new DescriptorEntryDirectory( getInstName());

#ifndef WIN_MDISWIZ_LIB
	DescriptorFactory dFact;
	HwComponent *parentComp;
	BusInterface *busIf;
	WIZ_DYNAMIC_CAST( getParent(), busIf, BusInterface * );

	parentComp = busIf->getParent();
	WIZ_ASSERT( parentComp );

	// create BBIS device descriptor
	myDesc->addChild( dFact.create( dFact.keyStr(DescriptorFactory::DESC_TYPE),
									(ulong)2 ));
	myDesc->addChild( dFact.create( dFact.keyStr(DescriptorFactory::HW_TYPE),
									getDriverName() ));

	myDesc->addChild( dFact.create(
		dFact.keyStr(DescriptorFactory::_WIZ_MODEL),
		getModelName() ));

	myDesc->addChild( dFact.createWizBusIf( parentComp->getInstName(),
											busIf->getBusIfNum()));
#endif

	// fill in device specific entries
	createSpecialDesc( myDesc );
	createDebugDesc( myDesc );
	parentDesc->addChild( myDesc );

	// build desc of all children
	HwComponent::createDesc( parentDesc );
}

// -----------------------------------------------------------------
// BbSlotSpecList

//! add \a spec to the list
/*!
  no checks are performed to see if the element is already in the list
*/
void
BbSlotSpecList::add( BbSlotSpec *spec )
{
	append( spec );
}

//! add another BbSlotSpec list to the list
/*!
  This makes a copy of each element of \a lst and adds it to this object.
*/
void
BbSlotSpecList::add( BbSlotSpecList *lst )
{
	BbSlotSpec *spec, *newSpec;

	for( spec=lst->first(); spec; spec=lst->next() ){
		newSpec = new BbSlotSpec(*spec);
		add( newSpec );
	}
}


// -----------------------------------------------------------------
// MdisDevice

//! constructs a new MdisDevice
/*!
all parameters are passed unchanged to Device::Device(), except \a _ifType,
which must be set to the upper interface type of the device.

sets the data/address requirements for M-modules to D16/A08
*/
MdisDevice::MdisDevice( const QString &_hwName, const QString &_driverName,
						bool isRemoveable, BusIfType _ifType) :
	Device( _hwName, _driverName, isRemoveable )
{
	ifType = _ifType;
	mmodDReq = MmodD16;
	mmodAReq = MmodA08;
	_nativeDevice = false;
	bbSlotSpecList = 0;
}

//! copy most important attributes of \a org to our object
void
MdisDevice::copy( const MdisDevice *org )
{
	Device::copy( org );
	_nativeDevice = org->_nativeDevice;
	mmodDReq = org->mmodDReq;
	mmodAReq = org->mmodAReq;

	// copy bbSlotSpec list
	if( org->bbSlotSpecList ){
		if( bbSlotSpecList )
			delete bbSlotSpecList;
		bbSlotSpecList = new BbSlotSpecList;
		bbSlotSpecList->add( org->bbSlotSpecList );
	}
}


Device *MdisDevice::create( bool withSubDevs )
{
	MdisDevice *newDev;

	withSubDevs = withSubDevs;
	newDev = new MdisDevice( hwName, driverName, removeable, ifType );
	newDev->copy( this );
	return newDev;
}



//! get device interface type
HwComponent::BusIfType
MdisDevice::getDevIfType()
{
	return ifType;
}

//! builds the graphical representation of the MDIS device
/*!
creates a MdisDeviceViewItem for the MDIS device. This will be added to the
QListViewItem pointed to by \a parent.
*/
ComponentViewItem *
MdisDevice::createGui(MdiswizView *wizView, Q3ListViewItem *parent)
{
	wDebug(("MdisDevice::createGui"));
	MdisDeviceViewItem *mv;

	mv = new MdisDeviceViewItem( wizView, parent, getInstName() );
	WIZ_ASSERT(mv);
	mv->setDevice( this );
	return mv;
}

QString
MdisDevice::getMainProperties()
{
	BbisSlot *bbSlot;
	WIZ_DYNAMIC_CAST( getParent(), bbSlot, BbisSlot * );
	QString str;

	return bbSlot->getSlotName();
}

//! create descriptor of this BBIS device and all its children (if any?).
/*! see description of HwComponent::createDesc() */
void
MdisDevice::createDesc( DescriptorEntryDirectory *parentDesc )
{
	DescriptorEntryDirectory *myDesc =
		new DescriptorEntryDirectory( getInstName());

#ifndef WIN_MDISWIZ_LIB
	DescriptorFactory dFact;
	BbisSlot *bbSlot;
	WIZ_DYNAMIC_CAST( getParent(), bbSlot, BbisSlot * );

	// create MDIS device descriptor
	myDesc->addChild( dFact.create( dFact.keyStr(DescriptorFactory::DESC_TYPE),
									(ulong)1 ));
	myDesc->addChild( dFact.create( dFact.keyStr(DescriptorFactory::HW_TYPE),
									getDriverName() ));
	myDesc->addChild( dFact.create(
		dFact.keyStr(DescriptorFactory::_WIZ_MODEL),
		getModelName() ));

	myDesc->addChild( dFact.create(
		dFact.keyStr(DescriptorFactory::BOARD_NAME),
		bbSlot->getParent()->getInstName()));

	myDesc->addChild( dFact.create(
		dFact.keyStr(DescriptorFactory::DEVICE_SLOT),
		bbSlot->getSlotNum()));
#endif

	// fill in device specific entries
	createSpecialDesc( myDesc );
	createDebugDesc( myDesc );

	parentDesc->addChild( myDesc );

	// build desc of all children (if any)
	HwComponent::createDesc( parentDesc );
}


// -----------------------------------------------------------------
// BusInterface

//! create a new BusInterface
/*!
\param _ifType the interface type that the BusInterface provides
\param _isExpandeable flags if user can attach more BBIS devs to the IF
*/
BusInterface::BusInterface( BusIfType _ifType, bool _isExpandeable )
{
	ifType = _ifType;
	isPci  = false;
	expandeable = _isExpandeable;
}

//! check if BBIS device can be connected
/*!
checks if the bus interface types of the device matches the BusInterface
*/
bool
BusInterface::canConnect( Device *dev )
{
	bool can = FALSE;

	// Special handling for attachable BBIS devices with bus interface "PciOrCham",
	// e.g. MEZZ_CHAM/MEZZ_PCI, that can be connected to any PCI like interface
	// e.g. PC-MIP/PMC like mezzanine slot of A21c (this->ifType=PciOrCham) or
	// CPCI bus of F19 (this->ifType=Cpci).
	//
	// Note:
	// 1) this->ifType       : interface to connect to
	// 2) dev->getBusIfType(): interface from BBIS device that may be connected to 1)
	if( dev->getBusIfType() == PciOrCham){
		switch( ifType ){
			case StdPci:
			case StdPciE:
			case Cpci:
			case CpciSer:
			case PcMip:
			case Pmc:
			case Xmc:
			case ESMexpress:
			case ESMini:
			case PciOrCham:
				can = true;
				break;
			default:
				can = false;
		}
		wDebug(("BusInterface::canConnect: PciOrCham "
			"ifType = %d, can=%d",
			ifType, can));
	}
	else {
		can = dev->getBusIfType() == ifType;
	}

	return can;
}

//! check if user can attach more BBISes
bool
BusInterface::isExpandeable()
{
	return expandeable;
}

//! get the bus interface number (of the CPU)
/*!
\return index of that bus interface in the CPU's BusInterface list (0..n)
*/
int
BusInterface::getBusIfNum()
{
	return getParent()->getChildren().findRef( this );
}

QString
BusInterface::getMainProperties()
{
	return busIfTypeMap.keyToStr(ifType);
}

// -----------------------------------------------------------------
// VmeBusInterface

//! creates a new VmeBusInterface
/*!
creates resource managers for the VME address spaces A16, A24, A32 and
a ResourceMng for VME interrupt vectors. All resources are initially empty.
*/
VmeBusInterface::VmeBusInterface() : BusInterface( Vme, true )
{
	vmeA16Res = new ResourceMng( QString("VME A16 Mem"));
	vmeA24Res = new ResourceMng( QString("VME A24 Mem"));
	vmeA32Res = new ResourceMng( QString("VME A32 Mem"));
	vmeIrqVectorRes = new ResourceMng( QString("VME IRQ Vectors"));
}

//! destructs the VmeBusInterface
VmeBusInterface::~VmeBusInterface()
{
	wDebug(("VmeBusInterface::~VmeBusInterface"));
	delete vmeA16Res;
	delete vmeA24Res;
	delete vmeA32Res;
	delete vmeIrqVectorRes;
}

//! return mask of supported irq levels (IRQ1=bit1)
ulong
VmeBusInterface::supportedIrqLevels()
{
	return 0;	// should be overwritten by inherited classes
}

// -----------------------------------------------------------------
// SMBusInterface

//! creates a new SMBusInterface
/*!
creates a new interface to which different SMBus Controllers can be
connected. 
Controllers can be accessed via PCI, but no PCI bus path
will be set here (has to be configured for the controller)
As one FPGA can contain several controllers, no slot management
is implemented for this interface. 
*/
SMBusInterface::SMBusInterface() : BusInterface( LocalBus, true )
{
	isPci = true;
	ifType = Smb;
	setInstName( QString("SMB Interface") );
}

// -----------------------------------------------------------------
// PciBusInterface (used for both STD and CPCI bus interfaces)

void
PciBusInterface::init(
	int _minSlot, int _maxSlot, int _minSlotDevId,
	const Q3MemArray<uchar> *_basePciPath,
	const Q3MemArray<uchar> *_directIdTbl)
{
	pciSlots = new ResourceMng( QString("PCI Slots"));
	minSlot = _minSlot;
	maxSlot = _maxSlot;
	minSlotDevId = _minSlotDevId;
	if( _basePciPath )
		basePciPath.duplicate(*_basePciPath);
	else
		basePciPath.resize(0);

	if( _directIdTbl )
		directIdTbl.duplicate(*_directIdTbl);
	else
		directIdTbl.resize(0);

	if( minSlot >= 0 && (maxSlot>=minSlot))
		pciSlots->addRegion( minSlot, maxSlot-minSlot+1 );

	setIsPci( true );
}

//! creates a new PCI Bus Interface
/*!
 If a subclass wants to use PciBusInterface::pciBusPath() it should
 fill:
 \param _ifType bus type to create (Cpci, StdPci, Pci104)
 \param	_minSlot first slot number
 \param _maxSlot last slot number
 \param _minSlotDevId PCI device ID of \a minSlot \n
				  (device IDs of other slots decrease with each slot like \n
				   on CPCI)
 \param _basePciPath: PCI path to that bus
 \param isExpandable: Flags if BBISes can be attached to this interface

 If pciBusPath() is overwritten by the subclass, the above parameters
 will be ignored.

 The constructor will also create a ResourceMng to keep track of the available
 PCI slots.
*/
PciBusInterface::PciBusInterface(
	BusIfType _ifType,
	int _minSlot, int _maxSlot, int _minSlotDevId,
	const Q3MemArray<uchar> *_basePciPath,
	bool isExpandable,
	int pciDomain) :

	BusInterface( _ifType, isExpandable )
{
	pciDomainNo = pciDomain;
	pciDomainNoUnknown = false;
	init( _minSlot, _maxSlot, _minSlotDevId, _basePciPath, 0 );
}

//! creates a new PCI Bus Interface with direct IDSEL mapping table
/*!

 Does not use autonumbering as in the above constructor, but uses
 \a _directIdTbl to map each slot to a IDSEL/Device number.

 _directIdTbl[0] must correspond to \a _minSlot.

*/
PciBusInterface::PciBusInterface(
	BusIfType _ifType,
	int _minSlot, int _maxSlot,
	const Q3MemArray<uchar> *_basePciPath,
	const Q3MemArray<uchar> *_directIdTbl,
	bool isExpandable,
	int pciDomain ):
	BusInterface( _ifType, isExpandable )
{
	pciDomainNo = pciDomain;
	pciDomainNoUnknown = false;
	init( _minSlot, _maxSlot, -1, _basePciPath, _directIdTbl );
}


//! destructs the PCI Bus Interface
PciBusInterface::~PciBusInterface()
{
	delete pciSlots;
}

//! get PCI bus path
/*!
should return the PCI_BUS_PATH in \a pciPath of that interface as
defined by MDIS

if \a slotNo is not -1, then append the path for the PCI slot

\return false if \a slotNo out of range or when the bus path is unknown
*/
bool
PciBusInterface::pciBusPath( int slotNo, Q3MemArray<uchar> *pciPath )
{
	if( slotNo == -1 ){
		pciPath->duplicate( basePciPath );
	}
	else if( slotNo >= minSlot && slotNo <= maxSlot ){
		int sz = basePciPath.size();
		pciPath->duplicate( basePciPath );
		pciPath->resize( sz + 1);

		if( directIdTbl.size() > 0 ){
			// use user's mapping
			if( slotNo-minSlot > (int)directIdTbl.size() )
				goto _ERROR;
			pciPath->at(sz) = directIdTbl[slotNo-minSlot];
		}
		else {
			// use automatic mapping
			pciPath->at(sz) = minSlotDevId - (slotNo-minSlot);
		}
	}
	else
		goto _ERROR;

	return true;
_ERROR:
	pciPath->resize(0);
	return false;
}

//! report wether this bus interface knows it's PCI bus path
bool
PciBusInterface::hasPciBusPath()
{
	return minSlot != -1;
}

//! convert given PCI_BUS_PATH to slotNo
/*!
Used when recreating the slot number from the PCI_BUS_PATH of the descriptor
\return slot number or -1 if no matching bus path
*/
int
PciBusInterface::busPathToSlot( const Q3MemArray<uchar> &pciPath )
{
	Q3MemArray<uchar> tmp;
	int slotNo;

	for( slotNo=minSlot; slotNo <= maxSlot; slotNo++ ){

		pciBusPath( slotNo, &tmp);

		if( tmp == pciPath )
			return slotNo;
	}
	return -1;
}

//! add child to end of list of children
/*!
	automatically set the child's parent entry
	override to set pci domain in child
*/
void
PciBusInterface::addChild( HwComponent *newComp )
{
	BbisDevice* bbisDev;
	ModBbisPciMmodProperties* mmodProp;
	HwComponent* child;
	PciBusInterface* childInterface;
	int childCount = 0;

	if ( pciDomainNo != 0 ) {
		if ( newComp->inherits( "BbisDevice" ) ) {
			WIZ_DYNAMIC_CAST( newComp, bbisDev, BbisDevice *);
			WIZ_DYNAMIC_CAST( bbisDev->getProp(), mmodProp, ModBbisPciMmodProperties *);
			mmodProp->pciDomainNo = pciDomainNo; /* hand my domain over */
		} else if ( (newComp->getChildren().count() > 0 )) { /* set pci Domain in child busIfs */
			childCount = newComp->getChildren().count();
			for (int i=0; i<childCount; i++) {
				child = newComp->getChildren().at(i);
				childInterface = dynamic_cast<PciBusInterface*>(child);
				if (childInterface != 0) {
					childInterface->pciDomainNo = this->pciDomainNo; /* hand my domain over */
				}
				child = newComp->getChildren().next();
			}
		}
	}

	/* hand over pciDomainNoUnknown */
	if ( pciDomainNoUnknown ) {
		if ( (newComp->getChildren().count() > 0 )) {
			childCount = newComp->getChildren().count();
			for (int i=0; i<childCount; i++) {
				child = newComp->getChildren().at(i);
				childInterface = dynamic_cast<PciBusInterface*>(child);
				if (childInterface != 0) {
					childInterface->pciDomainNoUnknown = true; 
				}
				child = newComp->getChildren().next();
			}
		}
	}

	BusInterface::addChild( newComp );
}

//! return number of child slots
int
PciBusInterface::getNbrOfSlots()
{
	int	nbrOfSlots;

	if( minSlot >= 0 && (maxSlot >= minSlot) )
		nbrOfSlots = maxSlot - minSlot + 1;
	else
		nbrOfSlots = INT_MAX;

	wDebug(("PBI::getNbrOfSlots: nbrOfSlots=%d", nbrOfSlots));

	return nbrOfSlots;
}

//! return number of free child slots
int
PciBusInterface::getNbrOfFreeSlots()
{
	HwComponent*	child;
	int				nbrOfSlots, nbrOfChilds=0, freeSlots;

	nbrOfSlots = getNbrOfSlots();

	for( child=firstChild(); child; child=nextChild() )
		nbrOfChilds++;

	freeSlots = nbrOfSlots - nbrOfChilds;

	wDebug(("PBI::getNbrOfFreeSlots: freeSlots=%d", freeSlots));

	return freeSlots;
}

// -----------------------------------------------------------------
// BbisSlot

//! create a generic BBIS slot
/*!
\param _ifType bus interface type that this slot provides
\param _slotNum MDIS slot number of this slot
\param _bbSlotSpecRequired set to true to tell canConnect() to check
       only mdisDevs that have bbSlotSpecList
\param _slotName to set non-default slot name
*/
BbisSlot::BbisSlot( BusIfType _ifType, int _slotNum,
					bool _bbSlotSpecRequired, const QString &_slotName )
{
	init( _ifType, _slotNum, -1, MmodDInvalid, MmodAInvalid,
		  _bbSlotSpecRequired, _slotName);
}

//! create a BBIS slot for PCI carriers
/*!
\param _ifType bus interface type that this slot provides
\param _slotNum MDIS slot number of this slot
\param _pciDevNo PCI device number of that slot
\param _bbSlotSpecRequired set to true to tell canConnect() to check
       only mdisDevs that have bbSlotSpecList
\param _slotName to set non-default slot name
*/
BbisSlot::BbisSlot( BusIfType _ifType, int _slotNum, int _pciDevNo,
					bool _bbSlotSpecRequired, const QString &_slotName )
{
	init( _ifType, _slotNum, _pciDevNo, MmodDInvalid, MmodAInvalid,
		  _bbSlotSpecRequired, _slotName );
}

//! create an M-module BBIS slot
/*!
_ifType is set to Mmod
\param _slotNum MDIS slot number of this slot
\param dcap M-module slot max. data capabilities
\param acap M-module slot max. address capabilities
\param _bbSlotSpecRequired set to true to tell canConnect() to check
       only mdisDevs that have bbSlotSpecList
\param _slotName to set non-default slot name
*/
BbisSlot::BbisSlot( int _slotNum, MmodDataCaps dcap, MmodAddrCaps acap,
					bool _bbSlotSpecRequired, const QString &_slotName )
{
	init( Mmod, _slotNum, -1, dcap, acap,
		  _bbSlotSpecRequired, _slotName );
}

void
BbisSlot::init( BusIfType _ifType, int _slotNum, int _pciDevNo,
				MmodDataCaps dcap, MmodAddrCaps acap,
				bool _bbSlotSpecRequired, const QString &_slotName)
{
	ifType = _ifType;
	slotNum = _slotNum;
	pciDevNo = _pciDevNo;
	mmodDCap = dcap;
	mmodACap = acap;
	bbSlotSpecRequired = _bbSlotSpecRequired;
	slotName = _slotName;
}

//! check if MDIS device can be connected to the slot
/*!
  Checks wether this slot allows to connect devices only that have
  a BbSlotSpec list.

  If \a mdisDev has no BbSlotSpec list, returns false.

  If \a mdisDev has a BbSlotSpec list, check if any of BbSlotSpec objects
  match this slot and return true if so. If none matches, return false.

  If bbSlotSpecRequired is false and devices has no BbSlotSpecList, check
  if the busIfType of \a mdisDev matches this slot. For M-Module slots,
  check also the data/address capabilities.
*/
bool
BbisSlot::canConnect( MdisDevice *mdisDev )
{
	// if the device has a bbSlot spec list, check if BbSlot matches
	BbSlotSpecList *bbSlotLst;

	bbSlotLst = mdisDev->getBbSlotSpecList();

	if( bbSlotLst == 0 && bbSlotSpecRequired )
		return false;

	if( bbSlotLst != 0 ){

		BbisDevice *bbDev;
		WIZ_DYNAMIC_CAST( getParent(), bbDev, BbisDevice *);
		QString bbModel = bbDev->getModelName().lower();
		QString bbInstance = bbDev->getInstName().lower();

		wDebug(("BbisSlot::canConnect: model:%s inst:%s slot:%d",
				bbModel.latin1(), bbInstance.latin1(), slotNum));

		for( BbSlotSpec *spec = bbSlotLst->first();
			 spec; spec = bbSlotLst->next() ){

			wDebug((" %s %s %d",
					spec->_bbModel.latin1(),
					spec->_bbInstance.latin1(),
					spec->_slotNo ));

			if( ( (spec->_bbModel.isEmpty()) ||
				  (spec->_bbModel.lower() == bbModel) ) &&
				( (spec->_bbInstance.isEmpty()) ||
				  (spec->_bbInstance.lower() == bbInstance) ) &&
				( (spec->_slotNo == -1 ) ||
				  (spec->_slotNo == slotNum )) ){
				return true;
			}

			// special handling for chameleon devices (e.g. F206)
			// bbModel != device model here!!
			if( (!spec->_bbModel.isEmpty()) &&
				spec->_bbModel.contains( "CHAMELEON" ) &&
				(ifType == ChameleonBus) )
			{
				  return true;
			}

		}

		// not matched. abort here
		return false;
	}

	// Special handling for MDIS drivers that handles PCI devices e.g. PCI/PC-MIP/PMC.
	// The MDISWIZ shall determine that this drivers are not only suitable for the
	// specified interface, but general for PCI/PC-MIP/PMC like interfaces.
	//
	// Note:
	// 1) this->ifType           : interface to connect to
	// 2) mdisDev->getDevIfType(): interface from MDIS driver that may be connected to 1)

	switch( mdisDev->getDevIfType() ){
		case StdPci:	// e.g. 13z018-06, Watchdog on MEN EM05 CPU
		case PcMip:		// e.g. 13p013-06, 48 Bit TTL I/O
		case Pci104:	// e.g. 13pp04-06, PCI104 MVB Interface
		case Pmc:		
		case Xmc:		
			switch( ifType ){
				case StdPci:	// e.g. MEZZ_PCI slot
				case PcMip:		// e.g. D202 slot
				case Pci104:	// e.g. PP1
				case Pmc:		 
				case Xmc:		 
					wDebug(("BbisSlot::canConnect: PCI/PC-MIP/PMC/PCI-104 match"));
					return true;
					break;
			}
		break;
	}

	// no bbslot specifications found, just check busiftype
	if( mdisDev->getDevIfType() != ifType )
		return false;

	// for M-module slots, check the data/address capabilities
	if( ifType == Mmod ){

		if( (mmodDCap < mdisDev->mmodDataRequirements() ) ||
			(mmodACap < mdisDev->mmodAddrRequirements() ))
			return false;
	}

	return true;
}

//! get the BBIS slot number
int
BbisSlot::getSlotNum()
{
	return slotNum;
}

//! get the PCI device number of that BBIS slot
int
BbisSlot::getPciDevNo()
{
	return pciDevNo;
}


//! get slot name of this bbisSlot
/*!
  By default returns a string like "M-Module slot 4".
  The default can be  overwritten using setSlotName(), typically done
  by BBIS drivers for onboard devices.
*/
QString
BbisSlot::getSlotName()
{
	if( slotName == QString::null ){
		// make default name
		return QString("%1 slot %2").
			arg(HwComponent::busIfTypeMap.keyToStr(ifType)).
			arg(slotNum);
	}
	else
		return slotName;
}

//! overwrite default name returned by slotName()
void
BbisSlot::setSlotName( const QString &_slotName )
{
	slotName = _slotName;
}

//! set bbSlotSpecRequired
void
BbisSlot::setBbSlotSpecRequired( bool _bbSlotSpecRequired )
{
	bbSlotSpecRequired = _bbSlotSpecRequired;
}





