/***************************************************************************
 */
/*!       \file  modcpuesmexpr.h
 *       \brief  MEN ESM express classes
 *      \author  christine.ruff@men.de
 *        $Date: 2011/09/28 09:07:24 $
 *    $Revision: 2.4 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuesmexpr.h,v $
 * Revision 2.4  2011/09/28 09:07:24  dpfeuffer
 * R: wrong CPU board name F50
 * M: F50 replaced by F50P
 *
 * Revision 2.3  2011/04/01 09:41:33  CRuff
 * R: support of pci domains
 * M: introduce new parameter pciDomain for method ModCpuEsmExpr::newEsmExprBusIf
 *
 * Revision 2.2  2010/06/21 10:58:06  CRuff
 * R: unexpected behaviour for smb controller handling
 * M: inherit from ModCpuEsmExpr directly instead of inheriting XM50 class
 *
 * Revision 2.1  2010/06/08 11:19:25  CRuff
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODCPUESMEXPR_H
#define MODCPUESMEXPR_H

#include <Q3MemArray>

#include "hwcomponent.h"


// -----------------------------------------------------------------
//! ESM class
class ModCpuEsmExpr : public CpuDeviceSmb
{
public:
    ModCpuEsmExpr( QString _hwName, QString _driverName  );
    BusInterface* newEsmExprBusIf( const Q3MemArray<uchar>* pciBusPath,
                               bool endianSwapping, int pciDomain=0 );
};

// -----------------------------------------------------------------
//! MEN CPU Xm50 (N/A)

class ModCpuXm50 : public ModCpuEsmExpr
{
public:
    ModCpuXm50(  bool withSubDevs );

    // create another instance
    virtual Device *create(bool withSubDevs=true){
        return new ModCpuXm50( withSubDevs );
    };

    Arch getArch() { return Ppc; };

};

// -----------------------------------------------------------------
//! MEN CPU XM01 (N/A)

class ModCpuXm1 : public ModCpuEsmExpr
{
public:
    ModCpuXm1(  bool withSubDevs );

    // create another instance
    virtual Device *create(bool withSubDevs=true){
        return new ModCpuXm1( withSubDevs );
    };

    Arch getArch() { return Ppc; };

};

// -----------------------------------------------------------------
//! MEN CPU XM02 (N/A)
class ModCpuXm2 : public ModCpuEsmExpr
{
public:
    ModCpuXm2(  bool withSubDevs );

    // create another instance
    virtual Device *create(bool withSubDevs=true){
        return new ModCpuXm2( withSubDevs );
    };

    Arch getArch() { return Ppc; };

};

// -----------------------------------------------------------------
//! MEN CPU F50P (N/A)

class ModCpuF50P : public ModCpuEsmExpr
{
public:
	ModCpuF50P( bool withSubDevs );
	
    // create another instance
    virtual Device *create(bool withSubDevs=true){
        return new ModCpuF50P( withSubDevs );
    };	
    
    Arch getArch() { return Ppc; };
};

#endif




