/***************************************************************************
 */
/*!       \file  modcpuem04.cpp 
 *       \brief  Classes for model MEN Em04
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2004/06/08 17:26:31 $
 *    $Revision: 1.1 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuem04.cpp,v $
 * Revision 1.1  2004/06/08 17:26:31  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright by MEN mikro elektronik GmbH, Nuernberg, Germany
 ****************************************************************************/
#include "modcpuem04.h"
#include "modbbischam.h"
#include "wizdebug.h"
#include "resourcemng.h"

// -----------------------------------------------------------------
// CPU Em04

//! Creates an Em04 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the 
  BBIS devices
*/
ModCpuEm04::ModCpuEm04( bool withSubDevs ): 
	CpuDevice( QString("EM04"), QString("PC"), false )
{
	BusInterface *busIf1, *busIf2;
	BbisDevice *bbis;

	setDescription( "Embedded System Module with PowerPC 8245" );

	lstCpuCores << Ppc603e;

	// ESM PciBusInterface
	QArray<uchar> esmPciBusPath(0);
	QArray<uchar> esmPciBusTbl(4);
	esmPciBusTbl[0] = 20;
	esmPciBusTbl[1] = 21;
	esmPciBusTbl[2] = 22;
	esmPciBusTbl[3] = 23;

	busIf1 = new PciBusInterface( Pc104PlusPci, 0, 3, 
								  &esmPciBusPath, &esmPciBusTbl, true );
	busIf1->setInstName( QString("ESM PCI Connector"));
	busIf1->setEndianSwapping( true );
	addChild( busIf1 );

	// Interfaces for EM04 BBISes	
	QArray<uchar> fpgaPciBusPath(1);
    fpgaPciBusPath[0] = 0x1d;
	busIf2 = new PciBusInterface( StdPci, -1, -1, 0, &fpgaPciBusPath, false );
	busIf2->setEndianSwapping( true );
	addChild( busIf2 );

	// Chameleon FPGA
	if( withSubDevs ){
		bbis = new ModBbisCham();
		bbis->setInstName( "FPGA" );
		busIf2->addChild( bbis );	
	}
}




