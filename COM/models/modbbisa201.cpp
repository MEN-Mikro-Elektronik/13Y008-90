/***************************************************************************
 */
/*!       \file  modbbisa201.cpp
 *       \brief  Implementation of A201 family BBIS devices
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:28:31 $
 *    $Revision: 2.0 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbisa201.cpp,v $
 * Revision 2.0  2010/01/22 11:28:31  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.9  2007/10/05 16:18:50  cs
 * fixed:
 *   - driver name for B202 to A201
 *
 * Revision 1.8  2004/10/07 11:13:32  dpfeuffer
 * - modified for windows "Force Found" option
 * - ModBbisA201Family() for B201/B202 changed
 *
 * Revision 1.7  2004/06/08 17:25:54  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.6  2004/01/09 08:20:44  kp
 * intermediate checkin
 *
 * Revision 1.5  2003/06/11 12:21:47  kp
 * intermediate checkin
 *
 * Revision 1.4  2003/06/05 11:59:23  dschmidt
 * intermediate checkin
 *
 * Revision 1.3  2003/06/05 09:35:00  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/06/03 10:17:43  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:33:07  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <qmessagebox.h>

#include <Q3MemArray>

#include "wizdebug.h"
#include "modbbisa201.h"
#include "modbbisa201proptab.h"
#include "configuration.h"
#include "resourcemng.h"
#include "qcursor.h"


// -----------------------------------------------------------------
// BBIS A201 Family

//! Create an A201 family BBIS device
/*!
  The device will have the default properties set by createProperties().

  It will have no BBIS slots (must be created by constructor of subclasses
  by calling createSlots()).

  \param instName initial instance name of device
  \param hwName	Hardware name (e.g. "B202")
  \param driverName (e.g. "b202")
  \todo instName really needed?
*/
ModBbisA201Family::ModBbisA201Family(
	QString instName,
	QString hwName,
	QString driverName) :
	BbisDevice( hwName, driverName, true, Vme )
{
	if( prop ) delete prop;	// delete Device::prop
	prop = createProperties();
	setInstName( instName );

	wDebug(("ModBbisA201Family: vmeAddr=%lx",
		   ((ModBbisA201Properties *)prop)->vmeAddr ));
}


//! destructs ModBbisA201Family instance
ModBbisA201Family::~ModBbisA201Family()
{
  wDebug(("~ModBbisA201Family()"));
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
ModBbisA201Family::createSlots()
{
	BbisSlot *bbSlot;

	wDebug(("ModBbisA201Family::createSlots getSlots: %d", getSlots()));
	for( int slotNum=0; slotNum<getSlots(); slotNum++ ){
		bbSlot = new BbisSlot( slotNum, MmodD16, MmodA08 );
		addChild( bbSlot );
	}
}

DevPropertiesDlg *
ModBbisA201Family::createPropDlg( QWidget *parent )
{
	DevPropertiesDlg *dp;
	QWidget *defTab;

	dp = new DevPropertiesDlg( parent, this );
	dp->addTab( (defTab = new ModBbisA201PropTab(getSlots(), parent, dp)),
				   "VME Settings");
	dp->setButtons();
	dp->showPage( defTab );

	wDebug(("ModBbisA201Family::createPropDlg dp=%p", dp ));
	return dp;
}

DevPropertiesDlg *
ModBbisA201Family::createPropDlg4Win( QWidget *parent )
{
	DevPropertiesDlg *dp;
	QWidget *defTab;

	dp = new DevPropertiesDlg( parent, this, FALSE );
	dp->addTab( (defTab = new ModBbisA201PropTab(getSlots(), parent, dp)),
				   "VME Settings");

	dp->setOkButton();
	dp->setCancelButton();

	// use current mouse position
	dp->move( QCursor::pos() );

	dp->showPage( defTab );

	wDebug(("ModBbisA201Family::createPropDlg dp=%p", dp ));
	return dp;
}

DeviceProperties *
ModBbisA201Family::createProperties()
{
	ModBbisA201Properties *p = new ModBbisA201Properties();

	p->dbgSettings.setModules(
		(1L<<DebugProperties::Driver) |
		(1L<<DebugProperties::BbisKernel) |
		(1L<<DebugProperties::Oss) |
		(1L<<DebugProperties::Desc));

	// set defaults
	p->vmeAddr = 0xe00000;
	p->isA16Space = false;

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
ModBbisA201Family::copyProperties(
	const DeviceProperties *from,
	DeviceProperties *to )
{
	const ModBbisA201Properties *_from;
	ModBbisA201Properties *_to;

	WIZ_DYNAMIC_CAST( to, _to, ModBbisA201Properties * );
	WIZ_DYNAMIC_CAST( from, _from, const ModBbisA201Properties * );

	*_to = *_from;
	wDebug(("ModBbisA201Family::copyProperties: %p->%p vme=%lx", _from, _to, _from->vmeAddr ));
}


ResourceMng *
ModBbisA201Family::getVmeAddRes( bool _isA16Space )
{
	VmeBusInterface *busIf = (VmeBusInterface *)getParent();

	if( !busIf )
		return 0;
	return _isA16Space ? busIf->vmeA16Res : busIf->vmeA24Res;
}

void
ModBbisA201Family::freeResources()
{
	VmeBusInterface *busIf = (VmeBusInterface *)getParent();
	if( busIf == 0 ) return;

	wDebug(("ModBbisA201Family::freeResources"));
	busIf->vmeA16Res->freeAllOfOwner( this );
	busIf->vmeA24Res->freeAllOfOwner( this );
	busIf->vmeIrqVectorRes->freeAllOfOwner( this );

	Device::freeResources();
}

Device::ApplyResult
ModBbisA201Family::applyProperties( DeviceProperties *newProp,
									QString &errMsg )
{
	if( newProp == 0 )
		newProp = prop;

	ModBbisA201Properties *_prop;
	ModBbisA201Properties *_propAct;

	WIZ_DYNAMIC_CAST( prop, _propAct, ModBbisA201Properties * );
	WIZ_DYNAMIC_CAST( newProp, _prop, ModBbisA201Properties * );

	ApplyResult ar;

	VmeBusInterface *busIf = (VmeBusInterface *)getParent();
	QString msg, msg2;
	ulong s;
	ResourceMng::IsAvailResult isAvail;
	ResourceMng::AllocResult alr = ResourceMng::AllocResFail;
	ResourceMng *vmeSpc;
	HwComponent *owner;

	wDebug(("ModBbisA201Family::applyProperties"));

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

	wDebug(("ModBbisA201Family::applyProperties errMsg=%s",
		   (const char *)errMsg));

	return ar;

 abort:
	freeResources();
	wDebug(("ModBbisA201Family::applyProperties failed errMsg=%s",
		   (const char *)errMsg));
	return ApplyPropFail;
}

Device::ApplyResult
ModBbisA201Family::applyProperties( DescriptorEntryDirectory *devDesc,
									QString &errMsg )
{
	DescriptorEntry *e, *eNext;
	DescriptorEntryUint32 *eUint32;
	DescriptorEntryBinary *eBinary;
	ModBbisA201Properties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisA201Properties * );

	if( !devDesc ) return ApplyPropFail;

	// scan all descriptor entries
	eNext=devDesc->firstChild();

	// ??? should do a little more sanity checking...
	while( (e=eNext) ){
		bool entryHandled = true;
		QString key = e->path().section("/",1);

		wDebug(("key=%s", (const char *)key ));

		if( key == "VME_A16_ADDR" ){
			_prop->isA16Space = true;
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->vmeAddr = eUint32->getValue();
		}
		else if( key == "VME_A24_ADDR" ){
			_prop->isA16Space = false;
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->vmeAddr = eUint32->getValue();
		}
		else if( key == "VME_DATA_WIDTH" ){
			;
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
ModBbisA201Family::getMainProperties()
{
	QString rv;

	ModBbisA201Properties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisA201Properties * );
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
ModBbisA201Family::createSpecialDesc( DescriptorEntryDirectory *parentDesc )
{
	int i;
	ModBbisA201Properties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisA201Properties * );

	DescriptorFactory dFact;
	Q3MemArray<uchar> arr(getSlots());

	wDebug(("ModBbisA201Family::createSpecialDesc ns=%d", getSlots()));
	parentDesc->addChild( dFact.create("VME_DATA_WIDTH", 1 ));	// always D16
	if( _prop->isA16Space )
		parentDesc->addChild( dFact.create("VME_A16_ADDR", _prop->vmeAddr ));
	else
		parentDesc->addChild( dFact.create("VME_A24_ADDR", _prop->vmeAddr ));

	parentDesc->addChild( dFact.create("VME_DATA_WIDTH", 1 ));

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

//! Create a A201 BBIS device
ModBbisA201::ModBbisA201() :
	ModBbisA201Family(QString("A201"),QString("A201"),QString("A201"))
{
	createSlots();
	setDescription("VME 6U Four Slot M-Module carrier board");
}

Device *ModBbisA201::create(bool withSubDevs)
{
	UTIL_UNREF_PARAM(withSubDevs);
	return new ModBbisA201();
}

//! Create a A302 BBIS device
ModBbisA302::ModBbisA302( bool withSubDevs ) :
	ModBbisA201Family(QString("A302"),QString("A302"),QString("A201"))
{
	BbisSlot *bbSlot;

	UTIL_UNREF_PARAM(withSubDevs);

	setDescription("VME 6U 128 Binary I/Os Board");

	// create slots and M66
	for( int slotNum=0; slotNum<getSlots(); slotNum++ ){
		bbSlot = new BbisSlot( slotNum, MmodD16, MmodA08 );
		bbSlot->setBbSlotSpecRequired( true );
		addChild( bbSlot );
	}
}

Device *ModBbisA302::create(bool withSubDevs)
{
	return new ModBbisA302( withSubDevs );
}

QString ModBbisA302::getDeviceNameTpl()
{
	return "A302";
}


//! Create a B201 BBIS device
ModBbisB201::ModBbisB201() :
	ModBbisA201Family(QString("B201"),QString("B201"),QString("A201"))
{
	createSlots();
	setDescription("VME 3U Single Slot M-Module carrier board");
}

Device *ModBbisB201::create(bool withSubDevs)
{
	UTIL_UNREF_PARAM(withSubDevs);
	return new ModBbisB201();
}

SwModuleList *
ModBbisB201::enumSwModules()
{
	if( lstSwModules == 0 ){
		lstSwModules = new SwModuleList;
		lstSwModules->add( makeStdDriverSwModule( "A201", "B201" ));
	}
	return lstSwModules;
}

//! Create a B202 BBIS device
ModBbisB202::ModBbisB202() :
	ModBbisA201Family(QString("B202"),QString("B202"),QString("A201"))
{
	createSlots();
	setDescription("VME 3U Two Slot M-Module carrier board");
}

// re-incarnation of this dev.
Device *ModBbisB202::create(bool withSubDevs)
{
	UTIL_UNREF_PARAM(withSubDevs);
	return new ModBbisB202();
}

SwModuleList *
ModBbisB202::enumSwModules()
{
	if( lstSwModules == 0 ){
		lstSwModules = new SwModuleList;
		lstSwModules->add( makeStdDriverSwModule( "A201", "B202" ));
	}
	return lstSwModules;
}




