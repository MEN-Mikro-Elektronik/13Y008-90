/***************************************************************************
 */
/*!       \file  modextf501.h
 *       \brief  class for F501
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:31:05 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modextf501.h,v $
 * Revision 2.0  2010/01/22 11:31:05  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.3  2006/11/30 11:28:41  cs
 * added:
 *   - moved classes for F50xPciSlotIf to here
 *   - model F500
 *
 * Revision 1.2  2005/07/15 15:47:41  ub
 * added model for F502
 *
 * Revision 1.1  2004/06/08 17:27:01  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODEXTF501_H
#define MODEXTF501_H

#include <Q3MemArray>

#include "hwcomponent.h"
#include "esmcarrier.h"
#include "wizexcept.h"


class ModExtF500 : public EsmCarrier
{
public:
	ModExtF500();
	Device *create(bool withSubDevs);
protected:
private:
};

class ModExtF501 : public EsmCarrier
{
public:
	ModExtF501();
	Device *create(bool withSubDevs);
protected:
private:
};


class ModExtF502 : public EsmCarrier
{
public:
	ModExtF502();
	Device *create(bool withSubDevs);
protected:
private:
};

// -----------------------------------------------------------------
//! Specialized PciBusInterface for F500 CPCI interface
/*!
  overrides pciBusPath to get the bus path from the ESM PCI bus
*/
class F500PciSlotIf : public PciBusInterface {
public:
	F500PciSlotIf( const Q3MemArray<uchar> *_basePciPath=0 ) :
		PciBusInterface( Cpci, 2, 8, 0xf, _basePciPath )
	{
		setInstName("CompactPci Bus");
  	}

	//! determine CPCI path from the ESM CPU board and add the CPCI slot
	bool pciBusPath( int slotNo, Q3MemArray<uchar> *pciPath );
};

//! Specialized PciBusInterface for F501 CPCI interface
/*!
  overrides pciBusPath to get the bus path from the ESM PCI bus
*/
class F501PciSlotIf : public PciBusInterface {
public:
	F501PciSlotIf( const Q3MemArray<uchar> *_basePciPath=0 ) :
		PciBusInterface( Cpci, 2, 8, 0xf, _basePciPath )
	{
		setInstName("CompactPci Bus");
  	}

	//! determine CPCI path from the ESM CPU board and add the CPCI slot
	bool pciBusPath( int slotNo, Q3MemArray<uchar> *pciPath );
};



#endif




