/***************************************************************************
 */
/*!       \file  modcpua21.cpp
 *       \brief  Classes for model MEN A21
 *      \author	 Dieter.Pfeuffer@men.de
 *		  $Date: 2016/04/29 10:43:31 $
 *    $Revision: 2.7 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpua21.cpp,v $
 * Revision 2.7  2016/04/29 10:43:31  ts
 * R: the A21b and c were named "A21A21b/c" in CPU View
 * M: removed 1st add of CPU name to model, was leftover from adding MSI variant
 *
 * Revision 2.6  2016/02/20 16:26:51  ts
 * R: BoxPCs based on SC24 need 2 SMBusses in MDIS wizard
 * M: 1. added member nrSmbBusses to class CpuDeviceSmb
 *    2. added parameter int nrSmbBusses to every constructor of CPUs with SMB
 *       controllers: 2 for SC24 (AMD based) Box PCs, 1 for every other CPU
 *
 * Revision 2.5  2015/05/26 17:18:47  ts
 * R: gituser autocheckin: updated source
 *
 * Revision 2.4  2014/08/22 15:56:52  dpfeuffer
 * R:1. SMB interface for internal SMB controller missing
 *   2. inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M:1.a) ModCpuA21 now derived from CpuDeviceSmb
 *     b) SMB interface for SMBPCI_NAT added
 *   2. PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.3  2014/07/18 15:12:28  dpfeuffer
 * R:1. saved A21c configuration produces error at open
 *   2. erroneous PMC support, missing ComExpress support, general maintenance
 * M:1. ModCpuA21 constructor fixed
 *   2. intermediate check-in during development
 *
 * Revision 2.2  2014/06/06 14:20:08  ts
 * R: small typo in comments corrected "isExpandable" instead "isExpanable"
 *
 * Revision 2.1  2013/03/04 13:07:29  dpfeuffer
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2013 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>
#include "modcpua21.h"
#include "modbbischam.h"
#include "wizdebug.h"
#include "resourcemng.h"
#include "modbbispcimmod.h"
#include "util.h"
#include "modbbissmbpcigen.h"

class ModVmeIfA21 : public VmeBusInterface
{
public:
	ModVmeIfA21();
	virtual ulong supportedIrqLevels();
};

// -----------------------------------------------------------------
// CPU A21

//! Creates an A21 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuA21::ModCpuA21(SubModel submod,
					 Mezzanines mezzanines,
					 bool withSubDevs ):
	CpuDeviceSmb( QString("A21"), QString("A21"), false, true, 1 )
{
	QString			hwName;
	QString			descr;

	_submod = submod;
	_mezzanines = mezzanines;

	lstCpuCores << PpcQorIQ;

	/////////////////////////////////////////////////////////////
	// set HW name and description

	descr += "PowerPC QorIQ CPU ";

	switch( _submod ){
	case A21: 	
		hwName += "A21"; 	
		descr = "VME "; 
		switch( _mezzanines )
		{
		case WithMmods:
			hwName += "b";
			descr += "with 3 M-Module slots";
			break;
		case WithPmcs:
			hwName += "c";
			descr += "with 2 PMC/XMC slots";
			break;
		}

		break;
	case A21Msi:
		hwName += "A21_MSI";
		descr = "Customer VME "; 
		switch( _mezzanines ){
		case WithMmods:
			hwName += "_b";
			descr += "with 3 M-Module slots and MSI support";
			break;
		case WithPmcs:
			hwName += "_c";
			descr += "with 2 PMC/XMC slots and MSI support";
		break;
		}
		break;
	}

	setHwName( hwName );
	setDescription(descr);

	/////////////////////////////////////////////////////////////
	// VME interface
	{
		BusInterface	*busIfVme=0;

		busIfVme = new ModVmeIfA21();					
		busIfVme->setInstName( QString("VME"));
		addChild( busIfVme );
	}

	/////////////////////////////////////////////////////////////
	// PCI interface for FPGA
	// 1 PCIe slot, bus-nbr 0, dev-nbr 0, not expandable, PCI Domain 2
	{
		BusInterface	*busIfFpga=0;

		Q3MemArray<uchar> pciBusPath(1); // path to FPGA (including FPGA PCI dev-nbr!)
	    pciBusPath[0] = 0; // FPGA PCI dev-nbr

		busIfFpga = new PciBusInterface(
			StdPci,			// ifType
			-1, -1,			// min/maxSlot
			&pciBusPath,	// basePciPath
			NULL,			// directIdTbl
			false,			// isExpandable
			2 );			// pciDomain

		busIfFpga->setEndianSwapping( true );
		busIfFpga->setInstName( QString("OnBoard PCI Bus"));
		addChild( busIfFpga );

		switch( _submod ){
		case A21: 	
		if( withSubDevs ){
			BbisDevice	*bbisDev=0;
			bbisDev = new ModBbisCham("PCITBL", pciBusPath[0]); // FPGA slots 0..31
			bbisDev->setInstName( "FPGA" );
			busIfFpga->addChild( bbisDev );
		} // withSubDevs
			break;
		case A21Msi:
			if( withSubDevs ){
				BbisDevice	*bbisDev=0;
				bbisDev = new ModBbisCham("PCITBL_MSI", pciBusPath[0]); // FPGA slots 0..31, use MSI variant
				bbisDev->setInstName( "FPGA" );
				busIfFpga->addChild( bbisDev );
			} // withSubDevs
			break;
		}
	}

	/////////////////////////////////////////////////////////////
	// Mezzanine interface
	switch( _mezzanines ){

	// A21b with M-Module slots
	case WithMmods:
	{
		BusInterface	*busIfMezz=0;

		// PCI interface for A21 BBIS
		// PCI dev location hard coded in A21 BBIS!
		busIfMezz = new PciBusInterface(
			LocalBus,		// ifType
			0, 0,			// min/maxSlot
			NULL,			// basePciPath
			NULL,			// directIdTbl
			false);			// pciDomain

		busIfMezz->setInstName( QString("PCI for MMOD"));
		busIfMezz->setEndianSwapping( true );
		addChild( busIfMezz );

		switch( _submod ) {
		case A21:
		if( withSubDevs ){
			BbisDevice	*bbisDev=0;
			bbisDev = new ModBbisA21();		
			busIfMezz->addChild( bbisDev );
		} // withSubDevs
			break;
		case A21Msi:
			if( withSubDevs ){
				BbisDevice	*bbisDev=0;
				bbisDev = new ModBbisA21Msi();		
				busIfMezz->addChild( bbisDev );
			} // withSubDevs
			break;
		}

		break;
	}

	// A21c with PMC/XMC slots
	case WithPmcs:
	{
		BusInterface *busIfMez=0;
		Q3MemArray<uchar> pciBusPath1(1);

		// PMC slot 0/1
		pciBusPath1[0] = 0x0;

		Q3MemArray<uchar> pciBusTbl(2);
		pciBusTbl[0] = 0x3;
		pciBusTbl[1] = 0x2;

		busIfMez = new PciBusInterface(
			Pmc,			// ifType
			0, 1,			// _minSlot, _maxSlot
			&pciBusPath1,	// _basePciPath
			&pciBusTbl,		// _directIdTbl
			true,			// isExpandable
			1 );			// pciDomain
		busIfMez->setEndianSwapping( true );
		busIfMez->setInstName(QString("onboard PMC"));
		addChild(busIfMez);

		// XMC slot 0/1
		pciBusPath1[0] = 0x0; // [1,0,0] P2P-Bridge to [2,0,0]     

		// XMC slot 0
		// PCI-Domain-1 --<rel-Bus-1>-- [1,0,0] P2P-Bridge to [2,0,0]
		//              --<rel-Bus-2>-- [2,1,0] P2P-Bridge to [3,0,0]
		//              --<rel-Bus-3>-- [3,0,0] XMC-slot-0
		busIfMez = new PciBusInterface(
			Xmc,			// ifType
			0, 0, 0x1,		// _minSlot, _maxSlot, _minSlotDevId ([2,1,0] P2P-Bridge to [3,0,0])
			&pciBusPath1,	// basePciPath
			true,			// isExpandable
			1 );			// pciDomain

		busIfMez->setInstName( QString("onboard XMC0"));
		addChild( busIfMez );

		// XMC slot 1
		// PCI-Domain-0 --<rel-Bus-1>-- [1,0,0] P2P-Bridge to [2,0,0]
		//              --<rel-Bus-2>-- [2,1,0] P2P-Bridge to [3,0,0]
		//              --<rel-Bus-3>-- [3,0,0] XMC-slot-1
		busIfMez = new PciBusInterface(
			Xmc,			// ifType
			0, 0, 0x1,		// _minSlot, _maxSlot, _minSlotDevId ([2,1,0] P2P-Bridge to [3,0,0])
			&pciBusPath1,	// basePciPath
			true,			// isExpandable
			0 );			// pciDomain

		busIfMez->setInstName( QString("onboard XMC1"));
		addChild( busIfMez );

		break;
	}

	} // switch

	/////////////////////////////////////////////////////////////
	// SMB interface
	{
	    ModBbisSmbPciGen* smbBbis = 0;

		this->smbusIf = new PciBusInterface( LocalBus, -1, -1, 0, 0, false );
		this->smbusIf->setInstName( QString("Onboard SMB"));
		addChild( this->smbusIf );  
		smbBbis = new ModBbisSmbPciGen("NAT");
		this->smbBbisList.push_back(smbBbis);
	}
}

Device* ModCpuA21::create(bool withSubDevs)
{
	return new ModCpuA21(_submod, _mezzanines, withSubDevs);
}

SwModuleList* ModCpuA21::enumSwModules()
{
	CpuDevice::enumSwModules();
	SwModuleList *addLst = enumSwModulesForExternalPackage( "PLDZ002" );

	if( lstSwModules == 0 && addLst )
		lstSwModules = new SwModuleList;

	if( lstSwModules && addLst )
		lstSwModules->add( addLst );

	return lstSwModules;
}

// -----------------------------------------------------------------
// BBIS A21 (for M-Modules)

//! Creates an A21 BBIS device
ModBbisA21::ModBbisA21(bool withSubDevs) : BbisDevice( QString("A21"), QString("A21"), false, LocalBus ) 
{
	BbisSlot *bbSlot;
	UTIL_UNREF_PARAM(withSubDevs);

	setInstName( QString("OB_MMODS") );
	setDescription("Pseudo device for onboard M-Modules slots");
	// create slots
	for( int slotNum=0; slotNum<3; slotNum++ ){
		bbSlot = new BbisSlot( slotNum, MmodD32, MmodA24 );
		addChild( bbSlot );
	}
}

//class BbisDevice : public Device
//{
//public:
//	BbisDevice( const QString &_hwName, const QString &_driverName,	bool isRemoveable, BusIfType _ifType );
// ts@men: enhgancement for MSI A21 model 

//! Creates an A21 BBIS device with MSI support
ModBbisA21Msi::ModBbisA21Msi(bool withSubDevs) : BbisDevice( QString("A21"), QString("A21_MSI"), false, LocalBus )
{
	BbisSlot *bbSlot;
	UTIL_UNREF_PARAM(withSubDevs);

	setInstName( QString("OB_MMODS") );
	setDescription("Pseudo device for onboard M-Modules slots with MSI support");

	// create slots
	for( int slotNum=0; slotNum<3; slotNum++ ){
		bbSlot = new BbisSlot( slotNum, MmodD32, MmodA24 );
		addChild( bbSlot );
	}
}

SwModuleList *ModBbisA21::enumSwModules()
{
	if( lstSwModules == 0 ){
		lstSwModules = new SwModuleList;

		// always ignore byteswapping (to generate driver.mak)
		lstSwModules->add( makeStdDriverSwModule("A21", "", true ));
	}
	return lstSwModules;
}

//! as Device::getDriverName, but always returns non-swapped variant
QString ModBbisA21::getDriverName( bool fullName, bool withSw)
{
	UTIL_UNREF_PARAM(withSw);
	return Device::getDriverName( fullName, false );
}

//! as Device::getDriverName, but always returns non-swapped variant
QString ModBbisA21Msi::getDriverName( bool fullName, bool withSw)
{
	UTIL_UNREF_PARAM(withSw);
	return Device::getDriverName( fullName, false );
}

SwModuleList *ModBbisA21Msi::enumSwModules()
{
	if( lstSwModules == 0 ){
		lstSwModules = new SwModuleList;

		// always ignore byteswapping (to generate driver_msi.mak)
		// ts:  this results in        base directory | Variant -> driver'_msi'.mak, no '_' needed
		lstSwModules->add( makeStdDriverSwModule("A21", "MSI", true ));
	}
	return lstSwModules;
}

// -----------------------------------------------------------------
// VME Bus If A21

ModVmeIfA21::ModVmeIfA21() : VmeBusInterface()
{
	vmeA16Res->addRegion( 0x0000, 0x10000 );
	vmeA24Res->addRegion( 0x000000, 0x1000000 );
	vmeIrqVectorRes->addRegion( 0x80, 0x7e );
	setInstName( "VMEbus" );
}

ulong
ModVmeIfA21::supportedIrqLevels()
{
	return 0xfe; // level 7-1
}




