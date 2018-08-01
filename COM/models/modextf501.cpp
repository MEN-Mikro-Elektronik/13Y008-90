/***************************************************************************
 */
/*!       \file  modextf501.cpp
 *       \brief  Implementation of F501
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:31:02 $
 *    $Revision: 2.0 $
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modextf501.cpp,v $
 * Revision 2.0  2010/01/22 11:31:02  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.3  2006/11/30 11:28:39  cs
 * added:
 *   - support for combined CPUs:
 *     create all Interfaces/BBISes in dedicated classes
 *   - model F500
 *
 * Revision 1.2  2005/07/15 15:47:39  ub
 * added model for F502
 *
 * Revision 1.1  2004/06/08 17:27:00  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>

#include "wizdebug.h"
#include "modextf501.h"
#include "wizexcept.h"
#include "util.h"


// -----------------------------------------------------------------
// F500

//! Create an F500  device
/*!

*/
ModExtF500::ModExtF500() :	EsmCarrier( "F500" )
{
	setDescription( "CPCI 3U Embedded system module carrier" );

	addChild( new F500PciSlotIf() );
}


Device *ModExtF500::create(bool withSubDevs)
{
	UTIL_UNREF_PARAM(withSubDevs);
	return new ModExtF500();
}

// ----------------------------------------
// F500PciSlotIf
//! determine CPCI path from the ESM CPU board and add the CPCI slot
bool
F500PciSlotIf::	pciBusPath( int slotNo, Q3MemArray<uchar> *pciPath )
{
	// when not connected directly to ESM go to ModExtF500
	if( basePciPath.size() == 0 ){
		HwComponent *parent;
		PciBusInterface *cpuIf;

		parent = getParent();
		WIZ_ASSERT(parent);

		parent = parent->getParent();	// goto CPU busIf
		WIZ_ASSERT(parent);

		WIZ_DYNAMIC_CAST( parent, cpuIf, PciBusInterface * );

		// get buspath up to CPCI bridge (connected to ESM IDSEL0)
		if( cpuIf->pciBusPath( 0, &basePciPath ) == false )
			return false;
	}

	return PciBusInterface::pciBusPath( slotNo, pciPath );
}

// -----------------------------------------------------------------
// F501

//! Create an F501  device
/*!

*/
ModExtF501::ModExtF501() :	EsmCarrier( "F501" )
{
	setDescription( "CPCI 3U Embedded system module carrier" );

	addChild( new F501PciSlotIf() );
}


Device *ModExtF501::create(bool withSubDevs)
{
	UTIL_UNREF_PARAM(withSubDevs);
	return new ModExtF501();
}

// ----------------------------------------
// F501PciSlotIf
//! determine CPCI path from the ESM CPU board and add the CPCI slot
bool
F501PciSlotIf::pciBusPath( int slotNo, Q3MemArray<uchar> *pciPath )
{
	// when not connected directly to ESM go to ModExtF500
	if( basePciPath.size() == 0 ){
		HwComponent *parent;
		PciBusInterface *cpuIf;

		parent = getParent();			// goto ModExtF501
		WIZ_ASSERT(parent);

		parent = parent->getParent();	// goto CPU busIf
		WIZ_ASSERT(parent);

		WIZ_DYNAMIC_CAST( parent, cpuIf, PciBusInterface * );

		// get buspath up to CPCI bridge (connected to ESM IDSEL3)
		if( cpuIf->pciBusPath( 3, &basePciPath ) == false )
			return false;
	}

	return PciBusInterface::pciBusPath( slotNo, pciPath );
}

// -----------------------------------------------------------------
// F502

//! Create an F502  device
/*!

*/
ModExtF502::ModExtF502() :	EsmCarrier( "F502" )
{
	setDescription( "CPCI 3U Embedded system module carrier with SRAM" );

	addChild( new F501PciSlotIf() );
}


Device *ModExtF502::create(bool withSubDevs)
{
	UTIL_UNREF_PARAM(withSubDevs);
	return new ModExtF502();
}


