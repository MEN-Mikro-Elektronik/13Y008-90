/***************************************************************************
 */
/*!       \file  modcpucomexpr.cpp
 *       \brief  MEN COM Express CPUs classes
 *      \author  dieter.pfeuffer@men.de
 *        $Date: 2014/08/22 15:57:01 $
 *    $Revision: 2.2 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpucomexpr.cpp,v $
 * Revision 2.2  2014/08/22 15:57:01  dpfeuffer
 * R: COM Express support not finished
 * M: COM Express support completed
 *
 * Revision 2.1  2014/07/18 15:12:32  dpfeuffer
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2014 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>
#include <Q3PtrList>

#include "modcpucomexpr.h"
#include "modbbischam.h"
#include "resourcemng.h"
#include "wizdebug.h"
#include "modbbissmbpcigen.h"
#include "modextf503.h"

BusInterface*
ModCpuComExpr::newComExprBusIf( const Q3MemArray<uchar>* pciBusPath, bool endianSwapping )
{
    BusInterface *busIf;
    
    // COM Express interface
    busIf = new PciBusInterface( ComExpr, 0, 0, pciBusPath, 0, true );
    busIf->setInstName( QString("COM Express Connector"));
    busIf->setEndianSwapping( endianSwapping );
	((PciBusInterface*)busIf)->setPciDomainNoUnknown();

    return( busIf );
}

// -----------------------------------------------------------------
// CPU CB70C

//! Creates an CB70C CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuCb70c::ModCpuCb70c( bool withSubDevs ):
    ModCpuComExpr( QString("CB70C"), QString("PC")  )
{
	UTIL_UNREF_PARAM(withSubDevs);

	BusInterface	*busIf1;
	int				i;
	
    setDescription( "Rugged COM Express module with Intel Core i7" );
    lstCpuCores << Pentium3 << Pentium4;

	///////////////////////////////////////////////////
    // COM Express Interface
	// Note:
	// A COM Express carrier can be connected to this interface.
	// With the generic COM Express carrier (ModExtComExpr),
	// any PCI/PCIe device can be connected to the CPU, but the
	// PCI adressing must be done manually.
    addChild( newComExprBusIf( 0, true ) );

	///////////////////////////////////////////////////
    // PCIe interface at COM Express Connector 
	// Note:
	// This PCIe interface contains the CPU specific PCI adressing.
	// It can be used to connect PCIe devices to the provided
	// PCIe slot without further PCI configuration.
	Q3MemArray<uchar> pciBusPath(1);

	// PCIe mapping from 15CB70-_HwArchSpec 2.0: Table 11. PCIe usage
	// and Intel 7 Series / C216 Chipset PPT EDS: 5.2 PCI Express* Root Ports
	//
	// Using COM Express Connector description from user manual as slot indication:
	// PEG_TX/RX0..15 : slot 1
	// PCIE_TX/RX2..8 : slot 2..8
	for( i=1; i<=8; i++ ){
		switch( i ){
			// <pci-dev-nbr> | (<pci-func-nbr> << 5)
			case 1: pciBusPath[0] = 0x1;              break; // PEG
			case 2: pciBusPath[0] = 0x1c | (0x01<<5); break;
			case 3: pciBusPath[0] = 0x1c | (0x02<<5); break;
			case 4: pciBusPath[0] = 0x1c | (0x03<<5); break;
			case 5: pciBusPath[0] = 0x1c | (0x04<<5); break;
			case 6: pciBusPath[0] = 0x1c | (0x05<<5); break;
			case 7: pciBusPath[0] = 0x1c | (0x06<<5); break;
			case 8: pciBusPath[0] = 0x1c | (0x07<<5); break;
		}

		// _minSlot must be !=-1 so that hasPciBusPath() reports true
	    busIf1 = new PciBusInterface(
	        StdPciE, i, i, 0, &pciBusPath);

		// PEG
		if( i==1 )
			busIf1->setInstName( QString("PEG_TX/RX0..15") );
		else
			busIf1->setInstName( QString("PCIE_TX/RX %1").arg(i) );
		
		addChild( busIf1 );
	}

	/////////////////////////////////////
	// SMBus 
	{
	    ModBbisSmbPciGen* smbBbis = 0;
		Q3MemArray<uchar> smbBusPath(1);

		// <pci-dev-nbr> | (<pci-func-nbr> << 5)
	    smbBusPath[0] = 0x1f | (0x03<<5);
	    
	    // cannot store the pci bus path in the busIf because one busIf can have several
	    // smb controller children
        this->smbusIf = new PciBusInterface( LocalBus, -1, -1, 0, 0, false );
		this->smbusIf->setInstName( QString("Onboard SMB"));
		addChild( this->smbusIf );
        
        smbBbis = new ModBbisSmbPciGen("ICH");
        // set pci bus path
        smbBbis->setPciBusPath(smbBusPath);
        
    	this->smbBbisList.push_back(smbBbis);
		this->hasObSmb = true;	
	}
}

// -----------------------------------------------------------------
// CPU CB30C

//! Creates an CB30C CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuCb30c::ModCpuCb30c( bool withSubDevs ):
    ModCpuComExpr( QString("CB30C"), QString("PC")  )
{
	UTIL_UNREF_PARAM(withSubDevs);

	BusInterface	*busIf1;
	
    setDescription( "Rugged COM Express module with PowerPC QorIQ CPU" );
	lstCpuCores << PpcQorIQ;

	///////////////////////////////////////////////////
    // COM Express Interface
	// Note:
	// A COM Express carrier can be connected to this interface.
	// With the generic COM Express carrier (ModExtComExpr),
	// any PCI/PCIe device can be connected to the CPU, but the
	// PCI adressing must be done manually.
    addChild( newComExprBusIf( 0, true ) );

	///////////////////////////////////////////////////
    // PCIe interface at COM Express Connector 
	// Note:
	// This PCIe interface contains the CPU specific PCI adressing.
	// It can be used to connect PCIe devices to the provided
	// PCIe slot without further PCI configuration.
	Q3MemArray<uchar> pciBusPath0(0);
	Q3MemArray<uchar> pciBusPath2(2);

	////////////////////////
    // slot 0: domain 0, bus 0

	// _minSlot must be !=-1 so that hasPciBusPath() reports true
	busIf1 = new PciBusInterface(
        StdPciE, 0, 0, 0, &pciBusPath0, true, 0);

	busIf1->setInstName( QString("PCIE_TX/RX 0") );
	addChild( busIf1 );

	////////////////////////
    // slot 1: domain 1, bus 0

	// _minSlot must be !=-1 so that hasPciBusPath() reports true
	busIf1 = new PciBusInterface(
        StdPciE, 1, 1, 0, &pciBusPath0, true, 1);

	busIf1->setInstName( QString("PCIE_TX/RX 1") );
	addChild( busIf1 );

	////////////////////////
    // slot 2, domain 2, PCIe lane behind pericom bridge
	// <pci-dev-nbr> | (<pci-func-nbr> << 5)
	pciBusPath2[0] = 0x0; // first bridge within pericom bridge
	pciBusPath2[1] = 0x1; // second bridge within pericom bridge

	// _minSlot must be !=-1 so that hasPciBusPath() reports true
	busIf1 = new PciBusInterface(
        StdPciE, 2, 2, 0, &pciBusPath2, true, 1);

	busIf1->setInstName( QString("PCIE_TX/RX 2") );
	addChild( busIf1 );

	/////////////////////////////////////
	// SMBus 
	{
	    ModBbisSmbPciGen* smbBbis = 0;

		this->smbusIf = new PciBusInterface( LocalBus, -1, -1, 0, 0, false );
		this->smbusIf->setInstName( QString("Onboard SMB"));
		addChild( this->smbusIf );  
		smbBbis = new ModBbisSmbPciGen("NAT");
		this->smbBbisList.push_back(smbBbis);
		this->hasObSmb = true;	
	}
}

// -----------------------------------------------------------------
// CPU CC10

//! Creates an CC10 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuCc10::ModCpuCc10( SubModel submod, bool withSubDevs ):
    ModCpuComExpr( QString(""), QString("PC")  )
{
	UTIL_UNREF_PARAM(withSubDevs);

	BusInterface	*busIf1;
	QString descr;
	
	lstCpuCores << IMX6;

    descr = "Rugged COM Express module with ARM i.MX 6 CPU";

	_submod = submod;

	///////////////////////////////////////////////////
    // COM Express Interface
	// Note:
	// A COM Express carrier can be connected to this interface.
	// With the generic COM Express carrier (ModExtComExpr),
	// any PCI/PCIe device can be connected to the CPU, but the
	// PCI adressing must be done manually.
    addChild( newComExprBusIf( 0, true ) );

	///////////////////////////////////////////////////
    // PCIe interface at COM Express Connector 
	// Note:
	// This PCIe interface contains the CPU specific PCI adressing.
	// It can be used to connect PCIe devices to the provided
	// PCIe slot without further PCI configuration.
	Q3MemArray<uchar> pciBusPath1(1);
	Q3MemArray<uchar> pciBusPath3(3);
	Q3MemArray<uchar> pciBusPath4(4);

	switch( submod ){
		// CC10
		case CC10:
		{
			hwName += "CC10";

		    // slot 0: domain 0
			// PCIe lane behind PCIe Swidge
			// <pci-dev-nbr> | (<pci-func-nbr> << 5)
			pciBusPath1[0] = 0;

			busIf1 = new PciBusInterface(
				StdPciE, 0, 0, 0, &pciBusPath1, true, 0);
			break;
		}

		// CC10C
		// root@generic-armv7a-hf:~# lspci -t
		// -[0000:00]---00.0-[01-05]----00.0-[02-05]--+-01.0-[03]--			COM Express
		//                |                           +-02.0-[04]----00.0   FPGA
		//                |              |                 |           |
		//    PCI bridge in ARM CPU      +-Pericom bridges-+         FPGA       
		case CC10C:
		{
			hwName += "CC10C";
			descr += " and FPGA";

		    // slot 1:
			// PCIe lane behind PCIe Swidge
			// <pci-dev-nbr> | (<pci-func-nbr> << 5)
			pciBusPath3[0] = 0;
			pciBusPath3[1] = 0;
			pciBusPath3[2] = 1;

			busIf1 = new PciBusInterface(
				StdPciE, 1, 1, 0, &pciBusPath3, true);

			///////////////////////////////////////////////////
			// PCIe interface for FPGA 
			BusInterface	*busIfFpga=0;

		    // slot 0:
			// PCIe lane behind PCIe Swidge
			// <pci-dev-nbr> | (<pci-func-nbr> << 5)
			pciBusPath4[0] = 0;
			pciBusPath4[1] = 0;
			pciBusPath4[2] = 2;
			pciBusPath4[3] = 0; // FPGA PCI dev-nbr

		    // minSlot != -1 reports hasPciBusPath!
			busIfFpga = new PciBusInterface(
				StdPciE,		// ifType
				0, 0,			// min/maxSlot
				&pciBusPath4,	// basePciPath
				NULL,			// directIdTbl
				false);			// isExpandable

			busIfFpga->setEndianSwapping( true );
			busIfFpga->setInstName( QString("OnBoard PCI Bus"));
			addChild( busIfFpga );

			if( withSubDevs ){
				BbisDevice	*bbisDev=0;
				bbisDev = new ModBbisCham("PCITBL", pciBusPath4[3]); // FPGA slots 0..31
				bbisDev->setInstName( "FPGA" );
				busIfFpga->addChild( bbisDev );
			} // withSubDevs

			break;
		}
	}

	setHwName( hwName );
    setDescription( descr );

	busIf1->setInstName( QString("PCIE_TX/RX 0") );
	addChild( busIf1 );

	/////////////////////////////////////
	// SMBus 
	{
	    ModBbisSmbPciGen* smbBbis = 0;

		this->smbusIf = new PciBusInterface( LocalBus, -1, -1, 0, 0, false );
		this->smbusIf->setInstName( QString("Onboard SMB"));
		addChild( this->smbusIf );  
		smbBbis = new ModBbisSmbPciGen("NAT");
		this->smbBbisList.push_back(smbBbis);
		this->hasObSmb = true;	
	}
}

