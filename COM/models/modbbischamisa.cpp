/***************************************************************************
 */
/*!       \file  ModBbisChamIsaisa.cpp
 *       \brief  Implementation of Chameleon ISA FPGA classes
 *      \author  Klaus.Popp@men.de
 *        $Date: 2014/07/18 15:12:10 $
 *    $Revision: 2.5 $
 *
 *        \todo needs cleanup
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbischamisa.cpp,v $
 * Revision 2.5  2014/07/18 15:12:10  dpfeuffer
 * R: SC24 device address 0xffffffffe000e000 displayed
 * M: shown SC24 device address fixed to 0xe000e000
 *
 * Revision 2.4  2013/03/04 13:02:58  dpfeuffer
 * R: open existing project with SC24 CPU doesn't work
 * M: getUnhandeledDesc: using class ModBbisChamBase instead of class ModBbisCham
 *
 * Revision 2.3  2012/04/25 17:40:05  ts
 * R: wrong .mak file driver_isa_.mak created when project for SC24 generated
 * M: corrected extension in BBIS mak file to "isa", not "isa_"
 *
 * Revision 2.2  2011/09/26 08:42:52  dpfeuffer
 * R:1. base class for ModBbisCham (PCI) and ModBbisChamIsa required
 *   2. ModBbisChamIsa not full implemented
 * M:1. some member functions removed (now in ModBbisChamBase)
 *   2. ModBbisChamIsa member function added/fixed
 *
 * Revision 2.1  2011/02/14 14:56:33  CRuff
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2011 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3ValueList>
#include <Q3MemArray>

#include "modbbischamisa.h"
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

//! Create an Cham BBIS device which resides on a CPU board directy
 // (Localbus)
/*!
  The device will have the default properties set by createProperties().

  It will have 16 BBIS slots (numbered 0..15) of type "ChameleonBus".

  Attention: no GUI functionality is implemented yet for this class.
             Is currently only used as model for mdiswiz_lib.

*/
ModBbisChamIsa::ModBbisChamIsa( const QString &_variant ) :
    ModBbisChamBase( "CHAMELEON_ISA", "CHAMELEON_ISA", false, LocalBus )
{
    setDescription( "Container device for chameleon ISA FPGA units" );

    variant = _variant;
    setHwName( getDriverName( false, false ) );

    if( prop ) delete prop; // delete Device::prop
    prop = createProperties();
    this->chamSmbActive = false;

	// create BBIS slots
    for( uint i=0; i<G_numSlots; i++ ){
        BbisSlot *bbSlot = new BbisSlot( ChameleonBus, i );
        bbSlot->setSlotName( QString("FPGA virtual slot %1").arg(i) );
        addChild( bbSlot );
    }
}


Device *ModBbisChamIsa::create(bool withSubDevs)
{
	UTIL_UNREF_PARAM(withSubDevs);
	
    return new ModBbisChamIsa( variant );
   
}

//! Set address parameters
/*!
  ISA special call. Can be used by CPU models to set a fixed device
  address/size for an onboard component 

*/  
void 
ModBbisChamIsa::setIsaAddress( ulong devAddr, bool devAddrIsIo )
{
	ModBbisChamIsaProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisChamIsaProperties * );

	_prop->deviceAddress = devAddr;
	_prop->devAddrIsIo = devAddrIsIo;
	_prop->irqNum = -1;  // invalid value
}

//! Set IRQ number
/*!  ISA special call. Can be used by CPU models to set a fixed IRQ number for
     an onboard component. \n 

     If the number is invalid (<0) then the descriptor entry IRQ_NUMBER will
     not be generated.
*/  
void 
ModBbisChamIsa::setIrqNumber( long irqNumber )
{
	ModBbisChamIsaProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisChamIsaProperties * );

	_prop->irqNum = irqNumber;
}

QString
ModBbisChamIsa::getDriverName( bool fullName, bool withSw )
{
    QString rv = "CHAMELEON_ISA";
	UTIL_UNREF_PARAM(withSw);
	UTIL_UNREF_PARAM(fullName);
	
    if( !variant.isEmpty() )
        rv += "_" + variant;
    return rv;
}

SwModuleList *
ModBbisChamIsa::enumSwModules()
{
    if( lstSwModules == 0 ){
        lstSwModules = new SwModuleList;

        lstSwModules->add(
            makeStdDriverSwModule("CHAMELEON", QString("ISA") + variant,  true ));
    }
    return lstSwModules;
}

//! destructs ModBbisChamIsa instance
ModBbisChamIsa::~ModBbisChamIsa()
{
    freeResources();
    delete prop;
    prop = 0;
}

void
ModBbisChamIsa::freeResources()
{
	BusInterface *busIf = (BusInterface *)getParent();
	if( busIf == 0 ) return;
	
	Device::freeResources();
}

Device::ApplyResult
ModBbisChamIsa::applyProperties( DeviceProperties *newProp,
									QString &errMsg )
{
	if( newProp == 0 )
		newProp = prop;

	ModBbisChamIsaProperties *_prop;
	ModBbisChamIsaProperties *_newProp;

    wDebug(("ModBbisChamIsa::ApplyProperties( newProp,...): instance=%s",
            getInstName().ascii() ));

	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisChamIsaProperties * );
	WIZ_DYNAMIC_CAST( newProp, _newProp, ModBbisChamIsaProperties * );

	*_prop = *_newProp;

	ApplyResult ar;

	// allocate name
	ar = Device::applyProperties( _prop, errMsg );
	if( ar == ApplyPropFail )
		goto abort;

	return ar;

 abort:
	freeResources();
	wDebug(("ModBbisChamIsa::applyProperties failed errMsg=%s",
		   (const char *)errMsg));
	return ApplyPropFail;
}

/*! apply properties which are read before from system.dsc file */
Device::ApplyResult
ModBbisChamIsa::applyProperties( DescriptorEntryDirectory *devDesc,
                                    QString &errMsg )
{
    DescriptorEntry *e, *eNext;
    DescriptorEntryUint32 *eUint32;
    ModBbisChamIsaProperties *_prop;

    wDebug(("ModBbisChamIsa::ApplyProperties( devDesc,...)" ));
    WIZ_ASSERT( prop );
    WIZ_DYNAMIC_CAST( prop, _prop, ModBbisChamIsaProperties * );
    
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

		if( key == "DEVICE_ADDR" ) {
        	WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
        	_prop->deviceAddress = eUint32->getValue();
        }
        else if ( key == "DEVICE_ADDR_IO" )
        {
        	WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
        	_prop->devAddrIsIo = eUint32->getValue();
        }
        else if ( key == "IRQ_NUMBER" )
		{
			WIZ_DYNAMIC_CAST( e, eUint32, DescriptorEntryUint32 *);
			_prop->irqNum = eUint32->getValue();
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

QString
ModBbisChamIsa::getMainProperties()
{
	QString rv;
	ModBbisChamIsaProperties *_prop;
	WIZ_DYNAMIC_CAST( prop, _prop, ModBbisChamIsaProperties * );

	rv += QString(" Device address: 0x") + QString::number((ulong)_prop->deviceAddress,16);
	if ( _prop->devAddrIsIo ) {
		rv += QString(" (IO)");
	}

	return rv;
}

//
// This is called by MdisDevice::createDesc
// to fill device specific descriptor entries
void
ModBbisChamIsa::createSpecialDesc( DescriptorEntryDirectory *parentDesc )
{
    
    ModBbisChamIsaProperties *_prop;
    WIZ_DYNAMIC_CAST( prop, _prop, ModBbisChamIsaProperties * );
    DescriptorFactory dFact;
    DescriptorEntry *e;

    wDebug(( "ModBbisChamIsa::createSpecialDesc()" ));
    
#ifdef WIN_MDISWIZ_LIB
	UTIL_UNREF_PARAM(parentDesc);

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
#endif // WIN_MDISWIZ_LIB

    // device address on local bus
	e = dFact.create("DEVICE_ADDR", _prop->deviceAddress);
	e->setHidden( false );
	parentDesc->addChild( e );

	e = dFact.create("DEVICE_ADDR_IO", _prop->devAddrIsIo);
	e->setHidden( false );
	parentDesc->addChild( e );

	if( _prop->irqNum > 0 ){
		e = dFact.create("IRQ_NUMBER", _prop->irqNum);
		e->setHidden( false );
		parentDesc->addChild( e );
	}
}

DeviceProperties *
ModBbisChamIsa::createProperties()
{
    ModBbisChamIsaProperties * p = new ModBbisChamIsaProperties();
    wDebug(("ModBbisChamIsa::createProperties for %s", getModelName().ascii() ));

    p->dbgSettings.setModules(
        (1L<<DebugProperties::Driver) |
        (1L<<DebugProperties::BbisKernel) |
        (1L<<DebugProperties::Oss) |
        (1L<<DebugProperties::Desc));

    // set defaults
    p->deviceAddress = 0x0;
    p->devAddrIsIo = 0;

#ifdef WIN_MDISWIZ_LIB	
    p->autoenum = -1;
    p->autoenum_excludingV2.resize(0);
    p->autoenum_excluding.resize(0);
#endif
    
    return p;
}

void
ModBbisChamIsa::copyProperties(
	const DeviceProperties *from,
	DeviceProperties *to )
{
	const ModBbisChamIsaProperties *_from;
	ModBbisChamIsaProperties *_to;

	WIZ_DYNAMIC_CAST( to, _to, ModBbisChamIsaProperties * );
	WIZ_DYNAMIC_CAST( from, _from, const ModBbisChamIsaProperties * );

	*_to = *_from;

#ifdef WIN_MDISWIZ_LIB
	_to->autoenum_excludingV2.duplicate(_from->autoenum_excludingV2);
	_to->autoenum_excluding.duplicate(_from->autoenum_excluding);
#endif
}

