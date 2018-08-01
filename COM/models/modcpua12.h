/***************************************************************************
 */
/*!       \file  modcpua12.h
 *       \brief  MEN CPU A12/SC13/D3 classes
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2014/07/18 15:12:22 $
 *    $Revision: 2.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpua12.h,v $
 * Revision 2.1  2014/07/18 15:12:22  dpfeuffer
 * R: erroneous PMC support, missing ComExpress support, general maintenance
 * M: intermediate check-in during development
 *
 * Revision 2.0  2010/01/22 11:29:40  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.4  2004/07/16 12:31:35  ufranke
 * moved
 *  - cpuCoreGetDefaults() to parent class
 *
 * Revision 1.3  2004/06/25 13:46:05  ufranke
 * added
 *  - cpuCoreGetDefaults()
 *
 * Revision 1.2  2003/06/11 12:21:57  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:33:23  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:26:36  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:18:01  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODCPUA12_H
#define MODCPUA12_H

#include "hwcomponent.h"
#include "descriptor.h"
#include "modbbispcigen.h"

// -----------------------------------------------------------------
//! MEN CPU A12 

class ModCpuA12 : public CpuDevice
{
public:
	enum SubModel { A12, D3, SC13, A15, KahluaBox };
	enum Mezzanines { WithPcMips, WithMmods, WithPmcs  };
	enum ExtensionBoard { NoExtension, D201, D203 }; //!< for Kahlua box
	ModCpuA12(SubModel submod, Mezzanines mezzanines, 
			  ExtensionBoard extensionBoard, bool withSubDevs=true );
	// create another instance
	virtual Device *create(bool withSubDevs=true);	
	SwModuleList *enumSwModules();
	Arch getArch() { return Ppc; }
private:
	SubModel _submod;
	Mezzanines _mezzanines;
	ExtensionBoard _extensionBoard;
};

//! BBIS class for A12 onboard M-Modules/devices
class ModBbisA12 : public BbisDevice
{
 public:
	ModBbisA12(bool withSubDevs=true);
	virtual Device *create(bool withSubDevs=true) { 
		return new ModBbisA12(withSubDevs);	
	}
	SwModuleList *enumSwModules();
	virtual QString getDriverName( bool fullName=false, bool withSw=true); 
};

#endif


