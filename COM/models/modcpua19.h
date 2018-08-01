/***************************************************************************
*/
/*!       \file  modcpua19.h
 *       \brief  MEN CPU A19/A20 classes
 *      \author  Christine.Ruff@men.de
 *        $Date: 2014/08/22 15:56:50 $
 *    $Revision: 2.2 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpua19.h,v $
 * Revision 2.2  2014/08/22 15:56:50  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.1  2010/05/19 17:44:54  CRuff
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
 
#ifndef MODCPUA19_H_
#define MODCPUA19_H_

#include "hwcomponent.h"
#include "modbbispcigen.h"


// -----------------------------------------------------------------
//! MEN CPU A19

class ModCpuA19 : public CpuDeviceSmb
{
public:
	ModCpuA19( bool withSubDevs );
	
	// create another instance
	virtual Device *create( bool withSubDevs=true ){
			return new ModCpuA19(withSubDevs);
	};
	virtual SwModuleList *enumSwModules();
	Arch getArch() { return X86; }
};

// -----------------------------------------------------------------
//! MEN CPU A20

class ModCpuA20 : public CpuDeviceSmb
{
public:
	ModCpuA20( bool withSubDevs );
	
	// create another instance
	virtual Device *create( bool withSubDevs=true ){
			return new ModCpuA20(withSubDevs);
	};
	virtual SwModuleList *enumSwModules();
	Arch getArch() { return X86; }
};

#endif /*MODCPUA19_H_*/
