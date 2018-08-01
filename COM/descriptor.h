/***************************************************************************
 */
/*!       \file  descriptor.h
 *       \brief  Representation of MDIS descriptors
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/05/25 10:48:30 $
 *    $Revision: 2.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: descriptor.h,v $
 * Revision 2.1  2010/05/25 10:48:30  CRuff
 * R: hide descriptor properties tab if all descriptor keys are hidden
 * M: added declaration for function hasVisibleChildren()
 *
 * Revision 2.0  2010/01/22 11:25:46  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.5  2006/11/30 11:27:32  cs
 * added:
 *   - configure whether help string ist to be printed before end of directory
 *
 * Revision 1.4  2004/06/08 17:25:15  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.3  2004/05/11 12:01:37  cs
 * added method print(stream, string, indent) method for DescriptorEntry class
 * added method isEmpty() for DescriptorEntryDirectory class
 *
 * Revision 1.2  2003/05/14 13:43:34  kp
 * added printout of descriptor helpText
 *
 * Revision 1.1  2003/05/02 14:34:32  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:19:56  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include <qstring.h>
#include <qlist.h>
#include <q3memarray.h>
#include <qmap.h>
#include <q3textstream.h>

#include <Q3MemArray>
#include <Q3PtrList>

class DescriptorEntry;
class DescriptorEntryDirectory;

// -----------------------------------------------------------------
//! representation of a single MDIS descriptor entry
/*!
  Each descriptor entry object has a key, type, help text and usually a value
  and a defaultValue (except for directories).

  The descriptor entry may be active or inactive. Inactive objects are
  not exported to the descriptor file.

  There is a special class DescriptorFactory to create descriptor entries
  easily.
*/
// Derived by: DescriptorEntryUint32, DescriptorEntryBinary,
// 			   DescriptorEntryString, DescriptorEntryDirectory

class DescriptorEntry
{
public:
	//! descriptor entry types
	enum DescType { Uint32, String, Binary, Directory, Invalid };

	DescriptorEntry();
	DescriptorEntry( const QString &_key, const QString &_helpText,
					 DescType _type);
	virtual ~DescriptorEntry();

	DescType getType();
	QString &getKey();
	QString &getHelpText();

	DescriptorEntryDirectory *getParent();			// get/set parent
	void setParent( DescriptorEntryDirectory *_parent );

	//! make an exact copy of this object
	virtual DescriptorEntry *clone() { return new DescriptorEntry(*this); }

	//! print Helptext to File
	virtual void print( Q3TextStream &stream, int indent );

	//! print the passed string as a comment to the output stream
	virtual void print( Q3TextStream &stream, QString &strtoprint, int indent );

	//! convert the object's value to a string
	/*!
	  if \a fromDefval is false, use \a value, else \a defaultValue
	*/
	virtual QString valueToString( bool fromDefVal=false)
		{ fromDefVal=fromDefVal; return QString(""); }

	//! fill object's \a value or \a defaultValue from \a str
	/*!
	  if \a toDefval is false, update \a value, else \a defaultValue.
	  \return true if conversion successfull
	*/
	virtual bool setValue( const QString &str, bool toDefVal=false )
		{ QString dummy=str; toDefVal=toDefVal; return false;}

	//! tell if the object is active. \sa setActive()
	bool isActive() { return active; }

	//! set object active. \sa isActive()
	/*!
	  If an descriptor entry is active, the entrie's value
	  is used to override the defaultvalue
	*/
	void setActive( bool activate ) { active=activate; }

	//! tell if the object is hidden
	bool isHidden() { return hidden; }

	//! set object hidden (not user modifiable)
	void setHidden(bool hid) { hidden=hid; }

	QString typeStr();
	QString path();
	DescriptorEntry *nextSibling(int descend=true);

protected:
	void printComment( Q3TextStream &stream, const QString &str, int indent );

	QString	key;			//!< entry name
	QString helpText;		//!< help text
	DescType type;			//!< descriptor entry type (U_INT32...)
	DescriptorEntryDirectory *parent; //!< parent directory
	bool active;			//!< entry is active
	bool hidden;			//!< entry is hidden (not user modifiable)
};

// -----------------------------------------------------------------
//! Descriptor directory
/*!
  A Descriptor directory serves as a container for other descriptor entries.

  The children can be added by addChild() and removed by removeChild().
  The list of children can be parsed using firstChild() and nextChild():

  \code
  for( DescriptorEntry *e=dir->firstChild(); e; e=dir->nextChild() )
      // do something with the child
  \endcode

  Alternatively, you can get direct access to the child list
  using getChildren().

  All children are owned by the DescriptorEntryDirectory object. When
  a child is removed, the child is deleted.
*/
class DescriptorEntryDirectory : public DescriptorEntry
{
public:
	DescriptorEntryDirectory( const QString &_key,
							  const QString &_helpText=0);

	~DescriptorEntryDirectory();

	// add child to list of children
	void addChild( DescriptorEntry *newEntry, bool overwrite=true );

	// remove child from list of children
	bool removeChild( DescriptorEntry *entry );

	// two methods to scan the list of children
	DescriptorEntry *firstChild();			// get first child
	DescriptorEntry *nextChild(); 			// get next child

	Q3PtrList<DescriptorEntry> &getChildren();	// get direct list access to
	                                        // children

	//! tell if a DescriptorEntryDirectory object is empty (doesn't have any children)
	bool isEmpty();

	void print( Q3TextStream &stream, int indent );

	// make a copy of entire hierarchy
	DescriptorEntry *clone();

	// take over data from another descriptor
	void take( DescriptorEntryDirectory *newDesc );

	// find entry by keyname and type
	DescriptorEntry *find( const QString &_key, DescType _type );

	// set whether help string ist to be printed on end of directory
	void printEndHelp( bool doPrint ){ _printEndHelp = doPrint; }
	
	// check if descriptor has visible children
	bool hasVisibleChildren();
protected:
	Q3PtrList<DescriptorEntry> lstChildren;	//!< list of children

private:
	bool _printEndHelp;					//!< print Helpstring at end of directory
};

// -----------------------------------------------------------------
//! Descriptor Entry for 32 bit unsigned integers
/*!
  U_INT32 descriptor entries provide additional features:
  - They have a valid \em range
  - The value can be selected from a list of choises

  If \em range shall be used, set the range using setRange(). By default,
  minimum is \c 0 and maximum is \c 0xffffffff. Any attempt to set \em
  value or \em defaultValue beyound the limits is ignored.

  To use \em choises, add each possible value to \em choises:
  \code
     choises[value] = description;
  \endcode
  After this has been done, isMultiChoise() returns true.
  To scan the list of choises, use the QMap::Iterator see QMap documentation
  for details.

  Don't use ranges and choises together.

  The entry also has a \em base radix (typically 10 or 16). The base
  is set either by the constructor or when converting the value from a string.

  When the entrie's value is converted to/from QStrings, the \em base
  radix is used for conversion
*/
class DescriptorEntryUint32 : public DescriptorEntry
{
public:
	//! type for multiple choise entries
	typedef QMap<ulong,QString> ChoisesMap;

	DescriptorEntryUint32( const QString &_key,
						   const QString &_helpText=0, int _base=16 );

	void setRange( ulong _minValue=0, ulong _maxValue=0xffffffff );

	bool isMultiChoise();

	ulong getDefaultValue();
	bool setDefaultValue( ulong _defaultValue );

	ulong getValue();
	bool setValue( ulong _value );
	void print( Q3TextStream &stream, int indent );

	DescriptorEntry *clone() { return new DescriptorEntryUint32(*this); }

	QString valueToString( bool fromDefVal=false);
	QString anyValueToString( ulong val );

	bool setValue( const QString &str, bool toDefVal=false );

	ChoisesMap choises; 	//!< list of choises

protected:
    ulong value;			//!< value
	ulong defaultValue;		//!< default value
	ulong minValue;			//!< minimum value (inclusive)
	ulong maxValue;			//!< maximum value (inclusive)
	int base;				//!< base radix (10 or 16)

};

// -----------------------------------------------------------------
//! Descriptor Entry for ASCII strings

class DescriptorEntryString : public DescriptorEntry
{
public:
	DescriptorEntryString( const QString &_key, const QString &_helpText=0);

	QString &getDefaultValue();
	void setDefaultValue( const QString &defaultValue );

	QString &getValue();
	void print( Q3TextStream &stream, int indent );
	DescriptorEntry *clone() { return new DescriptorEntryString(*this); }
	QString valueToString( bool fromDefVal=false);
	bool setValue( const QString &str, bool toDefVal=false );

protected:
	QString value;				//!< value
	QString defaultValue;		//!< default value
};

// -----------------------------------------------------------------
//! Descriptor Entry for arrays of 8 bit unsigned integers

class DescriptorEntryBinary : public DescriptorEntry
{
public:
	DescriptorEntryBinary( const QString &_key, const QString &_helpText=0);

	Q3MemArray<uchar> &getDefaultValue();
	void setDefaultValue( const Q3MemArray<uchar> &_defaultValue );

	Q3MemArray<uchar> &getValue();
	void setValue( const Q3MemArray<uchar> &_value );
	void print( Q3TextStream &stream, int indent );
	DescriptorEntry *clone() { return new DescriptorEntryBinary(*this); }
	QString valueToString( bool fromDefVal=false);
	bool setValue( const QString &str, bool toDefVal=false );

protected:
	Q3MemArray<uchar> value;			//!< value
	Q3MemArray<uchar> defaultValue;		//!< default value
};

// -----------------------------------------------------------------
//! A class to create DescriptorEntries
/*!
  \todo most functions should be static
*/
class DescriptorFactory
{
public:
	//! commonly used descriptor keys
	enum KeyType {
		DESC_TYPE, HW_TYPE, BOARD_NAME, DEVICE_SLOT,
		_WIZ_BUSIF, _WIZ_MODEL
	};

	// create U_INT32
	DescriptorEntry *create( const QString &key, ulong value,
							 const QString &helpText=0, bool active=true );
	// create STRING
	DescriptorEntry *create( const QString &key, const QString &value,
							 const QString &helpText=0, bool active=true );
	// create Binary
	DescriptorEntry *create( const QString &key, const Q3MemArray<uchar> &value,
							 const QString &helpText=0, bool active=true );

	// create entry based on string
	DescriptorEntry *create( const QString &key, const QString &typeStr,
							 const QString &value,
							 const QString &helpText, bool active );

	DescriptorEntry *createWizBusIf( const QString &parentName,
									 int busIfNum );


	// return type string (U_INT32 etc) based on _type
	QString typeStr( DescriptorEntry::DescType _type );

	// return common descriptor keys
	QString keyStr( KeyType _type );


};

#endif


