/***************************************************************************
 */
/*!       \file  hexeditwidget.h
 *       \brief  a lineedit widget that allows to enter hexchars only.h
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:26:08 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: hexeditwidget.h,v $
 * Revision 2.0  2010/01/22 11:26:08  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.1  2003/05/02 14:34:43  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:20:07  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef HEXEDITWIDGET_H
#define HEXEDITWIDGET_H

#include <qlineedit.h>
#include <qvalidator.h>

class HexWidValidator;

//! a lineedit widget that allows to enter hexchars only
/*!
  This Qt widget is an input box that allows the user to input 
  valid hexadecimal digits only. The digits are presented using
  a fixed font without a leading 0x.

  This widget is for 32 bit unsigned integers. The current value
  can be obtained using getValue() and set using setValue().

  Signals and slots are the same as for QLineEdit widgets.
*/
class HexEditWidget : public QLineEdit {
public:
	HexEditWidget( int _digits, QWidget *parent, const char *name = 0 );
	~HexEditWidget();
	ulong getValue();
	void setValue(ulong v);
	void setDigits( int _digits );
	//! get the number of digits
	int getDigits() { return digits; };
private:
	HexWidValidator *validator;
	int digits;
};


#endif


