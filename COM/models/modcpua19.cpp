/***************************************************************************
 */
/*!       \file  modcpua19.cpp
 *       \brief  MEN A17/A19/A20 classes
 *      \author  Christine.Ruff@men.de
 *        $Date: 2016/02/20 16:26:49 $
 *    $Revision: 2.3 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpua19.cpp,v $
 * Revision 2.3  2016/02/20 16:26:49  ts
 * R: BoxPCs based on SC24 need 2 SMBusses in MDIS wizard
 * M: 1. added member nrSmbBusses to class CpuDeviceSmb
 *    2. added parameter int nrSmbBusses to every constructor of CPUs with SMB
 *       controllers: 2 for SC24 (AMD based) Box PCs, 1 for every other CPU
 *
 * Revision 2.2  2014/08/22 15:56:47  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.1  2010/05/19 17:44:53  CRuff
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#include <q3memarray.h>

#include <Q3MemArray>

#include "modcpua19.h"
#include "modcpua17.h"
#include "modbbischam.h"
#include "resourcemng.h"
#include "wizdebug.h"
#include "util.h"
#include "modbbissmbpcigen.h"


// -----------------------------------------------------------------
// CPU A19

//! Creates an A19 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuA19::ModCpuA19( bool withSubDevs ) :
	CpuDeviceSmb( QString("A19"), QString("A19"), false, true, 1 )
{
	BusInterface *vmeBusIf=0;
    ModBbisSmbPciGen* smbBbis = 0;
 	UTIL_UNREF_PARAM(withSubDevs);

    setDescription( "VME Core Duo CPU with one PMC/XMC slot" );

	lstCpuCores << Pentium << Pentium2 << Pentium3 << Pentium4;

	// create vme bus interface
    vmeBusIf = new ModVmeIfA17();					// VME interface
	vmeBusIf->setInstName( QString("VME"));
	vmeBusIf->setEndianSwapping( true );
	addChild( vmeBusIf );


	// create mezzanine slot interface
	{
		BusInterface *busIfObPCI1;

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
		pciBusPath1[0] = 0x5c; // 0x1c.3 = 0b 0101 1100

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

SwModuleList *
ModCpuA19::enumSwModules()
{
	CpuDevice::enumSwModules();
	SwModuleList *addLst = enumSwModulesForExternalPackage( "TSI148" );

	if( lstSwModules == 0 && addLst )
		lstSwModules = new SwModuleList;

	if( lstSwModules && addLst )
		lstSwModules->add( addLst );

	return lstSwModules;
}

// -----------------------------------------------------------------
// CPU A20

//! Creates an A20 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuA20::ModCpuA20( bool withSubDevs ) :
	CpuDeviceSmb( QString("A20"), QString("A20"), false, true, 1 )
{
	BusInterface *busIfObPCI1=0;
	BusInterface *vmeBusIf=0;
    ModBbisSmbPciGen* smbBbis = 0;
	UTIL_UNREF_PARAM(withSubDevs);

    setDescription( "VME Core Duo CPU with one PMC/XMC slot" );

	lstCpuCores << Pentium << Pentium2 << Pentium3 << Pentium4;

	// create vme bus interface
    vmeBusIf = new ModVmeIfA17();					// VME interface
	vmeBusIf->setInstName( QString("VME"));
	vmeBusIf->setEndianSwapping( true );
	addChild( vmeBusIf );


	// create mezzanine slot interface
	{
		// PMC slot 0/1
		Q3MemArray<uchar> pciBusPath(2);
	    pciBusPath[0] = 0x1c; // 0x1c.0 = 0b 0001 1100
		pciBusPath[1] = 0x00;

		Q3MemArray<uchar> pciBusTbl(2);
		pciBusTbl[0] = 0x07;
		pciBusTbl[1] = 0x08;

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

SwModuleList *
ModCpuA20::enumSwModules()
{
	CpuDevice::enumSwModules();
	SwModuleList *addLst = enumSwModulesForExternalPackage( "TSI148" );

	if( lstSwModules == 0 && addLst )
		lstSwModules = new SwModuleList;

	if( lstSwModules && addLst )
		lstSwModules->add( addLst );

	return lstSwModules;
}

// -----------------------------------------------------------------
// CPU A25

//! Creates an A25 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuA25::ModCpuA25( bool withSubDevs ) :
	CpuDeviceSmb( QString("A25"), QString("A25"), false, true, 1 )
{
	BusInterface *busIfObPCI1=0;
	BusInterface *vmeBusIf=0;
    ModBbisSmbPciGen* smbBbis = 0;
	UTIL_UNREF_PARAM(withSubDevs);

    setDescription( "VME Xeon D-1500 server CPU with one PMC/XMC slot" );

	lstCpuCores << Pentium << Pentium2 << Pentium3 << Pentium4;

	// create vme bus interface
    vmeBusIf = new ModVmeIfA17();					// VME interface
	vmeBusIf->setInstName( QString("VME"));
	vmeBusIf->setEndianSwapping( true );
	addChild( vmeBusIf );


	// create mezzanine slot interface
	{
		// PMC slot 0/1
		Q3MemArray<uchar> pciBusPath(2);
	    pciBusPath[0] = 0x1c; // 0x1c.0 = 0b 0001 1100
		pciBusPath[1] = 0x00;

		Q3MemArray<uchar> pciBusTbl(2);
		pciBusTbl[0] = 0x07;
		pciBusTbl[1] = 0x08;

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

SwModuleList *
ModCpuA25::enumSwModules()
{
	CpuDevice::enumSwModules();
	SwModuleList *addLst = enumSwModulesForExternalPackage( "TSI148" );

	if( lstSwModules == 0 && addLst )
		lstSwModules = new SwModuleList;

	if( lstSwModules && addLst )
		lstSwModules->add( addLst );

	return lstSwModules;
}


