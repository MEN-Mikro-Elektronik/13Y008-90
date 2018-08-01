/***************************************************************************
 */
/*!       \file  modcpuf9.cpp 
 *       \brief  Classes for model MEN F9 
 *      \author	 Ulrich.Bogensperger@men.de
 *		  $Date: 2016/02/20 16:27:07 $
 *    $Revision: 2.4 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuf9.cpp,v $
 * Revision 2.4  2016/02/20 16:27:07  ts
 * R: BoxPCs based on SC24 need 2 SMBusses in MDIS wizard
 * M: 1. added member nrSmbBusses to class CpuDeviceSmb
 *    2. added parameter int nrSmbBusses to every constructor of CPUs with SMB
 *       controllers: 2 for SC24 (AMD based) Box PCs, 1 for every other CPU
 *
 * Revision 2.3  2014/06/12 15:29:22  ts
 * R: new CPU F75P could not be selected
 * M: added F75P to models
 *
 * Revision 2.2  2011/09/26 08:43:18  dpfeuffer
 * R: F11S CAN devices need to get the PCI interrupt of FPGA
 * M: ModCpuF11S::ModCpuF11S() specifies now driver_pcitbl.mak for the chameleon BBIS
 *
 * Revision 2.1  2010/06/02 16:52:46  CRuff
 * R: support for new board F11S
 * M: added class implementation for new class ModCpuF11S
 *
 * Revision 2.0  2010/01/22 11:30:35  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.2  2009/04/27 17:21:14  CRuff
 * R: Provide CPU selection possibility for Intel CPUs
 * M: Added CPU cores Pentium/Pentium2/Pentium3/Pentium4 to F9
 *
 * Revision 1.1  2005/07/15 15:47:31  ub
 * Initial Revision
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>

#include "modcpuf9.h"
#include "wizdebug.h"
#include "wizexcept.h"
#include "modbbisisa.h"
#include "modbbissmbpcigen.h"
#include "modbbischam.h"
#include "util.h"

// -----------------------------------------------------------------
// CPU F9

//! Creates an F9 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the 
  BBIS devices
*/
ModCpuF9::ModCpuF9(bool withSubDevs ): 
	CpuDevice( QString("F9"), QString("F9"), false )
{
	BusInterface *busIf1;

	UTIL_UNREF_PARAM(withSubDevs);

	lstCpuCores << Pentium << Pentium2 << Pentium3 << Pentium4;

	setDescription("CompactPCI Pentium M CPU");	

	// Create the bus interface

    // Interface for CompactPci
    Q3MemArray<uchar> cpciBusPath(2);
    cpciBusPath[0] = 0x1e;
    cpciBusPath[1] = 0x04;

    busIf1 = new PciBusInterface( 
        Cpci, 2, 8, 0xf, &cpciBusPath);		// Interface for CompactPci
    addChild( busIf1 );
    busIf1->setInstName( QString("CompactPci Bus"));

}

Device *ModCpuF9::create(bool withSubDevs)
{
	return new ModCpuF9(withSubDevs);
}

// -----------------------------------------------------------------
// CPU F11S

//! Creates an F11S CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the 
  BBIS devices
*/

ModCpuF11S::ModCpuF11S(bool withSubDevs ): 
	CpuDeviceSmb( QString("F11S"), QString("F11S"), false, true, 1 )
{
	BusInterface *busIf1, *fpgaBusIf;
	ModBbisCham* bbis;
	ModBbisSmbPciGen* smbBbis;
	
	lstCpuCores << Atom;

	setDescription("CompactPCI Intel Atom CPU");	

	// Create the bus interfaces

    // Interface for CompactPci
    Q3MemArray<uchar> cpciBusPath(2);
    cpciBusPath[0] = 0x3c;
    cpciBusPath[1] = 0x00;

    busIf1 = new PciBusInterface( 
        Cpci, 2, 8, 0xf, &cpciBusPath);		// Interface for CompactPci
    addChild( busIf1 );
    busIf1->setInstName( QString("CompactPci Bus"));
    
    // create fpga bus interface
    Q3MemArray<uchar> fpgaPciBusPath(3);
    fpgaPciBusPath[0] = 0x3c;
    fpgaPciBusPath[1] = 0x00;
    fpgaPciBusPath[2] = 0x08;
    // create 1 slot to mark that fpga is not on bus 0 !
    fpgaBusIf = new PciBusInterface( StdPci, 0, 0, 0, &fpgaPciBusPath, false );
    fpgaBusIf->setInstName( QString("OnBoard PCI Bus")); //?
    addChild( fpgaBusIf );

    // Chameleon FPGA
    if( withSubDevs ){
        bbis = new ModBbisCham("PCITBL", 0x08);
        bbis->setInstName( "FPGA" );
        fpgaBusIf->addChild( bbis );
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
        
        smbBbis = new ModBbisSmbPciGen("SCH");
        // set pci bus path
        smbBbis->setPciBusPath(smbBusPath);
        
    	this->smbBbisList.push_back(smbBbis);
		
	}
}

Device *ModCpuF11S::create(bool withSubDevs)
{
	return new ModCpuF11S(withSubDevs);
}

// -----------------------------------------------------------------
// CPU F75P

//! Creates an F75P CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the 
  BBIS devices
*/


ModCpuF75P::ModCpuF75P(bool withSubDevs ): CpuDeviceSmb( QString("F75P"), QString("F75P"), false, true, 1 )
{
  BusInterface *busIf1, *fpgaBusIf;
  ModBbisCham* bbis;
  ModBbisSmbPciGen* smbBbis;
  
  lstCpuCores << Atom;

  setDescription("3U CompactPCI PlusIO Safe Railway Computer");
  
  // Create the bus interfaces

    // Interface for CompactPci
    Q3MemArray<uchar> cpciBusPath(2);
    cpciBusPath[0] = 0x3c;
    cpciBusPath[1] = 0x00;

    busIf1 = new PciBusInterface( 
        Cpci, 2, 8, 0xf, &cpciBusPath);		// Interface for CompactPci
    addChild( busIf1 );
    busIf1->setInstName( QString("CompactPci Bus"));
    
    // create fpga bus interface
    Q3MemArray<uchar> fpgaPciBusPath(3);
    fpgaPciBusPath[0] = 0x3c;
    fpgaPciBusPath[1] = 0x00;
    fpgaPciBusPath[2] = 0x08;
    // create 1 slot to mark that fpga is not on bus 0 !
    fpgaBusIf = new PciBusInterface( StdPci, 0, 0, 0, &fpgaPciBusPath, false );
    fpgaBusIf->setInstName( QString("OnBoard PCI Bus")); //?
    addChild( fpgaBusIf );

    // Chameleon FPGA
    if( withSubDevs ){
        bbis = new ModBbisCham("PCITBL", 0x08);
        bbis->setInstName( "FPGA" );
        fpgaBusIf->addChild( bbis );
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
      
      smbBbis = new ModBbisSmbPciGen("SCH");
      // set pci bus path
      smbBbis->setPciBusPath(smbBusPath);
        
      this->smbBbisList.push_back(smbBbis);
		
    }
}

Device *ModCpuF75P::create(bool withSubDevs)
{
	return new ModCpuF75P(withSubDevs);
}


