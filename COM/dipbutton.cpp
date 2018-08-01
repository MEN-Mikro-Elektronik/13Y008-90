/***************************************************************************
 */
/*!       \file  dipbutton.cpp 
 *       \brief  special pushbutton for a DipSwitch
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2011/09/26 08:42:15 $
 *    $Revision: 2.1 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: dipbutton.cpp,v $
 * Revision 2.1  2011/09/26 08:42:15  dpfeuffer
 * R: cosmetics
 * M: cosmetics
 *
 * Revision 2.0  2010/01/22 11:25:53  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.2  2003/06/05 09:33:13  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:34:35  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <qpixmap.h>
#include <qpainter.h>
#include <qstyle.h>

#include "dipbutton.h"
#include "wizdebug.h"

static const char * p1_xpm[] = {
"7 16 3 1",
" 	c None",
".	c #000000000000",
"X	c #FFFFFFFFFFFF",
".......",
".......",
".XXXXX.",
".XXXXX.",
".XXXXX.",
".XXXXX.",
".......",
".......",
".......",
".......",
".......",
".......",
".......",
".......",
".......",
"......."};

static const char * p2_xpm[] = {
"7 16 3 1",
" 	c None",
".	c #000000000000",
"X	c #FFFFFFFFFFFF",
".......",
".......",
".......",
".......",
".......",
".......",
".......",
".......",
".......",
".......",
".XXXXX.",
".XXXXX.",
".XXXXX.",
".XXXXX.",
".......",
"......."};

//! create a DipButton
/*!
  Creates a DupBotton widget Id \a _butId and the default state
  \a defState.

  \a butId passed through butStateChange()
*/
DipButton::DipButton( QWidget *parent, int _butId, bool defState ) : 
	QPushButton( parent ) 
{
	setFlat(true); // no border
	state = defState;
	butId = _butId;
	pmSet();
	connect( this, SIGNAL(clicked()), this, SLOT(slotClicked()) );
}

void
DipButton::pmSet()
{
	QPixmap p1( p1_xpm );
	QPixmap p2( p2_xpm );
	
	setPixmap( state ? p2 : p1 );
}

//! reimplemented
QSize 
DipButton::sizeHint() const 
{ 
	return QSize( 13, 20 ); 
}

//! called when button clicked. 
/*!
  toggles state and bitmap, emits butStateChange()
*/
void 
DipButton::slotClicked()
{
	// toggle state and change pixmap 
	state = !state;
	pmSet();

	emit butStateChange( butId, state );
}

//! reimplements QPushButton::drawButton
/*!
  taken from qcommonstyle.cpp
  equired to avoid spaces between buttons
*/
void 
DipButton::drawButton( QPainter *paint )
{
	QIcon ic = icon();
	QSize siz = size();
	QPixmap pix = ic.pixmap( siz );

	style()->drawItemPixmap(  paint, rect(),
					  Qt::AlignCenter | Qt::TextShowMnemonic, pix );

	style()->drawItemText(  paint, rect(),
					  Qt::AlignCenter | Qt::TextShowMnemonic,
					  colorGroup(), isEnabled(), text() );
}

