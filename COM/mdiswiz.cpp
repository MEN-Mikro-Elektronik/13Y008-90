/***************************************************************************
 */
/*!       \file  mdiswiz.cpp
 *       \brief  Implementation of main mdiswiz GUI
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2018/06/13 14:16:02 $
 *    $Revision: 2.18 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: mdiswiz.cpp,v $
 * Revision 2.18  2018/06/13 14:16:02  ts
 * R: updated version to 02.05.00
 *
 * Revision 2.17  2016/04/29 10:44:45  ts
 * changed to Version 2.04.00
 *
 * Revision 2.16  2015/05/26 16:43:51  ts
 * R: gituser autocheckin: updated source
 *
 * Revision 2.15  2014/10/22 14:12:14  dpfeuffer
 * R: official release
 * M: beta note removed
 *
 * Revision 2.14  2014/08/22 15:56:07  dpfeuffer
 * R:1. inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 *   2. occupied PCIe slots not grayed out
 * M:1. PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *   2. CpuDeviceViewItem::showPopup: gray out all occupied PciBusInterface slots
 *
 * Revision 2.13  2014/07/18 15:11:52  dpfeuffer
 * R: erroneous PMC support, missing ComExpress support, general maintenance
 * M: intermediate check-in during development
 *
 * Revision 2.12  2014/06/06 14:13:50  ts
 * R: new CPUs G22, F75P added
 * M: updated version string to 2.01.024
 *
 * Revision 2.11  2013/09/27 08:19:07  dpfeuffer
 * R: A21 and G204 support added
 * M: version incremented
 *
 * Revision 2.10  2013/01/21 10:20:39  ts
 * R: version of mdiswiz not clear
 * M: display also build info in info label
 *
 * Revision 2.9  2012/05/14 11:16:41  ts
 * R: Keycode ALT+P was assigned to both Project and Packages
 * M: changed Keycode for Packages to ALT+A (P&ackages instead &Packages)
 *
 * Revision 2.8  2011/09/26 08:42:24  dpfeuffer
 * R: new bus interface CpciSer
 * M: modification for CpciSer
 *
 * Revision 2.7  2011/02/14 14:13:58  CRuff
 * R: new revision
 * M: changed version string
 *
 * Revision 2.6  2010/12/17 17:07:01  CRuff
 * R: new revision
 * M: changed version string
 *
 * Revision 2.5  2010/12/10 10:55:04  CRuff
 * R: Sporadic crashes at ChangeCPU
 * M: In ~ComponentViewItem: do not delete popup
 *
 * Revision 2.4  2010/10/26 09:07:19  CRuff
 * R: 1. application crash at add component action if no interfaces available
 *    2. cosmetics
 * M: 1a) in slotAddComponent(): only show dialog if interfaces available; show
 *        message box otherwise
 *    1b) added member function BeginnersAddDialog::numParentComponents that
 *        returns the number of interface parents in the system
 *    1c) leave slot slotParentCompChanged without doing anything if no valid
 *        parent component selected
 *    2a) renamed remove button to remove
 *    2b) changed debug messages
 *
 * Revision 2.3  2010/10/19 16:00:04  CRuff
 * R: no save dialog shown if project modified via the + button
 * M: call slotCfgModified() if addComponent dialog returns with value "OK"
 *
 * Revision 2.2  2010/07/12 10:06:23  dpfeuffer
 * R: access violation in mdiswiz_lib.lib under Windows with MDIS_CLINST
 * M: QComboBox replaced with Q3ComboBox
 *
 * Revision 2.1  2010/05/19 15:18:31  CRuff
 * R: Implementation of SmbPciBbis on z001 controllers (fpga)
 * M: implement method attachDev for BbisChamDeviceViewItems and allow fpga bbis
 *    devive view item to have bbis children
 *
 * Revision 2.0  2010/01/22 11:26:31  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.28  2009/08/17 11:56:14  CRuff
 * R: VxWorks: CPU core was not updated correctly when CPU was changed
 * M: emit signal to notify properties tab that cpu has changed
 *
 * Revision 1.27  2009/07/17 13:38:13  dpfeuffer
 * R: -pk2inf/-pk2inf5 option fixed
 * M: mdiswizVersion changed
 *
 * Revision 1.26  2009/04/30 21:23:43  CRuff
 * R: release MDIZWIZ
 * M: increased version number
 *
 * Revision 1.25  2008/10/27 12:29:45  channoyer
 * R: Remaining wrong box names
 * M: Call setCaption with G_progName constant
 *
 * Revision 1.24  2008/10/23 18:03:39  channoyer
 * R: Program name should be "MDIS Wizard" when displayed to the user
 * M:a) Replace progName with G_progName
 *   b) Use of the G_progName constant
 *
 * Revision 1.23  2008/07/31 14:30:59  dpfeuffer
 *  * R:1. Changes on common sources (support for QNX added)
 *  * M:1. Version incremented
 *
 * Revision 1.22  2007/02/23 13:16:28  DPfeuffer
 * revision changed
 *
 * Revision 1.21  2006/12/20 13:59:59  cs
 * raised version
 *
 * Revision 1.20  2006/11/30 11:50:40  cs
 * raised version
 *
 * Revision 1.19  2006/08/04 11:50:05  ub
 * incremented release number
 *
 * Revision 1.18  2006/07/27 13:43:23  ub
 * Version Number incremented
 *
 * Revision 1.17  2005/12/07 16:55:12  ub
 * changed version number
 *
 * Revision 1.16  2005/07/15 15:47:01  ub
 * version number 1.5.012_beta
 *
 * Revision 1.15  2005/03/09 13:48:32  ub
 * mdiswizVersion = "1.4.011"
 * Intermediate Release
 * - added support for Chameleon devices on F206 board
 * - new bus type "Chameleon Bus" for adding FPGA units
 *
 * Revision 1.14  2004/08/24 09:45:30  kp
 * mdiswizVersion = "1.4.010"
 *
 * Revision 1.13  2004/07/27 08:55:49  kp
 * Version changed
 *
 * Revision 1.12  2004/07/21 15:05:32  ufranke
 * changed
 *  - hide G_cfg
 *
 * Revision 1.11  2004/07/16 12:30:43  kp
 * Verision changed
 *
 * Revision 1.10  2004/06/25 13:42:37  ufranke
 * cosmetics
 *
 * Revision 1.9  2004/06/08 17:25:27  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.8  2004/01/09 08:20:04  kp
 * mdiswizVersion = "0.97a.003";
 * Don't allow CPU item to be deleted
 * cosmetics
 *
 * Revision 1.7  2003/06/11 12:21:32  kp
 * intermediate checkin
 *
 * Revision 1.6  2003/06/06 13:32:18  kp
 * mdiswizVersion = "0.96"
 *
 * Revision 1.5  2003/06/05 12:01:04  dschmidt
 * intermediate checkin
 *
 * Revision 1.4  2003/06/05 09:52:34  dschmidt
 * intermediate checkin
 *
 * Revision 1.3  2003/06/05 09:33:32  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/06/03 10:16:17  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:36:04  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <qlabel.h>
#include <qpainter.h>
#include <qpalette.h>
#include <qobject.h>
#include <q3popupmenu.h>
#include <q3scrollview.h>
#include <qmessagebox.h>
#include <q3header.h>
#include <qregexp.h>
#include <qlayout.h>
#include <qpixmap.h>
#include <qaction.h>
#include <q3toolbar.h>
#include <qstatusbar.h>
#include <qapplication.h>

#include <Q3HBoxLayout>
#include <Q3PtrList>
#include <Q3VBoxLayout>
#include <QCloseEvent>

#include "mdiswiz.h"
#include "wizdebug.h"
#include "hwcomponent.h"
#include "modelfactory.h"
#include "mdiswizdialogs.h"
#include "configuration.h"
#include "vmsgbox.h"
#include "targetos.h"
#include "packagedb.h"

// pixmaps
#include "images/cpu.xpm"
#include "images/bbis.xpm"
#include "images/mdisdev.xpm"
#include "images/fileopen.xpm"
#include "images/filenew.xpm"
#include "images/filesave.xpm"
#include "images/settings.xpm"
#include "images/addcomp.xpm"
#include "images/remove.xpm"

static QPixmap *G_pmCpu = 0;
static QPixmap *G_pmBbis = 0;
static QPixmap *G_pmDev = 0;

extern const QString	G_progName;     //!< main application name

/*!
  Global common version string (each target OS adds its own version):

  Common version x.yy.zzz

  x.yy = Major/Minor version number
  zzz  = Reflects changes in the CPU/BBIS model support (it is not reset on x.yy change!)
*/
extern const QString mdiswizVersion = "3.00.030";

// -----------------------------------------------------------------
//! constructs the root ComponentViewItem (QListViewItem)
/*!
  If this constructor is used, the new ComponentViewItem will be the root
  component.

  \param _wizview	link to main window
  \param parent		link to QListView container
  \param name		internal name of GUI object
*/
ComponentViewItem::ComponentViewItem( MdiswizView *_wizview,
									  Q3ListView *parent, const QString &name )
    : Q3ListViewItem( parent )
{
	init( _wizview, name );
}

// -----------------------------------------------------------------
//! constructs a subordinary ComponentViewItem (QListViewItem)
/*!
  If this constructor is used, the new ComponentViewItem will be added
  to the children of \a parent.

  \param _wizview	link to main window
  \param parent		link to parent
  \param name		internal name of GUI object
*/
ComponentViewItem::ComponentViewItem( MdiswizView *_wizview,
									  Q3ListViewItem *parent,
									  const QString &name )
    : Q3ListViewItem( parent )
{
	init( _wizview, name );
}

void
ComponentViewItem::init( MdiswizView *_wizview, const QString &name )
{
	QString dummy = name;
	device = 0;
	wizView = _wizview;
	popup = 0;
	// show all childrens
	setOpen(true);
}

//! remove automatically deletes all children GUI objects
ComponentViewItem::~ComponentViewItem()
{
	wDebug(("~ComponentViewItem dev=%s",
		   device ? device->getInstName().latin1() : "none" ));
}

//! show popup menu
void
ComponentViewItem::showPopup( const QPoint & point )
{
	UTIL_UNREF_PARAM(point);
	
}

//! redraw all text strings associated with this component
/*!
  Currently the following text is displayed:
  - Column 0: Device name
  - Column 1: Main property string
  - Column 2: Hardware (including variant) name
  - Column 3: Description
*/
void
ComponentViewItem::refresh()
{
	if( device ){
		setText( 0, device->getInstName());
		setText( 1, device->getMainProperties());
		setText( 2, device->getVendorModelName());
		setText( 3, device->getDescription());
	}
}

// -----------------------------------------------------------------

//! creates a CpuDeviceViewItem object
/*!
  this will be always the root component of mdiswiz.
*/
CpuDeviceViewItem::CpuDeviceViewItem( MdiswizView *_wizview,
									  Q3ListView *parent, const QString &name )
    : ComponentViewItem( _wizview, parent, name )
{
	if( ! G_pmCpu )
		G_pmCpu = new QPixmap( cpu_xpm );

	setPixmap( 0, *G_pmCpu );
}

//! creates a non-root CpuDeviceViewItem object
CpuDeviceViewItem::CpuDeviceViewItem( MdiswizView *_wizview,
									  Q3ListViewItem *parent,
									  const QString &name )
    : ComponentViewItem( _wizview, parent, name )
{
	if( ! G_pmCpu )
		G_pmCpu = new QPixmap( cpu_xpm );

	setPixmap( 0, *G_pmCpu );
}


//! save the underlying device; build popup menu
/*!
  normally called by the CpuDevice::createGui()
*/
void CpuDeviceViewItem::setDevice( CpuDevice *_device )
{
	BusInterface *busIf;
	int id;

	device = _device;

	if( popup == 0 )
		popup = new Q3PopupMenu(listView());

    popup->insertItem( QString( "Properties" ),
					   wizView, SLOT(slotProperties()));

	if( device->getParent() )
		// extension board
		popup->insertItem( QString( "Delete" ), wizView,
						   SLOT(slotDelete()));
	else
		popup->insertItem( QString( "Change CPU" ), wizView,
						   SLOT(slotChangeCpu()));

	for( id=0, busIf=(BusInterface *)device->firstChild(); busIf;
		 busIf=(BusInterface *)device->nextChild(), id++ ){

		if( busIf->isExpandeable())
			popup->insertItem(
				QString("Add Component to %1").arg(busIf->getInstName()),
				wizView, SLOT(slotAddToBusIf(int)), 0, id);
	}
	refresh();
}

void
CpuDeviceViewItem::showPopup( const QPoint & point )
{
	HwComponent* child;
	PciBusInterface *pciIface;
	int id;

	wDebug(("CDVI::showPopup"));

	// (De)activate BBIS slots depending wether they are already occupied
	for( id=0, child=device->firstChild(); child;
		 child=device->nextChild(), id++ ){

		 // try to cast instance to PciBusInterface
		 // and proceed if cast was successful
		 if( (pciIface = dynamic_cast<PciBusInterface*>(child)) ){

			wDebug(("child instance derived from PciBusInterface (busIf=%s)",
				(const char *)pciIface->busIfTypeMap.keyToStr(pciIface->getBusIfType())));

			popup->setItemEnabled( id, (pciIface->getNbrOfFreeSlots() > 0 ));
		 }
	}

	popup->popup( point );
}

void
CpuDeviceViewItem::getAttachableIfNames(QStringList *lst)
{
	BusInterface *busIf;
	int id;

	for( id=0, busIf=(BusInterface *)device->firstChild(); busIf;
		 busIf=(BusInterface *)device->nextChild(), id++ ){
		 	
		if( busIf && ( busIf->isExpandeable()) )
			(*lst) += busIf->getInstName();		
	}
}

void
CpuDeviceViewItem::getAttachableDevs(
	HwComponent *hwComp,
	Q3PtrList<Device> *lst)
{
	extern BbisModelFactory *G_bbisMods;
	extern CpuModelFactory *G_cpuMods;
	BusInterface *busIf;
	Device *dev;

	WIZ_DYNAMIC_CAST( hwComp, busIf, BusInterface * );

	// Build list of BBIS devices that can be added to that bus interface
 	for( dev=G_bbisMods->first(); dev; dev=G_bbisMods->next() ) {

        wDebug(("Device attachable? %s  removable: %d  canConnect: %d",
                dev->getModelName().ascii(),
                dev->isRemoveable(),
                busIf->canConnect( dev ) ));

		if( dev->isRemoveable() && busIf->canConnect( dev ) )
			lst->append(dev);
    }

	for( dev=G_cpuMods->first(); dev; dev=G_cpuMods->next() ) {
        wDebug(("Device attachable? %s  removable: %d  canConnect: %d",
                dev->getModelName().ascii(),
                dev->isRemoveable(),
                busIf->canConnect( dev ) ));
		if( dev->isRemoveable() && busIf->canConnect( dev ) )
			lst->append(dev);
    }

}

//! attach a device to the CPU
/*!
  \param hwComp      the component to which the device shall be added
  \param newDevName  name of device to attach

  \return true if component has been added
*/
bool
CpuDeviceViewItem::attachDev( HwComponent *hwComp, QString newDevName )
{
	extern BbisModelFactory *G_bbisMods;
	extern CpuModelFactory *G_cpuMods;
	Device *dev, *newDev;
	BusInterface *busIf;
	QString errMsg;
	ComponentViewItem *cv = 0;
	bool status=false;
	
	WIZ_DYNAMIC_CAST( hwComp, busIf, BusInterface * );

	dev = G_bbisMods->find( newDevName );
	if( !dev )
		// CPU extension boards...
		dev = G_cpuMods->find( newDevName );

	if( !dev )
		return false;
		
	// create an instance of template device
	newDev = dev->create();

	// suggest default name
	newDev->setInstName( MAIN_GetCurrentCfg()->nameMng.suggest(
							 newDev->getDeviceNameTpl()));

	// add it to the bus interface
	if( MAIN_GetCurrentCfg()->addDevice( busIf, newDev, errMsg ) == true ){
		cv = newDev->createGui( wizView, this );
		if( cv )
			status = true;
	}
	else {
		VMsgBox::criticalOk(
			MAIN_GetCurrentCfg()->msgBoxCaption(),
			"Could not add device",
			errMsg,
			wizView );
	}

	if( status == true ){
		// verify if all SW modules are valid
		// (disable any invalid module)
		errMsg = QString::null;
		MAIN_GetCurrentCfg()->validateSwModules( errMsg, true, true );

		// show the device's properties dialog
		newDev->propertiesDialog( wizView );
		cv->refresh();
	}
	
	return status;
}

//! add a component to a bus interface of the CPU
/*!
  called from MdiswizView::slotAddToBusIf().

  Brings up the "Add component" dialog presenting matching BBIS devices.

  \param id the nth bus interface of the CPU
  \return true if component has been added
*/
bool
CpuDeviceViewItem::addToBusIf( int id )
{
	Q3PtrList<HwComponent> &busIfLst = device->getChildren();
	BusInterface *busIf;
	int rv;
	bool status=false;
	Device *dev;
	QString compName = QString::null;

	AddComponentDialog dialog(wizView, 0,
							  "Add a component to the bus interface");

	busIf = (BusInterface *)busIfLst.at( id );

	if( busIf->getBusIfType() == HwComponent::ComExpr){
		QMessageBox::information( 
			wizView,
			"Important information to the COM Express Connector",
			"You can add a Generic COM Express carrier board to the COM Express connector. "
			"This Generic COM Express carrier board provides you the possibility "
			"to add devices to a PCI and PCIe bus. For this PCI(e) devices, you have to specify "
			"the PCI location (PCI domain/bus/device number) at the bus manually.\n\n"
			"Alternatively, you can add PCIe devices (that are physically connected to the CPU via "
			"a COM Express carrier board) directly to a provided PCIE_TX/RX slot of the CPU. "
			"The PCIE_TX/RX slots requires no further PCI location information (contains the CPU "
			"specific PCIe addressing).");
	}

	Q3PtrList<Device> lst;
	getAttachableDevs( busIf, &lst );


	if( lst.isEmpty() ){
		QMessageBox::information(
			wizView,
			MAIN_GetCurrentCfg()->msgBoxCaption(),
			"Sorry. There are no matching components to add.");
		return false;
	}

    for ( dev = lst.first(); dev; dev = lst.next() ){
		dialog.addItem( dev->getModelName(), dev->getDescription() );
	}

	dialog.setCaption(G_progName);
	rv = dialog.exec();
	compName = dialog.result();
	wDebug(( "dialog result=%d compName=%s", rv, compName.latin1() ));


	if( rv == 1 && compName != QString::null){ // ok button pressed
		status = attachDev( busIf, compName );
	}
	return status;
}

// -----------------------------------------------------------------

//! creates a BbisDeviceViewItem object
/*!
  this will be added to the children of \a parent.
*/
BbisDeviceViewItem::BbisDeviceViewItem( MdiswizView *_wizview,
										Q3ListViewItem *parent,
										const QString &name )
    : ComponentViewItem( _wizview, parent, name )
{
	if( ! G_pmBbis )
		G_pmBbis = new QPixmap( bbis_xpm );

	setPixmap( 0, *G_pmBbis );
}

//! save the underlying device; build popup menu
/*!
  normally called by the BbisDevice::createGui()
*/
void BbisDeviceViewItem::setDevice( BbisDevice *_device )
{
	BbisSlot *bbSlot;
	int id;

	device = _device;

	if( popup == 0 )
		popup = new Q3PopupMenu(listView());

    popup->insertItem( QString( "Properties" ),
					   wizView, SLOT(slotProperties()));

    popup->insertItem( QString( "Delete" ), wizView,
					   SLOT(slotDelete()));

	for( id=0, bbSlot=(BbisSlot *)device->firstChild(); bbSlot;
		 bbSlot=(BbisSlot *)device->nextChild(), id++ ){

		popup->insertItem(
			QString("Add Component to %1").arg( bbSlot->getSlotName()),
			wizView, SLOT(slotAddToBbSlot(int)), 0, id);
	}
	refresh();

}

void
BbisDeviceViewItem::showPopup( const QPoint & point )
{
	BbisSlot *bbSlot;
	int id;

	wDebug(("BDVI::showPopup"));

	// (De)activate BBIS slots depending wether they are already occupied
	for( id=0, bbSlot=(BbisSlot *)device->firstChild(); bbSlot;
		 bbSlot=(BbisSlot *)device->nextChild(), id++ ){
		popup->setItemEnabled( id, (bbSlot->firstChild() == 0));
	}

	popup->popup( point );
}

void
BbisDeviceViewItem::getAttachableIfNames(QStringList *lst)
{
	BbisSlot *bbSlot;
	int id;

	for( id=0, bbSlot=(BbisSlot *)device->firstChild(); bbSlot;
		 bbSlot=(BbisSlot *)device->nextChild(), id++ ){

		if( bbSlot->firstChild() == 0 )
			(*lst) += bbSlot->getSlotName();
	}
}

void
BbisDeviceViewItem::getAttachableDevs(
	HwComponent *hwComp,
	Q3PtrList<Device> *lst)
{
	extern MdisModelFactory *G_mdisMods;
	BbisSlot *bbSlot;
	MdisDevice *dev;

	WIZ_DYNAMIC_CAST( hwComp, bbSlot, BbisSlot * );
	wDebug(( "BbisDeviceViewItem::getAttachableDevs" ));

	// Build list of MDIS devices that can be added to that slot
	for( dev=(MdisDevice *)G_mdisMods->first(); dev; dev=(MdisDevice *)G_mdisMods->next() ) {
	  wDebug(( "getAttachableDevs: HwName=%s", dev->getHwName().ascii() ));
	  if( bbSlot->canConnect( dev ) )
	    lst->append(dev);
    }

}

//! attach a device to a BBIS slot
/*!
  \param hwComp      the component to which the device shall be added
  \param newDevName  name of device to attach

  \return true if component has been added
*/
bool
BbisDeviceViewItem::attachDev( HwComponent *hwComp, QString newDevName )
{
	extern MdisModelFactory *G_mdisMods;
	bool status = false;
	MdisDevice *dev, *newDev;
	BbisSlot *bbSlot;
	QString errMsg;
	int subDevNo = 0;

	WIZ_DYNAMIC_CAST( hwComp, bbSlot, BbisSlot * );

	dev = (MdisDevice *)G_mdisMods->find( newDevName );
	if( !dev )
		return false;

	// clone the template device
	newDev = (MdisDevice *)dev->create();

	while( subDevNo < newDev->numSubDevs() ){

		WIZ_ASSERT( newDev );

		// suggest default name
		if( newDev->numSubDevs() > 1 ){
			// For multi-Devices (M65 etc...) append "a", "b" etc
			// to device name
			newDev->setInstName( MAIN_GetCurrentCfg()->nameMng.suggest(
									 newDev->getDeviceNameTpl()) +
								 QChar('a'+subDevNo));
		}
		else {
			newDev->setInstName( MAIN_GetCurrentCfg()->nameMng.suggest(
									 newDev->getDeviceNameTpl()));
		}

		// add it to the bus interface
		errMsg = QString::null;
		if( MAIN_GetCurrentCfg()->addDevice( bbSlot, newDev, errMsg ) == true ){
			ComponentViewItem *cv;

			cv = ((MdisDevice *)newDev)->createGui( wizView, this );
			status = true;

			// show the device's properties dialog
			newDev->propertiesDialog( wizView );
			cv->refresh();
		}
		else {
			VMsgBox::criticalOk(
				MAIN_GetCurrentCfg()->msgBoxCaption(),
				"Could not add device",
				errMsg,
				wizView );
			break;
		}

		if( ++subDevNo < newDev->numSubDevs() )
			newDev = (MdisDevice *)newDev->createSubDev( subDevNo );
		else
			break;
	}
	// verify if all SW modules are valid (disable any invalid module)
	errMsg = QString::null;
	MAIN_GetCurrentCfg()->validateSwModules( errMsg, true, true );

	return status;

}

//! add a component to a slot of the BBIS device
/*!
  called from MdiswizView::slotAddToBbSlot().

  Brings up the "Add component" dialog presenting matching MDIS devices.

  \param id the nth slot of the BBIS device (not the slot number!)
  \return true if (at least one) component has been added
*/
bool
BbisDeviceViewItem::addToBbSlot( int id )
{
	Q3PtrList<HwComponent> &slotLst = device->getChildren();
	BbisSlot *bbSlot;
	int rv;
	bool status = false;
	QString compName = QString::null;
	Device *dev;

	AddComponentDialog dialog(wizView, 0, "Add a device to the slot");

	bbSlot = (BbisSlot *)slotLst.at( id );

	Q3PtrList<Device> lst;
	getAttachableDevs( bbSlot, &lst );

    for ( dev = lst.first(); dev; dev = lst.next() ){
		dialog.addItem( dev->getModelName(), dev->getDescription() );
	}

	if( lst.isEmpty() ){
		QMessageBox::information(
			wizView,
			MAIN_GetCurrentCfg()->msgBoxCaption(),
			"Sorry. There are no matching components to add.");
		return false;
	}

	dialog.setCaption(G_progName);
	rv = dialog.exec();
	compName = dialog.result();

	wDebug(( "dialog result=%d compName=%s", rv, compName.latin1() ));

	if( rv == 1 && compName != QString::null){ // ok button pressed
		status = attachDev( bbSlot, compName );
	}
	return status;
}

bool
BbisChamDeviceViewItem::attachDev(HwComponent *hwComp, QString newDevName )
{
	extern MdisModelFactory *G_mdisMods;
	extern BbisModelFactory *G_bbisMods;
	bool status = false;
	Device *dev, *newDev;
	//BbisSlot *bbSlot;
	QString errMsg;
	int subDevNo = 0;
	int numSubDevs = 1;
	bool isMdisDevice = true;

	wDebug(("BbisChamDeviceViewItem::attachDev"));
	
	// search in MdisModelFactory
	dev = G_mdisMods->find( newDevName );
	if( !dev )
	{
		dev = G_bbisMods->find( newDevName );
		if (!dev)
			return false;
		isMdisDevice = false;
	}
			
	// clone the template device
	newDev = dev->create();
	
	if ( isMdisDevice )
	{
		numSubDevs = ((MdisDevice*)newDev)->numSubDevs();
	}
	
	while( subDevNo < numSubDevs ){

		WIZ_ASSERT( newDev );
		// suggest default name
		if( numSubDevs > 1 ){
			// For multi-Devices (M65 etc...) append "a", "b" etc
			// to device name
			newDev->setInstName( MAIN_GetCurrentCfg()->nameMng.suggest(
									 newDev->getDeviceNameTpl()) +
								 QChar('a'+subDevNo));
		}
		else {
			newDev->setInstName( MAIN_GetCurrentCfg()->nameMng.suggest(
									 newDev->getDeviceNameTpl()));
		}

		// add it to the bus interface
		errMsg = QString::null;
		if( MAIN_GetCurrentCfg()->addDevice( hwComp, newDev, errMsg ) == true ){
			ComponentViewItem *cv;
			
			cv = newDev->createGui( wizView, this );
			status = true;
			
			// show the device's properties dialog
			newDev->propertiesDialog( wizView );
			cv->refresh();
		}
		else {
			VMsgBox::criticalOk(
				MAIN_GetCurrentCfg()->msgBoxCaption(),
				"Could not add device",
				errMsg,
				wizView );
			break;
		}

		if( ++subDevNo < numSubDevs )
			newDev = ((MdisDevice *)newDev)->createSubDev( subDevNo );
		else
			break;
	}
	// verify if all SW modules are valid (disable any invalid module)
	errMsg = QString::null;
	MAIN_GetCurrentCfg()->validateSwModules( errMsg, true, true );

	return status;
}

//! save the underlying device; build popup menu
/*!
  normally called by the BbisDevice::createGui()
*/
void BbisChamDeviceViewItem::setDevice( BbisDevice *_device )
{
	wDebug(("BbisChamDeviceViewItem::setDevice"));
	HwComponent *child;
	BbisSlot* bbSlot;
	int id;

	device = _device;

	if( popup == 0 )
		popup = new Q3PopupMenu(listView());

    popup->insertItem( QString( "Properties" ),
					   wizView, SLOT(slotProperties()));

    popup->insertItem( QString( "Delete" ), wizView,
					   SLOT(slotDelete()));

	// only create entries for real bbis slots (not for busIf for smb bbis)
	for( id=0, child=device->firstChild(); child;
		 child=device->nextChild(), id++ ){

		// is this a bbis slot?
		if (child->getDescription() != QString("SMB BBIS Interface"))
		{
			WIZ_DYNAMIC_CAST(child, bbSlot, BbisSlot*);
			popup->insertItem(
				QString("Add Component to %1").arg( bbSlot->getSlotName()),
				wizView, SLOT(slotAddToBbSlot(int)), 0, id);
		}
	}
	refresh();
	
	wDebug(("leave BbisChamDeviceViewItem::setDevice"));

}

void
BbisChamDeviceViewItem::showPopup( const QPoint & point )
{
	HwComponent* child;
	BbisSlot *bbSlot;
	int id;

	wDebug(("BDVI::showPopup"));

	// (De)activate BBIS slots depending wether they are already occupied
	for( id=0, child=device->firstChild(); child;
		 child=device->nextChild(), id++ ){
		
		// is this a bbis slot?
		if (child->getDescription() != QString("SMB BBIS Interface"))
		{
			WIZ_DYNAMIC_CAST(child, bbSlot, BbisSlot*);
			popup->setItemEnabled( id, (bbSlot->firstChild() == 0));
		}
	}

	popup->popup( point );
}

void
BbisChamDeviceViewItem::getAttachableIfNames(QStringList *lst)
{
	HwComponent* child;
	BbisSlot *bbSlot;
	int id;

	for( id=0, child=device->firstChild(); child;
		 child=device->nextChild(), id++ ){
		
		// is this a bbis slot?
		if (child->getDescription() != QString("SMB BBIS Interface"))
		{
			WIZ_DYNAMIC_CAST(child, bbSlot, BbisSlot*);
			
			if( bbSlot->firstChild() == 0 )
			(*lst) += bbSlot->getSlotName();
		}
	}
}

void
BbisChamDeviceViewItem::getAttachableDevs(
	HwComponent *hwComp,
	Q3PtrList<Device> *lst)
{
	extern MdisModelFactory *G_mdisMods;
	BbisSlot *bbSlot;
	MdisDevice *dev;
	
	wDebug(( "BbisChamDeviceViewItem::getAttachableDevs" ));

	// is this a bbis slot?
	if (hwComp->getDescription() != QString("SMB BBIS Interface"))
	{
		WIZ_DYNAMIC_CAST( hwComp, bbSlot, BbisSlot * );
	
		// Build list of MDIS devices that can be added to that slot
		for( dev=(MdisDevice *)G_mdisMods->first(); dev;
			 dev=(MdisDevice *)G_mdisMods->next() ) {
	        wDebug(( "getAttachableDevs: HwName=%s", dev->getHwName().ascii() ));
			if( bbSlot->canConnect( dev ) )
				lst->append(dev);
	    }
	}

}

// -----------------------------------------------------------------

//! creates a MdisDeviceViewItem object
/*!
  this will be added to the children of \a parent.
*/
MdisDeviceViewItem::MdisDeviceViewItem( MdiswizView *_wizview,
										Q3ListViewItem *parent,
										const QString &name )
    : ComponentViewItem( _wizview, parent, name )
{
	if( ! G_pmDev )
		G_pmDev = new QPixmap( dev_xpm );

	setPixmap( 0, *G_pmDev );

}

//! save the underlying device; build popup menu
/*!
  normally called by the MdisDevice::createGui()
*/
void MdisDeviceViewItem::setDevice( MdisDevice *_device )
{
	device = _device;

	if( popup == 0 )
		popup = new Q3PopupMenu(listView());

    popup->insertItem( QString( "Properties" ),
					   wizView, SLOT(slotProperties()));

    popup->insertItem( QString( "Delete" ), wizView,
					   SLOT(slotDelete()));
	refresh();

}

void
MdisDeviceViewItem::showPopup( const QPoint & point )
{
	popup->popup( point );
}

// -----------------------------------------------------------------

//! constructs the MdiswizView widget
/*!
  This is the main mdiswiz window containing the component hierarchy browser
  toolbar and menus.

  setupProjectActions() must be called after constructor or by subclasses!

  \param parent		parent widget (usually 0)
  \param name		internal name of GUI object
*/
MdiswizView::MdiswizView( QWidget *parent, const char *name )
    : Q3MainWindow( parent, name )
{
	modifiedLabel = 0;
	cfgModified = true;
    listView = new MdisWizListView( this );

    listView->addColumn( "Component" );
    listView->addColumn( "Location" );
    listView->addColumn( "Model" );
    listView->addColumn( "Functionality" );

	listView->setSorting( 1 ); // sort the "Location" column

	listView->setColumnWidth( 0, 170 );
	listView->setColumnWidth( 1, 190 );
	listView->setColumnWidth( 2, 150 );
	listView->setColumnWidthMode( 0, Q3ListView::Manual );
	listView->setColumnWidthMode( 1, Q3ListView::Manual );
	listView->setColumnWidthMode( 2, Q3ListView::Manual );

    listView->setRootIsDecorated( TRUE ); // level 0 entries have lines
	setCentralWidget( listView );

	// Connect right mouse button click event
    connect(listView,
	    SIGNAL( rightButtonPressed( Q3ListViewItem *, const QPoint& ,int )),
	    this,
	    SLOT( slotRMB( Q3ListViewItem *, const QPoint &, int ) ) );

	// Connect double click to properties
    connect(listView,
	    SIGNAL( doubleClicked( Q3ListViewItem *)),
	    this,
	    SLOT( slotPropertiesDc( Q3ListViewItem *)));

	menu = new QMenuBar( this );

	statusBar()->message("Ready");
	statusBar()->addWidget(
		(modifiedLabel = new QLabel( "", statusBar() )), 0, true);
	cfgUnmodified();
	popupMenuItem = 0;

	// make the main window bigger
	resize( 700, 300 );

	changeCaption();
}

//! returns the mdiswiz version
QString
MdiswizView::Version()
{
	return mdiswizVersion;
}

//! sets up the toolbar and project menu
void
MdiswizView::setupProjectActions()
{
	QAction *a;

	// generate toolbar
	tb = new Q3ToolBar( this, "project operations" );

	// generate project menu
	projectMenu = new Q3PopupMenu( this );


	menu->insertItem( "&Project", projectMenu );

	// generate project actions
	a = new QAction( QPixmap( filenew_xpm ),
					 "&New", Qt::CTRL+Qt::Key_N, this, "new" );
	a->setObjectName("New Project");
	connect( a, SIGNAL( activated() ) , this, SLOT( slotNewConfig() ) );
	a->addTo( tb );
	a->addTo( projectMenu );


	a = new QAction( QPixmap( fileopen_xpm ),
									 "&Open", Qt::CTRL+Qt::Key_O, this, "open" );
	a->setObjectName("Open Project");
 	connect( a, SIGNAL( activated() ) , this, SLOT( slotLoadConfig() ) );
	a->addTo( tb );
	a->addTo( projectMenu );
    projectMenu->insertSeparator();


	a = new QAction( QPixmap( filesave_xpm ),
					 "&Save", Qt::CTRL+Qt::Key_S, this, "save" );
	a->setObjectName("Save Project");
	connect( a, SIGNAL( activated() ) , this, SLOT( slotSaveConfig() ) );
	a->addTo( tb );
	a->addTo( projectMenu );


	a = new QAction( QPixmap(),
					 "Save &As", 0, this, "saveas" );
	a->setObjectName("Save Project As");
	connect( a, SIGNAL( activated() ) , this, SLOT( slotSaveAsConfig() ) );
	a->addTo( projectMenu );
    projectMenu->insertSeparator();

	a = new QAction( QPixmap(settings_xpm),
					 "S&ettings", 0, this, "settings" );
	a->setObjectName("Project Settings");
	connect( a, SIGNAL( activated() ) , this, SLOT( slotCfgSettings() ) );
	a->addTo( projectMenu );
	a->addTo( tb );

	a = new QAction( QPixmap(),
					 "&Quit", Qt::CTRL+Qt::Key_Q, this, "quit" );
	a->setObjectName("Exit mdiswiz");
	connect( a, SIGNAL( activated() ) , qApp, SLOT( closeAllWindows() ) );
	a->addTo( projectMenu );


	// generate editmenu
	Q3PopupMenu *editMenu = new Q3PopupMenu( this );
	menu->insertItem( "&Edit", editMenu );

	a = new QAction( QPixmap(addcomp_xpm),
					 "&Add Component", 0, this, "add" );
	a->setObjectName("Add Component to selected item");
	connect( a, SIGNAL( activated() ) , this, SLOT( slotAddComponent() ) );
	a->addTo( tb );
	a->addTo( editMenu );

	a = new QAction( QPixmap(remove_xpm),
					 "&Remove Component", 0, this, "remove" );
	a->setObjectName("Remove selected component");
	connect( a, SIGNAL( activated() ) , this, SLOT( slotDelete() ) );
	a->addTo( tb );
	a->addTo( editMenu );

	a = new QAction( QPixmap(),
					 "Component &Properties", 0, this, "properties" );
	a->setObjectName("Modify Component Properties");
	connect( a, SIGNAL( activated() ) , this, SLOT( slotProperties() ) );
	a->addTo( editMenu );

	// generate packagemenu
	Q3PopupMenu *pkgMenu = new Q3PopupMenu( this );
	menu->insertItem( "P&ackages", pkgMenu );

	a = new QAction( QPixmap(),
					 "&Install Driver", 0, this, "install" );
	a->setObjectName("Install Package");
	connect( a, SIGNAL( activated() ) , this, SLOT( slotPkgInstall() ) );
	a->addTo( pkgMenu );

	a = new QAction( QPixmap(),
					 "&Show Installed", 0, this, "show" );
	a->setObjectName("Show Installed Packages");
	connect( a, SIGNAL( activated() ) , this, SLOT( slotPkgShow() ) );
	a->addTo( pkgMenu );

	a = new QAction( QPixmap(),
					 "&Reparse", 0, this, "reparse" );
	a->setObjectName("Reparse Packages");
	connect( a, SIGNAL( activated() ) , this, SLOT( slotPkgReparse() ) );
	a->addTo( pkgMenu );

	// generate help menu
	Q3PopupMenu *helpMenu = new Q3PopupMenu( this );
	menu->insertItem( "&Help", helpMenu );

#if 0 //???
	a = new QAction( QPixmap(),
					 "&Contents", 0, this, "contents" );
	a->setObjectName("Contents");
	connect( a, SIGNAL( activated() ) , this, SLOT( slotHelp() ) );
	a->addTo( helpMenu );
#endif
	a = new QAction( QPixmap(),
					 "&Info", 0, this, "info" );
	a->setObjectName("Info");
	connect( a, SIGNAL( activated() ) , this, SLOT( slotInfo() ) );
	a->addTo( helpMenu );
}

//! check wether project needs to be saved
/*!
  If the project has been modified, brings up a dialog box to ask wether
  user wants to save the project.

  \return 0 - project has been saved\n
          1 - user pressed cancel in dialog - continue with project\n
		  2 - discard changes\n
		  3 - project was not modified
*/
int
MdiswizView::confirmClose()
{
	int rv;

	if( !cfgModified )
		return 3;

	rv = QMessageBox::information(
		this,
		MAIN_GetCurrentCfg()->msgBoxCaption(),
		"The project has been changed since "
		"the last save.",
		"Save Now", "Cancel", "Discard Changes",
		0, 1 );

    switch( rv ){
	case 0:	// save now
		slotSaveConfig();

		// if still modified, user pressed cancel in save dialog
		if( cfgModified )
			rv = 1;
		break;
	}

	return rv;
}

//! get the QListView object
Q3ListView *
MdiswizView::getListView()
{
	return listView;
}

//! handle close event of the application window
/*!
  Reimplemented from QMainWindow. Called when user attempts to close main
  application window
*/
void
MdiswizView::closeEvent( QCloseEvent* ce )
{
	switch( confirmClose() ){
	case 0:
	case 2:
	case 3:
        ce->accept();
		break;
	default:
        ce->ignore();
        break;
	}
}

//! slot: right mouse button clicked
void MdiswizView::slotRMB( Q3ListViewItem* Item, const QPoint & point, int )
{
	ComponentViewItem *cv = (ComponentViewItem *)Item;

	popupMenuItem = cv;

    if( Item )
		cv->showPopup( point );
}

//! Create a new configuration
/*!
  First the current configuration is checked for modification. User is asked
  to save changes.

  Then creates a new configuration and calls it's project wizard
  Config::newProjectWizard().

  If user cancels or in case of failure, the original configuration is
  restored.

  \return false=failed/aborted true=new config created

*/
bool
MdiswizView::newConfig()
{
	bool rv=false;
	Q3ListViewItem *it;
	Configuration *cfg;

	wDebug(("MdiswizView::newConfig()"));

	// check wether current project needs saving
	if( confirmClose() == 1 )
		return rv;

	// delete the old GUI items
	it = listView->firstChild();
	if( it )
		delete it;

	// create a new, empty configuration
	cfg = MAIN_GetCurrentCfg()->create();
	MAIN_SetCurrentCfgStoreLast( cfg );

	if( MAIN_GetCurrentCfg()->newProjectWizard(this) == true ){

		wDebug(("MdiswizView::newConfig() set new"));
		MAIN_DeleteOldCfg();

		slotCfgModified();
		rv = true;
	}
	else {
		// delete new
		wDebug(("MdiswizView::newConfig() delete new"));
		MAIN_SetCurrentCfgDeleteLast( MAIN_GetOldCfg() );
	}

	changeCaption();

	if( MAIN_GetCurrentCfg()->getRootDev() )
		MAIN_GetCurrentCfg()->getRootDev()->createGui( this, 0 );

	return rv;
}

//! create a new configuration
/*! \sa newConfig() */
void MdiswizView::slotNewConfig()
{
	newConfig();
}

void
MdiswizView::changeCaption()
{
	QString cap = G_progName;

	if( MAIN_GetCurrentCfg() ){
		if( MAIN_GetCurrentCfg()->configName() == QString::null )
			cap += ": (unnamed)";
		else
			cap += ": " + MAIN_GetCurrentCfg()->configName();
	}
	else
	{
		cap += ": (unnamed)";
	}

	cap += " ";

	setCaption( cap );
}

//! load configuration from filesystem
/*!
  First the current configuration is checked for modification. User is asked
  to save changes.

  Then loads a configuration from filesytem. If \a cfgName is not 0,
  the specified configuration is loaded, otherwise user will be asked which
  configuration to load.

  If user cancels or configuration load fails, the original configuration is
  preserved.

  \return false=failed/aborted true=new config loaded
*/
bool
MdiswizView::loadConfig( const QString &cfgName )
{
	wDebug(("loadConfig() %s", cfgName.latin1()));
	QString errMsg;
	bool success;
	Q3ListViewItem *it;
	Configuration *cfg;
	bool rv=false;

	// check wether current project needs saving
	if( confirmClose() == 1 )
		return rv;

	// delete the old GUI items
	it = listView->firstChild();
	if( it )
		delete it;

	// create a new, empty configuration
	cfg = MAIN_GetCurrentCfg()->create();
	MAIN_SetCurrentCfgStoreLast( cfg );

	// load new configuration
	success = MAIN_GetCurrentCfg()->load( errMsg, cfgName );

	// check if all loaded SW modules are valid (append errors to errMsg)
	MAIN_GetCurrentCfg()->validateSwModules( errMsg, true, false );

	rv = true;
	if( success )
	{
		MAIN_DeleteOldCfg();
		if( errMsg != QString::null )
		{
			VMsgBox::warningOk(
				MAIN_GetCurrentCfg()->msgBoxCaption(),
				QString("Warnings while loading project"),
				errMsg,	this );
		}
	}
	else
	{

		if( errMsg != QString::null )
		{
			// fatal errors while loading
			VMsgBox::criticalOk(
				MAIN_GetCurrentCfg()->msgBoxCaption(),
				QString("Could not load project"),
				errMsg,	this );
		}
		else
		{
			// user pressed cancel

		}
		// restore the old configuration (delete current)
		MAIN_SetCurrentCfgDeleteLast( MAIN_GetOldCfg() );
		rv = false;
	}

	cfgUnmodified();
	changeCaption();
	wDebug(("loadConfig()2 %s", MAIN_GetCurrentCfg()->configName().latin1()));

	// create the new GUI representation
	wDebug(("create new GUI"));
	if( MAIN_GetCurrentCfg()->getRootDev() )
		MAIN_GetCurrentCfg()->getRootDev()->createGui( this, 0 );

	return rv;
}


//! get currently active item in MDISWIZ listview
/*!
  the currently selected item is returned
*/
ComponentViewItem *
MdiswizView::activeItem()
{
	if( listView->selectedItem() ){
		ComponentViewItem *cvi;
		WIZ_DYNAMIC_CAST( listView->selectedItem(), cvi, ComponentViewItem *);
		return cvi;
	}
	return 0;
}


//! load configuration
/*!
  just calls loadConfig(0)
*/
void MdiswizView::slotLoadConfig()
{
	wDebug(("slotLoadConfig()"));
	loadConfig(0);
}

//! save configuration
void MdiswizView::slotSaveConfig()
{
	QString errMsg;

	wDebug(("slotSaveConfig()"));

	if( MAIN_GetCurrentCfg()->save(errMsg) == false ){
		if( !errMsg.isEmpty() )
			VMsgBox::criticalOk(
				MAIN_GetCurrentCfg()->msgBoxCaption(),
				QString("Could not save configuration"),
				errMsg,	this );
	}
	else {
		cfgUnmodified();
		changeCaption();
	}
}

//! save configuration
void MdiswizView::slotSaveAsConfig()
{
	QString errMsg;

	wDebug(("slotSaveAsConfig()"));
	if( MAIN_GetCurrentCfg()->saveAs(errMsg) == false ){
		//errMsg may contain messages if the config name is invalid
		//errMsg is empty when 'Cancel' button was used,
		if( !errMsg.isEmpty() ){
			VMsgBox::criticalOk(
					MAIN_GetCurrentCfg()->msgBoxCaption(),
					QString("Could not save configuration"),
					errMsg,	this );
		}
	}
	else {
		cfgUnmodified();
		changeCaption();
	}
}

//! project settings
void MdiswizView::slotCfgSettings()
{
	if( MAIN_GetCurrentCfg()->settingsDlg() )
		slotCfgModified();
}


//! delete a component (and all it's children)
void MdiswizView::slotDelete()
{
	ComponentViewItem *cvi = activeItem();

	if( cvi == 0 )
		return;

	Device *device = cvi->getDevice();

    QString msg = "Do you really want to delete " +
        device->getInstName();

    if( device->firstChild() )
        msg += "\n(And all of it's sub devices)";
    msg += " ?";

    if( QMessageBox::warning( this,
                              MAIN_GetCurrentCfg()->msgBoxCaption(),
                              msg,
                              "Delete",
                              "Cancel",
                              QString::null,
                              1,1) == 0 ){


        // delete GUI representation
        delete cvi;

        // delete device itsself
        MAIN_GetCurrentCfg()->removeDevice( device );

        slotCfgModified();
    }
}

void MdiswizView::slotDefineCpu()
{
}

//! Change CPU device
void MdiswizView::slotChangeCpu()
{
	ComponentViewItem *cvi = activeItem();

	if( cvi == 0 )
		return;

	extern CpuModelFactory *G_cpuMods;
	int rv;
	Configuration *cfg = MAIN_GetCurrentCfg();
	CpuDevice *dev;
	Device *newDev;
	QString errMsg, compName;

	if( !cfg )
		return;

	AddComponentDialog dialog( this, 0,
		   "Please select your target CPU board from the list below:");

	for( dev=(CpuDevice *)G_cpuMods->first(); dev;
		 dev=(CpuDevice *)G_cpuMods->next() ){

		if( cfg->cpuOkForCfg( dev ) )
			dialog.addItem( dev->getModelName(), dev->getDescription() );
	}

	dialog.setCaption(G_progName);
	rv = dialog.exec();
	compName = dialog.result();

	wDebug(( "dialog result=%d compName=%s", rv, compName.latin1() ));

	if( rv == 1 && compName != QString::null){ // ok button pressed
		QString errMsg;

		slotDelete();				// delete CPU item
		if( MAIN_GetCurrentCfg()->getRootDev() )
			return;					// root dev has not been deleted

		slotCfgModified();

		dev = (CpuDevice *)G_cpuMods->find( compName );

		WIZ_ASSERT(dev);

		// create an instance of template device
		newDev = dev->create();

		if( cfg->addDevice( 0, newDev, errMsg ) == false ){

			VMsgBox::criticalOk(
				cfg->msgBoxCaption(),
				"Couldn't create CPU device",
				errMsg,
				this );
			return;
		}
		// send signal that cpu has changed
		emit sigCpuChanged();
		newDev->createGui( this, 0 );
		//wDebug(("gui created\n"));
	}
	//wDebug(("leave slotChangeCpu\n"));

}



//!add component to BusInterface
/*!
  \param id the nth busInterface
*/
void MdiswizView::slotAddToBusIf( int id )
{
	ComponentViewItem *cvi = activeItem();
	CpuDeviceViewItem *cpuVi;

	if( !cvi )
		return;

	cpuVi = dynamic_cast<CpuDeviceViewItem  *>(cvi);

	// call the CpuDeviceViewItem's method
	if( cpuVi->addToBusIf( id ) )
		slotCfgModified();
}

//! add component to BBIS slot
/*!
  \param id the nth slot
*/
void MdiswizView::slotAddToBbSlot( int id )
{
	ComponentViewItem *cvi = activeItem();
	BbisDeviceViewItem *bbVi;

	if( !cvi )
		return;

	bbVi = dynamic_cast<BbisDeviceViewItem  *>(cvi);

	// call the BbisDeviceViewItem's method
	if( bbVi->addToBbSlot( id ) )
		slotCfgModified();

}

void MdiswizView::slotAddComponent()
{
	wDebug(("Mdiswiz::slotAddComponent"));

	BeginnersAddDialog dialog( listView, activeItem(), this );

	if ( dialog.numParentComponents() > 0 )
	{
		int rv = dialog.exec();
		QString compName = dialog.result();
	
		if( rv == 1 && compName != QString::null){ // ok button pressed
			dialog.getParentCvi()->attachDev(
				dialog.getParentHwComp(), compName );
			// configuration changed!
			slotCfgModified();
		}
	} else {
		QMessageBox::information(
			this,
			MAIN_GetCurrentCfg()->msgBoxCaption(),
			"Sorry. There are no interfaces available for this hardware.");
	}

}

//! show device properties (called by double click)
void MdiswizView::slotPropertiesDc( Q3ListViewItem *item )
{
	popupMenuItem = (ComponentViewItem*)item;
	slotProperties();
}

//! show device properties
void MdiswizView::slotProperties()
{
	Device *dev;
	ComponentViewItem *cvi = activeItem();

	if( cvi == 0 )
		return;

	dev = cvi->getDevice();

	if( dev->propertiesDialog( this ) ){
		slotCfgModified();

		// in case instance name changed
		cvi->refresh();
	}
	listView->sort(); // resort names. Does not work! why???
}

//! flag configuration has been modified
void MdiswizView::slotCfgModified()
{
	wDebug(("MdiswizView::slotCfgModified"));
	if( cfgModified == false ){
		modifiedLabel->setText("modified");
		cfgModified = true;
	}
}

void MdiswizView::cfgUnmodified()
{
	if( cfgModified ){
		modifiedLabel->setText("unmodified");
		cfgModified = false;
	}
}

//! installed package
void MdiswizView::slotPkgInstall()
{
	PackageDb pkgDb(MAIN_GetCurrentCfg()->targetOs()->pkgDbPath());
	extern MdisModelFactory *G_mdisMods;

	if( MAIN_GetCurrentCfg()->targetOs()->installDlg() == true )
		pkgDb.parse( G_mdisMods );

}

//! reparse package Db
void MdiswizView::slotPkgReparse()
{
	PackageDb pkgDb(MAIN_GetCurrentCfg()->targetOs()->pkgDbPath());
	extern MdisModelFactory *G_mdisMods;

	if( pkgDb.parse( G_mdisMods ) == true ){
		QMessageBox::information(
			this,
			MAIN_GetCurrentCfg()->msgBoxCaption(),
			QString("Packages have been reparsed from \"") +
			MAIN_GetCurrentCfg()->targetOs()->pkgDbPath() + "\"");
	}
}

//! show installed packages
void MdiswizView::slotPkgShow()
{
	PackageDb pkgDb(MAIN_GetCurrentCfg()->targetOs()->pkgDbPath());
	pkgDb.showPackages();
}


//! show about info
void MdiswizView::slotInfo()
{
	QMessageBox::about(
		this,
		G_progName + " - the MDIS configuration wizard",
		QString("mdiswiz version ") +
		MAIN_GetCurrentCfg()->targetOs()->mdiswizVersion() + "\n"
		"(c) MEN Mikro Elektronik GmbH\n"
		"Built with Qt version " + qVersion() + "\n"
		"Build date " + QString(__DATE__) + " : " + QString(__TIME__) +"\n"
		"In case of problems please contact support@men.de");
}

//------------------------------------------------------------------------

//! find component with name \a name in MdiswizView
ComponentViewItem *
MdisWizListView::findItemByInstName( const QString &name )
{
    Q3ListViewItemIterator it( this );

    while ( it.current() ) {
		ComponentViewItem *cvi =
			dynamic_cast<ComponentViewItem *>(it.current());

		if( cvi && cvi->getDevice() ){
			if( cvi->getDevice()->getInstName() == name )
				return cvi;
		}

        ++it;
    }
	return 0;
}

//------------------------------------------------------------------------

BeginnersAddDialog::BeginnersAddDialog(
	MdisWizListView *_wizView,
	ComponentViewItem *activeItem,
	QWidget *parent,
	const char *name,
	bool modal,
	Qt::WFlags f) : QDialog(parent,name,modal,f)
{
	Q3VBoxLayout* vb = new Q3VBoxLayout(this,10);
	QPushButton *but;


    wDebug(( "BeginnersAddDialog" ));

	setCaption(G_progName);
	wizView = _wizView;
	parentCvi = activeItem;
	parentHwComp = 0;

	vb->addWidget( new QLabel( "Where to add the component to:", this ));

	parentCompWdg = new Q3ComboBox( false, this );
	vb->addWidget( parentCompWdg );

	parentIfWdg = new Q3ComboBox( false, this );
	vb->addWidget( parentIfWdg );

   	addCompWdg = new AddComponentWidget(
		this, 0,
		"Please select the component to add" );
	vb->addWidget( addCompWdg );


	Q3HBoxLayout *hb = new Q3HBoxLayout();
	vb->addLayout( hb );
	hb->addStretch(1);

	okBut = new QPushButton("OK", this);
	hb->addWidget( okBut );
	connect( okBut, SIGNAL(clicked()), this, SLOT(accept()) );
	//okBut->setEnabled( false );

	but = new QPushButton("Cancel", this);
	hb->addWidget( but );
	connect( but, SIGNAL(clicked()), this, SLOT(reject()) );

	connect( parentCompWdg, SIGNAL(activated(int)),
			 this, SLOT(slotParentCompChanged(int)));

	connect( parentIfWdg, SIGNAL(activated(int)),
			 this, SLOT(slotParentIfChanged(int)));


	getAllParentComps();

	if( activeItem ){
		QStringList lst;
		activeItem->getAttachableIfNames( &lst );
		if( !lst.isEmpty() )
			parentCompWdg->setCurrentText(
				activeItem->getDevice()->getInstName());
	}
	slotParentCompChanged( parentCompWdg->currentItem() );
}

int 
BeginnersAddDialog::numParentComponents()
{
	return parentCompWdg->count();
}

void
BeginnersAddDialog::getAllParentComps()
{
    Q3ListViewItemIterator it( wizView );
	QStringList choices;

    while ( it.current() ) {
		ComponentViewItem *cvi =
			dynamic_cast<ComponentViewItem *>(it.current());

		if( cvi && cvi->getDevice() ){
			wDebug(("BeginnersAddDialog::getAllParentComps: %s",
					cvi->getDevice()->getInstName().latin1()));

			QStringList lst;
			cvi->getAttachableIfNames( &lst );
			if( !lst.isEmpty() )
				choices += cvi->getDevice()->getInstName();
		}
		++it;
	}
	parentCompWdg->insertStringList( choices );
}

void
BeginnersAddDialog::slotParentCompChanged( int idx )
{
	QString parentComp = parentCompWdg->currentText();
	if (parentComp.isEmpty())
		return;

	UTIL_UNREF_PARAM(idx);

	wDebug(("slotParentCompChanged: %s; idx: %d", parentComp.latin1(), idx));

	ComponentViewItem *cvi = wizView->findItemByInstName( parentComp );

	parentCvi = cvi;

	QStringList lst;
	cvi->getAttachableIfNames( &lst );

	parentIfWdg->clear();
	parentIfWdg->insertStringList( lst );
	slotParentIfChanged(0);
}

void
BeginnersAddDialog::slotParentIfChanged( int idx )
{
	HwComponent *hwComp;
	Device *dev;

	UTIL_UNREF_PARAM(idx);

	wDebug(("slotParentIfChanged: %s",  parentIfWdg->currentText().latin1()));

	addCompWdg->clearItems();

	if( !parentCvi )
		return;

	hwComp = parentCvi->getDevice()->findChild( parentIfWdg->currentText() );

	//hwComp = parentCvi->getDevice()->getChildren().at( idx );
	parentHwComp = hwComp;
	if( !hwComp )
		return;

	Q3PtrList<Device> lst;
	parentCvi->getAttachableDevs( hwComp, &lst );

    for ( dev = lst.first(); dev; dev = lst.next() ){
		addCompWdg->addItem( dev->getModelName(), dev->getDescription() );
	}
}


