/***************************************************************************
 */
/*!       \file  descriptor.cpp
 *       \brief   implementation of descriptor classes
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2012/12/20 13:16:31 $
 *    $Revision: 2.5 $
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: descriptor.cpp,v $
 * Revision 2.5  2012/12/20 13:16:31  ts
 * R: for F223 the enumeration of PCI bridges must be stopped after 1st bridge
 * M: introduced new key SKIP_DEV_BRIDGE_CHECK
 *
 * Revision 2.4  2011/03/31 16:37:11  CRuff
 * R: new descriptor key PCI_DOMAIN_NUMBER in pci key group
 * M: print pci key group info text also if PCI_DOMAIN_NUMBER is first pci key
 *
 * Revision 2.3  2010/07/05 17:50:21  CRuff
 * R: implementation of PCI Domains
 * M: also print PCI help when new PCI_BUS_PATH2 is written
 *
 * Revision 2.2  2010/05/25 10:48:54  CRuff
 * R: hide descriptor properties tab if all descriptor keys are hidden
 * M: added implementation for method hasVisibleChildren()
 *
 * Revision 2.1  2010/05/21 15:39:59  CRuff
 * R: handling of AUTOENUM keys needed
 * M: print section header for AUTOENUM keys
 *
 * Revision 2.0  2010/01/22 11:25:43  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.12  2009/09/03 10:37:37  CRuff
 * R: preparation for SMB support
 * M: print(): add help text for SMB parameters, PCI configuration
 *
 * Revision 1.11  2009/08/20 11:29:15  CRuff
 * R: Hints for chameleon devices were shown for all BBISes in the system
 *    descriptor
 * M: DescriptorEntryDirectory::print(): check HW_TYPE and only show hints for
 *    chameleon BBISes
 *
 * Revision 1.10  2009/05/11 16:34:43  CRuff
 * R: Cosmetics
 * M: DescriptorEntryDirectory::print(): added comment with Chameleon BBIS device
 *    descriptor explanation
 *
 * Revision 1.9  2006/11/30 11:27:30  cs
 * added:
 *   - configure whether help string ist to be printed before end of directory
 *
 * Revision 1.8  2004/10/07 11:13:22  dpfeuffer
 * cosmetics
 *
 * Revision 1.7  2004/06/08 17:25:13  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.6  2004/05/11 12:00:25  cs
 * added method isEmpty() for DescriptorEntryDirectory class
 * added method print(stream, string, indent) method for DescriptorEntry class
 * empty directories are not printed any more in *.dsc files
 * additional comments (group headers) are printed in *.dsc files
 *
 * Revision 1.5  2004/01/09 08:19:51  kp
 * cosmetics
 *
 * Revision 1.4  2003/06/05 12:00:46  dschmidt
 * intermediate checkin
 *
 * Revision 1.3  2003/06/05 09:33:09  kp
 * intermediate checkin
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <qstringlist.h>

#include <Q3TextStream>
#include <Q3MemArray>
#include <Q3PtrList>

#include "descriptor.h"
#include "wizdebug.h"
#include "wizexcept.h"
#include "util.h"

// -----------------------------------------------------------------

//! constructs an empty DescriptorEntry object
/*!
  \a key and \a helpText are set to null. \a type is set to Invalid
*/
DescriptorEntry::DescriptorEntry()
{
	key = QString::null;
	helpText = QString::null;
	type = Invalid;
	parent = 0;
	active = false;
	hidden = false;
}

//! constructs empty DescriptorEntry object
/*!
  \param _key 		the entry's key name
  \param _helpText 	help text
  \param _type		descriptor type
*/
DescriptorEntry::DescriptorEntry(
	const QString &_key, const QString &_helpText,
	DescType _type )
{
	key = _key;

	helpText = _helpText;
	type = _type;
	parent = 0;
	active = true;
	hidden = false;
}

//! destructs DescriptorEntry
DescriptorEntry::~DescriptorEntry()
{
}

//! get the type of the descriptor object
DescriptorEntry::DescType DescriptorEntry::getType()
{
	return type;
}

//! get the key name of the descriptor object
QString &
DescriptorEntry::getKey()
{
	return key;
}

//! get the help text of the descriptor object
QString &
DescriptorEntry::getHelpText()
{
	return helpText;
}

//! get the parent entry of this object
DescriptorEntryDirectory *
DescriptorEntry::getParent()
{
	return parent;
}

//! set the parent entry of this object
void
DescriptorEntry::setParent( DescriptorEntryDirectory *_parent )
{
	parent = _parent;
}

//! print the contents of this object to the output stream
/*!
  \param stream		the output stream
  \param indent		the indentation level (0,1...)

  \remark DescriptorEntry::print() only prints the helptext - if any.
*/
void
DescriptorEntry::print( Q3TextStream &stream, int indent )
{
	stream.reset();

	// print the helptext as a comment
	if( !helpText.isEmpty() ){
		stream << '\n';
		printComment( stream, helpText, indent );
	}

}

//! print the passed string as a comment to the output stream
/*!
  \param stream			the output stream
  \param strtoprint		the string to print (may be multiline)
  \param indent			the indentation level (0,1...)
*/
void
DescriptorEntry::print( Q3TextStream &stream, QString &strtoprint, int indent )
{
	stream.reset();

	// print the passed string as a comment
	if( !strtoprint.isEmpty() ){
		stream << '\n';
		printComment( stream, strtoprint, indent );
	}

}

//! print a multi-line string as a comment to the output stream
/*!
  \param stream		the output stream
  \param str		string to print
  \param indent		the indentation level (0,1...)

  Identation is done with \a indent * 4
*/
void
DescriptorEntry::printComment( Q3TextStream &stream, const QString &str,
							   int indent )
{
	QStringList strLst = QStringList::split('\n', str );
	QString joinStr, initStr, formatted;

	joinStr.sprintf("\n%*s# ", indent*4, "");
	initStr.sprintf("%*s# ", indent*4, "");
	formatted =  initStr + strLst.join( joinStr );
	stream << formatted << "\n";
}

//! get pathname of that object
/*!
  make a "/" seperated pathname of that key up to and including
  the root entry; e.g. "A201_1/CHANNEL_1/1"
*/
QString
DescriptorEntry::path()
{
	if( parent )
		return parent->path() + "/" + key;
	else
		return key;
}

//! return the object's type as a string
QString
DescriptorEntry::typeStr()
{
	DescriptorFactory f;
	return f.typeStr( type );
}

//! returns the next descriptor key in the hierarchy
/*!
  if you have the following hierarchy:

\verbatim
  root
   a
      b
      c
      d
         e
         f
      g
   h
\endverbatim
  Then
  - f->nextSibling returns g
  - a->nextSibling returns b
  - b->nextSibling returns c
  - h->nextSibling returns 0

  If \a descend is false, it doesn't enter subdirectories
*/
DescriptorEntry *
DescriptorEntry::nextSibling(int descend)
{
	DescriptorEntryDirectory *par;
	DescriptorEntry *sibling;

	//wDebug(("nextsibling: %s %d", (const char *)path(), descend ));

	if( !(par = getParent()) ) // root directory cannot have sibling
		return 0;

	// if I'm a directory, descend into dir
	if( descend && (type == Directory) &&
		(sibling = ((DescriptorEntryDirectory *)this)->firstChild()))
		return sibling;

    // set list's current item to me
	int pos = par->getChildren().findRef( this );
	WIZ_ASSERT( pos != -1 );

	// is there a successor in the current dir?
	if( (sibling = par->getChildren().next()) != 0 )
		return sibling;

	// no, go up one directory
	return par->nextSibling(false);
}


// -----------------------------------------------------------------


//! constructs DescriptorEntryDirectory object
/*!
  all parameters are passed unchanged to DescriptorEntry::DescriptorEntry
*/
DescriptorEntryDirectory::DescriptorEntryDirectory(
	const QString &_key,
	const QString &_helpText ) :
	DescriptorEntry( _key, _helpText, Directory )
{
	lstChildren.setAutoDelete( true );
	_printEndHelp = true;
}

//! deletes DescriptorEntryDirectory object
DescriptorEntryDirectory::~DescriptorEntryDirectory()
{
}


//! add child to end of list of children
/*!
  Adds \a newEntry to this object. If \a overwrite is true, overwrites an
  already existing entry with the same name, otherwise \a newEntry is not
  added.

  \remark automatically sets the child's \a parent
*/
void
DescriptorEntryDirectory::addChild( DescriptorEntry *newEntry, bool overwrite )
{
	DescriptorEntry *old;

	if( (old = find( newEntry->getKey(), newEntry->getType())) != 0 ){
		if( overwrite ){
			//wDebug(("DescriptorEntryDirectory::addChild: overwriting %s %s",
			//	   (const char *)newEntry->getKey(), (const char *)path()));
			lstChildren.remove( old );
		}
		else
			return;
	}
	lstChildren.append( newEntry );
	newEntry->setParent(this);
}

//! remove child from list of children
/*!
  returns true if child was in list.

  Automatically deletes child object.
*/
bool
DescriptorEntryDirectory::removeChild( DescriptorEntry *entry )
{
	return lstChildren.remove( entry );
}

//! get first child
/*!
  \return pointer to child or 0 if no children
  \sa nextChild()
*/
DescriptorEntry *
DescriptorEntryDirectory::firstChild()
{
	return lstChildren.first();
}

//! get next child
/*!
  \return pointer to child or 0 if no more children
  \sa firstChild()
*/
DescriptorEntry *
DescriptorEntryDirectory::nextChild()
{
	return lstChildren.next();
}

//! tell if a DescriptorEntryDirectory object is empty (doesn't have any children)
/*! returns
 *	1 if directory is empty
 *	0 for not empty
*/
bool DescriptorEntryDirectory::isEmpty()
{
	DescriptorEntry *entry;
	if( !active ) return 1;

	/* call isEmpty for all Objects (recursive) */
	for( entry=firstChild(); entry; entry=nextChild() ) {
		DescriptorEntryDirectory *ded = dynamic_cast<DescriptorEntryDirectory *>(entry);
		if( ded )
			return ded->isEmpty();
		else
			if( entry->isActive() ) return 0;
	}
	return 1;
}

//! get direct list access to children
Q3PtrList<DescriptorEntry> &
DescriptorEntryDirectory::getChildren()
{
	return lstChildren;
}

//! print the directory and all children to output stream
/*!
  calls the print() method of all children
*/
void DescriptorEntryDirectory::print( Q3TextStream &stream, int indent )
{
	DescriptorEntry *entry;
	if( !active ) return;
	else {
		DescriptorEntryDirectory *ded = dynamic_cast<DescriptorEntryDirectory *>(this);
		if( ded && ded->isEmpty() )  return;
	}

	DescriptorEntry::print( stream, indent );

	stream.width(indent*4);
	stream << "";

	stream.reset();
	stream.setf(Q3TextStream::left );
	stream.width(1);
	stream << key << " {" << '\n';

	bool chameleonBBIS = false; // only print chameleon bbis hint on bbis level, not on group level!
	bool autoenumAlreadyPrinted = false;
	bool pciConfigAlreadyPrinted = false;
	
	// print all children
	for( entry=firstChild(); entry; entry=nextChild() ) {
		if ( entry->getKey() == "DESC_TYPE" ) {
			QString helptxt="------------------------------------------------------------------------\n"\
							"       general parameters (don't modify)\n"\
							"------------------------------------------------------------------------\n";
			entry->print(stream,helptxt,1);
		} else if ( (entry->getKey() == "HW_TYPE") && (entry->valueToString().contains("CHAMELEON"))) {
				chameleonBBIS = true;
		} else if ( ( entry->getKey().contains( "GROUP_" ) || entry->getKey().contains("DEVICE_ID") ) &&
					  chameleonBBIS ) {
		    	QString helptxt="------------------------------------------------------------------------\n"\
				                "       Chameleon BBIS Device: \n"\
	                            "           DEVICE_IDV2_X is:   ((Cham devId) << 8 | instance)\n"\
	                            "       inside groups:  \n"\
	                            "           DEVICE_IDV2_X is:   ((Cham devId) << 8 | index inside group )\n"\
	                            "------------------------------------------------------------------------\n";
		    	entry->print(stream,helptxt,1);
		    	chameleonBBIS = false; // only print comment for the first occurrence
		} else if ( entry->getKey() == "BOARD_NAME" ) {
			QString helptxt="------------------------------------------------------------------------\n"\
			                "       reference to base board\n"\
			                "------------------------------------------------------------------------\n";
			entry->print(stream,helptxt,1);
		} else if ( (entry->getKey() == "DEBUG_LEVEL") && entry->isActive() ) {
			QString helptxt="------------------------------------------------------------------------\n"\
			                "       debug levels (optional)\n"\
			                "       these keys have only effect on debug drivers\n"\
			                "------------------------------------------------------------------------\n";
			entry->print(stream,helptxt,1);
		}
		else if ( (entry->getKey() == "SMB_BUSNBR") && entry->isActive() ) {
			QString helptxt="------------------------------------------------------------------------\n"\
							" 		SMBus parameters\n"\
							"------------------------------------------------------------------------\n";
			entry->print(stream,helptxt,1);
		}
		else if (((entry->getKey() == "PCI_BUS_NUMBER") || 
				  (entry->getKey().contains("PCI_BUS_PATH")) ||
				  (entry->getKey() == "PCI_DOMAIN_NUMBER") ||
				  (entry->getKey() == "SKIP_DEV_BRIDGE_CHECK")
				 ) && entry->isActive() && !pciConfigAlreadyPrinted ) {
			QString helptxt="------------------------------------------------------------------------\n"\
							" 		PCI configuration\n"\
							"------------------------------------------------------------------------\n";
			entry->print(stream,helptxt,1);
			pciConfigAlreadyPrinted = true;
		}
		else if ( (entry->getKey().startsWith(QString("AUTOENUM")) && !autoenumAlreadyPrinted)
			     && entry->isActive() ) {
			autoenumAlreadyPrinted = true;  
			QString helptxt="------------------------------------------------------------------------\n"\
							" 		AUTOENUM keys\n"\
							"------------------------------------------------------------------------\n";
			entry->print(stream,helptxt,1);   	
		}
		entry->print( stream, indent+1 );

		if ( entry->getKey() == "DEVICE_SLOT" ) {
			QString helptxt="------------------------------------------------------------------------\n"\
			                "       device parameters\n"\
			                "------------------------------------------------------------------------\n";
			entry->print(stream,helptxt,1);
		}
	}

	if( _printEndHelp ) {
		DescriptorEntry::print( stream, indent );
	}

	stream.width(indent*4);
	stream << "";
	stream.reset();
	stream.setf(Q3TextStream::left );
	stream.width(1);
	stream << "}" << '\n';
}

//! make a copy of entire descriptor and all children
/*!
  \return the new object
*/
DescriptorEntry *
DescriptorEntryDirectory::clone()
{
	DescriptorEntryDirectory *thisCopy =
		new DescriptorEntryDirectory(key,helpText);
	DescriptorEntry *e;
	Q3PtrListIterator<DescriptorEntry> it(lstChildren);

	*(DescriptorEntry *)thisCopy = *(DescriptorEntry *)this;

	//wDebug(("DescriptorEntryDirectory::clone %s %s %d %d", (const char *)key,
	//	   (const char *)thisCopy->key, hidden, thisCopy->hidden));

	while( (e = it.current() )){
		++it;
		thisCopy->addChild( e->clone());
	}

	return thisCopy;
}

//! take over data from another descriptor
/*!
  takes over all children from descriptor directory \a newDesc.
  All previous contents of the object are lost.
*/
void
DescriptorEntryDirectory::take( DescriptorEntryDirectory *newDesc )
{
	DescriptorEntry *e;

	//wDebug(("DescriptorEntryDirectory::take %s %s", (const char *)key,
	//	   (const char *)newDesc->key));

	*(DescriptorEntry *)this = *(DescriptorEntry *)newDesc;

	// delete all my entries
	while( (e=lstChildren.first()) != 0 ){
		//wDebug((" delete %s", (const char *)e->getKey() ));
		lstChildren.remove(e);
	}

	// take new entries
	for( e=newDesc->lstChildren.first(); e; e=newDesc->lstChildren.next() )
		addChild( e->clone() );
}

//! find entry by keyname \a key and \a type (relative to the called directory)
/*
  key can be a simple name "xxx" or a pathname "xxx/yyy"
  \return Descriptor entry or 0 if not found
*/
DescriptorEntry *
DescriptorEntryDirectory::find( const QString &_key, DescType _type )
{
	DescriptorEntry *e, *e2;
	Q3PtrListIterator<DescriptorEntry> it(lstChildren);

	//wDebug(("find: startpath=%s", (const char *)path()));

	while( (e = it.current() )){
		++it;
		//wDebug(("find: path=%s", (const char *)e->path()));
		if( e->getKey() == _key && e->getType() == _type )
			return e;

		// if the directory matches, descend into subdir
		if( e->getType() == DescriptorEntry::Directory ){
			DescriptorEntryDirectory *eDir = (DescriptorEntryDirectory *)e;

			if( e->getKey() == _key.section( '/', 0, 0 ))
				if( (e2 = eDir->find( _key.section( '/', 1 ), _type )))
					return e2;
		}
	}
	return 0;
}


//! check whether child descriptor keys are all invisible
/*
  \return true if at least one descriptor child is visible;
  otherwise return false
*/
bool 
DescriptorEntryDirectory::hasVisibleChildren()
{
	DescriptorEntry *e;

	if (!lstChildren.isEmpty()) {
		// delete all my entries
		e = lstChildren.first();
		while( e != 0) {
			if ( e->isHidden() == false )
				return true;
			e = lstChildren.next();
		}
	}
	return false;
}

// -----------------------------------------------------------------

//! constructs DescriptorEntryUint32 object
/*!
  \a _key and \a _helpText are passed unchanged to
  DescriptorEntry::DescriptorEntry.

  \param _key descriptor key name
  \param _helpText descriptor helptext
  \param _base base radix of entry (10 or 16)
*/
DescriptorEntryUint32::DescriptorEntryUint32(
	const QString &_key, const QString &_helpText, int _base) :
	DescriptorEntry( _key, _helpText, Uint32 )
{
	base = _base;
	value = 0;
	setRange();
}

//! set new range of values
/*!
  If the current \em value or \em defaultValue is not within the new
  min/max values, then \em value or \em defaultValue is adjusted to fit into
  new limits
*/
void
DescriptorEntryUint32::setRange( ulong _minValue, ulong _maxValue )
{
	minValue = _minValue;
	maxValue = _maxValue;

	if( value < minValue )
		value = minValue;
	if( value > maxValue )
		value = maxValue;
	if( defaultValue < minValue )
		defaultValue = minValue;
	if( defaultValue > maxValue )
		defaultValue = maxValue;

}

//! report wether this is a multiple choise setting
bool DescriptorEntryUint32::isMultiChoise()
{
	return !choises.isEmpty();
}


//! get the default value
ulong DescriptorEntryUint32::getDefaultValue()
{
	return defaultValue;
}

//! set the default value
/*!
  \returns true if \a _defaultValue is within range and if it is a
  valid choise (if this is a multiple choise setting)
*/
bool DescriptorEntryUint32::setDefaultValue( ulong _defaultValue )
{
	if( _defaultValue < minValue || _defaultValue > maxValue )
		return false;

	if( isMultiChoise() ){
		if( choises.find( _defaultValue ) == choises.end() )
			return false;
	}
	defaultValue = _defaultValue;
	return true;
}

//! get the value
ulong DescriptorEntryUint32::getValue()
{
	return value;
}

//! set the value
/*!
  \returns true if \a value is within range and if it is a valid choise (if
  this is a multiple choise setting)
*/
bool DescriptorEntryUint32::setValue( ulong _value )
{
	if( _value < minValue || _value > maxValue )
		return false;

	if( isMultiChoise() ){
		if( choises.find( _value ) == choises.end() )
			return false;
	}
	value = _value;
	return true;
}

void DescriptorEntryUint32::print( Q3TextStream &stream, int indent )
{
	if( !active ) return;

	DescriptorEntry::print( stream, indent );

	// print choises as helptext
	if( isMultiChoise() ){
		ChoisesMap::Iterator it;
		QString helpText;

        for ( it = choises.begin(); it != choises.end(); ++it ) {
			helpText += anyValueToString( it.key() );
			helpText += " := ";
			helpText += it.data() + '\n';
		}
		printComment( stream, helpText, indent );
	}

	// print range if not default range
	if( (minValue != 0)  || (maxValue != 0xffffffff)){
		QString helpText;

		helpText = QString("Range: ") + anyValueToString(minValue) +
			".." + anyValueToString(maxValue);
		printComment( stream, helpText, indent );
	}

	stream.width(indent*4);
	stream << "";
	stream.reset();
	if( base == 10 )
		stream.setf(Q3TextStream::dec | Q3TextStream::showbase |
					Q3TextStream::left );
	else
		stream.setf(Q3TextStream::hex | Q3TextStream::showbase |
					Q3TextStream::left );
	stream.width(1);
	stream << key << " = U_INT32 " << value << '\n';
}

//! Convert the specified value \a val value to string
/*
  If base is not 10, string is preceeded by \c 0x
*/
QString
DescriptorEntryUint32::anyValueToString( ulong val )
{
	QString rv;

	if( base == 10 )
		rv.sprintf("%lu", val );
	else
		rv.sprintf("0x%lx", val );
	return rv;
}

//! Convert value to string
/*
  if \a fromDefval is false, use \a value, else \a defaultValue

  If base is not 10, string is preceeded by \c 0x
*/
QString
DescriptorEntryUint32::valueToString( bool fromDefVal)
{
	ulong val = fromDefVal ? defaultValue : value;

	return anyValueToString( val );
}

// set value from string
// if toDefval is false, update <value>, else <defaultValue>
// returns true if conversion successfull
bool
DescriptorEntryUint32::setValue( const QString &str, bool toDefVal )
{
	ulong newVal;
	int bs;
	bool rv = false;

	if( Util::getInt( str, &newVal, &bs ) == true ){
		base = bs;

		if( toDefVal )
			rv = setDefaultValue( newVal );
		else
			rv = setValue( newVal );
	}
	return rv;
}

// -----------------------------------------------------------------

//! constructs DescriptorEntryString object
/*!
  all parameters are passed unchanged to DescriptorEntry::DescriptorEntry
*/
DescriptorEntryString::DescriptorEntryString(
	const QString &_key, const QString &_helpText) :
	DescriptorEntry( _key, _helpText, String )
{
}

//! get the default value
QString &DescriptorEntryString::getDefaultValue()
{
	return defaultValue;
}

//! set the default value
void DescriptorEntryString::setDefaultValue( const QString &_defaultValue )
{
	defaultValue = _defaultValue;
}

//! get the value
QString &DescriptorEntryString::getValue()
{
	return value;
}

void DescriptorEntryString::print( Q3TextStream &stream, int indent )
{
	if( !active ) return;

	DescriptorEntry::print( stream, indent );
	stream.width(indent*4);
	stream << "";
	stream.reset();
	stream.setf(Q3TextStream::hex |
				Q3TextStream::showbase |
				Q3TextStream::left );
	stream.width(1);
	stream << key << " = STRING " << value << '\n';
}

// Convert value to string
QString
DescriptorEntryString::valueToString( bool fromDefVal )
{
	return fromDefVal ? defaultValue : value;
}

// set value from string
bool
DescriptorEntryString::setValue( const QString &str, bool toDefVal )
{
	if( toDefVal )
		defaultValue = str;
	else
		value = str;
	return true;
}

// -----------------------------------------------------------------
//! constructs DescriptorEntryBinary object
/*!
  all parameters are passed unchanged to DescriptorEntry::DescriptorEntry
*/
DescriptorEntryBinary::DescriptorEntryBinary(
	const QString &_key, const QString &_helpText) :
	DescriptorEntry( _key, _helpText, Binary )
{
}

//! get the value
Q3MemArray<uchar> &DescriptorEntryBinary::getValue()
{
	return value;
}

//! set value from another array
void DescriptorEntryBinary::setValue( const Q3MemArray<uchar> &_value )
{
	value.duplicate(_value);
}

//! get the default value
Q3MemArray<uchar> &DescriptorEntryBinary::getDefaultValue()
{
	return defaultValue;
}


//! set the default value
void DescriptorEntryBinary::setDefaultValue(
	const Q3MemArray<uchar> &_defaultValue )
{
	value.duplicate(_defaultValue);
}

void DescriptorEntryBinary::print( Q3TextStream &stream, int indent )
{
	if( !active ) return;

	DescriptorEntry::print( stream, indent );
	stream.width(indent*4);
	stream << "";
	stream.reset();
	stream.setf(Q3TextStream::hex | Q3TextStream::showbase | Q3TextStream::left );
	stream << key << " = BINARY ";

	for( uint i=0; i<value.size(); i++ ){
		stream.width(1);
		stream << value.at(i);
		if( i+1 != value.size() )
			stream << ',';
	}
	stream << '\n';
}

// Convert value to string
QString
DescriptorEntryBinary::valueToString( bool fromDefVal )
{
	QString result;
	Q3MemArray<uchar> &val = fromDefVal ? defaultValue : value;

	for( uint i=0; i<val.size(); i++ ){
		QString tmp;
		tmp.sprintf( "0x%02x", val.at(i));
		result += tmp;
		if( i+1 != val.size() )
			result += ',';
	}
	return result;
}

// set value from string
bool
DescriptorEntryBinary::setValue( const QString &str, bool toDefVal)
{
	QString elem;
	int idx=0, bs;
	Q3MemArray<uchar> &arr = toDefVal ? defaultValue : value;
	ulong val;

	//wDebug(( "str=%s", (const char *)str ));
	arr.resize(0); // clear array

	while( true ) {
		elem = str.section( ',', idx, idx );

		if( elem == QString::null )
			break;

		if( Util::getInt( elem, &val, &bs ) == true ){
			if( val > 0xff ){
				wDebug(("DescriptorEntryBinary::setValue val too big %s",
					   (const char *)elem ));
				return false;
			}
			arr.resize(idx+1); // possibly slow
			arr[idx] = (uchar)val;
		}
		else {
			wDebug(("DescriptorEntryBinary::setValue bad val %s",
				   (const char *)elem ));
			return false;
		}
		idx++;
	}

	return true;
}


// -----------------------------------------------------------------

//! create an U_INT32 descriptor entry
/*!
  entry's defaultValue is set to value
*/
DescriptorEntry *
DescriptorFactory::create( const QString &key, ulong value,
						   const QString &helpText, bool active )
{
	DescriptorEntryUint32 *e = new DescriptorEntryUint32( key, helpText );

	e->setDefaultValue( value );
	e->setValue( value );
	e->setActive( active );
	return e;
}

//! create a STRING descriptor entry
/*!
  entry's defaultValue is set to value
*/
DescriptorEntry *
DescriptorFactory::create( const QString &key, const QString &value,
						   const QString &helpText, bool active)
{
	DescriptorEntryString *e = new DescriptorEntryString( key, helpText );

	e->setDefaultValue( value );
	e->setValue( value );
	e->setActive( active );
	return e;
}


//! create a BINARY descriptor entry
/*!
  entry's defaultValue is set to value
*/
DescriptorEntry *
DescriptorFactory::create( const QString &key, const Q3MemArray<uchar> &value,
						   const QString &helpText, bool active )
{
	DescriptorEntryBinary *e = new DescriptorEntryBinary( key, helpText );

	e->setDefaultValue( value );
	e->setValue( value );
	e->setActive( active );
	return e;
}

//! create entry based on \a typeStr (\c U_INT32, \c STRING or \c BINARY)
/*
  \return 0 if entry could not be created (propably due to illegal syntax)
*/
DescriptorEntry *
DescriptorFactory::create( const QString &key, const QString &typeStr,
						   const QString &value,
						   const QString &helpText, bool active )
{
	DescriptorEntry *e;
	DescriptorFactory dFact;

	if( typeStr == dFact.typeStr( DescriptorEntry::Uint32 ))
		e = new DescriptorEntryUint32( key, helpText );

	else if( typeStr == dFact.typeStr( DescriptorEntry::String ))
		e = new DescriptorEntryString( key, helpText );

	else if( typeStr == dFact.typeStr( DescriptorEntry::Binary ))
		e = new DescriptorEntryBinary( key, helpText );

	else
		e = 0;

	if( e ){
		e->setActive( active );

		if( (e->setValue( value, false ) == false) ||
			(e->setValue( value, true ) == false)){
			wDebug(("DescriptorFactory::create couln't set value %s",
				   (const char *)value ));
			delete e;
			e = 0;
		}
	}
	return e;
}

//! return type string (U_INT32 etc) based on _type
QString
DescriptorFactory::typeStr( DescriptorEntry::DescType _type )
{
	switch( _type ){
	case DescriptorEntry::Directory: return "DIR"; break;
	case DescriptorEntry::Uint32: return "U_INT32"; break;
	case DescriptorEntry::String: return "STRING"; break;
	case DescriptorEntry::Binary: return "BINARY"; break;
	default:	 return QString::null; break;
	}
}

//! return common descriptor keys as ASCII strings
QString
DescriptorFactory::keyStr( KeyType _type )
{
	switch( _type ){
	case DESC_TYPE: 	return "DESC_TYPE"; break;
	case HW_TYPE:		return "HW_TYPE"; break;
	case BOARD_NAME:	return "BOARD_NAME"; break;
	case DEVICE_SLOT:	return "DEVICE_SLOT"; break;
	case _WIZ_BUSIF:	return "_WIZ_BUSIF"; break;
	case _WIZ_MODEL:	return "_WIZ_MODEL"; break;
	default:			return QString::null; break;
	}
}

//! create special entry for _WIZ_BUSIF
/*!
  \param parentName Device that hosts the bus interface
  \param busIfNum 	bus interface index within hosting device

  creates string: <parentName>,<num>
*/
DescriptorEntry *
DescriptorFactory::createWizBusIf( const QString &parentName,
								   int busIfNum)
{
	return create( keyStr(_WIZ_BUSIF),
				   QString("%1,%2").arg(parentName).arg(busIfNum),
				   QString::null, true );
}






