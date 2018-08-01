/***************************************************************************
 */
/*!       \file  modbbisa203n.cpp 
 *       \brief  Implementation of A203N family BBIS devices
 *      \author	 dieter.pfeuffer@men.de
 *		  $Date: 2011/09/26 08:42:44 $
 *    $Revision: 2.1 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbisa203n.cpp,v $
 * Revision 2.1  2011/09/26 08:42:44  dpfeuffer
 * R: cosmetics
 * M: cosmetics
 *
 * Revision 2.0  2010/01/22 11:28:41  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.1  2005/08/02 11:20:41  dpfeuffer
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <qmessagebox.h>
#include <q3memarray.h>

#include <Q3MemArray>

#include "wizdebug.h"
#include "modbbisa203n.h"
#include "modbbisa203nproptab.h"
#include "configuration.h"
#include "resourcemng.h"
#include "qcursor.h"
#include "util.h"


// -----------------------------------------------------------------
// BBIS A203N Family

//! Create an A203N family BBIS device
/*!
  The device will have the default properties set by createProperties().
  
  It will have no BBIS slots (must be created by constructor of subclasses
  by calling createSlots()).
  
  \param instName initial instance name of device
  \param hwName	Hardware name (e.g. "B202")
  \param driverName (e.g. "b202")
  \todo instName really needed?
*/
ModBbisA203NFamily::ModBbisA203NFamily( 
	QString instName, 
	QString hwName,
	QString driverName) :
	BbisDevice( hwName, driverName, true, Vme )
{
	if( prop ) delete prop;	// delete Device::prop
	prop = createProperties();
	setInstName( instName );

	wDebug(("ModBbisA203NFamily: vmeAddr=%lx", 
		   ((ModBbisA203NProperties *)prop)->vmeAddr ));
}


//! destructs ModBbisA203NFamily instance
ModBbisA203NFamily::~ModBbisA203NFamily()
{
  wDebug(("~ModBbisA203NFamily()"));
	freeResources();
	delete prop;
	prop = 0;
}

//! create the required BbisSlot instances
/*!
  This functions calls getSlots(), which may be reimplemented 
  by subclasses to get the number of slots on the board.
*/
void
ModBbisA203NFamily::createSlots()
{
	BbisSlot *bbSlot;

	wDebug(("ModBbisA203NFamily::createSlots getSlots: %d", getSlots()));
	for( int slotNum=0; slotNum<getSlots(); slotNum++ ){
		bbSlot = new BbisSlot( slotNum, MmodD16, MmodA08 );
		addChild( bbSlot );
	}
}

DevPropertiesDlg *
ModBbisA203NFamily::createPropDlg( QWidget *parent )
{
	DevPropertiesDlg *dp;
	QWidget *defTab;

	dp = new DevPropertiesDlg( parent, this );
	dp->addTab( (defTab = new ModBbisA203NPropTab(getSlots(), parent, dp)), 
				   "VME Settings");
	dp->setButtons();
	dp->showPage( defTab );

	wDebug(("ModBbisA203NFamily::createPropDlg dp=%p", dp ));
	return dp;
}

DevPropertiesDlg *
ModBbisA203NFamily::createPropDlg4Win( QWidget *parent )
{
	DevPropertiesDlg *dp;
	QWidget *defTab;

	dp = new DevPropertiesDlg( parent, this, FALSE );
	dp->addTab( (defTab = new ModBbisA203NPropTab(getSlots(), parent, dp)), 
				   "VME Settings");
	
	dp->setOkButton();
	dp->setCancelButton();

	// use current mouse position
	dp->move( QCursor::pos() );

	dp->showPage( defTab );

	wDebug(("ModBbisA203NFamily::createPropDlg dp=%p", dp ));
	return dp;
}

DeviceProperties *
ModBbisA203NFamily::createProperties()
{
	ModBbisA203NProperties *p = new ModBbisA203NProperties();

	p->dbgSettings.setModules( 	
		(1L<<DebugProperties::Driver) |
		(1L<<DebugProperties::BbisKernel) |
		(1L<<DebugProperties::Oss) |
		(1L<<DebugProperties::Desc));

	// set defaults
	p->vmeAddr = 0xe00000;
	p->vmeA32Addr = 0x0;
	p->isA16Space = false;
	p->a32Enabled = false;

	for(int i=0; i<4; i++){
		p->irqLevels[i] = 3;
#ifdef WIN_MDISWIZ_LIB
		p->forceFound[i] = 0;
#endif
	}
	p->irqVectors[0] = 128;
	p->irqVectors[1] = 129;
	p->irqVectors[2] = 130;
	p->irqVectors[3] = 131;

	return p;
}

void 
ModBbisA203NFamily::copyProperties( 
	const DeviceProperties *from,
	DeviceProperties *to )
{
	const ModBbisA203NProperties *_from;
	ModBbisA203NProperties *_to;

	WIZ_DYNAMIC_CAST( to, _to, ModBbisA203NProperties * );
	WIZ_DYNAMIC_CAST( from, _from, const ModBbisA203NProperties * );

	*_to = *_from;
	wDebug(("ModBbisA203NFamily::copyProperties: %p->%p vme=%lx", _from, _to, _from->vmeAddr ));
}


ResourceMng *
ModBbisA203NFamily::getVmeAddRes( bool _isA16Space ) 
{
	VmeBusInterface *busIf = (VmeBusInterface *)getParent();

	if( !busIf )
		return 0;
	return _isA16Space ? busIf->vmeA16Res : busIf->vmeA24Res;
}
	
void
ModBbisA203NFamily::freeResources()
{
	VmeBusInterface *busIf = (VmeBusInterface *)getParent();
	if( busIf == 0 ) return;
	
	wDebug(("ModBbisA203NFamily::freeResources"));
	busIf->vmeA16Res->freeAllOfOwner( this );
	busIf->vmeA24Res->freeAllOfOwner( this );
	busIf->vmeIrqVectorRes->freeAllOfOwner( this );

	Device::freeResources();
}

Device::ApplyResult 
ModBbisA203NFamily::applyProperties( DeviceProperties *newProp, 
									QString &errMsg )
{
	if( newProp == 0 )
		newProp = prop;

	ModBbisA203NProperties *_prop;
	ModBbisA203NProperties *_propAct;

	WIZ_DYNAMIC_CAST( prop, _propAct, ModBbisA203NProperties * );
	WIZ_DYNAMIC_CAST( newProp, _prop, ModBbisA203NProperties * );

	ApplyResult ar;

	VmeBusInterface *busIf = (VmeBusInterface *)getParent();
	QString msg, msg2;
	ulong s;
	ResourceMng::IsAvailResult isAvail;
	ResourceMng::AllocResult alr = ResourceMng::AllocResFail;
	ResourceMng *vmeSpc;
	HwComponent *owner;

	wDebug(("ModBbisA203NFamily::applyProperties"));

	if( !busIf )
#ifdef WIN_MDISWIZ_LIB
		return Device::ApplyPropOk;
#else
		return Device::ApplyPropFail;
#endif

	// check if new properties are available / allocate 
	vmeSpc = getVmeAddRes(_prop->isA16Space);
	if( !vmeSpc )
		isAvail = ResourceMng::Unavail;
	else
		isAvail = vmeSpc->isAvail( 
			_prop->vmeAddr,
			getVmeSizeAndAlignment(),
			&owner );

	if( isAvail != ResourceMng::Avail ){
		msg.sprintf("Cannot use VME address 0x%lx in %s space\n",
					_prop->vmeAddr, _prop->isA16Space ? "A16":"A24" );
		
		if( isAvail == ResourceMng::Unavail )
			msg2.sprintf("Address not supported by VMEbus interface\n");
		else
			msg2.sprintf("Already used by device %s\n", 
						 (const char *)owner->getInstName() );
		msg += msg2;
		errMsg += msg;
		
		if( vmeSpc )
			alr = vmeSpc->alloc( getVmeSizeAndAlignment(),
							 getVmeSizeAndAlignment(),
							 s,
							 this);
	}
	else {
		if( vmeSpc )
			alr = vmeSpc->alloc( getVmeSizeAndAlignment(),
								 getVmeSizeAndAlignment(),
								 s,
								 this,
								 _prop->vmeAddr );
	}
	if( alr == ResourceMng::AllocResFail ){
		msg.sprintf("Could not allocate any VME address!\n");
		errMsg += msg;
		goto abort;
	}
	else if( s != _prop->vmeAddr ){
		msg.sprintf("Alternative address 0x%lx allocated\n", s );
		_prop->vmeAddr = s;
		errMsg += msg;
		ar = ApplyPropAlternate;
	}
	if( vmeSpc )
		vmeSpc->dump();
	
	//??? IRQ Levels
	int i;
	for( i=0; i<getSlots(); i++ ){
		isAvail = busIf->vmeIrqVectorRes->isAvail( _prop->irqVectors[i],
											 	   1,
											 	   &owner );
		if( isAvail != ResourceMng::Avail ){
			msg.sprintf("Slot %d: Cannot use IRQ vector %d\n",
						i, _prop->irqVectors[i] );
			if( isAvail == ResourceMng::Unavail )
				msg2.sprintf("Vector not supported by VMEbus interface\n");
			else
				msg2.sprintf("Already used by device %s\n", 
							 (const char *)owner->getInstName() );
			msg += msg2;
			errMsg += msg;
			alr = busIf->vmeIrqVectorRes->alloc( 1,
											    1,
											    s,
											    this);
		}
		else {
			alr = busIf->vmeIrqVectorRes->alloc( 1,
											    1,
											    s,
											    this,
											    _prop->irqVectors[i]);
		}
		if( alr == ResourceMng::AllocResFail ){
			msg.sprintf("Slot %d: Could not allocate any VME vector!\n", i);
			errMsg += msg;
			goto abort;
		}
		else if( (int)s != _prop->irqVectors[i] ){
			msg.sprintf("Slot %d: Alternative vector %ld allocated\n", i, s );
			_prop->irqVectors[i] = s;
			errMsg += msg;
			ar = ApplyPropAlternate;
		}
	}

	// allocate name	
	msg = QString::null;
	ar = Device::applyProperties( _prop, msg );
	errMsg += msg;
	if( ar == ApplyPropFail )
		goto abort;

	wDebug(("ModBbisA203NFamily::applyProperties errMsg=%s",
		   (const char *)errMsg));

	return ar;

 abort:	
	freeResources();	
	wDebug(("ModBbisA203NFamily::applyProperties failed errMsg=%s",
		   (const char *)errMsg));
	return ApplyPropFail;
}

Device::ApplyResult
ModBbisA203NFamily::applyProperties( DescriptorEntryDirectory *devDesc, 
									QString &errMsg )
{
	DescriptorEntry *e, *eNext;
	DescriptorEntryUint32 *eUint32;
	DescriptorEntryBinary *eBinary;
	ModBbisA203NProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisA203NProperties * );

	_prop->a32Enabled = false;

	if( !devDesc ) return ApplyPropFail;

	// scan all descriptor entries
	eNext=devDesc->firstChild();

	// ??? should do a little more sanity checking...
	while( (e=eNext) ){
		bool entryHandled = true;
		QString key = e->path().section("/",1);
		
		wDebug(("key=%s", (const char *)key ));

		if( key == "VME16_ADDR_MMOD08" ){
			_prop->isA16Space = true;
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->vmeAddr = eUint32->getValue();
		}
		else if( key == "VME24_ADDR_MMOD08" ){
			_prop->isA16Space = false;
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->vmeAddr = eUint32->getValue();			
		}
		else if( key == "VME32_ADDR_MMOD24" ){
			_prop->a32Enabled = true;
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->vmeA32Addr = eUint32->getValue();			
		}
		else if( key == "IRQ_VECTOR" ){
			WIZ_DYNAMIC_CAST( e, eBinary, DescriptorEntryBinary *);

			for(int i=0; i<getSlots() && 
					i<(int)eBinary->getValue().count(); i++) 
				_prop->irqVectors[i] = eBinary->getValue()[i];			
		}
		else if( key == "IRQ_LEVEL" ){
			WIZ_DYNAMIC_CAST( e, eBinary, DescriptorEntryBinary *);

			for(int i=0; i<getSlots() && 
					i<(int)eBinary->getValue().count(); i++) 
				_prop->irqLevels[i] = eBinary->getValue()[i];			
		}
#ifdef WIN_MDISWIZ_LIB
		else if( key == "PNP_FORCE_FOUND" ){
			WIZ_DYNAMIC_CAST( e, eBinary, DescriptorEntryBinary *);

			for(int i=0; i<getSlots() && 
					i<(int)eBinary->getValue().count(); i++) 
				_prop->forceFound[i] = eBinary->getValue()[i];			
		}
#endif
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
ModBbisA203NFamily::getMainProperties()
{
	QString rv;

	ModBbisA203NProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisA203NProperties * );
	WIZ_ASSERT( getParent() );

	rv = getParent()->getMainProperties();

	if( _prop->isA16Space )
		rv += " A16";
	else
		rv += " A24";

	rv += QString(" %1").arg(_prop->vmeAddr,0,16);

	return rv;
}

//
// This is called by MdisDevice::createDesc 
// to fill device specific descriptor entries
void
ModBbisA203NFamily::createSpecialDesc( DescriptorEntryDirectory *parentDesc )
{
	int i;
	ModBbisA203NProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisA203NProperties * );

	DescriptorFactory dFact;
	Q3MemArray<uchar> arr(getSlots());

	wDebug(("ModBbisA203NFamily::createSpecialDesc ns=%d", getSlots()));

	if( _prop->isA16Space )
		parentDesc->addChild( dFact.create("VME16_ADDR_MMOD08", _prop->vmeAddr ));
	else
		parentDesc->addChild( dFact.create("VME24_ADDR_MMOD08", _prop->vmeAddr ));

	if( _prop->a32Enabled )
		parentDesc->addChild( dFact.create("VME32_ADDR_MMOD24", _prop->vmeA32Addr ));

	for(i=0; i<getSlots(); i++)
		arr[i] = _prop->irqVectors[i];
	parentDesc->addChild( dFact.create("IRQ_VECTOR", arr ));

	for(i=0; i<getSlots(); i++) 
		arr[i] = _prop->irqLevels[i];
	parentDesc->addChild( dFact.create("IRQ_LEVEL", arr ));

#ifdef WIN_MDISWIZ_LIB
	for(i=0; i<getSlots(); i++) 
		arr[i] = _prop->forceFound[i];
	parentDesc->addChild( dFact.create("PNP_FORCE_FOUND", arr ));
#endif

}

// -----------------------------------------------------------------

//! Create a A203N BBIS device
ModBbisA203N::ModBbisA203N() : 
	ModBbisA203NFamily(QString("A203N"),QString("A203N"),QString("A203N"))
{
	createSlots();
	setDescription("VME64 6U Four Slot M-Module carrier board");	
}

Device *ModBbisA203N::create(bool withSubDevs)
{
	UTIL_UNREF_PARAM(withSubDevs);
	return new ModBbisA203N();
}



