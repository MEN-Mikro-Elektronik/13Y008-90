/***************************************************************************
 */
/*!       \file  modcpucpciserpc.cpp 
 *       \brief  Classes for model "generic CompactPCI Serial PC"
 *      \author	 dieter.pfeuffer@men.de
 *		  $Date: 2011/09/26 08:43:08 $
 *    $Revision: 2.1 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpucpciserpc.cpp,v $
 * Revision 2.1  2011/09/26 08:43:08  dpfeuffer
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2011 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include "modcpucpciserpc.h"
#include "wizdebug.h"
#include "util.h"

#define MAX_PCISER_SLOTS	8	// peripheral slot 2..9

//! Creates a "generic CompactPCI Serial PC" device
/*!
  including the bus interface (CPCI)
*/
ModCpuCpciSerPc::ModCpuCpciSerPc(bool withSubDevs) : 
	CpuDevice( QString("COMPACTPCI_SERIAL_PC"), QString("CPCISERPC"), false )
{
	BusInterface *busIf1;
	int i;

	UTIL_UNREF_PARAM(withSubDevs);

	lstCpuCores << I386 << I486 << Pentium << Pentium2 << Pentium3 << 
		Pentium4 << PentiumM << Duron << Athlon << Crusoe << Atom;

	setDescription("Generic CompactPci Serial X86 CPU");	
	setVendorName(QString::null);

	// Create the bus interfaces
	for( i=2; i<MAX_PCISER_SLOTS+2; i++ ){
		busIf1 = new PciBusInterface( CpciSer );
		addChild( busIf1 );
		busIf1->setInstName( QString("CompactPCI Serial slot %1").arg(i) );
	}
}

Device *ModCpuCpciSerPc::create(bool withSubDevs)
{
	return new ModCpuCpciSerPc(withSubDevs);
}



