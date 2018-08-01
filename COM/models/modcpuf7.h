/***************************************************************************
 */
/*!       \file  modcpuf7.h
 *       \brief  MEN CPU F7 classes
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:30:33 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuf7.h,v $
 * Revision 2.0  2010/01/22 11:30:33  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.4  2004/06/08 17:26:50  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.3  2004/01/09 08:21:00  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/06/11 12:22:18  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/06/03 10:18:11  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODCPUF7_H
#define MODCPUF7_H

#include "hwcomponent.h"
#include "descriptor.h"
#include "modbbisisa.h"

// -----------------------------------------------------------------
//! MEN CPU F7 

class ModCpuF7 : public CpuDevice
{
public:
	ModCpuF7(bool withSubDevs=true );
	// create another instance
	virtual Device *create(bool withSubDevs=true);	
	Arch getArch() { return X86; }
};

//! BBIS class for F7 onboard M-Modules/devices
class ModBbisF7 : public ModBbisIsa
{
 public:
	ModBbisF7(bool withSubDevs=true);
	virtual Device *create(bool withSubDevs=true) { 
		return new ModBbisF7(withSubDevs);	
	}
};


#endif


