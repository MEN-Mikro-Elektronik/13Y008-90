/***************************************************************************
 */
/*!       \file  modbbispcigen.cpp 
 *       \brief  Implementation of generic PCI BBIS devices (F203..)
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2016/02/20 16:26:40 $
 *    $Revision: 2.10 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbispcigen.cpp,v $
 * Revision 2.10  2016/02/20 16:26:40  ts
 * R: BoxPCs based on SC24 need 2 SMBusses in MDIS wizard
 * M: 1. added member nrSmbBusses to class CpuDeviceSmb
 *    2. added parameter int nrSmbBusses to every constructor of CPUs with SMB
 *       controllers: 2 for SC24 (AMD based) Box PCs, 1 for every other CPU
 *
 * Revision 2.9  2014/08/22 15:56:21  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.8  2014/07/18 15:12:12  dpfeuffer
 * R: erroneous PMC support, missing ComExpress support, general maintenance
 * M: intermediate check-in during development
 *
 * Revision 2.7  2012/12/20 13:23:45  ts
 * R: 1. new cPCI card F223 was introduced
 *    2. for F223 enumerate of PCI bridges must stop after 1st bridge
 * M: 1. added model for F223 derived from ModBbisPciGen
 *    2. introduced new key SKIP_DEV_BRIDGE_CHECK
 *
 * Revision 2.6  2011/09/26 08:42:56  dpfeuffer
 * R: new HW F207
 * M: ModBbisF207 added
 *
 * Revision 2.5  2011/04/01 09:36:20  CRuff
 * R: 1. pci bus interfaces may have a pci domain
 *    2. pci domain is always stored in key PCI_DOMAIN_NUMBER, not in bus path
 * M: 1. in applyProperties(), takeover pci domain from bus interface
 *    2. removed code to handle PCI_BUS_PATH2
 *
 * Revision 2.4  2010/07/05 18:05:53  CRuff
 * R: implementation of PCI Domains
 * M: handle PCI_DOMAIN_NR / PCI_BUS_PATH2 in createSpecialDesc and
 *    applyProperties
 *
 * Revision 2.3  2010/05/31 10:50:27  dpfeuffer
 * R: compiler warnings under Windows during mdiswiz_lib build
 * M: variable declarations moved
 *
 * Revision 2.2  2010/05/31 08:58:38  CRuff
 * R: 1. no object hierarchy available if MDIS Wizard is used as Lib
 * M: 1a) avoid using information from parent or child objects
 *    1b) remember the PCI Bus Path in properties if MDIS Wizard isused
 *        as library
 *    1c) add safety checks if parent is defined before using it
 *
 * Revision 2.1  2010/05/21 15:55:15  CRuff
 * R: 1. model OBPCIGEN needs to be exported for Windows registry handling
 * M: 1a) replaced generation of DEVICE_SLOT_x keys by read/write descriptor
 *        functionality (initialized in method ModBbisPciGen::createSlot)
 *    1b) added handling for new members in createProperties() and copyProperties
 *    1c) added handling for descriptor keys DEVNAME_SLOT_x and FUNCTION_SLOT_x
 *        in applyProperties() and createSpecialDesc()
 *
 * Revision 2.0  2010/01/22 11:29:10  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.7  2009/09/03 11:05:17  CRuff
 * R: cosmetics
 * M: add safety checks, debugs
 *
 * Revision 1.6  2005/03/09 13:49:30  ub
 * cosmetics
 *
 * Revision 1.5  2004/06/08 17:26:01  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.4  2003/06/05 11:59:29  dschmidt
 * intermediate checkin
 *
 * Revision 1.3  2003/06/05 09:35:08  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/06/03 10:17:49  kp
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:33:12  dschmidt
 * Initial Revision
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>

#include "modbbispcigen.h"
#include "wizdebug.h"
#include "modbbispcigenproptab.h"
#include "modserialproptab.h"
#include "configuration.h"
#include "modelfactory.h"
#include "resourcemng.h"


// -----------------------------------------------------------------
// Class for PCI / PCI carriers (F203, D202)

//! Create a BBIS device for a generic PCI carrier (F203..)
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
ModBbisPciGen::ModBbisPciGen( 
	QString instName, 
	QString hwName,
	QString driverName,
	BusIfType ifType) :
	BbisDevice( hwName, driverName, true, ifType )
{
	if( prop ) delete prop;	// delete Device::prop
	prop = createProperties();
	setInstName( instName );
}

ModBbisPciGen::ModBbisPciGen()
: BbisDevice( QString("OBPCIGEN"), QString("PCI"), true, LocalBus )
{
	if( prop ) delete prop;	// delete Device::prop
	prop = createProperties();
}
	 	
SwModuleList *
ModBbisPciGen::enumSwModules()
{
	if( lstSwModules == 0 ){
		lstSwModules = new SwModuleList;

		// always ignore byteswapping (to generate driver.mak)
		lstSwModules->add( makeStdDriverSwModule(
			"PCI", "", true, "Generic BBIS driver for PCI devices"));
	}
	return lstSwModules;
}

//! as Device::getDriverName, but always returns non-swapped variant
QString 
ModBbisPciGen::getDriverName( bool fullName, bool withSw)
{
	UTIL_UNREF_PARAM(withSw);
	return Device::getDriverName( fullName, false );
}

//! destructs ModBbisPciGen
ModBbisPciGen::~ModBbisPciGen()
{
	wDebug(("~ModBbisPciGen()"));
	freeResources();
	if (prop)
		delete prop;
	prop = 0;
}

//! create an BbisSlot on this BBIS device
/*!
  must be called for each provided slot
  \param slotNo	the MDIS device slot number
  \param pciDevNo PCI device number corresponding to \a slotNo
  \param slotIfType the bus interface type of that slot
*/
void
ModBbisPciGen::createSlot( int slotNo, int pciDevNo, BusIfType slotIfType )
{
	ModBbisPciGenProperties *p;
	WIZ_DYNAMIC_CAST( prop, p, ModBbisPciGenProperties* );
	
	BbisSlot *bbSlot;

	wDebug(("ModBbisPciGen::createSlot: %d -> %d", slotNo, pciDevNo ));

	bbSlot = new BbisSlot( slotIfType, slotNo, pciDevNo );
	addChild( bbSlot );
	
	// set properties
	uint count = p->bbSlotDevNbrList.count();
	if ( count <= (uint)slotNo) {
		p->bbSlotDevNbrList.resize(slotNo+1);
		for (int i=count; i<=slotNo; i++) {
			p->bbSlotDevNbrList[i] = -1; // initialize
		}
	}
	
	p->bbSlotDevNbrList[slotNo] = pciDevNo;
}

//! create an BbisSlot on this BBIS device
/*!
  must be called for each provided slot
  \param slotNo	the MDIS device slot number
  \param slotIfType the bus interface type of that slot
*/
void
ModBbisPciGen::createSlot( int slotNo, BusIfType slotIfType )
{
	ModBbisPciGenProperties *p;
	WIZ_DYNAMIC_CAST( prop, p, ModBbisPciGenProperties* );
	
	BbisSlot *bbSlot;

	wDebug(("ModBbisPciGen::createSlot: %d", slotNo ));

	bbSlot = new BbisSlot( slotIfType, slotNo );
	addChild( bbSlot );
	
	// set properties
	uint count = p->bbSlotDevNbrList.count();
	if ( count <= (uint)slotNo) {
		p->bbSlotDevNbrList.resize(slotNo+1);
		for (int i=count; i<=slotNo; i++) {
			p->bbSlotDevNbrList[i] = -1; // initialize
		}
	}
}

DevPropertiesDlg *
ModBbisPciGen::createPropDlg( QWidget *parent )
{
	DevPropertiesDlg *dp;
	QWidget *defTab=0;

	dp = new DevPropertiesDlg( parent, this );

	PciBusInterface *busIf = (PciBusInterface *)getParent();

	if( (busIf->getIsPciE()) ){
		dp->addTab( (defTab = new ModSerialPropTab(parent, dp, 0, 0, true)),
					QString("%1 Settings").arg(
						busIfTypeMap.keyToStr( getBusIfType() )));
	} // if ( ifType != LocalBus )
	else {
		dp->addTab( (defTab = new ModBbisPciGenPropTab(parent, dp)),
					QString("%1 Settings").arg( 
						busIfTypeMap.keyToStr( getBusIfType() )));
	}

	dp->setButtons();
	if( defTab )
		dp->showPage( defTab );
	wDebug(("ModBbisPciGen::createPropDlg dp=%p", dp ));
	return dp;
}

DeviceProperties *
ModBbisPciGen::createProperties()
{
	wDebug(("ModBbisPciGen::createProperties()"));

	ModBbisPciGenProperties *p = new ModBbisPciGenProperties();
	// set defaults
	p->slotNo = 2;
	p->pciBusNo = 1;
    p->pciDevNo = 0;
	p->pciDomainNo = 0;
	p->skipDevBridgeChk = 0;
#ifdef WIN_MDISWIZ_LIB
	p->pciBusPath.resize(0);
#endif
	p->bbSlotDevNbrList.resize(0);
	p->bbSlotFuncNbrList.resize(0);
	p->bbSlotDevNameList.clear();
	p->dbgSettings.setModules(		
		(1<<DebugProperties::Driver) | 
		(1<<DebugProperties::BbisKernel) | 
		(1<<DebugProperties::Oss) | 
		(1<<DebugProperties::Desc));

	return p;
}

// ts: overload createProperties with special handling for F223

DeviceProperties *
ModBbisPciGen::createProperties(int isF223)
{
	ModBbisPciGenProperties *p = new ModBbisPciGenProperties();
	// set defaults
	p->slotNo = 2;
	p->pciBusNo = 1;
	p->pciDomainNo = 0;

	p->skipDevBridgeChk = !!isF223;
	wDebug(("ModBbisPciGen::createProperties(isF223)"));
	wDebug((" p->skipDevBridgeChk = %d",p->skipDevBridgeChk));
	
#ifdef WIN_MDISWIZ_LIB
	p->pciBusPath.resize(0);
#endif
	p->bbSlotDevNbrList.resize(0);
	p->bbSlotFuncNbrList.resize(0);
	p->bbSlotDevNameList.clear();
	p->dbgSettings.setModules(		
		(1<<DebugProperties::Driver) | 
		(1<<DebugProperties::BbisKernel) | 
		(1<<DebugProperties::Oss) | 
		(1<<DebugProperties::Desc));

	return p;
}

void 
ModBbisPciGen::copyProperties( 
	const DeviceProperties *from,
	DeviceProperties *to )
{
	const ModBbisPciGenProperties *_from;
	ModBbisPciGenProperties *_to;

	WIZ_DYNAMIC_CAST( to, _to, ModBbisPciGenProperties * );
	WIZ_DYNAMIC_CAST( from, _from, const ModBbisPciGenProperties * );

	*_to = *_from;
	
	_to->bbSlotDevNbrList.duplicate(_from->bbSlotDevNbrList);
	_to->bbSlotFuncNbrList.duplicate(_from->bbSlotFuncNbrList);
}


void
ModBbisPciGen::freeResources()
{
	if ( getParent() )
	{
		PciBusInterface *busIf = (PciBusInterface *)getParent();
		if( busIf == 0 ) return;
		
		wDebug(("ModBbisPciGen::freeResources"));
		if (busIf->pciSlots)
			busIf->pciSlots->freeAllOfOwner( this );
	}
	Device::freeResources();
}

Device::ApplyResult 
ModBbisPciGen::applyProperties( DeviceProperties *newProp, 
									QString &errMsg )
{
	if( newProp == 0 )
		newProp = prop;

	ModBbisPciGenProperties *_prop;

	WIZ_DYNAMIC_CAST( newProp, _prop, ModBbisPciGenProperties * );

	ApplyResult ar;
	QString msg;
	wDebug(("ModBbisPciGen::applyProperties"));
	
#ifndef WIN_MDISWIZ_LIB
	ulong s;
	ResourceMng::IsAvailResult isAvail;
	ResourceMng::AllocResult alr = ResourceMng::AllocResOk;
	HwComponent *owner;
	PciBusInterface *busIf;
	WIZ_DYNAMIC_CAST( getParent(), busIf, PciBusInterface * );

	// only allocate something when busIf provides a pciBusPath
	if( busIf->hasPciBusPath() ){

		isAvail = busIf->pciSlots->isAvail( _prop->slotNo, 1, &owner );

		if( isAvail != ResourceMng::Avail ){
			msg.sprintf("Cannot use slot %d\n",	_prop->slotNo );
			errMsg += msg;
		
			if( isAvail == ResourceMng::InUse ){
				msg.sprintf("Already used by device %s\n", 
							(const char *)owner->getInstName() );
				errMsg += msg;
			}
		
			alr = busIf->pciSlots->alloc( 1, 1, s, this );
		}
		else {
			alr = busIf->pciSlots->alloc( 1, 1, s, this, _prop->slotNo );
		}
			
		if( alr == ResourceMng::AllocResFail ){
			msg.sprintf("Could not allocate any %s slot!\n", 
						(const char *)busIfTypeMap.keyToStr( getBusIfType()));
			errMsg += msg;
			goto abort;
		}

		else if( s != (ulong)_prop->slotNo ){
			 if( (_prop->deviceName.contains("f223")) )
			 {
				 msg.sprintf("Alternative address 0x%lx allocated\nFor F223 bus number has higher priority", s );
				 _prop->slotNo = s;
				 errMsg += msg;
				 ar = ApplyPropOk;
			 }
			 else
			 {
				 if ( !_prop->usePciBusNoAndDevNo() ) {
					 msg.sprintf("Alternative address 0x%lx allocated\n", s );
				 }
				 _prop->slotNo = s;
				 errMsg += msg;
				 ar = ApplyPropAlternate;
			 }
		}
	}

	if ( busIf ) {
		if ( busIf->getPciDomainNo() != 0 ) {
			_prop->pciDomainNo = busIf->getPciDomainNo();
		}
	}
#endif

	// allocate name	
	msg = QString::null;
	ar = Device::applyProperties( _prop, msg );
	errMsg += msg;
	if( ar == ApplyPropFail )
		goto abort;

	wDebug(("ModBbisPciGen::applyProperties errMsg=%s",
		   (const char *)errMsg));

	return ar;

 abort:	
	freeResources();	
	wDebug(("ModBbisPciGen::applyProperties failed errMsg=%s",
		   (const char *)errMsg));
	return ApplyPropFail;
}

Device::ApplyResult
ModBbisPciGen::applyProperties( DescriptorEntryDirectory *devDesc, 
									QString &errMsg )
{
	DescriptorEntry *e, *eNext;
	DescriptorEntryUint32 *eUint32;
	DescriptorEntryBinary *eBinary;
	DescriptorEntryString *eString;
	ModBbisPciGenProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisPciGenProperties * );

	if( !devDesc ) return ApplyPropFail;

	// scan all descriptor entries
	eNext=devDesc->firstChild();

	while( (e=eNext) ){
		bool entryHandled = true;
		QString key = e->path().section("/",1);
		
		wDebug(("key=%s", (const char *)key ));

		if( key == "PCI_BUS_NUMBER" ){
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->pciBusNo = eUint32->getValue();
			_prop->slotNo = -1;
			_prop->pciBusNoIsDef=true;
		}
		else if ( key == "PCI_DOMAIN_NUMBER" ) {
        	WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
        	_prop->pciDomainNo = eUint32->getValue();
        }

		else if ( key == "SKIP_DEV_BRIDGE_CHECK" ) {
        	WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
        	_prop->skipDevBridgeChk = eUint32->getValue();
        }
		else if( key == "PCI_BUS_PATH" ){
        	Q3MemArray<uchar> parentBusPath;
			Q3MemArray<uchar> busPath;
			WIZ_DYNAMIC_CAST( e, eBinary, DescriptorEntryBinary *);
			
			// get bus path from descriptor
      		busPath = eBinary->getValue();

  			for (unsigned int i=0; i<busPath.count(); i++) {
  				wDebug(("busPath[%d] = %d",i,busPath[i]));
  			}

#ifdef WIN_MDISWIZ_LIB
			_prop->pciBusPath.duplicate(busPath);
			_prop->slotNo = -1;
#else
			PciBusInterface *busIf = (PciBusInterface *)getParent();

			// Special handling for PCI BBIS usage without slot creation,
			// required for ModBbisPci4Mezz class.
			if ( modelName == "MEZZ_PCI" ) {

				if( busIf && busIf->hasPciBusPath() ){
            		// get path from parent (for comparison)
					busIf->pciBusPath( -1, &parentBusPath );

					for (unsigned int i=0; i<parentBusPath.count(); i++) {
	      				wDebug(("parentPath[%d] = %d",i,parentBusPath[i]));
	      			}

	      			// compare with parent path
	      			if( busPath != parentBusPath ){
						errMsg += "PCI_BUS_PATH descriptor/device discrepancy!\n";
					}

					_prop->slotNo = 0;
				}
			}
			else{
				_prop->slotNo = busIf->busPathToSlot(busPath);
			}

			if( _prop->slotNo == -1 )
				errMsg += "PCI_BUS_PATH invalid!";
#endif
		}
        else if( (key == "_WIZ_SLOT") && (modelName == "MEZZ_PCI") ){
            WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
            _prop->slotNo = eUint32->getValue();
        }
		else if( key.startsWith( "DEVICE_SLOT_" )) {
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			uint devNo = eUint32->getValue();
			
			// get slot number
			bool convSuccess = false;
			QString slotStr = key.section('_', -1);
			uint slotNum = slotStr.toUInt(&convSuccess);
			if (convSuccess) {
				uint count = 0;
				if (_prop->bbSlotDevNbrList) 
					count = _prop->bbSlotDevNbrList.count();
				if ( count <= slotNum) {
					_prop->bbSlotDevNbrList.resize(slotNum+1);
					for (uint i=count; i<=slotNum; i++) {
						_prop->bbSlotDevNbrList[i] = -1; // initialize
					}
				}
				_prop->bbSlotDevNbrList[slotNum] = devNo;
				_prop->pciDevNo = devNo;
				_prop->pciDevNoIsDef=true;
			}
		}
		else if( key.startsWith( "FUNCTION_SLOT_" )) {
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			uint funcNo = eUint32->getValue();
			
			// get slot number
			bool convSuccess = false;
			QString slotStr = key.section('_', -1);
			uint slotNum = slotStr.toUInt(&convSuccess);
			if (convSuccess) {
				uint count = 0;
				if (_prop->bbSlotFuncNbrList)
					count = _prop->bbSlotFuncNbrList.count();
				if (count <= slotNum) {
					_prop->bbSlotFuncNbrList.resize(slotNum+1);
					for (uint i=count; i<=slotNum; i++) {
						_prop->bbSlotFuncNbrList[i] = -1; // initialize
					}
				}
				_prop->bbSlotFuncNbrList[slotNum] = funcNo;
			}
		}
		else if ( key.startsWith( "DEVNAME_SLOT_")) {
			WIZ_DYNAMIC_CAST( e, eString, DescriptorEntryString* );
			QString name = eString->getValue();
			
			// get slot number
			bool convSuccess = false;
			QString slotStr = key.section('_', -1);
			uint slotNum = slotStr.toUInt(&convSuccess);
			if (convSuccess) {
				uint count = _prop->bbSlotDevNameList.size();
				if (count <= slotNum) {
					for (uint i=count; i<=slotNum; i++) {
						_prop->bbSlotDevNameList.append(""); // initialize
					}
				}
				_prop->bbSlotDevNameList[slotNum] = name;
			}
		}
		else
			entryHandled = false;

		eNext=e->nextSibling();

		// if entry has been handled, delete it 
		if( entryHandled == true )
			e->getParent()->removeChild(e);

	}
	_prop->useSlotNo = ( !_prop->usePciBusNoAndDevNo()) ;
	
	return Device::applyProperties( devDesc, errMsg );
}

QString 
ModBbisPciGen::getMainProperties()
{
	QString rv = "";
	ModBbisPciGenProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisPciGenProperties * );

	wDebug(( "ModBbisPciGen::getMainProperties() _prop->pciBusNo = %d",
		_prop->pciBusNo ));

	PciBusInterface *busIf = (PciBusInterface *)getParent();
	if( busIf ) {
		rv = getParent()->getMainProperties();
		if( busIf->hasPciBusPath() && !_prop->usePciBusNoAndDevNo() )
			rv += QString(" Slot %1").arg(_prop->slotNo);
		else {
			if( _prop->pciDomainNo > 0 )
				rv += QString(" Dom %1 /").arg(_prop->pciDomainNo);

			rv += QString(" Bus %1 / Dev %2").arg(_prop->pciBusNo).arg(_prop->pciDevNo);
		}
	}
	return rv;
}

//
// This is called by MdisDevice::createDesc 
// to fill device specific descriptor entries
void
ModBbisPciGen::createSpecialDesc( DescriptorEntryDirectory *parentDesc )
{
	ModBbisPciGenProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisPciGenProperties * );
	DescriptorFactory dFact;
	DescriptorEntry *e;
	bool isF223 = false;

	wDebug(("ModBbisPciGen::createSpecialDesc"));

#ifdef WIN_MDISWIZ_LIB
	if ( (_prop->pciBusPath) &&  (_prop->pciBusPath.count())) {
		parentDesc->addChild( dFact.create("PCI_BUS_PATH", _prop->pciBusPath ));
	} else {
		parentDesc->addChild( dFact.create("PCI_BUS_NUMBER", _prop->pciBusNo));
	}
	if (_prop->pciDomainNo != 0 ) {
		parentDesc->addChild( dFact.create("PCI_DOMAIN_NUMBER", _prop->pciDomainNo ));
	}
	if (_prop->skipDevBridgeChk != 0 ) {
		parentDesc->addChild( dFact.create("SKIP_DEV_BRIDGE_CHECK", _prop->skipDevBridgeChk ));
	}
#else // WIN_MDISWIZ_LIB

	PciBusInterface *busIf = (PciBusInterface *)getParent();
	WIZ_ASSERT(busIf);

	if( (_prop->deviceName.contains("f223")))
	{
		isF223 = true;
	}
	// use pci bus / dev number?
	if( !_prop->usePciBusNoAndDevNo() && !isF223 ){

		uchar				devNum;
		Q3MemArray<uchar>	busPath;

		busIf->pciBusPath( _prop->slotNo, &busPath );

		for (unsigned int i=0; i<busPath.count(); i++) {
			wDebug(("busPath[%d] = %d",i,busPath[i]));
		}

		// Special handling for PCI BBIS usage without slot creation,
		// required for ModBbisPci4Mezz class.
		if ( modelName == "MEZZ_PCI" ) {
			// same handling as for PCI_DEVICE_NUMBER in ModBbisCham::createSpecialDesc
			int sz;

			sz = busPath.size();

			devNum = busPath.at( sz - 1 ); // take last entry
			busPath.resize( sz - 1 );

  			for (unsigned int i=0; i<busPath.count(); i++) {
  				wDebug(("busPath[%d] = %d",i,busPath[i]));
  			}
		}

		parentDesc->addChild( dFact.create("PCI_BUS_PATH", busPath ));

		// print DEVICE_SLOT_0 after PCI_BUS_PATH
		if ( modelName == "MEZZ_PCI" ) {
			parentDesc->addChild( dFact.create("DEVICE_SLOT_0", devNum)); 
	        parentDesc->addChild( dFact.create("_WIZ_SLOT", _prop->slotNo ));
		}
	}
	// unknown bus path
	else {
		// PCI_BUS_NUMBER
		parentDesc->addChild( dFact.create("PCI_BUS_NUMBER", _prop->pciBusNo));

		// DEVICE_SLOT_0
		parentDesc->addChild( dFact.create("DEVICE_SLOT_0", _prop->pciDevNo));
	}

	if (_prop->pciDomainNo != 0 ) {
		parentDesc->addChild( dFact.create("PCI_DOMAIN_NUMBER", _prop->pciDomainNo ));
	}

	if (_prop->skipDevBridgeChk != 0 ) {
		parentDesc->addChild( dFact.create("SKIP_DEV_BRIDGE_CHECK", _prop->skipDevBridgeChk ));
	}

	if (_prop->bbSlotDevNbrList  && (modelName != "MEZZ_PCI") ) {
		for (uint i=0; i<_prop->bbSlotDevNbrList.count(); i++ ) {
			if ( _prop->bbSlotDevNbrList[i] == -1 )
				continue;
			
			parentDesc->addChild( 
				dFact.create(QString("DEVICE_SLOT_%1").arg(i), _prop->bbSlotDevNbrList[i])); 
		} 
	}

	if ( _prop->bbSlotFuncNbrList ) {
		for (uint i=0; i<_prop->bbSlotFuncNbrList.count(); i++ ) {
			if ( _prop->bbSlotFuncNbrList[i] == -1 )
				continue;
				
			e = dFact.create(QString("FUNCTION_SLOT_%1").arg(i), _prop->bbSlotFuncNbrList[i]);
			e->setHidden( true );
			parentDesc->addChild( e ); 
		}
	}

	for (int i=0; i<_prop->bbSlotDevNameList.size(); i++ ) {
		if ( _prop->bbSlotDevNameList[i].isEmpty() )
			continue;
		
		e = dFact.create(QString("DEVNAME_SLOT_%1").arg(i), _prop->bbSlotDevNameList[i]);
		e->setHidden( true );
		parentDesc->addChild( e ); 
	}
		
#endif // WIN_MDISWIZ_LIB
}

// -----------------------------------------------------------------

//! create an D202 BBIS device
ModBbisD202::ModBbisD202() : 
	ModBbisPciGen(QString("D202"),QString("D202"),QString("PCI"),Cpci)
{
	
	createSlot( 0, 0xc, PcMip);
	createSlot( 1, 0xd, PcMip);
	createSlot( 2, 0xe, PcMip);
	createSlot( 3, 0xf, PcMip);
	createSlot( 4, 0xa, PcMip);
	createSlot( 5, 0xb, PcMip);

	setDescription("CPCI 6U Six Slot PC-MIP Carrier Board");
}

Device *ModBbisD202::create(bool withSubDevs)
{
	withSubDevs = withSubDevs;
	return new ModBbisD202();
}

// -----------------------------------------------------------------

//! create an F203 BBIS device
ModBbisF203::ModBbisF203() : 
	ModBbisPciGen(QString("F203"),QString("F203"),QString("PCI"),Cpci)
{
	
	createSlot( 0, 0xe, PcMip);
	createSlot( 1, 0xf, PcMip);
	createSlot( 2, 0xd, PcMip);

	setDescription("CPCI 3U Three Slot PC-MIP Carrier Board");
}

Device *ModBbisF203::create(bool withSubDevs)
{
	withSubDevs = withSubDevs;
	return new ModBbisF203();
}

// -----------------------------------------------------------------

//! create an F207 BBIS device
ModBbisF207::ModBbisF207() : 
	ModBbisPciGen(QString("F207"),QString("F207"),QString("PCI"),Cpci)
{
	createSlot( 0, 0x4, Pci104);
	createSlot( 1, 0x5, Pci104);
	createSlot( 2, 0x6, Pci104);
	createSlot( 3, 0x7, Pci104);

	setDescription("CPCI 3U PCI-104 Carrier Board");
}

Device *ModBbisF207::create(bool withSubDevs)
{
	withSubDevs = withSubDevs;
	return new ModBbisF207();
}

// -----------------------------------------------------------------

//! create an F223 BBIS device
ModBbisF223::ModBbisF223() : 
	ModBbisPciGen(QString("F223"),QString("F223"),QString("PCI"), Cpci )
{
	wDebug(("ModBbisF223::ModBbisF223()"));
	// delete preconfigured Device::prop and make one for F223
	if( prop ) 
		delete prop;	
	prop = createProperties( 1 );

	// dummy Slot holding the f223_gpio driver 
	// use LocalBus because theres no other driver with that interface
	// to be added in MDIS yet.
	createSlot( 0, LocalBus);
	setDescription("CPCI 3U PCIe Mini Card Carrier");

	//pre-populate slot (only the f223_gpio_1 driver makes sense here)
	
}

Device *ModBbisF223::create(bool withSubDevs)
{
	withSubDevs = withSubDevs;
	return new ModBbisF223();
}

// -----------------------------------------------------------------

//! Creates a generic mezzanine PCI BBIS device
ModBbisPci4Mezz::ModBbisPci4Mezz(bool withSubDevs) :
	ModBbisPciGen( QString("PCI"),
				   QString("MEZZ_PCI"),
				   QString("PCI"),
				   PciOrCham )
{
	UTIL_UNREF_PARAM(withSubDevs);

	setDescription("PCI(e) device at PCI(e)/CPCI(Serial)/PMC/XMC/... mezzanine");

	// create slot for LL driver package
	createSlot( 0, StdPci );
}
