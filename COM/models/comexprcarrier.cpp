/***************************************************************************
 */
/*!       \file  comexprcarrier.cpp 
 *       \brief  MEN COM Express carrier board classes
 *      \author	 dieter.pfeuffer@men.de
 *		  $Date: 2014/07/18 15:12:02 $
 *    $Revision: 2.1 $
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: comexprcarrier.cpp,v $
 * Revision 2.1  2014/07/18 15:12:02  dpfeuffer
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2014 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include "comexprcarrier.h"
#include "wizdebug.h"
#include "wizexcept.h"
#include "resourcemng.h"


// -----------------------------------------------------------------
//! destructs ComExprCarrier
ComExprCarrier::~ComExprCarrier()
{
	wDebug(("~ComExprCarrier()"));
	freeResources();
}

void
ComExprCarrier::freeResources()
{
	PciBusInterface *busIf = (PciBusInterface *)getParent();


	wDebug(("ComExprCarrier::freeResources busIf=%p", busIf));
	if( busIf == 0 ) return;
	
	busIf->pciSlots->freeAllOfOwner( this );

	Device::freeResources();
}

Device::ApplyResult 
ComExprCarrier::applyProperties( DeviceProperties *newProp, 
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

	wDebug(("ComExprCarrier::applyProperties newProp=%p devName=%s", newProp,
			newProp->deviceName.latin1()));

	// allocate the COM Express "slot"
	s = 0;
	alr = busIf->pciSlots->alloc( 1, 1, s, this );
	
	if( alr == ResourceMng::AllocResFail ){
		errMsg = "Could not allocate the COM Express slot!\n";
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
