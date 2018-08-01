/***************************************************************************
 */
/*!       \file  modmdischam.h
 *       \brief  Representation chameleon mdis devices
 *      \author	 Christine.Ruff@men.de
 *		  $Date: 2010/01/22 11:31:14 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
 /*-------------------------------[ History ]---------------------------------
 *
 * $Log: modmdischam.h,v $
 * Revision 2.0  2010/01/22 11:31:14  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.4  2009/05/13 13:14:13  CRuff
 * R: Native drivers were not handled correctly, which caused application crash
 * M: added method type() to distinguish between MdisGeneric devices and MdisCham   devices
 *
 * Revision 1.3  2009/05/11 16:23:12  CRuff
 * R: 1. Device identifications in groups done with index, not instance number
 *    2. Cosmetics
 * M: 1a) added member 'index' in class DeviceChamIdent
 *    2a) removed some comments
 *
 * Revision 1.2  2009/04/21 15:28:41  CRuff
 * R: Cosmetics
 * M: added file header
 *
 * Revision 1.1  2009/04/01 15:49:37  cruff
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#ifndef MODMDISCHAM_H_
#define MODMDISCHAM_H_

#include <Q3ValueList>

#include "models/modmdisgeneric.h"
#include "devgenericprop.h"
#include "hwcomponent.h"
#include "descriptor.h"

#include <qstring.h>

class DeviceAutoId;

//! MDIS class for chameleon MDIS devices
/*!
  This class can be used as the base class for chameleon MDIS devices.
  All properties are kept in ModMdisChamProperties.

*/
class ModMdisCham : public ModMdisGeneric
{
public:
	ModMdisCham( QString hwName,
				 QString driverName,
				 bool isRemoveable,
				 BusIfType ifType,
				 DeviceAutoId* autoId);
				

	virtual ~ModMdisCham() {};
	
	virtual Type type() { return Device::MdisCham; }
	virtual ApplyResult applyProperties( DescriptorEntryDirectory *devDesc, QString &errMsg );
	virtual Device* create(bool withSubDevs);

protected:

	virtual DevPropertiesDlg *createPropDlg( QWidget *parent );
	virtual DeviceProperties *createProperties();
	virtual void copyProperties( const DeviceProperties *from,
								 DeviceProperties *to );
	
	void showWarningMsg( QString &errMsg );
};

class DeviceChamIdent
{
public:

	//! Constructor
	DeviceChamIdent( ulong _devId = 0, int _instNr = -1, int _index = -1)
	{
		devId = _devId;
		instanceNr = _instNr;
		index = _index;
	}

	//! LSB: chameleon unit module code
	ulong devId;
	//! Instance Nr for this device
	int instanceNr;
	//! Index of this device in group
	//! (only relevant for group devices)
	int index;
};

//! Device properties for ModMdisCham
class ModMdisChamProperties : public ModDescriptorProperties
{
public:
	enum ModMdisChamType {
		ChamOriginal,
		ChamV2
	};
	
	ModMdisChamProperties( ulong deviceId = 0, 
						   int instNr = -1, 
						   ModMdisChamType type = ModMdisChamProperties::ChamV2 ) {
		// init members				   	
		deviceIdent.devId = deviceId;
		deviceIdent.instanceNr = instNr;
		devType = type;
		grpId = 0;
		grpMemList = new Q3ValueList<DeviceChamIdent>();
	};
	
	~ModMdisChamProperties()
	{
		if (grpMemList)
		{
			grpMemList->clear();
			delete grpMemList; 
			grpMemList = NULL;
		}
	}
						   
	ModMdisChamType getType() { return devType; };
	ulong getGroupId() { return grpId; }
	DeviceChamIdent getDeviceIdent() { return deviceIdent; }
	Q3ValueList<DeviceChamIdent>* getGrpList() { return grpMemList; }
	
	//! unique identifier for this device (with id and instance nr)
	DeviceChamIdent deviceIdent;
	
	//! chameleon type of this device (original or chamV2)
	ModMdisChamType devType;
	
	//! Group Identifier (if applicable); default value is 0
	ulong grpId; 
	
	//! List of attached group devices (if applicable)
	Q3ValueList<DeviceChamIdent>* grpMemList;

};

#endif /*MODMDISCHAM_H_*/

