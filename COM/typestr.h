/***************************************************************************
 */
/*!       \file  typestr.h
 *       \brief  Template class for type/string value pairs
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:28:07 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: typestr.h,v $
 * Revision 2.0  2010/01/22 11:28:07  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.3  2004/06/08 17:25:48  kp
 * Initial Release for Linux 1.0.010-linux1
 * fixed gcc3 compiler warning
 *
 * Revision 1.2  2004/01/09 08:20:36  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:36:34  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#ifndef TYPESTR_H
#define TYPESTR_H

#include <qstring.h>
#include <qmap.h>
//----------------------------------------------------------------------
//! map between an integer type (enum) and their string representations
/*!
  Typical usage: Subclass it and provide only the constructor:

  Each enumeration shall include an "invalid" number that is -1.

  \code
  class MmodDataMap : public TypeStrMap<HwComponent::MmodDataCaps> {
  public:
	MmodDataMap() {
	  mmap[HwComponent::MmodDInvalid] = "Invalid";
	  mmap[HwComponent::MmodD16] = "D16";
	  mmap[HwComponent::MmodD32] = "D32";
	  }
  };
  \endcode

*/
template<class Key>
class TypeStrMap {
public:
	//! type for QMap
	typedef QMap<Key,QString> Map;
	typedef typename Map::Iterator MapIterator;

	//! find string for \a key
	/*!
	  \returns string or 0 if not found
	*/
	QString keyToStr( Key key ) {
	    MapIterator it = mmap.find( key );
		return it == mmap.end() ? QString::null : it.data();
	}

	//! find key for \a string
	/*!
	  \returns key or -1 if not found
	*/
	Key strToKey( QString str ) {
		MapIterator it;

		for ( it = mmap.begin(); it != mmap.end(); ++it ) {

			if( it.data() == str )
				return it.key();
		}
		return (Key)-1;
	}
protected:
	Map mmap; //!< holds the mapped elements
};


#endif


