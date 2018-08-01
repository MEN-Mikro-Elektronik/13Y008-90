/***************************************************************************
 */
/*!       \file  modcpucpcipc.cpp 
 *       \brief  Classes for model "generic CompactPCI PC"
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2014/07/18 15:12:35 $
 *    $Revision: 2.2 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpucpcipc.cpp,v $
 * Revision 2.2  2014/07/18 15:12:35  dpfeuffer
 * R: unused cpciBusPath variable
 * M: cpciBusPath removed
 *
 * Revision 2.1  2011/09/26 08:43:03  dpfeuffer
 * R: new model COMPACTPCI_PLUSIO_PC
 * M: new model class ModCpuCpciPcPlusIo added
 *
 * Revision 2.0  2010/01/22 11:29:53  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.6  2009/04/27 17:14:23  CRuff
 * R: Provide CPU selection possibility for Intel CPUs
 * M: Added CPU core Pentium2 to CPCIPC
 *
 * Revision 1.5  2004/06/08 17:26:18  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.4  2003/06/11 12:22:04  kp
 * intermediate checkin
 *
 * Revision 1.3  2003/06/05 09:35:23  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/06/03 10:18:03  kp
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:33:26  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include "modcpucpcipc.h"
#include "wizdebug.h"
#include "util.h"

#define MAX_PCISER_SLOTS	4	// peripheral slot 2..5

//! Creates a "generic CompactPCI PC" device
/*!
  including the bus interface (CPCI)
*/
ModCpuCpciPc::ModCpuCpciPc(bool withSubDevs) : 
	CpuDevice( QString("COMPACTPCI_PC"), QString("CPCIPC"), false )
{
	BusInterface *busIf1;

	UTIL_UNREF_PARAM(withSubDevs);

	lstCpuCores << I386 << I486 << Pentium << Pentium2 << Pentium3 << 
		Pentium4 << Duron << Athlon;

	setDescription("Generic CompactPci X86 CPU");	
	setVendorName(QString::null);

	// Create the bus interfaces
	busIf1 = new PciBusInterface( Cpci );
	addChild( busIf1 );
	busIf1->setInstName( QString("CompactPCI bus"));
}

Device *ModCpuCpciPc::create(bool withSubDevs)
{
	return new ModCpuCpciPc(withSubDevs);
}


// -----------------------------------------------------------------
// CPU Generic CompactPCI PlusIO

//! Creates a "generic CompactPCI PlusIO PC" device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuCpciPcPlusIo::ModCpuCpciPcPlusIo( bool withSubDevs ):
		ModCpuCpciPc( withSubDevs )
{	
	BusInterface *busIf1=0;
	int i;

	UTIL_UNREF_PARAM(withSubDevs);

    setHwName( "COMPACTPCI_PLUSIO_PC" );
	setDriverName( "CPCIPLUSIOPC" );
    setDescription( "Generic CompactPci PlusIO X86 CPU" );
	setVendorName(QString::null);
    
	// Create the bus interfaces

	/////////////////////////////////////
    // Interface for CompactPci Serial
	for( i=2; i<MAX_PCISER_SLOTS+2; i++ ){
		busIf1 = new PciBusInterface( CpciSer );
		busIf1->setInstName( QString("CompactPCI Serial slot %1").arg(i) );
		addChild( busIf1 );
	}
}
