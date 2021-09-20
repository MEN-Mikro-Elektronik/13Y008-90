/***************************************************************************
 */
/*!       \file  modcpuf19p.cpp
 *       \brief  Classes for model MEN F19P
 *      \author	 dieter.pfeuffer@men.de
 *		  $Date: 2018/06/20 11:34:59 $
 *    $Revision: 2.6 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuf19p.cpp,v $
 * Revision 2.6  2018/06/20 11:34:59  dpfeuffer
 * R: F26L introduction
 * M: add F26L support (implementation merged from M.Moese Git commit)
 *
 * Revision 2.5  2015/05/26 17:19:59  ts
 * R: gituser autocheckin: updated source
 *
 * Revision 2.4  2014/06/12 15:27:31  ts
 * R: new CPU F22P could not be selected
 * M: added new model F22P to models
 *
 * Revision 2.3  2013/05/17 18:47:46  ts
 * R: scan_system script failed with new cpu names for conductive cooled boards
 * M: model names F19P/F19C changed to F19P_F19C ('/' breaks sed commands)
 *
 * Revision 2.2  2013/03/28 15:14:32  ts
 * R: Conductive cooled CPU model was not mentioned in MDIS wizard
 * M: added F19C as HwName String
 *
 * Revision 2.1  2011/09/26 09:17:43  dpfeuffer
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2006 - 2011 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>

#include "modcpuf19p.h"
#include "wizdebug.h"
#include "wizexcept.h"
#include "modbbisisa.h"
#include "modbbissmbpcigen.h"
#include "propertiesdlg.h"

#include "util.h"

#define MAX_PCISER_SLOTS	4	// peripheral slot 2..5

// -----------------------------------------------------------------
// CPU F19P

//! Creates an F19P CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuF19P::ModCpuF19P( bool withSubDevs ):
		ModCpuF14( withSubDevs )
{	
	BusInterface *busIf1=0;
	int i;

	UTIL_UNREF_PARAM(withSubDevs);

	// delete CPU cores from parent class and set it new
	lstCpuCores = Q3ValueList<CpuCore>(); 
	lstCpuCores << Pentium3 << Pentium4;

    setHwName( "F19P_F19C" );
    setDescription( "CompactPCI PlusIO Core 2 Duo 64bit CPU" );
    
	// Create the bus interfaces

	/////////////////////////////////////
    // Interface for CompactPci Serial
	Q3MemArray<uchar> cpciBusPath(1);

	for( i=2; i<MAX_PCISER_SLOTS+2; i++ ){
		switch( i ){
			// <pci-dev-nbr> | (<pci-func-nbr> << 5)
			case 2: cpciBusPath[0] = 0x1c | (0x00<<5); break;
			case 3: cpciBusPath[0] = 0x1c | (0x01<<5); break;
			case 4: cpciBusPath[0] = 0x1c | (0x02<<5); break;
			case 5: cpciBusPath[0] = 0x1c | (0x03<<5); break;
		}

		// _minSlot must be !=-1 so that hasPciBusPath() reports true
	    busIf1 = new PciBusInterface(
	        CpciSer, i, i, 0, &cpciBusPath);
		busIf1->setInstName( QString("CompactPCI Serial slot %1").arg(i) );
		addChild( busIf1 );
	}
}

// -----------------------------------------------------------------
// CPU F21P

//! Creates an F21P CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuF21P::ModCpuF21P( bool withSubDevs ):
		ModCpuF14( withSubDevs, false )
{	
	BusInterface *busIf1=0;
	int i;

	UTIL_UNREF_PARAM(withSubDevs);

	// delete CPU cores from parent class and set it new
	lstCpuCores = Q3ValueList<CpuCore>(); 
	lstCpuCores << Pentium3 << Pentium4;

    setHwName( "F21P_F21C" );
    setDescription( "CompactPCI PlusIO Core i7 64bit CPU" );

    
	// Create the bus interfaces

	/////////////////////////////////////
    // Interface for CompactPci
	{	// Create the CPCI bus interface
	    Q3MemArray<uchar> cpciBusPath(2);
		// <pci-dev-nbr> | (<pci-func-nbr> << 5)
	    cpciBusPath[0] = 0x01 | (0x02<<5);	// PCIe-to-PCIe bridge at bus0
	    cpciBusPath[1] = 0x00;				// PCIe-to-PCI bridge

	    busIf1 = new PciBusInterface(
	        Cpci, 2, 8, 0xf, &cpciBusPath);		// Interface for CompactPci
	    busIf1->setInstName( QString("CompactPci Bus"));
	    addChild( busIf1 );
	}

	/////////////////////////////////////
    // Interface for CompactPci Serial
	Q3MemArray<uchar> cpciBusPath(1);

	for( i=2; i<MAX_PCISER_SLOTS+2; i++ ){
		switch( i ){
			// <pci-dev-nbr> | (<pci-func-nbr> << 5)
			case 2: cpciBusPath[0] = 0x1c | (0x00<<5); break;
			case 3: cpciBusPath[0] = 0x1c | (0x01<<5); break;
			case 4: cpciBusPath[0] = 0x1c | (0x02<<5); break;
			case 5: cpciBusPath[0] = 0x1c | (0x03<<5); break;
		}

		// _minSlot must be !=-1 so that hasPciBusPath() reports true
	    busIf1 = new PciBusInterface(
	        CpciSer, i, i, 0, &cpciBusPath);
		busIf1->setInstName( QString("CompactPCI Serial slot %1").arg(i) );
		addChild( busIf1 );
	}
}

// -----------------------------------------------------------------
// CPU F22P

//! Creates an F22P CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuF22P::ModCpuF22P( bool withSubDevs ):
		ModCpuF14( withSubDevs, false )
{	
	BusInterface *busIf1=0;
	int i;

	UTIL_UNREF_PARAM(withSubDevs);

	// delete CPU cores from parent class and set it new
	lstCpuCores = Q3ValueList<CpuCore>(); 
	lstCpuCores << Pentium3 << Pentium4;

    setHwName( "F22P" );
    setDescription( "CompactPCI PlusIO Core i7 64bit CPU" );

    
	// Create the bus interfaces

	/////////////////////////////////////
    // Interface for CompactPci
	{	// Create the CPCI bus interface
	    Q3MemArray<uchar> cpciBusPath(2);
		// <pci-dev-nbr> | (<pci-func-nbr> << 5)
	    cpciBusPath[0] = 0x01 | (0x02<<5);	// PCIe-to-PCIe bridge at bus0
	    cpciBusPath[1] = 0x00;		// PCIe-to-PCI bridge

	    busIf1 = new PciBusInterface(
	       Cpci, 2, 8, 0xf, &cpciBusPath);		// Interface for CompactPci
	    busIf1->setInstName( QString("CompactPci Bus"));
	    addChild( busIf1 );
	}

	/////////////////////////////////////
    // Interface for CompactPci Serial
	Q3MemArray<uchar> cpciBusPath(1);

	for( i=2; i<MAX_PCISER_SLOTS+2; i++ ){
		switch( i ){
			// <pci-dev-nbr> | (<pci-func-nbr> << 5)
			case 2: cpciBusPath[0] = 0x1c | (0x00<<5); break;
			case 3: cpciBusPath[0] = 0x1c | (0x01<<5); break;
			case 4: cpciBusPath[0] = 0x1c | (0x02<<5); break;
			case 5: cpciBusPath[0] = 0x1c | (0x03<<5); break;
		}

		// _minSlot must be !=-1 so that hasPciBusPath() reports true
	    busIf1 = new PciBusInterface(
	        CpciSer, i, i, 0, &cpciBusPath);
		busIf1->setInstName( QString("CompactPCI Serial slot %1").arg(i) );
		addChild( busIf1 );
	}
}

// ts@men: F23P added 15.5.2015 
// -----------------------------------------------------------------
// CPU F23P

//! Creates an F23P CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuF23P::ModCpuF23P( bool withSubDevs ):
		ModCpuF14( withSubDevs, false )
{	
	BusInterface *busIf1=0;
	int i;

	UTIL_UNREF_PARAM(withSubDevs);

	// delete CPU cores from parent class and set it new
	lstCpuCores = Q3ValueList<CpuCore>(); 
	lstCpuCores << Pentium3 << Pentium4;

    setHwName( "F23P" );
    setDescription( "CompactPCI PlusIO Core i7 64bit CPU" );

    
	// Create the bus interfaces

	/////////////////////////////////////
    // Interface for CompactPci
	{	// Create the CPCI bus interface
	    Q3MemArray<uchar> cpciBusPath(2);
	    // <pci-dev-nbr> | (<pci-func-nbr> << 5)
	    cpciBusPath[0] = 0x01 | (0x02<<5);	// PCIe-to-PCIe bridge at bus0
	    cpciBusPath[1] = 0x00;		// PCIe-to-PCI bridge

	    busIf1 = new PciBusInterface(
	       Cpci, 2, 8, 0xf, &cpciBusPath);		// Interface for CompactPci
	    busIf1->setInstName( QString("CompactPci Bus"));
	    addChild( busIf1 );
	}

	/////////////////////////////////////
    // Interface for CompactPci Serial
	Q3MemArray<uchar> cpciBusPath(1);

	for( i=2; i<MAX_PCISER_SLOTS+2; i++ ){
		switch( i ){
			// <pci-dev-nbr> | (<pci-func-nbr> << 5)
			case 2: cpciBusPath[0] = 0x1c | (0x00<<5); break;
			case 3: cpciBusPath[0] = 0x1c | (0x01<<5); break;
			case 4: cpciBusPath[0] = 0x1c | (0x02<<5); break;
			case 5: cpciBusPath[0] = 0x1c | (0x03<<5); break;
		}

		// _minSlot must be !=-1 so that hasPciBusPath() reports true
	    busIf1 = new PciBusInterface(
	        CpciSer, i, i, 0, &cpciBusPath);
		busIf1->setInstName( QString("CompactPCI Serial slot %1").arg(i) );
		addChild( busIf1 );
	}
}


// mmo@men: F26L added 23.5.2017
// -----------------------------------------------------------------
// CPU F26L

//! Creates an F26L CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuF26L::ModCpuF26L( bool withSubDevs ):
		ModCpuF14( withSubDevs, false )
{	
	BusInterface *busIf1=0;
	int i;

	UTIL_UNREF_PARAM(withSubDevs);

	// delete CPU cores from parent class and set it new
	lstCpuCores = Q3ValueList<CpuCore>(); 
	lstCpuCores << Pentium3 << Pentium4;

    setHwName( "F26L" );
    setDescription( "CompactPCI PlusIO Apollo Lake-I CPU" );

    
	// Create the bus interfaces

	/////////////////////////////////////
    // Interface for CompactPci
	{	// Create the CPCI bus interface
	    Q3MemArray<uchar> cpciBusPath(2);
	    // <pci-dev-nbr> | (<pci-func-nbr> << 5)
	    cpciBusPath[0] = 0x01 | (0x02<<5);	// PCIe-to-PCIe bridge at bus0
	    cpciBusPath[1] = 0x00;		// PCIe-to-PCI bridge

	    busIf1 = new PciBusInterface(
	       Cpci, 2, 8, 0xf, &cpciBusPath);		// Interface for CompactPci
	    busIf1->setInstName( QString("CompactPci Bus"));
	    addChild( busIf1 );
	}

	/////////////////////////////////////
    // Interface for CompactPci Serial
	Q3MemArray<uchar> cpciBusPath(1);

	for( i=2; i<MAX_PCISER_SLOTS+2; i++ ){
		switch( i ){
			// <pci-dev-nbr> | (<pci-func-nbr> << 5)
			case 2: cpciBusPath[0] = 0x1c | (0x00<<5); break;
			case 3: cpciBusPath[0] = 0x1c | (0x01<<5); break;
			case 4: cpciBusPath[0] = 0x1c | (0x02<<5); break;
			case 5: cpciBusPath[0] = 0x1c | (0x03<<5); break;
		}

		// _minSlot must be !=-1 so that hasPciBusPath() reports true
	    busIf1 = new PciBusInterface(
	        CpciSer, i, i, 0, &cpciBusPath);
		busIf1->setInstName( QString("CompactPCI Serial slot %1").arg(i) );
		addChild( busIf1 );
	}
}

// mmo@men: F27P added 20.9.2021
// -----------------------------------------------------------------
// CPU F27P

//! Creates an F27P CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuF27P::ModCpuF27P( bool withSubDevs ):
		ModCpuF14( withSubDevs, false )
{	
	BusInterface *busIf1=0;
	int i;

	UTIL_UNREF_PARAM(withSubDevs);

	// delete CPU cores from parent class and set it new
	lstCpuCores = Q3ValueList<CpuCore>(); 
	lstCpuCores << Pentium4 << Athlon;

    setHwName( "F27P" );
    setDescription( "CompactPCI PlusIO AMD Ryzen Embedded V1000 APU CPU" );

    
	// Create the bus interfaces

	/////////////////////////////////////
    // Interface for CompactPci
	{	// Create the CPCI bus interface
	    Q3MemArray<uchar> cpciBusPath(2);
	    // <pci-dev-nbr> | (<pci-func-nbr> << 5)
	    cpciBusPath[0] = 0x01 | (0x02<<5);	// PCIe-to-PCIe bridge at bus0
	    cpciBusPath[1] = 0x00;		// PCIe-to-PCI bridge

	    busIf1 = new PciBusInterface(
	       Cpci, 2, 8, 0xf, &cpciBusPath);		// Interface for CompactPci
	    busIf1->setInstName( QString("CompactPci Bus"));
	    addChild( busIf1 );
	}

	/////////////////////////////////////
    // Interface for CompactPci Serial
	Q3MemArray<uchar> cpciBusPath(1);

	for( i=2; i<MAX_PCISER_SLOTS+2; i++ ){
		switch( i ){
			// <pci-dev-nbr> | (<pci-func-nbr> << 5)
			case 2: cpciBusPath[0] = 0x1c | (0x00<<5); break;
			case 3: cpciBusPath[0] = 0x1c | (0x01<<5); break;
			case 4: cpciBusPath[0] = 0x1c | (0x02<<5); break;
			case 5: cpciBusPath[0] = 0x1c | (0x03<<5); break;
		}

		// _minSlot must be !=-1 so that hasPciBusPath() reports true
	    busIf1 = new PciBusInterface(
	        CpciSer, i, i, 0, &cpciBusPath);
		busIf1->setInstName( QString("CompactPCI Serial slot %1").arg(i) );
		addChild( busIf1 );
	}
	
}