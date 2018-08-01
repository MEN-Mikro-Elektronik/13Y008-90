/***************************************************************************
 */
/*!       \file  tristatebutton.h
 *       \brief  special pushbutton for a Tristate Switch
 *      \author	 Christian.Schuster@men.de
 *		  $Date: 2010/01/22 11:28:00 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: tristatebutton.h,v $
 * Revision 2.0  2010/01/22 11:28:00  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.1  2006/11/30 11:28:02  cs
 * Initial Revision
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2006-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#ifndef TRISTATEBUTTON_H
#define TRISTATEBUTTON_H

#include <qradiobutton.h>
#include <q3buttongroup.h>
#include <qsize.h>


//! a special pushbutton for a TristateSwitch
/*!
  a number of TristateButtons make up a TristateSwitch.

  A TristateButton is a normal pushbutton that changes it's bitmap every
  time the button is pressed (from on to neutral to off and back to on)

  Whenever the button is toggled, it emits butStateChange()
*/
class TristateButton : public QWidget
{
	Q_OBJECT
public:
	enum TriState { pos1=1, pos2=2, pos3=3, posInval=-1 };
	TristateButton( QWidget *parent, int _butId, TriState defState );

	//! set current state of button (doesn't emit butStateChanged())
	void setState( TriState _state ) { state=_state; slotSetState(); };

	//! get current state of button
	TriState getState() { return state; };

protected slots:
    void slotClicked(int id);

    //! sets/unsets corresponding buttons
	void slotSetState(){
		switch( state ) {
		case( pos1 ): bgroup->setButton(0); break;
		case( pos3 ): bgroup->setButton(2); break;
		default:	  bgroup->setButton(1); break;
		}
	};

signals:
    //! emitted whenever the state of the button changes
    void triButStateChange( int butId, TristateButton::TriState state );

private:
	Q3ButtonGroup *bgroup;
	//RadioButton *rb1, *rb2, *rb3;
	TriState state;
	int butId;

};

#endif // TRISTATEBUTTON_H

