/***************************************************************************
 */
/*!       \file  modbbisisa.cpp 
 *       \brief  Implementation of Isa BBIS devices
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:29:04 $
 *    $Revision: 2.0 $
 *
 *  	  \todo add properties tab 
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbisisa.cpp,v $
 * Revision 2.0  2010/01/22 11:29:04  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.5  2006/08/04 11:50:09  ub
 * added property "irqNumber" to ModBbisIsa for use in ModBbisMscan5200
 *
 * Revision 1.4  2004/01/09 08:20:46  kp
 * intermediate checkin
 *
 * Revision 1.3  2003/06/05 11:59:27  dschmidt
 * intermediate checkin
 *
 * Revision 1.2  2003/06/05 09:35:04  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/06/03 10:17:47  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include "modbbisisa.h"
#include "wizdebug.h"
#include "configuration.h"


// -----------------------------------------------------------------
// BBIS Isa

//! Create an Isa BBIS device
/*!
  The device will have the default properties set by createProperties().
  
  It will have one BBIS slot (numbered 0).
  
*/
ModBbisIsa::ModBbisIsa( bool isIo) :
	BbisDevice( isIo ? "ISA_IO" : "ISA", 
				isIo ? "ISA_IO" : "ISA", true, LocalBus )
{
	if( prop ) delete prop;	// delete Device::prop
	prop = createProperties();
	
	BbisSlot *bbSlot = new BbisSlot( LocalBus, 0 );
	addChild( bbSlot );

	_isIo = isIo;
	
	if( isIo )
		setDescription( "Pseudo device for I/O mapped hardware" );
	else
		setDescription( "Pseudo device for memory mapped hardware" );

}

Device *ModBbisIsa::create(bool withSubDevs)
{
	UTIL_UNREF_PARAM(withSubDevs);
	return new ModBbisIsa( _isIo );
}

//! Set address parameters
/*!
  ISA special call. Can be used by CPU models to set a fixed device
  address/size for an onboard component 

*/  
void 
ModBbisIsa::setIsaAddress( ulong devAddr, ulong devAddrSize )
{
	ModBbisIsaProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisIsaProperties * );

	_prop->devAddr = devAddr;
	_prop->devAddrSize = devAddrSize;
}

//! Set IRQ number
/*!  ISA special call. Can be used by CPU models to set a fixed IRQ number for
     an onboard component. \n 

     If the number is invalid (<0) then the descriptor entry IRQ_NUMBER will
     not be generated.
*/  
void 
ModBbisIsa::setIrqNumber( long irqNumber )
{
	ModBbisIsaProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisIsaProperties * );

	_prop->irqNumber = irqNumber;
}


SwModuleList *
ModBbisIsa::enumSwModules()
{
	if( lstSwModules == 0 ){
		lstSwModules = new SwModuleList;

		lstSwModules->add( 
			makeStdDriverSwModule("ISA", 
								  _isIo ? "IO" : "",  true ));
	}
	return lstSwModules;
}

//! destructs ModBbisIsa instance
ModBbisIsa::~ModBbisIsa()
{
	wDebug(("~ModBbisIsa()"));
	freeResources();
	delete prop;
	prop = 0;
}



DeviceProperties *
ModBbisIsa::createProperties()
{
	ModBbisIsaProperties *p = new ModBbisIsaProperties();

	p->dbgSettings.setModules( 	
		(1L<<DebugProperties::Driver) |
		(1L<<DebugProperties::BbisKernel) |
		(1L<<DebugProperties::Oss) |
		(1L<<DebugProperties::Desc));

	// set defaults
	p->devAddr = 0xFFFF;
	p->devAddrSize = 0;
	p->irqNumber = -1;  // invalid value

	return p;
}

void 
ModBbisIsa::copyProperties( 
	const DeviceProperties *from,
	DeviceProperties *to )
{
	const ModBbisIsaProperties *_from;
	ModBbisIsaProperties *_to;

	WIZ_DYNAMIC_CAST( to, _to, ModBbisIsaProperties * );
	WIZ_DYNAMIC_CAST( from, _from, const ModBbisIsaProperties * );

	*_to = *_from;
}

	
void
ModBbisIsa::freeResources()
{
	BusInterface *busIf = (BusInterface *)getParent();
	if( busIf == 0 ) return;
	
	Device::freeResources();
}

Device::ApplyResult 
ModBbisIsa::applyProperties( DeviceProperties *newProp, 
							 QString &errMsg )
{
	if( newProp == 0 )
		newProp = prop;

	ModBbisIsaProperties *_prop;
	ModBbisIsaProperties *_newProp;

	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisIsaProperties * );
	WIZ_DYNAMIC_CAST( newProp, _newProp, ModBbisIsaProperties * );

	*_prop = *_newProp;
	
	ApplyResult ar;

	// allocate name	
	ar = Device::applyProperties( _prop, errMsg );
	if( ar == ApplyPropFail )
		goto abort;

	return ar;

 abort:	
	freeResources();	
	wDebug(("ModBbisIsa::applyProperties failed errMsg=%s",
		   (const char *)errMsg));
	return ApplyPropFail;
}

Device::ApplyResult
ModBbisIsa::applyProperties( DescriptorEntryDirectory *devDesc, 
									QString &errMsg )
{
	DescriptorEntry *e, *eNext;
	DescriptorEntryUint32 *eUint32;
	ModBbisIsaProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisIsaProperties * );

	if( !devDesc ) return ApplyPropFail;

	// scan all descriptor entries
	eNext=devDesc->firstChild();

	while( (e=eNext) ){
		bool entryHandled = true;
		QString key = e->path().section("/",1);
		
		wDebug(("key=%s", (const char *)key ));

		if( key == "DEVICE_ADDR" ){
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->devAddr = eUint32->getValue();
		}
		else if( key == "DEVICE_ADDRSIZE" ){
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->devAddrSize = eUint32->getValue();			
		}
		else if( key == "IRQ_NUMBER" ){
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->irqNumber = eUint32->getValue();			
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
ModBbisIsa::getMainProperties()
{
	QString rv;

	ModBbisIsaProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisIsaProperties * );
	WIZ_ASSERT( getParent() );

	rv = getParent()->getMainProperties();
	rv += "ISA" + _isIo ? " IO " : " ";
	rv += QString("%1").arg(_prop->devAddr,0,16);

	return rv;
}

//
// This is called by MdisDevice::createDesc 
// to fill device specific descriptor entries
void
ModBbisIsa::createSpecialDesc( DescriptorEntryDirectory *parentDesc )
{
	ModBbisIsaProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisIsaProperties * );

	DescriptorFactory dFact;

	parentDesc->addChild( dFact.create("DEVICE_ADDR", _prop->devAddr ));
	parentDesc->addChild( dFact.create("DEVICE_ADDRSIZE", _prop->devAddrSize));

    // irqNumber is used only with ModBbisMscan5200
    // for backward compatibility create entry only when valid
    if( _prop->irqNumber > 0 )
        parentDesc->addChild( dFact.create("IRQ_NUMBER", _prop->irqNumber));
}

