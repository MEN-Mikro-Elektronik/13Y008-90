/***************************************************************************
 */
/*!       \file  dipbutton.h
 *       \brief  special pushbutton for a DipSwitch
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:25:55 $
 *    $Revision: 1.2 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: dipbutton.h,v $
 * Revision 1.2  2010/01/22 11:25:55  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.1  2003/05/02 14:34:36  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:20:00  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#ifndef DIPBUTTON_H
#define DIPBUTTON_H

#include <qpushbutton.h>
#include <qsize.h>

//! a special pushbutton for a DipSwitch
/*!
  a number of DipButtons make up a DipSwitch.

  A DipButton is a normal pushbutton that changes it's bitmap every
  time the button is pressed (from on to off and vice versa)

  Whenever the button is toggled, it emits butStateChange()
*/
class DipButton : public QPushButton 
{	
	Q_OBJECT
public:
	DipButton( QWidget *parent, int _butId, bool defState );
	QSize sizeHint() const;
	void drawButton( QPainter *paint );

	//! set current state of button (doesn't emit butStateChanged())
	void setState( bool _state ) { state=_state; pmSet(); };

	//! get current state of button
	bool getState() { return state; };

protected slots:
    void slotClicked();
	
signals:
    //! emitted whenever the state of the button changes
    void butStateChange( int butId, bool state );

private:
	void pmSet();
	bool state;
	int butId;

};

#endif
