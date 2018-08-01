/***************************************************************************
 */
/*!       \file  modbbissmbisagen.cpp
 *       \brief  Generic class for SMB ISA BBIS
 *      \author	 Christine.Ruff@men.de
 *		  $Date: 2018/06/13 14:53:35 $
 *    $Revision: 2.2 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbissmbisagen.cpp,v $
 * Revision 2.2  2018/06/13 14:53:35  ts
 * R: increased nr. of SMB slots from 4 to 32
 *
 * Revision 2.1  2011/02/14 14:56:37  CRuff
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include "modbbissmbisagen.h"
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
// Class for Generic SMB ISA BBIS

//! Create a generic SMB ISA BBIS
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
ModBbisSmbIsaGen::ModBbisSmbIsaGen(const QString& _variant, bool _isIo )
: BbisDevice( QString("OBSMBISA"), QString("SMBISA"), true, HwComponent::LocalBus)
{
	wDebug(("ModBbisSmbIsaGen::ModBbisSmbIsaGen"));

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
        bbSlot->setSlotName( QString("SMBus slot %1").arg(i) );
        addChild( bbSlot );
    }
}

//! destructs ModBbisSmbPciGen
ModBbisSmbIsaGen::~ModBbisSmbIsaGen()
{
	wDebug(("~ModBbisSmbIsaGen()"));
	freeResources();
	if (prop)
		delete prop;
	prop = 0;
}

DeviceProperties*
ModBbisSmbIsaGen::createProperties( )
{
    ModBbisSmbIsaGenProperties *p = new ModBbisSmbIsaGenProperties();
    wDebug(("ModBbisSmbIsaGen::createProperties for %s", getModelName().ascii() ));

    p->dbgSettings.setModules(
        (1L<<DebugProperties::Driver) |
        (1L<<DebugProperties::BbisKernel) |
        (1L<<DebugProperties::Oss) |
        (1L<<DebugProperties::Desc));

    // set defaults
    p->deviceAddress = 0;
    p->smBusNo = 0;
    p->smbPollAlertFreq = 0;
    p->smbBusyWait = 100;

#ifdef WIN_MDISWIZ_LIB
	p->smbDevNameList.clear();
#endif

    return p;
}

//
// This is called by MdisDevice::createDesc
// to fill device specific descriptor entries
void
ModBbisSmbIsaGen::createSpecialDesc( DescriptorEntryDirectory *parentDesc )
{
	ModBbisSmbIsaGenProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisSmbIsaGenProperties * );
	DescriptorFactory dFact;
	int i=0;

	wDebug(("ModBbisSmbIsaGen::createSpecialDesc"));

	parentDesc->addChild( dFact.create("DEVICE_ADDR", _prop->deviceAddress));
	
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
ModBbisSmbIsaGen::applyProperties( DescriptorEntryDirectory *devDesc,
									QString &errMsg )
{
	DescriptorEntry *e, *eNext;
	DescriptorEntryUint32 *eUint32;
	ModBbisSmbIsaGenProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisSmbIsaGenProperties * );

	if( !devDesc ) return ApplyPropFail;

	// scan all descriptor entries
	eNext=devDesc->firstChild();

	wDebug(("ModBbisSmbIsaGen::applyProperties()"));

	while( (e=eNext) ){
		bool entryHandled = true;
		QString key = e->path().section("/",1);

		wDebug(("key=%s", (const char *)key ));

		if (key == "DEVICE_ADDR") {
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->deviceAddress = eUint32->getValue();
		}
		else if (key == "SMB_BUSNBR" ) {
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->smBusNo = eUint32->getValue();
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

Device::ApplyResult
ModBbisSmbIsaGen::applyProperties( DeviceProperties *newProp,
									QString &errMsg )
{
	QString msg;

	if( newProp == 0 )
		newProp = prop;

	ModBbisSmbIsaGenProperties *_prop;

	WIZ_DYNAMIC_CAST( newProp, _prop, ModBbisSmbIsaGenProperties * );

	ApplyResult ar;

	wDebug(("ModBbisSmbIsaGen::applyProperties2"));

	// allocate name
	msg = QString::null;
	ar = Device::applyProperties( _prop, msg );
	errMsg += msg;
	if( ar == ApplyPropFail )
		goto abort;

	wDebug(("ModBbisSmbIsaGen::applyProperties errMsg=%s",
		   (const char *)errMsg));

	return ar;

 abort:
	freeResources();
	wDebug(("ModBbisSmbIsaGen::applyProperties failed errMsg=%s",
		   (const char *)errMsg));
	return ApplyPropFail;
}

void
ModBbisSmbIsaGen::copyProperties(
	const DeviceProperties *from,
	DeviceProperties *to )
{
	const ModBbisSmbIsaGenProperties *_from;
	ModBbisSmbIsaGenProperties *_to;

	WIZ_DYNAMIC_CAST( to, _to, ModBbisSmbIsaGenProperties * );
	WIZ_DYNAMIC_CAST( from, _from, const ModBbisSmbIsaGenProperties * );

	*_to = *_from;
}

QString
ModBbisSmbIsaGen::getMainProperties()
{
	QString rv="";
	ModBbisSmbIsaGenProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisSmbIsaGenProperties * );
	
	rv = QString("Device Address = %1; SMB Bus Number = %2")
			.arg(_prop->deviceAddress).arg(_prop->smBusNo);

	return rv;
}

void
ModBbisSmbIsaGen::freeResources()
{
	BusInterface *busIf = (BusInterface *)getParent();
	if( busIf == 0 ) return;

	wDebug(("ModBbisSmbIsaGen::freeResources"));

	Device::freeResources();
}

SwModuleList*
ModBbisSmbIsaGen::enumSwModules()
{
	if( lstSwModules == 0 ){
		lstSwModules = new SwModuleList;

		/*if ( isIo ) {
			variant += QString("_IO");
		}*/
		// always ignore byteswapping (to generate driver.mak)
		lstSwModules->add( makeStdDriverSwModule("SMBPCI",
				QString("ISA_") + variant + (isIo ? QString("_IO") : QString("")), true, 0, false ));

	}
	return lstSwModules;
}

QString
ModBbisSmbIsaGen::getDriverName( bool fullName, bool withSw )
{
	UTIL_UNREF_PARAM( fullName );
	UTIL_UNREF_PARAM( withSw );
	QString rv = "SMBISA";

    if( !variant.isEmpty() && ( variant != "") )
        rv += "_" + variant;
    if ( isIo ) {
    	rv += QString("_IO");
    }
    return rv;
}


Device *
ModBbisSmbIsaGen::create( bool withSubDevs )
{
	withSubDevs = withSubDevs;
	if (MAIN_GetCurrentCfg()->targetOs()->osType() == OsFactory::Linux ) {
		/*wDebug(("ModBbisSmbPciGen::create ModBbisSmbPciLinux"));
		return new ModBbisSmbPciLinux( variant ); // special native version for linux
		*/
		return 0;
	} else {
		wDebug(("ModBbisSmbIsaGen::create ModBbisSmbPciGen"));
		return new ModBbisSmbIsaGen( variant, isIo );
	}
}


