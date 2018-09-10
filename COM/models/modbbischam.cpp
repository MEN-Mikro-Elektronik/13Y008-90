/***************************************************************************
 */
/*!       \file  modbbischam.cpp
 *       \brief  Implementation of Chameleon FPGA classes
 *      \author  Klaus.Popp@men.de
 *        $Date: 2016/04/29 10:41:24 $
 *    $Revision: 2.22 $
 *
 *        \todo needs cleanup
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbischam.cpp,v $
 * Revision 2.22  2016/04/29 10:41:24  ts
 * R: A21 onboard FPGA bus number 5 was not stored in fpga section, was always 0
 * M: made PCI busnr. configurable, not always 0.
 *    Nr. 0 was ok in old PCI bus configs where 1st PCIbus is always 0 but
 *    with multi PCI domains and PCIe this isnt the case anymore.
 *
 * Revision 2.21  2016/02/20 16:26:38  ts
 * R: BoxPCs based on SC24 need 2 SMBusses in MDIS wizard
 * M: 1. added member nrSmbBusses to class CpuDeviceSmb
 *    2. added parameter int nrSmbBusses to every constructor of CPUs with SMB
 *       controllers: 2 for SC24 (AMD based) Box PCs, 1 for every other CPU
 *
 * Revision 2.20  2015/05/26 17:18:14  ts
 * R: gituser autocheckin: updated source
 *
 * Revision 2.19  2014/08/22 15:56:14  dpfeuffer
 * R:1. inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 *   2. some unnecessary models
 * M:1. PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *   2. useless models removed
 *
 * Revision 2.18  2014/07/18 15:12:05  dpfeuffer
 * R: erroneous PMC support, missing ComExpress support, general maintenance
 * M: intermediate check-in during development
 *
 * Revision 2.17  2014/06/06 14:26:45  ts
 * R: projects with F210/F215/CPCI_STANDARD CPU show default PCI bus 0 dev 15
 *    after reload
 * M: implemented missing readout of PCI_[BUS/DEVICE]_NUMBER keys
 *
 * Revision 2.16  2013/11/27 10:44:14  ts
 * R: F213 was unavailable in the Wizard
 * M: added F213
 *
 * Revision 2.15  2013/03/28 15:31:20  ts
 * R: conductive cooled G215C Variant was not clearly visible in wizard
 * M: added string G215C to description
 *
 * Revision 2.14  2013/03/04 13:02:53  dpfeuffer
 * R: open existing project with SC24 CPU doesn't work
 * M: using class ModBbisChamBase instead of class ModBbisCham
 *
 * Revision 2.13  2012/05/23 15:40:04  ts
 * R: choosing cPCI slot for F210 was not possible
 * M: condition to select between cPciSerial and cPCI corrected
 *
 * Revision 2.12  2011/09/26 08:42:46  dpfeuffer
 * R:1. base class for ModBbisCham (PCI) and ModBbisChamIsa required
 *   2. new bus interfaces required: CpciSer
 *   3. new model G215
 * M:1. ModBbisChamBase implemented with common chameleon stuff
 *   2. modifications for CpciSer
 *   3. new model class ModBbisG215 added
 *
 * Revision 2.11  2011/04/01 09:31:46  CRuff
 * R: 1. pci bus interfaces may have a pci domain
 *    2. pci domain is always stored in key PCI_DOMAIN_NUMBER, not in bus path
 *    3. ModBbisSmbPciGenProperties now inherits ModBbisPciMmodProperties
 * M: 1. in applyProperties(), takeover pci domain from bus interface
 *    2. removed code to handle PCI_BUS_PATH2
 *    3. changed property member names of pci device number and pci slot number
 *
 * Revision 2.10  2010/12/10 11:04:34  CRuff
 * R: cosmetics
 * M: removed unneccessary debug print
 *
 * Revision 2.9  2010/07/12 10:07:34  dpfeuffer
 * R: compiler warning under Windows for mdiswiz_lib.lib
 * M: UTIL_UNREF_PARAM added to unreferenced parameters
 *
 * Revision 2.8  2010/07/05 18:03:14  CRuff
 * R: implementation of PCI Domains
 * M: handle PCI_DOMAIN_NR / PCI_BUS_PATH2 in createSpecialDesc and
 *    applyProperties
 *
 * Revision 2.7  2010/06/21 17:03:13  CRuff
 * R: implemented checkin mechanism corrupts general fpga handling
 * M: undo changes in create(); for explicit onboard fpga creation, implemented
 *    new method create_onboard()
 *
 * Revision 2.6  2010/06/21 11:03:41  CRuff
 * R: Explicit creation of non-onboard devices necessary
 * M: change implementation of create(): call onboard constructor if necessary;
 *    create Bbis slots before returning new device
 *
 * Revision 2.5  2010/06/02 16:50:49  CRuff
 * R: compiler warning because of unused variable
 * M: in createSpecialDesc(), move e to MDISWIZ_LIB code part
 *
 * Revision 2.4  2010/05/31 10:50:23  dpfeuffer
 * R:1. compiler warnings under Windows during mdiswiz_lib build
 *   2. #endif form WIN_MDISWIZ_LIB wrong
 * M:1. variable declarations moved
 *   2. #ifdef WIN_MDISWIZ_LIB section moved
 *
 * Revision 2.3  2010/05/31 09:04:17  CRuff
 * R: AUTOENUM keys not needed for traditional GUI application
 * M: only handle AUTOENUM keys if MDIS Wizard is used as library
 *
 * Revision 2.2  2010/05/21 15:44:51  CRuff
 * R: 1. handling of AUTOENUM keys needed
 *    2. pciPath not copied correctly
 * M: 1. handle AUTOENUM keys in applyProperties() and createSpecialDesc()
 *    2. perform deep copy of pciBusPath with function duplicate()
 *
 * Revision 2.1  2010/05/19 16:57:28  CRuff
 * R: 1. need to support 16Z001 SMB controllers as children of chameleon bbis
 * M: 1a) add a new PciBusInterface without PCI Path (use path of cham BBIS)
 *    1b) getDeviceAtSlot(): return NULL if no slot is found at given slotnumber
 *    1c) use own property class ModBbisPciChamProperties for ModBbisCham
 *    1d) In createSpecialDesc(): forward the pci settings to the
 *        ModBbisSmbPci children
 *    1e) add an SMB Tab to the Properties Dialog of each chameleon BBIS
 *    1f) implement method activateChamSmb() that creates a ModBbisSmbPci
 *        instance that is specified in the properties dialog
 *    1g) implement virtual method createGui() to allow adding
 *        BbisDeviceViewItems to the chameleon BBIS device view item
 *
 * Revision 2.0  2010/01/22 11:28:50  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.13  2009/12/07 13:24:02  dpfeuffer
 * R: unnecessary code for Windows mdiswiz_lib library
 * M: ModBbisCham::createSpecialDesc: ifdef WIN_MDISWIZ_LIB moved
 *
 * Revision 1.12  2009/05/13 13:13:17  CRuff
 * R: Native drivers were not handled correctly, which caused application crash
 * M: Check for device type in Device::MdisCham before casting the device
 *    properties to MdoMdisChamProperties
 *
 * Revision 1.11  2009/05/11 16:19:58  CRuff
 * R: 1. Device identifications in groups done with index, not instance number
 * M: 1a) createSpecialDesc(): last byte of device descriptor for group members
 *        contains index, not instance number
 *
 * Revision 1.10  2009/04/30 17:31:27  CRuff
 * R: Cosmetics
 * M: initialize pointers to avoid compiler warnings
 *
 * Revision 1.9  2009/04/21 14:57:53  CRuff
 * R: 1. support chameleon mezzanine cards
 * M: 1.a) add flag _mezz to bbis (default = false)
 *      b) ModBbisCham::createSpecialDesc(): special handling for mezzanines
 *      c) ModBbisCham::createPropDlg(): enable properties dialog for mezzanines
 *
 * Revision 1.8  2009/04/07 11:42:27  dpfeuffer
 * R: ModBbisCham::setOtherInstances(): signed/unsigned warning with VC compiler
 * M: type of loop variable changed
 *
 * Revision 1.7  2009/04/01 15:07:52  CRuff
 * R: 1.Chameleon V2 support
 * M: 1.a) extended functions applyProperties() and createSpecialDesc()
 * 	to also support cham v2 ids and groups
 *      b) added new methods to keep track of used instance nrs and group ids
 *
 * Revision 1.6  2007/10/05 16:21:16  cs
 * changed:
 *   - raised number of slots to 32
 *   - changed slot description to "virtual slot #" to avoid
 *     allusions to index/instance in table
 *
 * Revision 1.5  2006/11/30 11:28:10  cs
 * added:
 *   + support for onboard devices with actual PCI_BUS_PATH
 *   + don't show PCI properties dialog for onboard devices (is known/fixed)
 *
 * Revision 1.4  2005/08/02 11:20:47  dpfeuffer
 * now usable from W2k MDIS class installer
 *
 * Revision 1.3  2005/07/15 15:47:22  ub
 * enhanced to support F206
 *
 * Revision 1.2  2005/03/09 13:49:26  ub
 * new model "ModBbisF206"
 *
 * Revision 1.1  2004/06/08 17:25:58  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3ValueList>
#include <Q3MemArray>

#include "modbbischam.h"
#include "modmdischam.h"
#include "modbbissmbpcigen.h"
#include "wizdebug.h"
#include "mdiswiz.h"
#include "configuration.h"
#include "resourcemng.h"
#include "vmsgbox.h"

extern QWidget *G_mainWindow; 

// max. number of chameleon units inside FPGA
static const uint G_numSlots = 32;

// -----------------------------------------------------------------
// BBIS Cham

//! Create an Cham BBIS device which resides on a CPU board directy.
/*!
  The device will have the default properties set by createProperties().
  It will have always PCI bus number 0.

  It will have 16 BBIS slots (numbered 0..15) of type "ChameleonBus".

*/
ModBbisCham::ModBbisCham( const QString &_variant, int devNo ) :
    ModBbisChamBase( "CHAMELEON", "CHAMELEON", false, StdPci )
{
    
    _onboard = true;
    _mezz = false;
    variant = _variant;
    setHwName( getDriverName( false, false ) );

	if ( variant == "PCITBL" )
		setDescription( "Container device for chameleon FPGA units" );
	else if ( variant == "PCITBL_MSI" )
		setDescription( "Container device for chameleon FPGA units with MSI Support" );
    if( prop ) delete prop; // delete Device::prop
    prop = createProperties( devNo );
    this->chamSmbActive = false;
    
    // smbus interface
    this->smbusIf = new PciBusInterface( LocalBus, -1, -1, 0, 0, false );
	this->smbusIf->setInstName( QString("BusIf_SMB"));
	this->smbusIf->setEndianSwapping( true );
	this->smbusIf->setDescription("SMB BBIS Interface");
	addChild( this->smbusIf );

	// create BBIS slots
    for( uint i=0; i<G_numSlots; i++ ){
        BbisSlot *bbSlot = new BbisSlot( ChameleonBus, i );
        bbSlot->setSlotName( QString("FPGA virtual slot %1").arg(i) );
        addChild( bbSlot );
    }
}


//! Create a BBIS device for a FPGA carrier (e.g. F206)
/*!
  The device will have the default properties set by createProperties().
  It will have no BBIS slots (must be created by constructor of subclasses
  by calling createSlot() for each provided slot).

  \param instName initial instance name of device
  \param hwName Hardware name (e.g. "F206")
  \param driverName (e.g. "CHAMELEON")
  \param ifType upper interface type (e.g. Cpci).

  \todo instName really needed?
*/
ModBbisCham::ModBbisCham(
    QString instName,
    QString hwName,
    QString driverName,
    BusIfType ifType,
	const int devNo) :
    ModBbisChamBase( hwName, driverName, true, ifType )
{
    if( prop ) delete prop; // delete Device::prop
    prop = createProperties( devNo );
    setInstName( instName );
    _onboard = false;
    _mezz = false;
    this->chamSmbActive = false;
    
    // smbus interface
    this->smbusIf = new PciBusInterface( LocalBus, -1, -1, 0, 0, false );
	this->smbusIf->setInstName( QString("BusIf_SMB"));
	this->smbusIf->setEndianSwapping( true );
	this->smbusIf->setDescription("SMB BBIS Interface");
	addChild( this->smbusIf );
}


Device *ModBbisCham::create(bool withSubDevs)
{
	UTIL_UNREF_PARAM(withSubDevs);
	
    return new ModBbisCham( variant );
   
}

Device* ModBbisCham::create_onboard(bool withSubDevs)
{
	UTIL_UNREF_PARAM(withSubDevs);
	ModBbisPciChamProperties *p;
	ModBbisCham* bbischam = new ModBbisCham(QString("FPGA"), QString("CHAMELEON"), QString("CHAMELEON"), StdPci);
	WIZ_DYNAMIC_CAST(bbischam->getProp(), p, ModBbisPciChamProperties * );

    p->pciDevNo = 0x0;
    
	// create BBIS slots
    for( uint i=0; i<G_numSlots; i++ ){
        BbisSlot *bbSlot = new BbisSlot( ChameleonBus, i );
        bbSlot->setSlotName( QString("FPGA virtual slot %1").arg(i) );
        bbischam->addChild( bbSlot );
    }
    return bbischam;
}

QString
ModBbisCham::getDriverName( bool fullName, bool withSw )
{
    QString rv = "CHAMELEON";
	UTIL_UNREF_PARAM(withSw);
	UTIL_UNREF_PARAM(fullName);
	
    if( !variant.isEmpty() )
        rv += "_" + variant;
    return rv;
}

SwModuleList *
ModBbisCham::enumSwModules()
{
    if( lstSwModules == 0 ){
        lstSwModules = new SwModuleList;

        lstSwModules->add(
            makeStdDriverSwModule("CHAMELEON", variant,  true ));
    }
    return lstSwModules;
}

//! destructs ModBbisCham instance
ModBbisCham::~ModBbisCham()
{
    freeResources();
    delete prop;
    prop = 0;
}

void
ModBbisCham::freeResources()
{
    PciBusInterface *busIf = (PciBusInterface *)getParent();

    if( busIf == 0 ) return;

    busIf->pciSlots->freeAllOfOwner( this );

    Device::freeResources();
}

Device*
ModBbisChamBase::getDeviceAtSlot( int slotNo )
{
	Device *dev = NULL;
	BbisSlot *slot = bbSlot( slotNo );
	if (!slot)
		return dev;
	
    if( slot->firstChild() ){
        WIZ_DYNAMIC_CAST( slot->firstChild(), dev, Device * );
    }
    
    return dev;
}

Device::ApplyResult
ModBbisCham::applyProperties( DeviceProperties *newProp,
									QString &errMsg )
{
	if( newProp == 0 )
		newProp = prop;

	ModBbisPciChamProperties *_prop;

    wDebug(("ModBbisCham::ApplyProperties( newProp,...): instance=%s",
            getInstName().ascii() ));

	WIZ_DYNAMIC_CAST( newProp, _prop, ModBbisPciChamProperties * );

	ApplyResult ar;

	PciBusInterface *busIf = (PciBusInterface *)getParent();
	QString msg;
	ulong s;
	ResourceMng::IsAvailResult isAvail;
	ResourceMng::AllocResult alr = ResourceMng::AllocResOk;
	HwComponent *owner;

	// only allocate something when busIf provides a pciBusPath
	if( busIf && busIf->hasPciBusPath() ){
		
		isAvail = busIf->pciSlots->isAvail( _prop->slotNo, 1, &owner );

		if( isAvail != ResourceMng::Avail ){
			msg.sprintf("Cannot use slot %d\n",	_prop->slotNo );
			errMsg += msg;

			if( isAvail == ResourceMng::InUse ){
				msg.sprintf("Already used by device %s\n",
							(const char *)owner->getInstName() );
				errMsg += msg;
			}

			alr = busIf->pciSlots->alloc( 1, 1, s, this );
		}
		else {
			alr = busIf->pciSlots->alloc( 1, 1, s, this, _prop->slotNo );
		}

		if( alr == ResourceMng::AllocResFail ){
			msg.sprintf("Could not allocate any %s slot!\n",
						(const char *)busIfTypeMap.keyToStr( getBusIfType()));
			errMsg += msg;
			goto abort;
		}
		else if( s != (ulong)_prop->slotNo ){
			msg.sprintf("Alternative address 0x%lx allocated\n", s );
			_prop->slotNo = s;
			errMsg += msg;
			ar = ApplyPropAlternate;
		}
	}

	if ( busIf ) {
		if ( busIf->getPciDomainNo() != 0 ) {
			_prop->pciDomainNo = busIf->getPciDomainNo();
		}
	}

	// allocate name
	msg = QString::null;
	ar = Device::applyProperties( _prop, msg );
	errMsg += msg;
	if( ar == ApplyPropFail )
		goto abort;

	return ar;

 abort:
	freeResources();
	wDebug(("ModBbisCham::applyProperties failed errMsg=%s",
		   (const char *)errMsg));
	return ApplyPropFail;
}

/*! apply properties which are read before from system.dsc file */
Device::ApplyResult
ModBbisCham::applyProperties( DescriptorEntryDirectory *devDesc,
                                    QString &errMsg )
{
    DescriptorEntry *e, *eNext;
    DescriptorEntryUint32 *eUint32;
    ModBbisPciChamProperties *_prop;

    wDebug(("ModBbisCham::ApplyProperties( devDesc,...)" ));
    WIZ_ASSERT( prop );
    WIZ_DYNAMIC_CAST( prop, _prop, ModBbisPciChamProperties * );
    
    if( !devDesc ) return ApplyPropFail;
    
    // start instance counting with empty structures
    clearInstanceTrack();

    // scan all descriptor entries
    eNext=devDesc->firstChild();

    while( (e=eNext) ){
        bool entryHandled = true;
        QString key = e->path().section("/",1);
        wDebug(("key=%s", (const char *)key ));

		// if this descriptor describes a group, it is a directory!
		// check for this and parse the group member entries
		DescriptorEntry::DescType newType = eNext->getType();
		if ( ( newType == DescriptorEntry::Directory ) &&
			 ( key.startsWith("GROUP_") ) )
		{
			bool convSuccess = false;
			QString slotStr = key.section('_', -1);
			uint slotNum = slotStr.toUInt(&convSuccess);
			
			DescriptorEntryDirectory* dirDesc;
			WIZ_DYNAMIC_CAST( e, dirDesc, DescriptorEntryDirectory* );
			
			// scan all group members
			DescriptorEntry* currGrpEntr = dirDesc->firstChild();
			DescriptorEntry* grpEntry;
			while ( (grpEntry = currGrpEntr) )
			{
				bool groupMemHandled = true;
				QString grpMemKey = grpEntry->path().section("/",2);
				
				if ( grpMemKey == "GROUP_ID" ||
					 grpMemKey.startsWith("DEVICE_IDV2_") ) { 
					 	
					// add these entries to descriptor list for this slot
					// have to be handeled later with the corresponding mdis device
					if ( convSuccess )
					{
						if (map_unhandeledDescriptorEntries[slotNum] == NULL)
						{
							map_unhandeledDescriptorEntries[slotNum] = new DescriptorEntryDirectory(key);
						}
						WIZ_ASSERT(map_unhandeledDescriptorEntries[slotNum]);
						DescriptorEntryUint32* newDesc = new DescriptorEntryUint32((DescriptorEntryUint32&)*grpEntry);
						map_unhandeledDescriptorEntries[slotNum]->addChild( newDesc );
					}
			    }
				else {
					groupMemHandled = false;
				}
				currGrpEntr = dirDesc->getChildren().next();
				
				if( groupMemHandled == true )
				{
            		grpEntry->getParent()->removeChild(grpEntry);
				}
			}

			if ( !dirDesc->isEmpty() )
			{
				wDebug(("Could not parse Group %s; has invalid members!\n", dirDesc->getKey().ascii()));
				errMsg += QString("Error while reading the system configuration for FPGA device. Could not restore the system correctly\n");
			}
			
			// remove the whole directory from parent node
			eNext = dirDesc->nextSibling(false); // do not descend to children
			dirDesc->getParent()->removeChild(dirDesc);
			
			continue;
		} // if directory

	if( key == "PCI_DEVICE_NUMBER") 
	{
	    WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
	    _prop->pciDevNo = eUint32->getValue();
	    wDebug((" key PCI_DEVICE_NUMBER: value = %d", _prop->pciDevNo ));

	} else if ( key == "PCI_BUS_NUMBER" ) {

	    WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
	    _prop->pciBusNo = eUint32->getValue();
	    wDebug(("XXXXX key PCI_DEVICE_NUMBER: value = %d", _prop->pciBusNo ));

	} else if ( key == "PCI_DOMAIN_NUMBER" ) {
	    WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
	    _prop->pciDomainNo = eUint32->getValue();
	    wDebug((" key PCI_DOMAIN_NUMBER: value = %d", _prop->pciDomainNo ));
        }
#ifdef WIN_MDISWIZ_LIB	
        else if (key.startsWith("AUTOENUM_EXCLUDINGV2")) {
        	DescriptorEntryBinary *eBinary;
            WIZ_DYNAMIC_CAST( e, eBinary, DescriptorEntryBinary *);
            _prop->autoenum_excludingV2.duplicate( eBinary->getValue() );
        }
        else if (key.startsWith("AUTOENUM_EXCLUDING")) {
        	DescriptorEntryBinary *eBinary;
            WIZ_DYNAMIC_CAST( e, eBinary, DescriptorEntryBinary *);
            _prop->autoenum_excluding.duplicate( eBinary->getValue() );
        }
        else if ( key == "AUTOENUM" ) {
        	WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
            _prop->autoenum = eUint32->getValue();
        }
#endif
        else if ( key.startsWith("DEVICE_ID_") ||
            	  key.startsWith("DEVICE_IDV2_")) {
            	  	
        	// remember this key for later processing with the corresponding mdis device	
            QString key = e->getKey();
			bool convSuccess = false;
			QString slotStr = key.section('_', -1);
			uint slotNum = slotStr.toUInt(&convSuccess);
			wDebug(("add to slot num: %u", slotNum)); 
			if (convSuccess)
			{
				if (map_unhandeledDescriptorEntries[slotNum] == NULL)
				{
					map_unhandeledDescriptorEntries[slotNum] = new DescriptorEntryDirectory(key);
				}
				WIZ_ASSERT(map_unhandeledDescriptorEntries[slotNum]);
				DescriptorEntryUint32* newDesc = new DescriptorEntryUint32((DescriptorEntryUint32&)*e);
				map_unhandeledDescriptorEntries[slotNum]->addChild( newDesc, false ); 
			}
        }
        else if( key == "PCI_BUS_SLOT" ){
            WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
            _prop->slotNo = eUint32->getValue();
        }
        else if( key == "PCI_BUS_PATH" ) {
        	Q3MemArray<uchar> parentBusPath;
        	Q3MemArray<uchar> busPath;

            DescriptorEntryBinary *eBinary;
            PciBusInterface *busIf = (PciBusInterface *)getParent();
            WIZ_DYNAMIC_CAST( e, eBinary, DescriptorEntryBinary *);
            
            if( busIf && busIf->hasPciBusPath() ){
            	// get path from parent (for comparison)
                busIf->pciBusPath( -1, &parentBusPath );
				for (unsigned int i=0; i<parentBusPath.count(); i++) {
	      			wDebug(("parentPath[%d] = %d",i,parentBusPath[i]));
	      		}
                // onboard pciBus Ifs for Cham BBISes have dev ID on last entry!
                // trim this before check!
                if( _onboard )
	      			parentBusPath.resize( parentBusPath.size() - 1 );
	      		
	      		// get bus path from descriptor
	      		busPath = eBinary->getValue();
	      		
	      		for (unsigned int i=0; i<parentBusPath.count(); i++) {
	      			wDebug(("parentPath[%d] = %d",i,parentBusPath[i]));
	      		}
	      		for (unsigned int i=0; i<busPath.count(); i++) {
	      			wDebug(("busPath[%d] = %d",i,busPath[i]));
	      		}
	      		// compare with parent path
	      		if( busPath != parentBusPath ){
                    errMsg += "PCI_BUS_PATH descriptor/device discrepancy!\n";
                }
            }
        }
        else {
        	wDebug(("not handled: %s", key.ascii()));
            entryHandled = false;
        }

        eNext=e->nextSibling();

        // if entry has been handled, delete it
        if( entryHandled == true )
            e->getParent()->removeChild(e);

    }
	
    return Device::applyProperties( devDesc, errMsg );
}

DescriptorEntryDirectory* 
ModBbisChamBase::getUnhandeledDesc(uint slot)
{
	return  map_unhandeledDescriptorEntries[slot];
}

QString
ModBbisCham::getMainProperties()
{
	QString rv = "";
	ModBbisPciChamProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisPciChamProperties * );

	wDebug(( "ModBbisCham::getMainProperties() _prop->pciBusNo = %d", _prop->pciBusNo ));

	PciBusInterface *busIf = (PciBusInterface *)getParent();
	if( busIf ) {

		rv = getParent()->getMainProperties();

		if( busIf->hasPciBusPath() )
			rv += QString(" Slot %1").arg(_prop->slotNo);
		else {
			if( _prop->pciDomainNo > 0 )
				rv += QString(" Dom %1 /").arg(_prop->pciDomainNo);

			rv += QString(" Bus %1 / Dev %2").arg(_prop->pciBusNo).arg(_prop->pciDevNo);
		}
	}

	return rv;
}

//
// This is called by MdisDevice::createDesc
// to fill device specific descriptor entries
void
ModBbisCham::createSpecialDesc( DescriptorEntryDirectory *parentDesc )
{
    PciBusInterface *busIf = (PciBusInterface *)getParent();
    
    ModBbisPciChamProperties *_prop;
    WIZ_DYNAMIC_CAST( prop, _prop, ModBbisPciChamProperties * );
    DescriptorFactory dFact;

    wDebug(( "-> ModBbisCham::createSpecialDesc() ------------------ " ));
    
#ifdef WIN_MDISWIZ_LIB
	UTIL_UNREF_PARAM(parentDesc);
	UTIL_UNREF_PARAM(busIf);
	DescriptorEntry *e;

    // create the autoenum keys
    if ( _prop->autoenum != -1 ) {
    	e = dFact.create("AUTOENUM", _prop->autoenum);
    	e->setHidden( false );
    	parentDesc->addChild( e );
    }
    if ( _prop->autoenum_excludingV2.count() != 0 ) {
    	e = dFact.create("AUTOENUM_EXCLUDINGV2", _prop->autoenum_excludingV2);
    	e->setHidden( true );
    	parentDesc->addChild( e );
    }
    if ( _prop->autoenum_excluding.count() != 0 ) {
    	e = dFact.create("AUTOENUM_EXCLUDING", _prop->autoenum_excluding);
    	e->setHidden( true );
    	parentDesc->addChild( e );
    }
#else // WIN_MDISWIZ_LIB
    ModBbisSmbPciGen* smbBbisDev=0;
    // if chamSmbActive, set the pci props for cham smb controllers
    ModBbisSmbPciGenProperties *_smbProp=0;
    WIZ_ASSERT(busIf);

	// mezzanine?
	if( _mezz )
	{
		wDebug(( "	mezzanine given ----------------------" ));
		
		// no PCI bus path?
		if( !busIf->hasPciBusPath() ) {

			int pciDevNo;

	        // no bus path known
	        Q3MemArray<uchar> busPath;
	        Q3MemArray<uchar> busPath2;
	        busIf->pciBusPath( _prop->slotNo, &busPath );

			if( busPath.size() >= 1 )
				pciDevNo = busPath[0];
			else
				pciDevNo = _prop->pciDevNo;

			wDebug(( "ModBbisCham::createSpecialDesc: addChild(PCI_BUS_NUMBER): _prop->pciBusNo = %d", _prop->pciBusNo ));
			parentDesc->addChild( dFact.create("PCI_BUS_NUMBER", _prop->pciBusNo ));
			wDebug(( "ModBbisCham::createSpecialDesc: addChild(PCI_DEVICE_NUMBER): _prop->pciDevNo = %d", _prop->pciDevNo ));
	        parentDesc->addChild( dFact.create("PCI_DEVICE_NUMBER", pciDevNo ));

			// PCI_BUS_SLOT not necessary
			if (_prop->pciDomainNo != 0 ) {
	        	parentDesc->addChild( dFact.create("PCI_DOMAIN_NUMBER", _prop->pciDomainNo ));
	        }
	        
	        if (this->chamSmbActive)
	        {
	        	smbBbisDev = (ModBbisSmbPciGen*)this->smbusIf->firstChild();
				while (smbBbisDev)
				{
					WIZ_DYNAMIC_CAST(smbBbisDev->getProp(), _smbProp, ModBbisSmbPciGenProperties *);
					_smbProp->pciBusNo = 0;
					_smbProp->pciDevNo = pciDevNo;
					_smbProp->slotNo = _prop->slotNo;
					_smbProp->pciDomainNo = _prop->pciDomainNo;
					
					smbBbisDev = (ModBbisSmbPciGen*)this->smbusIf->nextChild();
				}
	    	}
	        
	    }
		// PCI bus path given
		else 
		{    	
	        Q3MemArray<uchar> busPath;
	        int sz;
	        uchar devNum;

	        busIf->pciBusPath( _prop->slotNo, &busPath );
	        sz = busPath.size();

	        devNum = busPath.at( sz - 1 ); // take last entry
	        busPath.resize( sz - 1 );

			if ( _prop->pciDomainNo != 0 ) {
				parentDesc->addChild( dFact.create("PCI_DOMAIN_NUMBER", _prop->pciDomainNo ) );
			}

            //parentDesc->addChild( dFact.create("PCI_BUS_PATH", busPath ));
            //parentDesc->addChild( dFact.create("PCI_BUS_SLOT", _prop->slotNo ));
	        //parentDesc->addChild( dFact.create("PCI_DEVICE_ID", _prop->pciDevNo ));
	        parentDesc->addChild( dFact.create("PCI_BUS_NUMBER", _prop->pciBusNo ));
	        parentDesc->addChild( dFact.create("PCI_DEVICE_NUMBER", _prop->pciDevNo ));

	        
	        if (this->chamSmbActive)
	        {
	        	smbBbisDev = (ModBbisSmbPciGen*)this->smbusIf->firstChild();
				while (smbBbisDev)
				{
					WIZ_DYNAMIC_CAST(smbBbisDev->getProp(), _smbProp, ModBbisSmbPciGenProperties *);
					_smbProp->pciBusPath.duplicate( busPath );
					_smbProp->pciDevNo = devNum;
					_smbProp->slotNo = _prop->slotNo;
					_smbProp->pciDomainNo = _prop->pciDomainNo;
					
					smbBbisDev = (ModBbisSmbPciGen*)this->smbusIf->nextChild();
				}
	    	}
	    }
	}
	// onboard?
    else if( _onboard ) {
		wDebug(( "	onboard given ----------------------" ));
		
		// no PCI bus path?
    	if( !busIf->hasPciBusPath() ) {
			wDebug(( "	busIf->hasPciBusPath() is false..." ));
	        // for now, take the last element of busPath...
	        Q3MemArray<uchar> busPath;
	        Q3MemArray<uchar> busPath2;
	        busIf->pciBusPath( -1, &busPath );
	        
	        WIZ_ASSERT( busPath.size() == 1 );

			wDebug(( "ModBbisCham::createSpecialDesc: addChild(PCI_BUS_NUMBER): %d ", _prop->pciBusNo ));
	        parentDesc->addChild( dFact.create("PCI_BUS_NUMBER", _prop->pciBusNo ));
	        parentDesc->addChild( dFact.create("PCI_DEVICE_NUMBER", busPath[0] ));
	        if (_prop->pciDomainNo != 0 )
	        	parentDesc->addChild( dFact.create("PCI_DOMAIN_NUMBER", _prop->pciDomainNo ));
                                 
	        if (this->chamSmbActive)
	        {
	        	smbBbisDev = (ModBbisSmbPciGen*)this->smbusIf->firstChild();
				while (smbBbisDev)
				{
					WIZ_DYNAMIC_CAST(smbBbisDev->getProp(), _smbProp, ModBbisSmbPciGenProperties *);
					_smbProp->pciBusNo = 0;
					_smbProp->pciDevNo = busPath[0];
					_smbProp->pciDomainNo = _prop->pciDomainNo;
					
					smbBbisDev = (ModBbisSmbPciGen*)this->smbusIf->nextChild();
				}
	    	}
	    	
	    }
		// PCI bus path given
		else {
	    	// onboard bus: device number is known   	
	        Q3MemArray<uchar> busPath;
	        int sz;
	        uchar devNum;

	        busIf->pciBusPath( -1, &busPath );
	        sz = busPath.size();

	        devNum = busPath.at( sz - 1 ); // take last entry
	        busPath.resize( sz - 1 );

	        if ( _prop->pciDomainNo != 0 ) {
				parentDesc->addChild( dFact.create("PCI_DOMAIN_NUMBER", _prop->pciDomainNo ) );
	        }

	        //parentDesc->addChild( dFact.create("PCI_BUS_PATH", busPath ));
	        //parentDesc->addChild( dFact.create("PCI_DEVICE_NUMBER", devNum ));
	        parentDesc->addChild( dFact.create("PCI_BUS_NUMBER", _prop->pciBusNo ));
	        parentDesc->addChild( dFact.create("PCI_DEVICE_NUMBER", _prop->pciDevNo ));

	        if (this->chamSmbActive)
	        {
	        	smbBbisDev = (ModBbisSmbPciGen*)this->smbusIf->firstChild();
				while (smbBbisDev)
				{
					WIZ_DYNAMIC_CAST(smbBbisDev->getProp(), _smbProp, ModBbisSmbPciGenProperties *);
					_smbProp->pciBusPath.duplicate( busPath );
					_smbProp->pciDevNo = devNum;
					_smbProp->pciDomainNo = _prop->pciDomainNo;
					
					smbBbisDev = (ModBbisSmbPciGen*)this->smbusIf->nextChild();
				}
	    	}
	    }
    }
	// neither mezzanine nor onboard
    else {
        wDebug(( "   non-onboard" ));
        
		// PCI bus path given?
        if( busIf->hasPciBusPath() ){
        	
            // PCI_BUS_PATH
            Q3MemArray<uchar> busPath;
            busIf->pciBusPath( -1, &busPath );
            
            if ( _prop->pciDomainNo != 0 ) {
				parentDesc->addChild( dFact.create("PCI_DOMAIN_NUMBER", _prop->pciDomainNo ) );
			}
	        parentDesc->addChild( dFact.create("PCI_BUS_PATH", busPath ));

			// PCI device number of PCIe endpoints is always 0
			if( busIf->getIsPciE() ){
				// PCI_DEVICE_NUMBER
				parentDesc->addChild( dFact.create("PCI_DEVICE_NUMBER", 0));		
			}
			else{
				// PCI_BUS_SLOT
				parentDesc->addChild( dFact.create("PCI_BUS_SLOT", _prop->slotNo ));
			}

			if (this->chamSmbActive)
	        {
	        	smbBbisDev = (ModBbisSmbPciGen*)this->smbusIf->firstChild();
				while (smbBbisDev)
				{
					WIZ_DYNAMIC_CAST(smbBbisDev->getProp(), _smbProp, ModBbisSmbPciGenProperties *);
					_smbProp->pciBusPath.duplicate( busPath );
					_smbProp->slotNo = _prop->slotNo;
					_smbProp->pciDomainNo = _prop->pciDomainNo;
					
					smbBbisDev = (ModBbisSmbPciGen*)this->smbusIf->nextChild();
				}
	    	}
		}
		// no PCI bus path
        else {
            // PCI_BUS_NUMBER
			wDebug(( "ModBbisCham::createSpecialDesc: no PCI BUS PATH. Create PCI_BUS_NUMBER = %d ", _prop->pciBusNo ));
            parentDesc->addChild( dFact.create("PCI_BUS_NUMBER", _prop->pciBusNo));
            // PCI_DEVICE_NUMBER
            parentDesc->addChild( dFact.create("PCI_DEVICE_NUMBER", _prop->pciDevNo));
            // PCI_DOMAIN_NUMBER
            if (_prop->pciDomainNo != 0 )
	        	parentDesc->addChild( dFact.create("PCI_DOMAIN_NUMBER", _prop->pciDomainNo ));
                                               
            if (this->chamSmbActive)
	        {
	        	smbBbisDev = (ModBbisSmbPciGen*)this->smbusIf->firstChild();
				while (smbBbisDev)
				{
					WIZ_DYNAMIC_CAST(smbBbisDev->getProp(), _smbProp, ModBbisSmbPciGenProperties *);
					_smbProp->pciBusNo = _prop->pciBusNo;
					_smbProp->pciDevNo = _prop->pciDevNo;
					_smbProp->pciDomainNo = _prop->pciDomainNo;
					
					smbBbisDev = (ModBbisSmbPciGen*)this->smbusIf->nextChild();
				}
	    	}
        }
	} // neither mezzanine nor onboard
     
	// clear the instance number data structures for uniqueness check
	this->clearInstanceTrack();
    for( uint i=0; i<G_numSlots; i++ ){
    	
        Device* dev = getDeviceAtSlot( i );
        if ( NULL != dev )
        {
        	
        	ulong devId = 0;
        	int instance = 0;
        	ulong id = 0;
        	DescriptorEntry* helpDescriptor = 0;
        	QString errMsg = "";
        	
        	if ( dev->type() != Device::MdisCham ) {
  				// no chameleon device; no descriptor creation necessary here!
  				continue;
			}
        	
        	// get device properties
        	DeviceProperties* prop = dev->getProp();
        	ModMdisChamProperties* mdisChamProp;
        	WIZ_DYNAMIC_CAST( prop, mdisChamProp, ModMdisChamProperties* );
        	
        	devId = mdisChamProp->getDeviceIdent().devId;
        	if (0 == devId) continue;
        	
        	// is this device member of a group?
    		bool isGroup = false;
    		if ( ( ModMdisChamProperties::ChamV2 == mdisChamProp->getType() ) &&
    		     ( 0 < mdisChamProp->getGrpList()->size() ) )
    		{
    			isGroup = true;
    		}
        	
        	if ( !isGroup )
        	{
	        	instance = mdisChamProp->getDeviceIdent().instanceNr;
        	}
        	else
        	{
        		instance = mdisChamProp->getDeviceIdent().index;
        	}
        	
        	id = (devId << 8) | instance;
        	if ( !isGroup && ((instance == -1) || (!this->addInstance(devId, id))) )
        	{
        		if (instance == -1)
        			errMsg += QString("ERROR: invalid instance number in device '%1'.\n").arg(dev->getInstName());
        		else
        			errMsg += QString("ERROR: a device '%1' with instance nr '%2' already exists in this system. \n").arg(devId).arg(instance);
        		
        		errMsg += "Device will not be added to the system!\n\n";
        		
        		// show error message
        		VMsgBox::informationOk( "Saving system information",
										"Error while saving the chameleon device properties",
										errMsg,
										G_mainWindow );
        		continue;
        	}
        	
        	
        	if ( ModMdisChamProperties::ChamV2 == mdisChamProp->getType() )
        	{
        		
        		// create group if necessery
        		DescriptorEntryDirectory* grpDesc = NULL;
        		if ( isGroup )
        		{
        			grpDesc = new DescriptorEntryDirectory( QString("GROUP_%1").arg(i) );
        			
        			// add the group id entry
        			ulong grpId = mdisChamProp->getGroupId();
        			if ( (grpId == 0) || (!this->addGroup(grpId)) )
        			{
        				if (grpId == 0)
        					errMsg += QString("ERROR: invalid group id in device '%1'.\n").arg(devId);
        				else
        					errMsg += QString("ERROR: group Id %1 for device %2 is not unique. \n").arg(grpId).arg(dev->getInstName() );
        				
        				errMsg += "Device will not be added to the system. \n\n";
        			}
        			
        			grpDesc->addChild( dFact.create( QString("GROUP_ID"), grpId ) );
        		
        			// add the main device as first group member (slot 0)
        			grpDesc->addChild( dFact.create( QString("DEVICE_IDV2_0"), id ));
        			
        			// add all group members 
        			for ( int grpIdx = 0; grpIdx < mdisChamProp->getGrpList()->size(); grpIdx++ )
        			{
        				DeviceChamIdent grpMemIdent = *(mdisChamProp->getGrpList()->at( grpIdx ));
        				ulong groupMemId = grpMemIdent.devId;
        				
        				instance = grpMemIdent.index; // use index for group members!
        				id = (groupMemId << 8) | instance;
        				
        				// user has no control on index value -> value check is not necessary
        				
        				grpDesc->addChild( dFact.create( 
        										QString("DEVICE_IDV2_%1").arg( grpIdx + 1 ), id ));
        			}
        			
        			// add group to parent 
        			helpDescriptor = grpDesc;
        		
        		}
        		else // no group
        		{
        			helpDescriptor = dFact.create(
        								QString("DEVICE_IDV2_%1").arg(i), 
										id );
        		}
        		
        	} // end if AutoIdChamV2
        	else if (ModMdisChamProperties::ChamOriginal == mdisChamProp->getType())
        	{
        		 helpDescriptor = dFact.create(
                                      QString("DEVICE_ID_%1").arg(i),
                                      id );
        	}
        	
        	if (errMsg == "")
        	{
        		wDebug(("no error for dev %s",dev->getInstName().ascii() ));
        		parentDesc->addChild( helpDescriptor );
        	}
        	else
        	{
        		// show error message
        		VMsgBox::informationOk( "Saving system information",
							"Error while saving the chameleon device properties",
							errMsg,
							G_mainWindow );
        	}
        	
        } // end if dev != NULL
        
    } // end for
    wDebug(( "<- ModBbisCham::createSpecialDesc() ------------------ " ));
#endif // WIN_MDISWIZ_LIB
}

DeviceProperties *
ModBbisCham::createProperties( void )
{
    return createProperties( 0x0 );
}

DeviceProperties *
ModBbisCham::createProperties( int devNo )
{
    //ModBbisPciMmodProperties *p = new ModBbisPciMmodProperties();
    ModBbisPciChamProperties * p = new ModBbisPciChamProperties();
    wDebug(("ModBbisCham::createProperties for %s", getModelName().ascii() ));

    p->dbgSettings.setModules(
        (1L<<DebugProperties::Driver) |
        (1L<<DebugProperties::BbisKernel) |
        (1L<<DebugProperties::Oss) |
        (1L<<DebugProperties::Desc));

    // set defaults
    p->slotNo = 2;
    p->pciBusNo = 0;
    p->pciDevNo = devNo;
    p->pciDomainNo = 0;

#ifdef WIN_MDISWIZ_LIB	
    p->autoenum = -1;
    p->autoenum_excludingV2.resize(0);
    p->autoenum_excluding.resize(0);
#endif
    
    return p;
}

void
ModBbisCham::copyProperties(
	const DeviceProperties *from,
	DeviceProperties *to )
{
	const ModBbisPciChamProperties *_from;
	ModBbisPciChamProperties *_to;

	WIZ_DYNAMIC_CAST( to, _to, ModBbisPciChamProperties * );
	WIZ_DYNAMIC_CAST( from, _from, const ModBbisPciChamProperties * );

	*_to = *_from;

#ifdef WIN_MDISWIZ_LIB
	_to->autoenum_excludingV2.duplicate(_from->autoenum_excludingV2);
	_to->autoenum_excluding.duplicate(_from->autoenum_excluding);
#endif
}

DevPropertiesDlg *
ModBbisCham::createPropDlg( QWidget *parent )
{
    DevPropertiesDlg *dp;
    QWidget *defTab=0;
	DevSmbTab* smbTab;
	MdiswizView* wizView;
	
    dp = new DevPropertiesDlg( parent, this );
  
	dp->addTab( (smbTab = new DevSmbTab(true, parent, dp)), "SMBus" );
	dp->showPage( smbTab );
	
	// cast to MdiswizView
	WIZ_DYNAMIC_CAST( parent, wizView, MdiswizView* );
	
	// establish connection between dlg and hw models
	connect( smbTab, SIGNAL( activateChamSmb( bool, int, MdiswizView* ) ), this, SLOT(activateChamSmb( bool, int, MdiswizView* )) );

	// PCIe devices with known PCI bus paths requires no further location settings,
	// don't show properties dlg
	// ts@men: here the tab "Mezzanine Settings" for MEZZ_CHAM devices is shown
	PciBusInterface *busIf;
	WIZ_DYNAMIC_CAST( getParent(), busIf, PciBusInterface * );
	if( (busIf->getIsPciE() && !busIf->hasPciBusPath()) || (!busIf->getIsPciE()) ){
	    dp->addTab( (defTab = new ModBbisPciMmodPropTab(parent, dp)),
	                QString("%1 Settings").arg( busIfTypeMap.keyToStr(
	                    getBusIfType() )));
	}

    dp->setButtons();
    if ( defTab )
    	dp->showPage( defTab );

    wDebug(("ModBbisCham::createPropDlg dp=%p", dp ));
    return dp;
}

void 
ModBbisCham::activateChamSmb( bool active, int numDevices, MdiswizView *wizView )
{
	MdisWizListView *listView=0;
	ComponentViewItem *cvi=0;
	HwComponent* bbisDev=0;
	HwComponent* oldBbisDev=0;
	int i=0;
	
	if (active)
	{
		this->chamSmbActive = true;
		
		bbisDev = this->smbusIf->firstChild();
		for (i=0; i<numDevices; i++)
		{
			if (!bbisDev)
				wizView->activeItem()->attachDev( this->smbusIf, QString("SMBPCI_16Z001") );
			bbisDev = this->smbusIf->nextChild();
		}
		
		// remove unneeded devices
		while (bbisDev)
		{
			WIZ_DYNAMIC_CAST(wizView->getListView(), listView, MdisWizListView*);
			cvi = listView->findItemByInstName( bbisDev->getInstName() );
			if (cvi) {
				delete cvi; // delete gui object
			}
			oldBbisDev = bbisDev; // remember
			bbisDev = this->smbusIf->nextChild();
			this->smbusIf->removeChild(oldBbisDev);
		}
	
	}
	else if ( !active && (this->chamSmbActive) )
	{
		this->chamSmbActive = false;
		WIZ_DYNAMIC_CAST(wizView->getListView(), listView, MdisWizListView*);
			
		// cleanup 
		bbisDev = this->smbusIf->firstChild();
		while (bbisDev)
		{
			cvi = listView->findItemByInstName( bbisDev->getInstName() );
			if (cvi) {
				delete cvi; // delete gui object
			}
			
			oldBbisDev = bbisDev; // remember
			bbisDev = this->smbusIf->nextChild();
			this->smbusIf->removeChild(oldBbisDev); // delete corresponding devices
		}
	}
}

//! Check a pair of instance number and device Id for existance and remember it
/*!
    Check if the given pair of instance number and device id already exists in 
    this chameleon device. If not, remember it. 

  \param devId Device Id 
  \param instNr instance Number
  \return true if no such pair exists for device, false if pair already exists
*/
bool 
ModBbisChamBase::addInstance( ulong devId, int instNr )
{
	// check if there already exists a device of this type with this inst nr
	wDebug(("add instance to devId %lu",devId));
	
	Q3ValueList<int>& devList = map_usedInstanceNrs[devId];
	Q3ValueListIterator<int> instIter;
	for ( instIter = devList.begin();
		  instIter != devList.end();
		  instIter++ )
	{
		if ( *instIter == instNr )
		{
			return false;
		}
	}
	
	// add instance nr to list
	devList.push_back( instNr );
	
	return true;
}

//! Check if group id already exists in device and remember it
/*!
    Check if a group with the given group id already exists in this device. 
    If not, remember it. 

  \param grpId group Id 
  \return true if the group Id does not already exist, false if it already exists
*/	
bool 
ModBbisChamBase::addGroup( ulong grpId )
{
	// check if this group id already exists for this device
	//bool exists = false;
	Q3ValueListIterator<ulong> grpIter;
	for ( grpIter = list_usedGrpIds.begin(); 
		  grpIter != list_usedGrpIds.end();
		  grpIter++)
    {
		if ( *grpIter == grpId ) 
		{
			wDebug(("found grp Id %lu in list",grpId));
			return false;
		}
    }
	
	// add the group id to the list
	list_usedGrpIds.push_back( grpId );
	wDebug(("added grp Id %lu to list",grpId));
	
	return true;
}
	
//! Clear internal data structures for instance num and group Id management
/*!
    Clear list of used device Id / instance number pairs and list of 
    used group Ids. 
*/	
void 
ModBbisChamBase::clearInstanceTrack()
{
	list_usedGrpIds.clear();
	
	QMap<ulong,Q3ValueList<int> >::iterator iter;
	for ( iter = map_usedInstanceNrs.begin();
		  iter != map_usedInstanceNrs.end();
		  iter++ )
	{
		Q3ValueList<int>* list = &(*iter);
		list->clear();
	}
	map_usedInstanceNrs.clear();
		  	
}

//! Add instance nums and group ids of all devices except the given one
/*!
    Calls addInstance() and addGroup() for all existing chameleon mdis devices
    except the given one.
    Called when propertiesDlg::applyProperties is called for a device to check
    the instance num settings for this device. 

  \param mdisChamDevice The mdis device that is excluded from handling 
*/	
void 
ModBbisChamBase::setOtherInstances( ModMdisCham* mdisChamDevice )
{
	int numSlots = getChildren().count();
	wDebug(("num slots: %i",numSlots));
	
	for (int i=0; i<numSlots; i++)
	{
		Device* dev = getDeviceAtSlot( i );
		if (!dev) continue;
		
		if ( dev->type() != Device::MdisCham ) {
  			// no chameleon device
  			continue;
		}
		
		ModMdisCham* currChamDev;
		WIZ_DYNAMIC_CAST( dev, currChamDev, ModMdisCham* );
		
		// do settings for all devices except the given one
		if ( currChamDev == mdisChamDevice ) continue;
			
		// get properties of this device
		ModMdisChamProperties* mdisProp;
		DeviceProperties* devProp = currChamDev->getProp();
		WIZ_DYNAMIC_CAST( devProp, mdisProp, ModMdisChamProperties* );

		// set the instance numbers and group id for the device
		addInstance( mdisProp->deviceIdent.devId, mdisProp->deviceIdent.instanceNr );
		
		// add group id if necessary
		if ( (mdisProp->getType() == ModMdisChamProperties::ChamV2) &&
			 (mdisProp->grpMemList->size() > 0 ) )
		{
			if ( mdisProp->getGroupId() != 0 ) {
				addGroup( mdisProp->getGroupId() );
			}
			
			for (int j=0; j<mdisProp->grpMemList->size(); j++)
			{
				DeviceChamIdent* devIdent = &(*mdisProp->grpMemList->at(j));
				addInstance( devIdent->devId, devIdent->instanceNr );
			}
		}
	}
}

ComponentViewItem *
ModBbisCham::createGui(MdiswizView *wizView, Q3ListViewItem *parent)
{
	wDebug(("ModBbisCham::createGui"));
	BbisDeviceViewItem *bv;
	HwComponent *bbSlot, *devHw;
	Device *dev;
	unsigned int i=0;

	// create special DeviceViewItem (allow bbis children!) 
	bv = new BbisChamDeviceViewItem( wizView, parent, getInstName() );
	WIZ_ASSERT(bv);
	
	// create GUI components for all MDIS children
	bbSlot = firstChild();
	for( i=0; i<G_numSlots; i++ ){
		if (!bbSlot) {
			bbSlot = nextChild();
			continue;
		}
			
		for( devHw=bbSlot->firstChild(); devHw; devHw=bbSlot->nextChild() ){
			WIZ_DYNAMIC_CAST( devHw, dev, Device *)
			dev->createGui( wizView, bv );
		}
		bbSlot = nextChild();
	}
	bv->setDevice( this );
	wDebug(("leave ModBbisCham::createGui"));
	return bv;
}

// -----------------------------------------------------------------

//! create generic mezzanine chameleon PCI device
ModBbisCham4Mezz::ModBbisCham4Mezz() :
    ModBbisCham(QString("CHAM"), QString("MEZZ_CHAM"), QString("CHAMELEON"), PciOrCham)
{

    // create BBIS slots
    for( uint i=0; i<G_numSlots; i++ ){
        BbisSlot *bbSlot = new BbisSlot( ChameleonBus, i );
        bbSlot->setSlotName( QString("FPGA internal slot %1").arg(i) );
        addChild( bbSlot );
    }

    // this is valid for every CPCI board
    variant = "PCITBL"; 

    setDescription("Generic Chameleon FPGA card: F206/208/210/215,G215,P506 etc.");

	// use pci bus path and device number for pci identification
	_mezz = true;
}

Device *ModBbisCham4Mezz::create(bool withSubDevs)
{
    withSubDevs = withSubDevs;
    return new ModBbisCham4Mezz();
}
SwModuleList *
ModBbisCham4Mezz::enumSwModules()
{
    SwModuleList *natLst;

    if( lstSwModules == 0 ){
        lstSwModules = new SwModuleList;

        if( lstSwModules ) {
            // BBIS Chameleon driver
            lstSwModules->add(
                makeStdDriverSwModule("CHAMELEON", "PCITBL",  true ));

            // Native Chameleon driver
            if( (natLst = CpuDevice::enumSwModulesForExternalPackage(
                     "CHAMELEON") ))
               lstSwModules->add( natLst );
        }
    }

    return lstSwModules;
}
