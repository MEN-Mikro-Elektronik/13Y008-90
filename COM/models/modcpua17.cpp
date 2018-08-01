/***************************************************************************
 */
/*!       \file  modcpua17.cpp
 *       \brief  MEN A17/A19/A20 classes
 *      \author  Christine.Ruff@men.de
 *        $Date: 2016/02/20 16:26:47 $
 *    $Revision: 2.4 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpua17.cpp,v $
 * Revision 2.4  2016/02/20 16:26:47  ts
 * R: BoxPCs based on SC24 need 2 SMBusses in MDIS wizard
 * M: 1. added member nrSmbBusses to class CpuDeviceSmb
 *    2. added parameter int nrSmbBusses to every constructor of CPUs with SMB
 *       controllers: 2 for SC24 (AMD based) Box PCs, 1 for every other CPU
 *
 * Revision 2.3  2014/08/22 15:56:43  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.2  2014/07/18 15:12:24  dpfeuffer
 * R: erroneous PMC support, missing ComExpress support, general maintenance
 * M: intermediate check-in during development
 *
 * Revision 2.1  2010/05/19 17:44:40  CRuff
 * R: 1. SMB controller support
 *    2. cosmetics
 * M: 1. inherit from CpuDeviceSmb instead of CpuDevice
 *    2. move classes for A19 and A20 in new file modcpua19.cpp
 *
 * Revision 2.0  2010/01/22 11:29:43  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.3  2009/04/27 17:13:08  CRuff
 * R: Provide CPU selection possibility for Intel CPUs
 * M: Added CPU cores Pentium/Pentium2/Pentium3 to A19, A20
 *
 * Revision 1.2  2009/04/21 16:53:41  CRuff
 * R: incorrect pci bus path for onboard pmc bbises for A19, A20
 * M: corrected bus path for onboard pmc bbises
 *
 * Revision 1.1  2009/04/21 15:38:42  CRuff
 * Initial Revision
 *
 * Initial revision
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#include <q3memarray.h>

#include <Q3MemArray>

#include "modcpua17.h"
#include "modbbischam.h"
#include "resourcemng.h"
#include "wizdebug.h"
#include "util.h"
#include "modbbissmbpcigen.h"

//! Creates an A17 CPU device
ModCpuA17::ModCpuA17(  bool withSubDevs )
: CpuDeviceSmb( QString("A17"), QString("A17"), false, true, 1 )
{
	BusInterface *fpgaBusIf=0;
	BusInterface *busIfObPCI1=0;
	BusInterface *vmeBusIf=0;
    BbisDevice *bbis = 0;
    ModBbisSmbPciGen* smbBbis = 0;

    lstCpuCores << PpcE500;

    setDescription("VME PowerQuicc III with two PMC slots");

    // add PCI interface for FPGA
    Q3MemArray<uchar> fpgaPciBusPath(1);
    fpgaPciBusPath[0] = 0x1c;
    fpgaBusIf = new PciBusInterface( StdPci, -1, -1, &fpgaPciBusPath, 0, false );
    fpgaBusIf->setEndianSwapping( true );
    fpgaBusIf->setInstName( QString("OnBoard PCI Bus")); //?
    addChild( fpgaBusIf );

     if( withSubDevs ) {
        // Chameleon FPGA
        bbis = new ModBbisCham("PCITBL", fpgaPciBusPath[0]);
        bbis->setInstName( "FPGA" );
        fpgaBusIf->addChild( bbis );
     }

    // create vme bus interface
    vmeBusIf = new ModVmeIfA17();					// VME interface
	vmeBusIf->setInstName( QString("VME"));
	addChild( vmeBusIf );

    // create mezzanine slot interfaces
    Q3MemArray<uchar> pciBusPath(0);

	Q3MemArray<uchar> pciBusTbl1(1);
	Q3MemArray<uchar> pciBusTbl2(2);
	
	// PMC slot 0/1
	pciBusTbl2[0] = 0x1e;
	pciBusTbl2[1] = 0x1b;

	busIfObPCI1 = new PciBusInterface( Pmc, 0, 1, &pciBusPath, &pciBusTbl2, true );
	busIfObPCI1->setInstName(QString("onboard PMC"));
	busIfObPCI1->setEndianSwapping( true );
	addChild(busIfObPCI1);

	// XMC
    // undefined paths because PCI-X lines/bridges can be configured by SW
    // and can shift whole bus structure
	busIfObPCI1 = new PciBusInterface( Xmc, -1, -1, 0, 0, true );
	busIfObPCI1->setInstName(QString("onboard XMC"));
	busIfObPCI1->setEndianSwapping( true );
	((PciBusInterface*)busIfObPCI1)->setPciDomainNoUnknown();
	addChild(busIfObPCI1);

	// create smb bbis interface
    this->smbusIf = new PciBusInterface( LocalBus, -1, -1, 0, 0, false );
	this->smbusIf->setInstName( QString("Onboard SMB"));
	addChild( this->smbusIf );  
    smbBbis = new ModBbisSmbPciGen("NAT");
	this->smbBbisList.push_back(smbBbis);
}

// -----------------------------------------------------------------
// VME Bus If A17

ModVmeIfA17::ModVmeIfA17() : VmeBusInterface()
{
	vmeA16Res->addRegion( 0x0000, 0x10000 );
	vmeA24Res->addRegion( 0x000000, 0x1000000 );
	vmeA32Res->addRegion( 0x00000000, 0xffffffff );
	vmeIrqVectorRes->addRegion( 0x80, 0x7e );
	// FIXME: add A64
	setInstName( "VMEbus" );
}

ulong
ModVmeIfA17::supportedIrqLevels()
{
	return 0xfe; // level 7-1
}

SwModuleList *
ModCpuA17::enumSwModules()
{
	CpuDevice::enumSwModules();
	SwModuleList *addLst = enumSwModulesForExternalPackage( "TSI148" );

	if( lstSwModules == 0 && addLst )
		lstSwModules = new SwModuleList;

	if( lstSwModules && addLst )
		lstSwModules->add( addLst );

	return lstSwModules;
}



