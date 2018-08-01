/***************************************************************************
 */
/*!       \file  hexeditwidget.cpp 
 *       \brief  a lineedit widget that allows to enter hexchars only
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:26:06 $
 *    $Revision: 1.4 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: hexeditwidget.cpp,v $
 * Revision 1.4  2010/01/22 11:26:06  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.3  2003/06/05 12:00:55  dschmidt
 * intermediate checkin
 *
 * Revision 1.2  2003/06/05 09:33:20  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:34:42  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include "hexeditwidget.h"
#include "wizdebug.h"

//! class to validate hexstrings 
class HexWidValidator : public QValidator {
public:
	//! constructs HexWidValidator 
	HexWidValidator( QWidget *parent, const char *name = 0 ) : QValidator( 
		parent, name ){}

	//! checks if the current text is valid
	/*!
	  \return Intermediate, Acceptable, Invalid
	*/
	State validate( QString &input, int &pos ) const {
		int i;
		//wDebug(( "validate: %s %d", input.latin1(), pos ));

		if( pos==0 )
			return Intermediate;

		// check if all are valid hexchars
		for( i=0; i<pos; i++ ){
			if( (input[i] >= '0' && input[i] <= '9') ||
				(input[i] >= 'a' && input[i] <= 'f') ||
				(input[i] >= 'A' && input[i] <= 'F'))
				;
			else break;
		}
		if( i==pos )		
			return Acceptable;
		else
			return Invalid;
	}

	//! called when the input string is invalid, sets value to 0
	void fixup( QString &input ) const {
		input.sprintf("%0*lx", ((HexEditWidget *)parent())->getDigits(), 0L );
		//wDebug(("fixup"));
	}
};	

//! Creates a HexEditWidget
/*!
  \param _digits the number of digits to be used
  \param parent parent widget
  \param name widget internal name
*/
HexEditWidget::HexEditWidget( 
	int _digits, 
	QWidget *parent, 
	const char *name ) :
	QLineEdit( parent, name )
{
	QFont font("Courier", 12 );

	validator = new HexWidValidator( this );
	digits = _digits;

	setMaxLength(digits);	
	setValidator( validator );
	setFont( font );
}

//! destructs HexEditWidget
HexEditWidget::~HexEditWidget() 
{
	delete validator;
}

//! get the current value of the text displayed in the widget
ulong
HexEditWidget::getValue()
{
	return text().toULong( 0, 16 );
}
//! sets the current value of the widget
void
HexEditWidget::setValue(ulong v)
{
	QString t;
	t.sprintf("%0*lx", digits, v );
	setText(t);
}

//! set the number of digits
void
HexEditWidget::setDigits( int _digits )
{
	digits = _digits;
	setValue( getValue());
}



