/***************************************************************************
 */
/*!       \file  configuration.cpp
 *       \brief  Implementation of class Configuration
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2018/06/13 14:13:33 $
 *    $Revision: 2.8 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: configuration.cpp,v $
 * Revision 2.8  2018/06/13 14:13:33  ts
 * R: customer required fpga_load build to be possible together with project
 * M: added SW module for "fpga_load" for VxWorks
 *
 * Revision 2.7  2013/03/04 13:02:42  dpfeuffer
 * R: open existing project with SC24 CPU doesn't work
 * M: using class ModBbisChamBase instead of class ModBbisCham
 *
 * Revision 2.6  2011/03/31 16:30:31  CRuff
 * R: 1. pci domains are supported in MDIS now
 * M: 1a) removed special handling for F50 in loadDevice()
 *    1b) in addDevice(): for root device, remember if pci domains are needed
 *
 * Revision 2.5  2010/07/14 17:01:43  CRuff
 * R: creation of patch before pci domains are implemented in MDIS
 * M: reactivate special handling for F50 during loadDevice()
 *
 * Revision 2.4  2010/07/14 10:48:51  CRuff
 * R: make building of core lib chameleon configurable
 * M: add chameleon library to configurable list of core libraries
 *
 * Revision 2.3  2010/07/05 17:48:22  CRuff
 * R: implementation of PCI Domains
 * M: removed special handling for F50 in loadDevice(); not needed any more
 *
 * Revision 2.2  2010/06/21 17:05:36  CRuff
 * R: ModBbisCham::create() cannot explicitly create onboard fpgas any more
 * M: in loadDevice(), call method create_onboard() for fpga on F50
 *
 * Revision 2.1  2010/06/21 10:54:26  CRuff
 * R: F50 projects could not be loaded correctly
 * M: implemented special handling for loading of F50 projects: use non-onboard
 *    constructor for the chameleon bbis to allow PCI bus number identification
 *
 * Revision 2.0  2010/01/22 11:25:33  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.14  2009/05/13 15:12:29  CRuff
 * R: Compiler warning because of uninitialized pointers
 * M: initialize pointers with declaration in Configuration::loadDevice()
 *
 * Revision 1.13  2009/04/27 17:29:23  CRuff
 * R: make Kernel modules OSS and DBG configurable
 * M: Configuration::Configuration(): added SwModules OSS and DBG to list
 *    of selectable modules
 *
 * Revision 1.12  2009/04/01 15:16:08  CRuff
 * R: 1.Chameleon V2 support
 * M: 1.a) extended function loadDevice():
 * 	special handling of chameleon bbis devices
 *
 * Revision 1.11  2006/11/30 11:27:28  cs
 * cosmetics
 *
 * Revision 1.10  2004/07/21 14:55:19  ufranke
 * added
 *   + common SW modules like M_MOD_ID in constructor Configuration
 *
 * Revision 1.9  2004/07/14 11:18:39  ufranke
 * cosmetics
 *
 * Revision 1.8  2004/06/08 17:25:07  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.7  2004/01/09 08:19:49  kp
 * added use of swmod supportsOs
 * removed "UNUSUABLE" handling
 *
 * Revision 1.6  2003/06/11 12:21:26  kp
 * intermediate checkin
 *
 * Revision 1.5  2003/06/05 12:00:42  dschmidt
 * intermediate checkin
 *
 * Revision 1.4  2003/06/05 09:33:05  kp
 * intermediate checkin
 *
 * Revision 1.3  2003/06/03 10:16:00  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/05/14 13:42:24  kp
 * new param to descFile.write
 *
 * Revision 1.1  2003/05/02 14:34:27  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#include <q3tabdialog.h>
#include <qdatetime.h>

#include <Q3PtrList>

#include "cfgmakefile.h"
#include "configuration.h"
#include "hwcomponent.h"
#include "descfile.h"
#include "descriptor.h"
#include "modelfactory.h"
#include "targetos.h"
#include "swmodule.h"
#include "wizdebug.h"
#include "wizexcept.h"
#include "swmodulestab.h"
#include "vmsgbox.h"
#include "modbbischam.h"

extern QWidget *G_mainWindow; //!< used as anchor for dialog

//! creates Configuration object
Configuration::Configuration() :
	nameMng(QString("Device Names"))
{
	SwModule *swMod;
	root = 0;
	cfgName = QString::null;
	os = 0;

	wDebug(("Configuration::Configuration()"));
	lstSettings.setAutoDelete(true);

	lstSwModules.setAutoDelete(true);

	// prepare default list    CORE LIBS
	wDebug(("add default CORE LIBS to lstSwModules"));
	SwModuleTypes::ModType type = SwModuleTypes::CoreLib;
	swMod = new SwModule( type, "dbg", "DBG/library.mak",
						  "Kernel Space Debug Library" );
	lstSwModules.add( swMod );
	
	swMod = new SwModule( type, "oss", "OSS/library.mak", 
						  "Kernel Space Operation System Services Library" );
	lstSwModules.add( swMod );
	
	swMod = new SwModule( type, "chameleon", "CHAMELEON/COM/library.mak",
						  "Kernel Space Chameleon Library" );
	lstSwModules.add( swMod );
	
	// prepare default list    USR LIBS
	wDebug(("add default USR LIBS to lstSwModules"));
	type = SwModuleTypes::UserLib;
	swMod = new SwModule( type, "usr_oss", "USR_OSS/library.mak",
						 "User Space Operation System Services Library" );
	lstSwModules.add( swMod );

	swMod = new SwModule( type, "usr_utl", "USR_UTL/COM/library.mak",
						 "Common User Space Util Library" );
	lstSwModules.add( swMod );


	// prepare default list    COM TOOLS
	wDebug(("add default COM TOOLS to lstSwModules"));
	type = SwModuleTypes::ComTool;

	swMod = new SwModule( type, "m_errstr", "MDIS_API/M_ERRSTR/COM/program.mak",
						 "Convert a MDIS error code to a human readable string" );
	lstSwModules.add( swMod );

	swMod = new SwModule( type, "m_getblock", "MDIS_API/M_GETBLOCK/COM/program.mak",
						 "Testing M_getblock function" );
	lstSwModules.add( swMod );

	swMod = new SwModule( type, "m_getstat", "MDIS_API/M_GETSTAT/COM/program.mak",
						 "Testing M_getstat function" );
	lstSwModules.add( swMod );

	swMod = new SwModule( type, "m_getstat_blk", "MDIS_API/M_GETSTAT_BLK/COM/program.mak",
						 "Testing M_getstat() function (blockwise)" );
	lstSwModules.add( swMod );

	swMod = new SwModule( type, "m_mod_id", "MDIS_API/M_MOD_ID/COM/program.mak",
						 "Print or program module id" );
	lstSwModules.add( swMod );

	swMod = new SwModule( type,
	                     "m_open", "MDIS_API/M_OPEN/COM/program.mak",
						 "Open and close a MDIS device" );
	lstSwModules.add( swMod );

	swMod = new SwModule( type, "m_read", "MDIS_API/M_READ/COM/program.mak",
						 "Testing M_read() function" );
	lstSwModules.add( swMod );

	swMod = new SwModule( type, "m_rev_id", "MDIS_API/M_REV_ID/COM/program.mak",
						 "Print software revision ids" );
	lstSwModules.add( swMod );

	swMod = new SwModule( type, "m_setblock", "MDIS_API/M_SETBLOCK/COM/program.mak",
						 "Testing M_setblock() function" );
	lstSwModules.add( swMod );

	swMod = new SwModule( type, "m_setstat", "MDIS_API/M_SETSTAT/COM/program.mak",
						 "Testing M_setstat() function" );
	lstSwModules.add( swMod );

	swMod = new SwModule( type, "m_setstat_blk", "MDIS_API/M_SETSTAT_BLK/COM/program.mak",
						 "Testing M_setstat() function (blockwise)" );
	lstSwModules.add( swMod );

	swMod = new SwModule( type, "m_write", "MDIS_API/M_WRITE/COM/program.mak",
						 "Testing M_write() function" );
	lstSwModules.add( swMod );

	swMod = new SwModule( type, "hwbug_ppc", "HWBUG/program.mak",
						 "Simple Debugger for Hardware Development Purpose" );
	swMod->setInternal(true);
	lstSwModules.add( swMod );

	swMod = new SwModule( type, "mm_ident", "mm_ident/program.mak",
						 "Tool to read the M-Module EEPROM." );
	swMod->setInternal(true);
	lstSwModules.add( swMod );

	// Should not be compiled as a default module
	//swMod = new SwModule( type, "fpga_load", "FPGA_LOAD/program.mak",
	//					 "FPGA update utility (VxWorks only)" );
	//lstSwModules.add( swMod );

	// dbgSwModuleList();
	
	// set default
	mdisSupportsPciDomains = false;

}

//! creates a new configuration of the same class.
Configuration *
Configuration::create()
{
	return new Configuration();
}


//! destructs Configuration object
Configuration::~Configuration()
{
	if( root )
		delete root;
}

//! add a new device to the device hierarchy
/*!
  adds \a dev and all of its children to the configuration.

  For each device in the hierarchy, applyProperties is called.

  If \a parentComp is null, \a dev will be the new CPU device.

  \return true if \a dev has been sucessfully added
*/
bool
Configuration::addDevice( HwComponent *parentComp, Device *dev,
						  QString &errMsg )
{
	if( parentComp == 0 ){
		// set new root device
		WIZ_ASSERT( root == 0 );
		WIZ_DYNAMIC_CAST( dev, root, CpuDevice *);
		
		if (root->hasPciDomains()) {
			this->setPciDomainsNeeded( true );
		} else {
			this->setPciDomainsNeeded( false );
		}
	}
	else {
		wDebug(("addDevice %s to %s", dev->getInstName().latin1(),
				parentComp->getInstName().latin1()));
		// paranoia check: is parentComp really in current Cfg?
		WIZ_ASSERT( root );
		WIZ_ASSERT( root->findChild( parentComp ) != 0 );

		// BBIS/MDIS device
		parentComp->addChild( dev );
	}

	return applyDefaultDevProps( dev, errMsg );
}

//! add the SwModules of \a comp
/*!
  (if the modules are not yet known in configuration)
*/
void
Configuration::addSwMods( HwComponent *comp )
{
	// only check for software modules if the HwComponent is a device
	Device *dev = dynamic_cast<Device *>(comp);

	if( dev ){
		wDebug(("Configuration::addSwMods Hw"));
		SwModuleList *mods = dev->enumSwModules();
		SwModule *swMod;

		for( swMod = mods ? mods->first():0; swMod; swMod=mods->next() )
			addSwMod( swMod );
	}
}

//! add software module \a swMod to configuration
/*!
  Adds it, if the module is not yet known in configuration. Otherwise
  just increments its use count.

  Also checks if \a swMod supports the current OS
*/
void
Configuration::addSwMod( SwModule *swMod )
{
	SwModule *cfgMod;

	wDebug(("Configuration::addSwMod"));
	if( os && !swMod->supportsOs( os->osType() ) )
		return;

	wDebug(("Configuration::addSwMod %s", swMod->modName().latin1() ));
	if( (cfgMod = lstSwModules.find( swMod->modType(),
									 swMod->modName() )) == 0){
		lstSwModules.add( cfgMod = (new SwModule( *swMod )), false );
		wDebug(("addSwMods: added %s", (const char *)
			   cfgMod->makefilePath()));
	}
	cfgMod->incUseCount();
}


void
Configuration::dbgSwModuleList( void )
{
	SwModule *swMod;


    for ( swMod = lstSwModules.first(); swMod; swMod = lstSwModules.next() )
		wDebug(("dbgSwModuleList: %s path %s",
				swMod->modName().latin1(),
			   	swMod->makefilePath().latin1() ));

}


//! set default properties for all devs in hierarchy of \a comp
bool
Configuration::applyDefaultDevProps( HwComponent *comp, QString &errMsg )
{
	HwComponent *subComp;

	// only check for software modules if the HwComponent is a device
	Device *dev = dynamic_cast<Device *>(comp);

	if( dev ){
		if( dev->applyProperties( (DeviceProperties *)0, errMsg ) ==
			Device::ApplyPropFail ){
			dev->getParent()->removeChild( dev );
			return false;
		}
	}
	addSwMods( comp );

	for( subComp=comp->firstChild(); subComp; subComp=comp->nextChild())
		applyDefaultDevProps( subComp, errMsg );
	return true;
}

//! set the state of all modules in lstSwModules to disabled
/*!
  It is typically called from TargetOs::loadConfiguration()
*/
void
Configuration::setAllSwModulesDisabled()
{
	SwModule *mod;

	for( mod=lstSwModules.first(); mod; mod=lstSwModules.next())
		mod->setEnabled( false );
}


//! remove device from device hierarchy
/*!
  It also unlinks the SW modules referenced by this device.
*/
void
Configuration::removeDevice( Device *dev )
{
	// paranoia check: is dev really in current Cfg?
	WIZ_ASSERT( root );
	WIZ_ASSERT( root->findChild( dev ) != 0 );

	// recursively remove SwMods
	removeSwMods( dev );

	HwComponent *parent = dev->getParent();
	if( parent == 0 ){
		// delete root device
		delete root;
		root = 0;
	}
	else
		parent->removeChild( dev );

}

//! Removes the SwModules of devices and its subdevices if its useCount is 0
void
Configuration::removeSwMods( HwComponent *comp )
{
	HwComponent *subComp;

	// only check for software modules if the HwComponent is a device
	Device *dev = dynamic_cast<Device *>(comp);

	if( dev ){
		SwModuleList *mods = dev->enumSwModules();
		SwModule *swMod;

		for( swMod = mods ? mods->first():0; swMod; swMod=mods->next() )
			removeSwMod( swMod );
	}

	// check for SwModules in subcomponents
	for( subComp=comp->firstChild(); subComp; subComp=comp->nextChild())
		removeSwMods( subComp );
}

//! remove software module \a swMod from configuration
void
Configuration::removeSwMod( SwModule *swMod )
{
	SwModule *cfgMod;

	if( !swMod->supportsOs( os->osType() ) )
		return;

	if( (cfgMod = lstSwModules.find( swMod->modType(),
									 swMod->modName() )) != 0){

		cfgMod->decUseCount();

		if( cfgMod->useCount() == 0 ){
			wDebug(("removeSwMods: removed %s", (const char *)
				   cfgMod->makefilePath()));
			lstSwModules.remove( cfgMod );
		}
	}
	WIZ_ASSERT( cfgMod );
}

//! save the the configuration to filesystem
/*!
  If the configuration is unnamed, let user choose a configuration
  name (OS-specific, usually a directory name)

  \return true if configuration successfully stored or false if failed
  and \a errMsg contains a newline seperated string with error messages

  \remark Configuration::save does nothing, subclasses must reimplement it
*/
bool
Configuration::save(QString &errMsg)
{
	UTIL_UNREF_PARAM( errMsg );
	return false;
}

//! save the the configuration to filesystem (let user choose a name)
/*!
  \return true if configuration successfully stored or false if failed
  and \a errMsg contains a newline seperated string with error messages

  \remark Configuration::saveAs does nothing, subclasses must reimplement it
*/
bool
Configuration::saveAs(QString &errMsg)
{
	UTIL_UNREF_PARAM( errMsg );
	return false;
}

//! Load the current configuration from filesystem
/*!
  If \a _cfgName is passed as 0, let user choose a configuration name,
  otherwise load the specified configuration.

  \return true if configuration successfully loaded or false if failed
  and \a errMsg contains a newline seperated string with error messages

  \remark Configuration::load does nothing, subclasses must reimplement it
*/
bool
Configuration::load( QString &errMsg, const QString &_cfgName )
{
	UTIL_UNREF_PARAM( errMsg );
	UTIL_UNREF_PARAM( _cfgName );
	return false;
}

//! Bring up dialog to select the configuration name
/*!
  \param action 	dialog's title
  \param mustExist	If true, allow to select existing files
                    (configurations) only
  \param errMsg		will receive error messages
  \return name of selected configuration or QString::null if user
  canceled dialog. \a errMsg may contain messages if the selected config name
  is invalid for some other reason.
*/
QString
Configuration::selectConfigName( const QString &action,
								 bool mustExist, QString &errMsg )
{
	UTIL_UNREF_PARAM( action );
	UTIL_UNREF_PARAM( mustExist );
	UTIL_UNREF_PARAM( errMsg );
	return QString::null;
}

//! save the current device hierarchy to the system descriptor file
/*!
  Called normally by TargetOs::saveConfiguration()

  \param systemDescName file name of system descriptor file (with path)
  \param errMsg		will receive error messages

  \return true if configuration successfully loaded or false if failed
  and \a errMsg contains a newline seperated string with error messages
*/
bool
Configuration::saveDesc( const QString &systemDescName, QString &errMsg )
{
	DescFile descFile(systemDescName);
	DescriptorEntryDirectory descTmp( "_TMP_" );

	errMsg += QString::null;

	// create descriptor hierarchy starting from root device
	if( !root ) return false;

	root->createDesc( &descTmp );

	QString headerText;

	headerText.sprintf("#\n# Automatically generated by "
					   "mdiswiz %s\n# %s\n#\n\n",
					   targetOs()->mdiswizVersion().latin1(),
					   QDate::currentDate().toString(Qt::ISODate).latin1() );
	try {
		descFile.write( &descTmp, headerText );
	}
	catch ( DescFileExcept &e ){
		// error writing descfile
		errMsg = QString("Could not write %1\n").arg(systemDescName) +
			e.getMsg();
		return false;
	}
	return true;
}

//! load the current device hierarchy from the system descriptor file
/*!
  Load the system descriptor file and build internal device hierarchy.
  Called normally by TargetOs::loadConfiguration()

  \param systemDescName file name of system descriptor file (with path)
  \param errMsg		will receive error messages

  \return true if configuration could be loaded successfully or\n
	      with non-fatal errors (warnings contained in errMsg)\n
		  false on fatal errors (errors contained in errMsg)

  Replaces the current device hierarchy when no fatal error occurred.
  On fatal errors, \em root will be set to 0 (if fatal error appears after
  successfull open of descriptor file)
*/
bool
Configuration::loadDesc( const QString &systemDescName, QString &errMsg )
{
	DescFile descFile(systemDescName);
	DescriptorEntryDirectory *rootDesc;
	CpuDevice *newRootDev = 0;

	// read the descriptor file
	try {
		rootDesc = descFile.read();
	}
	catch ( DescFileExcept &e ){
		// error read descfile
		errMsg = QString("Could not read %1\n").arg(systemDescName) +
			e.getMsg();
		return false;
	}

	// replace old hierarchy
	if( root )
		delete root;
	root = 0;
	wDebug(("old deleted"));

	//
	// Create the devices
	//
	DescriptorEntryDirectory *devDesc;
	QString errMsg2;

	for( devDesc=(DescriptorEntryDirectory *)rootDesc->firstChild(); devDesc;
		 devDesc=(DescriptorEntryDirectory *)rootDesc->nextChild() ){

		wDebug(("Loading %s", (const char *)devDesc->getKey()));

		devDesc->setParent(0);	// disconnect from root entry
		errMsg2 = QString::null;

		int status = loadDevice( &newRootDev, devDesc, errMsg2 );

		if( errMsg2 != QString::null ){
			wDebug(("*** error=%s", (const char *)errMsg2 ));

			if( status == false )
				errMsg += QString("Cannot load %1:\n%2\n").
					arg(devDesc->getKey()).arg( errMsg2 );
			else
				errMsg += QString("Warning while loading %1:\n%2\n").
					arg(devDesc->getKey()).arg( errMsg2 );
		}
	}

	// delete temp descriptor read from descfile
	delete rootDesc;

	if( newRootDev == 0 ){
		errMsg += "No CPU device defined\n";
		return false;
	}

	root = newRootDev;

	return true;
}

//! process one device
/*!
  Insert it into the device structure.

  If it's the CPU device, *rootDevP is set to the new CPU device
  returns true if device could be loaded successfully (errMsg may contain
  warnings)
  \return false if device could not be loaded and errMsg set

  \remark modifies devDesc (deletes the entries processed by loadDevice())
*/
bool
Configuration::loadDevice(
	CpuDevice **rootDevP,
	DescriptorEntryDirectory *devDesc,
	QString &errMsg)
{
	extern CpuModelFactory *G_cpuMods;
	extern BbisModelFactory *G_bbisMods;
	extern MdisModelFactory *G_mdisMods;

	ulong descType;
	QString modelName;
	QString bbName;
	QString hwType;
	QString errMsg2;
	Device *dev = 0;
	HwComponent *parentComp=0, *tmpComp=0;
	BusInterface *busIf=0;
	BbisSlot *bbSlot=0;
	BbisDevice *bbDev=0;
	ulong deviceSlot=0;
	bool isChameleonMdis = false;
	
	wDebug(("Configuration::loadDevice()"));

	// lookup DESC_TYPE
	if( findDescKey( devDesc, DescriptorFactory::DESC_TYPE,
					 errMsg, &descType ) == false )
	{
		wDebug(("*** loadDevice: DESC_TYPE" ));
		return false;
	}

	// lookup _WIZ_MODEL (i.e. real hardware)
	if( findDescKey( devDesc, DescriptorFactory::_WIZ_MODEL,
					 errMsg, &modelName ) == false )
	{
		wDebug(("*** loadDevice: _WIZ_MODEL" ));
		return false;
	}


	// remove HW_TYPE from descriptor
	if( findDescKey( devDesc, DescriptorFactory::HW_TYPE,
					 errMsg, &hwType ) == false )
	{
		wDebug(("*** loadDevice: HW_TYPE" ));
		return false;
	}

	// find the model
	switch( descType ){
	case 0:					// CPU
		dev = G_cpuMods->find( modelName );
		break;

	case 1:					// MDIS dev
		if( dev == 0 )
			dev = G_mdisMods->find( modelName );
		break;

	case 2:					// BBIS dev
		dev = G_bbisMods->find( modelName );
		break;

	default:
		errMsg = QString("Unknown descriptor type %1\n").arg(descType);
		return false;
	}

	if( dev == 0 ){
		errMsg = QString("Unknown model \"%1\"\n").arg(modelName);
		return false;
	}

	// get name of the new component
	QString newCompName = devDesc->getKey();

	switch( descType ){
    //---- CPU/BBIS/EXT ---------------------------------------------------
	case 0:
	case 2:
		// lookup _WIZ_BUSIF descriptor key for CPU extension devs
		if( findBusIf( devDesc, errMsg, &busIf, *rootDevP ) == false )
			return false;

		if( *rootDevP ){
			// if a root device already defined, busIf must exist
			if( busIf == 0 )
				return false;
		}
		else {
			if( busIf ){
				errMsg = "Root device must not refer to uppper device\n";
				return false;
			}
		}
		parentComp = busIf;
		errMsg = QString::null;

		// check if we can connect the BBIS device
		if( busIf && busIf->canConnect( dev ) == false ){
			errMsg = QString("Can't connect to CPU: incompatible interface\n");
			return false;
		}

		break;
    //---- MDIS -----------------------------------------------------
	case 1:
		if( *rootDevP == 0 ){
			errMsg = "Missing CPU device\n";
			return false;
		}

		// get BOARD_NAME
		if( findDescKey( devDesc, DescriptorFactory::BOARD_NAME,
						 errMsg, &bbName ) == false )
			return false;

		// get DEVICE_SLOT
		if( findDescKey( devDesc, DescriptorFactory::DEVICE_SLOT,
						 errMsg, &deviceSlot ) == false )
			return false;

		// find it's board device, which must be already defined
		tmpComp = (*rootDevP)->findChild( bbName );

		if( tmpComp == 0 ){
			errMsg = QString("Could not find BBIS device %1\n").arg(bbName);
			return false;
		}
		WIZ_DYNAMIC_CAST( tmpComp, bbDev, BbisDevice *);

		// get the BBIS slot
		bbSlot = bbDev->bbSlot(deviceSlot);
		parentComp = bbSlot;

		if( bbSlot == 0 ){
			errMsg = QString("No slot %1 on BBIS device %2\n").
				arg(deviceSlot).arg( bbName );
			return false;
		}
		
		if ( bbSlot->getIfType() == HwComponent::ChameleonBus )
		{
			isChameleonMdis = true;
		}

		// check if we can connect the MDIS device
		if( bbSlot->canConnect( (MdisDevice *)dev ) == false ){
			errMsg = QString("Can't connect to %1: incompatible "
							 "interface\n").
				arg( bbName );
			return false;
		}
		break;

	default:
		break;
	}

	// create an instance of that model (without subdevices)
	dev = dev->create( false );
	WIZ_ASSERT(dev);
	
	if ( isChameleonMdis )
	{
		// cast bbis to chameleon bbis 
		ModBbisChamBase* bbisCham;
		WIZ_DYNAMIC_CAST( bbDev, bbisCham, ModBbisChamBase* );
		
		// add unhandeled descriptor keys from cham bbis to the mdis device
		DescriptorEntryDirectory* dirDesc = bbisCham->getUnhandeledDesc(deviceSlot);
		if ( dirDesc )
		{
			Q3PtrList<DescriptorEntry> oldDescs = dirDesc->getChildren();
			for (uint i=0; i<oldDescs.count(); i++)
			{
				devDesc->addChild(oldDescs.at(i));
			}
		}
	}

	// add the new component to it's parent (ignore errMsg2)
	addDevice( parentComp, dev, errMsg2 );
	if( parentComp == 0 ){
		CpuDevice *cDev;
		WIZ_DYNAMIC_CAST( dev, cDev, CpuDevice *);
		*rootDevP = cDev;
	}

	dev->freeResources(); // free default resources

	// apply resources from scanned descriptor
	if( dev->applyProperties( devDesc, errMsg ) == Device::ApplyPropFail ){
		// failed to allocate resources, delete device
		removeDevice( dev );
	}

	return true;
}

//! find the bus interface according to _WIZ_BUSIF
/*!
*/
bool
Configuration::findBusIf(
	DescriptorEntryDirectory *devDesc,
	QString &errMsg,
	BusInterface **busIfP,
	CpuDevice *rootDev)
{
	QString val, parentName;
	int busIfNum;
	bool ok;
	HwComponent *hw, *_busIf;
	BusInterface *busIf;
	DescriptorFactory dFact;

	*busIfP = 0;

	if( findDescKey( devDesc, DescriptorFactory::_WIZ_BUSIF,
					 errMsg, &val, true ) == false )
		return true; 	// is normal for root CPU

	wDebug(("Configuration::findBusIf: %s", val.latin1()));

	// split
	parentName 	= val.section( ',', 0, 0 );
	busIfNum 	= val.section( ',', 1, 1 ).toInt( &ok );
	wDebug(("Configuration::findBusIf: %s , %d", parentName.latin1(),
			busIfNum));

	if( !ok )
		goto fail;

	if( !rootDev )
		goto fail;

	if( !(hw = rootDev->findChild( parentName )) )
		goto fail;

	if( !(_busIf = hw->getChildren().at(busIfNum)))
		goto fail;


	WIZ_DYNAMIC_CAST( _busIf, busIf, BusInterface *);

	*busIfP = busIf;
	wDebug(("Configuration::findBusIf: OK!" ));

	return true;

 fail:
	errMsg = QString("Bad value for %1: %2\n").
		arg(dFact.keyStr( DescriptorFactory::_WIZ_BUSIF )).arg(val);
	return false;
}

//! get value of an U_INT32 descriptor entry
/*!
  \return true and *valueP if key was found or
  false and errMsg set if key was not found
*/
bool
Configuration::findDescKey(
	DescriptorEntryDirectory *devDesc,
	DescriptorFactory::KeyType key,
	QString &errMsg,
	ulong *valueP,
	bool deleteKey)
{
	DescriptorEntry *e;
	DescriptorEntryUint32 *entry;
	DescriptorFactory dFact;

	e = devDesc->find( dFact.keyStr( key ),
					   DescriptorEntry::Uint32 );

	if( e == 0 ){
		errMsg = QString("Descriptor key %1 missing\n").
			arg(dFact.keyStr( key ));
		return false;
	}

	WIZ_DYNAMIC_CAST(
		e,
		entry,
		DescriptorEntryUint32 *);

	*valueP = entry->getValue();

	if( deleteKey )
		if( entry->getParent() )
			entry->getParent()->removeChild( entry );

	return true;
}


//! get value of a STRING descriptor entry
/*!
  if \a deleteKey is true, delete key from descriptor
  \return true and *valueP if key was found,
  false and \a errMsg set if key was not found
*/
bool
Configuration::findDescKey(
	DescriptorEntryDirectory *devDesc,
	DescriptorFactory::KeyType key,
	QString &errMsg,
	QString *valueP,
	bool deleteKey)
{
	DescriptorEntry *e;
	DescriptorEntryString *entry;
	DescriptorFactory dFact;

	e = devDesc->find( dFact.keyStr( key ),
					   DescriptorEntry::String );

	if( e == 0 ){
		errMsg = QString("Descriptor key %1 missing\n").
			arg(dFact.keyStr( key ));
		return false;
	}

	WIZ_DYNAMIC_CAST(
		e,
		entry,
		DescriptorEntryString *);

	*valueP = entry->getValue();

	if( deleteKey )
		if( entry->getParent() )
			entry->getParent()->removeChild( entry );

	return true;
}

//! Bring up project configuration settings dialog
/*!
  This should bring up a dialog where all operating system specific
  and global configuration properties can be modified

  \return true if user modified something (or pressed at least OK button)
  \remark Configuration::settingsDlg does nothing,
  subclasses must reimplement it
*/
bool
Configuration::settingsDlg()
{
	return false;
}

//! bring up wizard for new project
/*!
  \return true if user did not cancel wizard
  \remark Configuration::newProjectWizard does nothing,
  subclasses must reimplement it
*/
bool
Configuration::newProjectWizard( QWidget *parent, const char *name)
{
	UTIL_UNREF_PARAM( parent );
	UTIL_UNREF_PARAM( name );
	return false;
}

//! check all defined SwModules for validity
/*!
  Typically checks the existance of the makefiles for modules delivered as
  source code.

  If \a disableInvalidMods is true, sets all invalid modules to disabled

  If \a showMsgBox is true, displays a VMsgBox if any invalid SW module
  has been found.

  \returns true if all modules are valid, false if not and errMsg contains
  newline separated error messages.

  \sa validateSwModule
*/
bool
Configuration::validateSwModules(
	QString &errMsg,
	bool disableInvalidMods,
	bool showMsgBox )
{
	SwModule *mod;
	bool rv=true;

	for( mod=lstSwModules.first(); mod; mod=lstSwModules.next() ){
		extern bool G_menInternal;
		QString errMsg2;

		if( validateSwModule( mod, errMsg2, disableInvalidMods ) == false ){
			// for internal modules, ignore them silently if not
			// in internal mode
			if( !(mod->isInternal() && !G_menInternal) ){
				errMsg += errMsg2 + "\n";
				rv = false;
			}
		}
	}

	if( rv == false && showMsgBox ){
		VMsgBox::warningOk(
			msgBoxCaption(),
			"Some software modules can't be compiled\n"
			"These modules have been disabled for compilation.",
			errMsg,
			G_mainWindow );
	}

	return rv;
}

//! check SwModule \a mod for validity
/*!
  Typically checks the existance of the makefiles for modules delivered as
  source code.

  If \a disableIfInvalid is true, sets the module to disabled if invalid.

  \returns true if \a mod is valid, false if not and \a errMsg contains
  newline separated error messages.

  Must be reimplemented by subclasses

  \sa validateSwModules
*/
bool
Configuration::validateSwModule(
	SwModule *mod,
	QString &errMsg,
	bool disableIfInvalid )
{
	UTIL_UNREF_PARAM( mod );
	UTIL_UNREF_PARAM( errMsg );
	UTIL_UNREF_PARAM( disableIfInvalid );
	return true;
}

