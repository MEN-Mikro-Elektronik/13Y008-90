/***************************************************************************
 */
/*!       \file  modcpuesmini.h
 *       \brief  MEN ESMini classes
 *      \author  christine.ruff@men.de
 *        $Date: 2010/06/08 11:19:28 $
 *    $Revision: 2.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpuesmini.h,v $
 * Revision 2.1  2010/06/08 11:19:28  CRuff
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODCPUESMINI_H
#define MODCPUESMINI_H

#include <Q3MemArray>

#include "hwcomponent.h"


// -----------------------------------------------------------------
//! ESM class
class ModCpuEsmini : public CpuDeviceSmb
{
public:
    ModCpuEsmini( QString _hwName, QString _driverName  );
    BusInterface* newEsminiBusIf( const Q3MemArray<uchar>* pciBusPath,
                               bool endianSwapping );
};

// -----------------------------------------------------------------
//! MEN CPU Mm01 (N/A)

class ModCpuMm1 : public ModCpuEsmini
{
public:
    ModCpuMm1(  bool withSubDevs );

    // create another instance
    virtual Device *create(bool withSubDevs=true){
        return new ModCpuMm1( withSubDevs );
    };

    Arch getArch() { return X86; };

};

// -----------------------------------------------------------------
//! MEN CPU MM50 (N/A)

class ModCpuMm50 : public ModCpuEsmini
{
public:
    ModCpuMm50(  bool withSubDevs );

    // create another instance
    virtual Device *create(bool withSubDevs=true){
        return new ModCpuMm50( withSubDevs );
    };

    Arch getArch() { return Ppc; };

};



#endif



