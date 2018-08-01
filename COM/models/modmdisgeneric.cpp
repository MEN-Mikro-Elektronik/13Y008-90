/***************************************************************************
 */
/*!       \file  modmdisgeneric.cpp 
 *       \brief  Implementation of generic MDIS devices
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/05/25 10:55:13 $
 *    $Revision: 2.1 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modmdisgeneric.cpp,v $
 * Revision 2.1  2010/05/25 10:55:13  CRuff
 * R: hide descriptor properties tab if all descriptor keys are hidden
 * M: in createPropDlg() and createPropDlg4Win(): only show descriptor tab if
 *    device has visible keys
 *
 * Revision 2.0  2010/01/22 11:31:07  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.10  2004/10/07 11:13:41  dpfeuffer
 * cosmetics
 *
 * Revision 1.9  2004/08/24 09:45:35  kp
 * bug fix: Exception/crash happened when trying to connect a non-mdis device
 * (M77) to a byteswapped slot.
 *
 * Revision 1.8  2004/08/23 16:17:45  kp
 * fixed crash in enumSwModules when trying to create a non-MDIS device on
 * a swapped slot.
 *
 * Revision 1.7  2004/07/21 14:59:05  ufranke
 * changed
 *  - currentCfg() by MAIN_GetCurrentCfg()
 *
 * Revision 1.6  2004/06/08 17:27:02  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.5  2004/01/09 08:21:02  kp
 * intermediate checkin
 *
 * Revision 1.4  2003/06/05 11:59:42  dschmidt
 * intermediate checkin
 *
 * Revision 1.3  2003/06/05 09:35:33  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/06/03 10:18:14  kp
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:33:34  dschmidt
 * Initial Revision
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <qregexp.h>
#include <qcursor.h>
#include "wizdebug.h"
#include "modmdisgeneric.h"
#include "devgenericprop.h"
#include "wizexcept.h"
#include "swmodule.h"

#include "targetos.h"
#include "configuration.h"
// -----------------------------------------------------------------

//! create a generic MDIS device
/*!
  The device will have no properties. The descriptor has to be created
  by the subclasses.
  
  It creates a single device without subdevices. To create a device
  with subdevices, subclass must call setNumSubDevs() and setSubDevOff().

  \param hwName	Hardware name (e.g. "MEN D201")
  \param driverName (e.g. "d201")
  \param isRemoveable flags if the device can be removed from its carrier
  \param ifType upper interface type (e.g. Mmod)
*/
ModMdisGeneric::ModMdisGeneric( 
	QString hwName, 
	QString driverName, 
	bool isRemoveable, 
	BusIfType ifType) :
	MdisDevice( hwName, driverName, isRemoveable, ifType ) 
{
	if( prop ) delete prop;	// delete Device::prop
	prop = createProperties();
	subDevOff = -1;
	devNameTpl = QString();
	swModulesBuilt = false;
}

//! destructs ModMdisGeneric
ModMdisGeneric::~ModMdisGeneric()
{
	freeResources();
	delete prop;
	prop = 0;
}

DeviceProperties *
ModMdisGeneric::createProperties()
{
	ModDescriptorProperties *p = new ModDescriptorProperties();
	return p;
}

//! set the descriptor from outside (packagefile)
/*!
  This creates a copy of each descriptor entry
*/
void 
ModMdisGeneric::setDescriptor( DescriptorEntryDirectory *desc )
{
	ModDescriptorProperties *p;

	WIZ_DYNAMIC_CAST( prop, p, ModDescriptorProperties * );
	p->desc.take( desc );
}
	
	
DevPropertiesDlg *
ModMdisGeneric::createPropDlg( QWidget *parent )
{
	DevPropertiesDlg *dp;
	QWidget *defTab;
	ModDescriptorProperties *p;

	WIZ_DYNAMIC_CAST( prop, p, ModDescriptorProperties * );
	
	dp = new DevPropertiesDlg( parent, this );
	dp->setButtons();
	if (p->desc.hasVisibleChildren()) {
		dp->addTab( (defTab = new DevGenericPropTab(parent, dp)), "Descriptor" );
		dp->showPage( defTab );
	}
	return dp;
}

DevPropertiesDlg *
ModMdisGeneric::createPropDlg4Win( QWidget *parent )
{
	DevPropertiesDlg *dp;
	QWidget *defTab;
	ModDescriptorProperties *p;

	WIZ_DYNAMIC_CAST( prop, p, ModDescriptorProperties * );
	
	dp = new DevPropertiesDlg( parent, this, FALSE );
	if (p->desc.hasVisibleChildren()) {
		dp->addTab( (defTab = new DevGenericPropTab(parent, dp)), "Descriptor" );
		dp->showPage( defTab );
	}

	dp->setOkButton(); 
	dp->setCancelButton();

	// use current mouse position
	dp->move( QCursor::pos() );

	return dp;
}

void 
ModMdisGeneric::copyProperties( 
	const DeviceProperties *from,
	DeviceProperties *to )
{
	const ModDescriptorProperties *_from;
	ModDescriptorProperties *_to;

	WIZ_DYNAMIC_CAST( to, _to, ModDescriptorProperties * );
	WIZ_DYNAMIC_CAST( from, _from, const ModDescriptorProperties * );

	Device::copyProperties( from, to );

	// copy descriptor
	_to->desc.take( (DescriptorEntryDirectory *)&_from->desc );

	
}

//
// This is called by MdisDevice::createDesc 
// to fill device specific descriptor entries
// Here: makes a copy of the current device specific descriptor entries
// and adds it to parentDesc
void
ModMdisGeneric::createSpecialDesc( DescriptorEntryDirectory *parentDesc )
{
	ModDescriptorProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModDescriptorProperties * );
	DescriptorEntry *e;
	DescriptorFactory dFact;

	if( subDevOff >= 0 ){
		// note: SUBDEVICE_OFFSET_0 should be hidden and not user modifiable	
		e = dFact.create("SUBDEVICE_OFFSET_0", subDevOff );
		e->setHidden( true );
		parentDesc->addChild( e );
	}	
	for( e=_prop->desc.firstChild(); e; e=_prop->desc.nextChild() )
		parentDesc->addChild( e->clone() );
}

SwModuleList *
ModMdisGeneric::enumSwModules()
{
	// check if we need swapped variant, correct LL driver SwModule if so

	if( swModulesBuilt == false ){
		if( isEndianSwapping()) {
			SwModuleTypes::ModType mType = SwModuleTypes::LlDriver;

			// find the LL or native driver SwModule
			SwModule *llDrv = lstSwModules->find( mType );

			if( !llDrv ){
				mType = SwModuleTypes::NativeDriver;
				llDrv = lstSwModules->find( mType );
			}
			
			if( llDrv ){
				// create a LL driver for swapped variant
				QString makef = llDrv->makefilePath();
				if( !makef.endsWith( "_sw.mak" )){
					makef = makef.replace( QRegExp("\\.mak$"), "_sw.mak" );

					if( llDrv->isSwapCapable() ){
						SwModule *swLlDrv = new SwModule( 
							mType,
							llDrv->modName() + "_sw",
							makef,
							llDrv->description() + "-- byteswapped access" );
                        // remove org. LLDRV SwModule
						lstSwModules->remove( llDrv );
						lstSwModules->add( swLlDrv ); // replace with new one
					}
					else {
						// no swapping driver available (perhaps not needed)
						//lstSwModules->remove( llDrv );
					}
				}
			}
		}
		swModulesBuilt = true;
	}

	return lstSwModules;
}


QString
ModMdisGeneric::getMainProperties()
{
	QString rv = MdisDevice::getMainProperties();

	if( subDevOff >= 0 )
		rv += QString(" Off 0x%1").arg(subDevOff,0,16);
	return rv;
}

//! as Device::getDriverName, but hides _SW extension if no swapping driver
// defined
QString 
ModMdisGeneric::getDriverName( bool fullName, bool withSw)
{
	QString rv;

	rv = driverName.upper();
	if( rv == QString::null )
		return rv;

	if( withSw ){
		if( isEndianSwapping() ){
			SwModule *llDrv = lstSwModules->find( SwModuleTypes::LlDriver );
			if( !llDrv )
				llDrv = lstSwModules->find( SwModuleTypes::NativeDriver );

			if( llDrv==0 || llDrv->isSwapCapable()) {
				rv += "_SW";
			}
		}
	}

	if( fullName )
		rv = MAIN_GetCurrentCfg()->targetOs()->driverName( rv, this );	

	return rv;	
}

//
// take over passed descriptor into our properties
// compares the passed descriptor with the template descriptor
//
Device::ApplyResult
ModMdisGeneric::applyProperties( DescriptorEntryDirectory *devDesc, 
								 QString &errMsg )
{
	ModDescriptorProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModDescriptorProperties * );
	DescriptorEntry *te, *e, *eNext;

	if( !devDesc ) 
		return ApplyPropFail;

	// get the template descriptor
	DescriptorEntryDirectory tmplDesc("");
	Device *tmplDev = create(false);
	tmplDev->createSpecialDesc(&tmplDesc);
	_prop->desc.take( &tmplDesc );
	delete tmplDev;
	
	// check for SUBDEVICE_OFFSET_0
	if( (e = devDesc->find( "SUBDEVICE_OFFSET_0", DescriptorEntry::Uint32 ))){
		setSubDevOff( ((DescriptorEntryUint32 *)e)->getValue());
		// note: SUBDEVICE_OFFSET_0 still required to obtain set value
		// from passed descriptor
	}

	// scan all entries of template device
	for( te=_prop->desc.firstChild(); te; te=te->nextSibling() ){
		QString key = te->path().section("/",1);

		wDebug(("tmpl=%s", (const char *)(devDesc->getKey() + "/" + key)));

		// check if the descriptor entry exists in passed descriptor
		if( ( e = devDesc->find( key, te->getType() ) ) != 0 ){
			wDebug(("found"));
			te->setActive(true);
			te->setValue( e->valueToString() ); 

			// remove entry from passed descriptor to flag it has been 
			// processed
			if( e->getType() != DescriptorEntry::Directory )
				e->getParent()->removeChild(e);

		}
		else {
			// entry does not exist in passed descriptor
			// set default value and deactivate key
			if( te->getType() != DescriptorEntry::Directory )
				te->setActive(false);
		}
		//wDebug(("defval=%s val=%s", (const char *)te->valueToString(true),
		//	   (const char *)te->valueToString(false)));
	}

	// delete all empty directories in passed descriptor
	eNext=devDesc->firstChild();

	while( (e=eNext) ){
		eNext=e->nextSibling();

		if( e->getType() == DescriptorEntry::Directory && 
			((DescriptorEntryDirectory *)e)->firstChild() == 0 )
			e->getParent()->removeChild(e);
	}
		
	return Device::applyProperties( devDesc, errMsg );
}

//! set the modules for which a debug level exists
/*! \sa DebugProperties::setModules */
void
ModMdisGeneric::setDebugSettingMods( ulong modules )
{
	prop->dbgSettings.setModules( modules );
}

//! create a new instance of this device class (with properties of template)
/*!
  \remark It is assumed that this method is called only on the template
  device (device that is in the ModelFactory)
*/
Device *
ModMdisGeneric::create(bool withSubDevs)
{
	ModMdisGeneric *newDev;
	withSubDevs = withSubDevs;

	newDev = new ModMdisGeneric( getModelName(), driverName, removeable, ifType );
	newDev->copy( this );
	newDev->setDescriptor( &((ModDescriptorProperties *)prop)->desc );
	newDev->setDebugSettingMods( prop->dbgSettings.getModules());
	newDev->setDescription( description );

	if( !devNameTpl.isEmpty() )
		newDev->setDeviceNameTpl( devNameTpl );


	newDev->setSubDevOffTbl( subDevOffTbl );

	if( subDevOffTbl.count() > 0 )
		newDev->setSubDevOff( subDevOffTbl[0] );

	return newDev;
}

Device *
ModMdisGeneric::createSubDev(int subDevNo)
{
	ModMdisGeneric *newDev = NULL;

	if( subDevNo > 0 && subDevNo < (int)subDevOffTbl.count()){
		newDev = (ModMdisGeneric *)create(false);
		newDev->setSubDevOff( subDevOffTbl[subDevNo] );
	}
	return newDev;
}

