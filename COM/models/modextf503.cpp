/***************************************************************************
 */
/*!       \file  modextf503.cpp
 *       \brief  Implementation of F503
 *      \author	 christine.ruff@men.de
 *		  $Date: 2014/07/18 15:12:40 $
 *    $Revision: 2.5 $
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modextf503.cpp,v $
 * Revision 2.5  2014/07/18 15:12:40  dpfeuffer
 * R: unused cpciBusPath, fpgaBusPath variable
 * M: cpciBusPath, fpgaBusPath removed
 *
 * Revision 2.4  2011/05/20 13:18:28  CRuff
 * R: 1. support of pci domains
 * M: 1a) added pciDomain to F503 slot interfaces
 *    1b) corrected pci bus path in F503 slot interfaces
 *
 * Revision 2.3  2010/10/22 15:06:43  CRuff
 * R: Swapping was switched on for F503 fpga (wrong for x86 cpus)
 * M: removed swapping for fpga bus interface in ModExtF503
 *
 * Revision 2.2  2010/07/12 10:08:22  dpfeuffer
 * R: compiler warning under Windows for mdiswiz_lib.lib
 * M: UTIL_UNREF_PARAM added to unreferenced parameters
 *
 * Revision 2.1  2010/06/08 11:19:01  CRuff
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>

#include "wizdebug.h"
#include "modextf503.h"
#include "wizexcept.h"
#include "util.h"
#include "modbbischam.h"
#include "hwcomponent.h"


// -----------------------------------------------------------------
// F500

//! Create an F500  device
/*!

*/
ModExtF503::ModExtF503( bool withSubDevs ) :	ESMexprCarrier( "F503" )
{
	BusInterface* fpgaBusIf=0;
	ModBbisCham* bbis=0;

	setDescription( "CPCI 3U ESMexpress carrier" );
	setInstName("F503");

	addChild( new F503PciSlotIf( 0, pciDomain ) );
	
	// add fpga interface
	
	fpgaBusIf = new F503PciFpgaIf( 0, pciDomain );
	addChild( fpgaBusIf );
	
    // Chameleon FPGA
    if (withSubDevs)
    {
	    bbis = new ModBbisCham("PCITBL", 0x00);
	    bbis->setInstName("FPGA");
	    fpgaBusIf->addChild( bbis );
	}
}

// ----------------------------------------
// F500PciSlotIf
//! determine CPCI path from the ESM CPU board and add the CPCI slot
bool
F503PciSlotIf::pciBusPath( int slotNo, Q3MemArray<uchar> *pciPath )
{
	UTIL_UNREF_PARAM(slotNo);
	wDebug(("F503PciSlotIf::pciBusPath"));
	// when not connected directly to ESM go to ModExtF503
	if( (basePciPath.size() == 0) && getParent() && getParent()->getParent() ){
		HwComponent *parent;
		PciBusInterface *cpuIf;
			
		parent = getParent();
		WIZ_ASSERT(parent);;
			
		parent = parent->getParent();	// goto CPU busIf
		WIZ_ASSERT(parent);
	
		WIZ_DYNAMIC_CAST( parent, cpuIf, PciBusInterface * );

		if( cpuIf->pciBusPath( -1, &basePciPath ) == false )
			return false;
	}
	
	pciPath->duplicate( basePciPath );
	int count = pciPath->size();
	pciPath->resize( count + 4 );
	pciPath->at(count) = 0x0;
	pciPath->at(count+1) = 0x0;
	pciPath->at(count+2) = 0x2;
	pciPath->at(count+3) = 0x0;
	
	return true;
}

// ----------------------------------------
// F503PciFpgaIf
//! determine fpga path from the ESM CPU board and add the CPCI slot
bool
F503PciFpgaIf::pciBusPath( int slotNo, Q3MemArray<uchar> *pciPath )
{
	UTIL_UNREF_PARAM(slotNo);
	wDebug(("F503PciFpgaIf::pciBusPath"));
	wDebug(("basepcipath: %d",basePciPath.size()));
	// when not connected directly to ESM go to ModExtF500
	if( (basePciPath.size() == 0) && getParent() && getParent()->getParent()){
		wDebug(("path size 0 "));
		HwComponent *parent;
		PciBusInterface *cpuIf;

		parent = getParent();
		WIZ_ASSERT(parent);

		parent = parent->getParent();	// goto CPU busIf
		WIZ_ASSERT(parent);
		wDebug(("parent name: %s",parent->getInstName().ascii()));
		WIZ_DYNAMIC_CAST( parent, cpuIf, PciBusInterface * );

		if( cpuIf->pciBusPath( -1, &basePciPath ) == false )
			return false;
		wDebug(("basePciPathLen: %d",basePciPath.size()));
	}
	pciPath->duplicate( basePciPath );
	int count = pciPath->size();
	pciPath->resize( count + 4);
	pciPath->at(count) = 0x0;
	pciPath->at(count+1) = 0x0;
	pciPath->at(count+2) = 0x1;
	pciPath->at(count+3) = 0x0;
	
	return true;
}


