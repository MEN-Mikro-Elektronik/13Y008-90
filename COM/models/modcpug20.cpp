/***************************************************************************
 */
/*!       \file  modcpug20.cpp
 *       \brief  Classes for model MEN G20
 *      \author	 dieter.pfeuffer@men.de
 *		  $Date: 2016/02/20 16:27:09 $
 *    $Revision: 2.4 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpug20.cpp,v $
 * Revision 2.4  2016/02/20 16:27:09  ts
 * R: BoxPCs based on SC24 need 2 SMBusses in MDIS wizard
 * M: 1. added member nrSmbBusses to class CpuDeviceSmb
 *    2. added parameter int nrSmbBusses to every constructor of CPUs with SMB
 *       controllers: 2 for SC24 (AMD based) Box PCs, 1 for every other CPU
 *
 * Revision 2.3  2014/08/22 15:57:27  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.2  2014/06/06 14:19:08  ts
 * R: new CPU G22 was missing
 * M: added G22 (same device as G20 but should appear separately for customers)
 *
 * Revision 2.1  2011/09/26 09:17:45  dpfeuffer
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2006 - 2011 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>

#include "modcpug20.h"
#include "wizdebug.h"
#include "wizexcept.h"
#include "modbbisisa.h"
#include "modbbissmbpcigen.h"
#include "propertiesdlg.h"

#include "util.h"

#define MAX_PCISER_SLOTS	7	// peripheral slot 2..8


//! Creates a "generic CompactPCI Serial PC" device
/*!
  including the bus interface (CPCI)
*/
// -----------------------------------------------------------------
// CPU G20

//! Creates an G20 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuG20::ModCpuG20( bool withSubDevs ):CpuDeviceSmb( QString("G20"), QString("PC"), false, true, 1 )
{
	BusInterface *busIf1;
	int i;

	UTIL_UNREF_PARAM(withSubDevs);

	lstCpuCores << I386 << I486 << Pentium << Pentium2 << Pentium3 <<
		Pentium4 << PentiumM << Duron << Athlon << Crusoe << Atom;

	setDescription("CompactPCI Serial Intel Core i7 CPU");
	setInstName("CPU");
	setVendorName(QString::null);

	// Create the bus interfaces
	for( i=2; i<MAX_PCISER_SLOTS+2; i++ ){
		busIf1 = new PciBusInterface( CpciSer );
		addChild( busIf1 );
		busIf1->setInstName( QString("CompactPCI Serial slot %1").arg(i) );
	}
}


// -----------------------------------------------------------------
// CPU G22

//! Creates an G22 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuG22::ModCpuG22( bool withSubDevs ):
	CpuDeviceSmb( QString("G22"), QString("PC"), false, true, 1 )
{	
	BusInterface *busIf1=0;
	ModBbisSmbPciGen* smbBbis = 0;
	int i;

	UTIL_UNREF_PARAM(withSubDevs);

	lstCpuCores << I386 << I486 << Pentium << Pentium2 << Pentium3 <<
		Pentium4 << PentiumM << Duron << Athlon << Crusoe << Atom;

	setDescription("CompactPCI Serial Intel Core i7 CPU");
	setInstName("CPU");	
    
	// Create the bus interfaces

	/////////////////////////////////////
	// Interface for CompactPci Serial
	// ts@men: Theres no chance anymore for SW to automatically set the
	// location for a certain G2xx card depending on her slot. So, leave
	// _minSlot =-1 so that hasPciBusPath() reports false, this causes
	// the "Mezzanine settings" Tab to appear in the properties of the mezzanine card.

	for( i=2; i<MAX_PCISER_SLOTS+2; i++ ){
		busIf1 = new PciBusInterface( CpciSer );
		busIf1->setInstName( QString("CompactPCI Serial slot %1").arg(i) );
		addChild( busIf1 );
	}

	/////////////////////////////////////
	// SMBus 
	{
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
		
	}
}

SwModuleList *
ModCpuG22::enumSwModules()
{
	SwModule *swMod;
	SwModuleTypes::ModType type = SwModuleTypes::NativeDriver;
	swMod = new SwModule( type, "G22_g2x", "DRIVERS/Z001_SMB/driver_g2x.mak",
						 "G22 special IO mapped driver" );

	if( lstSwModules == 0 && swMod )
		lstSwModules = new SwModuleList;

	if( lstSwModules && swMod )
	{
		lstSwModules->add( swMod );
	}
	else if ( swMod )
	{
		delete swMod;
		return lstSwModules;
	}
	else
	{
		return lstSwModules;
	}

	swMod = new SwModule( type, "men_lx_chameleon", "DRIVERS/CHAMELEON/driver.mak",
						 "Linux native chameleon driver" );
	if( swMod )
	{
		lstSwModules->add( swMod );
	}

	return lstSwModules;
}
// -----------------------------------------------------------------
// CPU G23

//! Creates an G23 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuG23::ModCpuG23( bool withSubDevs ):
	CpuDeviceSmb( QString("G23"), QString("PC"), false, true, 1 )
{	
	BusInterface *busIf1=0;
	ModBbisSmbPciGen* smbBbis = 0;
	int i;

	UTIL_UNREF_PARAM(withSubDevs);

	lstCpuCores << Pentium4;

	setDescription("CompactPCI Serial Intel Core i7 4th Gen. CPU");
	setInstName("CPU");	
    
	// Create the bus interfaces

	/////////////////////////////////////
	// Interface for CompactPci Serial
	// ts@men: Theres no chance anymore for SW to automatically set the
	// location for a certain G2xx card depending on her slot. So, leave
	// _minSlot =-1 so that hasPciBusPath() reports false, this causes
	// the "Mezzanine settings" Tab to appear in the properties of the mezzanine card.

	for( i=2; i<MAX_PCISER_SLOTS+2; i++ ){
		busIf1 = new PciBusInterface( CpciSer );
		busIf1->setInstName( QString("CompactPCI Serial slot %1").arg(i) );
		addChild( busIf1 );
	}

	/////////////////////////////////////
	// SMBus 
	{
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
		
	}
}


// -----------------------------------------------------------------
// CPU G25A

//! Creates an G25A CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuG25A::ModCpuG25A( bool withSubDevs ):
	CpuDeviceSmb( QString("G25A"), QString("PC"), false, true, 1 )
{	
	BusInterface *busIf1=0;
	ModBbisSmbPciGen* smbBbis = 0;
	int i;

	UTIL_UNREF_PARAM(withSubDevs);

	lstCpuCores << Pentium4;

	setDescription("CompactPCI Serial Intel Xeon D CPU");
	setInstName("CPU");	
    
	// Create the bus interfaces

	/////////////////////////////////////
	// Interface for CompactPci Serial
	// ts@men: Theres no chance anymore for SW to automatically set the
	// location for a certain G2xx card depending on her slot. So, leave
	// _minSlot =-1 so that hasPciBusPath() reports false, this causes
	// the "Mezzanine settings" Tab to appear in the properties of the mezzanine card.

	for( i=2; i<MAX_PCISER_SLOTS+2; i++ ){
		busIf1 = new PciBusInterface( CpciSer );
		busIf1->setInstName( QString("CompactPCI Serial slot %1").arg(i) );
		addChild( busIf1 );
	}

	/////////////////////////////////////
	// SMBus 
	{
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
		
	}
}

// -----------------------------------------------------------------
// CPU G28

//! Creates an G28 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuG28::ModCpuG28( bool withSubDevs ):
	CpuDeviceSmb( QString("G28"), QString("PC"), false, true, 1 )
{
	BusInterface *busIf1=0;
	ModBbisSmbPciGen* smbBbis = 0;
	int i;

	UTIL_UNREF_PARAM(withSubDevs);

	lstCpuCores << Pentium4;

	setDescription("CompactPCI Serial Intel Xeon W CPU");
	setInstName("CPU");

	// Create the bus interfaces

	/////////////////////////////////////
	// Interface for CompactPci Serial
	// ts@men: Theres no chance anymore for SW to automatically set the
	// location for a certain G2xx card depending on her slot. So, leave
	// _minSlot =-1 so that hasPciBusPath() reports false, this causes
	// the "Mezzanine settings" Tab to appear in the properties of the mezzanine card.

	for( i=2; i<MAX_PCISER_SLOTS+2; i++ ){
		busIf1 = new PciBusInterface( CpciSer );
		busIf1->setInstName( QString("CompactPCI Serial slot %1").arg(i) );
		addChild( busIf1 );
	}

	/////////////////////////////////////
	// SMBus
	{
		Q3MemArray<uchar> smbBusPath(1);
		// <pci-dev-nbr> | (<pci-func-nbr> << 5)
		smbBusPath[0] = 0x1f | (0x04<<5);

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
