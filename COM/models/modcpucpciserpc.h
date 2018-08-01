/***************************************************************************
 */
/*!       \file  modcpucpciserpc.h
 *       \brief  contains subclasses for real HW components
 *      \author	 dieter.pfeuffer@men.de
 *		  $Date: 2011/09/26 08:43:10 $
 *    $Revision: 2.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpucpciserpc.h,v $
 * Revision 2.1  2011/09/26 08:43:10  dpfeuffer
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2011 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODCPUCPCISER_H
#define MODCPUCPCISER_H

#include "hwcomponent.h"

// -----------------------------------------------------------------
//! Generic CompactPCI Serial PC class

class ModCpuCpciSerPc : public CpuDevice 
{
 public:
	ModCpuCpciSerPc(bool withSubDevs=true);
	// create another instance
	virtual Device *create(bool withSubDevs=true);	
	Arch getArch() { return X86; }	
};

#endif

