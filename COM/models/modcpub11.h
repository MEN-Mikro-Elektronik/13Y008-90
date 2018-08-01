/***************************************************************************
 */
/*!       \file  modcpub11.h
 *       \brief  MEN CPU B11 classes
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2014/08/22 15:56:59 $
 *    $Revision: 2.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpub11.h,v $
 * Revision 2.1  2014/08/22 15:56:59  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.0  2010/01/22 11:29:50  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.5  2004/07/16 12:31:57  ufranke
 * moved
 *  - cpuCoreGetDefaults() to parent class
 *
 * Revision 1.4  2004/06/25 14:42:18  ufranke
 * added
 *  - cpuCoreGetDefaults()
 *
 * Revision 1.3  2004/06/08 17:26:15  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.2  2003/06/11 12:22:02  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:33:25  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODCPUB11_H
#define MODCPUB11_H

#include "hwcomponent.h"
#include "descriptor.h"
#include "modbbispcigen.h"

// -----------------------------------------------------------------
//! MEN CPU B11 

class ModCpuB11 : public CpuDevice
{
public:
	ModCpuB11(bool withSubDevs=true );
	// create another instance
	virtual Device *create(bool withSubDevs=true);	
	virtual SwModuleList *enumSwModules();
	Arch getArch() { return Ppc; }
};

#endif


