/***************************************************************************
 */
/*!       \file  modcpuf2.h
 *       \brief  MEN CPU F2 classes
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2014/08/22 15:57:24 $
 *    $Revision: 2.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuf2.h,v $
 * Revision 2.1  2014/08/22 15:57:24  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.0  2010/01/22 11:30:24  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.2  2004/06/08 17:26:42  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.1  2004/01/09 08:20:57  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODCPUF2_H
#define MODCPUF2_H

#include "hwcomponent.h"
#include "descriptor.h"
#include "modbbispcigen.h"

// -----------------------------------------------------------------
//! MEN CPU F2 

class ModCpuF2 : public CpuDevice
{
public:
	ModCpuF2(bool withSubDevs=true );
	// create another instance
	virtual Device *create(bool withSubDevs=true);	
	Arch getArch() { return X86; }
};

//! BBIS class for F2 onboard M-Modules/devices
class ModBbisF2 : public BbisDevice
{
 public:
	ModBbisF2(bool withSubDevs=true);
	virtual Device *create(bool withSubDevs=true) { 
		return new ModBbisF2(withSubDevs);	
	}
	SwModuleList *enumSwModules();
};

#endif


