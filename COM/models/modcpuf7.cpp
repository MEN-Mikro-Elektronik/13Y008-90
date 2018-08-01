/***************************************************************************
 */
/*!       \file  modcpuf7.cpp 
 *       \brief  Classes for model MEN F7 
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:30:30 $
 *    $Revision: 2.0 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuf7.cpp,v $
 * Revision 2.0  2010/01/22 11:30:30  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.8  2009/04/27 17:20:41  CRuff
 * R: Provide CPU selection possibility for Intel CPUs
 * M: Added CPU cores Pentium/Pentium2 to F7
 *
 * Revision 1.7  2004/06/08 17:26:48  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.6  2004/01/09 09:00:46  kp
 * removed syntax errors
 *
 * Revision 1.5  2004/01/09 08:20:58  kp
 * intermediate checkin
 *
 * Revision 1.4  2003/06/11 12:22:16  kp
 * intermediate checkin
 *
 * Revision 1.3  2003/06/05 11:59:39  dschmidt
 * intermediate checkin
 *
 * Revision 1.2  2003/06/05 09:35:29  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/06/03 10:18:10  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>

#include "modcpuf7.h"
#include "wizdebug.h"
#include "wizexcept.h"
#include "modbbisisa.h"
#include "util.h"

// -----------------------------------------------------------------
// CPU F7

//! Creates an F7 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the 
  BBIS devices
*/
ModCpuF7::ModCpuF7(bool withSubDevs ): 
	CpuDevice( QString("F7"), QString("F7"), false )
{
	BusInterface *busIf1, *busIf2;

	lstCpuCores << Pentium << Pentium2 << Pentium3;

	setDescription("CompactPCI Pentium CPU");	

	// Create the bus interfaces

	{
		// Interface for CompactPci
		Q3MemArray<uchar> cpciBusPath(2);
		cpciBusPath[0] = 0x1e;
		cpciBusPath[1] = 0x06;

		busIf1 = new PciBusInterface( 
			Cpci, 2, 8, 0xf, &cpciBusPath);		// Interface for CompactPci
		addChild( busIf1 );
		busIf1->setInstName( QString("CompactPci Bus"));
	}

	{		
        // Interface for F7 BBISes	
		busIf2 = new BusInterface( LocalBus, false );
		addChild( busIf2 );
	}

	if( withSubDevs ){
		ModBbisF7 *wdogBbis = new ModBbisF7( true );   // local BBIS dev (wdog)
		busIf2->addChild( wdogBbis );
	}
}

Device *ModCpuF7::create(bool withSubDevs)
{
	return new ModCpuF7(withSubDevs);
}


// -----------------------------------------------------------------
// BBIS F7

ModBbisF7::ModBbisF7(bool withSubDevs) : ModBbisIsa( true )
{
	UTIL_UNREF_PARAM(withSubDevs);

	setHwName("F7_BBIS");
	setIsaAddress( 0x50e, 1 );
	setInstName( "F7_OBDEVS" );
	setInstNameChangeAllowed( false );

	// Modify parameters of BBIS slot
	BbisSlot *bbSlot;
	WIZ_DYNAMIC_CAST( firstChild(), bbSlot, BbisSlot * );

	bbSlot->setSlotName( "pseudo slot for F7_WDOG" );
	bbSlot->setBbSlotSpecRequired( true );
}


