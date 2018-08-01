/***************************************************************************
 */
/*!       \file  modcpuem02.cpp 
 *       \brief  Classes for model MEN Em02
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2004/06/08 17:26:28 $
 *    $Revision: 1.1 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuem02.cpp,v $
 * Revision 1.1  2004/06/08 17:26:28  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright by MEN mikro elektronik GmbH, Nuernberg, Germany
 ****************************************************************************/
#include "modcpuem02.h"
#include "modbbischam.h"
#include "wizdebug.h"
#include "resourcemng.h"

// -----------------------------------------------------------------
// CPU Em02

//! Creates an Em02 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the 
  BBIS devices
*/
ModCpuEm02::ModCpuEm02( bool withSubDevs ): 
	CpuDevice( QString("EM02"), QString("PC"), false )
{
	BusInterface *busIf1;

	setDescription( "Embedded System Module with Pentium3" );

	lstCpuCores << Pentium3;

	// ESM PciBusInterface
	QArray<uchar> esmPciBusPath(1);	// ???
	esmPciBusPath[0] = 0x1e;

	QArray<uchar> esmPciBusTbl(4);
	esmPciBusTbl[0] = 20;		
	esmPciBusTbl[1] = 21;
	esmPciBusTbl[2] = 22;
	esmPciBusTbl[3] = 23;

	busIf1 = new PciBusInterface( Pc104PlusPci, 0, 3, 
								  &esmPciBusPath, &esmPciBusTbl, true );
	busIf1->setInstName( QString("ESM PCI Connector"));
	addChild( busIf1 );

}





