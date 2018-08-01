/***************************************************************************
 */
/*!       \file  modbbisd203famproptab.cpp
 *       \brief  Properties tab for D203 family PCI M-Module carriers
 *      \author	 Christian.Schuster@men.de
 *		  $Date: 2010/07/12 10:07:40 $
 *    $Revision: 2.1 $
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbisd203famproptab.cpp,v $
 * Revision 2.1  2010/07/12 10:07:40  dpfeuffer
 * R: access violation in mdiswiz_lib.lib under Windows with MDIS_CLINST
 * M: QComboBox replaced with Q3ComboBox
 *
 * Revision 2.0  2010/01/22 11:29:00  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.1  2006/11/30 11:28:17  cs
 * Initial Revision
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2006-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <qlayout.h>
#include <q3vbox.h>
#include <q3hbox.h>
#include <q3hgroupbox.h>
#include <qlabel.h>
#include <q3combobox.h>
#include <qspinbox.h>
#include <qpixmap.h>

#include <Q3VBoxLayout>
#include <Q3Frame>

#include "modbbispcimmodproptab.h"
#include "modbbisd203famproptab.h"
#include "dipswitch.h"
#include "tristateswitch.h"
#include "propertiesdlg.h"
#include "wizdebug.h"
#include "wizexcept.h"
#include "resourcemng.h"

#include "images/d203_trig.xpm"

ModBbisD203FamPropTab::ModBbisD203FamPropTab(
	int nSlots,
	QWidget *parent,
	DevPropertiesDlg *_mainDlg,
	const char *name,
	Qt::WFlags f) :
	DevPropertiesTab(parent,_mainDlg,name,f)
{
	_nSlots = nSlots;

	Q3VBoxLayout *vb = new Q3VBoxLayout( this, 10 ); // whole box
	vb->addSpacing( 0 );

	Q3HGroupBox *grp = new Q3HGroupBox( 0, this);    // container in whole box
	vb->addWidget(grp);

	Q3Frame *grpFrm1 = new Q3Frame( grp );           // PXI setting frame
	Q3Frame *grpFrm2 = new Q3Frame( grp );           // Trigger bus pic frame
	Q3Frame *grpFrm3 = new Q3Frame( grp );           // M-Mod setting frame
	Q3VBoxLayout *grpF1Vb = new Q3VBoxLayout( grpFrm1 );
	Q3VBoxLayout *grpF3Vb = new Q3VBoxLayout( grpFrm3 );

	QLabel *lab = new QLabel( grpFrm1 );
	lab->setText( QString("PXI Trigger Lines") );
	grpF1Vb->addWidget( lab );

	grpF1Vb->addSpacing( 8 );


	Q3HGroupBox *grpPxi = new Q3HGroupBox( (const char*)0, grpFrm1);
	grpF1Vb->addWidget(grpPxi);

	Q3Frame *grpPxiFrm = new Q3Frame( grpPxi );
	Q3VBoxLayout *grpPxiFrmVb = new Q3VBoxLayout( grpPxiFrm );

	triPxiWdg = new TristateSwitch( 0, 8, grpPxiFrm, 0, "Source", 0, "Destination" );
	grpPxiFrmVb->addWidget( triPxiWdg );
	grpPxiFrmVb->addStretch( 2 );

	// M-Modules Trigger Lines
	lab = new QLabel( grpFrm3 );
	lab->setText( QString("M-Module Trigger Lines") );
	grpF3Vb->addWidget( lab );

	// set Trigger Line Mode for each M-Module
	for( int slt = 0; slt < _nSlots; slt++ ) {
		Q3HGroupBox *grpMm = new Q3HGroupBox( QString("Slot %1").arg(slt), grpFrm3);
		grpF3Vb->addWidget(grpMm);

		Q3Frame *grpMmFrm1 = new Q3Frame( grpMm );
		Q3Frame *grpMmFrm2 = new Q3Frame( grpMm );
		Q3VBoxLayout *grpMmFrm1Vb = new Q3VBoxLayout( grpMmFrm1 );
		Q3VBoxLayout *grpMmFrm2Vb = new Q3VBoxLayout( grpMmFrm2 );

		// get DipSwitch for TRIGA
		lab = new QLabel( QString("TRIGA"), grpMmFrm1 );

		grpMmFrm1Vb->addStretch(4);
		grpMmFrm1Vb->addWidget( lab );
		grpMmFrm1Vb->addStretch(4);
		triWdgTrigASlot[slt] = new TristateSwitch( 0, 8, grpMmFrm1, 0, "Source", 0, "Destination" );
		grpMmFrm1Vb->addWidget( triWdgTrigASlot[slt] );

		grpMmFrm1Vb->addStretch( 20 );

		// get DipSwitch for TRIGB
		lab = new QLabel( QString("TRIGB"), grpMmFrm2 );

		grpMmFrm2Vb->addStretch(4);
		grpMmFrm2Vb->addWidget( lab );
		grpMmFrm2Vb->addStretch(4);
		triWdgTrigBSlot[slt] = new TristateSwitch( 0, 8, grpMmFrm2, 0, "Source", 0, "Destination" );
		grpMmFrm2Vb->addWidget( triWdgTrigBSlot[slt] );

		grpMmFrm2Vb->addStretch( 20 );
	}

	if( _nSlots < 4 ) {
		grpFrm2->setFixedSize(   80, 180 );
	} else {
		grpFrm2->setFixedSize(   80, 360 );
	}
	grpFrm2->setMargin( 0 );
	grpFrm2->setLineWidth(2);
	grpFrm2->setPaletteBackgroundPixmap( QPixmap(d203_trig_xpm) );

	grpF1Vb->addStretch(4);
	grpF3Vb->addStretch(4);
	vb->addStretch(4);

}

void
ModBbisD203FamPropTab::updateProperties()
{
	ModBbisD203FamProperties *_prop;
	TristateButton::TriState state;

	WIZ_DYNAMIC_CAST( getMainDlg()->getProperties(), _prop,
					  ModBbisD203FamProperties *);

	wDebug(("ModBbisD203FamPropTab::updateProperties() %p", _prop ));

	// get PXI Trigger line settings
	_prop->pxiTrigSrc = _prop->pxiTrigDst = 0x00;
	for( int i = 7; i >= 0; i-- ){
		state = triPxiWdg->getSwitchState(i);
		_prop->pxiTrigSrc = _prop->pxiTrigSrc << 1;
		_prop->pxiTrigDst = _prop->pxiTrigDst << 1;
		if( state == TristateButton::pos1 ) {
			_prop->pxiTrigSrc |= 1;
		} else if( state == TristateButton::pos3 ) {
			_prop->pxiTrigDst |= 1;
		}
	}

	//wDebug(("updateProperties() PxiSrc   = %x; PxiDst   = %x",
	//		  _prop->pxiTrigSrc, _prop->pxiTrigDst ));


	// walk through M-Module slots
	for( int slt = 0; slt < _nSlots; slt++ ){
		_prop->trigASrc[slt] = _prop->trigADst[slt] = 0x00;
		_prop->trigBSrc[slt] = _prop->trigBDst[slt] = 0x00;
		for( int i = 7; i >= 0; i-- ){
			_prop->trigASrc[slt] = _prop->trigASrc[slt] << 1;
			_prop->trigADst[slt] = _prop->trigADst[slt] << 1;
			_prop->trigBSrc[slt] = _prop->trigBSrc[slt] << 1;
			_prop->trigBDst[slt] = _prop->trigBDst[slt] << 1;

			// TrigA
			state = triWdgTrigASlot[slt]->getSwitchState(i);
			//wDebug(("updateProperties() triWdgTrigASlot %d: %d", i, state ));
			if( state == TristateButton::pos1 ) {
				_prop->trigASrc[slt] |= 1;
			} else if( state == TristateButton::pos3 ) {
				_prop->trigADst[slt] |= 1;
			}

			//TrigB
			state = triWdgTrigBSlot[slt]->getSwitchState(i);
			//wDebug(("updateProperties() triWdgTrigBSlot %d: %d", i, state ));
			if( state == TristateButton::pos1 ) {
				_prop->trigBSrc[slt] |= 1;
			} else if( state == TristateButton::pos3 ) {
				_prop->trigBDst[slt] |= 1;
			}
		}
		//wDebug(("        Slot %d    TrigASrc = %x; TrigADst = %x\n"
		//        "                  TrigBSrc = %x; TrigBDst = %x", slt,
		//        _prop->trigASrc[slt], _prop->trigADst[slt],
		//        _prop->trigBSrc[slt], _prop->trigBDst[slt] ));
	}

}

void
ModBbisD203FamPropTab::setProperties()
{
	ModBbisD203FamProperties *_prop;
	int warn = FALSE;

	WIZ_DYNAMIC_CAST( (getMainDlg()->getProperties()), _prop,
					  ModBbisD203FamProperties *);

	wDebug(("ModBbisD203FamPropTab::setProperties() %p", _prop ));

	// PXI lines
	for( int i = 7; i >= 0; i-- ){
		// sanity check
		if( _prop->pxiTrigSrc & _prop->pxiTrigDst & (1 << i) ) {
			// issue warning and reset value for this line
			warn = TRUE;
			_prop->pxiTrigSrc &= ~(1 << i);
			_prop->pxiTrigDst &= ~(1 << i);
		}

		// set value
		if( _prop->pxiTrigSrc & (1 << i) ){
			triPxiWdg->setSwitchState( i, TristateButton::pos1 );
		} else if( _prop->pxiTrigDst & (1 << i) ){
			triPxiWdg->setSwitchState( i, TristateButton::pos3 );
		} else {
			triPxiWdg->setSwitchState( i, TristateButton::pos2 );
		}
	}

	// M-Module trigger lines
	for( int slt = 0; slt < _nSlots; slt++ ){
		for( int i = 7; i >= 0; i-- ){
			// sanity check TrigA
			if( _prop->trigASrc[slt] & _prop->trigADst[slt] & (1 << i) ) {
				// issue warning and reset value for this line
				warn = TRUE;

				_prop->trigASrc[slt] &= ~(1 << i);
				_prop->trigADst[slt] &= ~(1 << i);
			}

			// set value TrigA
			if( _prop->trigASrc[slt] & (1 << i) ){
				triWdgTrigASlot[slt]->setSwitchState( i, TristateButton::pos1 );
			} else if( _prop->trigADst[slt] & (1 << i) ){
				triWdgTrigASlot[slt]->setSwitchState( i, TristateButton::pos3 );
			} else {
				triWdgTrigASlot[slt]->setSwitchState( i, TristateButton::pos2 );
			}

			// sanity check TrigB
			if( _prop->trigBSrc[slt] & _prop->trigBDst[slt] & (1 << i) ) {
				// issue warning and reset value for this line
				warn = TRUE;

				_prop->trigBSrc[slt] &= ~(1 << i);
				_prop->trigBDst[slt] &= ~(1 << i);
			}
			// set value TrigB
			if( _prop->trigBSrc[slt] & (1 << i) ){
				triWdgTrigBSlot[slt]->setSwitchState( i, TristateButton::pos1 );
			} else if( _prop->trigBDst[slt] & (1 << i) ){
				triWdgTrigBSlot[slt]->setSwitchState( i, TristateButton::pos3 );
			} else {
				triWdgTrigBSlot[slt]->setSwitchState( i, TristateButton::pos2 );
			}
		}
	}
}

