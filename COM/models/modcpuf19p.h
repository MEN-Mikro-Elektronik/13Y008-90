/***************************************************************************
 */
/*!       \file  modcpuf19p.h
 *       \brief  MEN CPU F19P classes
 *      \author	 dieter.pfeuffer@men.de
 *		  $Date: 2018/06/20 11:35:01 $
 *    $Revision: 2.4 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuf19p.h,v $
 * Revision 2.4  2018/06/20 11:35:01  dpfeuffer
 * R: F26L introduction
 * M: add F26L support (implementation merged from M.Moese Git commit)
 *
 * Revision 2.3  2015/05/26 17:20:01  ts
 * R: gituser autocheckin: updated source
 *
 * Revision 2.2  2014/06/12 15:27:40  ts
 * R: new CPU F22P could not be selected
 * M: added new model F22P to models
 *
 * Revision 2.1  2011/09/26 09:17:44  dpfeuffer
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2006-2011 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODCPUF19P_H
#define MODCPUF19P_H

#include "modcpuf14.h"
#include "descriptor.h"
#include "modbbisisa.h"

// -----------------------------------------------------------------
//! MEN CPU F19P

class ModCpuF19P : public ModCpuF14
{
public:
	ModCpuF19P( bool withSubDevs=true );

	// create another instance
	virtual Device *create( bool withSubDevs=true ){
			return new ModCpuF19P( withSubDevs );
	};
};

// -----------------------------------------------------------------
//! MEN CPU F21P

class ModCpuF21P : public ModCpuF14
{
public:
	ModCpuF21P( bool withSubDevs=true );

	// create another instance
	virtual Device *create( bool withSubDevs=true ){
			return new ModCpuF21P( withSubDevs );
	};
};

// -----------------------------------------------------------------
//! MEN CPU F22P

class ModCpuF22P : public ModCpuF14
{
public:
	ModCpuF22P( bool withSubDevs=true );

	// create another instance
	virtual Device *create( bool withSubDevs=true ){
			return new ModCpuF22P( withSubDevs );
	};
};

// -----------------------------------------------------------------
//! MEN CPU F23P

class ModCpuF23P : public ModCpuF14
{
public:
	ModCpuF23P( bool withSubDevs=true );

	// create another instance
	virtual Device *create( bool withSubDevs=true ){
			return new ModCpuF23P( withSubDevs );
	};
};

// -----------------------------------------------------------------
//! MEN CPU F26L

class ModCpuF26L : public ModCpuF14
{
public:
	ModCpuF26L( bool withSubDevs=true );

	// create another instance
	virtual Device *create( bool withSubDevs=true ){
			return new ModCpuF26L( withSubDevs );
	};
};

// -----------------------------------------------------------------
//! MEN CPU F27P

class ModCpuF27P : public ModCpuF14
{
public:
	ModCpuF27P( bool withSubDevs=true );

	// create another instance
	virtual Device *create( bool withSubDevs=true ){
			return new ModCpuF27P( withSubDevs );
	};
};


#endif


