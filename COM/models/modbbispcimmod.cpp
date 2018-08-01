/***************************************************************************
 */
/*!       \file  modbbispcimmod.cpp
 *       \brief  Implementation of PCI/M-Module BBIS devices
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2014/08/22 15:56:31 $
 *    $Revision: 2.4 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbispcimmod.cpp,v $
 * Revision 2.4  2014/08/22 15:56:31  dpfeuffer
 * R: domain number missing
 * M: show domain number (if>0)
 *
 * Revision 2.3  2011/04/01 09:36:45  CRuff
 * R: 1. pci bus interfaces may have a pci domain
 *    2. pci domain is always stored in key PCI_DOMAIN_NUMBER, not in bus path
 * M: 1. in applyProperties(), takeover pci domain from bus interface
 *    2. removed code to handle PCI_BUS_PATH2
 *
 * Revision 2.2  2010/07/05 18:11:24  CRuff
 * R: implementation of PCI Domains
 * M: handle PCI_DOMAIN_NR / PCI_BUS_PATH2 in createSpecialDesc and
 *    applyProperties
 *
 * Revision 2.1  2010/06/21 11:00:34  CRuff
 * R: console error message for QString: not enough arguments
 * M: in getMainProperties: fix argument number in QString
 *
 * Revision 2.0  2010/01/22 11:29:19  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.11  2006/11/30 11:28:21  cs
 * removed:
 *   - D203 family moved to own files
 *
 * Revision 1.10  2005/07/15 15:47:26  ub
 * cosmetics
 *
 * Revision 1.9  2005/03/09 13:49:34  ub
 * cosmetics
 *
 * Revision 1.8  2004/07/21 15:14:49  kp
 * cosmetic change for VC6.0 compatibility
 *
 * Revision 1.7  2004/06/08 17:26:03  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.6  2004/01/09 08:20:48  kp
 * intermediate checkin
 *
 * Revision 1.5  2003/06/11 12:21:51  kp
 * intermediate checkin
 *
 * Revision 1.4  2003/06/05 11:59:33  dschmidt
 * intermediate checkin
 *
 * Revision 1.3  2003/06/05 09:35:12  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/06/03 10:17:53  kp
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:33:16  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>

#include "modbbispcimmod.h"
#include "modbbispcimmodproptab.h"
#include "wizdebug.h"
#include "configuration.h"
#include "resourcemng.h"


// -----------------------------------------------------------------
// Class for PCI M-Module carriers


//! Create a BBIS device for a PCI carrier with M-Module slots
/*!
  The device will have the default properties set by createProperties().
  It will have no BBIS slots (must be created by constructor of subclasses
  by calling createSlots()).

  \param instName initial instance name of device
  \param hwName	Hardware name (e.g. "D201")
  \param driverName (e.g. "D201")
  \param ifType upper interface type (e.g. Cpci)
  \todo instName really needed?
*/
ModBbisPciMmod::ModBbisPciMmod(
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


//! destructs ModBbisPciMmod
ModBbisPciMmod::~ModBbisPciMmod()
{
	wDebug(("~ModBbisPciMmod()"));
	freeResources();
	if( prop ) delete prop; /* already deleted by child class (e.g. D203Fam)? */
	prop = 0;
}

//! create the required BbisSlot instances
/*!
  This functions calls getSlots(), which may be reimplemented
  by subclasses to get the number of slots on the board.
*/
void
ModBbisPciMmod::createSlots()
{
	BbisSlot *bbSlot;

	wDebug(("ModBbisPciMmod::createSlots slots: %d", getSlots()));
	for( int slotNum=0; slotNum<getSlots(); slotNum++ ){
		bbSlot = new BbisSlot( slotNum, MmodD32, MmodA24 );
		addChild( bbSlot );
	}
}

//! Set PCI_BUS_SLOT parameter
/*!
  BBIS PCI-MMOD special call. Can be used by CPU models to set a fixed
  PCI slot number for the carrier board (e.g. Kahlua Box)
*/
void
ModBbisPciMmod::setSlotNo( int slotNo )
{
	ModBbisPciMmodProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisPciMmodProperties * );

	_prop->slotNo = slotNo;
}

DevPropertiesDlg *
ModBbisPciMmod::createPropDlg( QWidget *parent )
{
	DevPropertiesDlg *dp;
	QWidget *defTab;

	dp = new DevPropertiesDlg( parent, this );

	dp->addTab( (defTab = new ModBbisPciMmodPropTab(parent, dp)),
				QString("%1 Settings").arg( busIfTypeMap.keyToStr(
					getBusIfType() )));

	dp->setButtons();
	dp->showPage( defTab );

	wDebug(("ModBbisPciMmod::createPropDlg dp=%p", dp ));
	return dp;
}

DeviceProperties *
ModBbisPciMmod::createProperties()
{
	ModBbisPciMmodProperties *p = new ModBbisPciMmodProperties();

	p->dbgSettings.setModules(
		(1L<<DebugProperties::Driver) |
		(1L<<DebugProperties::BbisKernel) |
		(1L<<DebugProperties::Oss) |
		(1L<<DebugProperties::Desc));

	// set defaults
	p->slotNo = 2;
	p->pciDevNo = 15;
	p->pciBusNo = 0;
	p->pciDomainNo = 0;

	wDebug(("ModBbisPciMmod::createProperties p=%p", p ));
	return p;
}

void
ModBbisPciMmod::copyProperties(
	const DeviceProperties *from,
	DeviceProperties *to )
{
	const ModBbisPciMmodProperties *_from;
	ModBbisPciMmodProperties *_to;

	WIZ_DYNAMIC_CAST( to, _to, ModBbisPciMmodProperties * );
	WIZ_DYNAMIC_CAST( from, _from, const ModBbisPciMmodProperties * );

	*_to = *_from;
}


void
ModBbisPciMmod::freeResources()
{
	PciBusInterface *busIf = (PciBusInterface *)getParent();
	if( busIf == 0 ) return;

	wDebug(("ModBbisPciMmod::freeResources"));
	busIf->pciSlots->freeAllOfOwner( this );

	Device::freeResources();
}

Device::ApplyResult
ModBbisPciMmod::applyProperties( DeviceProperties *newProp,
									QString &errMsg )
{
	if( newProp == 0 )
		newProp = prop;

	ModBbisPciMmodProperties *_prop;

	WIZ_DYNAMIC_CAST( newProp, _prop, ModBbisPciMmodProperties * );

	ApplyResult ar;

	PciBusInterface *busIf = (PciBusInterface *)getParent();
	QString msg;
	ulong s;
	ResourceMng::IsAvailResult isAvail;
	ResourceMng::AllocResult alr = ResourceMng::AllocResOk;
	HwComponent *owner;

	wDebug(("ModBbisPciMmod::applyProperties"));

	// only allocate something when busIf provides a pciBusPath
	if( busIf && busIf->hasPciBusPath() ){

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
			msg.sprintf("Alternative address 0x%lx allocated\n", s );
			_prop->slotNo = s;
			errMsg += msg;
			ar = ApplyPropAlternate;
		}
	}
	
	if ( busIf ) {
		if ( busIf->getPciDomainNo() != 0 ) {
			_prop->pciDomainNo = busIf->getPciDomainNo();
		}
	}

	// allocate name
	msg = QString::null;
	ar = Device::applyProperties( _prop, msg );
	errMsg += msg;
	if( ar == ApplyPropFail )
		goto abort;

	wDebug(("ModBbisPciMmod::applyProperties errMsg=%s",
		   (const char *)errMsg));

	return ar;

 abort:
	freeResources();
	wDebug(("ModBbisPciMmod::applyProperties failed errMsg=%s",
		   (const char *)errMsg));
	return ApplyPropFail;
}

Device::ApplyResult
ModBbisPciMmod::applyProperties( DescriptorEntryDirectory *devDesc,
									QString &errMsg )
{
	DescriptorEntry *e, *eNext;
	DescriptorEntryUint32 *eUint32;
	DescriptorEntryBinary *eBinary;
	ModBbisPciMmodProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisPciMmodProperties * );

	if( !devDesc ) return ApplyPropFail;

	// scan all descriptor entries
	eNext=devDesc->firstChild();

	// ??? should do a little more sanity checking...
	while( (e=eNext) ){
		bool entryHandled = true;
		QString key = e->path().section("/",1);

		wDebug(("%s: desc.key=%s", getInstName().ascii(), (const char *)key ));

		if( key == "PCI_BUS_SLOT" ){
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->slotNo = eUint32->getValue();
		}
		else if( key == "PCI_BUS_NUMBER" ){
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->pciBusNo = eUint32->getValue();
		}
		else if ( key == "PCI_DOMAIN_NUMBER" )
        {
        	WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
        	_prop->pciDomainNo = eUint32->getValue();
        }
		else if( key == "PCI_DEVICE_ID" ){
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->pciDevNo = eUint32->getValue();
		}
		else if( key == "PCI_BUS_PATH" ){
			Q3MemArray<uchar> busPath;
			Q3MemArray<uchar> parentBusPath;
			PciBusInterface *busIf = (PciBusInterface *)getParent();

			WIZ_DYNAMIC_CAST( e, eBinary, DescriptorEntryBinary *);

			if( busIf && busIf->hasPciBusPath() ){
				
				busIf->pciBusPath( -1, &parentBusPath );
				
				// get bus path from descriptor
	      		busPath = eBinary->getValue();
	      		
				if( busPath != parentBusPath ){
					errMsg += "PCI_BUS_PATH descriptor/device discrepancy!";
				}
			}
		}
		else
			entryHandled = false;

		eNext=e->nextSibling();

		// if entry has been handled, delete it
		if( entryHandled == true )
			e->getParent()->removeChild(e);

	}

	return Device::applyProperties( devDesc, errMsg );
}

QString
ModBbisPciMmod::getMainProperties()
{
	QString rv;
	ModBbisPciMmodProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisPciMmodProperties * );
	PciBusInterface *busIf = (PciBusInterface *)getParent();

	if( busIf ){
		WIZ_ASSERT( getParent() );

		rv = getParent()->getMainProperties();

		if( busIf->hasPciBusPath() )
			rv += QString(" Slot %1").arg(_prop->slotNo);
		else {
			if( _prop->pciDomainNo > 0 )
				rv += QString(" Dom %1 /").arg(_prop->pciDomainNo);

			rv += QString(" Bus %1 / Dev %2").arg(_prop->pciBusNo).arg(_prop->pciDevNo);
		}
	}
	else{
		rv = "";
	}

	return rv;
}

//
// This is called by MdisDevice::createDesc
// to fill device specific descriptor entries
void
ModBbisPciMmod::createSpecialDesc( DescriptorEntryDirectory *parentDesc )
{
	ModBbisPciMmodProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisPciMmodProperties * );
	PciBusInterface *busIf = (PciBusInterface *)getParent();
	DescriptorFactory dFact;

	wDebug(("ModBbisPciMmod::createSpecialDesc"));

	if( busIf && busIf->hasPciBusPath() ){

		// PCI_BUS_PATH
		Q3MemArray<uchar> busPath;
		busIf->pciBusPath( -1, &busPath );
		
		if (_prop->pciDomainNo != 0 )
			parentDesc->addChild( dFact.create("PCI_DOMAIN_NUMBER", _prop->pciDomainNo ));
		
		parentDesc->addChild( dFact.create("PCI_BUS_PATH", busPath ));

		// PCI_BUS_SLOT
		parentDesc->addChild( dFact.create("PCI_BUS_SLOT", _prop->slotNo ));

		// PCI_DEVICE_ID (name is missleading...)
		//
		// This forces the D201 driver to ignore OSS_PciSlotToPciDevice...
		busIf->pciBusPath( _prop->slotNo, &busPath );
		if( busPath.size() > 0 ){
			int ix = busPath.size() -1;
			parentDesc->addChild( dFact.create("PCI_DEVICE_ID", busPath[ix] ));
		}

	}
	else {
		// PCI_BUS_NUMBER
		parentDesc->addChild( dFact.create("PCI_BUS_NUMBER", _prop->pciBusNo));
		
		if (_prop->pciDomainNo != 0 )
	        parentDesc->addChild( dFact.create("PCI_DOMAIN_NUMBER", _prop->pciDomainNo ));

		// PCI_DEVICE_ID (name is missleading...)
		parentDesc->addChild( dFact.create("PCI_DEVICE_ID", _prop->pciDevNo));
	}
}

// -----------------------------------------------------------------

//! Create a D201 BBIS device
ModBbisD201::ModBbisD201() :
	ModBbisPciMmod(QString("D201"),QString("D201"),QString("D201"),Cpci)
{
	createSlots();
	setDescription("CPCI 6U Four Slot M-Module Carrier Board");
}

Device *ModBbisD201::create(bool withSubDevs)
{
	withSubDevs = withSubDevs;
	return new ModBbisD201();
}


// -----------------------------------------------------------------

//! Create a D302 BBIS device
ModBbisD302::ModBbisD302( bool withSubDevs ) :
	ModBbisPciMmod(QString("D302"),QString("D302"),QString("D302"),Cpci)
{
	setDescription("CPCI 6U 128 Binary I/Os Board");

	UTIL_UNREF_PARAM(withSubDevs);

	// create slots
	for( int slotNum=0; slotNum<getSlots(); slotNum++ ){
		BbisSlot *bbSlot;

		bbSlot = new BbisSlot( slotNum, MmodD32, MmodA24 );
		bbSlot->setBbSlotSpecRequired( true );
		addChild( bbSlot );
	}
}

Device *ModBbisD302::create( bool withSubDevs )
{
	return new ModBbisD302( withSubDevs );
}


// -----------------------------------------------------------------

//! Create a C203 BBIS device
ModBbisC203::ModBbisC203() :
	ModBbisPciMmod(QString("C203"),QString("C203"),QString("C203"),StdPci)
{
	createSlots();
	setDescription("Standard PCI Three Slot M-Module Carrier Board");
}

Device *ModBbisC203::create(bool withSubDevs)
{
	withSubDevs = withSubDevs;
	return new ModBbisC203();
}

SwModuleList *
ModBbisC203::enumSwModules()
{
	if( lstSwModules == 0 ){
		lstSwModules = new SwModuleList;

		lstSwModules->add( makeStdDriverSwModule("D201", "C203" ));
	}
	return lstSwModules;
}

// -----------------------------------------------------------------

//! Create a C204 BBIS device
ModBbisC204::ModBbisC204() :
	ModBbisPciMmod(QString("C204"),QString("C204"),QString("C204"),StdPci)
{
	createSlots();
	setDescription("Standard PCI Single Slot M-Module Carrier Board");
}

Device *ModBbisC204::create(bool withSubDevs)
{
	withSubDevs = withSubDevs;
	return new ModBbisC204();
}

SwModuleList *
ModBbisC204::enumSwModules()
{
	if( lstSwModules == 0 ){
		lstSwModules = new SwModuleList;

		lstSwModules->add( makeStdDriverSwModule("D201", "C204" ));
	}
	return lstSwModules;
}
// -----------------------------------------------------------------

//! Create a F201 BBIS device
ModBbisF201::ModBbisF201() :
	ModBbisPciMmod(QString("F201"),QString("F201"),QString("F201"),Cpci)
{
	createSlots();
	setDescription("CPCI 3U Single Slot M-Module Carrier Board");
}

Device *ModBbisF201::create(bool withSubDevs)
{
	withSubDevs = withSubDevs;
	return new ModBbisF201();
}

SwModuleList *
ModBbisF201::enumSwModules()
{
	if( lstSwModules == 0 ){
		lstSwModules = new SwModuleList;

		lstSwModules->add( makeStdDriverSwModule("D201", "F201" ));
	}
	return lstSwModules;
}
// -----------------------------------------------------------------

//! Create a F202 BBIS device
ModBbisF202::ModBbisF202() :
	ModBbisPciMmod(QString("F202"),QString("F202"),QString("F202"),Cpci)
{
	createSlots();
	setDescription("CPCI 3U Dual Slot M-Module Carrier Board");
}

Device *ModBbisF202::create(bool withSubDevs)
{
	withSubDevs = withSubDevs;
	return new ModBbisF202();
}

SwModuleList *
ModBbisF202::enumSwModules()
{
	if( lstSwModules == 0 ){
		lstSwModules = new SwModuleList;

		lstSwModules->add( makeStdDriverSwModule("D201", "F202" ));
	}
	return lstSwModules;
}


