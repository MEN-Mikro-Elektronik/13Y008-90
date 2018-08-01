/***************************************************************************
 */
/*!       \file  hexspinbox.h
 *       \brief  a spin box widget for hex values
 *      \author	 dieter.pfeuffer@men.de
 *		  $Date: 2010/01/22 11:26:10 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: hexspinbox.h,v $
 * Revision 2.0  2010/01/22 11:26:10  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.1  2005/08/02 11:20:36  dpfeuffer
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef HEXSPINBOX_H
#define HEXSPINBOX_H

#include <qspinbox.h>
#include <qvalidator.h>


class HexSpinBox : public QSpinBox

//! a spin box widget for hex values
/*!
  This Qt widget is a spin box that allows the user to choose 
  a hexadecimal value only. The digits are presented using
  a fixed font with a leading 0x.

  This widget is for 32 bit unsigned integers. The current value
  can be obtained using getValue() and set using setValue().

  Signals and slots are the same as for QSpinBox widgets.
*/
	
{
	public:
 
		HexSpinBox( int minValue, int maxValue, int step = 1,
			QWidget* parent=0, const char* name=0 ) :
		QSpinBox( minValue, maxValue, step,
			parent, name ) {};

		~HexSpinBox() {};

        QString mapValueToText( int value )
        {
			QString s;
			s.sprintf ( "%x", value );
			return s;
        }
};

#endif


