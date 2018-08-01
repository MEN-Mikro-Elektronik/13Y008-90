/***************************************************************************
 */
/*!       \file  modcpupp1.cpp
 *       \brief  Classes for model MEN PP1
 *      \author  Ulrich.Bogensperger@men.de
 *        $Date: 2016/02/20 16:27:14 $
 *    $Revision: 2.4 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpupp1.cpp,v $
 * Revision 2.4  2016/02/20 16:27:14  ts
 * R: BoxPCs based on SC24 need 2 SMBusses in MDIS wizard
 * M: 1. added member nrSmbBusses to class CpuDeviceSmb
 *    2. added parameter int nrSmbBusses to every constructor of CPUs with SMB
 *       controllers: 2 for SC24 (AMD based) Box PCs, 1 for every other CPU
 *
 * Revision 2.3  2011/09/28 09:07:29  dpfeuffer
 * R:1. no '-' allowed in instance/hw name
 *   2. set swapping variant erroneous
 *   3. PCI_BUS_PATH 0 illegal
 * M:1. OBPCI-104 replaced by OBPCI104
 *   2. set swapping variant fixed
 *   3. PCI_BUS_PATH replaced by PCI_BUS_NUMBER 0
 *
 * Revision 2.2  2011/09/26 09:17:56  dpfeuffer
 * R: no MEN HW with PC104+ PCI (it is PCI-104)
 * M: Pc104PlusPci replaced with Pci104
 *
 * Revision 2.1  2010/05/19 17:50:10  CRuff
 * R: 1. add SMB controller support
 * M: 1a) inherit from CpuDeviceSmb instead of CpuDevice
 *    1b) add BusInterface for SMB BBISes
 *
 * Revision 2.0  2010/01/22 11:30:48  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.6  2007/05/07 13:24:51  cs
 * fixed:
 *   PP1 MPC5200 on board MSCAN devices: IRQ number changed to 0x41/0x42
 *
 * Revision 1.5  2006/11/30 11:28:33  cs
 * fixed:
 *   - remove device 0x00 from empty busPaths: now length 0!
 *
 * Revision 1.4  2006/08/04 11:50:17  ub
 * added irqNumber property
 *
 * Revision 1.3  2006/07/27 13:43:33  ub
 * Added: Support for MSCAN on PP1
 *
 * Revision 1.2  2005/12/07 16:55:21  ub
 * PP1 model enhanced
 *
 * Revision 1.1  2005/07/15 15:47:33  ub
 * Initial Revision
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>

#include "modcpupp1.h"
#include "wizdebug.h"
#include "resourcemng.h"
#include "modcpuesm.h"          // for ModBbisMscan5200
#include "modbbissmbpcigen.h"
#include "util.h"

// -----------------------------------------------------------------
// CPU PP1

//! Creates an PP1 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuPp1::ModCpuPp1( bool withSubDevs ):
    CpuDeviceSmb( QString("PP1"), QString("PC"), false, true, 1 )
{
    BusInterface *busIf1, *busIf2, *canBusIf1, *canBusIf2;
    ModBbisSmbPciGen* smbBbis;
	BbisDevice *bbis1=0;

    setDescription( "PCI-104 COM Module with PowerPC MPC5200" );

    lstCpuCores << Ppc603e;

    // PciBusInterface
    Q3MemArray<uchar> pciBusPath(0); // the Pci104 connector directly carries bus 0

    Q3MemArray<uchar> pciBusTbl(4);
    pciBusTbl[0] = 20;           // IDSEL A20
    pciBusTbl[1] = 21;           //       A21 etc.
    pciBusTbl[2] = 22;
    pciBusTbl[3] = 23;

    busIf1 = new PciBusInterface( Pci104, 0, 3,
                                  &pciBusPath, &pciBusTbl, true );
    busIf1->setInstName( QString("PCI-104 Connector"));
    busIf1->setEndianSwapping( true );
    addChild( busIf1 );

    // Interfaces for built-in MSCAN
    canBusIf1 = new BusInterface( LocalBus, false );
    canBusIf1->setInstName( QString("MSCAN1 on MPC5200"));
    addChild( canBusIf1 );

    canBusIf2 = new BusInterface( LocalBus, false );
    canBusIf2->setInstName( QString("MSCAN2 on MPC5200"));
    addChild( canBusIf2 );

    if( withSubDevs ) {
        // the two MSCAN controllers
        ModBbisMscan5200 *canBbis1 = new ModBbisMscan5200( "BBMSCAN_1",
                                                           0xf0000900,
                                                           0x41 );
        canBusIf1->addChild( canBbis1 );
        ModBbisMscan5200 *canBbis2 = new ModBbisMscan5200( "BBMSCAN_2",
                                                           0xf0000980,
                                                           0x42 );
        canBusIf2->addChild( canBbis2 );

		{	// Interface for PP1 onboard PCI-104 BBIS device
			busIf2 = new PciBusInterface( LocalBus, -1, -1, 0, 0,
										  false );
			//busIf2->setEndianSwapping( true );
			addChild( busIf2 );

			bbis1 = new ModBbisPp1ObPci104();
			busIf2->addChild( bbis1 );
		}
    }
    
    this->hasObSmb = true;
    
    this->smbusIf = new PciBusInterface( LocalBus, -1, -1, 0, 0, false );
	this->smbusIf->setInstName( QString("Onboard SMB"));
	addChild( this->smbusIf );  
    smbBbis = new ModBbisSmbPciGen("NAT");
	this->smbBbisList.push_back(smbBbis);
}

// -----------------------------------------------------------------
// BBIS PP1 PCI-104

//! Creates an PP1 onboard PCI-104 BBIS device
ModBbisPp1ObPci104::ModBbisPp1ObPci104(bool withSubDevs) :
	ModBbisPciGen( QString("OBPCI104"),
				   QString("PP1_OBPCI104"),
				   QString("PCI"),
				   LocalBus )
{
	ModBbisPciGenProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisPciGenProperties * );

	UTIL_UNREF_PARAM(withSubDevs);

	setDescription("Pseudo device for onboard PCI-104 slots");
	setEndianSwapping( true );
	
	_prop->pciBusNo = 0;

	createSlot( 0, 0x14, Pci104);
	createSlot( 1, 0x15, Pci104);
	createSlot( 2, 0x16, Pci104);
	createSlot( 3, 0x17, Pci104);
}

