/***************************************************************************
 */
/*!       \file  modcpuem02.h
 *       \brief  MEN EM02 module classes
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2004/06/08 17:26:29 $
 *    $Revision: 1.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuem02.h,v $
 * Revision 1.1  2004/06/08 17:26:29  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright by MEN mikro elektronik GmbH, Nuernberg, Germany
 ****************************************************************************/
#ifndef MODCPUEM02_H
#define MODCPUEM02_H

#include "hwcomponent.h"

// -----------------------------------------------------------------
//! MEN CPU Em02 

class ModCpuEm02 : public CpuDevice
{
public:
	ModCpuEm02(	bool withSubDevs );

	// create another instance
	virtual Device *create(bool withSubDevs=true){
		return new ModCpuEm02( withSubDevs);		
	}
	Arch getArch() { return X86; }
};

#endif


