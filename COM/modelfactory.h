/***************************************************************************
 */
/*!       \file  modelfactory.h
 *       \brief  CPU, BBIS and MDIS model factory
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:26:42 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modelfactory.h,v $
 * Revision 2.0  2010/01/22 11:26:42  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.2  2003/06/03 10:16:25  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:36:09  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODELFACTORY_H
#define MODELFACTORY_H

#include <qlist.h>

#include <Q3PtrList>

#include "hwcomponent.h"

class Device;
// -----------------------------------------------------------------
//! Model factory 
/*!
  The model factory is used to group models of the same type together.
  There is one global object created for CPUs #G_cpuMods, BBIS models 
  #G_bbisMods and MDIS devices #G_mdisMods.
*/
class ModelFactory 
{
public:
	ModelFactory();
	
	Device *first();			// get first model
	Device *next();				// get next model
	Device *find( const QString &modelName ); // find device by hw name 
	void append( Device *dev );
protected:
	Q3PtrList<Device> lstModels;	//!< models maintained by this factory
};

// -----------------------------------------------------------------
//! CpuModelFactory

class CpuModelFactory : public ModelFactory
{
public:
	CpuModelFactory();
};

// -----------------------------------------------------------------
//! BbisModelFactory

class BbisModelFactory : public ModelFactory
{
public:
	BbisModelFactory();
};

// -----------------------------------------------------------------
//! MdisModelFactory

class MdisModelFactory : public ModelFactory
{
public:
	MdisModelFactory();
};

#endif

