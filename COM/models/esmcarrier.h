/***************************************************************************
 */
/*!       \file  esmcarrier.h
 *       \brief  classes for ESM carriers
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2011/09/26 08:42:42 $
 *    $Revision: 2.2 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: esmcarrier.h,v $
 * Revision 2.2  2011/09/26 08:42:42  dpfeuffer
 * R: no MEN HW with PC104+ PCI (it is PCI-104)
 * M: Pc104PlusPci replaced with Pci104
 *
 * Revision 2.1  2010/06/02 16:33:58  CRuff
 * R: new connector types ESMini and ESMexpress
 * M: added new classes ESMexprCarrier and ESMiniCarrier
 *
 * Revision 2.0  2010/01/22 11:28:28  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.1  2004/06/08 17:25:53  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef ESMCARRIER_H
#define ESMCARRIER_H

#include "hwcomponent.h"

//! common class for ESM carrier boards
class EsmCarrier : public CpuExtDevice
{	
public:
	EsmCarrier(const QString &_hwName, bool isRemoveable=true) :
		CpuExtDevice( _hwName, isRemoveable, Pci104 ) {}
	~EsmCarrier();

	virtual void freeResources();
	virtual Device::ApplyResult applyProperties( DeviceProperties *newProp, 
												 QString &errMsg );
};

class ESMexprCarrier : public CpuExtDevice
{
public:
	ESMexprCarrier(const QString &_hwName, bool isRemoveable=true) :
		CpuExtDevice( _hwName, isRemoveable, ESMexpress ) {}
	~ESMexprCarrier();

	virtual void freeResources();
	virtual Device::ApplyResult applyProperties( DeviceProperties *newProp, 
												 QString &errMsg );
};

class ESMiniCarrier : public CpuExtDevice
{
public:
	ESMiniCarrier(const QString &_hwName, bool isRemoveable=true) :
		CpuExtDevice( _hwName, isRemoveable, ESMini ) {}
	~ESMiniCarrier();

	virtual void freeResources();
	virtual Device::ApplyResult applyProperties( DeviceProperties *newProp, 
												 QString &errMsg );
};

#endif



