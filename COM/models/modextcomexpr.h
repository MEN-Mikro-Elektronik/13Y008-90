/***************************************************************************
 */
/*!       \file  modextcomexpr.h
 *       \brief  class for generic COM Express carrier board 
 *      \author	 dieter.pfeuffer@men.de
 *		  $Date: 2014/08/22 15:57:40 $
 *    $Revision: 2.2 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modextcomexpr.h,v $
 * Revision 2.2  2014/08/22 15:57:40  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.1  2014/07/18 15:12:39  dpfeuffer
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2014 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODEXTCOMEXPR_H
#define MODEXTCOMEXPR_H

#include <Q3MemArray>

#include "hwcomponent.h"
#include "comexprcarrier.h"
#include "wizexcept.h"

#define MAX_PCIE_SLOTS	8	// PCIe slots 1..8

class ModExtComExpr : public ComExprCarrier
{
public:
	ModExtComExpr( bool withSubDevs );
	Device *create(bool withSubDevs=true){
        return new ModExtComExpr( withSubDevs );
    }
};

#endif




