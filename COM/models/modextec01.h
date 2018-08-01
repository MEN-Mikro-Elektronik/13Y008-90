/***************************************************************************
 */
/*!       \file  modextec01.h
 *       \brief  class for EC01
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:31:00 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modextec01.h,v $
 * Revision 2.0  2010/01/22 11:31:00  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.2  2005/07/15 15:47:37  ub
 * added model for EC01N
 *
 * Revision 1.1  2004/06/08 17:26:59  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODEXTEC01_H
#define MODEXTEC01_H

#include "hwcomponent.h"
#include "esmcarrier.h"

class ModExtEc01 : public EsmCarrier
{
public:
	ModExtEc01();
	Device *create(bool withSubDevs);
protected:	
private:
};

class ModExtEc01n : public EsmCarrier
{
public:
	ModExtEc01n();
	Device *create(bool withSubDevs);
protected:	
private:
};

#endif
