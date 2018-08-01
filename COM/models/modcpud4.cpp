/***************************************************************************
 */
/*!       \file  modcpud4.cpp 
 *       \brief  Classes for model MEN D4 and derivates
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2014/08/22 15:57:11 $
 *    $Revision: 2.1 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpud4.cpp,v $
 * Revision 2.1  2014/08/22 15:57:11  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.0  2010/01/22 11:30:03  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.2  2009/04/27 17:14:52  CRuff
 * R: Provide CPU selection possibility for Intel CPUs
 * M: Added CPU cores Pentium/Pentium2/Pentium3 to D4
 *
 * Revision 1.1  2004/06/08 17:26:26  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>

#include "modcpud4.h"
#include "wizdebug.h"
#include "util.h"

// -----------------------------------------------------------------
// CPU D4

//! Creates an D4 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the 
  BBIS devices
*/
ModCpuD4::ModCpuD4( 
	Mezzanines mezzanines,
	bool withSubDevs ): 
	CpuDevice( QString(""), QString("PC"), false )
{
	BusInterface *busIf2, *busIf1;
	QString hwName = "D4";
	QString descr = "CompactPci Pentium4 CPU ";

	UTIL_UNREF_PARAM(withSubDevs);

	_mezzanines = mezzanines;

	lstCpuCores << Pentium << Pentium2 << Pentium3 << Pentium4;

	// D4
	switch( _mezzanines ){
	case WithPcMips: 	
		hwName += "_PCMIP"; 
	 	descr += "with 2 PC-MIP slots"; 

	{	
        // Interface for D4 BBISes	
		Q3MemArray<uchar> pciBusPath(3);
		pciBusPath[0] = 0x1e; 
		pciBusPath[1] = 0x5; 
		pciBusPath[2] = 0xd; 

		Q3MemArray<uchar> pciBusTbl(2);
		pciBusTbl[0] = 0x0f;
		pciBusTbl[1] = 0x0e;

		busIf2 = new PciBusInterface( PcMip, 0, 1, &pciBusPath, &pciBusTbl, true );
		busIf2->setInstName(QString("onboard PC-MIP"));
		addChild( busIf2 );
	}

		break;
	case WithPmcs:
 		hwName += "_PMC";
		descr += "with PMC slot"; 

	{	
        // Interface for D4 BBISes	
		Q3MemArray<uchar> pciBusPath(2);
		pciBusPath[0] = 0x1e; 
		pciBusPath[1] = 0x5; 

		Q3MemArray<uchar> pciBusTbl(1);
		pciBusTbl[0] = 0x0d;

		busIf2 = new PciBusInterface( Pmc, 0, 0, &pciBusPath, &pciBusTbl, true );
		busIf2->setInstName(QString("onboard PMC"));
		addChild( busIf2 );
	}

		break;
	}

	setHwName( hwName );
	setDescription(descr);	

	// Create the bus interfaces

	Q3MemArray<uchar> cpciBusPath(2);
	cpciBusPath[0] = 0x1e; 
	cpciBusPath[1] = 0x04;
	
	// Interface for CompactPci
	busIf1 = new PciBusInterface( Cpci, 2, 8, 0xf, &cpciBusPath, true);

	addChild( busIf1 );
	busIf1->setInstName( QString("CompactPci Bus"));
}

Device *ModCpuD4::create(bool withSubDevs)
{
	return new ModCpuD4(_mezzanines, withSubDevs);
}



