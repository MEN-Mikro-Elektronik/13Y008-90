/***************************************************************************
 */
/*!       \file  modcpud2.cpp 
 *       \brief  Classes for model MEN D2 
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2014/08/22 15:57:06 $
 *    $Revision: 2.1 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpud2.cpp,v $
 * Revision 2.1  2014/08/22 15:57:06  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.0  2010/01/22 11:29:58  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.8  2009/09/03 11:10:30  CRuff
 * R: preparation for SMB support
 * M: added example how to integrate smb support (add SmbBusInterface)
 *
 * Revision 1.7  2004/06/08 17:26:22  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.6  2004/01/09 08:20:54  kp
 * intermediate checkin
 *
 * Revision 1.5  2003/06/11 12:22:08  kp
 * intermediate checkin
 *
 * Revision 1.4  2003/06/05 11:59:37  dschmidt
 * intermediate checkin
 *
 * Revision 1.3  2003/06/05 09:35:25  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/06/03 10:18:05  kp
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:33:28  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>

#include "modcpud2.h"
#include "wizdebug.h"
#include "resourcemng.h"

#include "modelfactory.h"
#include "configuration.h"
#include "qstringlist.h"

// -----------------------------------------------------------------
// CPU D2

//! Creates an D2 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the 
  BBIS devices
*/
ModCpuD2::ModCpuD2(bool withSubDevs ): 
	CpuDevice( QString("D2"), QString("D2"), false )
{
	BusInterface *busIf1, *busIf2;
	BbisDevice *bbis1;


	lstCpuCores << Pentium;

	setDescription("CompactPCI Pentium CPU with PC-MIP slots");	

	// Create the bus interfaces

	{
		// Interface for CompactPci
		Q3MemArray<uchar> cpciBusPath(1);
		cpciBusPath[0] = 0x08;

		busIf1 = new PciBusInterface( 
			Cpci, 2, 8, 0xf, &cpciBusPath);		   	// Interface for CompactPci
		addChild( busIf1 );
		busIf1->setInstName( QString("CompactPci Bus"));
		
		// SMB deactivated for now
		// SMB interface
		//busIfSmb = new SMBusInterface();
		//addChild( busIfSmb );
	}

	{		
        // Interface for D2 BBISes	
		Q3MemArray<uchar> pciBusPath(0);

		busIf2 = new PciBusInterface( LocalBus, -1, -1, 0, 0, false );
		addChild( busIf2 );
	}

	// Interface for PC-MIPs
	{
		BusInterface *busIfMezz;
		Q3MemArray<uchar> pciBusPath(0);

		Q3MemArray<uchar> pciBusTbl(3);
		pciBusTbl[0] = 0xc;     
		pciBusTbl[1] = 0xb;  
		pciBusTbl[2] = 0xd;     

		busIfMezz = new PciBusInterface( PcMip, 0, 2,
                          				   &pciBusPath, &pciBusTbl, true );
		busIfMezz->setInstName(QString("onboard PC-MIP"));
		addChild(busIfMezz);
	}

	if( withSubDevs ){
		bbis1 = new ModBbisD2();					// local devs BBIS
		busIf2->addChild( bbis1 );
	}
}

Device *ModCpuD2::create(bool withSubDevs)
{
	return new ModCpuD2(withSubDevs);
}


// -----------------------------------------------------------------
// BBIS D2

//! Creates an D2 BBIS device
ModBbisD2::ModBbisD2(bool withSubDevs) :
	BbisDevice( QString("D2_OBDEVS"), 
				QString("D2"), false, LocalBus )
{
	BbisSlot *bbSlot;
	QString errMsg;
	int slot;
	const int firstSlot=0x1000, lastSlot=0x1002;
	const QString locModels[] = { "Z8536_D1", "LM78_D1", "D2_LEDS" };

	UTIL_UNREF_PARAM(withSubDevs);

	setInstName( QString("D2_OBDEVS") );
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

SwModuleList *ModBbisD2::enumSwModules()
{
	if( lstSwModules == 0 ){
		lstSwModules = new SwModuleList;

		// always ignore byteswapping (to generate driver.mak)
		lstSwModules->add( makeStdDriverSwModule("D1", "D2", true ));
	}
	return lstSwModules;
}
