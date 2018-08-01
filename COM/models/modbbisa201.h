/***************************************************************************
 */
/*!       \file  modbbisa201.h
 *       \brief  BBIS class for the A201 family (including B201/B202)
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:28:34 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbisa201.h,v $
 * Revision 2.0  2010/01/22 11:28:34  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.4  2004/10/07 11:13:34  dpfeuffer
 * - modified for windows "Force Found" option
 *
 * Revision 1.3  2004/06/08 17:25:56  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.2  2003/06/11 12:21:49  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:33:08  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:26:21  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:17:46  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODBBISA201_H
#define MODBBISA201_H

#include "hwcomponent.h"
#include "descriptor.h"

class ResourceMng;

//! Device properties for A201 family 
class ModBbisA201Properties : public DeviceProperties
{
public:
	ulong vmeAddr;		//!< VME address of carrier
	bool isA16Space;	//!< true if \a vmeAddr is in VME A16 space
	int irqLevels[4];	//!< VME interrupt level for each slot
	int irqVectors[4];	//!< VME interrupt vector for each slot
#ifdef WIN_MDISWIZ_LIB
	int forceFound[4];	//!< force found for each slot
#endif
};

//! BBIS class for the A201 family (including B201/B202)
class ModBbisA201Family : public BbisDevice
{
public:
	ModBbisA201Family( 
		QString instName, 
		QString hwName,
		QString driverName);

	virtual ~ModBbisA201Family();

	virtual ApplyResult applyProperties( DeviceProperties *newProp, 
										 QString &errMsg );
	virtual ApplyResult applyProperties( DescriptorEntryDirectory *devDesc,
										 QString &errMsg );
	virtual QString getMainProperties(); 
	void createSpecialDesc( DescriptorEntryDirectory *parentDesc );

protected:	
	virtual DevPropertiesDlg *createPropDlg( QWidget *parent );
	virtual DevPropertiesDlg *createPropDlg4Win( QWidget *parent );
	virtual DeviceProperties *createProperties();
	virtual void copyProperties( const DeviceProperties *from,
								 DeviceProperties *to );
	virtual void freeResources();

	//! get the number of M-Module slots
	virtual int getSlots() { return 0; }

	//! get size of board's address space (which is also the required alignement)
	virtual int getVmeSizeAndAlignment() { return 0x800; }
	
	void createSlots();

private:
	ResourceMng *getVmeAddRes( bool _isA16Space );
};

//! BBIS class for the A201
class ModBbisA201 : public ModBbisA201Family
{
public:
	ModBbisA201();	
	// create another instance
	virtual Device *create(bool withSubDevs=true);	
protected:	
	int getSlots() { return 4; }

};

//! BBIS class for the A302
class ModBbisA302 : public ModBbisA201Family
{
public:
	ModBbisA302(bool withSubDevs=false);	
	// create another instance
	virtual Device *create(bool withSubDevs=true);	
	QString getDeviceNameTpl();
protected:	
	int getSlots() { return 4; }

};
//! BBIS class for the B201
class ModBbisB201 : public ModBbisA201Family
{
public:
	ModBbisB201();	
	// create another instance
	virtual Device *create(bool withSubDevs=true);	
	SwModuleList *enumSwModules();
	QString getDeviceNameTpl() { return "B201"; }
protected:	
	int getSlots() { return 1; }
};

//! BBIS class for the B202
class ModBbisB202 : public ModBbisA201Family
{
public:
	ModBbisB202();	
	// create another instance
	virtual Device *create(bool withSubDevs=true);	
	SwModuleList *enumSwModules();
	QString getDeviceNameTpl() { return "B202"; }
protected:	
	int getSlots() { return 2; }
};

#endif


