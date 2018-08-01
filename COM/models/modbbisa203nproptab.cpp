/***************************************************************************
 */
/*!       \file  modbbisa203nproptab.cpp 
 *       \brief  Implementation of A203N family VME configuration tab
 *      \author	 dieter.pfeuffer@men.de
 *		  $Date: 2010/07/12 10:07:25 $
 *    $Revision: 2.1 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbisa203nproptab.cpp,v $
 * Revision 2.1  2010/07/12 10:07:25  dpfeuffer
 * R: access violation in mdiswiz_lib.lib under Windows with MDIS_CLINST
 * M: QComboBox replaced with Q3ComboBox
 *
 * Revision 2.0  2010/01/22 11:28:45  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.2  2009/08/21 14:47:19  CRuff
 * R: 1. MModule M199_A24 cannot be connected to BBIS, even if
 *       A32 switch is enabled
 * M: 1a) Change address mode capability of bbis slots if flag is enabled
 *    1b) inform user if A32 switch is disabled, but mmodules that need
 *        A24 address mode are connectd
 *
 * Revision 1.1  2005/08/02 11:20:44  dpfeuffer
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include "modbbisa203nproptab.h"
#include <q3memarray.h>
#include <qlayout.h>
#include <q3vbox.h>
#include <qlabel.h>
#include <q3buttongroup.h>
#include <q3combobox.h>
#include <qradiobutton.h>
#include <q3hgroupbox.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qvalidator.h>

#include <Q3HBoxLayout>
#include <QTimerEvent>
#include <Q3Frame>
#include <Q3VBoxLayout>

#include "wizdebug.h"
#include "dipswitch.h"
#include "hexeditwidget.h"
#include "propertiesdlg.h"
#include "wizexcept.h"
#include "vmsgbox.h"
#include "util.h"

//! creates the A203N family configuration tab
/*!
  \a _nSlots is the number of M-Module slots
*/
ModBbisA203NPropTab::ModBbisA203NPropTab(
	int _nSlots,
	QWidget *parent,
	DevPropertiesDlg *_mainDlg,
	const char *name, 
	Qt::WFlags f) : 
	DevPropertiesTab(parent,_mainDlg,name,f)
{
	QLabel *lab;

#ifndef WIN_MDISWIZ_LIB
	VmeBusInterface *busIf;
	WIZ_DYNAMIC_CAST( (getMainDlg()->getDevice()->getParent()), busIf, 
					  VmeBusInterface * );
#endif

	nSlots = _nSlots;
	timerId = 0;

	Q3VBoxLayout *vb = new Q3VBoxLayout( this );
	
	////////////////////////////////
	// vme A16/A24 addressing group
	////////////////////////////////
    Q3HGroupBox *group1 = new Q3HGroupBox( "VME A16/A24 Addressing", this);
	vb->addWidget(group1);

	Q3Frame *group1Frm = new Q3Frame( group1 );
	Q3VBoxLayout *group1Vb1 = new Q3VBoxLayout( group1Frm );
	Q3HBoxLayout *group1Hb1 = new Q3HBoxLayout( group1Vb1 );
	Q3HBoxLayout *group1Hb2 = new Q3HBoxLayout( group1Vb1 );

	QLabel *lab2 = new QLabel( QString("VME Address"), group1Frm );	
	group1Hb1->addWidget( lab2 );
	vmeAddWdg = new HexEditWidget( 6, group1Frm );
	group1Hb1->addWidget( vmeAddWdg );
	group1Hb1->addStretch(1);

	Q3ButtonGroup *vmeModWdg = new Q3ButtonGroup( 1, Qt::Horizontal, 
												"VME Space", group1Frm);
	group1Hb1->addWidget( vmeModWdg );
    vmeA16Wdg = new QRadioButton( "A16", vmeModWdg );
    vmeA24Wdg = new QRadioButton( "A24", vmeModWdg );
	
	// vme A16/A24 addressing HW group
    Q3HGroupBox *group2 = new Q3HGroupBox( "A16/A24 HW Configuration", group1Frm);
	group2->setFrameStyle( Q3Frame::Panel | Q3Frame::Sunken );
	group2->setLineWidth( 3 );
	group1Hb2->addWidget(group2);

	Q3Frame *group2Frm = new Q3Frame( group2 );
	Q3HBoxLayout *group2Hb = new Q3HBoxLayout( group2Frm );

	lab = new QLabel( QString("Hex Switches "), group2Frm );	
	group2Hb->addWidget( lab );

	hexSpin1Wdg = new HexSpinBox ( 0, 0xf, 1, group2Frm );
	hexSpin1Wdg->setWrapping( TRUE );
	hexSpin1Wdg->setFixedWidth ( 50 ); 
	group2Hb->addWidget( hexSpin1Wdg );

	hexSpin2Wdg = new HexSpinBox ( 0, 0xf, 1, group2Frm );
	hexSpin2Wdg->setWrapping( TRUE );
	hexSpin2Wdg->setFixedWidth ( 50 ); 
	group2Hb->addWidget( hexSpin2Wdg );

	hexSpin3Wdg = new HexSpinBox ( 0, 0xf, 1, group2Frm );
	hexSpin3Wdg->setWrapping( TRUE );
	hexSpin3Wdg->setFixedWidth ( 50 ); 
	group2Hb->addWidget( hexSpin3Wdg );

	group2Hb->addStretch( 1 );

	lab = new QLabel( QString("     A16 Jumper "), group2Frm );	
	group2Hb->addWidget( lab );

	chkBoxA16 = new QCheckBox( group2Frm );
	group2Hb->addWidget( chkBoxA16 );

	////////////////////////////////
	// vme A32 addressing group
	////////////////////////////////
    Q3HGroupBox *group3 = new Q3HGroupBox( "VME A32 Addressing", this);
	vb->addWidget(group3);

	Q3Frame *group3Frm = new Q3Frame( group3 );
	Q3HBoxLayout *group3Hb = new Q3HBoxLayout( group3Frm );


	vmeA32AddWdgLab = new QLabel( QString("VME Address"), group3Frm );	
	group3Hb->addWidget( vmeA32AddWdgLab );
	vmeA32AddWdg = new HexEditWidget( 8, group3Frm );
	group3Hb->addWidget( vmeA32AddWdg );
	group3Hb->addStretch(1);

	lab = new QLabel( QString("     A32 Space Enabled "), group3Frm );	
	group3Hb->addWidget( lab );

	chkBoxA32 = new QCheckBox( group3Frm );
	group3Hb->addWidget( chkBoxA32 );

	////////////////////////////////
	// slot settings
	////////////////////////////////
	for( int slt=0; slt<nSlots; slt++ ){
		Q3HGroupBox *grp = new Q3HGroupBox( QString("Slot %1").arg(slt), this);
		vb->addWidget(grp);
		
		lab = new QLabel( QString("IRQ Vector"), grp );	
		lab->setAlignment( Qt::AlignRight );
		vectorSpBox[slt] = new QSpinBox( 1, 255, 1, grp );
		
		// make a combobox with supported irq levels
		lab = new QLabel( QString("IRQ Level"), grp );	
		lab->setAlignment( Qt::AlignRight );
		levelSpBox[slt] = new Q3ComboBox( false, grp );

#ifndef WIN_MDISWIZ_LIB
		ulong irqs = busIf->supportedIrqLevels();
#else
		ulong irqs = 0xfe; // level 7-1
#endif
		for( int lvl=1; lvl<=7; lvl++ )
			if( irqs & (1L<<lvl) )
				levelSpBox[slt]->insertItem(QString("%1").arg(lvl));

#ifdef WIN_MDISWIZ_LIB
		// make a checkbox for "force found"
		lab = new QLabel( QString("Force Found"), grp );	
		lab->setAlignment( Qt::AlignRight );
		forceFoundChBox[slt] = new QCheckBox( false, grp );
#endif

	}

	connect( chkBoxA16, SIGNAL( toggled( bool )),
			 this, SLOT( slotChkBoxToggled( bool )));
	connect( chkBoxA32, SIGNAL( toggled( bool )),
			 this, SLOT( slotChkBoxA32Toggled( bool )));

	connect( hexSpin1Wdg, SIGNAL( valueChanged( int )),
			 this, SLOT( slotHexSpinChanged( int )));
	connect( hexSpin2Wdg, SIGNAL( valueChanged( int )),
			 this, SLOT( slotHexSpinChanged( int )));
	connect( hexSpin3Wdg, SIGNAL( valueChanged( int )),
			 this, SLOT( slotHexSpinChanged( int )));

	connect( vmeAddWdg, SIGNAL( textChanged(const QString&)),
			 this, SLOT( slotVmeAddrTextChanged()));
	connect( vmeA32AddWdg, SIGNAL( textChanged(const QString&)),
			 this, SLOT( slotVmeAddrTextChanged()));
	connect( vmeA16Wdg, SIGNAL( toggled( bool )),
			 this, SLOT( slotSpaceRbChanged( bool )));
}

//! destructs ModBbisA203NPropTab
ModBbisA203NPropTab::~ModBbisA203NPropTab()
{
}


void
ModBbisA203NPropTab::updateProperties()
{
	ModBbisA203NProperties *_prop;
	WIZ_DYNAMIC_CAST( getMainDlg()->getProperties(), _prop, 
					  ModBbisA203NProperties *);

	timerEvent(0); // force address update

	wDebug(("ModBbisA203NPropTab::updateProperties"));
	for( int slt=0; slt<nSlots; slt++ ){
		_prop->irqVectors[slt] = vectorSpBox[slt]->value();
		_prop->irqLevels[slt] = levelSpBox[slt]->currentText().toInt();
#ifdef WIN_MDISWIZ_LIB
		_prop->forceFound[slt] = forceFoundChBox[slt]->isChecked();
#endif
	}
}

void
ModBbisA203NPropTab::setProperties()
{
	wDebug(("ModBbisA203NPropTab::setProperties parent=%p", parent()));
	ModBbisA203NProperties *_prop;
	WIZ_DYNAMIC_CAST( (getMainDlg()->getProperties()), _prop, 
					  ModBbisA203NProperties *);

	if( _prop->isA16Space )
		vmeA16Wdg->setChecked( true );
	else
		vmeA24Wdg->setChecked( true );
	chkBoxA16->setChecked( _prop->isA16Space);
	vmeAddWdg->setDigits( _prop->isA16Space ? 4 : 6 );

	addrToSwitches( _prop->vmeAddr );
	vmeAddWdg->setValue( _prop->vmeAddr );

	chkBoxA32->setChecked( _prop->a32Enabled );
	slotChkBoxA32Toggled( _prop->a32Enabled );
	vmeA32AddWdg->setValue( _prop->vmeA32Addr );
	
	for( int slt=0; slt<nSlots; slt++ ){
		vectorSpBox[slt]->setValue(_prop->irqVectors[slt]);
		levelSpBox[slt]->setCurrentText(QString("%1").arg(_prop->irqLevels[slt]));
#ifdef WIN_MDISWIZ_LIB
		forceFoundChBox[slt]->setChecked( _prop->forceFound[slt] );
#endif
	}
}

// set DIP switches according to <adr>
void
ModBbisA203NPropTab::addrToSwitches(ulong adr)
{
	hexSpin3Wdg->setValue( (adr >> 12) & 0xf );
	hexSpin2Wdg->setValue( (adr >> 16) & 0xf );
	hexSpin1Wdg->setValue( (adr >> 20) & 0xf );
}

//! called when the check box toggles
void 
ModBbisA203NPropTab::slotChkBoxToggled( bool state )
{
	ModBbisA203NProperties *_prop;
	WIZ_DYNAMIC_CAST( getMainDlg()->getProperties(), _prop, 
					  ModBbisA203NProperties *);
	if( state ) {
		vmeA16Wdg->setChecked( true );
		_prop->isA16Space = true;
	}
	else {
		vmeA24Wdg->setChecked( true );
		_prop->isA16Space = false;
	}
}

//! called when the check box toggles
void 
ModBbisA203NPropTab::slotChkBoxA32Toggled( bool state )
{
	ModBbisA203NProperties *_prop;
	WIZ_DYNAMIC_CAST( getMainDlg()->getProperties(), _prop, 
					  ModBbisA203NProperties *);

	_prop->a32Enabled = state;
	vmeA32AddWdg->setEnabled(state);
	vmeA32AddWdgLab->setEnabled(state);
	
	// set A24 address mode flag if A32 Space is Enabled
	ModBbisA203N* bbisDevice;
	WIZ_DYNAMIC_CAST( getMainDlg()->getDevice(), bbisDevice,
					  ModBbisA203N * );
	
	HwComponent::MmodAddrCaps addrType = HwComponent::MmodA08;
	if (state == true) {
		addrType = HwComponent::MmodA24;
	}
	
	for( int slotNum=0; slotNum<bbisDevice->numSlots(); slotNum++ ){
		BbisSlot* slot = bbisDevice->bbSlot(slotNum);
		slot->setMmodACap(addrType);
	}
	
	if (state == false) {
		// check if child device cannot be accessed in A08 mode
		HwComponent* child = bbisDevice->firstChild();

		while ( child != NULL ) {
			HwComponent* dev = child->firstChild();
			
			if (dev != NULL) {
				MdisDevice* mdisDev;
				WIZ_DYNAMIC_CAST(dev, mdisDev, MdisDevice*);
				
				if (mdisDev->mmodAddrRequirements() > addrType) {
					QString errMsg = QString("Device '%1' cannot be accessed if A32 flag is disabled")
					                 .arg(mdisDev->getInstName());
					// warn user: this device cannot be used properly
					VMsgBox::informationOk( "Change VME Address Mode",
								"Warning: Disabling VME32 switch affects child devices",
								errMsg,
								getMainDlg() );
				}
			} 
			
			child = bbisDevice->nextChild();
		}
	}
}

//! called when any hex switch changes
void 
ModBbisA203NPropTab::slotHexSpinChanged( int value )
{
	ModBbisA203NProperties *_prop;
	WIZ_DYNAMIC_CAST( getMainDlg()->getProperties(), _prop, 
					  ModBbisA203NProperties *);

	ulong adr;

	UTIL_UNREF_PARAM(value);

	adr = hexSpin3Wdg->value() << 12;

	// A24?
	if( _prop->isA16Space == FALSE ){
		adr |= hexSpin2Wdg->value() << 16;
		adr |= hexSpin1Wdg->value() << 20;
	}

	wDebug(("ModBbisA203NPropTab::slotHexSpinChanged adr from swtch=%lx", adr));

	vmeAddWdg->setValue( adr );
	_prop->vmeAddr = adr;
}

//!called when VME address widget text changed
/*!
  this starts a 1 second timer to periodically validate the VME address
  and to update the switches. 
*/
void
ModBbisA203NPropTab::slotVmeAddrTextChanged()
{
	//wDebug(("ModBbisA203NPropTab::slotVmeAddrTextChanged"));
	if( timerId )
		killTimer( timerId );
	timerId = startTimer(1000);
}

// see above function
void
ModBbisA203NPropTab::timerEvent( QTimerEvent *e )
{
	ModBbisA203NProperties *_prop;
	WIZ_DYNAMIC_CAST( getMainDlg()->getProperties(), _prop, 
					  ModBbisA203NProperties *);

	ulong oldadr = vmeAddWdg->getValue();
	ulong adr = oldadr & 0xfff800;
	e = e;

	ulong oldadrA32 = vmeA32AddWdg->getValue();
	ulong adrA32 = oldadrA32 & 0xf0000000;

	wDebug(("ModBbisA203NPropTab::slotAddrChanged adr=%lx, adrA32=%lx",
		adr, adrA32));

	if( _prop->isA16Space )
		adr &= 0xffff;

	if( oldadr != adr ){
		vmeAddWdg->setValue( adr );
	}

	if( oldadrA32 != adrA32 ){
		vmeA32AddWdg->setValue( adrA32 );
	}

	_prop->vmeAddr = adr;
	_prop->vmeA32Addr = adrA32;

  	// update VME address on switches
	addrToSwitches( adr );
	killTimer( timerId );
}

//! called when VME space radio buttons changed
void
ModBbisA203NPropTab::slotSpaceRbChanged( bool state )
{
	ModBbisA203NProperties *_prop;
	WIZ_DYNAMIC_CAST( getMainDlg()->getProperties(), _prop, 
					  ModBbisA203NProperties *);

	wDebug(("ModBbisA203NPropTab::slotSpaceRbChanged a16checked=%d",
		   vmeA16Wdg->isChecked()));
	state = state;

	_prop->isA16Space = vmeA16Wdg->isChecked();
	if( _prop->isA16Space )	
		_prop->vmeAddr &= 0xffff;
		
	chkBoxA16->setChecked( _prop->isA16Space);
	vmeAddWdg->setDigits( _prop->isA16Space ? 4 : 6 );
	vmeAddWdg->setValue( _prop->vmeAddr );
	addrToSwitches( _prop->vmeAddr );
}


