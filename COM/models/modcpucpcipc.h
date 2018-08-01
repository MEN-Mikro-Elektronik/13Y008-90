/***************************************************************************
 */
/*!       \file  modcpucpcipc.h
 *       \brief  contains subclasses for real HW components
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2011/09/26 08:43:06 $
 *    $Revision: 2.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpucpcipc.h,v $
 * Revision 2.1  2011/09/26 08:43:06  dpfeuffer
 * R: new model COMPACTPCI_PLUSIO_PC
 * M: new model class ModCpuCpciPcPlusIo added
 *
 * Revision 2.0  2010/01/22 11:29:55  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.3  2004/06/08 17:26:20  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.2  2003/06/11 12:22:06  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:33:27  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:26:41  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:18:06  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODCPUCPCI_H
#define MODCPUCPCI_H

#include "hwcomponent.h"

// -----------------------------------------------------------------
//! Generic CompactPCI PC class

class ModCpuCpciPc : public CpuDevice 
{
 public:
	ModCpuCpciPc(bool withSubDevs=true);
	// create another instance
	virtual Device *create(bool withSubDevs=true);	
	Arch getArch() { return X86; }	
};

// -----------------------------------------------------------------
//! Generic CompactPCI PlusIO PC class

class ModCpuCpciPcPlusIo : public ModCpuCpciPc
{
public:
	ModCpuCpciPcPlusIo( bool withSubDevs=true );

	// create another instance
	virtual Device *create( bool withSubDevs=true ){
			return new ModCpuCpciPcPlusIo( withSubDevs );
	};
};

#endif

