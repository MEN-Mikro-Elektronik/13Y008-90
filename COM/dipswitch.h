/***************************************************************************
 */
/*!       \file  dipswitch.h
 *       \brief  a Qt widget that looks like a DIP switch
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:25:59 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: dipswitch.h,v $
 * Revision 2.0  2010/01/22 11:25:59  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.2  2006/11/30 11:27:36  cs
 * added:
 *   + optionally pass labels for state On/Off to constructor
 *
 * Revision 1.1  2003/05/02 14:34:38  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:20:03  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#ifndef DIPSWITCH_H
#define DIPSWITCH_H
#include <qwidget.h>

#include <Q3MemArray>

class DipButton;

//! a Qt widget that looks like a DIP switch
/*!
  a DipSwitch may have a configurable number of DipButton elements.

  Each DipSwitch can be assigned a group ID. The group ID is passed along
  with the switch ID to the switchChanged() signal that is emitted
  whenever a switch is toggled by the user.
*/

class DipSwitch : public QWidget
{
	Q_OBJECT
public:

    DipSwitch( int _grpId, int _nSwitches, QWidget *parent = 0,
			   const char *name = 0,
			   const char *labOn = 0, const char *labOff = 0 );
	~DipSwitch();

	bool getSwitchState( int sw );
	void setSwitchState( int sw, bool state );

private:
	int nSwitches;
	int grpId;
	Q3MemArray<DipButton *> *swWidgets;

protected slots:
    void slotSwitchChanged( int butId, bool state );

signals:
    //! emitted whenever any button of the DIP switch is changed
    void switchChanged( int grpId, int butId, bool state );
};

#endif


