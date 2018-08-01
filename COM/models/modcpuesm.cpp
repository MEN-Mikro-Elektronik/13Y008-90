/***************************************************************************
 */
/*!       \file  modcpuesm.cpp
 *       \brief  MEN embedded system module classes
 *      \author  Ulrich.Bogensperger@men.de
 *        $Date: 2016/02/20 16:26:58 $
 *    $Revision: 2.4 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuesm.cpp,v $
 * Revision 2.4  2016/02/20 16:26:58  ts
 * R: BoxPCs based on SC24 need 2 SMBusses in MDIS wizard
 * M: 1. added member nrSmbBusses to class CpuDeviceSmb
 *    2. added parameter int nrSmbBusses to every constructor of CPUs with SMB
 *       controllers: 2 for SC24 (AMD based) Box PCs, 1 for every other CPU
 *
 * Revision 2.3  2011/09/26 08:43:11  dpfeuffer
 * R: no MEN HW with PC104+ PCI (it is PCI-104)
 * M: Pc104PlusPci replaced with Pci104
 *
 * Revision 2.2  2010/10/20 16:30:34  CRuff
 * R: Swapping was not correct for F12, F12N and F13
 * M: set swapping flag for the F500 Bus Interface in the Cpu constructors
 *
 * Revision 2.1  2010/05/19 17:47:21  CRuff
 * R: 1. add SMB controller support
 * M: 1a) inherit from CpuDeviceSmb instead of CpuDevice
 *    1b) add BusInterface for SMB BBISes
 *
 * Revision 2.0  2010/01/22 11:30:12  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.8  2009/04/27 17:18:35  CRuff
 * R: Provide CPU selection possibility for Intel CPUs
 * M: Added CPU cores Pentium/Pentium2/Pentium3 to A13
 *    and Pentium/Pentium2 to EM2, EM7
 *
 * Revision 1.7  2007/10/05 16:22:53  cs
 * added:
 *   + EM9
 *
 * Revision 1.6  2007/05/07 13:24:37  cs
 * fixed:
 *   EM1 MPC5200 on board MSCAN devices: IRQ number changed to 0x41/0x42
 *
 * Revision 1.5  2006/11/30 11:28:27  cs
 * fixed:
 *   - remove device 0x00 from empty busPaths: now length 0!
 *   - EM2: generated busPaths for CPCI interface were wrong
 *   - EM7: - use CPU Pentium3
 *   - wrong paths / IDs for CPCI and FPGA were used
 * added:
 *   - EM7N, EM8, A13, A14, F11, F11N, F12, F13,
 *
 * Revision 1.4  2006/08/04 11:50:13  ub
 * added irqNumber property in EM1
 *
 * Revision 1.3  2006/07/27 13:43:29  ub
 * Added: interfaces for MSCAN on MGT5200
 *
 * Revision 1.2  2005/12/07 16:55:19  ub
 * Chameleon model for EM1 uses irq from PCI config space now
 *
 * Revision 1.1  2005/07/15 15:47:28  ub
 * Initial Revision
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>
#include <Q3PtrList>

#include "modcpuesm.h"
#include "modbbischam.h"
#include "resourcemng.h"
#include "wizdebug.h"
#include "modbbissmbpcigen.h"

ModCpuEsm::ModCpuEsm( QString _hwName, QString _driverName  ):
    CpuDeviceSmb( _hwName, _driverName, false, false, 1 )
{
}

BusInterface*
ModCpuEsm::newEsmBusIf( const Q3MemArray<uchar>* pciBusPath, bool endianSwapping )
{
    // defineESM PCI bus interface
    BusInterface *esmBusIf;

    Q3MemArray<uchar> esmPciBusTbl(4);
    esmPciBusTbl[0] = 20;
    esmPciBusTbl[1] = 21;
    esmPciBusTbl[2] = 22;
    esmPciBusTbl[3] = 23;

    esmBusIf = new PciBusInterface( Pci104, 0, 3,
                                    pciBusPath, &esmPciBusTbl, true );
    esmBusIf->setInstName( QString("ESM PCI Connector"));
    esmBusIf->setEndianSwapping( endianSwapping );

    return( esmBusIf );
}


// -----------------------------------------------------------------
// CPU Em1

//! Creates an Em1 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuEm1::ModCpuEm1( bool withSubDevs ):
    ModCpuEsm( QString("EM1"), QString("PC")  )
{
    BusInterface *fpgaBusIf, *canBusIf1, *canBusIf2;
    BbisDevice *bbis;
    ModBbisSmbPciGen* smbBbis = 0;

    setDescription( "Embedded System Module with MPC5200" );

    lstCpuCores << Ppc603e;

    // add Pc104Plus interface
    Q3MemArray<uchar> esmPciBusPath(0);

    addChild( newEsmBusIf( &esmPciBusPath, true ) );

    // add PCI interface for FPGA
    // Interfaces for EM01 BBISes
    Q3MemArray<uchar> fpgaPciBusPath(1);
    fpgaPciBusPath[0] = 0x1d;

    fpgaBusIf = new PciBusInterface( StdPci, -1, -1, 0, &fpgaPciBusPath,
                                     false );
    fpgaBusIf->setEndianSwapping( true );
    fpgaBusIf->setInstName( QString("OnBoard PCI Bus")); //?
    addChild( fpgaBusIf );


    // Interfaces for built-in MSCAN
    canBusIf1 = new BusInterface( LocalBus, false );
    canBusIf1->setInstName( QString("MSCAN1 on MPC5200"));
    addChild( canBusIf1 );

    canBusIf2 = new BusInterface( LocalBus, false );
    canBusIf2->setInstName( QString("MSCAN2 on MPC5200"));
    addChild( canBusIf2 );

    if( withSubDevs ) {
        // Chameleon FPGA
        bbis = new ModBbisCham("PCITBL", 0x1d);
        bbis->setInstName( "FPGA" );
        fpgaBusIf->addChild( bbis );

        // the two MSCAN controllers
        ModBbisMscan5200 *canBbis1 = new ModBbisMscan5200( "BBMSCAN_1",
                                                          0xf0000900, 0x41 );
        canBusIf1->addChild( canBbis1 );
        ModBbisMscan5200 *canBbis2 = new ModBbisMscan5200( "BBMSCAN_2",
                                                          0xf0000980, 0x42 );
        canBusIf2->addChild( canBbis2 );
    }
    
    this->hasObSmb = true;
    
    this->smbusIf = new PciBusInterface( LocalBus, -1, -1, 0, 0, false );
	this->smbusIf->setInstName( QString("Onboard SMB"));
	addChild( this->smbusIf );  
    smbBbis = new ModBbisSmbPciGen("NAT");
	this->smbBbisList.push_back(smbBbis);
}

// -----------------------------------------------------------------
// BBIS for MSCAN on MPC5200

ModBbisMscan5200::ModBbisMscan5200( QString instName, long address,
                                    long irqNumber ) :
    ModBbisIsa( false )
{
    setHwName("MSCAN5200_BBIS");        // Model name in Mdiswiz main view
	setIsaAddress( address, 0x80 );
    setIrqNumber( irqNumber );
	setInstName( instName );      // must match <bbisinstance> in .xml file
	setInstNameChangeAllowed( false );

	// Modify parameters of BBIS slot
	BbisSlot *bbSlot;
	WIZ_DYNAMIC_CAST( firstChild(), bbSlot, BbisSlot * );

	bbSlot->setSlotName( "pseudo slot for MSCAN on MPC5200" );
	bbSlot->setBbSlotSpecRequired( true );
}


// -----------------------------------------------------------------
// CPU Em02

//! Creates an Em02 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuEm2::ModCpuEm2( bool withSubDevs ):
	ModCpuEsm( QString("EM2"), QString("PC") )
{
	UTIL_UNREF_PARAM(withSubDevs);

	setDescription( "Embedded System Module with Pentium III" );

	lstCpuCores << Pentium << Pentium2 << Pentium3;

	// add ESM PciBusInterface
	Q3MemArray<uchar> esmPciBusPath(1);
	esmPciBusPath[0] = 0x1e;

    addChild( newEsmBusIf( &esmPciBusPath, false ) );  // no endian swapping
}

BusInterface*
ModCpuEm2::newEsmBusIf( const Q3MemArray<uchar>* pciBusPath, bool endianSwapping )
{
    // defineESM PCI bus interface
    BusInterface *esmBusIf;

    Q3MemArray<uchar> esmPciBusTbl(4);
    esmPciBusTbl[0] = 4;
    esmPciBusTbl[1] = 5;
    esmPciBusTbl[2] = 6;
    esmPciBusTbl[3] = 7;

    esmBusIf = new PciBusInterface( Pci104, 0, 3,
                                    pciBusPath, &esmPciBusTbl, true );
    esmBusIf->setInstName( QString("ESM PCI Connector"));
    esmBusIf->setEndianSwapping( endianSwapping );

    return( esmBusIf );
}


// -----------------------------------------------------------------
// CPU Em3

//! Creates an Em3 CPU device
ModCpuEm3::ModCpuEm3( bool withSubDevs ):
    ModCpuEsm( QString("EM3"), QString("PC") )
{
    BusInterface *fpgaBusIf;
    BbisDevice *bbis;

    setDescription( "Embedded System Module with PowerQuicc III" );

    lstCpuCores << PpcE500;

    // add Pc104Plus interface
    Q3MemArray<uchar> esmPciBusPath(0);

    addChild( newEsmBusIf( &esmPciBusPath, true ) );

    // add PCI interface for FPGA
    // Interfaces for EM04 BBISes
    Q3MemArray<uchar> fpgaPciBusPath(1);
    fpgaPciBusPath[0] = 0x1d;

    fpgaBusIf = new PciBusInterface( StdPci, -1, -1, 0, &fpgaPciBusPath,
                                     false );
    fpgaBusIf->setEndianSwapping( true );
    fpgaBusIf->setInstName( QString("OnBoard PCI Bus")); //?
    addChild( fpgaBusIf );

    // Chameleon FPGA
    if( withSubDevs ){
        bbis = new ModBbisCham(0, 0x1d);
        bbis->setInstName( "FPGA" );
        fpgaBusIf->addChild( bbis );
    }
}

// -----------------------------------------------------------------
// CPU Em04

//! Creates an Em04 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuEm04::ModCpuEm04( bool withSubDevs ):
    ModCpuEsm( QString("EM04"), QString("PC") )
{
    BusInterface *fpgaBusIf;
    BbisDevice *bbis;

    setDescription( "Embedded System Module with PowerPC 8245" );

    lstCpuCores << Ppc603e;

    // add Pc104Plus interface
    Q3MemArray<uchar> esmPciBusPath(0);

    addChild( newEsmBusIf( &esmPciBusPath, true ) );

    // Interfaces for EM04 BBISes
    Q3MemArray<uchar> fpgaPciBusPath(1);
    fpgaPciBusPath[0] = 0x1d;
    fpgaBusIf = new PciBusInterface( StdPci, -1, -1, 0, &fpgaPciBusPath,
                                     false );
    fpgaBusIf->setEndianSwapping( true );
    fpgaBusIf->setInstName( QString("OnBoard PCI Bus")); //?
    addChild( fpgaBusIf );

    // Chameleon FPGA
    if( withSubDevs ){
        bbis = new ModBbisCham(0, 0x1d);
        bbis->setInstName( "FPGA" );
        fpgaBusIf->addChild( bbis );
    }
}

// -----------------------------------------------------------------
// CPU Em05

//! Creates an Em05 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuEm5::ModCpuEm5( bool withSubDevs ):
	ModCpuEsm( QString("EM5"), QString("PC") )
{
	BusInterface *fpgaBusIf;
	BbisDevice *bbis;

	setDescription( "Embedded System Module with Transmeta TM5800/5900" );

	lstCpuCores << Crusoe;

    // add Pc104Plus interface
    Q3MemArray<uchar> esmPciBusPath(0);

    addChild( newEsmBusIf( &esmPciBusPath, false ) );  // no endian swapping


	// Interfaces for EM05 BBISes
	Q3MemArray<uchar> fpgaPciBusPath(1);
    fpgaPciBusPath[0] = 13;
	fpgaBusIf = new PciBusInterface( StdPci, -1, -1, 0, &fpgaPciBusPath,
                                     false );
    fpgaBusIf->setInstName( QString("OnBoard PCI Bus")); //?
	addChild( fpgaBusIf );

	// Chameleon FPGA
	if( withSubDevs ){
		bbis = new ModBbisCham("PCITBL", 0x0d);
		bbis->setInstName( "FPGA" );
		fpgaBusIf->addChild( bbis );
	}
}

// -----------------------------------------------------------------
// CPU Em7

//! Creates an Em7 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuEm7::ModCpuEm7( bool withSubDevs ):
	ModCpuEsm( QString("EM7"), QString("PC") )
{
	BusInterface *fpgaBusIf;
	BbisDevice *bbis;

	setDescription( "Embedded System Module with PIII / Celeron" );

	lstCpuCores << Pentium << Pentium2 << Pentium3;

    // add Pc104Plus interface
	Q3MemArray<uchar> esmPciBusPath(1);
    esmPciBusPath[0] = 0x1e;

    addChild( newEsmBusIf( &esmPciBusPath, false ) );  // no endian swapping


	// Interfaces for EM7 BBISes
	Q3MemArray<uchar> fpgaPciBusPath(2);
    fpgaPciBusPath[0] = 0x1e;
    fpgaPciBusPath[1] = 0x08;

    // minSlot != -1 reports hasPciBusPath!
	fpgaBusIf = new PciBusInterface( StdPci, 0, 0, 0, &fpgaPciBusPath,
                                     false );
    fpgaBusIf->setInstName( QString("OnBoard PCI Bus")); //?
	addChild( fpgaBusIf );

	// Chameleon FPGA
	if( withSubDevs ){
		bbis = new ModBbisCham("PCITBL", 0x08);
		bbis->setInstName( "FPGA" );
		fpgaBusIf->addChild( bbis );
	}
}

BusInterface*
ModCpuEm7::newEsmBusIf( const Q3MemArray<uchar>* pciBusPath, bool endianSwapping )
{
    // defineESM PCI bus interface
    BusInterface *esmBusIf;

    Q3MemArray<uchar> esmPciBusTbl(4);
    esmPciBusTbl[0] = 4;
    esmPciBusTbl[1] = 5;
    esmPciBusTbl[2] = 6;
    esmPciBusTbl[3] = 7;

    esmBusIf = new PciBusInterface( Pci104, 0, 3,
                                    pciBusPath, &esmPciBusTbl, true );
    esmBusIf->setInstName( QString("ESM PCI Connector"));
    esmBusIf->setEndianSwapping( endianSwapping );

    return( esmBusIf );
}


// -----------------------------------------------------------------
// CPU Em7N

//! Creates an Em7N CPU device
/*!
  just like Em7 but path to FPGA is different
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuEm7N::ModCpuEm7N( bool withSubDevs ):
	ModCpuEm7( withSubDevs )
{
	BusInterface *fpgaBusIf;
	BbisDevice *bbis;

   	setHwName( "EM7N" );

	// remove old FPGA (Iface 1)
	uint s = 1;
	Q3PtrList<HwComponent> &busIfLst = getChildren();
	fpgaBusIf = (BusInterface *)busIfLst.at( s );
	removeChild( fpgaBusIf );

	// Interfaces for EM7N BBISes
	Q3MemArray<uchar> fpgaPciBusPath(2);
    fpgaPciBusPath[0] = 0x1e;
    fpgaPciBusPath[1] = 0x09;
    // minSlot != -1 reports hasPciBusPath!
	fpgaBusIf = new PciBusInterface( StdPci, 0, 0, 0, &fpgaPciBusPath,
                                     false );
    fpgaBusIf->setInstName( QString("OnBoard PCI Bus")); //?
	addChild( fpgaBusIf );

	// Chameleon FPGA
	if( withSubDevs ){
		bbis = new ModBbisCham("PCITBL", 0x09);
		bbis->setInstName( "FPGA" );
		fpgaBusIf->addChild( bbis );
	}
}
// -----------------------------------------------------------------
// CPU Em8

//! Creates an Em8 CPU device
ModCpuEm8::ModCpuEm8( bool withSubDevs ):
	ModCpuEm3( withSubDevs )
{
    setHwName( "EM8" );
}

// -----------------------------------------------------------------
// CPU EM9

//! Creates an EM9 CPU device
ModCpuEm9::ModCpuEm9( bool withSubDevs ):
    ModCpuEsm( QString("EM9"), QString("PC") )
{
    BusInterface *fpgaBusIf;
    BbisDevice *bbis;

    setDescription( "Embedded System Module with PowerQuicc III" );

    lstCpuCores << PpcE500;

    // add Pc104Plus interface
    Q3MemArray<uchar> esmPciBusPath(0);

    addChild( newEsmBusIf( &esmPciBusPath, true ) );

    // add PCI interface for FPGA
    // Interfaces for EM09 BBISes
    Q3MemArray<uchar> fpgaPciBusPath(1);
    fpgaPciBusPath[0] = 0x13;

    fpgaBusIf = new PciBusInterface( StdPci, -1, -1, 0, &fpgaPciBusPath,
                                     false );
    fpgaBusIf->setEndianSwapping( true );
    fpgaBusIf->setInstName( QString("OnBoard PCI Bus")); //?
    addChild( fpgaBusIf );

    // Chameleon FPGA
    if( withSubDevs ){
        bbis = new ModBbisCham(0, 0x13);
        bbis->setInstName( "FPGA" );
        fpgaBusIf->addChild( bbis );
    }
}

//--------------------------------------------------------------------
// combined CPUs (ESM + Carrier)

//! Creates an A14 CPU device: EM8 + A500
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuA13::ModCpuA13( ModExtA500::Mezzanines mezzanines, bool withSubDevs ):
    ModCpuEm2( withSubDevs )
{
	QString hwName = QString( "A13" );
	QString descr  = QString( "VME Pentium III CPU " );
	
	lstCpuCores << Pentium << Pentium2 << Pentium3;

	_mezzanines = mezzanines;

    // Add VME Interface and BBISes!
    Q3MemArray<uchar> cpciBusPath(2);
    cpciBusPath[0] = 0x1e;
    cpciBusPath[1] = 0x04;      // A500 PCI2PCI bridge on IDSEL0

	ModExtA500IfsCreate( this, hwName, descr, mezzanines, &cpciBusPath, withSubDevs);


    setHwName( hwName );
    setDescription( descr );

	// remove all ESM PCI slots
	uint s = 0;
	Q3PtrList<HwComponent> &busIfLst = getChildren();
	PciBusInterface *esmBif = (PciBusInterface *)busIfLst.at( s );
	removeChild( esmBif );
}

SwModuleList *
ModCpuA13::enumSwModules(){
	SwModuleList *addLst = enumSwModulesForExternalPackage( "PLDZ002" );

	if( lstSwModules == 0 && addLst )
		lstSwModules = new SwModuleList;

	if( lstSwModules && addLst )
		lstSwModules->add( addLst );

	return lstSwModules;
}

//! Creates an A14 CPU device: EM8 + A500
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuA14::ModCpuA14( ModExtA500::Mezzanines mezzanines, bool withSubDevs ):
    ModCpuEm8( withSubDevs )
{
	QString hwName = QString( "A14" );
	QString descr  = QString( "VME PowerQuicc III CPU " );

	_mezzanines = mezzanines;

    // Add VME Interface and BBISes!
    Q3MemArray<uchar> cpciBusPath(1);
    cpciBusPath[0] = 0x14;      // A500 PCI2PCI bridge on IDSEL0

	ModExtA500IfsCreate( this, hwName, descr, mezzanines, &cpciBusPath, withSubDevs);


    setHwName( hwName );
    setDescription( descr );

	// remove all ESM PCI slots
	uint s = 0;
	Q3PtrList<HwComponent> &busIfLst = getChildren();
	PciBusInterface *esmBif = (PciBusInterface *)busIfLst.at( s );
	removeChild( esmBif );
}

SwModuleList *
ModCpuA14::enumSwModules(){
	SwModuleList *addLst = enumSwModulesForExternalPackage( "PLDZ002" );

	if( lstSwModules == 0 && addLst )
		lstSwModules = new SwModuleList;

	if( lstSwModules && addLst )
		lstSwModules->add( addLst );

	return lstSwModules;
}

//! Creates an F11 CPU device (EM7 + F502)
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuF11::ModCpuF11( bool withSubDevs ):
    ModCpuEm7( withSubDevs )
{
    setHwName( "F11" );
    setDescription( "CompactPCI PIII / Celeron CPU" );

    // replace ESM interface with CPCI interface on first slot!
    Q3MemArray<uchar> cpciBusPath(2);
    cpciBusPath[0] = 0x1e;
    cpciBusPath[1] = 0x17;      // F502 PCI2PCI bridge on IDSEL3

	replChild( 0, new F501PciSlotIf( &cpciBusPath ) );
}

//! Creates an F11N CPU device (EM7N + F502)
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuF11N::ModCpuF11N( bool withSubDevs ):
    ModCpuEm7N( withSubDevs )
{
    setHwName( "F11N" );
    setDescription( "CompactPCI PIII / Celeron CPU" );

    // replace ESM interface with CPCI interface on first slot!
    Q3MemArray<uchar> cpciBusPath(2);
    cpciBusPath[0] = 0x1e;
    cpciBusPath[1] = 0x17;      // F502 PCI2PCI bridge on IDSEL3

	replChild( 0, new F501PciSlotIf( &cpciBusPath ) );
}

//! Creates an F12 CPU device (EM1 + F500)
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuF12::ModCpuF12( bool withSubDevs ):
    ModCpuEm1( withSubDevs )
{
	PciBusInterface* f500BusIf =0;
	
    setHwName( "F12" );
    setDescription( "CompactPCI PowerPC MPC5200 CPU" );

    // replace ESM interface with CPCI interface on first slot!
    Q3MemArray<uchar> cpciBusPath(1);
    cpciBusPath[0] = 0x14;      // F500 PCI2PCI bridge on IDSEL0

	f500BusIf = new F500PciSlotIf( &cpciBusPath );
	replChild( 0, f500BusIf );
	f500BusIf->setEndianSwapping(true);
}


//! Creates an F13 CPU device: EM8 + F500
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuF13::ModCpuF13( bool withSubDevs ):
    ModCpuEm8( withSubDevs )
{
	PciBusInterface* f500BusIf =0;
    setHwName( "F13" );
    setDescription( "CompactPCI PowerQuicc III CPU" );

    // replace ESM interface with CPCI interface on first slot!
    Q3MemArray<uchar> cpciBusPath(1);
    cpciBusPath[0] = 0x14;      // F500 PCI2PCI bridge on IDSEL0

	f500BusIf = new F500PciSlotIf( &cpciBusPath );
	replChild( 0, f500BusIf );
	f500BusIf->setEndianSwapping(true);
}




