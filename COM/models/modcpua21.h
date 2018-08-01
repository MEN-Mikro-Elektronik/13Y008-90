/***************************************************************************
 */
/*!       \file  modcpua21.h
 *       \brief  MEN CPU A21 classes
 *      \author	 Dieter.Pfeuffer@men.de
 *		  $Date: 2015/05/26 17:19:18 $
 *    $Revision: 2.4 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpua21.h,v $
 * Revision 2.4  2015/05/26 17:19:18  ts
 * R: gituser autocheckin: updated source
 *
 * Revision 2.3  2014/08/22 15:56:54  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.2  2014/07/18 15:12:30  dpfeuffer
 * R: erroneous PMC support, missing ComExpress support, general maintenance
 * M: intermediate check-in during development
 *
 * Revision 2.1  2013/03/04 13:07:31  dpfeuffer
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2013 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODCPUA21_H
#define MODCPUA21_H

#include "hwcomponent.h"
#include "descriptor.h"
#include "modbbispcigen.h"

// -----------------------------------------------------------------
//! MEN CPU A21 

class ModCpuA21 : public CpuDeviceSmb
{
public:
	enum SubModel { A21, A21Msi };
	enum Mezzanines { WithMmods, WithPmcs  };
	ModCpuA21(SubModel submod, Mezzanines mezzanines, bool withSubDevs=true );
	// create another instance
	virtual Device *create(bool withSubDevs=true);	
	SwModuleList *enumSwModules();
	Arch getArch() { return Ppc; }
private:
	SubModel _submod;
	Mezzanines _mezzanines;
};

//! BBIS class for A21 onboard M-Modules/devices
class ModBbisA21 : public BbisDevice
{
 public:
	ModBbisA21(bool withSubDevs=true);
	virtual Device *create(bool withSubDevs=true) { 
		return new ModBbisA21(withSubDevs);	
	}
	SwModuleList *enumSwModules();
	virtual QString getDriverName( bool fullName=false, bool withSw=true); 
};

//! BBIS class for A21 onboard M-Modules/devices with MSI support
class ModBbisA21Msi : public BbisDevice
{
 public:
	ModBbisA21Msi(bool withSubDevs=true);
	virtual Device *create(bool withSubDevs=true) { 
		return new ModBbisA21Msi(withSubDevs);	
	}
	SwModuleList *enumSwModules();
	virtual QString getDriverName( bool fullName=false, bool withSw=true); 
};

//! BBIS helper class for A21 MSI activation
class ModBbisA21MsiEnable : public BbisDevice
{
 public:
	ModBbisA21MsiEnable(bool withSubDevs=true);
	virtual Device *create(bool withSubDevs=true) { 
		return new ModBbisA21MsiEnable(withSubDevs);	
	}
	SwModuleList *enumSwModules();
	virtual QString getDriverName( bool fullName=false, bool withSw=true); 
};

#endif
