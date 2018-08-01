/***************************************************************************
 */
/*!       \file  modcpuf6.h
 *       \brief  MEN CPU F6 classes
 *      \author	 uf
 *		  $Date: 2010/01/22 11:30:28 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuf6.h,v $
 * Revision 2.0  2010/01/22 11:30:28  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.4  2004/07/16 12:33:40  ufranke
 * moved
 *  - cpuCoreGetDefaults() to parent class
 *
 * Revision 1.3  2004/06/25 13:46:31  ufranke
 * added
 *  - cpuCoreGetDefaults()
 *
 * Revision 1.2  2004/06/08 17:26:46  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.1  2004/05/14 17:13:41  UFranke
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODCPUF6_H
#define MODCPUF6_H

#include "hwcomponent.h"
#include "descriptor.h"

// -----------------------------------------------------------------
//! MEN CPU F6 

class ModCpuF6 : public CpuDevice
{
public:
	ModCpuF6(bool withSubDevs=true );
	virtual Device *create(bool withSubDevs=true);	
	Arch getArch() { return Ppc; }
};


#endif


