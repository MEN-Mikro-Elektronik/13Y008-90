/***************************************************************************
 */
/*!       \file  modbbispcimmod.h
 *       \brief  BBIS base class for PCI M-Module carriers
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/07/05 18:13:21 $
 *    $Revision: 2.2 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbispcimmod.h,v $
 * Revision 2.2  2010/07/05 18:13:21  CRuff
 * R: implementation of PCI Domains
 * M: added new member pciDomainNo to ModBbisPciMmodProperties class
 *
 * Revision 2.1  2010/05/19 17:21:09  CRuff
 * R: 1. need to use multiple inheritance for ModBbisChamProperties
 * M: 1. make inheritance to DeviceProperties virtual for
 *       ModBbisPciMmodProperties
 *
 * Revision 2.0  2010/01/22 11:29:22  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.7  2006/11/30 11:28:23  cs
 * removed:
 *   - D203 family boards moved to own files
 *
 * Revision 1.6  2004/06/08 17:26:05  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.5  2004/01/09 08:20:50  kp
 * intermediate checkin
 *
 * Revision 1.4  2003/06/11 12:21:53  kp
 * intermediate checkin
 *
 * Revision 1.3  2003/06/05 09:35:14  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/06/03 10:17:55  kp
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:33:17  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODBBISPCIMMOD_H
#define MODBBISPCIMMOD_H

#include "hwcomponent.h"
#include "descriptor.h"


//! Device properties for ModBbisPciMmod (PCI M-Module carriers)
class ModBbisPciMmodProperties : virtual public DeviceProperties
{
public:
	int slotNo;			//!< PCI geographical slot number
	int pciDevNo;		//!< PCI device number
	int pciBusNo;		//!< PCI bus number
	int pciDomainNo; 	//!< PCI domain number; default: 0
};

//! BBIS base class for PCI M-Module carriers
class ModBbisPciMmod : public BbisDevice
{
public:
	ModBbisPciMmod(
		QString instName,
		QString modelName,
		QString driverName,
		BusIfType ifType);

	virtual ~ModBbisPciMmod();

	virtual ApplyResult applyProperties( DeviceProperties *newProp,
										 QString &errMsg );
	virtual ApplyResult applyProperties( DescriptorEntryDirectory *devDesc,
										 QString &errMsg );
	virtual QString getMainProperties();
	void createSpecialDesc( DescriptorEntryDirectory *parentDesc );

	// PCI-Mmod special calls
	void setSlotNo( int slotNo );

protected:
	virtual DevPropertiesDlg *createPropDlg( QWidget *parent );
	virtual DeviceProperties *createProperties();
	virtual void copyProperties( const DeviceProperties *from,
								 DeviceProperties *to );
	virtual void freeResources();

	//! get the number of M-Module slots
	virtual int getSlots() { return 0; }

	virtual void createSlots();
};

//! BBIS class for MEN D201
class ModBbisD201 : public ModBbisPciMmod
{
public:
	ModBbisD201();
	// create another instance
	virtual Device *create(bool withSubDevs=true);
protected:
	int getSlots() { return 4; }

};

//! BBIS class for MEN D302
class ModBbisD302 : public ModBbisPciMmod
{
public:
	ModBbisD302(bool withSubDevs=false);
	// create another instance
	virtual Device *create(bool withSubDevs=true);
protected:
	int getSlots() { return 4; }

};

//! BBIS class for MEN C203
class ModBbisC203 : public ModBbisPciMmod
{
public:
	ModBbisC203();
	// create another instance
	virtual Device *create(bool withSubDevs=true);
	SwModuleList *enumSwModules();
protected:
	int getSlots() { return 3; }

};

//! BBIS class for MEN C204
class ModBbisC204 : public ModBbisPciMmod
{
public:
	ModBbisC204();
	// create another instance
	virtual Device *create(bool withSubDevs=true);
	SwModuleList *enumSwModules();
protected:
	int getSlots() { return 1; }

};

//! BBIS class for MEN F201
class ModBbisF201 : public ModBbisPciMmod
{
public:
	ModBbisF201();
	// create another instance
	virtual Device *create(bool withSubDevs=true);
	SwModuleList *enumSwModules();
protected:
	int getSlots() { return 1; }

};

//! BBIS class for MEN F202
class ModBbisF202 : public ModBbisPciMmod
{
public:
	ModBbisF202();
	// create another instance
	virtual Device *create(bool withSubDevs=true);
	SwModuleList *enumSwModules();
protected:
	int getSlots() { return 2; }

};
#endif









