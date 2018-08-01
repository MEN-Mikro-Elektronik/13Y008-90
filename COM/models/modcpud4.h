/***************************************************************************
 */
/*!       \file  modcpud4.h
 *       \brief  MEN CPU D4 classes
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2014/08/22 15:57:13 $
 *    $Revision: 2.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpud4.h,v $
 * Revision 2.1  2014/08/22 15:57:13  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.0  2010/01/22 11:30:05  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.1  2004/06/08 17:26:27  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODCPUD4_H
#define MODCPUD4_H

#include "hwcomponent.h"
#include "descriptor.h"
#include "modbbispcigen.h"

// -----------------------------------------------------------------
//! MEN CPU D4 

class ModCpuD4 : public CpuDevice
{
public:
	enum Mezzanines { WithPcMips, WithPmcs  };

	ModCpuD4( Mezzanines mezzanines, 
			  bool withSubDevs=true );
	// create another instance
	virtual Device *create(bool withSubDevs=true);	
	Arch getArch() { return X86; }
private:
	Mezzanines _mezzanines;
};

#endif


