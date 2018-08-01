/***************************************************************************
 */
/*!       \file  modcpupp1.h
 *       \brief  MEN PP1 module classes
 *      \author	 Ulrich.Bogensperger@men.de
 *		  $Date: 2011/09/26 09:17:58 $
 *    $Revision: 2.2 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpupp1.h,v $
 * Revision 2.2  2011/09/26 09:17:58  dpfeuffer
 * R: PCI-104 support for PP1 model
 * M: ModBbisPp1ObPci104 added
 *
 * Revision 2.1  2010/05/19 17:50:50  CRuff
 * R: 1. add SMB controller support
 * M: 1. inherit from CpuDeviceSmb instead of CpuDevice
 *
 * Revision 2.0  2010/01/22 11:30:50  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.3  2006/07/27 13:43:35  ub
 * Added: Class for BBIS on PP1
 *
 * Revision 1.2  2005/12/07 16:55:23  ub
 * PP1 model enhanced
 *
 * Revision 1.1  2005/07/15 15:47:34  ub
 * Initial Revision
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODCPUPP1_H
#define MODCPUPP1_H

#include "hwcomponent.h"
#include "modbbispcigen.h"

// -----------------------------------------------------------------
//! MEN CPU PP1

class ModCpuPp1 : public CpuDeviceSmb
{
public:
	ModCpuPp1( bool withSubDevs );

	// create another instance
	virtual Device *create( bool withSubDevs ){
		return new ModCpuPp1( withSubDevs );
	}

	Arch getArch() { return Ppc; }
};

//! BBIS class for PP1 onboard PCI-104
class ModBbisPp1ObPci104 : public ModBbisPciGen
{
public:
	ModBbisPp1ObPci104(bool withSubDevs=true );
	virtual Device *create(bool withSubDevs=true) { 
		withSubDevs = withSubDevs;
		return new ModBbisPp1ObPci104();	
	}
};

#endif
