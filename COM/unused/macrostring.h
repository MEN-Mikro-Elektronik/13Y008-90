/***************************************************************************
 */
/*!       \file  macrostring.h
 *       \brief  class for string with macros
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2003/05/02 14:33:53 $
 *    $Revision: 1.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: macrostring.h,v $
 * Revision 1.1  2003/05/02 14:33:53  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:27:11  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:19:10  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright by MEN mikro elektronik GmbH, Nuernberg, Germany
 ****************************************************************************/
#ifndef MACROSTRING_H
#define MACROSTRING_H

#include <qstring.h>
#include <qlist.h>

class MacroStringGroup;

//! class for QString with macros
/*!
  \sa MacroStringGroup
*/
class MacroString : public QString {
public:
	MacroString( MacroStringGroup *msGrp, const QString &key, 
				 const QString &initVal=0 );

	QString expand();
	//! get the key name of the macro
	QString key() { return _key; };
private:
	QString _key;
	MacroStringGroup *group;
};

//! class for QStrings with macros
/*!
  This class manages a number of MacroString objects. Each of these objects
  have a name and a value. The value is a string that may include 
  Makefile-like macros, e.g.
  \code
  $(TARGET_TREE)/usr/bin
  \endcode

  In the example above, \c TARGET_TREE must be also a known MacroString object.

  To define a MacroString group and their objects, proceed as follows:
  \code
  	MacroStringGroup msGrp( "test", true );
	new MacroString ( &msGrp, "MACRO1", "ValueOfMacro1" );
	new MacroString ( &msGrp, "MACRO2", "ValueOfMacro2" );
	new MacroString ( &msGrp, "MACRO3", "x$(MACRO2)y$(MACRO1)" );
	new MacroString ( &msGrp, "MACRO4", "zzz$(MACRO3)ooo" );
	new MacroString ( &msGrp, "MACRO5", "$(HOME)$(MACRO3)" );
	
	s = msGrp.expand( "MACRO3" )); // s = xValueOfMacro2yValueOfMacro1z
	s = msGrp.expand( "MACRO4" )); // s = zzzxValueOfMacro2yValueOfMacro1ooo
	s = msGrp.expand( "MACRO5" )); // s = /home/kpxValueOfMacro2yValueOfMacro1~
  \endcode

  If \a useEnv is passed \c true to the constructor, macros can be also 
  resolved from environment variables.
  
*/

class MacroStringGroup : public QList<MacroString> {
public:
	MacroStringGroup( const char *_name, bool _useEnv=false );
	MacroString *lookup( const QString &macName );
	QString expand( const QString &macName );
private:
	const char *name;
	bool useEnv;
};


#endif
