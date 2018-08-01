/***************************************************************************
 */
/*!       \file  modcpud6.cpp
 *       \brief  Classes for model MEN D6
 *      \author	 Christian.Schuster@men.de
 *		  $Date: 2016/02/20 16:26:56 $
 *    $Revision: 2.3 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpud6.cpp,v $
 * Revision 2.3  2016/02/20 16:26:56  ts
 * R: BoxPCs based on SC24 need 2 SMBusses in MDIS wizard
 * M: 1. added member nrSmbBusses to class CpuDeviceSmb
 *    2. added parameter int nrSmbBusses to every constructor of CPUs with SMB
 *       controllers: 2 for SC24 (AMD based) Box PCs, 1 for every other CPU
 *
 * Revision 2.2  2014/08/22 15:57:15  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.1  2010/05/19 17:46:49  CRuff
 * R: 1. add SMB controller support
 * M: 1a) inherit from CpuDeviceSmb instead of CpuDevice
 *    1b) add BusInterface for SMB BBISes
 *
 * Revision 2.0  2010/01/22 11:30:08  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.4  2009/04/27 17:15:50  CRuff
 * R: Provide CPU selection possibility for Intel CPUs
 * M: Added CPU cores Pentium/Pentium2/Pentium3 to D6, D9
 *
 * Revision 1.3  2009/04/21 16:54:35  CRuff
 * R: incorrect pci bus path for onboard pmc bbises for D9
 * M: corrected bus path for onboard pmc bbis
 *
 * Revision 1.2  2009/04/21 15:19:38  CRuff
 * R: 1. support D6 Pmc slots
 *    2. support new boards D7, D9
 * M: 1. ModCpuD6::ModCpuD6(): added bus interface for pmc slots
 *       (with pci bus path)
 *    2. added ModCpuD9() and supporting Bbis class constructors
 *       ModBbisD9ObPmc() and ModBbisD9ObXmc()
 *
 * Revision 1.1  2006/11/30 11:28:25  cs
 * Initial Revision
 *
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2006-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>

#include "modcpud6.h"
#include "wizdebug.h"
#include "wizexcept.h"
#include "modbbischam.h"
#include "modbbissmbpcigen.h"

// -----------------------------------------------------------------
// CPU D6

//! Creates an D6 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuD6::ModCpuD6( bool withSubDevs ):
	CpuDeviceSmb( QString("D6"), QString("PC"), false, true, 1 )
{
	BusInterface *busIf1=0, *busIf2=0, *fpgaBusIf=0;
	BbisDevice *bbis1=0;
	ModBbisSmbPciGen* smbBbis=0;

	lstCpuCores << Pentium << Pentium2 << Pentium3 << Pentium4;

	setDescription("CompactPCI/Express Pentium M CPU");

    { // Create OB PCI BBIS Interface for FPGA
		Q3MemArray<uchar> fpgaPciBusPath(2);
	    fpgaPciBusPath[0] = 0x1e;
	    fpgaPciBusPath[1] = 0x06;
	    // minSlot != -1 reports hasPciBusPath!
		fpgaBusIf = new PciBusInterface( StdPci, 0, 0, 0, &fpgaPciBusPath,
	                                     false );
	    fpgaBusIf->setInstName( QString("OnBoard PCI Bus"));
		addChild( fpgaBusIf );
	}

	{	// Create the CPCI bus interface
	    Q3MemArray<uchar> cpciBusPath(1);
	    cpciBusPath[0] = 0x1c;

	    busIf1 = new PciBusInterface(
	        Cpci, 2, 8, 0xf, &cpciBusPath);		// Interface for CompactPci
	    busIf1->setInstName( QString("CompactPci Bus"));
	    addChild( busIf1 );
	}

    { // Create OB PCI BBIS Interface
		Q3MemArray<uchar> obPciBusPath(0);

		busIf2 = new PciBusInterface( LocalBus, 0, 0, 0x1e, &obPciBusPath,
									  false );
		busIf2->setInstName( QString("PCI local"));
		addChild( busIf2 );
	}
	
	// create mezzanine slot interfaces
	{
		BusInterface *busIfObPCI1=0;

		// PMC slot 0/1
		Q3MemArray<uchar> pciBusPath(1);
	    pciBusPath[0] = 0x1e;
		
		Q3MemArray<uchar> pciBusTbl(2);
		pciBusTbl[0] = 0x04;           
		pciBusTbl[1] = 0x05;      

		busIfObPCI1 = new PciBusInterface( Pmc, 0, 1, &pciBusPath, &pciBusTbl, true );
		busIfObPCI1->setInstName(QString("onboard PMC"));
		addChild(busIfObPCI1);

		// XMC
		// undefined paths because PCI-X lines/bridges can be configured by SW
		// and can shift whole bus structure
		busIfObPCI1 = new PciBusInterface( Xmc, -1, -1, 0, 0, true );
		busIfObPCI1->setInstName(QString("onboard XMC"));
		addChild(busIfObPCI1);
	}

	if( withSubDevs ){
		// CHAMELEON BBIS
		bbis1 = new ModBbisCham("PCITBL", 0x06);
		bbis1->setInstName( "FPGA" );
		fpgaBusIf->addChild( bbis1 );
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

// -----------------------------------------------------------------
// CPU D9

//! Creates an D9 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/

ModCpuD9::ModCpuD9( bool withSubDevs=true )
: CpuDeviceSmb( QString("D9"), QString("PC"), false, true, 1 )
{
	BusInterface *busIf1=0;
	BusInterface *busIf2=0;
	ModBbisSmbPciGen* smbBbis=0;
 	UTIL_UNREF_PARAM(withSubDevs);
	
	lstCpuCores << Pentium << Pentium2 << Pentium3 << Pentium4;

	setDescription("CompactPCI/Express Core Duo CPU");

	{	// Create the CPCI bus interface
	    Q3MemArray<uchar> cpciBusPath(1);
	    cpciBusPath[0] = 0x1e;

	    busIf1 = new PciBusInterface(
	        Cpci, 2, 8, 0xf, &cpciBusPath);		// Interface for CompactPci
	    busIf1->setInstName( QString("CompactPci Bus"));
	    addChild( busIf1 );
	}
	
	{ // Create OB PCI BBIS Interface
		Q3MemArray<uchar> obPciBusPath(1);
		obPciBusPath[0] = 0x7c; // 0x1c.3 = 0b 0111 1100

		busIf2 = new PciBusInterface( LocalBus, 0, 0, 0, &obPciBusPath,
									  false );
		busIf2->setInstName( QString("PCI local") );
		addChild( busIf2 );
	}
	
	// create mezzanine slot interfaces
	{
		BusInterface *busIfObPCI1=0;

		// PMC slot 0
		Q3MemArray<uchar> pciBusPath2(2);
		pciBusPath2[0] = 0x7c; // 0x1c.3 = 0b 0111 1100
		pciBusPath2[1] = 0x00;
		
		Q3MemArray<uchar> pciBusTbl(1);
		pciBusTbl[0] = 0x08;              

		busIfObPCI1 = new PciBusInterface( Pmc, 0, 0, &pciBusPath2, &pciBusTbl, true );
		busIfObPCI1->setInstName(QString("onboard PMC"));
		addChild(busIfObPCI1);

		// XMC slot 0
		Q3MemArray<uchar> pciBusPath1(1);
		pciBusPath1[0] = 0x5c; // 0x1c.2 = 0b 0101 1100

		busIfObPCI1 = new PciBusInterface( Xmc, 0, 0, 0, &pciBusPath1, true );
		busIfObPCI1->setInstName(QString("onboard XMC"));
		addChild(busIfObPCI1);
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


