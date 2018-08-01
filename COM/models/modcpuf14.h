/***************************************************************************
 */
/*!       \file  modcpuf14.h
 *       \brief  MEN CPU F14 classes
 *      \author	 Christian.Schuster@men.de
 *		  $Date: 2012/05/23 11:42:57 $
 *    $Revision: 2.3 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuf14.h,v $
 * Revision 2.3  2012/05/23 11:42:57  ts
 * R: Adding of cPCI components to F14 wasnt possible anymore
 * M: changed constructor of CpuModF14 to not overwrite cPCI property
 *
 * Revision 2.2  2011/09/26 09:17:40  dpfeuffer
 * R:1. ModCpuF19P moved from modcpuf14.h to modcpuf19p.h
 *   2. ModCpuF14 modifications required
 * M:1. ModCpuF19P removed
 *   2. minor changes in ModCpuF14 member functions
 *
 * Revision 2.1  2010/05/19 17:50:48  CRuff
 * R: 1. add SMB controller support
 * M: 1. inherit from CpuDeviceSmb instead of CpuDevice
 *
 * Revision 2.0  2010/01/22 11:30:42  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.3  2009/04/21 15:26:40  CRuff
 * R: support new board F19P
 * M: added new class ModCpuF19P
 *
 * Revision 1.2  2007/10/05 16:23:24  cs
 * added:
 *   + F18
 *
 * Revision 1.1  2006/11/30 11:28:32  cs
 * Initial Revision
 *
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2006-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODCPUF14_H
#define MODCPUF14_H

#include "hwcomponent.h"
#include "descriptor.h"
#include "modbbisisa.h"

// -----------------------------------------------------------------
//! MEN CPU F14

class ModCpuF14 : public CpuDeviceSmb
{
public:
	ModCpuF14( bool withSubDevs=true, bool withCpci=true );
	// create another instance
	virtual Device *create( bool withSubDevs=true ){
			return new ModCpuF14( withSubDevs );
	};

	Arch getArch() { return X86; }

	virtual QString getMainProperties(){
		return QString("CompactPCI Slot 1 (System Slot)");
	}
};

// -----------------------------------------------------------------
//! MEN CPU F15

class ModCpuF15 : public ModCpuF14
{
public:
	ModCpuF15( bool withSubDevs=true ):
		ModCpuF14( withSubDevs )
	{
	    setHwName( "F15" );
	    setDescription( "CompactPCI/Express Core Duo CPU" );

	};
	// create another instance
	virtual Device *create( bool withSubDevs=true ){
			return new ModCpuF15( withSubDevs );
	};
};

// -----------------------------------------------------------------
//! MEN CPU F17

class ModCpuF17 : public ModCpuF15
{
public:
	ModCpuF17( bool withSubDevs=true ):
		ModCpuF15( withSubDevs )
	{
	    setHwName( "F17" );
	    setDescription( "CompactPCI/Express Core 2 Duo 64bit CPU" );
	};
	// create another instance
	virtual Device *create( bool withSubDevs=true ){
			return new ModCpuF17(withSubDevs);
	};
};

// -----------------------------------------------------------------
//! MEN CPU F18

class ModCpuF18 : public ModCpuF15
{
public:
	ModCpuF18( bool withSubDevs=true ):
		ModCpuF15( withSubDevs )
	{
	    setHwName( "F18" );
	    setDescription( "CompactPCI/Express Core 2 Duo 64bit CPU" );
	};
	// create another instance
	virtual Device *create( bool withSubDevs=true ){
			return new ModCpuF18(withSubDevs);
	};
};

#endif


