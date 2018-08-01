/***************************************************************************
 */
/*!       \file  modcpupc.cpp 
 *       \brief  Classes for model standard PCs
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2014/08/22 15:57:29 $
 *    $Revision: 2.3 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpupc.cpp,v $
 * Revision 2.3  2014/08/22 15:57:29  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.2  2011/09/28 09:07:26  dpfeuffer
 * R: no '-' allowed in instance/hw name
 * M: OBPCI-104 replaced by OBPCI104
 *
 * Revision 2.1  2011/09/26 09:17:51  dpfeuffer
 * R: tests to add PCI-104 interface to standard PC model
 * M: no functional changes yet
 *
 * Revision 2.0  2010/01/22 11:30:44  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.6  2009/04/27 17:21:49  CRuff
 * R: Provide CPU selection possibility for Intel CPUs
 * M: Added CPU cores Pentium2 to StandardPC
 *
 * Revision 1.5  2004/06/08 17:26:52  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.4  2003/06/11 12:22:20  kp
 * intermediate checkin
 *
 * Revision 1.3  2003/06/05 09:35:31  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/06/03 10:18:12  kp
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:33:32  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#include "modcpupc.h"
#include "wizdebug.h"
#include "util.h"

#define MAX_PCIE_SLOTS	8	// peripheral slot 1..8

// -----------------------------------------------------------------
// CPU PC

//! Creates an Stadard PC CPU device
/*!
  including the bus interface (PCI)
*/
ModCpuPc::ModCpuPc(bool withSubDevs) : 
	CpuDevice( QString("STANDARD_PC"), QString("PC"), false )
{
	BusInterface *busIf1;

	UTIL_UNREF_PARAM(withSubDevs);

	lstCpuCores << I386 << I486 << Pentium << Pentium2 <<Pentium3 << 
		Pentium4 << Duron << Athlon;

	setVendorName(QString::null);
	setDescription("Standard Desktop X86 PC");	

	// Create the PCI bus interfaces
	busIf1 = new PciBusInterface( StdPci );
	addChild( busIf1 );
	busIf1->setInstName( QString("PCI bus"));

	// Create the PCIe bus interfaces
	busIf1 = new PciBusInterface( StdPciE );
	addChild( busIf1 );
	busIf1->setInstName( QString("PCIe bus"));
}

Device *ModCpuPc::create(bool withSubDevs)
{
	return new ModCpuPc(withSubDevs);
}
