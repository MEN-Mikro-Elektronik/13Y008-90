/***************************************************************************
 */
/*!       \file  modextec01.cpp 
 *       \brief  Implementation of EC01
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:30:58 $
 *    $Revision: 2.0 $
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modextec01.cpp,v $
 * Revision 2.0  2010/01/22 11:30:58  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.2  2005/07/15 15:47:35  ub
 * added model for EC01N
 *
 * Revision 1.1  2004/06/08 17:26:58  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>

#include "modextec01.h"
#include "wizdebug.h"
#include "wizexcept.h"
#include "resourcemng.h"
#include "util.h"

// -----------------------------------------------------------------
//! Specialized PciBusInterface for EC01 PCI slots
/*!
  overrides pciBusPath to get the bus path from the ESM PCI bus
*/
class Ec01PciSlotIf : public PciBusInterface {
public:
	Ec01PciSlotIf() : PciBusInterface( StdPci, 0, 2 )
	{
		setInstName("PCI slots");
  	}

	//! determine PCI path from the ESM CPU board
	bool pciBusPath( int slotNo, Q3MemArray<uchar> *pciPath )
	{
		HwComponent *parent;
		PciBusInterface *cpuIf;
		bool rv = false;

		parent = getParent();			// goto ModExtEc01
		WIZ_ASSERT(parent);

		parent = parent->getParent();	// goto CPU busIf
		WIZ_ASSERT(parent);

		WIZ_DYNAMIC_CAST( parent, cpuIf, PciBusInterface * );

		if( slotNo >= minSlot && slotNo <= maxSlot ){
			// The PCI card slot n on EC01 corresponds to ESM IDSEL n+1
            rv = cpuIf->pciBusPath( slotNo+1, pciPath );
        }

		return rv;
	}
};

// -----------------------------------------------------------------
// EC01

//! Create an EC01  device
/*!
  
*/
ModExtEc01::ModExtEc01() :	EsmCarrier( "EC01")
{
	setDescription( "Embedded system module carrier with PCI slots" );

	addChild( new Ec01PciSlotIf() );
}


Device *ModExtEc01::create(bool withSubDevs)
{
	UTIL_UNREF_PARAM(withSubDevs);
	return new ModExtEc01();
}


// -----------------------------------------------------------------
//! Specialized PciBusInterface for EC01N PCI slots
/*!
  overrides pciBusPath to get the bus path from the ESM PCI bus
*/
class Ec01nPciSlotIf : public PciBusInterface {
public:
	Ec01nPciSlotIf() : PciBusInterface( StdPci, 0, 2 )
	{
		setInstName("PCI slots");
  	}

	//! determine PCI path from the ESM CPU board
	bool pciBusPath( int slotNo, Q3MemArray<uchar> *pciPath )
	{
		HwComponent *parent;
		PciBusInterface *cpuIf;
		bool rv = false;

		parent = getParent();			// goto ModExtEc01n
		WIZ_ASSERT(parent);

		parent = parent->getParent();	// goto CPU busIf
		WIZ_ASSERT(parent);

		WIZ_DYNAMIC_CAST( parent, cpuIf, PciBusInterface * );

		if( slotNo >= minSlot && slotNo <= maxSlot ){
			// The PCI card slot n on EC01 corresponds to ESM IDSEL n+1,
            // exception: slot 0
            if( slotNo == 0 )
                rv = cpuIf->pciBusPath( slotNo, pciPath );
            else
                rv = cpuIf->pciBusPath( slotNo+1, pciPath );
        }

		return rv;
	}
};

// -----------------------------------------------------------------
// EC01N

//! Create an EC01N  device
/*!
  
*/
ModExtEc01n::ModExtEc01n() :	EsmCarrier( "EC01N")
{
	setDescription( "Embedded system module carrier with PCI slots" );

	addChild( new Ec01nPciSlotIf() );
}


Device *ModExtEc01n::create(bool withSubDevs)
{
	UTIL_UNREF_PARAM(withSubDevs);
	return new ModExtEc01n();
}

