/***************************************************************************
 */
/*!       \file  modcpusc24.cpp 
 *       \brief  Classes for model MEN SC24 
 *      \author	 dieter.pfeuffer@men.de
 *		  $Date: 2016/02/24 17:21:08 $
 *    $Revision: 2.7 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpusc24.cpp,v $
 * Revision 2.7  2016/02/24 17:21:08  ts
 * R: SC25 based Box Computers dont need 2 onBoard SMBusses (like SC24), just 1
 * M: changed parameter for SMB bus count from 2 to 1
 *
 * Revision 2.6  2016/02/20 16:27:16  ts
 * R: BoxPCs based on SC24 need 2 SMBusses in MDIS wizard
 * M: 1. added member nrSmbBusses to class CpuDeviceSmb
 *    2. added parameter int nrSmbBusses to every constructor of CPUs with SMB
 *       controllers: 2 for SC24 (AMD based) Box PCs, 1 for every other CPU
 *
 * Revision 2.5  2015/05/26 17:20:03  ts
 * R: gituser autocheckin: updated source
 *
 * Revision 2.4  2013/03/04 13:03:04  dpfeuffer
 * R: open existing project with SC24 CPU doesn't work
 * M: hw-name changed from SC24_FPGA to CHAMELEON_ISA
 *
 * Revision 2.3  2012/08/07 18:46:59  ts
 * R: Box Computer models were missing together with SC24
 * M: expanded model name SC24 to SC24_BC2_BCxx
 *
 * Revision 2.2  2012/04/25 17:42:38  ts
 * R: opening MDIS devices on SC24 FPGA failed due to outdated IO mapped address
 * M: use Address 0xe000e000(=base 0xe0000000+0xe000) in SetIsaAddress()
 *
 * Revision 2.1  2011/09/26 09:17:48  dpfeuffer
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2011 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>

#include "modcpusc24.h"
#include "wizdebug.h"
#include "wizexcept.h"
#include "modbbisisa.h"
#include "modbbissmbpcigen.h"
#include "modbbischamisa.h"
#include "util.h"

// -----------------------------------------------------------------
// CPU SC24

//! Creates an SC24 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the 
  BBIS devices
*/
ModCpuDc15::ModCpuDc15(bool withSubDevs ):CpuDeviceSmb( QString("DC15"), QString("SC24"), false, true, 2 )
{
	BusInterface *busIf2;
	ModBbisSmbPciGen* smbBbis;
	
	lstCpuCores << Pentium4 << Athlon;

	setDescription("Rugged 10.4\" Panel PC");

	// Interface for SC24 FPGA @ LPC bus	
	busIf2 = new BusInterface( LocalBus, false );
	addChild( busIf2 );

	if( withSubDevs ) {
		ModBbisDc15 *isaBbis = new ModBbisDc15( true );   // local BBIS dev
		busIf2->addChild( isaBbis );
	}
    
    // SMBus interface
	{
		Q3MemArray<uchar> smbBusPath(1);
	    smbBusPath[0] = 0x1f;
	    
	    // cannot store the pci bus path in the busIf because one busIf can have several
	    // smb controller children
        this->smbusIf = new PciBusInterface( LocalBus, -1, -1, 0, 0, false );
		this->smbusIf->setInstName( QString("Onboard SMB"));
		addChild( this->smbusIf );
        
        smbBbis = new ModBbisSmbPciGen("FCH");
        // set pci bus path
        smbBbis->setPciBusPath(smbBusPath);
        
    	this->smbBbisList.push_back(smbBbis);
		
	}
}

Device *ModCpuDc15::create(bool withSubDevs)
{
	return new ModCpuDc15(withSubDevs);
}

// -----------------------------------------------------------------
// BBIS SC24

ModBbisDc15::ModBbisDc15(bool withSubDevs) : ModBbisChamIsa()
{
	UTIL_UNREF_PARAM(withSubDevs);

	setHwName("CHAMELEON_ISA");
	// setIsaAddress( 0xe000, true );
	// ts: changed to memmapped, only needed for Linux, VxWorks
	setIsaAddress( 0xe000e000, false );
	setInstName( "SC24_FPGA" );
	setInstNameChangeAllowed( false );
}

/////////////////////////////////////////////////////////////////////////
// ts@men: the Box Computer series Bx50x is based on AMD SC24, 
// the Bx70x on intel SC25. The SBCs are not visible to customers, just
// the Boxcomputers. According to Team I the FPGA is same on both families.
//


///////// Bx50x (contains SC24) ///
// -----------------------------------------------------------------
// CPU Bx50x

//! Creates an SC25 based Box computer device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the 
  BBIS devices
*/
ModCpuBx50x::ModCpuBx50x(bool withSubDevs ): 
	CpuDeviceSmb( QString("Bx50x"), QString("SC24"), false, true, 2 )
{
	BusInterface *busIf2;
	ModBbisSmbPciGen* smbBbis;
	
	lstCpuCores << Pentium4 << Athlon;

	setDescription("AMD based Box Computers");	

	// Interface for SC24 FPGA @ LPC bus	
	busIf2 = new BusInterface( LocalBus, false );
	addChild( busIf2 );

	if( withSubDevs ){
		ModBbisBx50x *isaBbis = new ModBbisBx50x( true );   // local BBIS dev
		busIf2->addChild( isaBbis );
	}
    
    // SMBus interface
	{
	  Q3MemArray<uchar> smbBusPath(1);
	    smbBusPath[0] = 0x1f;
	    
	    // cannot store the pci bus path in the busIf because one busIf can have several
	    // smb controller children
	    this->smbusIf = new PciBusInterface( LocalBus, -1, -1, 0, 0, false );
	    this->smbusIf->setInstName( QString("Onboard SMB"));
	    addChild( this->smbusIf );
        
	    smbBbis = new ModBbisSmbPciGen("FCH");
	    // set pci bus path
	    smbBbis->setPciBusPath(smbBusPath);
	    this->smbBbisList.push_back(smbBbis);

	}
}

Device *ModCpuBx50x::create(bool withSubDevs)
{
	return new ModCpuBx50x(withSubDevs);
}

// -----------------------------------------------------------------
// BBIS Bx50x

ModBbisBx50x::ModBbisBx50x(bool withSubDevs) : ModBbisChamIsa()
{
	UTIL_UNREF_PARAM(withSubDevs);

	setHwName("CHAMELEON_ISA");
	// setIsaAddress( 0xe000, true );
	// ts: changed to memmapped, only needed for Linux, VxWorks
	setIsaAddress( 0xe000e000, false );
	setInstName( "SC24_FPGA" );
	setInstNameChangeAllowed( false );
}

// -----------------------------------------------------------------
// CPU Bx51x

//! Creates an SC31 based Box computer device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuBx51x::ModCpuBx51x(bool withSubDevs ):
	CpuDeviceSmb( QString("Bx51x"), QString("SC31"), false, true, 1 )
{
	BusInterface *busIf2;
	ModBbisSmbPciGen* smbBbis;

	lstCpuCores << Pentium4 << Athlon;

	setDescription("Intel based Box Computers");

	// Interface for SC31 FPGA @ LPC bus
	busIf2 = new BusInterface( LocalBus, false );
	addChild( busIf2 );

	if( withSubDevs ){
		ModBbisBx51x *isaBbis = new ModBbisBx51x( true );   // local BBIS dev
		busIf2->addChild( isaBbis );
	}

    // SMBus interface
	{
		Q3MemArray<uchar> smbBusPath(1);
	    smbBusPath[0] = 0x1f;

	    // cannot store the pci bus path in the busIf because one busIf can have several
	    // smb controller children
	    this->smbusIf = new PciBusInterface( LocalBus, -1, -1, 0, 0, false );
	    this->smbusIf->setInstName( QString("Onboard SMB"));
	    addChild( this->smbusIf );

	    smbBbis = new ModBbisSmbPciGen("FCH");
	    // set pci bus path
	    smbBbis->setPciBusPath(smbBusPath);
	    this->smbBbisList.push_back(smbBbis);
	}
}

Device *ModCpuBx51x::create(bool withSubDevs)
{
	return new ModCpuBx51x(withSubDevs);
}

// -----------------------------------------------------------------
// BBIS Bx51x

ModBbisBx51x::ModBbisBx51x(bool withSubDevs) : ModBbisChamIsa()
{
	UTIL_UNREF_PARAM(withSubDevs);

	setHwName("CHAMELEON_ISA");
	// setIsaAddress( 0xe000, true );
	// ts: changed to memmapped, only needed for Linux, VxWorks
	setIsaAddress( 0xe000e000, false );
	setInstName( "SC31_FPGA" );
	setInstNameChangeAllowed( false );
}

// -----------------------------------------------------------------
// CPU Bx70x

//! Creates an SC25 based Box computer device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the 
  BBIS devices
*/
ModCpuBx70x::ModCpuBx70x(bool withSubDevs ): 
	CpuDeviceSmb( QString("Bx70x"), QString("SC25"), false, true, 1 )
{
	BusInterface *busIf2;
	ModBbisSmbPciGen* smbBbis;
	
	lstCpuCores << Pentium4 << Athlon;

	setDescription("Intel based Box Computers");	

	// Interface for SC25 FPGA @ LPC bus	
	busIf2 = new BusInterface( LocalBus, false );
	addChild( busIf2 );

	if( withSubDevs ){
		ModBbisBx70x *isaBbis = new ModBbisBx70x( true );   // local BBIS dev
		busIf2->addChild( isaBbis );
	}
    
    // SMBus interface
	{
		Q3MemArray<uchar> smbBusPath(1);
	    smbBusPath[0] = 0x1f;
	    
	    // cannot store the pci bus path in the busIf because one busIf can have several
	    // smb controller children
	    this->smbusIf = new PciBusInterface( LocalBus, -1, -1, 0, 0, false );
	    this->smbusIf->setInstName( QString("Onboard SMB"));
	    addChild( this->smbusIf );
        
	    smbBbis = new ModBbisSmbPciGen("FCH");
	    // set pci bus path
	    smbBbis->setPciBusPath(smbBusPath);
	    this->smbBbisList.push_back(smbBbis);		
	}
}

Device *ModCpuBx70x::create(bool withSubDevs)
{
	return new ModCpuBx70x(withSubDevs);
}

// -----------------------------------------------------------------
// BBIS Bx70x

ModBbisBx70x::ModBbisBx70x(bool withSubDevs) : ModBbisChamIsa()
{
	UTIL_UNREF_PARAM(withSubDevs);

	setHwName("CHAMELEON_ISA");
	// setIsaAddress( 0xe000, true );
	// ts: changed to memmapped, only needed for Linux, VxWorks
	setIsaAddress( 0xe000e000, false );
	setInstName( "SC24_FPGA" );
	setInstNameChangeAllowed( false );
}
