/***************************************************************************
 */
/*!       \file  modcpud6.h
 *       \brief  MEN CPU D6 classes
 *      \author	 Christian.Schuster@men.de
 *		  $Date: 2014/08/22 15:57:17 $
 *    $Revision: 2.2 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpud6.h,v $
 * Revision 2.2  2014/08/22 15:57:17  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.1  2010/05/19 17:50:43  CRuff
 * R: 1. add SMB controller support
 * M: 1. inherit from CpuDeviceSmb instead of CpuDevice
 *
 * Revision 2.0  2010/01/22 11:30:10  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.2  2009/04/21 15:22:20  CRuff
 * R: support new boards D7, D9
 * M: added classes ModCpuD7, ModCpuD9 and supporting mezzanine bbises
 *
 * Revision 1.1  2006/11/30 11:28:26  cs
 * Initial Revision
 *
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2006-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODCPUD6_H
#define MODCPUD6_H

#include "hwcomponent.h"
#include "descriptor.h"
#include "modbbispcigen.h"

// -----------------------------------------------------------------
//! MEN CPU D6

class ModCpuD6 : public CpuDeviceSmb
{
public:
	ModCpuD6( bool withSubDevs=true );

	// create another instance
	virtual Device *create( bool withSubDevs=true ){
			return new ModCpuD6( withSubDevs );
	};
	Arch getArch() { return X86; }
};

// -----------------------------------------------------------------
//! MEN CPU D7

class ModCpuD7 : public ModCpuD6
{
public:
	ModCpuD7( bool withSubDevs=true )
	: ModCpuD6 ( withSubDevs )
	{
		hwName = QString("D7");
		setDescription("CompactPCI/Express Core Duo Xeon CPU");
		setModelName( QString("D7") );
	}

	// create another instance
	virtual Device *create( bool withSubDevs=true ){
			return new ModCpuD7( withSubDevs );
	};
	Arch getArch() { return X86; }
};

// -----------------------------------------------------------------
//! MEN CPU D9

class ModCpuD9 : public CpuDeviceSmb
{
public:
	ModCpuD9( bool withSubDevs );

	// create another instance
	virtual Device *create( bool withSubDevs=true ){
			return new ModCpuD9( withSubDevs );
	};
	Arch getArch() { return X86; }
};

#endif // MODCPUD6_H

