/***************************************************************************
 */
/*!       \file  modcpua11.cpp 
 *       \brief  Classes for model MEN A11
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2014/08/22 15:56:36 $
 *    $Revision: 2.1 $
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpua11.cpp,v $
 * Revision 2.1  2014/08/22 15:56:36  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.0  2010/01/22 11:29:33  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.1  2004/06/08 17:26:09  kp
 * Initial Revision
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#include <Q3MemArray>

#include "modcpua11.h"
#include "resourcemng.h"
#include "util.h"

class ModVmeIfA11 : public VmeBusInterface 
{
public:
	ModVmeIfA11();
	virtual ulong supportedIrqLevels();
};


// -----------------------------------------------------------------
// CPU A11

//! Creates an A11 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the 
  BBIS devices
*/
ModCpuA11::ModCpuA11(bool withSubDevs) : 
	CpuDevice( "A11", "A11", false )
{
	BusInterface *busIf1, *busIf2;
	BbisDevice *bbis1;

	lstCpuCores << Ppc603e << Ppc740 << Ppc750;

	setDescription("VME 6U PowerPC 603/7xx CPU");	

	// Create the bus interfaces
	busIf1 = new ModVmeIfA11();					// VME interface
	addChild( busIf1 );
	busIf1->setInstName( QString("VMEbus"));

    // Interface for A11 BBISes	
	Q3MemArray<uchar> pciBusPath(0);
	busIf2 = new PciBusInterface( LocalBus, 0, 0, 0x14, &pciBusPath, false );
	addChild( busIf2 );

	// Interface for PC-MIPs
	{
		BusInterface *busIfMezz;
		Q3MemArray<uchar> pciBusPath(1);
		pciBusPath[0] = 0x14;        

		Q3MemArray<uchar> pciBusTbl(2);
		pciBusTbl[0] = 0x0;     
		pciBusTbl[1] = 0x1;  

		busIfMezz = new PciBusInterface( PcMip, 0, 1,
                          				   &pciBusPath, &pciBusTbl, true );
		busIfMezz->setInstName(QString("onboard PC-MIP"));
		busIfMezz->setEndianSwapping( true );
		addChild(busIfMezz);
	}


	if( withSubDevs ){
		// Create the local BBISes
		bbis1 = new ModBbisA11();					// MMod-Bbis
		busIf2->addChild( bbis1 );
	}
}

Device *ModCpuA11::create(bool withSubDevs)
{
	return new ModCpuA11(withSubDevs);
}

// -----------------------------------------------------------------
// BBIS A11

//! Creates an A11 BBIS device
ModBbisA11::ModBbisA11(bool withSubDevs) :
	BbisDevice( QString("A11_OBDEVS"), 
				QString("A11"), false, LocalBus )
{
	BbisSlot *bbSlot;

	UTIL_UNREF_PARAM(withSubDevs);

	setInstName( QString("OB_DEVS") );
	setDescription("Pseudo device for onboard devices");	

	bbSlot = new BbisSlot( LocalBus, 0x1000 );	// Z8536
	bbSlot->setSlotName( "pseudo slot for Z8536" );
	bbSlot->setBbSlotSpecRequired( true );
	addChild( bbSlot );

	bbSlot = new BbisSlot( LocalBus, 0x1001 );	// LM78
	bbSlot->setSlotName( "pseudo slot for LM78" );
	bbSlot->setBbSlotSpecRequired( true );
	addChild( bbSlot );

	bbSlot = new BbisSlot( LocalBus, 0x1002 );	// LEDS
	bbSlot->setSlotName( "pseudo slot for LEDs" );
	bbSlot->setBbSlotSpecRequired( true );
	addChild( bbSlot );

}

// -----------------------------------------------------------------
// VME Bus If 

ModVmeIfA11::ModVmeIfA11() : VmeBusInterface()
{
	vmeA16Res->addRegion( 0x0000, 0x10000 );
	vmeA24Res->addRegion( 0x000000, 0x1000000 );
	vmeA32Res->addRegion( 0x01000000, 0x0f00000 ); 
	vmeIrqVectorRes->addRegion( 0x80, 0x7e );
}

ulong
ModVmeIfA11::supportedIrqLevels()
{
	return 0xfe; // level 7-1
}





