/***************************************************************************
 */
/*!       \file  modcpuesmini.cpp
 *       \brief  MEN ESMini classes
 *      \author  christine.ruff@men.de
 *        $Date: 2016/02/20 16:27:03 $
 *    $Revision: 2.4 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuesmini.cpp,v $
 * Revision 2.4  2016/02/20 16:27:03  ts
 * R: BoxPCs based on SC24 need 2 SMBusses in MDIS wizard
 * M: 1. added member nrSmbBusses to class CpuDeviceSmb
 *    2. added parameter int nrSmbBusses to every constructor of CPUs with SMB
 *       controllers: 2 for SC24 (AMD based) Box PCs, 1 for every other CPU
 *
 * Revision 2.3  2011/09/26 08:43:16  dpfeuffer
 * R: MM1 CAN devices need to get the PCI interrupt of FPGA
 * M: ModCpuMm1::ModCpuMm1() specifies now driver_pcitbl.mak for the chameleon BBIS
 *
 * Revision 2.2  2010/07/12 10:08:17  dpfeuffer
 * R: compiler warning under Windows for mdiswiz_lib.lib
 * M: UTIL_UNREF_PARAM added to unreferenced parameters
 *
 * Revision 2.1  2010/06/08 11:19:27  CRuff
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>
#include <Q3PtrList>

#include "modcpuesmini.h"
#include "modbbischam.h"
#include "resourcemng.h"
#include "wizdebug.h"
#include "modbbissmbpcigen.h"
#include "modextf503.h"

ModCpuEsmini::ModCpuEsmini( QString _hwName, QString _driverName  ):
    CpuDeviceSmb( _hwName, _driverName, false, false, 1 )
{
}

BusInterface*
ModCpuEsmini::newEsminiBusIf( const Q3MemArray<uchar>* pciBusPath, bool endianSwapping )
{
    // defineESM PCI bus interface
    BusInterface *esminiBusIf;
    
    esminiBusIf = new PciBusInterface( ESMini, 0, 0, pciBusPath, 0, true );
    esminiBusIf->setInstName( QString("ESMini PCI Connector"));
    esminiBusIf->setEndianSwapping( endianSwapping );

    return( esminiBusIf );
}


// -----------------------------------------------------------------
// CPU MM1

//! Creates an MM1 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuMm1::ModCpuMm1( bool withSubDevs ):
    ModCpuEsmini( QString("MM1"), QString("PC")  )
{
    ModBbisSmbPciGen* smbBbis = 0;
	BusInterface* fpgaBusIf = 0;
	ModBbisCham *bbis=0;
	
    setDescription( "ESMini module with Intel Atom" );

    lstCpuCores << Atom;

    // add ESMini interface

	// empty path; pciDomain: 1; not supported for now
    //addChild( newEsminiBusIf( 0, true ) );
    
    // create fpga bus interface
    Q3MemArray<uchar> fpgaPciBusPath(2);
    fpgaPciBusPath[0] = 0x1c;
    fpgaPciBusPath[1] = 0x00;
    // create 1 slot to mark that fpga is not on bus 0 !
    fpgaBusIf = new PciBusInterface( StdPci, 0, 0, 0, &fpgaPciBusPath, false );
    fpgaBusIf->setInstName( QString("OnBoard PCI Bus")); //?
    addChild( fpgaBusIf );

    // Chameleon FPGA
    if( withSubDevs ){
        bbis = new ModBbisCham("PCITBL", 0x0);
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
		this->hasObSmb = true;
	}
}

// -----------------------------------------------------------------
// CPU MM50

//! Creates an MM50 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuMm50::ModCpuMm50( bool withSubDevs ):
    ModCpuEsmini( QString("MM50"), QString("PC")  )
{
	UTIL_UNREF_PARAM(withSubDevs);
    ModBbisSmbPciGen* smbBbis = 0;
	
    setDescription( "ESMini module with PowerPC MPC512x" );

    lstCpuCores << Ppc603e;

    // add ESMini interface

	// empty path; pciDomain: 1; not supported for now
    //addChild( newEsminiBusIf( 0, true ) );


 	// onchip smb controller
	this->hasObSmb = true;
    this->smbusIf = new PciBusInterface( LocalBus, -1, -1, 0, 0, false );
	this->smbusIf->setInstName( QString("Onboard SMB"));
	addChild( this->smbusIf );  
    smbBbis = new ModBbisSmbPciGen("NAT");
	this->smbBbisList.push_back(smbBbis);
}





