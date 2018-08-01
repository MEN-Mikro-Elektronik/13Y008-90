/***************************************************************************
 */
/*!       \file  comexprcarrier.h
 *       \brief  MEN COM Express carrier board classes
 *      \author	 dieter.pfeuffer@men.de
 *		  $Date: 2014/07/18 15:12:04 $
 *    $Revision: 2.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: comexprcarrier.h,v $
 * Revision 2.1  2014/07/18 15:12:04  dpfeuffer
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2014 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef COMEXPRCARRIER_H
#define COMEXPRCARRIER_H

#include "hwcomponent.h"

//! common class for COM Express carrier boards
class ComExprCarrier : public CpuExtDevice
{	
public:
	ComExprCarrier(const QString &_hwName, bool isRemoveable=true) :
		CpuExtDevice( _hwName, isRemoveable, ComExpr ) {}
	~ComExprCarrier();

	virtual void freeResources();
	virtual Device::ApplyResult applyProperties( DeviceProperties *newProp, 
												 QString &errMsg );
};

#endif
