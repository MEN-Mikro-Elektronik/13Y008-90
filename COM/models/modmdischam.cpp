/***************************************************************************
 */
/*!       \file  modmdischam.cpp
 *       \brief  Implementation of chameleon MDIS devices
 *      \author	 Christine.Ruff@men.de
 *		  $Date: 2013/03/04 13:03:06 $
 *    $Revision: 2.4 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modmdischam.cpp,v $
 * Revision 2.4  2013/03/04 13:03:06  dpfeuffer
 * R: open existing project with SC24 CPU doesn't work
 * M: using class ModBbisChamBase instead of class ModBbisCham
 *
 * Revision 2.3  2010/06/21 16:55:04  CRuff
 * R: version 2.2 has errors
 * M: undo changes of version 2.2; does not work in all cases
 *
 * Revision 2.2  2010/05/26 09:05:25  CRuff
 * R: casting error if LL device has no autoid specified
 * M: in ModMdisCham::ModMdisCham, only extract device ID if autoId is specified
 *
 * Revision 2.1  2010/05/25 10:50:56  CRuff
 * R: hide descriptor properties tab if all descriptor keys are hidden
 * M: in createPropDlg(): only show descriptor tab if device has visible keys
 *
 * Revision 2.0  2010/01/22 11:31:12  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.4  2009/12/07 13:26:06  dpfeuffer
 * R: Windows mdiswiz_lib: device manager access violation
 * M: ModMdisCham::applyProperties: ifdef WIN_MDISWIZ_LIB added
 *
 * Revision 1.3  2009/05/11 16:21:39  CRuff
 * R: 1. Device identifications in groups done with index, not instance number
 * M: 1a) ModMdisCham::ModMdisCham(): generate index of group members
 *       (in order of occurrence)
 *    1b) ModMdisCham::applyProperties(): Instance nr is not read for group
 *        members
 *    1c) ModMdisCham::copyProperties(): also copy the index member
 *
 * Revision 1.2  2009/04/21 15:27:45  CRuff
 * R: Cosmetics
 * M: added Revision history macro in file header
 *
 * Revision 1.1  2009/04/01 15:23:20  CRuff
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#include "modmdischam.h"
#include "modmdischamproptab.h"
#include "hwcomponent.h"
#include <qstring.h>
#include "wizdebug.h"
#include "wizexcept.h"
#include "modbbischam.h"
#include "vmsgbox.h"

extern QWidget *G_mainWindow;

ModMdisCham::ModMdisCham( QString hwName,
				 QString driverName,
				 bool isRemoveable,
				 BusIfType ifType,
				 DeviceAutoId* autoId)
				: ModMdisGeneric (hwName, driverName, isRemoveable, ifType)
{
	wDebug(("ModMdisCham::ModMdisCham"));

	if( prop ) delete prop;	// delete Device::prop
	prop = createProperties(); // create default props

	ModMdisChamProperties* myProp;
	WIZ_DYNAMIC_CAST( prop, myProp, ModMdisChamProperties* );

	DeviceChamAutoId* chamAutoId;
	WIZ_DYNAMIC_CAST( autoId, chamAutoId, DeviceChamAutoId* );
	// set the device id
	myProp->deviceIdent.devId = chamAutoId->deviceId;

	if (chamAutoId->getType() == DeviceAutoId::AutoIdChamV2)
	{
		DeviceChamV2AutoId* chamV2AutoId;
		WIZ_DYNAMIC_CAST( chamAutoId, chamV2AutoId, DeviceChamV2AutoId* );

		// copy the group members if necessary
		if ( chamV2AutoId->chamv2grpList )
		{
			// remember the number of occurrences of each device in the group
			QMap<ulong,int> map_currIndex;
			myProp->deviceIdent.index = 0; // main device in group has index 0
			map_currIndex.insert(myProp->deviceIdent.devId, 1);

			// for each group member, remember device id and index
			for (int i=0; i< chamV2AutoId->chamv2grpList->size(); i++)
			{
				// get device id
				ulong grpMemId = *(chamV2AutoId->chamv2grpList->at(i));
				DeviceChamIdent newDevIdent;
				newDevIdent.devId = grpMemId;

				// determine index for this device
				int ind = 0;
				if ( map_currIndex.contains(newDevIdent.devId) )
				{
					ind = map_currIndex[newDevIdent.devId];
				}
				newDevIdent.index = ind;
				map_currIndex[newDevIdent.devId] = ind + 1;

				// remember this device
				myProp->grpMemList->push_back(newDevIdent);
			}
		}
	}
	else
	{
		// correct the chameleon type if necessary
		if ( autoId->getType() == DeviceAutoId::AutoIdCham )
		myProp->devType = ModMdisChamProperties::ChamOriginal;
	}

}


//! create the properties
/*!
  This creates the properties of the mdis chameleon device
*/
DeviceProperties *
ModMdisCham::createProperties()
{
	wDebug(("create ModMdisChamProperties()"));
	ModMdisChamProperties *p = new ModMdisChamProperties();
	return p;
}

//! create a new instance of this device class (with properties of template)
/*!
  \remark It is assumed that this method is called only on the template
  device (device that is in the ModelFactory)
*/
Device *
ModMdisCham::create(bool withSubDevs)
{
	ModMdisCham *newDev;
	withSubDevs = withSubDevs;

	newDev = new ModMdisCham( getModelName(), driverName, removeable, ifType, autoId() );
	newDev->copy( this );
	newDev->setDescriptor( &((ModDescriptorProperties *)prop)->desc );
	newDev->setDebugSettingMods( prop->dbgSettings.getModules());
	newDev->setDescription( description );

	if( !devNameTpl.isEmpty() )
		newDev->setDeviceNameTpl( devNameTpl );

	newDev->setSubDevOffTbl( subDevOffTbl );

	if( subDevOffTbl.count() > 0 )
		newDev->setSubDevOff( subDevOffTbl[0] );

	/*ModMdisChamProperties* mdisProp, *newMdisProp;
	WIZ_DYNAMIC_CAST( prop, mdisProp, ModMdisChamProperties* );
	WIZ_DYNAMIC_CAST( newDev->prop, newMdisProp, ModMdisChamProperties* );*/

	//copyProperties(mdisProp, newMdisProp);
	//newMdisProp->deviceIdent.devId = mdisProp->deviceIdent.devId;
	//newMdisProp->deviceIdent.instanceNr = mdisProp->deviceIdent.instanceNr;

	return newDev;
}

DevPropertiesDlg*
ModMdisCham::createPropDlg( QWidget *parent )
{
	DevPropertiesDlg *dp;
	QWidget *defTab;
	ModMdisChamPropTab* chamTab;
	ModDescriptorProperties *p;

	WIZ_DYNAMIC_CAST( prop, p, ModDescriptorProperties * );

	dp = new DevPropertiesDlg( parent, this );
	if (p->desc.hasVisibleChildren()) {
		dp->addTab( (defTab = new DevGenericPropTab(parent, dp)), "Descriptor" );
	}

	wDebug(("add cham prop tab"));
	dp->addTab( (chamTab = new ModMdisChamPropTab(parent, dp)), "Chameleon" );
	dp->showPage( chamTab );

	dp->setButtons();

	//dp->setWidth(dp->tabBar()->width() + 20);

	return dp;
}

Device::ApplyResult
ModMdisCham::applyProperties( DescriptorEntryDirectory *devDesc, QString &errMsg )
{
	bool isChamV2Group = false;

	ModMdisChamProperties* _prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModMdisChamProperties* );

	wDebug(("enter ModMdisCham::applyProperties "));

#ifdef WIN_MDISWIZ_LIB
	UTIL_UNREF_PARAM(isChamV2Group);
#else
	DescriptorEntry *e, *eNext;

	// get bbis parent
	HwComponent* parent = this->getParent()->getParent();
	ModBbisChamBase* bbisDev;
	WIZ_DYNAMIC_CAST(parent, bbisDev, ModBbisChamBase* );

	// check if the mdis device represents a chameleon group
	if ( ( e = devDesc->find( "GROUP_ID", DescriptorEntry::Uint32 ) ) ) {
		isChamV2Group = true;
		DescriptorEntryUint32* eUint32;
		WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
		ulong grpId = eUint32->getValue();

		// set group id
		if ( bbisDev->addGroup( grpId ) )
		{
    		_prop->grpId = grpId;
		}
		else
		{
			errMsg += QString("Could not add group '%1' with id %2\n").arg(devDesc->getKey()).arg(grpId);
			errMsg += "A group with this id already exists for this device\n\n";

			_prop->grpId = grpId; // leave value although duplicate to avoid removing of correct values
		}

    	e->getParent()->removeChild(e);
	}

	// delete all empty directories in passed descriptor
	eNext=devDesc->firstChild();
	bool foundDevId = false;
	while( (e=eNext) ){
		eNext=e->nextSibling();

		QString key = e->getKey();
		if ( key.contains("DEVICE_ID_") ||
			 key.contains("DEVICE_IDV2_") )
		{
			wDebug(("type: %u", e->getType()));
			foundDevId = true;

			if (e->getType() == DescriptorEntry::Uint32 )
			{
				DescriptorEntryUint32 *e32;
				WIZ_DYNAMIC_CAST( e, e32, DescriptorEntryUint32* );
				ulong descValue = e32->getValue();
				wDebug((" read value for key %s: %lx", key.ascii(), descValue));

				// last 8 bit represent the instance number
				int instNr = descValue & 0x000000ff;
				// get device id
				uint devId = descValue & 0x0000ff00;
				devId = devId >> 8;

				wDebug((" set instance nr for key %s: %i", key.ascii(), instNr));

				// check if this is a valid instance nr for this device
				if ( !isChamV2Group )
				{
					if ( !bbisDev->addInstance( devId, instNr ) )
					{
						errMsg += QString("Could not add device %1 with instance number %2\n").arg(devId).arg(instNr);
						errMsg += "A device with this instance number already exists in this chameleon device\n\n";

						// leave value as is to avoid deleting of correct values
					}
				}

				if ( !isChamV2Group )  // in groups, index is generated automatically!
				{
					_prop->deviceIdent.instanceNr = instNr;
				}
			}
			else {
				errMsg += QString("Instance number entry %1 has wrong type; need U_INT32 here!\n\n").arg(key);
			}

			e->getParent()->removeChild(e);
		}
	}

	if ( !foundDevId )
	{
		errMsg += QString("ERROR: no chameleon properties specified for this device. \n");
		errMsg += QString("Please specify chameleon properties, or the device cannot be used properly.\n\n");
	}
	wDebug(("leave ModMdisCham::applyProperties "));

#endif // WIN_MDISWIZ_LIB

	return ModMdisGeneric::applyProperties( devDesc, errMsg );
}

void
ModMdisCham::showWarningMsg( QString &errMsg )
{
	VMsgBox::informationOk( "Device Properties",
							"Error while reading chameleon device properties",
							errMsg,
							G_mainWindow );
}


void
ModMdisCham::copyProperties(
	const DeviceProperties *from,
	DeviceProperties *to )
{
	wDebug(("ModMdisCham::copyProperties"));
	const ModMdisChamProperties *_from;
	ModMdisChamProperties *_to;

	WIZ_DYNAMIC_CAST( to, _to, ModMdisChamProperties * );
	WIZ_DYNAMIC_CAST( from, _from, const ModMdisChamProperties * );

	Device::copyProperties( from, to );

	// copy descriptor
	_to->desc.take( (DescriptorEntryDirectory *)&_from->desc );

	// copy cham properties
	_to->deviceIdent.devId = _from->deviceIdent.devId;
	_to->deviceIdent.instanceNr = _from->deviceIdent.instanceNr;
	_to->deviceIdent.index = _from->deviceIdent.index;

	_to->grpId = _from->grpId;

	_to->grpMemList->clear();
	for ( int i=0; i<_from->grpMemList->size(); i++ )
	{
		DeviceChamIdent newDevIdent;
		DeviceChamIdent oldDevIdent = *(_from->grpMemList->at(i));
		newDevIdent.devId = oldDevIdent.devId;
		newDevIdent.instanceNr = oldDevIdent.instanceNr;
		newDevIdent.index = oldDevIdent.index;
		_to->grpMemList->push_back(newDevIdent);
	}


}



