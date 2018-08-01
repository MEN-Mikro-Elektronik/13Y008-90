/***************************************************************************
 */
/*!       \file  tristateswitch.cpp
 *       \brief  a Qt widget that incorporates a tristate switch
 *      \author	 Christian.Schuster@men.de
 *		  $Date: 2010/01/22 11:28:03 $
 *    $Revision: 2.0 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: tristateswitch.cpp,v $
 * Revision 2.0  2010/01/22 11:28:03  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.1  2006/11/30 11:28:03  cs
 * Initial Revision
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2006-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include "tristatebutton.h"
#include "tristateswitch.h"
#include "wizdebug.h"
#include <qlayout.h>
#include <qlabel.h>
#include <q3memarray.h>

#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <Q3Frame>
#include <Q3MemArray>

//! create a Tristate switch
/*!
  \param _grpId the group ID for the entire switch. Passed through to
  switchChanged()

  \param _nSwitches number of DipButton elements in that switch.
  \param parent parent widget
  \param name internal (non visible) name
*/
TristateSwitch::TristateSwitch(
	int _grpId, int _nSwitches, QWidget *parent, const char *name,
	const char *labPos1, const char *labPos2, const char *labPos3 ) :
	QWidget( parent, name )
{
	// set frames geometry
	Q3HBoxLayout *hbox = new Q3HBoxLayout( this, 0, 0, (const char *)0 );
	Q3Frame *hbFrame = new Q3Frame( this );
	hbox->addWidget( hbFrame );

	Q3VBoxLayout *vbox = new Q3VBoxLayout( hbFrame, 0, 0, (const char *)0 );
	QLabel *lab;
	QFont labelFont("Helvetica", 8, QFont::Normal);

	TristateButton *but;

	grpId	  = _grpId;
	nSwitches = _nSwitches;

	//wDebug(("TristateSwitch %p %d %d", this, grpId, nSwitches ));
	swWidgets = new Q3MemArray<TristateButton *>(nSwitches);

	vbox->addSpacing( 13 );

	if( labPos1 ) {
		lab = new QLabel( labPos1, hbFrame );
		lab->setAlignment( Qt::AlignRight | Qt::AlignBottom );
		lab->setFont( labelFont );
		lab->setMaximumHeight( 25 );
		vbox->addWidget( lab );
		vbox->addSpacing( 0 );
	} else {
		vbox->addSpacing( 12 );
	}

	if( labPos2 ) {
		lab = new QLabel( labPos2, hbFrame );
		lab->setAlignment( Qt::AlignRight | Qt::AlignBottom );
		lab->setFont( labelFont );
		lab->setMaximumHeight( 25 );
		vbox->addWidget( lab );
		vbox->addSpacing( 0 );
	} else {
		vbox->addSpacing( 12 );
	}

	if( labPos3 ) {
		lab = new QLabel( labPos3, hbFrame );
		lab->setAlignment( Qt::AlignRight | Qt::AlignBottom );
		lab->setFont( labelFont );
		lab->setMaximumHeight( 25 );
		vbox->addWidget( lab );
		vbox->addSpacing( 0 );
	}
	vbox->addStretch( 15 );

	// label is always right next to box (no stretch here)
	hbox->addSpacing( 3 );

	for( int swtch=0; swtch<nSwitches; swtch++ ){

		vbox = new Q3VBoxLayout( hbox,0,0 );

		lab = new QLabel( this );
		lab->setText( QString("%1").arg(swtch) );
		lab->setAlignment( Qt::AlignHCenter );
		lab->setFont( labelFont );
		lab->setBackgroundMode( Qt::PaletteDark );

		vbox->addWidget( lab );

		but = new TristateButton( this, swtch, TristateButton::pos2 );
		swWidgets->at(swtch) = but;
		but->setBackgroundMode( Qt::PaletteDark );
		vbox->addWidget( but );

		vbox->addStretch( 15 );
		connect( but, SIGNAL( triButStateChange( int, TristateButton::TriState )),
				 this, SLOT( slotSwitchChanged( int, TristateButton::TriState ) ));
	}
}

//! destructs DipSwitch
TristateSwitch::~TristateSwitch()
{
	delete swWidgets;
}

//! get the state of switch number \a sw
/*!
  \return state of the specified switch or \c false if \a sw is out of range
*/
TristateButton::TriState
TristateSwitch::getSwitchState( int sw )
{
	if( sw < nSwitches ) {
		//wDebug(("TristateSwitch::getSwitchState %d is %d",
		//	   sw, swWidgets->at(sw)->getState() ));
		return swWidgets->at(sw)->getState();
	}
	else {
		wDebug(("TristateSwitch::getSwitchState bad sw %p %d %d", this, sw, nSwitches));
		return TristateButton::posInval;
	}
}

//! set the state of switch \a sw.
void
TristateSwitch::setSwitchState( int sw, TristateButton::TriState state )
{
	if( sw < nSwitches )
		swWidgets->at(sw)->setState( state );
}

//! called when any button is toggled. Emits switchChanged()
void
TristateSwitch::slotSwitchChanged( int butId, TristateButton::TriState state )
{
	//wDebug(( "TristateSwitch:: But state change butId %d state %d", butId, state ));
	emit switchChanged( grpId, butId, state );
}

