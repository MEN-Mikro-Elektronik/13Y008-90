/***************************************************************************
 */
/*!       \file  modbbissmbpcigen.h
 *       \brief  Generic class for SMB PCI BBIS
 *      \author	 Christine.Ruff@men.de
 *		  $Date: 2011/04/01 09:40:16 $
 *    $Revision: 2.5 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbissmbpcigen.h,v $
 * Revision 2.5  2011/04/01 09:40:16  CRuff
 * R: 1. ModBbisSmbPciGenProperties needs to have consistent pci properties
 * M: 1. inherit from ModBbisPciMmodProperties to have consistent pci property
 *       names
 *
 * Revision 2.4  2011/02/14 14:06:47  CRuff
 * R: 1. smbpci bbis for 16Z001 needed in IO variant
 *    2. compiler warnings because of overload of non-virtual functions
 * M: 1a) added flag isIo
 *    1b) added parameter isIo to constructor
 *    2. make createProperties(), copyProperties() and freeResources() virtual
 *       protected
 *
 * Revision 2.3  2010/07/06 08:25:03  CRuff
 * R: implementation of PCI Domains
 * M: added new member pciDomainNo to ModBbisSmbPciGenProperties class
 *
 * Revision 2.2  2010/05/31 09:13:20  CRuff
 * R: 1. For MDIS Wizard library, no object hierarchy is available
 * M: 1. hold SMBDEV_NAME list in ModBbisSmbPciGenProperties for MDISWIZ library
 *
 * Revision 2.1  2010/05/19 17:31:04  CRuff
 * R: 1. adapt ModBbisSmbPciGenProperties to SMBPCI driver
 *    2. The Z001 variant of ModBbisSmbPciGen uses the pci bus path of the parent
 *       ModBbisCham
 * M: 1. added new members pciBusSlot and smbBusWait
 *    2. added method declaration setPciBusPath() to copy the pci path from the
 *       parent device
 *
 * Revision 2.0  2010/01/22 11:29:31  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.2  2009/12/07 10:00:35  CRuff
 * R: Implementation not finished, but interrupted
 * M: Intermediate checkin
 *
 * Revision 1.1  2009/09/03 11:37:06  CRuff
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODBBISSMBPCIGEN_H
#define MODBBISSMBPCIGEN_H

#include <Q3MemArray>

#include "hwcomponent.h"
#include "descriptor.h"
#include "modbbispcigen.h"
#include "modbbispcimmod.h"

//! Device properties for ModBbisSmbPciGen 
class ModBbisSmbPciGenProperties : public ModBbisPciMmodProperties
{
public:
	Q3MemArray<uchar> 	pciBusPath;	//!< PCI bus path to the device
	int 			pciFuncNo;    	//!< PCI function number of PCI bus on carrier
	int 			bar;			//!< bar of smbus control register
	unsigned long 	offs; 			//!< offset of smbus controller register (def=0)
	
	int 			smBusNo;	    //!< SMBus number
	int 			smbPollAlertFreq; //!< SMB alert polling frequency
	int 			smbBusyWait;
	bool 			idCheck;

#ifdef WIN_MDISWIZ_LIB
    QStringList     smbDevNameList;
#endif
	
};

class BbisSlotSmb : public BbisSlot
{
public:
	BbisSlotSmb( BusIfType _ifType, int _slotNum,
			     bool _bbSlotSpecRequired=false, const QString &_slotName = 0) 
    : BbisSlot( _ifType, _slotNum, _bbSlotSpecRequired, _slotName ) {}
    	
    // add child to list of children (and set child's parent)
	virtual void addChild( HwComponent *newComp );	
   
};

//! class for generic SMB PCI BBIS devices
class ModBbisSmbPciGen : public BbisDevice
{
public:
	ModBbisSmbPciGen( const QString &variant=0, bool isIo=false );
	~ModBbisSmbPciGen();
	
	virtual Device *create(bool withSubDevs=true);

	DevPropertiesDlg *createPropDlg( QWidget *parent );
    QString getMainProperties();
    SwModuleList* enumSwModules();
        
    void createSpecialDesc( DescriptorEntryDirectory *parentDesc );    
    ApplyResult applyProperties( DescriptorEntryDirectory *devDesc,
										 QString &errMsg );
	ApplyResult applyProperties( DeviceProperties *newProp, 
	                                     QString &errMsg );
	                                     
	QString getDriverName( bool fullName, bool withSw );
	
	void setPciBusPath( Q3MemArray<uchar>& 	pciBusPath );
	 
	QString variant; 
	bool isIo;

protected:

	virtual DeviceProperties *createProperties( void);
	virtual void copyProperties( const DeviceProperties *from,
								 DeviceProperties *to );
	virtual void freeResources();
};


class ModBbisSmbPciLinux: public ModBbisSmbPciGen
{
public:
	ModBbisSmbPciLinux( const QString &variant=0 );
	~ModBbisSmbPciLinux();
	
	DeviceProperties *createProperties( void );
	DevPropertiesDlg *createPropDlg( QWidget *parent );
	void copyProperties(
        const DeviceProperties *from,
        DeviceProperties *to );
    QString getMainProperties();
    void freeResources();
    SwModuleList* enumSwModules();
    
    void createSpecialDesc( DescriptorEntryDirectory *parentDesc );
    ApplyResult applyProperties( DescriptorEntryDirectory *devDesc, QString &errMsg );
    ApplyResult applyProperties( DeviceProperties *newProp, 
	                                     QString &errMsg );
	                                     
	QString getDriverName( bool fullName, bool withSw );
	
};


#endif

