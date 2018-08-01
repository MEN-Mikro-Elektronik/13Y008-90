/***************************************************************************
 */
/*!       \file  modcpuesm.h
 *       \brief  MEN embedded system module classes
 *      \author  Klaus.Popp@men.de
 *        $Date: 2010/10/20 16:28:52 $
 *    $Revision: 2.2 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuesm.h,v $
 * Revision 2.2  2010/10/20 16:28:52  CRuff
 * R: cosmetics
 * M: inherit F12N from F12 to avoid inconsistencies
 *
 * Revision 2.1  2010/05/19 17:50:46  CRuff
 * R: 1. add SMB controller support
 * M: 1. inherit from CpuDeviceSmb instead of CpuDevice
 *
 * Revision 2.0  2010/01/22 11:30:15  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.6  2007/10/05 16:23:11  cs
 * added:
 *   + EM9
 *
 * Revision 1.5  2006/11/30 11:28:29  cs
 * added:
 *   + EM1N/A, EM7N, EM8, A13, A14, F11, F11N, F12, F13
 *   + EM2::-, EM7::newEsmBusIf()
 *
 * Revision 1.4  2006/08/04 11:50:15  ub
 * added irqNumber property
 *
 * Revision 1.3  2006/07/27 13:43:31  ub
 * Added: BBIS for MSCAN on MGT5200
 *
 * Revision 1.2  2005/07/15 15:47:29  ub
 * contains now models for all ESM modules
 *
 * Revision 1.1  2004/06/08 17:26:35  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODCPUESM_H
#define MODCPUESM_H

#include <Q3MemArray>

#include "hwcomponent.h"
#include "modbbisisa.h"

#include "models/modextf501.h"
#include "models/modexta500.h"

// -----------------------------------------------------------------
//! ESM class
class ModCpuEsm : public CpuDeviceSmb
{
public:
    ModCpuEsm( QString _hwName, QString _driverName  );
    BusInterface* newEsmBusIf( const Q3MemArray<uchar>* pciBusPath,
                               bool endianSwapping );
};


// -----------------------------------------------------------------
//! BBIS for MSCAN on MPC5200
class ModBbisMscan5200 : public ModBbisIsa
{
 public:
    ModBbisMscan5200( QString instName, long address, long irqNumber );

	virtual Device *create(QString instName, long address, long irqNumber ) {
		return new ModBbisMscan5200(instName, address, irqNumber);
	}
};

// -----------------------------------------------------------------
//! MEN CPU Em1 (N/A)

class ModCpuEm1 : public ModCpuEsm
{
public:
    ModCpuEm1(  bool withSubDevs );

    // create another instance
    virtual Device *create(bool withSubDevs=true){
        return new ModCpuEm1( withSubDevs );
    }

    Arch getArch() { return Ppc; }
};

class ModCpuEm1N : public ModCpuEm1
{
public:
    ModCpuEm1N(  bool withSubDevs ):
    	ModCpuEm1( withSubDevs )
    {
    	setHwName( "EM1N" );
    	setDescription( "Embedded System Module with MPC5200B" );
    };

    // create another instance
    virtual Device *create(bool withSubDevs=true){
        return new ModCpuEm1N( withSubDevs );
    }
};

class ModCpuEm1A : public ModCpuEm1
{
public:
    ModCpuEm1A(  bool withSubDevs ):
    	ModCpuEm1( withSubDevs )
    {
    	setHwName( "EM1A" );
    	setDescription( "Embedded System Module with MPC5200B" );
    };

    // create another instance
    virtual Device *create(bool withSubDevs=true){
        return new ModCpuEm1A( withSubDevs );
    }
};

// -----------------------------------------------------------------
//! MEN CPU Em2

class ModCpuEm2 : public ModCpuEsm
{
public:
    ModCpuEm2(  bool withSubDevs );

    // create another instance
    virtual Device *create(bool withSubDevs=true){
        return new ModCpuEm2( withSubDevs );
    }

    BusInterface* newEsmBusIf( const Q3MemArray<uchar>* pciBusPath,
                               bool endianSwapping );
    Arch getArch() { return X86; }
};

// -----------------------------------------------------------------
//! MEN CPU Em3

class ModCpuEm3 : public ModCpuEsm
{
public:
    ModCpuEm3(  bool withSubDevs );

    // create another instance
    virtual Device *create(bool withSubDevs=true){
        return new ModCpuEm3( withSubDevs );
    }

    Arch getArch() { return Ppc; }
};

// -----------------------------------------------------------------
//! MEN CPU Em04

class ModCpuEm04 : public ModCpuEsm
{
public:
    ModCpuEm04(  bool withSubDevs );

    // create another instance
    virtual Device *create(bool withSubDevs=true){
        return new ModCpuEm04( withSubDevs );
    }

    Arch getArch() { return Ppc; }
};

// -----------------------------------------------------------------
//! MEN CPU Em5

class ModCpuEm5 : public ModCpuEsm
{
public:
    ModCpuEm5(  bool withSubDevs );

    // create another instance
    virtual Device *create(bool withSubDevs=true){
        return new ModCpuEm5( withSubDevs );
    }

    Arch getArch() { return X86; }
};

// -----------------------------------------------------------------
//! MEN CPU Em7

class ModCpuEm7 : public ModCpuEsm
{
public:
    ModCpuEm7(  bool withSubDevs );

    // create another instance
    virtual Device *create(bool withSubDevs=true){
        return new ModCpuEm7( withSubDevs );
    }

    BusInterface* newEsmBusIf( const Q3MemArray<uchar>* pciBusPath,
                               bool endianSwapping );
    Arch getArch() { return X86; }
};

// -----------------------------------------------------------------
//! MEN CPU Em7N

class ModCpuEm7N : public ModCpuEm7
{
public:
    ModCpuEm7N(  bool withSubDevs );

    // create another instance
    virtual Device *create(bool withSubDevs=true){
        return new ModCpuEm7N( withSubDevs );
    }

    Arch getArch() { return X86; }
};


// -----------------------------------------------------------------
//! MEN CPU Em8

class ModCpuEm8 : public ModCpuEm3
{
public:
    ModCpuEm8(  bool withSubDevs );

    // create another instance
    virtual Device *create(bool withSubDevs=true){
        return new ModCpuEm8( withSubDevs );
    }


};

// -----------------------------------------------------------------
//! MEN CPU EM9

class ModCpuEm9 : public ModCpuEsm
{
public:
    ModCpuEm9(  bool withSubDevs );

    // create another instance
    virtual Device *create(bool withSubDevs=true){
        return new ModCpuEm9( withSubDevs );
    }

    Arch getArch() { return Ppc; }
};

//--------------------------------------------------------------------
// combined CPUs (ESM + Carrier)


// -----------------------------------------------------------------
//! MEN CPU A13 (EM2 + A500)
class ModCpuA13 : public ModCpuEm2
{
public:
    ModCpuA13( ModExtA500::Mezzanines mezzanines, bool withSubDevs );

    // create another instance
    virtual Device *create( bool withSubDevs=true ){
		return( new ModCpuA13( _mezzanines, withSubDevs ) );
	};

	virtual SwModuleList *enumSwModules();
protected:
	ModExtA500::Mezzanines _mezzanines;
};

// -----------------------------------------------------------------
//! MEN CPU A14 (EM8 + A500)
class ModCpuA14 : public ModCpuEm8
{
public:
    ModCpuA14( ModExtA500::Mezzanines mezzanines, bool withSubDevs );

    // create another instance
    virtual Device *create( bool withSubDevs=true ){
		return( new ModCpuA14( _mezzanines, withSubDevs ) );
	};

	virtual SwModuleList *enumSwModules();
protected:
	ModExtA500::Mezzanines _mezzanines;
};

// -----------------------------------------------------------------
//! MEN CPU F11 (EM7 + F502)
class ModCpuF11 : public ModCpuEm7
{
public:
    ModCpuF11( bool withSubDevs );

    // create another instance
    virtual Device *create( bool withSubDevs=true ){
		return( new ModCpuF11( withSubDevs ) );
	};
};

// -----------------------------------------------------------------
//! MEN CPU F11N (EM7N + F502)
class ModCpuF11N : public ModCpuEm7N
{
public:
    ModCpuF11N( bool withSubDevs );

    // create another instance
    virtual Device *create(bool withSubDevs=true){
		return( new ModCpuF11( withSubDevs ) );
	};
};

// -----------------------------------------------------------------
//! MEN CPU F12 (N) (EM1(N) + F500)
class ModCpuF12 : public ModCpuEm1
{
public:
    ModCpuF12(  bool withSubDevs );

    // create another instance
    virtual Device *create(bool withSubDevs=true){
		return( new ModCpuF12( withSubDevs ) );
	};
};

class ModCpuF12N : public ModCpuF12
{
public:
    ModCpuF12N(  bool withSubDevs ) : 
    	ModCpuF12 ( withSubDevs )
    {
    	setHwName( "F12N" );
    	setDescription( "CompactPCI PowerPC MPC5200B CPU" );
    };

    // create another instance
    virtual Device *create(bool withSubDevs=true){
		return( new ModCpuF12N( withSubDevs ) );
	};
};

// -----------------------------------------------------------------
//! MEN CPU F13 (EM8 + F500)
class ModCpuF13 : public ModCpuEm8
{
public:
    ModCpuF13(  bool withSubDevs );

    // create another instance
    virtual Device *create(bool withSubDevs=true){
		return( new ModCpuF13( withSubDevs ) );
	};
};

#endif



