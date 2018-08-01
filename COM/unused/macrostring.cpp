/***************************************************************************
 */
/*!       \file  macrostring.cpp 
 *       \brief  Implementation of MacroString classes
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2003/05/02 14:33:52 $
 *    $Revision: 1.1 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: macrostring.cpp,v $
 * Revision 1.1  2003/05/02 14:33:52  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:27:10  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:19:09  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright by MEN mikro elektronik GmbH, Nuernberg, Germany
 ****************************************************************************/

#include "macrostring.h"
#include <stdlib.h>

//----------------------------------------------------------------------
//! creates a MacroString object
/*!
  
  This creates a MacroString object with the name \a key, an initial
  value \a initVal and adds it to the group \a msGrp.

  This object will be owned by \a msGrp. If the group is deleted, this
  object will be also deleted.  
*/
MacroString::MacroString( MacroStringGroup *msGrp, 
						  const QString &key,
						  const QString &initVal )
	: QString( initVal )
{
	_key = key;
	group = msGrp;
	msGrp->append( this );
}

//! return the string and expanded all macros
/*!
  Expansion is done recursively; i.e. if a macro contains other macros, they
  are expanded as well.

  If the string contains incorrectly terminated macros, e.g. "$(MACRO", it
  returns QString::null
*/
QString 
MacroString::expand()
{
	QString rval = *this, macName, expMacro;
	int startIdx, endIdx;

	// resolve macros
	while( (startIdx = rval.find( "$(" )) >= 0 ){

		if( (endIdx = rval.find( ')', startIdx )) < 0 )
			return QString::null;
		
		macName = rval.mid( startIdx+2, endIdx-startIdx-2 );

		qDebug("%s found macro %s", (const char *)rval, (const char *)macName);
		
		// expand found macro
		expMacro = group->expand( macName );

		// replace macro name in string with expanded macro
		rval = rval.replace( startIdx, endIdx-startIdx+1, expMacro );
	}
			
	return rval;
}

//----------------------------------------------------------------------
//! creates a MacroStringGroup object

MacroStringGroup::MacroStringGroup( const char *_name, bool _useEnv )
{
	name = _name;
	useEnv = _useEnv;
	setAutoDelete(true);
}

//! find the MacroString item with name \macName
/*!
  \return MacroString item or 0 if not found
  \remark Will not find environment macros
*/
MacroString *
MacroStringGroup::lookup( const QString &macName )
{
	MacroString *ms;

	for( ms=first(); ms; ms=next() ){
		if( ms->key() == macName )
			return ms;
	}
	return 0;
}

//! expand macro 
/*!
  Find the macro named \a macName in the group and expand the macro.

  If the macro name is not found in the group and if \a useEnv was set
  to \c true in the constructor, it will attempt to find an environment
  variable with the name \a macName.

  \return expanded macro or 0 if macro not found/invalid macro 

  \sa MacroString::expand()
*/
QString
MacroStringGroup::expand( const QString &macName )
{
	MacroString *ms;
	char *env;

	if( (ms = lookup( macName )) == 0 ){
		if( useEnv ){
			if( (env = getenv( (const char *)macName )))
				return (QString)env;
		}
		return QString::null;
	}
	return ms->expand();
}

