/***************************************************************************
 */
/*!       \file  modcpud2.h
 *       \brief  MEN CPU D2 classes
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2014/08/22 15:57:08 $
 *    $Revision: 2.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpud2.h,v $
 * Revision 2.1  2014/08/22 15:57:08  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.0  2010/01/22 11:30:00  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.3  2004/06/08 17:26:24  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.2  2003/06/11 12:22:10  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:33:29  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:26:43  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:18:08  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODCPUD2_H
#define MODCPUD2_H

#include "hwcomponent.h"
#include "descriptor.h"
#include "modbbispcigen.h"

// -----------------------------------------------------------------
//! MEN CPU D2 

class ModCpuD2 : public CpuDevice
{
public:
	ModCpuD2(bool withSubDevs=true );
	// create another instance
	virtual Device *create(bool withSubDevs=true);	
	Arch getArch() { return X86; }
};

//! BBIS class for D2 onboard M-Modules/devices
class ModBbisD2 : public BbisDevice
{
 public:
	ModBbisD2(bool withSubDevs=true);
	virtual Device *create(bool withSubDevs=true) { 
		return new ModBbisD2(withSubDevs);	
	}
	SwModuleList *enumSwModules();
};

#endif


