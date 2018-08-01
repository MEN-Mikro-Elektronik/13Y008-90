/***************************************************************************
 */
/*!       \file  modexta500.h
 *       \brief  class for A500
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2014/08/22 15:57:35 $
 *    $Revision: 2.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modexta500.h,v $
 * Revision 2.1  2014/08/22 15:57:35  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.0  2010/01/22 11:30:55  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.2  2006/11/30 11:28:37  cs
 * export ModExtA500IfsCreate()
 *
 * Revision 1.1  2004/06/08 17:26:57  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODEXTA500_H
#define MODEXTA500_H

#include <Q3MemArray>

#include "hwcomponent.h"
#include "esmcarrier.h"
#include "modbbispcigen.h"

class ModExtA500 : public EsmCarrier
{
public:
	enum Mezzanines { WithPcMips, WithMmods, WithPmcs  };
	ModExtA500( Mezzanines mezzanines, bool withSubDevs=true);
	Device *create(bool withSubDevs);
	virtual SwModuleList *enumSwModules();
protected:
	Mezzanines _mezzanines;
};

//! Add A500 Interfaces and BBISes
extern void ModExtA500IfsCreate( HwComponent *parentComp,
								 QString &hwName,
								 QString &descr,
								 ModExtA500::Mezzanines mezzanines,
								 Q3MemArray<uchar> *pciPath=0,
								 bool withSubDevs=true );

//! BBIS class for A500 onboard M-Modules/devices
class ModBbisA500 : public BbisDevice
{
 public:
	ModBbisA500(bool withSubDevs=true);
	virtual Device *create(bool withSubDevs=true) {
		return new ModBbisA500(withSubDevs);
	}
};

#endif




