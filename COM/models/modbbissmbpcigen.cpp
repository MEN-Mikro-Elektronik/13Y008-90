/***************************************************************************
 */
/*!       \file  modbbissmbpcigen.cpp
 *       \brief  Generic class for SMB PCI BBIS
 *      \author	 Christine.Ruff@men.de
 *		  $Date: 2018/06/13 14:56:07 $
 *    $Revision: 2.9 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbissmbpcigen.cpp,v $
 * Revision 2.9  2018/06/13 14:56:07  ts
 * R: increased nr. of SMB slots to 32
 *
 * Revision 2.8  2011/04/01 09:34:15  CRuff
 * R: 1. pci bus interfaces may have a pci domain
 *    2. pci domain is always stored in key PCI_DOMAIN_NUMBER, not in bus path
 *    3. ModBbisSmbPciGenProperties now inherits ModBbisPciMmodProperties
 * M: 1. in applyProperties(), takeover pci domain from bus interface
 *    2. removed code to handle PCI_BUS_PATH2
 *    3. changed property member names of pci device number and pci slot number
 *
 * Revision 2.7  2011/02/14 14:11:53  CRuff
 * R: 1. smbpci bbis for 16Z001 needed in IO variant
 *    2. SMBDEV_x keys were filled up if lower instances were not defined
 * M: 1. handle isIo flag in constructor
 *    1b) added parameter isIo to constructor, getDriverName() and
 *        enumSwModules()
 *    2. in createSpecialDesc: only write key SMBDEV_x if NAME is not empty
 *
 * Revision 2.6  2010/07/06 08:23:22  CRuff
 * R: implementation of PCI Domains
 * M: handle PCI_DOMAIN_NR / PCI_BUS_PATH2 in createSpecialDesc and
 *    applyProperties
 *
 * Revision 2.5  2010/06/16 14:37:33  CRuff
 * R: SMB_ALERT_POLL_FREQ was wrongly treated as boolean
 * M: treat SMB_ALERT_POLL_FREQ descriptor key as integer
 *
 * Revision 2.4  2010/06/16 14:14:34  CRuff
 * R: default values for SMB_BUSY_WAIT, SMB_ALERT_POLL_FREQ do not match default
 *    values in driver
 * M: set SMB_BUSY_WAIT default value to 100, SMB_ALERT_POLL_FREQ default to 0
 *
 * Revision 2.3  2010/05/31 09:12:10  CRuff
 * R: 1. For MDIS Wizard library, no object hierarchy is available
 * M: 1a) hold SMBDEV_x and SMBDEV_NAME information in properties instead
 *        of creating it from object hierarchy if WIN_MDISWIZ_LIB is defined
 *    1b) do not notify parent device if smb controllers have been activated
 *
 * Revision 2.2  2010/05/20 16:13:05  CRuff
 * R: Descriptor key SMB_BUSNBR not active, even if <value> is specified
 * M: removed explicit call setActive(false) for key
 *
 * Revision 2.1  2010/05/19 17:42:08  CRuff
 * R: 1. The Z001 variant of ModBbisSmbPciGen uses the pci bus path of the parent
 *       ModBbisCham
 *    2. continue implementation of ModBbisSmbPciGen
 * M: 1. added method implementation setPciBusPath() to copy the pci path from
 *       the parent device
 *    2a) use BusIfType LocalBus instead of Smb
 *    2b) changed initializations in createProperties()
 *    2c) in createSpecialDesc(): add PCI_BUS_SLOT if given by parent, and only
 *        write key PCI_BUS_PATH if path is not empty
 *    2d) update methods applyProperties, copyProperties to current
 *        implementation
 *    2e) for ModBbisSmbPciLinux, use driver name SMB2 instead of SMB2BB
 *    2f) for ModBbisSmbPciLinux, use ModBbisSmbPciGenProperties
 *
 * Revision 2.0  2010/01/22 11:29:29  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.2  2009/12/07 10:00:27  CRuff
 * R: Implementation not finished, but interrupted
 * M: Intermediate checkin
 *
 * Revision 1.1  2009/09/03 11:37:03  CRuff
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include "modbbissmbpcigen.h"
#include "modbbischam.h"
#include "modbbispcigenproptab.h"
#include "configuration.h"
#include "devgenericprop.h"
#include "wizexcept.h"
#include "wizdebug.h"
#include "osfactory.h"
#include "targetos.h"

// max. number of smb devices connected to a smb controller
static const uint G_numSlots = 32;

// -----------------------------------------------------------------
// Class for Generic SMB PCI BBIS

//! Create a generic SMB PCI BBIS
/*!
  The device will have the default properties set by createProperties().
  It will have no BBIS slots (must be created by constructor of subclasses
  by calling createSlot() for each provided slot).

  \param instName initial instance name of device
  \param hwName	Hardware name (e.g. "D201")
  \param driverName (e.g. "d201")
  \param ifType upper interface type (e.g. Cpci). If LocalBus is passed
         as \a ifType, no dialog tab will be added for PCI
  \todo instName really needed?
*/

//! Creates an D2 onboard SMBPCI BBIS device
ModBbisSmbPciGen::ModBbisSmbPciGen(const QString& _variant, bool _isIo )
: BbisDevice( QString("OBSMBPCI"), QString("SMBPCI"), true, HwComponent::LocalBus)
{
	wDebug(("ModBbisSmbPciGen::ModBbisSmbPciGen"));

	//setInstName( QString( "OBSMBPCI") );
	setDescription("Pseudo device for SMBus slots");
	variant = _variant;
	isIo = _isIo;
		
	wDebug(("variant: %s",variant.ascii()));
	setHwName( getDriverName( false, false ) );

	if( prop ) delete prop; // delete Device::prop
    prop = createProperties();

	// create BBIS slots
    for( uint i=0; i<G_numSlots; i++ ){
        BbisSlot *bbSlot = new BbisSlotSmb( Smb, i );
        bbSlot->setSlotName( QString("SMBus Slot %1").arg(i) );
        addChild( bbSlot );
    }
}

//! destructs ModBbisSmbPciGen
ModBbisSmbPciGen::~ModBbisSmbPciGen()
{
	wDebug(("~ModBbisSmbPciGen()"));
	freeResources();
	if (prop)
		delete prop;
	prop = 0;
}

DeviceProperties*
ModBbisSmbPciGen::createProperties( )
{
    ModBbisSmbPciGenProperties *p = new ModBbisSmbPciGenProperties();
    wDebug(("ModBbisSmbPciGen::createProperties for %s", getModelName().ascii() ));

    p->dbgSettings.setModules(
        (1L<<DebugProperties::Driver) |
        (1L<<DebugProperties::BbisKernel) |
        (1L<<DebugProperties::Oss) |
        (1L<<DebugProperties::Desc));

    // set defaults
    p->pciBusPath.resize(0);
    p->pciBusNo = 0;
    p->pciDevNo = 0;
    p->pciFuncNo = 0;
    p->slotNo = -1;
    p->pciDomainNo = 0;
    p->bar = 0;
    p->offs = 0;

    p->smBusNo = 0;
    p->smbPollAlertFreq = 0;
    p->smbBusyWait = 100;
    p->idCheck = false;

#ifdef WIN_MDISWIZ_LIB
	p->smbDevNameList.clear();
#endif

	wDebug(("leave"));
    return p;
}

void 
ModBbisSmbPciGen::setPciBusPath( Q3MemArray<uchar>& pciPath )
{
	wDebug(("ModBbisSmbPciGen::setPciBusPath"));
	ModBbisSmbPciGenProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisSmbPciGenProperties * );
	wDebug(("set path: %d",pciPath[0]));
	_prop->pciBusPath.duplicate(pciPath);
	wDebug(("result: %d",_prop->pciBusPath[0]));
}

//
// This is called by MdisDevice::createDesc
// to fill device specific descriptor entries
void
ModBbisSmbPciGen::createSpecialDesc( DescriptorEntryDirectory *parentDesc )
{
	ModBbisSmbPciGenProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisSmbPciGenProperties * );
	DescriptorFactory dFact;
	int i=0;

	wDebug(("ModBbisSmbPciGen::createSpecialDesc"));

	// only create PCI parameters if this is a pci smb controller
	if (variant != QString("NAT")) {
		
		if ( (_prop->pciBusPath.count() == 0) ||
			((_prop->pciBusPath.count() == 1) && (_prop->pciBusPath[0] == 0)) ) {
			// PCI_BUS_NUMBER
			parentDesc->addChild( dFact.create("PCI_BUS_NUMBER", _prop->pciBusNo));
		}
		else {
			// PCI_BUS_PATH
			parentDesc->addChild( dFact.create("PCI_BUS_PATH", _prop->pciBusPath));
		}
		if (_prop->pciDomainNo != 0 )
	        parentDesc->addChild( dFact.create("PCI_DOMAIN_NUMBER", _prop->pciDomainNo ));
	
		if (_prop->slotNo != -1)
			parentDesc->addChild( dFact.create("PCI_BUS_SLOT", _prop->slotNo));
	
		// PCI_DEVICE_NUMBER
		parentDesc->addChild( dFact.create("PCI_DEVICE_NUMBER", _prop->pciDevNo));
	
		// PCI_FUNCTION_NUMBER
		parentDesc->addChild( dFact.create("PCI_FUNCTION_NUMBER", _prop->pciFuncNo));
		if (variant.contains("Z001"))
		{
			// CTRLR_BAR
			if (_prop->bar != -1 )
			{
				parentDesc->addChild( dFact.create("CTRLR_BAR", _prop->bar));
				parentDesc->addChild( dFact.create("CTRLR_OFFSET", _prop->offs));
			}
		}
		// ID_CHECK
		if (_prop->idCheck)
			parentDesc->addChild( dFact.create("ID_CHECK", _prop->idCheck));
	}

	// SMB_BUSNBR
	parentDesc->addChild( dFact.create("SMB_BUSNBR", _prop->smBusNo));

#ifdef WIN_MDISWIZ_LIB
	for (i=0; i<_prop->smbDevNameList.size(); i++)
	{
		QString name = _prop->smbDevNameList.at(i);
		DescriptorEntryDirectory* grpDesc = NULL;

		if (name != "") {
			// SMBDEV
			grpDesc = new DescriptorEntryDirectory( QString("SMBDEV_%1").arg(i) );
			grpDesc->addChild( dFact.create( QString("NAME"), name ) );
			parentDesc->addChild(grpDesc);
		}
	}
	
#else
	// create list of child devices
	Q3PtrList<HwComponent> &children = getChildren();
	for ( Q3PtrListStdIterator<HwComponent> it = children.begin(); it != children.end(); ++it ) {
		HwComponent* child = *it;
		QString instName = "";
		if (child->firstChild()) {
			QString instName = child->firstChild()->getInstName();
			DescriptorEntryDirectory* grpDesc = NULL;

			// SMBDEV
			grpDesc = new DescriptorEntryDirectory( QString("SMBDEV_%1").arg(i) );
			grpDesc->addChild( dFact.create( QString("NAME"), instName ) );
			parentDesc->addChild(grpDesc);
		}
		i++;
	}
#endif

	// SMB_ALERT_POLL_FREQ
	if (_prop->smbPollAlertFreq != -1)
		parentDesc->addChild( dFact.create("SMB_ALERT_POLL_FREQ", _prop->smbPollAlertFreq));
		
	
	// SMB_BUSY_WAIT
	if (_prop->smbBusyWait != -1)
		parentDesc->addChild( dFact.create("SMB_BUSY_WAIT", _prop->smbBusyWait));
}

Device::ApplyResult
ModBbisSmbPciGen::applyProperties( DescriptorEntryDirectory *devDesc,
									QString &errMsg )
{
	DescriptorEntry *e, *eNext;
	DescriptorEntryUint32 *eUint32;
	DescriptorEntryBinary *eBinary;
	ModBbisSmbPciGenProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisSmbPciGenProperties * );

	if( !devDesc ) return ApplyPropFail;

	// scan all descriptor entries
	eNext=devDesc->firstChild();

	wDebug(("ModBbisSmbPciGen::applyProperties()"));

	while( (e=eNext) ){
		bool entryHandled = true;
		QString key = e->path().section("/",1);

		wDebug(("key=%s", (const char *)key ));

		if( key == "PCI_BUS_NUMBER" ){
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->pciBusNo = eUint32->getValue();
		}
		else if ( key == "PCI_DOMAIN_NUMBER" ) {
        	WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
        	_prop->pciDomainNo = eUint32->getValue();
        }
		else if ( key == "PCI_BUS_PATH" ) {
			WIZ_DYNAMIC_CAST( e, eBinary, DescriptorEntryBinary *);
			_prop->pciBusPath = eBinary->getValue();
		}
		else if( key == "PCI_DEVICE_NUMBER" ) {
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->pciDevNo = eUint32->getValue();
		}
		else if( key == "PCI_FUNCTION_NUMBER" ) {
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->pciFuncNo = eUint32->getValue();
		}
		else if ( key == "PCI_BUS_SLOT" ) {
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->slotNo = eUint32->getValue();
		}
		else if (key == "SMB_BUSNBR" ) {
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->smBusNo = eUint32->getValue();
		}
		else if (key == "CTRLR_BAR" ) {
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->bar = eUint32->getValue();
		}
		else if (key == "CTRLR_OFFSET" ) {
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->offs = eUint32->getValue();
		}
		else if (key == "ID_CHECK" ) {
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->idCheck = (bool)eUint32->getValue();
		}
		else if (key == "SMB_ALERT_POLL_FREQ" ) {
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->smbPollAlertFreq = eUint32->getValue();
		}
		else if (key == "SMB_BUSY_WAIT" ) {
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->smbBusyWait = eUint32->getValue();
		}
		else if( key.startsWith( "DEVICE_SLOT_" )) 
			; // ignore, but remove
		else if ( key.startsWith( "SMBDEV_" )) {
#ifdef WIN_MDISWIZ_LIB
			DescriptorEntryDirectory *eDir;
			DescriptorEntryString *eString;
			WIZ_DYNAMIC_CAST( e, eDir, DescriptorEntryDirectory* );
			if ( eDir->firstChild() ) {
				WIZ_DYNAMIC_CAST( eDir->firstChild(), eString, DescriptorEntryString* );
				QString name = eString->getValue();
			
				// get slot number
				bool convSuccess = false;
				QString slotStr = key.section('_', -1);
				uint slotNum = slotStr.toUInt(&convSuccess);
				if (convSuccess) {
					uint count = _prop->smbDevNameList.size();
					if (count <= slotNum) {
						for (uint i=count; i<=slotNum; i++) {
							_prop->smbDevNameList.append(""); // initialize
						}
					}
					_prop->smbDevNameList[slotNum] = name;
				}
			}
#else
			; // ignore, but remove
#endif
		}
		else
			entryHandled = false;

		eNext=e->nextSibling(false); // do not descend into directories

		// if entry has been handled, delete it
		if( entryHandled == true )
			e->getParent()->removeChild(e);

	}

#ifndef WIN_MDISWIZ_LIB	
	// notify parent cpu that onboard smb controller is active
	Device* parentDev;
	DevicePropertiesWithSmb* myprop;
	
	WIZ_DYNAMIC_CAST( getParent()->getParent(), parentDev, Device* );
	WIZ_DYNAMIC_CAST( parentDev->getProp(), myprop, DevicePropertiesWithSmb* );
		
	if (variant.contains("Z001"))
	{
		ModBbisCham* bbisDev;
		WIZ_DYNAMIC_CAST(parentDev, bbisDev, ModBbisCham*);
		
		myprop->chamSmbControllersActive = true;
		myprop->numChamSmbControllers++;
		bbisDev->chamSmbActive = true;
		
	} else {
		CpuDeviceSmb* cpuDev;
		WIZ_DYNAMIC_CAST(parentDev, cpuDev, CpuDeviceSmb*);
		
		myprop->obSmbControllerActive = true;
		cpuDev->obSmbActive = true;
	}
#endif

	return Device::applyProperties( devDesc, errMsg );
}

DevPropertiesDlg *
ModBbisSmbPciGen::createPropDlg( QWidget *parent )
{
	DevPropertiesDlg *dp;
	QWidget *defTab=0;

	dp = new DevPropertiesDlg( parent, this );

	dp->addTab( (defTab = new ModBbisSmbPciGenPropTab(parent, dp)),
				 QString("SMB PCI Settings"));

	dp->setButtons();
	if( defTab )
		dp->showPage( defTab );
	wDebug(("ModBbisSmbPciGen::createPropDlg dp=%p", dp ));
	return dp;

}

Device::ApplyResult
ModBbisSmbPciGen::applyProperties( DeviceProperties *newProp,
									QString &errMsg )
{
	QString msg;

	if( newProp == 0 )
		newProp = prop;

	ModBbisSmbPciGenProperties *_prop;
	PciBusInterface *busIf = (PciBusInterface *)getParent();

	WIZ_DYNAMIC_CAST( newProp, _prop, ModBbisSmbPciGenProperties * );

	ApplyResult ar;

	wDebug(("ModBbisSmbPciGen::applyProperties2"));

	// allocate name
	msg = QString::null;
	ar = Device::applyProperties( _prop, msg );
	errMsg += msg;
	if( ar == ApplyPropFail )
		goto abort;

	wDebug(("ModBbisSmbPciGen::applyProperties errMsg=%s",
		   (const char *)errMsg));

	if ( busIf ) {
		if ( busIf->getPciDomainNo() != 0 ) {
			_prop->pciDomainNo = busIf->getPciDomainNo();
		}
	}

	return ar;

 abort:
	freeResources();
	wDebug(("ModBbisSmbPciGen::applyProperties failed errMsg=%s",
		   (const char *)errMsg));
	return ApplyPropFail;
}

void
ModBbisSmbPciGen::copyProperties(
	const DeviceProperties *from,
	DeviceProperties *to )
{
	const ModBbisSmbPciGenProperties *_from;
	ModBbisSmbPciGenProperties *_to;

	WIZ_DYNAMIC_CAST( to, _to, ModBbisSmbPciGenProperties * );
	WIZ_DYNAMIC_CAST( from, _from, const ModBbisSmbPciGenProperties * );

	*_to = *_from;
	if (_from->pciBusPath.count() > 0)
	{
		wDebug(("copyProperties: bus path %d",_from->pciBusPath[0]));
		_to->pciBusPath.duplicate(_from->pciBusPath);
		wDebug(("copyProperties dest: bus path %d",_to->pciBusPath[0]));
	}
}

QString
ModBbisSmbPciGen::getMainProperties()
{
	QString rv="";
	ModBbisSmbPciGenProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisSmbPciGenProperties * );
	
	if (!_prop->pciBusPath.isEmpty())
		rv = QString("SMB bus nr.=%1").arg(_prop->smBusNo);

	return rv;
}

void
ModBbisSmbPciGen::freeResources()
{
	PciBusInterface *busIf = (PciBusInterface *)getParent();
	if( busIf == 0 ) return;

	wDebug(("ModBbisSmbPciGen::freeResources"));

	Device::freeResources();
}

SwModuleList*
ModBbisSmbPciGen::enumSwModules()
{
	if( lstSwModules == 0 ){
		lstSwModules = new SwModuleList;

		// always ignore byteswapping (to generate driver.mak)
		lstSwModules->add( makeStdDriverSwModule("SMBPCI",
				variant + (isIo ? QString("_IO") : QString("")), true, 0, false ));

	}
	return lstSwModules;
}

QString
ModBbisSmbPciGen::getDriverName( bool fullName, bool withSw )
{
	UTIL_UNREF_PARAM( fullName );
	UTIL_UNREF_PARAM( withSw );
	QString rv = "SMBPCI";

    if( !variant.isEmpty() && ( variant != "") )
        rv += "_" + variant;
    if ( isIo ) {
    	rv += QString("_IO");
    }
    return rv;
}


Device *
ModBbisSmbPciGen::create( bool withSubDevs )
{
	withSubDevs = withSubDevs;
	if (MAIN_GetCurrentCfg()->targetOs()->osType() == OsFactory::Linux ) {
		return new ModBbisSmbPciLinux( variant ); // special native version for linux
	} else {
		return new ModBbisSmbPciGen( variant, isIo );
	}
}

//! Creates an D2 onboard PC-MIP BBIS device
// IO Variant not needed (only for enumSwModules, which is overridden here)
ModBbisSmbPciLinux::ModBbisSmbPciLinux( const QString &variant )
: ModBbisSmbPciGen( variant )
{
	wDebug(("ModBbisSmbPciLinux::ModBbisSmbPciLinux"));
	setDriverName("SMB2");

	if( prop ) delete prop; // delete Device::prop prop =
	prop = createProperties();
    
	setDescription("Pseudo device for Linux SMBus slots");
	
	wDebug(("leave ModBbisSmbPciLinux::ModBbisSmbPciLinux"));
}

//! destructs ModBbisSmbPciLinux
ModBbisSmbPciLinux::~ModBbisSmbPciLinux()
{
	wDebug(("~ModBbisSmbPciLinux()"));
	freeResources();
	if (prop)
		delete prop;
	prop = 0;
}

DeviceProperties*
ModBbisSmbPciLinux::createProperties( )
{
    ModBbisSmbPciGenProperties *p = new ModBbisSmbPciGenProperties();
    wDebug(("ModBbisSmbPciLinux::createProperties for %s", getModelName().ascii() ));

    p->dbgSettings.setModules(
        (1L<<DebugProperties::Driver) |
        (1L<<DebugProperties::BbisKernel) |
        (1L<<DebugProperties::Oss) |
        (1L<<DebugProperties::Desc));

    // set defaults
    p->smBusNo = 0;
    
    wDebug(("leave ModBbisSmbPciLinux::createProperties" ));

    return p;
}


void
ModBbisSmbPciLinux::copyProperties(
	const DeviceProperties *from,
	DeviceProperties *to )
{
	const ModBbisSmbPciGenProperties *_from;
	ModBbisSmbPciGenProperties *_to;

	WIZ_DYNAMIC_CAST( to, _to, ModBbisSmbPciGenProperties * );
	WIZ_DYNAMIC_CAST( from, _from, const ModBbisSmbPciGenProperties * );

	*_to = *_from;
}

Device::ApplyResult
ModBbisSmbPciLinux::applyProperties( DeviceProperties *newProp,
									QString &errMsg )
{
	QString msg;

	if( newProp == 0 )
		newProp = prop;

	ModBbisSmbPciGenProperties *_prop;

	WIZ_DYNAMIC_CAST( newProp, _prop, ModBbisSmbPciGenProperties * );

	ApplyResult ar;

	wDebug(("ModBbisSmbPciLinux::applyProperties"));

	// allocate name
	msg = QString::null;
	ar = Device::applyProperties( _prop, msg );
	errMsg += msg;
	if( ar == ApplyPropFail )
		goto abort;

	wDebug(("ModBbisSmbPciLinux::applyProperties errMsg=%s",
		   (const char *)errMsg));

	return ar;

 abort:
	freeResources();
	wDebug(("ModBbisSmbPciLinux::applyProperties failed errMsg=%s",
		   (const char *)errMsg));
	return ApplyPropFail;
}

Device::ApplyResult
ModBbisSmbPciLinux::applyProperties( DescriptorEntryDirectory *devDesc,
									QString &errMsg )
{
	DescriptorEntry *e, *eNext;
	DescriptorEntryUint32 *eUint32;
	ModBbisSmbPciGenProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisSmbPciGenProperties * );

	if( !devDesc ) return ApplyPropFail;

	// scan all descriptor entries
	eNext=devDesc->firstChild();

	while( (e=eNext) ){
		bool entryHandled = true;
		QString key = e->path().section("/",1);

		wDebug(("key=%s", (const char *)key ));

		if (key == "SMB_BUSNBR" ) {
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->smBusNo = eUint32->getValue();
		}
		else if ( key.startsWith( "SMBDEV_" ))
			; // ignore, but remove
		else
			entryHandled = false;

		eNext=e->nextSibling(false); // do not descend into directories

		// if entry has been handled, delete it
		if( entryHandled == true )
			e->getParent()->removeChild(e);

	}

	// notify parent cpu that onboard smb controller is active
	// notify parent cpu that onboard smb controller is active
	Device* parentDev;
	DevicePropertiesWithSmb* prop;
	
	WIZ_DYNAMIC_CAST( getParent()->getParent(), parentDev, Device* );
	WIZ_DYNAMIC_CAST( parentDev->getProp(), prop, DevicePropertiesWithSmb* );
		
	if (variant.contains("Z001"))
	{
		ModBbisCham* bbisDev;
		WIZ_DYNAMIC_CAST(parentDev, bbisDev, ModBbisCham*);
		
		prop->chamSmbControllersActive = true;
		prop->numChamSmbControllers++;
		bbisDev->chamSmbActive = true;
		
	} else {
		CpuDeviceSmb* cpuDev;
		WIZ_DYNAMIC_CAST(parentDev, cpuDev, CpuDeviceSmb*);
		
		prop->obSmbControllerActive = true;
		cpuDev->obSmbActive = true;
	}
	
	return Device::applyProperties( devDesc, errMsg );
}

//
// This is called by MdisDevice::createDesc
// to fill device specific descriptor entries
void
ModBbisSmbPciLinux::createSpecialDesc( DescriptorEntryDirectory *parentDesc )
{

	ModBbisSmbPciGenProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisSmbPciGenProperties * );
	DescriptorFactory dFact;
	int i=0;

	wDebug(("ModBbisSmbPciLinux::createSpecialDesc"));

	// SMB_BUSNBR
	parentDesc->addChild( dFact.create("SMB_BUSNBR", _prop->smBusNo));

	// create list of child devices
	Q3PtrList<HwComponent> &children = getChildren();
	for ( Q3PtrListStdIterator<HwComponent> it = children.begin(); it != children.end(); ++it ) {
		HwComponent* child = *it;
		QString instName = "";
		if (child->firstChild()) {
			QString instName = child->firstChild()->getInstName();
			DescriptorEntryDirectory* grpDesc = NULL;

			// SMBDEV
			grpDesc = new DescriptorEntryDirectory( QString("SMBDEV_%1").arg(i) );
			grpDesc->addChild( dFact.create( QString("NAME"), instName ) );
			parentDesc->addChild(grpDesc);
		}
		i++;
	}
}

QString
ModBbisSmbPciLinux::getMainProperties()
{
	QString rv;
	ModBbisSmbPciGenProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisSmbPciGenProperties * );

	rv = QString("Local Bus");

	return rv;
}


DevPropertiesDlg *
ModBbisSmbPciLinux::createPropDlg( QWidget *parent )
{
	DevPropertiesDlg *dp;
	QWidget *defTab=0;

	dp = new DevPropertiesDlg( parent, this );

	dp->addTab( (defTab = new ModBbisSmbPciLinuxPropTab(parent, dp)),
				 QString("SMB PCI Settings"));

	dp->setButtons();
	if( defTab )
		dp->showPage( defTab );
	wDebug(("ModBbisSmbPciLinux::createPropDlg dp=%p", dp ));
	return dp;

}

void
ModBbisSmbPciLinux::freeResources()
{
	SMBusInterface *busIf = (SMBusInterface *)getParent();
	if( busIf == 0 ) return;

	wDebug(("ModBbisSmbPciLinux::freeResources"));

	Device::freeResources();
}

SwModuleList*
ModBbisSmbPciLinux::enumSwModules()
{
	QString baseDir;

	if( lstSwModules == 0 ){
		lstSwModules = new SwModuleList;
		lstSwModules->add( makeStdDriverSwModule("SMB2BB", "", true, 0, true ));
		
		wDebug(("myVariant: %s",variant.ascii()));
		if (variant.contains("Z001")) {
			QString makefpath = QString("DRIVERS/Z001_SMB/driver");

			if( isEndianSwapping())
				makefpath += QString("_sw");

			makefpath += ".mak";

			lstSwModules->add(new SwModule( SwModuleTypes::NativeDriver, QString("Z001_SMB"), makefpath,
						 QString("Linux native Z001 driver")));
		}
	}
	return lstSwModules;
}

QString
ModBbisSmbPciLinux::getDeviceNameTpl()
{
	QString devName;
	if ( this->modelName.contains("SMBPCI") )
		devName = "SMB2_BUS__idx0";
	else
		devName = "SMB2";

	return devName;
}

QString
ModBbisSmbPciLinux::getDriverName( bool fullName, bool withSw )
{
	UTIL_UNREF_PARAM( fullName );
	UTIL_UNREF_PARAM( withSw );
	QString rv = "SMB2";
	return rv;
}

// add child to list of children (and set child's parent)
void BbisSlotSmb::addChild( HwComponent *newComp )
{
	// get smb bus number from parent
	int smbBusNum;
	Device* parDev = (Device*) getParent();
	Device* newDev;

	ModBbisSmbPciGenProperties* parProp;
	WIZ_DYNAMIC_CAST( parDev->getProp(), parProp, ModBbisSmbPciGenProperties * );
	smbBusNum = parProp->smBusNo;

	// find smbus descriptor in child properties
	WIZ_DYNAMIC_CAST( newComp, newDev, Device * );
	ModDescriptorProperties* newDevProp;
	WIZ_DYNAMIC_CAST( newDev->getProp(), newDevProp, ModDescriptorProperties * );
	DescriptorEntryDirectory* desc = &newDevProp->desc;

	DescriptorEntryUint32* smBusDesc = (DescriptorEntryUint32*)desc->find( "SMB_BUSNBR", DescriptorEntry::Uint32 );
	smBusDesc->setValue( smbBusNum );

	BbisSlot::addChild( newComp );


}

