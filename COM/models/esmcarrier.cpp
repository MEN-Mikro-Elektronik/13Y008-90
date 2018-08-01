/***************************************************************************
 */
/*!       \file  esmcarrier.cpp 
 *       \brief  Implementation of ESM carrier board classes
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/06/02 16:33:32 $
 *    $Revision: 2.1 $
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: esmcarrier.cpp,v $
 * Revision 2.1  2010/06/02 16:33:32  CRuff
 * R: new connector types ESMini and ESMexpress
 * M: implement new classes ESMexprCarrier and ESMiniCarrier
 *
 * Revision 2.0  2010/01/22 11:28:26  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.1  2004/06/08 17:25:52  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include "esmcarrier.h"
#include "wizdebug.h"
#include "wizexcept.h"
#include "resourcemng.h"


// -----------------------------------------------------------------
//! destructs ModBbisPciMmod
EsmCarrier::~EsmCarrier()
{
	wDebug(("~EsmCarrier()"));
	freeResources();
}

void
EsmCarrier::freeResources()
{
	PciBusInterface *busIf = (PciBusInterface *)getParent();


	wDebug(("EsmCarrier::freeResources busIf=%p", busIf));
	if( busIf == 0 ) return;
	
	busIf->pciSlots->freeAllOfOwner( this );

	Device::freeResources();
}

Device::ApplyResult 
EsmCarrier::applyProperties( DeviceProperties *newProp, 
							 QString &errMsg )
{
	if( newProp == 0 )
		newProp = prop;

	ApplyResult ar;
	PciBusInterface *busIf;
	WIZ_DYNAMIC_CAST( getParent(), busIf, PciBusInterface * );
	QString msg;
	ulong s;
	ResourceMng::AllocResult alr = ResourceMng::AllocResOk;

	wDebug(("EsmCarrier::applyProperties newProp=%p devName=%s", newProp,
			newProp->deviceName.latin1()));

	// allocate all of the 4 ESM "slots"
	s = 0;
	alr = busIf->pciSlots->alloc( 4, 1, s, this );
	
	if( alr == ResourceMng::AllocResFail ){
		errMsg = "Could not allocate the ESM slot!\n";
		goto abort;
	}

	// allocate name	
	msg = QString::null;
	ar = Device::applyProperties( newProp, msg );
	errMsg += msg;
	if( ar == ApplyPropFail )
		goto abort;

	return ar;

 abort:	
	freeResources();	
	return ApplyPropFail;
}

// -----------------------------------------------------------------
//! destructs ModBbisPciMmod
ESMexprCarrier::~ESMexprCarrier()
{
	wDebug(("~ESMexprCarrier()"));
	freeResources();
}

void
ESMexprCarrier::freeResources()
{
	PciBusInterface *busIf = (PciBusInterface *)getParent();


	wDebug(("ESMexprCarrier::freeResources busIf=%p", busIf));
	if( busIf == 0 ) return;
	
	busIf->pciSlots->freeAllOfOwner( this );

	Device::freeResources();
}

Device::ApplyResult 
ESMexprCarrier::applyProperties( DeviceProperties *newProp, 
							 QString &errMsg )
{
	if( newProp == 0 )
		newProp = prop;

	ApplyResult ar;
	PciBusInterface *busIf;
	WIZ_DYNAMIC_CAST( getParent(), busIf, PciBusInterface * );
	QString msg;
	ulong s;
	ResourceMng::AllocResult alr = ResourceMng::AllocResOk;

	wDebug(("ESMexprCarrier::applyProperties newProp=%p devName=%s", newProp,
			newProp->deviceName.latin1()));

	// allocate the ESMexpress "slot"
	s = 0;
	alr = busIf->pciSlots->alloc( 1, 1, s, this );
	
	if( alr == ResourceMng::AllocResFail ){
		errMsg = "Could not allocate the ESMexpress slot!\n";
		goto abort;
	}

	// allocate name	
	msg = QString::null;
	ar = Device::applyProperties( newProp, msg );
	errMsg += msg;
	if( ar == ApplyPropFail )
		goto abort;

	return ar;

 abort:	
	freeResources();	
	return ApplyPropFail;
}

// -----------------------------------------------------------------
//! destructs ModBbisPciMmod
ESMiniCarrier::~ESMiniCarrier()
{
	wDebug(("~ESMiniCarrier()"));
	freeResources();
}

void
ESMiniCarrier::freeResources()
{
	PciBusInterface *busIf = (PciBusInterface *)getParent();


	wDebug(("ESMiniCarrier::freeResources busIf=%p", busIf));
	if( busIf == 0 ) return;
	
	busIf->pciSlots->freeAllOfOwner( this );

	Device::freeResources();
}

Device::ApplyResult 
ESMiniCarrier::applyProperties( DeviceProperties *newProp, 
							 QString &errMsg )
{
	if( newProp == 0 )
		newProp = prop;

	ApplyResult ar;
	PciBusInterface *busIf;
	WIZ_DYNAMIC_CAST( getParent(), busIf, PciBusInterface * );
	QString msg;
	ulong s;
	ResourceMng::AllocResult alr = ResourceMng::AllocResOk;

	wDebug(("ESMiniCarrier::applyProperties newProp=%p devName=%s", newProp,
			newProp->deviceName.latin1()));

	// allocate all of the ESMini "slot"
	s = 0;
	alr = busIf->pciSlots->alloc( 1, 1, s, this );
	
	if( alr == ResourceMng::AllocResFail ){
		errMsg = "Could not allocate the ESMini slot!\n";
		goto abort;
	}

	// allocate name	
	msg = QString::null;
	ar = Device::applyProperties( newProp, msg );
	errMsg += msg;
	if( ar == ApplyPropFail )
		goto abort;

	return ar;

 abort:	
	freeResources();	
	return ApplyPropFail;
}

