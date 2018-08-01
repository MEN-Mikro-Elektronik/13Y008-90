/***************************************************************************
 */
/*!       \file  modcpuf1n.h
 *       \brief  MEN CPU F1N classes
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2014/08/22 15:57:22 $
 *    $Revision: 2.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuf1n.h,v $
 * Revision 2.1  2014/08/22 15:57:22  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.0  2010/01/22 11:30:19  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.5  2004/07/16 12:32:28  ufranke
 * moved
 *  - cpuCoreGetDefaults() to parent class
 *
 * Revision 1.4  2004/06/25 14:42:23  ufranke
 * added
 *  - cpuCoreGetDefaults()
 *
 * Revision 1.3  2004/06/08 17:26:38  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.2  2003/06/11 12:22:14  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:33:31  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:26:45  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:18:10  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODCPUF1N_H
#define MODCPUF1N_H

#include "hwcomponent.h"
#include "descriptor.h"
#include "modbbispcigen.h"

// -----------------------------------------------------------------
//! MEN CPU F1N 

class ModCpuF1N : public CpuDevice
{
public:
	ModCpuF1N(bool withSubDevs=true );
	// create another instance
	virtual Device *create(bool withSubDevs=true);	
	Arch getArch() { return Ppc; }
};

#endif


