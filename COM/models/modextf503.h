/***************************************************************************
 */
/*!       \file  modextf503.h
 *       \brief  class for F501
 *      \author	 christine.ruff@men.de
 *		  $Date: 2014/07/18 15:12:43 $
 *    $Revision: 2.3 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modextf503.h,v $
 * Revision 2.3  2014/07/18 15:12:43  dpfeuffer
 * R: unnecessary modextf503.h include
 * M: modextf503.h include removed
 *
 * Revision 2.2  2011/04/01 09:44:29  CRuff
 * R: support of pci domains
 * M: for F503PciSlotIf and F503PciFpgaIf, introduce new constructor parameter
 *    pciDomain and hand it over to parent class constructor PciBusInterface
 *
 * Revision 2.1  2010/06/08 11:19:02  CRuff
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODEXTF503_H
#define MODEXTF503_H

#include <Q3MemArray>

#include "hwcomponent.h"
#include "esmcarrier.h"
#include "wizexcept.h"

class ModExtF503 : public ESMexprCarrier
{
public:
	ModExtF503( bool withSubDevs );
	Device *create(bool withSubDevs=true){
        return new ModExtF503( withSubDevs );
    }
};


// -----------------------------------------------------------------
//! Specialized PciBusInterface for F500 CPCI interface
/*!
  overrides pciBusPath to get the bus path from the ESM PCI bus
*/
class F503PciSlotIf : public PciBusInterface {
public:
	F503PciSlotIf( const Q3MemArray<uchar> *_basePciPath=0, int pciDomain=0 ) :
		PciBusInterface( Cpci, 2, 8, 0xf, _basePciPath, true, pciDomain )
	{
		setInstName("CompactPci Bus");
  	}

	//! determine CPCI path from the ESM CPU board and add the CPCI slot
	bool pciBusPath( int slotNo, Q3MemArray<uchar> *pciPath );
};

class F503PciFpgaIf : public PciBusInterface {
public:
	F503PciFpgaIf( const Q3MemArray<uchar> *_basePciPath=0, int pciDomain=0 ) :
		PciBusInterface( StdPci, 0, 0, 0, _basePciPath, false, pciDomain )
	{
		setInstName("F503 Fpga Bus Interface");
  	}

	//! determine CPCI path from the ESM CPU board and add the CPCI slot
	bool pciBusPath( int slotNo, Q3MemArray<uchar> *pciPath );
};

#endif




