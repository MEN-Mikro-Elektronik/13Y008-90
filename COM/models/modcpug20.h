/***************************************************************************
 */
/*!       \file  modcpug20.h
 *       \brief  MEN CPU G20 classes
 *      \author	 dieter.pfeuffer@men.de
 *		  $Date: 2016/02/20 16:27:11 $
 *    $Revision: 2.3 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpug20.h,v $
 * Revision 2.3  2016/02/20 16:27:11  ts
 * R: BoxPCs based on SC24 need 2 SMBusses in MDIS wizard
 * M: 1. added member nrSmbBusses to class CpuDeviceSmb
 *    2. added parameter int nrSmbBusses to every constructor of CPUs with SMB
 *       controllers: 2 for SC24 (AMD based) Box PCs, 1 for every other CPU
 *
 * Revision 2.2  2014/06/06 14:19:14  ts
 * R: new CPU G22 was missing
 * M: added G22 (same device as G20 but should appear separately for customers)
 *
 * Revision 2.1  2011/09/26 09:17:47  dpfeuffer
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2006-2011 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODCPUG20_H
#define MODCPUG20_H

#include "hwcomponent.h"
#include "descriptor.h"
#include "modbbisisa.h"

// -----------------------------------------------------------------
//! MEN CPU G20

class ModCpuG20 : public CpuDeviceSmb
{
public:
	ModCpuG20( bool withSubDevs=true );
	// create another instance
	virtual Device *create( bool withSubDevs=true ){
			return new ModCpuG20( withSubDevs );
	};
	
	Arch getArch() { return X86; }

	virtual QString getMainProperties(){
		return QString("CompactPCI Serial Slot 1 (System Slot)");
	}
};

class ModCpuG22 : public CpuDeviceSmb
{
public:
	ModCpuG22( bool withSubDevs=true );
	// create another instance
	virtual Device *create( bool withSubDevs=true ){
			return new ModCpuG22( withSubDevs );
	};
	
	Arch getArch() { return X86; }

	virtual QString getMainProperties(){
		return QString("CompactPCI Serial Slot 1 (System Slot)");
	}
};

class ModCpuG23 : public CpuDeviceSmb
{
public:
	ModCpuG23( bool withSubDevs=true );
	// create another instance
	virtual Device *create( bool withSubDevs=true ){
			return new ModCpuG23( withSubDevs );
	};
	
	Arch getArch() { return X86; }

	virtual QString getMainProperties(){
		return QString("CompactPCI Serial Slot 1 (System Slot)");
	}
};

class ModCpuG25A : public CpuDeviceSmb
{
public:
	ModCpuG25A( bool withSubDevs=true );
	// create another instance
	virtual Device *create( bool withSubDevs=true ){
			return new ModCpuG25A( withSubDevs );
	};
	
	Arch getArch() { return X86; }

	virtual QString getMainProperties(){
		return QString("CompactPCI Serial Slot 1 (System Slot)");
	}
};


#endif


