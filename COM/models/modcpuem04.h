/***************************************************************************
 */
/*!       \file  modcpuem04.h
 *       \brief  MEN EM04 module classes
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2004/06/08 17:26:32 $
 *    $Revision: 1.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuem04.h,v $
 * Revision 1.1  2004/06/08 17:26:32  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright by MEN mikro elektronik GmbH, Nuernberg, Germany
 ****************************************************************************/
#ifndef MODCPUEM04_H
#define MODCPUEM04_H

#include "hwcomponent.h"

// -----------------------------------------------------------------
//! MEN CPU Em04 

class ModCpuEm04 : public CpuDevice
{
public:
	ModCpuEm04(	bool withSubDevs );

	// create another instance
	virtual Device *create(bool withSubDevs=true){
		return new ModCpuEm04( withSubDevs );
	}

	Arch getArch() { return Ppc; }
};

#endif


