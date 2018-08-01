/***************************************************************************
 */
/*!       \file  modcpuf1n.cpp 
 *       \brief  Classes for model MEN F1N
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2014/08/22 15:57:20 $
 *    $Revision: 2.1 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuf1n.cpp,v $
 * Revision 2.1  2014/08/22 15:57:20  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.0  2010/01/22 11:30:17  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.5  2004/06/08 17:26:36  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.4  2003/06/11 12:22:12  kp
 * intermediate checkin
 *
 * Revision 1.3  2003/06/05 09:35:27  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/06/03 10:18:08  kp
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:33:30  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>

#include "modcpuf1n.h"
#include "wizdebug.h"
#include "wizexcept.h"
#include "util.h"


// -----------------------------------------------------------------
// CPU F1N

//! Creates an F1N CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the 
  BBIS devices
*/
ModCpuF1N::ModCpuF1N( bool withSubDevs ): 
	CpuDevice( QString("F1N"), QString("B11"), false )
{
	BusInterface *busIf1;

	UTIL_UNREF_PARAM(withSubDevs);

	lstCpuCores << Ppc603e;
	setDescription("CompactPCI PowerPC 824x CPU with PC-MIP slots");	

	// Create the bus interfaces
	Q3MemArray<uchar> cpciBusPath(1);
	cpciBusPath[0] = 0x1e;

	busIf1 = new PciBusInterface( 
		Cpci, 2, 8, 0xf, &cpciBusPath);		   	// Interface for CompactPci
	addChild( busIf1 );
	busIf1->setInstName( QString("CompactPci Bus"));
	busIf1->setEndianSwapping( true );

	// Interface for PC-MIPs
	{
		BusInterface *busIfMezz;
		Q3MemArray<uchar> pciBusPath(0);

		Q3MemArray<uchar> pciBusTbl(2);
		pciBusTbl[0] = 0x1a;     
		pciBusTbl[1] = 0x1d;  

		busIfMezz = new PciBusInterface( PcMip, 0, 1,
                          				   &pciBusPath, &pciBusTbl, true );
		busIfMezz->setInstName(QString("onboard PC-MIP"));
		busIfMezz->setEndianSwapping( true );
		addChild(busIfMezz);
	}
}

Device *ModCpuF1N::create(bool withSubDevs)
{
	return new ModCpuF1N(withSubDevs);
}
