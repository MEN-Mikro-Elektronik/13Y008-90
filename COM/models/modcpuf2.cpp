/***************************************************************************
 */
/*!       \file  modcpuf2.cpp 
 *       \brief  Classes for model MEN F2 
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:30:22 $
 *    $Revision: 2.0 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuf2.cpp,v $
 * Revision 2.0  2010/01/22 11:30:22  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.3  2009/04/27 17:20:14  CRuff
 * R: Provide CPU selection possibility for Intel CPUs
 * M: Added CPU cores Pentium/Pentium2 to F2
 *
 * Revision 1.2  2004/06/08 17:26:40  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.1  2004/01/09 08:20:56  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>

#include "modcpuf2.h"
#include "wizdebug.h"
#include "resourcemng.h"

#include "modelfactory.h"
#include "configuration.h"
#include "qstringlist.h"

// -----------------------------------------------------------------
// CPU F2

//! Creates an F2 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the 
  BBIS devices
*/
ModCpuF2::ModCpuF2(bool withSubDevs ): 
	CpuDevice( QString("F2"), QString("F2"), false )
{
	BusInterface *busIf1, *busIf2;

	lstCpuCores << Pentium << Pentium2 << Pentium3;

	setDescription("CompactPCI Pentium CPU");	

	// Create the bus interfaces

	{
		// Interface for CompactPci
		Q3MemArray<uchar> cpciBusPath(1);
		cpciBusPath[0] = 0x08;

		busIf1 = new PciBusInterface( 
			Cpci, 2, 8, 0xf, &cpciBusPath);		// Interface for CompactPci
		addChild( busIf1 );
		busIf1->setInstName( QString("CompactPci Bus"));
	}

	{		
        // Interface for F2 BBISes	
		busIf2 = new BusInterface( LocalBus, false );
		addChild( busIf2 );
	}

	if( withSubDevs ){
		ModBbisF2 *bbis = new ModBbisF2( true );   // local BBIS dev 
		busIf2->addChild( bbis );
	}
}

Device *ModCpuF2::create(bool withSubDevs)
{
	return new ModCpuF2(withSubDevs);
}


// -----------------------------------------------------------------
// BBIS F2


ModBbisF2::ModBbisF2(bool withSubDevs) :
	BbisDevice( QString("F2_OBDEVS"), 
				QString("F2"), false, LocalBus )
{
	BbisSlot *bbSlot;
	QString errMsg;
	int slot;
	const int firstSlot=0x1000, lastSlot=0x1002;
	const QString locModels[] = { "Z8536", "LEDs/GPIOs", "LM79" };

	UTIL_UNREF_PARAM(withSubDevs);

	setInstName( QString("F2_OBDEVS") );
	setInstNameChangeAllowed( false );

	setDescription("Pseudo device for onboard devices");	

	// create slots
	for( slot=firstSlot; slot<=lastSlot; slot++ ){
		bbSlot = new BbisSlot( LocalBus, slot );

		bbSlot->setSlotName( QString("pseudo slot for ") + 
							 locModels[slot-firstSlot] );
		bbSlot->setBbSlotSpecRequired( true );
		addChild( bbSlot );
	}
}



SwModuleList *ModBbisF2::enumSwModules()
{
	if( lstSwModules == 0 ){
		lstSwModules = new SwModuleList;

		// always ignore byteswapping (to generate driver.mak)
		lstSwModules->add( makeStdDriverSwModule("F2", "", true ));
	}
	return lstSwModules;
}

