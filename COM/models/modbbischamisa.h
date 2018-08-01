/***************************************************************************
 */
/*!       \file  modbbischamisa.h
 *       \brief  BBIS class for the CHAMELEON FPGAs (ISA variant)
 *      \author	 Christine.Ruff@men.de
 *		  $Date: 2013/03/04 13:03:01 $
 *    $Revision: 2.3 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbischamisa.h,v $
 * Revision 2.3  2013/03/04 13:03:01  dpfeuffer
 * R: open existing project with SC24 CPU doesn't work
 * M: getUnhandeledDesc: using class ModBbisChamBase instead of class ModBbisCham
 *
 * Revision 2.2  2011/09/26 08:42:54  dpfeuffer
 * R:1. base class for ModBbisCham (PCI) and ModBbisChamIsa required
 *   2. ModBbisChamIsa not full implemented
 * M:1. some member functions removed (now in ModBbisChamBase)
 *   2. ModBbisChamIsa member function added
 *
 * Revision 2.1  2011/02/14 14:56:35  CRuff
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODBBISCHAMISA_H
#define MODBBISCHAMISA_H

#include <Q3ValueList>

#include "hwcomponent.h"
#include "descriptor.h"
#include "util.h"
#include "modbbischam.h"

class ModMdisCham;

//! BBIS class for CHAMELEON FPGA ISA bbis
/*!
  This BbisDevice is attatched to the CpuDevice through LocalBus, not
  through PCI. Therefore, there is no PCI_PATH here, but an absolute
  address which gives the location of the fpga in the system.

  It provides 16 pseudo slots. MDIS devices can be connected to each of
  the slots.

  The MDIS device shall have
  - a "bbslot" specification telling mdiswiz that it can connect to all
    "CHAMELEON" BBIS models
  - a <chamunitid> tag that tells mdiswiz which FPGA module code it needs.

  Based on the connected devices, this class automatically creates
  the DEVICE_ID_x descriptor entries. It assumes that the user knows
  which FPGA units are present in his FPGA.

*/
class ModBbisChamIsa : public ModBbisChamBase
{
	Q_OBJECT
public:
	ModBbisChamIsa( const QString &variant=0 );

	virtual ~ModBbisChamIsa();
    void freeResources();
	Device *create(bool withSubDevs);

    virtual Device::ApplyResult
        applyProperties( DeviceProperties *newProp, QString &errMsg );
	virtual ApplyResult applyProperties( DescriptorEntryDirectory *devDesc,
										 QString &errMsg );
    QString getMainProperties();
	void createSpecialDesc( DescriptorEntryDirectory *parentDesc );
	SwModuleList *enumSwModules();

	QString getDriverName( bool fullName, bool withSw);
    DeviceProperties *createProperties( void );
    void copyProperties(
        const DeviceProperties *from,
        DeviceProperties *to );

	// ISA special calls
	void setIsaAddress( ulong devAddr, bool devAddrIsIo=false );
	void setIrqNumber( long irqNumber );  

	bool chamSmbActive;
	
protected:
	QString variant;

private: 
};

class ModBbisChamIsaProperties : public DeviceProperties
{
public:
	int				deviceAddress;  //!< device address
	int				devAddrIsIo;	//!< flag: is device address IO addr?
	int				irqNum;
	
#ifdef WIN_MDISWIZ_LIB	
	// members for autoenum keys
	int autoenum;
	Q3MemArray <uchar> autoenum_excludingV2;
	Q3MemArray <uchar> autoenum_excluding;
#endif

};

#endif





