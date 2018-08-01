/***************************************************************************
 */
/*!       \file  modbbisa203n.h
 *       \brief  BBIS class for the A203N family
 *      \author	 dieter.pfeuffer@men.de
 *		  $Date: 2010/01/22 11:28:43 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbisa203n.h,v $
 * Revision 2.0  2010/01/22 11:28:43  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.2  2009/08/21 14:48:16  CRuff
 * R: no public possibility to get number of available slots
 * M: added public access method numSlots()
 *
 * Revision 1.1  2005/08/02 11:20:43  dpfeuffer
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODBBISA203N_H
#define MODBBISA203N_H

#include "hwcomponent.h"
#include "descriptor.h"

class ResourceMng;

//! Device properties for A203N family 
class ModBbisA203NProperties : public DeviceProperties
{
public:
	ulong vmeAddr;		//!< VME A16/A24 address of carrier
	ulong vmeA32Addr;	//!< VME A32 address of carrier
	bool isA16Space;	//!< true if \a vmeAddr is in VME A16 space
	bool a32Enabled;	//!< true if VME A32 space enabled
	int irqLevels[4];	//!< VME interrupt level for each slot
	int irqVectors[4];	//!< VME interrupt vector for each slot
#ifdef WIN_MDISWIZ_LIB
	int forceFound[4];	//!< force found for each slot
#endif
};

//! BBIS class for the A203N family
class ModBbisA203NFamily : public BbisDevice
{
public:
	ModBbisA203NFamily( 
		QString instName, 
		QString hwName,
		QString driverName);

	virtual ~ModBbisA203NFamily();

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

//! BBIS class for the A203N
class ModBbisA203N : public ModBbisA203NFamily
{
public:
	ModBbisA203N();	
	// create another instance
	virtual Device *create(bool withSubDevs=true);	
	int numSlots() { return getSlots(); }
protected:	
	int getSlots() { return 4; }

};

#endif


