/***************************************************************************
 */
/*!       \file  modcpuf6.cpp 
 *       \brief  Classes for model MEN F6 
 *      \author	 uf
 *		  $Date: 2010/01/22 11:30:26 $
 *    $Revision: 2.0 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuf6.cpp,v $
 * Revision 2.0  2010/01/22 11:30:26  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.3  2004/07/23 14:50:17  ufranke
 * fixed
 *  - PCIbus swapped
 *
 * Revision 1.2  2004/06/08 17:26:44  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.1  2004/05/14 17:13:38  UFranke
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>

#include "modcpuf6.h"
#include "wizdebug.h"
#include "util.h"

// -----------------------------------------------------------------
// CPU F6

//! Creates an F6 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the 
  BBIS devices
*/
ModCpuF6::ModCpuF6(bool withSubDevs ): 
	CpuDevice( QString("F6"), QString("F6"), false )
{
	BusInterface *busIf1;

	UTIL_UNREF_PARAM(withSubDevs);

	lstCpuCores << Ppc603e;

	setDescription("CompactPCI MPC8260 CPU");	

	// Create the bus interfaces

	{
		// Interface for CompactPci
		Q3MemArray<uchar> cpciBusPath(0);

		busIf1 = new PciBusInterface( 
			Cpci, 2, 8, 0xf, &cpciBusPath);		// Interface for CompactPci
		addChild( busIf1 );
		busIf1->setInstName( QString("CompactPci Bus"));
		busIf1->setEndianSwapping( true );
	}

}

Device *ModCpuF6::create(bool withSubDevs)
{
	return new ModCpuF6(withSubDevs);
}




