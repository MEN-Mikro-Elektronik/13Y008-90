/***************************************************************************
*/
/*!       \file  modcpua17.h
 *       \brief  MEN CPU A17 classes
 *      \author  Christine.Ruff@men.de
 *        $Date: 2014/08/22 15:56:45 $
 *    $Revision: 2.3 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpua17.h,v $
 * Revision 2.3  2014/08/22 15:56:45  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.2  2014/07/18 15:12:26  dpfeuffer
 * R: erroneous PMC support, missing ComExpress support, general maintenance
 * M: intermediate check-in during development
 *
 * Revision 2.1  2010/05/19 17:44:03  CRuff
 * R: 1. SMB controller support
 *    2. cosmetics
 * M: 1. inherit from CpuDeviceSmb instead of CpuDevice
 *    2. move classes for A19 and A20 in new file modcpua19.h
 *
 * Revision 2.0  2010/01/22 11:29:45  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.1  2009/04/21 15:39:10  CRuff
 * Initial Revision
 *
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
 
#ifndef MODCPUA17_H_
#define MODCPUA17_H_

#include "hwcomponent.h"
#include "modbbispcigen.h"

// -----------------------------------------------------------------
//! MEN CPU A17

class ModCpuA17 : public CpuDeviceSmb
{
public:
    ModCpuA17(  bool withSubDevs );

    // create another instance
    virtual Device *create(bool withSubDevs=true){
        return new ModCpuA17( withSubDevs );
    }
    virtual SwModuleList *enumSwModules();
    Arch getArch() { return Ppc; }
};

class ModVmeIfA17 : public VmeBusInterface
{
public:
	ModVmeIfA17();
	virtual ulong supportedIrqLevels();
};

#endif /*MODCPUA17_H_*/
