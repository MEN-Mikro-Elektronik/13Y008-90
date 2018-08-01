/***************************************************************************
 */
/*!       \file  modcpuf14.cpp
 *       \brief  Classes for model MEN F14
 *      \author	 Christian.Schuster@men.de
 *		  $Date: 2016/02/20 16:27:05 $
 *    $Revision: 2.3 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuf14.cpp,v $
 * Revision 2.3  2016/02/20 16:27:05  ts
 * R: BoxPCs based on SC24 need 2 SMBusses in MDIS wizard
 * M: 1. added member nrSmbBusses to class CpuDeviceSmb
 *    2. added parameter int nrSmbBusses to every constructor of CPUs with SMB
 *       controllers: 2 for SC24 (AMD based) Box PCs, 1 for every other CPU
 *
 * Revision 2.2  2011/09/26 09:17:38  dpfeuffer
 * R:1. ModCpuF14 modifications required
 * M:2. minor changes
 *
 * Revision 2.1  2010/05/19 17:49:29  CRuff
 * R: 1. add SMB controller support
 * M: 1a) inherit from CpuDeviceSmb instead of CpuDevice
 *    1b) add BusInterface for SMB BBISes
 *
 * Revision 2.0  2010/01/22 11:30:40  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.2  2009/04/27 17:19:26  CRuff
 * R: Provide CPU selection possibility for Intel CPUs
 * M: Added CPU cores Pentium/Pentium2/Pentium3 to F14
 *
 * Revision 1.1  2006/11/30 11:28:31  cs
 * Initial Revision
 *
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2006 - 2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>

#include "modcpuf14.h"
#include "wizdebug.h"
#include "wizexcept.h"
#include "modbbisisa.h"
#include "modbbissmbpcigen.h"
#include "propertiesdlg.h"

#include "util.h"


// -----------------------------------------------------------------
// CPU F14

//! Creates an F14 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuF14::ModCpuF14( bool withSubDevs, bool withCpci ):
	CpuDeviceSmb( QString("F14"), QString("PC"), false, true, 1 )
{	
	BusInterface *busIf1=0;
	ModBbisSmbPciGen* smbBbis = 0;

	UTIL_UNREF_PARAM(withSubDevs);

	lstCpuCores << Pentium << Pentium2 << Pentium3 << Pentium4;

	setDescription("CompactPCI/Express Pentium M CPU");
	setInstName("CPU");
    
	// Create the bus interfaces

    // Interface for CompactPci
	if( withCpci )
	{	// Create the CPCI bus interface
	    Q3MemArray<uchar> cpciBusPath(1);
	    cpciBusPath[0] = 0x1e;

	    busIf1 = new PciBusInterface(
	        Cpci, 2, 8, 0xf, &cpciBusPath);		// Interface for CompactPci
	    busIf1->setInstName( QString("CompactPci Bus"));
	    addChild( busIf1 );
	}
	
	// SMBus 
	{
		Q3MemArray<uchar> smbBusPath(1);
	    smbBusPath[0] = 0x7f;
	    
	    // cannot store the pci bus path in the busIf because one busIf can have several
	    // smb controller children
        this->smbusIf = new PciBusInterface( LocalBus, -1, -1, 0, 0, false );
		this->smbusIf->setInstName( QString("Onboard SMB"));
		addChild( this->smbusIf );
        
        smbBbis = new ModBbisSmbPciGen("ICH");
        // set pci bus path
        smbBbis->setPciBusPath(smbBusPath);
        
    	this->smbBbisList.push_back(smbBbis);
		
	}
}

