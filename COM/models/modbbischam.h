/***************************************************************************
 */
/*!       \file  modbbischam.h
 *       \brief  BBIS class for the CHAMELEON FPGAs
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2014/08/22 15:56:17 $
 *    $Revision: 2.10 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbischam.h,v $
 * Revision 2.10  2014/08/22 15:56:17  dpfeuffer
 * R:1. inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 *   2. some unnecessary models
 * M:1. PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *   2. useless models removed
 *
 * Revision 2.9  2014/07/18 15:12:08  dpfeuffer
 * R: erroneous PMC support, missing ComExpress support, general maintenance
 * M: intermediate check-in during development
 *
 * Revision 2.8  2013/11/27 10:44:16  ts
 * R: F213 was unavailable in the Wizard
 * M: added F213
 *
 * Revision 2.7  2013/03/04 13:02:55  dpfeuffer
 * R: open existing project with SC24 CPU doesn't work
 * M: getUnhandeledDesc: using class ModBbisChamBase instead of class ModBbisCham
 *
 * Revision 2.6  2011/09/26 08:42:49  dpfeuffer
 * R:1. base class for ModBbisCham (PCI) and ModBbisChamIsa required
 *   2. new model G215
 * M:1. ModBbisChamBase implemented with common chameleon stuff
 *   2. new model class ModBbisG215 added
 *
 * Revision 2.5  2010/06/21 17:01:17  CRuff
 * R: version 2.4 has errors
 * M: undo changes of version 2.4;
 *    add new method create_onboard for this functionality
 *
 * Revision 2.4  2010/06/21 11:02:08  CRuff
 * R: Explicit creation of non-onboard devices necessary
 * M: add parameter onboard to signature of create(); default val = true
 *
 * Revision 2.3  2010/05/31 09:04:35  CRuff
 * R: AUTOENUM keys not needed for traditional GUI application
 * M: only handle AUTOENUM keys if MDIS Wizard is used as library
 *
 * Revision 2.2  2010/05/20 15:49:13  CRuff
 * R: MDIS Wizard must handle all keys that are supported by Chameleon BBIS
 * M: Added support for the keys AUTOENUM, AUTOENUM_EXCLUDING and -V2
 *
 * Revision 2.1  2010/05/19 17:01:43  CRuff
 * R: 1. need to support 16Z001 SMB controllers as children of chameleon bbis
 * M: 1a) added declaration of virtual method createGui() in chameleon BBIS
 *    1b) added flag chamSmbActive in chameleon BBIS
 *    1c) added declaration of slot activateChamSmb() in chameleon BBIS
 *    1d) added new class ModBbisChamProperties
 *
 * Revision 2.0  2010/01/22 11:28:52  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.8  2009/08/14 15:10:18  CRuff
 * R: no support for P512
 * M: add support for P512 PMC
 *
 * Revision 1.7  2009/04/21 15:08:36  CRuff
 * R: 1. support chameleon mezzanine cards
 *    2. support new bbises F215, F216
 *    3. support new mezzanines P506, P507, P599
 * M: 1. added flag _mezz to ModBbisCham
 * 2. added classes ModBbisF215, ModBbisF216
 * 3. added classes ModBbisP506, ModBbisP507, ModBbisP599
 *
 * Revision 1.6  2009/04/01 15:10:25  CRuff
 * R: 1.Chameleon V2 support
 * M: 1.a) added new methods to keep track of used instance nrs and group ids
 *      b) added data structures to keep track of used instance nrs and group ids
 *         in a chameleon device
 *
 * Revision 1.5  2008/10/23 18:04:52  channoyer
 * R: During build we get an error: extra qualification
 * M: Remove extra qualification
 *
 * Revision 1.4  2006/11/30 11:28:13  cs
 * added classes for F208, F210
 *
 * Revision 1.3  2005/07/15 15:47:24  ub
 * enhanced to support F206
 *
 * Revision 1.2  2005/03/09 13:49:28  ub
 * new model "ModBbisF206"
 *
 * Revision 1.1  2004/06/08 17:25:59  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODBBISCHAM_H
#define MODBBISCHAM_H

#include <Q3ValueList>

#include "hwcomponent.h"
#include "descriptor.h"
#include "modbbispcimmod.h"
#include "modbbispcimmodproptab.h"
#include "util.h"

class ModMdisCham;

//! BBIS base class for CHAMELEON FPGA bbis and CHAMELEON FPGA ISA bbis
/*!
  This class specifies common behaviour used from the child classes.
*/
class ModBbisChamBase : public BbisDevice
{
	Q_OBJECT
public:
	ModBbisChamBase( const QString &_hwName, const QString &_driverName,
		bool isRemoveable, BusIfType _ifType ) :
			BbisDevice( _hwName, _driverName, isRemoveable, _ifType ){}
	
    // methods to track the uniqueness of instance numbers and group Ids of mdis devices
    
    // add a pair of deviceId and instance number to the internal list
    bool addInstance( ulong devId, int instNr );
    // add a group id to internal list
	bool addGroup( ulong grpId );
	// clear the internal data structures
	void clearInstanceTrack();
	// go through all mdisCham devices and set instance nrs and group ids 
	// except the given one (used to validate settings in properties tab)
	void setOtherInstances( ModMdisCham* mdisChamDevice );

    // get the unhandeled descriptior entries that were read for the bbis device,
    // but have to be handeled in the mdis cham device
    DescriptorEntryDirectory* getUnhandeledDesc(uint slot);

protected:
	Device* getDeviceAtSlot( int slotNo );

    // descriptors that have to be handeled in the mdis device
    QMap< uint, DescriptorEntryDirectory* > map_unhandeledDescriptorEntries;

private:
    // used instance numbers for mdis devices
    QMap<ulong, Q3ValueList<int> > map_usedInstanceNrs;
    // used group ids for mdis devices
    Q3ValueList<ulong> list_usedGrpIds;
};

//! BBIS class for CHAMELEON FPGA bbis
/*!
  This BbisDevice must be attached by the CpuDevice to a StdPci bus interface.
  The bus interface's pciBusPath() routine must return the pciPath of the
  FPGA (when called with slotN as 1).

  Chameleon BBIS driver should be extended to handle the full pciPath. Up
  to now, it is assumed that FPGA sits on bus 0.

  It provides 16 pseudo slots. MDIS devices can be connected to each of
  the slots.

  The MDIS device shall have
  - a "bbslot" specification telling mdiswiz that it can connect to all
    "CHAMELEON" BBIS models
  - a <chamunitid> tag that tells mdiswiz which FPGA module code it needs.

  Based on the connected devices, this class automatically creates
  the DEVICE_ID_x descriptor entries. It assumes that the user knows
  which FPGA units are present in his FPGA.

*/
class ModBbisCham : public ModBbisChamBase
{
	Q_OBJECT
public:
	ModBbisCham( const QString &variant=0, const int devNo=0 );
    ModBbisCham( QString instName,
                              QString hwName,
                              QString driverName,
                              BusIfType ifType,
							  const int devNo=0);

	virtual ~ModBbisCham();
    void freeResources();
	Device *create(bool withSubDevs);
	Device *create_onboard(bool withSubDevs);

    virtual Device::ApplyResult
        applyProperties( DeviceProperties *newProp, QString &errMsg );
	virtual ApplyResult applyProperties( DescriptorEntryDirectory *devDesc,
										 QString &errMsg );
	virtual ComponentViewItem *createGui(MdiswizView *wizView,
										 Q3ListViewItem *parent);
    QString getMainProperties();
	void createSpecialDesc( DescriptorEntryDirectory *parentDesc );
	SwModuleList *enumSwModules();

	QString getDriverName( bool fullName, bool withSw);
    DeviceProperties *createProperties( int devNo );
    DeviceProperties *createProperties( void );
    void copyProperties(
        const DeviceProperties *from,
        DeviceProperties *to );
    DevPropertiesDlg *createPropDlg( QWidget *parent );
    
	bool chamSmbActive;
	
public slots:
	void activateChamSmb( bool active, int numDevices, MdiswizView *wizView );
	
protected:
	QString variant;
	
	bool _mezz; // is mezzanine bbis

private:
	bool _onboard; 
         
    PciBusInterface* smbusIf;
};

class ModBbisPciChamProperties : public DevicePropertiesWithSmb, public ModBbisPciMmodProperties
{
	public:
	ModBbisPciChamProperties()
		: DevicePropertiesWithSmb() {};
	
#ifdef WIN_MDISWIZ_LIB	
	// members for autoenum keys
	int autoenum;
	Q3MemArray <uchar> autoenum_excludingV2;
	Q3MemArray <uchar> autoenum_excluding;
#endif

};

//! BBIS class for generic mezzanine chameleon PCI device
class ModBbisCham4Mezz : public ModBbisCham
{
public:
	ModBbisCham4Mezz();
	// create another instance
	virtual Device *create(bool withSubDevs=true);
	virtual QString getDeviceNameTpl(){ return "MEZZ_CHAM";}
	SwModuleList *enumSwModules();
};

#endif





