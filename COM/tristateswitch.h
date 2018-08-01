/***************************************************************************
 */
/*!       \file  tristateswitch.h
 *       \brief  a Qt widget that incorporates a tristate switch
 *      \author	 Christian.Schuster@men.de
 *		  $Date: 2010/01/22 11:28:05 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: tristateswitch.h,v $
 * Revision 2.0  2010/01/22 11:28:05  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.1  2006/11/30 11:28:04  cs
 * Initial Revision
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2006-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#ifndef TRISTATESWITCH_H
#define TRISTATESWITCH_H
#include <qwidget.h>

#include <Q3MemArray>

#include "tristatebutton.h"

class TristateButton;

//! a Qt widget that looks like a Tristate switch
/*!
  a TristateSwitch may have a configurable number of TristateButton elements.

  Each TristateSwitch can be assigned a group ID. The group ID is passed along
  with the switch ID to the switchChanged() signal that is emitted
  whenever a switch is toggled by the user.
*/

class TristateSwitch : public QWidget
{
	Q_OBJECT
public:
//	enum State { pos1, pos2, pos3 };

    TristateSwitch( int _grpId, int _nSwitches, QWidget *parent = 0,
			   const char *name = 0,
			   const char *lab1 = 0, const char *lab2 = 0, const char *lab3 = 0 );
	~TristateSwitch();

	TristateButton::TriState getSwitchState( int sw );
	void setSwitchState( int sw, TristateButton::TriState state );

private:
	int nSwitches;
	int grpId;
	Q3MemArray<TristateButton *> *swWidgets;

protected slots:
	void slotSwitchChanged( int butId, TristateButton::TriState state );

signals:
    //! emitted whenever any button of the Tristate switch is changed
    void switchChanged( int grpId, int butId, TristateButton::TriState state );
};

#endif // TRISTATESWITCH_H

