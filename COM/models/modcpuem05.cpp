/***************************************************************************
 */
/*!       \file  modcpuem05.cpp 
 *       \brief  Classes for model MEN Em05
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2005/03/09 13:49:39 $
 *    $Revision: 1.2 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuem05.cpp,v $
 * Revision 1.2  2005/03/09 13:49:39  ub
 * added Chameleon native Linux driver to software module list
 *
 * Revision 1.1  2004/06/08 17:26:33  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright by MEN mikro elektronik GmbH, Nuernberg, Germany
 ****************************************************************************/
#include "modcpuem05.h"
#include "modbbischam.h"
#include "wizdebug.h"
#include "resourcemng.h"

// -----------------------------------------------------------------
// CPU Em05

//! Creates an Em05 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the 
  BBIS devices
*/
ModCpuEm05::ModCpuEm05( bool withSubDevs ): 
	CpuDevice( QString("EM05"), QString("PC"), false )
{
	BusInterface *busIf1, *busIf2;
	BbisDevice *bbis;

	setDescription( "Embedded System Module with Transmeta TM5800/5900" );

	lstCpuCores << Crusoe;

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
	addChild( busIf1 );

	// Interfaces for EM05 BBISes	
	QArray<uchar> fpgaPciBusPath(1);
    fpgaPciBusPath[0] = 13;
	busIf2 = new PciBusInterface( StdPci, -1, -1, 0, &fpgaPciBusPath, false );
	addChild( busIf2 );

	// Chameleon FPGA
	if( withSubDevs ){
		bbis = new ModBbisCham("PCITBL");
		bbis->setInstName( "FPGA" );
		busIf2->addChild( bbis );	
	}
}

Device *ModCpuEm05::create(bool withSubDevs)
{
	return new ModCpuEm05( withSubDevs);
}

SwModuleList *
ModCpuEm05::enumSwModules()
{	
	SwModuleList *addLst = enumSwModulesForExternalPackage( "CHAMELEON" );

	if( lstSwModules == 0 && addLst )
		lstSwModules = new SwModuleList;

	if( lstSwModules && addLst )
		lstSwModules->add( addLst );

	return lstSwModules;
}
