/***************************************************************************
 */
/*!       \file  modcpua11.h
 *       \brief  MEN CPU A11 classes
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2014/08/22 15:56:38 $
 *    $Revision: 2.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpua11.h,v $
 * Revision 2.1  2014/08/22 15:56:38  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.0  2010/01/22 11:29:36  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.1  2004/06/08 17:26:10  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODCPUA11_H
#define MODCPUA11_H

#include "hwcomponent.h"
#include "modbbispcigen.h"
#include "util.h"

// -----------------------------------------------------------------
//! MEN CPU A11 

class ModCpuA11 : public CpuDevice
{
public:
	ModCpuA11(bool withSubDevs=true );
	virtual Device *create(bool withSubDevs=true);	
	Arch getArch() { return Ppc; }
};

//! BBIS class for A11 onboard M-Modules/devices
class ModBbisA11 : public BbisDevice
{
 public:
	ModBbisA11(bool withSubDevs=true);
	virtual Device *create(bool withSubDevs=true) { 
		return new ModBbisA11(withSubDevs);	
	}
};

#endif



