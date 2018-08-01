/***************************************************************************
 */
/*!       \file  modbbisa201proptab.cpp 
 *       \brief  Implementation of A201 family VME configuration tab
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/07/12 10:07:13 $
 *    $Revision: 2.1 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbisa201proptab.cpp,v $
 * Revision 2.1  2010/07/12 10:07:13  dpfeuffer
 * R: access violation in mdiswiz_lib.lib under Windows with MDIS_CLINST
 * M: QComboBox replaced with Q3ComboBox
 *
 * Revision 2.0  2010/01/22 11:28:36  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.4  2004/10/07 11:13:36  dpfeuffer
 * - modified for windows "Force Found" option
 *
 * Revision 1.3  2003/06/05 11:59:25  dschmidt
 * intermediate checkin
 *
 * Revision 1.2  2003/06/05 09:35:02  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:33:09  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include "modbbisa201proptab.h"
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
#include <Q3VBoxLayout>
#include <Q3Frame>
#include <QTimerEvent>

#include "wizdebug.h"
#include "dipswitch.h"
#include "hexeditwidget.h"
#include "propertiesdlg.h"
#include "wizexcept.h"

//! creates the A201 family configuration tab
/*!
  \a _nSlots is the number of M-Module slots
*/
ModBbisA201PropTab::ModBbisA201PropTab(
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
	
	// vme addressing group
    Q3HGroupBox *group1 = new Q3HGroupBox( "VME Addressing", this);
	vb->addWidget(group1);

	Q3Frame *group1Frm = new Q3Frame( group1 );
	Q3VBoxLayout *group1Vb = new Q3VBoxLayout( group1Frm );
	Q3HBoxLayout *group1Hb1 = new Q3HBoxLayout( group1Vb );
	Q3HBoxLayout *group1Hb2 = new Q3HBoxLayout( group1Vb );

	group1Vb->addStretch(10);

	QLabel *lab2 = new QLabel( QString("VME Address"), group1Frm );	
	group1Hb1->addWidget( lab2 );
	vmeAddWdg = new HexEditWidget( 6, group1Frm );
	group1Hb1->addWidget( vmeAddWdg );
	group1Hb1->addStretch(10);

	Q3ButtonGroup *vmeModWdg = new Q3ButtonGroup( 1, Qt::Horizontal, 
												"VME Space", group1Frm);
	group1Hb1->addWidget( vmeModWdg );
    vmeA16Wdg = new QRadioButton( "A16", vmeModWdg );
    vmeA24Wdg = new QRadioButton( "A24", vmeModWdg );
	
	group1Hb2->addStretch( 10 );
	dip1Wdg = new DipSwitch( 0, 8, group1Frm );
	lab = new QLabel( QString("S2"), group1Frm );	
	group1Hb2->addWidget( lab );
	group1Hb2->addWidget( dip1Wdg );
	group1Hb2->addStretch( 2 );

	lab = new QLabel( QString("S1"), group1Frm );	
	group1Hb2->addWidget( lab );
	dip2Wdg = new DipSwitch( 1, 8, group1Frm );
	group1Hb2->addWidget( dip2Wdg );
	group1Hb2->addStretch( 10 );

	// slot settings
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

	// connect slots
	connect( dip1Wdg, SIGNAL( switchChanged( int, int, bool )),
			 this, SLOT( slotDipChanged( int, int, bool )));
	connect( dip2Wdg, SIGNAL( switchChanged( int, int, bool )),
			 this, SLOT( slotDipChanged( int, int, bool )));
	connect( vmeAddWdg, SIGNAL( textChanged(const QString&)),
			 this, SLOT( slotVmeAddrTextChanged()));
	connect( vmeA16Wdg, SIGNAL( toggled( bool )),
			 this, SLOT( slotSpaceRbChanged( bool )));
}

//! destructs ModBbisA201PropTab
ModBbisA201PropTab::~ModBbisA201PropTab()
{
}


void
ModBbisA201PropTab::updateProperties()
{
	ModBbisA201Properties *_prop;
	WIZ_DYNAMIC_CAST( getMainDlg()->getProperties(), _prop, 
					  ModBbisA201Properties *);

	timerEvent(0); // force address update

	wDebug(("ModBbisA201PropTab::updateProperties"));
	for( int slt=0; slt<nSlots; slt++ ){
		_prop->irqVectors[slt] = vectorSpBox[slt]->value();
		_prop->irqLevels[slt] = levelSpBox[slt]->currentText().toInt();
#ifdef WIN_MDISWIZ_LIB
		_prop->forceFound[slt] = forceFoundChBox[slt]->isChecked();
#endif
	}
}

void
ModBbisA201PropTab::setProperties()
{
	wDebug(("ModBbisA201PropTab::setProperties parent=%p", parent()));
	ModBbisA201Properties *_prop;
	WIZ_DYNAMIC_CAST( (getMainDlg()->getProperties()), _prop, 
					  ModBbisA201Properties *);

	if( _prop->isA16Space )
		vmeA16Wdg->setChecked( true );
	else
		vmeA24Wdg->setChecked( true );
	switchA16setOn( _prop->isA16Space );
	vmeAddWdg->setDigits( _prop->isA16Space ? 4 : 6 );

	addrToSwitches( _prop->vmeAddr );
	vmeAddWdg->setValue( _prop->vmeAddr );
	
	for( int slt=0; slt<nSlots; slt++ ){
		vectorSpBox[slt]->setValue(_prop->irqVectors[slt]);
		levelSpBox[slt]->setCurrentText(QString("%1").arg(_prop->irqLevels[slt]));
#ifdef WIN_MDISWIZ_LIB
		forceFoundChBox[slt]->setChecked( _prop->forceFound[slt] );
#endif
	}
}


// convert the DIP switches to VME addr (don't care about vme space)
ulong 
ModBbisA201PropTab::switchesToAddr()
{
	ulong adr=0;
	int i;

	// compute VME address
	for( i=3; i<8; i++)
		if( dip1Wdg->getSwitchState(i) == true ) 
			adr |= (1L<<( i+8 ));

	for( i=0; i<8; i++)
		if( dip2Wdg->getSwitchState(i) == true ) 
			adr |= (1L<<( i+16 ));

	return adr;
}

// set DIP switches according to <adr>
void
ModBbisA201PropTab::addrToSwitches(ulong adr)
{
	int i;

	for( i=3; i<8; i++)
		dip1Wdg->setSwitchState(i, (adr & (1L<<( i+8 ))) != 0 ); 

	for( i=0; i<8; i++)
		dip2Wdg->setSwitchState(i, (adr & (1L<<( i+16 ))) != 0 ); 
	
}

// check if A16 DIP switch set
bool
ModBbisA201PropTab::switchA16isOn()
{
	return (dip1Wdg->getSwitchState(0) == false);
}

void
ModBbisA201PropTab::switchA16setOn(bool on)
{
	dip1Wdg->setSwitchState(0, !on);
}

//! called when any dipswitch changes
void 
ModBbisA201PropTab::slotDipChanged( int grpId, int butId, bool state )
{
	ModBbisA201Properties *_prop;
	WIZ_DYNAMIC_CAST( getMainDlg()->getProperties(), _prop, 
					  ModBbisA201Properties *);

	grpId = grpId;
	butId = butId;
	state = state;

	ulong adr;

	adr = switchesToAddr();
	
	wDebug(("ModBbisA201PropTab::slotDipChanged adr from swtch=%lx", adr));

	if( switchA16isOn() ) {
		adr &= 0xffff;
		vmeA16Wdg->setChecked( true );
		_prop->isA16Space = true;
	}
	else {
		vmeA24Wdg->setChecked( true );
		_prop->isA16Space = false;
	}

	vmeAddWdg->setValue( adr );
	_prop->vmeAddr = adr;
	vmeAddWdg->setDigits( _prop->isA16Space ? 4 : 6 );
}

//!called when VME address widget text changed
/*!
  this starts a 1 second timer to periodically validate the VME address
  and to update the switches. 
*/
void
ModBbisA201PropTab::slotVmeAddrTextChanged()
{
	//wDebug(("ModBbisA201PropTab::slotVmeAddrTextChanged"));
	if( timerId )
		killTimer( timerId );
	timerId = startTimer(1000);
}

// see above function
void
ModBbisA201PropTab::timerEvent( QTimerEvent *e )
{
	ModBbisA201Properties *_prop;
	WIZ_DYNAMIC_CAST( getMainDlg()->getProperties(), _prop, 
					  ModBbisA201Properties *);

	ulong oldadr = vmeAddWdg->getValue();
	ulong adr = oldadr & 0xfff800;
	e = e;

	wDebug(("ModBbisA201PropTab::slotAddrChanged adr=%lx", adr));

	if( _prop->isA16Space )
		adr &= 0xffff;

	if( oldadr != adr ){
		vmeAddWdg->setValue( adr );
	}

	_prop->vmeAddr = adr;

  	// update VME address on switches
	addrToSwitches( adr );
	killTimer( timerId );
}

//! called when VME space radio buttons changed
void
ModBbisA201PropTab::slotSpaceRbChanged( bool state )
{
	ModBbisA201Properties *_prop;
	WIZ_DYNAMIC_CAST( getMainDlg()->getProperties(), _prop, 
					  ModBbisA201Properties *);

	wDebug(("ModBbisA201PropTab::slotSpaceRbChanged a16checked=%d",
		   vmeA16Wdg->isChecked()));
	state = state;

	_prop->isA16Space = vmeA16Wdg->isChecked();
	if( _prop->isA16Space )	
		_prop->vmeAddr &= 0xffff;
		
	switchA16setOn( _prop->isA16Space );
	vmeAddWdg->setDigits( _prop->isA16Space ? 4 : 6 );
	vmeAddWdg->setValue( _prop->vmeAddr );
	addrToSwitches( _prop->vmeAddr );
}
 








