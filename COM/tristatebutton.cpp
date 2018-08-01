/***************************************************************************
 */
/*!       \file  tristatebutton.cpp
 *       \brief  special pushbutton for a TristateSwitch
 *      \author	 Christian.Schuster@men.de
 *		  $Date: 2010/01/22 11:27:57 $
 *    $Revision: 2.0 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: tristatebutton.cpp,v $
 * Revision 2.0  2010/01/22 11:27:57  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.2  2007/02/23 12:20:32  DPfeuffer
 * TristateButton::TristateButton(): widget size changed for WinXP
 *
 * Revision 1.1  2006/11/30 11:28:01  cs
 * Initial Revision
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2006-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#include "tristatebutton.h"
#include <qlayout.h>
#include <q3hgroupbox.h>
#include <q3hbox.h>
#include <q3vbox.h>
#include <q3buttongroup.h>

#include <Q3VBoxLayout>
#include <Q3HBoxLayout>

#include "wizdebug.h"
#include "util.h"

class Q3HBoxLayout;
class Q3VBoxLayout;
class QPainter;


//! create a TristateButton
/*!
  Creates a TristateButton widget Id \a _butId and the default state
  \a defState.

  \a butId passed through triButStateChange()
*/
TristateButton::TristateButton( QWidget *parent, int _butId, TriState defState ) :
	QWidget( parent )
{
	// widget as small as possible
	setFixedSize(14,44); // 44 is necessary to display the entire widget under WinXP
    Q3VBoxLayout *vbox = new Q3VBoxLayout( this, 0, 0, (const char *)0  );
    Q3HBoxLayout *box1 = new Q3HBoxLayout( vbox  );

	bgroup = new Q3ButtonGroup( 1, Qt::Horizontal, this );
    box1->addWidget( bgroup );

	// configure button group layout: as compact as possible
	bgroup->setExclusive(true);
    bgroup->layout()->setSpacing( 0 );
    bgroup->layout()->setMargin( 0 );
    bgroup->setInsideSpacing( 0 );  // no spaces between buttons
    bgroup->setInsideMargin( 0 );
    bgroup->setFlat(true); // no border
	bgroup->setBackgroundMode( Qt::PaletteDark );

	connect( bgroup, SIGNAL(clicked(int)), SLOT(slotClicked(int)) );

	// generate the three buttons
	QRadioButton *rb;
	rb = new QRadioButton( bgroup );
	rb = new QRadioButton( bgroup );
	rb = new QRadioButton( bgroup );

	state = defState;
	butId = _butId;
	slotSetState();
}

//! called when button clicked.
/*!
  toggles state and bitmap, emits triButStateChange()
*/
void
TristateButton::slotClicked(int id)
{
	//wDebug(("TristateButton::slotClicked %d", butId ));
	// toggle state and change pixmap

	UTIL_UNREF_PARAM(id);

	if( bgroup->selectedId() == 0 ) {
		state = pos1;
	} else if( bgroup->selectedId() == 2  ) {
		state = pos3;
	} else {
		state = pos2;
	}
	emit triButStateChange( butId, state );
}


