/***************************************************************************
 */
/*!       \file  modcpuem05.h
 *       \brief  MEN EM05 module classes
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2005/03/09 13:49:41 $
 *    $Revision: 1.2 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuem05.h,v $
 * Revision 1.2  2005/03/09 13:49:41  ub
 * added Chameleon native Linux driver to software module list
 *
 * Revision 1.1  2004/06/08 17:26:34  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright by MEN mikro elektronik GmbH, Nuernberg, Germany
 ****************************************************************************/
#ifndef MODCPUEM05_H
#define MODCPUEM05_H

#include "hwcomponent.h"
//#include "descriptor.h"

// -----------------------------------------------------------------
//! MEN CPU Em05 

class ModCpuEm05 : public CpuDevice
{
public:
	ModCpuEm05(	bool withSubDevs );

	// create another instance
	virtual Device *create(bool withSubDevs=true);	
	Arch getArch() { return X86; }
    virtual SwModuleList *enumSwModules();
};

#endif


