/***************************************************************************
 */
/*!       \file  propertiesdlg.cpp 
 *       \brief  Implementation of device properties dialog
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2018/06/13 14:23:34 $
 *    $Revision: 2.3 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: propertiesdlg.cpp,v $
 * Revision 2.3  2018/06/13 14:23:34  ts
 * R: # of Z001 instances in a CC10 FPGA design was very large (22 or so)
 * M: increased max. nr. in QSpinBox from 10 to 30
 *
 * Revision 2.2  2010/07/12 10:06:59  dpfeuffer
 * R: access violation in mdiswiz_lib.lib under Windows with MDIS_CLINST
 * M: QComboBox replaced with Q3ComboBox
 *
 * Revision 2.1  2010/05/19 16:35:52  CRuff
 * R: 1. cascading BbisViewItems needed for SmbPci implementation; this changes
 *       the given object hierarchies
 *    2. Cpu devices have onboad smb controllers, chameleon BBISes have z001
 *       smb controllers
 *    3. SmbPciBbis has to be added to device tree if activate smb checkbox is
 *       checked
 * M: 1. at various methods, get properties from device instead
 *       using the current properties the main dialog
 *    2. if Device is chameleon bbis, hide the onboard activation widgets and the
 *       other way round
 *    3. emit signal to notify the device tree that a change is necessary
 *
 * Revision 2.0  2010/01/22 11:27:30  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.8  2009/12/07 09:07:22  CRuff
 * R: 1. add SMBus support
 *    2. cosmetics
 * M: 1. add new tab DevSmbTab to handle SMBus settings
 *    2. add function headers where needed
 *
 * Revision 1.7  2004/04/13 11:01:22  dpfeuffer
 * cosmetics
 *
 * Revision 1.6  2004/01/09 08:20:24  kp
 * support isInstNameChangeAllowed
 *
 * Revision 1.5  2003/06/11 12:21:45  kp
 * intermediate checkin
 *
 * Revision 1.4  2003/06/05 12:01:19  dschmidt
 * intermediate checkin
 *
 * Revision 1.3  2003/06/05 09:33:51  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/06/03 10:16:42  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:36:20  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#include <qlabel.h>
#include <q3combobox.h>
#include <qstringlist.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3VBoxLayout>
#include "wizdebug.h"
#include "propertiesdlg.h"
#include "instnamewidget.h"
#include "hwcomponent.h"
#include "wizexcept.h"
#include "mdiswiz.h"

//----------------------------------------------------------------------
//! create a device properties dialog
/*!
  adds one tab, the "General" tab.
  call setButtons() before bringing up the dialog

  \param parent parent widget
  \param dev associated device
  \param generalTab if true, create the "general" tab
*/
DevPropertiesDlg::DevPropertiesDlg( QWidget *parent, Device *dev, 
									bool generalTab ) : 
	Q3TabDialog( parent, 0, true ) 
{
	prop = 0;
	device = dev;
	debugTabAdded = false;

	if( generalTab )
		addTab( new DevGeneralTab( parent, this ), "General");
	connect( this, SIGNAL(applyButtonPressed()), this, SLOT(slotApply()));
	updateCaption();
}

//! add the "Ok", "Apply" and "Cancel" button to dialog
/*!
  must be called after addTab() and before exec()
*/
void 
DevPropertiesDlg::setButtons() 
{ 
	setOkButton();
	setApplyButton(); 
	setCancelButton();
}

//! set the device properties pointer
/*!
  It creates a debug Tab if not already done (and device has debug settings)
  emits sigNewProps()
*/
void
DevPropertiesDlg::setProperties( DeviceProperties *_prop )
{
	prop = _prop;

	if( debugTabAdded == false ){
		if( prop->dbgSettings.hasModules() ){
			debugTabAdded = true;
			addTab( new DevDebugTab( parentWidget(), this ), "Debug Settings");
		}
	}

	// inform tabs about new settings
	emit sigNewProps();	
}

//! called when dialog Apply button pressed
/*!
  emits sigUpProps() to update dialog tabs.
  
  emits apply() to inform users of this dialog.
*/
void
DevPropertiesDlg::slotApply()
{
	// force tabs to update properties from their widgets
	emit sigUpProps();
	// inform user
	emit apply();
	updateCaption();
}

//! get pointer to the connected device
/*!
  \throw WizExceptAssert if no associated device
*/
Device *
DevPropertiesDlg::getDevice()
{
	WIZ_ASSERT( device != 0 );
	return device;
}

void 
DevPropertiesDlg::updateCaption()
{
	setCaption( "Properties of " + device->getInstName() );
}

//----------------------------------------------------------------------

//! constructs a DevPropertiesTab
/*!
  \param parent parent widget
  \param _mainDlg pointer to main dialog
  \param name internal name
  \param f window flags
*/
DevPropertiesTab::DevPropertiesTab( 
	QWidget *parent, 
	DevPropertiesDlg *_mainDlg,
	const char *name, 
	Qt::WFlags f) : 
	QWidget(parent,name,f) 
{
	mainDlg = _mainDlg;
	connect( getMainDlg(), SIGNAL(sigNewProps()), this, SLOT(slotNewProps()));
	connect( getMainDlg(), SIGNAL(sigUpProps()), this, SLOT(slotUpProps()));
}

//! get a pointer to the main dialog
DevPropertiesDlg *
DevPropertiesTab::getMainDlg()
{	
	return mainDlg;
}

//! called whenever the properties of the device have changed
/*!
  update the tab widgets from the new values
*/
void 
DevPropertiesTab::slotNewProps()
{
	wDebug(("DevPropertiesTab::slotNewProps"));
	setProperties();
}

//! called to read out the widgets and update the device properties object
void 
DevPropertiesTab::slotUpProps()
{
	wDebug(("DevPropertiesTab::slotUpProps"));
	updateProperties();
}

//----------------------------------------------------------------------

//! constructs general device properties tab
DevGeneralTab::DevGeneralTab(
	QWidget *parent,
	DevPropertiesDlg *_mainDlg,
	const char *name, 
	Qt::WFlags f) : 
	DevPropertiesTab(parent,_mainDlg,name,f)
{
	QLabel *lab;
	int row=0;

	Q3VBoxLayout *vb = new Q3VBoxLayout( this, 10 );
	Q3GridLayout *gl = new Q3GridLayout( vb, 4, 2 );
	// Device Name
	lab = new QLabel( QString("Device Name:"), this );	
	gl->addWidget( lab, row, 0, Qt::AlignRight );

	instNameWidget = new InstNameWidget( this );
	gl->addWidget( instNameWidget, row, 1 );
	row++;

	if( getMainDlg()->getDevice()->isInstNameChangeAllowed() == false )
		instNameWidget->setEnabled( false );

	// Hardware Name
	lab = new QLabel( QString("Vendor/Model:"), this );	
	gl->addWidget( lab, row, 0, Qt::AlignRight );

	lab = new QLabel( getMainDlg()->getDevice()->getVendorModelName(), this);
	gl->addWidget( lab, row, 1, Qt::AlignLeft );

	row++;

	// Description
	lab = new QLabel( QString("Description:"), this );	
	gl->addWidget( lab, row, 0, Qt::AlignRight );

	lab = new QLabel( getMainDlg()->getDevice()->getDescription(), this );	
	gl->addWidget( lab, row, 1, Qt::AlignLeft );

	row++;

	// Driver Name
	lab = new QLabel( QString("Driver:"), this );	
	gl->addWidget( lab, row, 0, Qt::AlignRight );

	lab = new QLabel( getMainDlg()->getDevice()->getDriverName(true), this );
	gl->addWidget( lab, row, 1, Qt::AlignLeft );

	row++;

	vb->addStretch(5);
}

void
DevGeneralTab::updateProperties()
{
	DeviceProperties *_prop = getMainDlg()->getProperties();

	_prop->deviceName = instNameWidget->text();
}

void
DevGeneralTab::setProperties()
{
	DeviceProperties *_prop = getMainDlg()->getDevice()->getProp();

	instNameWidget->setText( _prop->deviceName );
}

//----------------------------------------------------------------------

//! constructs device debug properties tab
DevDebugTab::DevDebugTab(
	QWidget *parent,
	DevPropertiesDlg *_mainDlg,
	const char *name, 
	Qt::WFlags f) : 
	DevPropertiesTab(parent,_mainDlg,name,f)
{
	QLabel *lab;
	int row=0;
	Q3ComboBox *cb;
	QStringList cboxItems;
	DeviceProperties *_prop = getMainDlg()->getProperties();

	// combobox items
	cboxItems << "Errors only" << "Basic" << "Verbose" << "Very verbose";

	Q3VBoxLayout *vb = new Q3VBoxLayout( this, 10 );
	lab = new QLabel( QString("Debug Settings..."), this );	
	vb->addWidget( lab );
	
	Q3VBoxLayout *vb2 = new Q3VBoxLayout( vb );

	Q3GridLayout *gl = new Q3GridLayout( vb2, 0, 2, 5 );

	// make a combobox for each module's debug setting
	for( int module=DebugProperties::Driver; module<DebugProperties::_Max;
		 module++ ){
		
		if( _prop->dbgSettings.hasModule((DebugProperties::Module)module ) ){
			lab = new QLabel( DebugProperties::moduleMap.keyToStr(
				(DebugProperties::Module)module), this );	
			gl->addWidget( lab, row, 0, Qt::AlignRight );

			cb = new Q3ComboBox( this );
			cb->insertStringList( cboxItems );
			dbgLevCbox[module] = cb;

			gl->addWidget( cb, row, 1, Qt::AlignLeft );
			row++;
		}
	}

	vb->addStretch(5);
}

//! implementation of updateProperties() 
/*!
  reads the current settings from Qt widgets and
  updates the device debug properties 
*/
void
DevDebugTab::updateProperties()
{
	DeviceProperties *_prop = getMainDlg()->getProperties();
	Q3ComboBox *cb;
	DebugProperties::Module mod;

	for( int module=DebugProperties::Driver; module<DebugProperties::_Max;
		 module++ ){
		
		mod = (DebugProperties::Module)module;

		if( _prop->dbgSettings.hasModule( mod) ){
			cb = dbgLevCbox[module];

			switch( cb->currentItem() ){
			case 0:	
				_prop->dbgSettings.setModuleLevel( mod, ErrorsOnly );
				break;
			case 1:	
				_prop->dbgSettings.setModuleLevel( mod, Basic );
				break;
			case 2:	
				_prop->dbgSettings.setModuleLevel( mod, Verbose );
				break;
			case 3:	
				_prop->dbgSettings.setModuleLevel( mod, VeryVerbose );
				break;
			default:
				break;
			}	
		}
	}
}

//! implementation of setProperties()
/*!
  fills the Qt widgets for debug settings with
  values of internal device properties structure
*/
void
DevDebugTab::setProperties()
{
	DeviceProperties *_prop = getMainDlg()->getProperties();

	Q3ComboBox *cb;
	DebugProperties::Module mod;

	for( int module=DebugProperties::Driver; module<DebugProperties::_Max;
		 module++ ){
		
		mod = (DebugProperties::Module)module;

		if( _prop->dbgSettings.hasModule( mod) ){
			cb = dbgLevCbox[module];
			
			switch( _prop->dbgSettings.moduleLevel( mod )){
			case ErrorsOnly:
				cb->setCurrentItem(0);
				break;
			case Basic:
				cb->setCurrentItem(1);
				break;
			case Verbose:
				cb->setCurrentItem(2);
				break;
			case VeryVerbose:
				cb->setCurrentItem(3);
				break;
			default:
				// if somebody has changed descriptor file by hand...
				cb->setCurrentItem(3);
				break;
			}
		}
	}
}

//----------------------------------------------------------------------

//! constructs device smb properties tab
DevSmbTab::DevSmbTab(
	bool isCham,
	QWidget *parent,
	DevPropertiesDlg *_mainDlg,
	const char *name, 
	Qt::WFlags f) : 
	DevPropertiesTab(parent,_mainDlg,name,f)
{
	DevicePropertiesWithSmb *_smbProp;
	DeviceProperties *_prop = getMainDlg()->getDevice()->getProp();
	WIZ_DYNAMIC_CAST(_prop, _smbProp, DevicePropertiesWithSmb* );
	
	Q3VBoxLayout *vb = new Q3VBoxLayout( this, 10 );
	QLabel* lab = new QLabel( QString("SMBus Settings: "), this );	
	vb->addWidget( lab );
	
	Q3VBoxLayout *vb2 = new Q3VBoxLayout( vb );
	
	obSmbActiveChkBox = new QCheckBox(QString("Activate Onboard SMB controllers"), this);
	vb2->addWidget(obSmbActiveChkBox);
	
	z001SmbActiveChkBox = new QCheckBox(QString("Activate Chameleon SMB controllers"), this);
	vb2->addWidget(z001SmbActiveChkBox);
	
	Q3GridLayout *gl = new Q3GridLayout( vb2, 1, 2, 3 );
	numZ001Label = new QLabel( QString("Number of Z001 devices: "), this );
	numZ001Label->setEnabled(false);
	gl->addWidget(numZ001Label, 0, 0);
	
	numZ001SmbSpinBox = new QSpinBox(1,30,1,this);
	gl->addWidget(numZ001SmbSpinBox, 0, 1);
	numZ001SmbSpinBox->setEnabled(false);

	vb->addStretch(5);
	
	// connect z001 checkbox and spinbox
	connect(z001SmbActiveChkBox, SIGNAL(stateChanged(int)), this, SLOT(enableSpinBox(int)));
	
	// hide the z001 widgets if no fpga on board
	if (isCham) {
		hideOnboardSmbWidgets(true);
		hideChamSmbWidgets(false);
	} else {
		hideOnboardSmbWidgets(false);
		hideChamSmbWidgets(true);
	}
}

//! slot to enable/disable the z001 property widgets
/*!
  called when the z001SmbActiveChkBox state is changed;
  enables / disables the widgets for number of z001 smb
  controllers

  \param state state of z001SmbActiveChkBox
*/
void 
DevSmbTab::enableSpinBox(int state)
{
	wDebug(("DevSmbTab::enableSpinBox"));
	if (state == QCheckBox::On) {
		numZ001Label->setEnabled(true);
		numZ001SmbSpinBox->setEnabled(true);
	} else if (state == QCheckBox::Off) {
		numZ001Label->setEnabled(false);
		numZ001SmbSpinBox->setEnabled(false);
	}
}

void 
DevSmbTab::hideOnboardSmbWidgets(bool hide)
{
	this->obSmbActiveChkBox->setHidden(hide);
}

void 
DevSmbTab::hideChamSmbWidgets(bool hide)
{
	numZ001Label->setHidden(hide);
	numZ001SmbSpinBox->setHidden(hide);
	z001SmbActiveChkBox->setHidden(hide);
}

//! implementation of updateProperties() 
/*!
  reads the current settings from Qt widgets and
  updates the device debug properties 
*/
void
DevSmbTab::updateProperties()
{
	wDebug(("DevSmbTab::updateProperties"));
	
	DevicePropertiesWithSmb *_smbProp;
	DeviceProperties *_prop = getMainDlg()->getProperties();
	WIZ_DYNAMIC_CAST(_prop, _smbProp, DevicePropertiesWithSmb* );
	MdiswizView *wizView;
	WIZ_DYNAMIC_CAST(getMainDlg()->parent(), wizView, MdiswizView* );
	
	_smbProp->obSmbControllerActive = obSmbActiveChkBox->isChecked();
	_smbProp->chamSmbControllersActive = z001SmbActiveChkBox->isChecked();
	_smbProp->numChamSmbControllers = numZ001SmbSpinBox->value();
	
	// signal "activateSmbController"
	emit activateOnboardSmb( _smbProp->obSmbControllerActive, wizView );
	emit activateChamSmb( _smbProp->chamSmbControllersActive, _smbProp->numChamSmbControllers, wizView );
}

//! implementation of setProperties()
/*!
  fills the Qt widgets for smb settings with
  values of internal device properties structure
*/
void
DevSmbTab::setProperties()
{
	wDebug(("DevSmbTab::setProperties"));
	
	DevicePropertiesWithSmb *_smbProp;
	DeviceProperties *_prop = getMainDlg()->getProperties();
	WIZ_DYNAMIC_CAST(_prop, _smbProp, DevicePropertiesWithSmb* );

	obSmbActiveChkBox->setChecked(_smbProp->obSmbControllerActive);
	z001SmbActiveChkBox->setChecked(_smbProp->chamSmbControllersActive);
	numZ001SmbSpinBox->setValue(_smbProp->numChamSmbControllers);
	
	wDebug(("leave DevSmbTab::setProperties"));
}


