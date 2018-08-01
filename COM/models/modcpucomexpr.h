/***************************************************************************
 */
/*!       \file  modcpucomexpr.h
 *       \brief  MEN COM Express CPUs classes
 *      \author  dieter.pfeuffer@men.de
 *        $Date: 2016/02/20 16:26:54 $
 *    $Revision: 2.3 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpucomexpr.h,v $
 * Revision 2.3  2016/02/20 16:26:54  ts
 * R: BoxPCs based on SC24 need 2 SMBusses in MDIS wizard
 * M: 1. added member nrSmbBusses to class CpuDeviceSmb
 *    2. added parameter int nrSmbBusses to every constructor of CPUs with SMB
 *       controllers: 2 for SC24 (AMD based) Box PCs, 1 for every other CPU
 *
 * Revision 2.2  2014/08/22 15:57:04  dpfeuffer
 * R: COM Express support not finished
 * M: COM Express support completed
 *
 * Revision 2.1  2014/07/18 15:12:34  dpfeuffer
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2014 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODCPUCOMEXPR_H
#define MODCPUCOMEXPR_H

#include <Q3MemArray>

#include "hwcomponent.h"


// -----------------------------------------------------------------
//! ESM class
class ModCpuComExpr : public CpuDeviceSmb
{
public:
	ModCpuComExpr( QString _hwName, QString _driverName  )
		: CpuDeviceSmb( _hwName, _driverName, false, false, 1 ){}
    BusInterface* newComExprBusIf( const Q3MemArray<uchar>* pciBusPath,
                               bool endianSwapping );
};

// -----------------------------------------------------------------
//! MEN CPU CB70C

class ModCpuCb70c : public ModCpuComExpr
{
public:
    ModCpuCb70c(  bool withSubDevs );

    // create another instance
    virtual Device *create(bool withSubDevs=true){
        return new ModCpuCb70c( withSubDevs );
    };

    Arch getArch() { return X86; };
};

// -----------------------------------------------------------------
//! MEN CPU CB30C

class ModCpuCb30c : public ModCpuComExpr
{
public:
    ModCpuCb30c(  bool withSubDevs );

    // create another instance
    virtual Device *create(bool withSubDevs=true){
        return new ModCpuCb30c( withSubDevs );
    };

    Arch getArch() { return Ppc; };
};

// -----------------------------------------------------------------
//! MEN CPU CC10

class ModCpuCc10 : public ModCpuComExpr
{
public:
	enum SubModel { CC10, CC10C };
    ModCpuCc10( SubModel submod, bool withSubDevs );

    // create another instance
    virtual Device *create(bool withSubDevs=true){
        return new ModCpuCc10( _submod, withSubDevs );
    };

    Arch getArch() { return Arm; };

private:
	SubModel _submod;
};

#endif



