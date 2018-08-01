/***************************************************************************
 */
/*!       \file  modextcomexpr.cpp
 *       \brief  class for generic COM Express carrier board
 *      \author	 dieter.pfeuffer@men.de
 *		  $Date: 2014/08/22 15:57:37 $
 *    $Revision: 2.2 $
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modextcomexpr.cpp,v $
 * Revision 2.2  2014/08/22 15:57:37  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.1  2014/07/18 15:12:38  dpfeuffer
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2014 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>

#include "wizdebug.h"
#include "modextcomexpr.h"
#include "wizexcept.h"
#include "util.h"
#include "modbbischam.h"
#include "hwcomponent.h"

//! Create generic COM Express carrier board
/*!

*/
ModExtComExpr::ModExtComExpr( bool withSubDevs ) :	ComExprCarrier( "ComExpr" )
{
	UTIL_UNREF_PARAM(withSubDevs);

	setDescription( "Generic COM Express carrier board" );
	setInstName("ComExpr");

	BusInterface *busIf1;

	// PCIe interface
	busIf1 = new PciBusInterface( StdPciE );
	busIf1->setInstName( QString("PCIe bus") );
	addChild(busIf1);

	// PCI interface
	busIf1 = new PciBusInterface( StdPci );
	busIf1->setInstName( QString("PCI bus") );
	addChild(busIf1);
}



