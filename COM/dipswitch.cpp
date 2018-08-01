/***************************************************************************
 */
/*!       \file  dipswitch.cpp
 *       \brief  a Qt widget that looks like a DIP switch
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:25:57 $
 *    $Revision: 2.0 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: dipswitch.cpp,v $
 * Revision 2.0  2010/01/22 11:25:57  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.4  2006/11/30 11:27:34  cs
 * added:
 *   + optionally pass labels for state On/Off to constructor
 *
 * Revision 1.3  2003/06/05 12:00:51  dschmidt
 * intermediate checkin
 *
 * Revision 1.2  2003/06/05 09:33:15  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:34:37  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <qlayout.h>
#include <qlabel.h>
#include <q3memarray.h>

#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <Q3MemArray>

#include "dipbutton.h"
#include "dipswitch.h"
#include "wizdebug.h"

//! create a DIP switch
/*!
  \param _grpId the group ID for the entire switch. Passed through to
  switchChanged()

  \param _nSwitches number of DipButton elements in that switch.
  \param parent parent widget
  \param name internal (non visible) name
*/
DipSwitch::DipSwitch(
	int _grpId, int _nSwitches, QWidget *parent, const char *name,
	const char *labOn, const char *labOff ) :
	QWidget( parent, name )
{
	Q3HBoxLayout *hbox;
	Q3VBoxLayout *vbox;
	QLabel *lab, *lab1, *lab2;
	QFont labelFont("Helvetica", 8, QFont::Normal);

	DipButton *but;


	grpId	  = _grpId;
	nSwitches = _nSwitches;
	wDebug(("DipSwitch %p %d %d", this, grpId, nSwitches ));
	swWidgets = new Q3MemArray<DipButton *>(nSwitches);

	hbox = new Q3HBoxLayout( this, 0, 0, (const char *)0 );
	vbox = new Q3VBoxLayout( hbox,0,0 );

	lab1 = new QLabel( " ", this );
	lab1->setMaximumHeight( 2 );
	vbox->addWidget( lab1 );
	vbox->addStretch( 10 );

	if( labOn ) {
		lab1 = new QLabel( labOn, this );
	} else {
		lab1 = new QLabel( "  on", this );
	}
	lab1->setAlignment( Qt::AlignHCenter | Qt::AlignBottom );
	lab1->setFont( labelFont );
	//lab->setBackgroundMode( Qt::PaletteDark );
	lab1->setMaximumHeight( 20 );
	vbox->addWidget( lab1 );
	vbox->addStretch( 20 );

	if( labOff ) {
		lab2 = new QLabel( labOff, this );
		lab2->setAlignment( Qt::AlignHCenter );
		lab2->setFont( labelFont );
		//lab->setBackgroundMode( Qt::PaletteDark );
		lab2->setMaximumHeight( 20 );
		vbox->addWidget( lab2 );
		vbox->addStretch( 10 );
	}

	for( int swtch=0; swtch<nSwitches; swtch++ ){

		vbox = new Q3VBoxLayout( hbox,0,0 );

		lab = new QLabel( this );
		lab->setText( QString("%1").arg(swtch+1) );
		lab->setAlignment( Qt::AlignHCenter );
		lab->setFont( labelFont );
		lab->setBackgroundMode( Qt::PaletteDark );

		vbox->addWidget( lab );

		but = new DipButton( this, swtch, 0 );
		swWidgets->at(swtch) = but;
		but->setBackgroundMode( Qt::PaletteDark );
		vbox->addWidget( but );

		vbox->addStretch( 10 );

		connect( but, SIGNAL( butStateChange( int, bool )),
				 this, SLOT( slotSwitchChanged( int, bool ) ));
	}

}

//! destructs DipSwitch
DipSwitch::~DipSwitch()
{
	delete swWidgets;
}

//! get the state of switch number \a sw
/*!
  \return state of the specified switch or \c false if \a sw is out of range
*/
bool
DipSwitch::getSwitchState( int sw )
{
	if( sw < nSwitches ) {
		wDebug(("DipSwitch::getSwitchState %d is %d",
			   sw, swWidgets->at(sw)->getState() ));
		return swWidgets->at(sw)->getState();
	}
	else {
		wDebug(("DipSwitch::getSwitchState bad sw %p %d %d", this, sw, nSwitches));
		return false;
	}
}

//! set the state of switch \a sw.
void
DipSwitch::setSwitchState( int sw, bool state )
{
	if( sw < nSwitches )
		swWidgets->at(sw)->setState( state );
}

//! called when any button is toggled. Emits switchChanged()
void
DipSwitch::slotSwitchChanged( int butId, bool state )
{
	wDebug(( "But state change butId %d state %d", butId, state ));
	emit switchChanged( grpId, butId, state );
}


