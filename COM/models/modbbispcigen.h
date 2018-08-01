/***************************************************************************
 */
/*!       \file  modbbispcigen.h
 *       \brief  BBIS class for PCI PC-MIP/PMC/XMC carriers (F203 etc..)
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2014/08/22 15:56:24 $
 *    $Revision: 2.8 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbispcigen.h,v $
 * Revision 2.8  2014/08/22 15:56:24  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.7  2014/07/18 15:12:15  dpfeuffer
 * R: erroneous PMC support, missing ComExpress support, general maintenance
 * M: intermediate check-in during development
 *
 * Revision 2.6  2013/11/27 10:44:18  ts
 * R: F213 was unavailable in the Wizard
 * M: added F213
 *
 * Revision 2.5  2012/12/20 13:27:15  ts
 * R: 1. new cPCI card F223 was introduced
 *    2. for F223 enumerate of PCI bridges must stop after 1st bridge
 * M: 1. added model for F223 derived from ModBbisPciGen
 *    2. introduced new key SKIP_DEV_BRIDGE_CHECK
 *
 * Revision 2.4  2011/09/26 08:42:59  dpfeuffer
 * R: new HW F207
 * M: ModBbisF207 added
 *
 * Revision 2.3  2010/07/05 18:06:41  CRuff
 * R: implementation of PCI Domains
 * M: added new member pciDomainNo to ModBbisPciGenProperties class
 *
 * Revision 2.2  2010/05/31 09:01:52  CRuff
 * R: 1. no object hierarchy available if MDIS Wizard is used as Lib
 *    2. need to implement the default constructor that the matching properties
 *       are created
 * M: 1. remember the PCI Bus Path in properties if MDIS Wizard is used as
 *       library
 *    2. remove empty implementation of the default constructor; is implemented
 *       in modbbispcigen.cpp
 *
 * Revision 2.1  2010/05/21 15:50:22  CRuff
 * R: 1. model OBPCIGEN needs to be exported for Windows registry handling
 * M: 1a) added default constructor to class ModBbisPciGen
 *    1b) added members to hold descriptor keys DEVICE_SLOT_x, FUNCTION_SLOT_x,
 *        DEVNAME_SLOT_x to class ModBbisPciGenProperties
 *
 * Revision 2.0  2010/01/22 11:29:12  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.4  2006/11/30 11:28:19  cs
 * cosmetics
 *
 * Revision 1.3  2005/03/09 13:49:32  ub
 * cosmetics
 *
 * Revision 1.2  2003/06/03 10:17:51  kp
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:33:13  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODBBISPCIGEN_H
#define MODBBISPCIGEN_H

#include "hwcomponent.h"
#include "descriptor.h"

//! Device properties for ModBbisPciGen (PCI PC-MIP/PMC/XMC carriers)
class ModBbisPciGenProperties : public DeviceProperties
{
public:
	int slotNo;			 //!< PCI geographical slot number
	int pciDevNo;		 //!< PCI device number
	int pciBusNo;		 //!< PCI bus number of PCI bus on carrier
	int pciDomainNo;     //!< PCI domain number
	int skipDevBridgeChk;//!< PCI bridge iteration skipping
#ifdef WIN_MDISWIZ_LIB
	Q3MemArray<uchar> 	pciBusPath; //!< PCI bus path
#endif
	
	// hidden
	Q3MemArray<int> bbSlotDevNbrList;
	Q3MemArray<int> bbSlotFuncNbrList;
	QStringList bbSlotDevNameList;
	
};

//! BBIS class for PCI PC-MIP/PMC/XMC carriers (F203 etc..)
class ModBbisPciGen : public BbisDevice
{
public:
	ModBbisPciGen(
		QString instName,
		QString hwName,
		QString driverName,
		BusIfType ifType);
		
	ModBbisPciGen();
	 

	virtual ~ModBbisPciGen();

	virtual ApplyResult applyProperties( DeviceProperties *newProp,
										 QString &errMsg );
	virtual ApplyResult applyProperties( DescriptorEntryDirectory *devDesc,
										 QString &errMsg );
	virtual QString getMainProperties();
	void createSpecialDesc( DescriptorEntryDirectory *parentDesc );
	SwModuleList *enumSwModules();
	virtual QString getDriverName( bool fullName=false, bool withSw=true);

protected:
	virtual DevPropertiesDlg *createPropDlg( QWidget *parent );
	virtual DeviceProperties *createProperties();
	virtual DeviceProperties *createProperties(int isF223);
	virtual void copyProperties( const DeviceProperties *from,
								 DeviceProperties *to );
	virtual void freeResources();

	void createSlot(int slotNo, int pciDevNo, BusIfType slotIfType);
	void createSlot(int slotNo, BusIfType slotIfType);

};


//! BBIS class for MEN D202
class ModBbisD202 : public ModBbisPciGen
{
public:
	ModBbisD202();
	// create another instance
	virtual Device *create(bool withSubDevs=true);
	virtual QString getDeviceNameTpl(){ return "D202";}
};

//! BBIS class for MEN F203
class ModBbisF203 : public ModBbisPciGen
{
public:
	ModBbisF203();
	// create another instance
	virtual Device *create(bool withSubDevs=true);
	virtual QString getDeviceNameTpl(){ return "F203";}
};

//! BBIS class for MEN F207
class ModBbisF207 : public ModBbisPciGen
{
public:
	ModBbisF207();
	// create another instance
	virtual Device *create(bool withSubDevs=true);
	virtual QString getDeviceNameTpl(){ return "F207";}
};

//! BBIS class for MEN F223
class ModBbisF223 : public ModBbisPciGen
{
public:
	ModBbisF223();
	// create another instance
	virtual Device *create(bool withSubDevs=true);
	virtual QString getDeviceNameTpl(){ return "F223";}
};

//! BBIS class for generic mezzanine PCI device
class ModBbisPci4Mezz : public ModBbisPciGen
{
public:
	ModBbisPci4Mezz(bool withSubDevs=true );
	virtual Device *create(bool withSubDevs=true) { 
		withSubDevs = withSubDevs;
		return new ModBbisPci4Mezz();	
	}
	virtual QString getDeviceNameTpl(){ return "MEZZ_PCI";}
protected:
	//void createSlot(int slotNo);

};

#endif

