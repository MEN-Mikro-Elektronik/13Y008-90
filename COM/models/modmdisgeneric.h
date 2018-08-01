/***************************************************************************
 */
/*!       \file  modmdisgeneric.h
 *       \brief  MDIS base class for typical MDIS devices
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:31:09 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modmdisgeneric.h,v $
 * Revision 2.0  2010/01/22 11:31:09  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.4  2004/10/07 11:13:43  dpfeuffer
 * cosmetics
 *
 * Revision 1.3  2004/06/08 17:27:05  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.2  2003/06/03 10:18:16  kp
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:33:35  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODMDISGENERIC_H
#define MODMDISGENERIC_H

#include <q3memarray.h>

#include <Q3MemArray>

#include "hwcomponent.h"
#include "descriptor.h"

class ResourceMng;

//! Device properties for ModMdisGeneric
class ModDescriptorProperties : public DeviceProperties
{
public:
	//! create ModDescriptorProperties
	ModDescriptorProperties() : desc(QString("_ROOT_")) {}
	//! the descriptor
	DescriptorEntryDirectory desc;
};

//! MDIS base class for typical MDIS devices
/*!
  This class can be used as the base class for any MDIS device.
  All properties are kept into the descriptor of ModDescriptorProperties.
  
  It has also a special feature for devices implementing several sub devices.
  See numSubDevs()
*/
class ModMdisGeneric : public MdisDevice
{
public:
	ModMdisGeneric( 
		QString hwName,
		QString driverName,
		bool isRemoveable,
		BusIfType ifType);

	virtual ~ModMdisGeneric();
	void createSpecialDesc( DescriptorEntryDirectory *parentDesc );
	SwModuleList *enumSwModules();
	virtual ApplyResult applyProperties( DescriptorEntryDirectory *devDesc,
										 QString &errMsg );
	virtual QString getMainProperties(); // get main properties in a string
	virtual QString getDriverName( bool fullName=false, bool withSw=true); 

	// for multiple sub-devices
	int numSubDevs() { 
		return subDevOffTbl.count() == 0 ? 1 : subDevOffTbl.count(); 
	}

	//! set the subdevice offset of that device
	/*!
	  usually called from createSubDev()
	*/
	void setSubDevOff( int _subDevOff ) { subDevOff = _subDevOff; }
	virtual Device *createSubDev( int subDevNo );

	void setDescriptor( DescriptorEntryDirectory *desc );
	virtual Device *create(bool withSubDevs=true);	
		
	//! set device name template
	/*!
	  if this is not called, getDeviceNameTpl() returns getDriverName()
	*/
	void setDeviceNameTpl( const QString &tpl ){
		devNameTpl = tpl;
	}

	virtual QString getDeviceNameTpl(){
		return !devNameTpl.isEmpty() ? devNameTpl : getDriverName( false, false );
	}
	
	//! sets the subdevice offset table
	void setSubDevOffTbl( const Q3MemArray<int> &tbl ){
	 	subDevOffTbl.duplicate( tbl );
	}

	void setDebugSettingMods( ulong modules );

protected:	
	virtual DevPropertiesDlg *createPropDlg( QWidget *parent );
	virtual DevPropertiesDlg *createPropDlg4Win( QWidget *parent );
	virtual DeviceProperties *createProperties();
	virtual void copyProperties( const DeviceProperties *from,
								 DeviceProperties *to );
	Q3MemArray<int> subDevOffTbl; //!< offsets for all devices of that model
	int subDevOff;	//!< subdevice offset of that device (-1 for none)
	QString devNameTpl; //!< device name template
	bool swModulesBuilt; //!< flags lstSwModules has been built correctly
};
#endif



