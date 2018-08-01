/***************************************************************************
 */
/*!       \file  modcpuf9.h
 *       \brief  MEN CPU F9 classes
 *      \author	 Ulrich.Bogensperger@men.de
 *		  $Date: 2014/06/12 15:29:24 $
 *    $Revision: 2.2 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuf9.h,v $
 * Revision 2.2  2014/06/12 15:29:24  ts
 * R: new CPU F75P could not be selected
 * M: added F75P to models
 *
 * Revision 2.1  2010/06/02 16:52:09  CRuff
 * R: support for new board F11S
 * M: added class ModCpuF11S
 *
 * Revision 2.0  2010/01/22 11:30:37  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.1  2005/07/15 15:47:32  ub
 * Initial Revision
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODCPUF9_H
#define MODCPUF9_H

#include "hwcomponent.h"
#include "descriptor.h"
#include "modbbisisa.h"

// -----------------------------------------------------------------
//! MEN CPU F9 

class ModCpuF9 : public CpuDevice
{
public:
	ModCpuF9(bool withSubDevs=true );
	// create another instance
	virtual Device *create(bool withSubDevs=true);	
	Arch getArch() { return X86; }
};

// -----------------------------------------------------------------
//! MEN CPU F11S 

class ModCpuF11S : public CpuDeviceSmb
{
public:
	ModCpuF11S(bool withSubDevs = true);
	// create another instance
	virtual Device *create(bool withSubDevs=true);	
	Arch getArch() { return X86; }
};

// -----------------------------------------------------------------
//! MEN CPU F75P 

class ModCpuF75P : public CpuDeviceSmb
{
public:
	ModCpuF75P(bool withSubDevs = true);
	// create another instance
	virtual Device *create(bool withSubDevs=true);	
	Arch getArch() { return X86; }
};

#endif
