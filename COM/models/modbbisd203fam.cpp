/***************************************************************************
 */
/*!       \file  modbbisd203fam.cpp
 *       \brief  Implementation of D203 and related BBIS devices
 *      \author	 Christian.Schuster@men.de
 *		  $Date: 2014/10/22 14:12:16 $
 *    $Revision: 2.5 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbisd203fam.cpp,v $
 * Revision 2.5  2014/10/22 14:12:16  dpfeuffer
 * R: gray PCI Bus Number is 15 for G204 M-Module Carrier
 * M: special handling in ModBbisD203Family::ModBbisD203Family for G204 to
 *    set 0 instead of 15
 *
 * Revision 2.4  2014/08/22 15:56:19  dpfeuffer
 * R: improvement for PCIe
 * M: usage of getIsPciE()
 *
 * Revision 2.3  2013/09/27 08:19:17  dpfeuffer
 * R: G204 support
 * M: modifications for G204 in ModBbisD203Family
 *
 * Revision 2.2  2011/04/01 09:35:37  CRuff
 * R: 1. pci bus interfaces may have a pci domain
 *    2. pci domain is always stored in key PCI_DOMAIN_NUMBER, not in bus path
 * M: 1. in applyProperties(), takeover pci domain from bus interface
 *    2. removed code to handle PCI_BUS_PATH2
 *
 * Revision 2.1  2010/07/05 18:04:59  CRuff
 * R: implementation of PCI Domains
 * M: handle PCI_DOMAIN_NR / PCI_BUS_PATH2 in createSpecialDesc and
 *    applyProperties
 *
 * Revision 2.0  2010/01/22 11:28:55  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.4  2007/02/23 13:16:30  DPfeuffer
 * cosmetics
 *
 * Revision 1.3  2007/02/23 12:20:34  DPfeuffer
 * ModBbisD203Family::createPropDlg4Win() implemented
 *
 * Revision 1.2  2006/12/20 12:03:36  cs
 * fixed:
 *    - HW_TYPE for _A24 models
 *
 * Revision 1.1  2006/11/30 11:28:15  cs
 * Initial Revision
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2006-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#include <Q3MemArray>

#include "modbbispcimmod.h"
#include "modbbispcimmodproptab.h"
#include "modbbisd203fam.h"
#include "modbbisd203famproptab.h"
#include "wizdebug.h"
#include "configuration.h"
#include "resourcemng.h"


// -----------------------------------------------------------------
// BBIS D203 Family

//! Create an D203 family BBIS device
/*!
  The device will have the default properties set by createProperties().

  It will have no BBIS slots (must be created by constructor of subclasses
  by calling createSlots()).

  \param instName initial instance name of device
  \param hwName	Hardware name (e.g. "D203")
  \param driverName (e.g. "d203")
  \todo instName really needed?
*/
ModBbisD203Family::ModBbisD203Family( ModBbisD203Family::Model model, BusIfType ifType ) :
	ModBbisPciMmod( QString::null, QString::null, QString("D203"), ifType)
{
	QString descr, hwName;

	_model = model;
	_numSlots = 0;

	switch( _model ){
	case D203:
	case D203_A24:
		_numSlots = 4;
		hwName = "D203";
		descr = "CPCI 6U Four Slot M-Module Carrier Board";
		break;
	case F204:
	case F204_A24:
		_numSlots = 1;
		hwName = "F204";
		descr = "CPCI 3U Single Slot M-Module Carrier Board";
		break;
	case F205:
	case F205_A24:
		_numSlots = 2;
		hwName = "F205";
		descr = "CPCI 3U Dual Slot M-Module Carrier Board";
		break;
	case G204:
	case G204_A24:
		_numSlots = 1;
		hwName = "G204";
		descr = "CPCI Serial 3U Single Slot M-Module Carrier Board";
		break;
	}
	if( _model >= D203_A24 ) {
		hwName     += "_A24";
		driverName += "_A24";
		descr  += " with A24 MM bus support";
	}

	setHwName( hwName );
	setDescription(descr);

	if( prop ) delete prop;	// delete ModBbisPciMmod::prop
	prop = createProperties();

	switch( _model ){
		case G204:
		case G204_A24:
			ModBbisD203FamProperties *_prop;
			WIZ_DYNAMIC_CAST(prop,_prop,ModBbisD203FamProperties*);
			_prop->pciDevNo = 0;
		break;
	}

	createSlots();
}

//! destructs ModBbisD203Family
ModBbisD203Family::~ModBbisD203Family()
{
	wDebug(("~ModBbisD203Family()"));
	if( prop ) delete prop;
	prop = 0;
}

Device *ModBbisD203Family::create(bool withSubDevs)
{
	withSubDevs = withSubDevs;
	return new ModBbisD203Family( _model, ifType );
}

void ModBbisD203Family::createSlots()
{
	BbisSlot *bbSlot;
	MmodAddrCaps addrCap = MmodA08;

	if( _model >= D203_A24 ) {
		addrCap = MmodA24;
	}

	for( int slotNum=0; slotNum<getSlots(); slotNum++ ){
		bbSlot = new BbisSlot( slotNum, MmodD32, addrCap );
		addChild( bbSlot );
	}
}

SwModuleList *
ModBbisD203Family::enumSwModules()
{
	QString variant = "";

	if( _model >= D203_A24 ) {
		variant = QString("A24");
	}

	if( lstSwModules == 0 ){
		lstSwModules = new SwModuleList;
	}

	lstSwModules->add( makeStdDriverSwModule("D203", variant ));
	return lstSwModules;
}

DevPropertiesDlg *
ModBbisD203Family::createPropDlg( QWidget *parent )
{
	DevPropertiesDlg *dp;
	QWidget *tabCpci, *tabD203;

	dp = new DevPropertiesDlg( parent, this );

	// PCIe devices with known PCI bus paths requires no further location settings,
	// don't show properties dlg
	PciBusInterface *busIf;
	WIZ_DYNAMIC_CAST( getParent(), busIf, PciBusInterface * );
	if( (busIf->getIsPciE() && !busIf->hasPciBusPath()) || !busIf->getIsPciE() ){
	    dp->addTab( (tabCpci = new ModBbisPciMmodPropTab(parent, dp)),
	                QString("%1 Settings").arg( busIfTypeMap.keyToStr(
	                    getBusIfType() )));
	}

	dp->addTab( (tabD203 = new ModBbisD203FamPropTab(getSlots(), parent, dp)),
				QString("Trigger Line Settings") );

	dp->setButtons();
    if ( tabCpci )
    	dp->showPage( tabCpci );

	wDebug(("ModBbisD203Fam::createPropDlg dp=%p", dp ));
	return dp;
}

DevPropertiesDlg *
ModBbisD203Family::createPropDlg4Win( QWidget *parent )
{
	DevPropertiesDlg *dp;
	QWidget *tabD203;

	dp = new DevPropertiesDlg( parent, this, FALSE );

	dp->addTab( (tabD203 = new ModBbisD203FamPropTab(getSlots(), parent, dp)),
				QString("Trigger Line Settings") );


	dp->setOkButton();
	dp->setCancelButton();

	wDebug(("ModBbisD203Fam::createPropDlg dp=%p", dp ));
	return dp;
}

DeviceProperties *
ModBbisD203Family::createProperties()
{
	ModBbisD203FamProperties *p = new ModBbisD203FamProperties();

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

	p->pxiTrigSrc = 0x00;
	p->pxiTrigDst = 0x00;
	for(int i = 0; i < _numSlots; i++){
		p->trigASrc[i]= 0x00;
		p->trigADst[i]= 0x00;
		p->trigBSrc[i]= 0x00;
		p->trigBDst[i]= 0x00;
	}

	wDebug(("ModBbisD203Fam::createProperties p=%p", p ));
	return p;
}

void
ModBbisD203Family::copyProperties(
	const DeviceProperties *from,
	DeviceProperties *to )
{
	const ModBbisD203FamProperties *_from;
	ModBbisD203FamProperties *_to;
	wDebug(("ModBbisD203Fam::copyProperties from=%p to=%p", from, to ));

	WIZ_DYNAMIC_CAST( to, _to, ModBbisD203FamProperties * );
	WIZ_DYNAMIC_CAST( from, _from, const ModBbisD203FamProperties * );

	*_to = *_from;
}

Device::ApplyResult
ModBbisD203Family::applyProperties( DeviceProperties *newProp,
									QString &errMsg )
{
	if( newProp == 0 )
		newProp = prop;

	ModBbisD203FamProperties *_prop;

	WIZ_DYNAMIC_CAST( newProp, _prop, ModBbisD203FamProperties * );

	ApplyResult ar;

	PciBusInterface *busIf = (PciBusInterface *)getParent();
	QString msg;
	ulong s;
	ResourceMng::IsAvailResult isAvail;
	ResourceMng::AllocResult alr = ResourceMng::AllocResOk;
	HwComponent *owner;

	wDebug(("ModBbisD203Family::applyProperties"));

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

	// check trigger lines
	msg = QString::null;
	{
		int dblMask = (_prop->pxiTrigSrc & _prop->pxiTrigDst);

		if( dblMask ){
			msg.sprintf("PXI trigger lines (mask = 0x%02x) set to source and destination (ignored)\n",
						 dblMask );
			errMsg += msg;
			_prop->pxiTrigSrc &= ~dblMask;
			_prop->pxiTrigDst &= ~dblMask;
		}
		for( int slt = 0; slt <getSlots(); slt++ ){
			dblMask = (_prop->trigASrc[slt] & _prop->trigADst[slt]);
			if( dblMask ){
				msg.sprintf("MM Slot %d TrigA lines (mask = 0x%02x) set to source and destination (ignored)\n",
							 slt, dblMask );
				errMsg += msg;
				_prop->trigASrc[slt] &= ~dblMask;
				_prop->trigADst[slt] &= ~dblMask;
			}
			dblMask = (_prop->trigBSrc[slt] & _prop->trigBDst[slt]);
			if( dblMask ){
				msg.sprintf("MM Slot %d TrigB lines (mask = 0x%02x) set to source and destination (ignored)\n",
							 slt, dblMask );
				errMsg += msg;
				_prop->trigBSrc[slt] &= ~dblMask;
				_prop->trigBDst[slt] &= ~dblMask;
			}
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

// get slot from descriptor directory
/*
  \param e descriptor entry
  \return slot or -1 if invalid
*/
int
ModBbisD203Family::getDescMmSlot( DescriptorEntry *e )
{
	DescriptorEntryDirectory *par = e->getParent();
	QString key;
	int rv;


	if( !par )
	{
		rv = -1;
		goto ERR_EXIT;
	}

	key = par->path().section("/",1);

	// interprete key string
	if( key == "MSLOT_0" ){
		rv = 0;
	} else if( key == "MSLOT_1" ){
		rv = 1;
	} else if( key == "MSLOT_2" ){
		rv = 2;
	} else if( key == "MSLOT_3" ){
		rv = 3;
	} else {
		rv = -1;
	}

	// slot in range?
	if( rv >= getSlots() ){
		rv = 0;
	}

ERR_EXIT:
	wDebug(("ModBbisD203Family::getDescMmSlot slt = %d", rv));
	return rv;
}

Device::ApplyResult
ModBbisD203Family::applyProperties( DescriptorEntryDirectory *devDesc,
									QString &errMsg )
{
	DescriptorEntry *e, *eNext;
	DescriptorEntryUint32 *eUint32;
	DescriptorEntryBinary *eBinary;
	ModBbisD203FamProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisD203FamProperties * );

	wDebug(("ModBbisD203Family::applyProperties( devDesc )"));

	if( !devDesc ) return ApplyPropFail;

	_prop->idCheck = -1;

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
		else if ( key == "PCI_DOMAIN_NUMBER" ) {
        	WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
        	_prop->pciDomainNo = eUint32->getValue();
        }
		else if( key == "PCI_DEVICE_ID" ){
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->pciDevNo = eUint32->getValue();
		}
		else if( key == "PCI_BUS_PATH" ) {
        	Q3MemArray<uchar> parentBusPath;
        	Q3MemArray<uchar> busPath;
            PciBusInterface *busIf = (PciBusInterface *)getParent();
            WIZ_DYNAMIC_CAST( e, eBinary, DescriptorEntryBinary *);
            
            if( busIf && busIf->hasPciBusPath() ){
            	// get path from parent (for comparison)
                busIf->pciBusPath( -1, &parentBusPath );
	      		
	      		// get bus path from descriptor
	      		busPath = eBinary->getValue();

	      		for (unsigned int i=0; i<parentBusPath.count(); i++) {
	      			wDebug(("parentPath[%d] = %d",i,parentBusPath[i]));
	      		}
	      		for (unsigned int i=0; i<busPath.count(); i++) {
	      			wDebug(("busPath[%d] = %d",i,busPath[i]));
	      		}
	      		// compare with parent path
	      		if( busPath != parentBusPath ){
                    errMsg += "PCI_BUS_PATH descriptor/device discrepancy!\n";
                }
            }
        }
		else if( key == "ID_CHECK" ){
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->idCheck = eUint32->getValue();
		}
		else if( key == "PXI_TRIG_SRC" ){
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->pxiTrigSrc = eUint32->getValue() & 0xFF;
		}
		else if( key == "PXI_TRIG_DST" ){
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->pxiTrigDst = eUint32->getValue() & 0xFF;
		}
		else if( strstr(key, "TRIGA_SRC") ){
			int slt = getDescMmSlot( e );
			if( slt >= 0 ){
				WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
				_prop->trigASrc[slt] = eUint32->getValue() & 0xFF;
			}
		}
		else if( strstr(key, "TRIGA_DST") ){
			int slt = getDescMmSlot( e );
			if( slt >= 0 ){
				WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
				_prop->trigADst[slt] = eUint32->getValue() & 0xFF;
			}
		}
		else if( strstr(key, "TRIGB_SRC") ){
			int slt = getDescMmSlot( e );
			if( slt >= 0 ){
				WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
				_prop->trigBSrc[slt] = eUint32->getValue() & 0xFF;
			}
		}
		else if( strstr(key, "TRIGB_DST") ){
			int slt = getDescMmSlot( e );
			if( slt >= 0 ){
				WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
				_prop->trigBDst[slt] = eUint32->getValue() & 0xFF;
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

//
// This is called by MdisDevice::createDesc
// to fill device specific descriptor entries
void
ModBbisD203Family::createSpecialDesc( DescriptorEntryDirectory *parentDesc )
{
	ModBbisD203FamProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisD203FamProperties * );
	PciBusInterface *busIf = (PciBusInterface *)getParent();
	DescriptorFactory dFact;
	DescriptorEntryDirectory *newDir;
	QString helpTxt;
	wDebug(("ModBbisD203Family::createSpecialDesc"));

	helpTxt="------------------------------------------------------------------------\n"\
			"       device parameters\n"												\
			"------------------------------------------------------------------------\n"\
			"\n"
			"--- C-PCI parameters\n";
	if( busIf && busIf->hasPciBusPath() ){

		// PCI_BUS_PATH
		Q3MemArray<uchar> busPath;
		busIf->pciBusPath( -1, &busPath );

		//parentDesc->addChild( dFact.create("PCI_BUS_PATH", busPath, helpTxt ) );
		parentDesc->addChild( dFact.create("PCI_BUS_NUMBER", _prop->pciBusNo, helpTxt ) );

		// PCI device number (=PCI_DEVICE_ID) of PCIe endpoints is always 0
		if( busIf->getIsPciE() ){
			// PCI_DEVICE_ID
			parentDesc->addChild( dFact.create("PCI_DEVICE_ID", 0));		
		}
		else{
			// PCI_BUS_SLOT
			//parentDesc->addChild( dFact.create("PCI_BUS_SLOT", _prop->slotNo ));
			parentDesc->addChild( dFact.create("PCI_DEVICE_NUMBER", _prop->pciDevNo ));
		}

		// PCI_DEVICE_ID (name is missleading...)
		//
		// This forces the D201 driver to ignore OSS_PciSlotToPciDevice...
		//busIf->pciBusPath( _prop->slotNo, &busPath );
		//if( busPath.size() > 0 ){
		//	int ix = busPath.size() -1;
		//	parentDesc->addChild( dFact.create("PCI_DEVICE_ID", busPath[ix] ));
		//}
		parentDesc->addChild( dFact.create("PCI_DEVICE_ID", _prop->pciDevNo ));
	}
	else {
		// PCI_BUS_NUMBER
		parentDesc->addChild( dFact.create("PCI_BUS_NUMBER", _prop->pciBusNo, helpTxt));

		// PCI_DEVICE_ID (name is missleading...)
		parentDesc->addChild( dFact.create("PCI_DEVICE_ID", _prop->pciDevNo));
	}

	if (_prop->pciDomainNo != 0 ) {
	    parentDesc->addChild( dFact.create("PCI_DOMAIN_NUMBER", _prop->pciDomainNo ));
	}
	        	
	// PXI Trigger Settings
	helpTxt="--- Global Trigger Settings (see HW manual)\n";
	bool dEntActive =  (_prop->pxiTrigSrc | _prop->pxiTrigDst ) ? true : false;

	parentDesc->addChild( dFact.create("PXI_TRIG_SRC", _prop->pxiTrigSrc, helpTxt, dEntActive));
	parentDesc->addChild( dFact.create("PXI_TRIG_DST", _prop->pxiTrigDst, QString::null, dEntActive));

	// M-Module specific Trigger Settings
	bool slotHelpPrinted = false;
	for( int slt = 0; slt < getSlots(); slt++ ) {
		QString subDirName = QString("MSLOT_%1").arg(slt);
		if( !slotHelpPrinted &&
			(_prop->trigASrc[slt] | _prop->trigADst[slt] | _prop->trigBSrc[slt] | _prop->trigBDst[slt])  ){
			slotHelpPrinted = true;
			helpTxt = "--- M-Module specific Trigger Settings (see HW manual)\n";
		} else {
			helpTxt = QString::null;
		}


		newDir = new DescriptorEntryDirectory( subDirName, helpTxt );
		newDir->printEndHelp( false );

		parentDesc->addChild( newDir );

		newDir->addChild( dFact.create( "TRIGA_SRC", _prop->trigASrc[slt],
										QString::null,	_prop->trigASrc[slt] ? true : false ) );
		newDir->addChild( dFact.create( "TRIGA_DST", _prop->trigADst[slt],
										QString::null,	_prop->trigADst[slt] ? true : false ) );
		newDir->addChild( dFact.create( "TRIGB_SRC", _prop->trigBSrc[slt],
										QString::null,	_prop->trigBSrc[slt] ? true : false ) );
		newDir->addChild( dFact.create( "TRIGB_DST", _prop->trigBDst[slt],
										QString::null,	_prop->trigBDst[slt] ? true : false ) );
	}
}






