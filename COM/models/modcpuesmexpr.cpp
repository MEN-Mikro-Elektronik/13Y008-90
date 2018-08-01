/***************************************************************************
 */
/*!       \file  modcpuesmexpr.cpp
 *       \brief  MEN ESMexpress classes
 *      \author  christine.ruff@men.de
 *        $Date: 2016/02/20 16:27:00 $
 *    $Revision: 2.9 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuesmexpr.cpp,v $
 * Revision 2.9  2016/02/20 16:27:00  ts
 * R: BoxPCs based on SC24 need 2 SMBusses in MDIS wizard
 * M: 1. added member nrSmbBusses to class CpuDeviceSmb
 *    2. added parameter int nrSmbBusses to every constructor of CPUs with SMB
 *       controllers: 2 for SC24 (AMD based) Box PCs, 1 for every other CPU
 *
 * Revision 2.8  2011/09/28 09:07:21  dpfeuffer
 * R: wrong CPU board name F50
 * M: F50 replaced by F50P, description changed
 *
 * Revision 2.7  2011/09/26 08:43:13  dpfeuffer
 * R: cosmetics
 * M: cosmetics
 *
 * Revision 2.6  2011/05/20 13:13:45  CRuff
 * R: 1. support pci domains
 *    2. support cPCI interface and FPGA of F50 / XM50
 * M: 1. extended method newEsmExprBusIf() by pci domain parameter
 *    2. in F50 and XM50, set flag _hasPciDomains and create corresponding
 *       bus interfaces
 *
 * Revision 2.5  2010/10/22 15:24:05  CRuff
 * R: cpu architecture for XM50 was wrong
 * M: changed cpu architecture for XM50 to PpcE500 (Ppc85xx)
 *
 * Revision 2.4  2010/10/20 17:38:22  CRuff
 * R: Processor architecture of F50 was wrong
 * M: changed architecture to PpcE500
 *
 * Revision 2.3  2010/07/12 10:08:11  dpfeuffer
 * R: compiler warning under Windows for mdiswiz_lib.lib
 * M: UTIL_UNREF_PARAM added to unreferenced parameters
 *
 * Revision 2.2  2010/06/21 10:57:52  CRuff
 * R: unexpected behaviour for smb controller handling
 * M: inherit from ModCpuEsmExpr directly instead of inheriting XM50 class
 *
 * Revision 2.1  2010/06/08 11:19:24  CRuff
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>
#include <Q3PtrList>

#include "modcpuesmexpr.h"
#include "modbbischam.h"
#include "resourcemng.h"
#include "wizdebug.h"
#include "modbbissmbpcigen.h"
#include "modextf503.h"

// max. number of chameleon units inside FPGA
static const uint numFpgaSlots = 32;

ModCpuEsmExpr::ModCpuEsmExpr( QString _hwName, QString _driverName  ):
    CpuDeviceSmb( _hwName, _driverName, false, false, 1 )
{
}

BusInterface*
ModCpuEsmExpr::newEsmExprBusIf( const Q3MemArray<uchar>* pciBusPath, bool endianSwapping, int pciDomain )
{
    // defineESM PCI bus interface
    BusInterface *esmExprBusIf;
    wDebug(("create EsmExprBusIf"));

   	esmExprBusIf = new PciBusInterface( ESMexpress, 0, 0, pciBusPath, 0, true, pciDomain );
    	
    esmExprBusIf->setInstName( QString("ESMexpress PCI Connector"));
    esmExprBusIf->setEndianSwapping( endianSwapping );

    return( esmExprBusIf );
}


// -----------------------------------------------------------------
// CPU XM50

//! Creates an XM50 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuXm50::ModCpuXm50( bool withSubDevs ):
    ModCpuEsmExpr( QString("XM50"), QString("PC")  )
{
	UTIL_UNREF_PARAM(withSubDevs);
    ModBbisSmbPciGen* smbBbis = 0;

    setDescription( "ESMexpress module with MPC8548" );

    lstCpuCores << PpcE500;
    this->_hasPciDomains = true;

    // add ESMexpress interface
   
	// pciDomain: 2; not supported for now
    addChild( newEsmExprBusIf( 0, true, 2 ) );

	// SMB controller
    this->hasObSmb = true;
    this->smbusIf = new PciBusInterface( LocalBus, -1, -1, 0, 0, false );
	this->smbusIf->setInstName( QString("Onboard SMB"));
	addChild( this->smbusIf );  
    smbBbis = new ModBbisSmbPciGen("NAT");
	this->smbBbisList.push_back(smbBbis);
}

// -----------------------------------------------------------------
// CPU XM1

//! Creates an XM1 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuXm1::ModCpuXm1( bool withSubDevs ):
    ModCpuEsmExpr( QString("XM1"), QString("PC")  )
{
	UTIL_UNREF_PARAM(withSubDevs);
    ModBbisSmbPciGen* smbBbis = 0;

    setDescription( "ESMexpress module with Intel Atom" );

    lstCpuCores << Atom;

    // add ESMini interface

	// empty path; pciDomain: 1 
    //addChild( newEsmExprBusIf( 0, true, 1 ) );


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
// CPU XM2

//! Creates an XM2 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuXm2::ModCpuXm2( bool withSubDevs ):
    ModCpuEsmExpr( QString("XM2"), QString("PC")  )
{
	UTIL_UNREF_PARAM(withSubDevs);
    ModBbisSmbPciGen* smbBbis = 0;
	
    setDescription( "ESMexpress module with Intel Core 2 Duo CPU" );

    lstCpuCores << Pentium << Pentium2 << Pentium3 << Pentium4;

    // add ESMini interface

	// empty path; pciDomains: 0-3; not supported for now
    //addChild( newEsmExprBusIf( 0, true ) );


	// SMBus interface
	{
		Q3MemArray<uchar> smbBusPath(1);
	    smbBusPath[0] = 0x1f;
	    
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

//! Creates an F50P CPU device (XM50 + F503)
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuF50P::ModCpuF50P( bool withSubDevs ):
	ModCpuEsmExpr( QString("F50P"), QString("PC")  )
    //ModCpuXm50( withSubDevs )
{
	BusInterface* fpgaBusIf = 0;
	BusInterface* f503BusIf = 0;
	ModBbisSmbPciGen* smbBbis = 0;
	ModBbisCham *bbis=0;

    lstCpuCores << PpcE500;
    setDescription( "CompactPCI PlusIO MPC8548 CPU" );
    this->_hasPciDomains = true;

    // replace ESM interface with CPCI interface on first slot! 
    // CPCI interface cannot be supported for now (pci domains)
    /*f503BusIf = new F503PciSlotIf( );
	f503BusIf->setEndianSwapping( true );
	replChild( 0, f503BusIf );*/

    Q3MemArray<uchar> cPciBusPath(4);
    cPciBusPath[0] = 0x0;
    cPciBusPath[1] = 0x0;
    cPciBusPath[2] = 0x2;
    cPciBusPath[3] = 0x0;
    f503BusIf = new PciBusInterface( Cpci, 2, 8, 0xf, &cPciBusPath, true, 2 );
    f503BusIf->setEndianSwapping( true );
    f503BusIf->setInstName( QString("CompactPci Bus"));
    //replChild( 0, new F503PciSlotIf(&cPciBusPath, 2) );
    addChild(f503BusIf);
	
	// add interface for fpga
    Q3MemArray<uchar> fpgaPciBusPath(4);
	fpgaPciBusPath[0] = 0x0;
	fpgaPciBusPath[1] = 0x0;
	fpgaPciBusPath[2] = 0x1;
	fpgaPciBusPath[3] = 0x0;
	// create 1 slot to mark that fpga is not on bus 0 !
	fpgaBusIf = new PciBusInterface( StdPci, 0, 0, 0, &fpgaPciBusPath, false, 2 );

	//fpgaBusIf = new PciBusInterface( StdPci, -1, -1, 0, 0, false, 1 );
	fpgaBusIf->setEndianSwapping( true );
    fpgaBusIf->setInstName( QString("Onboard PCI Bus"));
    addChild(fpgaBusIf );
    
    if( withSubDevs ) {
        // Chameleon FPGA
        bbis = new ModBbisCham(0, 0x0);
    	bbis->setInstName("FPGA");
        
        // create BBIS slots
	    for( uint i=0; i<numFpgaSlots; i++ ){
	        BbisSlot *bbSlot = new BbisSlot( ChameleonBus, i );
	        bbSlot->setSlotName( QString("FPGA virtual slot %1").arg(i) );
	        bbis->addChild( bbSlot );
	    }

        fpgaBusIf->addChild( bbis );
    }
    
    // SMB controller
    this->hasObSmb = true;
    this->smbusIf = new PciBusInterface( LocalBus, -1, -1, 0, 0, false );
	this->smbusIf->setInstName( QString("Onboard SMB"));
	addChild( this->smbusIf );  
    smbBbis = new ModBbisSmbPciGen("NAT");
	this->smbBbisList.push_back(smbBbis);
    
}


