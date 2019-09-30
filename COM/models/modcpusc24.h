/***************************************************************************
 */
/*!       \file  modcpusc24.h
 *       \brief  MEN CPU SC24 classes
 *      \author	 dieter.pfeuffer@men.de
 *		  $Date: 2016/02/20 16:27:19 $
 *    $Revision: 2.3 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpusc24.h,v $
 * Revision 2.3  2016/02/20 16:27:19  ts
 * R: BoxPCs based on SC24 need 2 SMBusses in MDIS wizard
 * M: 1. added member nrSmbBusses to class CpuDeviceSmb
 *    2. added parameter int nrSmbBusses to every constructor of CPUs with SMB
 *       controllers: 2 for SC24 (AMD based) Box PCs, 1 for every other CPU
 *
 * Revision 2.2  2015/05/26 17:20:06  ts
 * R: gituser autocheckin: updated source
 *
 * Revision 2.1  2011/09/26 09:17:50  dpfeuffer
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2011 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODCPUSC24_H
#define MODCPUSC24_H

#include "hwcomponent.h"
#include "descriptor.h"
#include "modbbischamisa.h"

// -----------------------------------------------------------------
//! MEN CPU DC15

class ModCpuDc15 : public CpuDeviceSmb
{
public:
	ModCpuDc15(bool withSubDevs=true );
	// create another instance
	virtual Device *create(bool withSubDevs=true);	
	Arch getArch() { return X86; }
};

//! BBIS class for SC24 onboard Chameleon FPGA
class ModBbisDc15 : public ModBbisChamIsa
{
 public:
	ModBbisDc15(bool withSubDevs=true);
	virtual Device *create(bool withSubDevs=true) { 
		return new ModBbisDc15(withSubDevs);
	}
};

// -----------------------------------------------------------------
//! MEN CPU Bx50x

class ModCpuBx50x : public CpuDeviceSmb
{
public:
	ModCpuBx50x(bool withSubDevs=true );
	// create another instance
	virtual Device *create(bool withSubDevs=true);	
	Arch getArch() { return X86; }
};

//! BBIS class for Bx50X onboard Chameleon FPGA
class ModBbisBx50x : public ModBbisChamIsa
{
 public:
	ModBbisBx50x(bool withSubDevs=true);
	virtual Device *create(bool withSubDevs=true) { 
		return new ModBbisBx50x(withSubDevs);	
	}
};

// -----------------------------------------------------------------
//! MEN CPU Bx51x

class ModCpuBx51x : public CpuDeviceSmb
{
public:
	ModCpuBx51x(bool withSubDevs=true );
	// create another instance
	virtual Device *create(bool withSubDevs=true);
	Arch getArch() { return X86; }
};

//! BBIS class for Bx51X onboard Chameleon FPGA
class ModBbisBx51x : public ModBbisChamIsa
{
 public:
	ModBbisBx51x(bool withSubDevs=true);
	virtual Device *create(bool withSubDevs=true) {
		return new ModBbisBx51x(withSubDevs);
	}
};

// -----------------------------------------------------------------
//! MEN CPU DC19/DC20

class ModCpuDCxx : public CpuDeviceSmb
{
public:
	ModCpuDCxx(bool withSubDevs=true );
	// create another instance
	virtual Device *create(bool withSubDevs=true);
	Arch getArch() { return X86; }
};

//! BBIS class for DC19/DC20 onboard Chameleon FPGA
class ModBbisDCxx : public ModBbisChamIsa
{
 public:
	ModBbisDCxx(bool withSubDevs=true);
	virtual Device *create(bool withSubDevs=true) {
		return new ModBbisDCxx(withSubDevs);
	}
};
// -----------------------------------------------------------------
//! MEN CPU Bx70x

class ModCpuBx70x : public CpuDeviceSmb
{
public:
	ModCpuBx70x(bool withSubDevs=true );
	// create another instance
	virtual Device *create(bool withSubDevs=true);	
	Arch getArch() { return X86; }
};

//! BBIS class for Bx70X onboard Chameleon FPGA
class ModBbisBx70x : public ModBbisChamIsa
{
 public:
	ModBbisBx70x(bool withSubDevs=true);
	virtual Device *create(bool withSubDevs=true) { 
		return new ModBbisBx70x(withSubDevs);	
	}
};
#endif
