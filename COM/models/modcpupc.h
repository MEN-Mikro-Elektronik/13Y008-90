/***************************************************************************
 */
/*!       \file  modcpupc.h
 *       \brief  Standard PC class
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2014/08/22 15:57:31 $
 *    $Revision: 2.2 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpupc.h,v $
 * Revision 2.2  2014/08/22 15:57:31  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.1  2011/09/26 09:17:54  dpfeuffer
 * R: tests to add PCI-104 interface to standard PC model
 * M: no functional changes yet
 *
 * Revision 2.0  2010/01/22 11:30:46  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.3  2004/06/08 17:26:54  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.2  2003/06/11 12:22:22  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:33:33  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:26:48  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:18:13  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODCPUPC_H
#define MODCPUPC_H

#include "hwcomponent.h"
#include "descriptor.h"
#include "modbbispcigen.h"


// -----------------------------------------------------------------
//! Standard PC class

class ModCpuPc : public CpuDevice 
{
 public:
	ModCpuPc(bool withSubDevs=true);
	// create another instance
	virtual Device *create(bool withSubDevs=true);	
	Arch getArch() { return X86; }
};

#endif


