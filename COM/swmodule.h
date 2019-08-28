/***************************************************************************
 */
/*!       \file  swmodule.h
 *       \brief  class to represent software module
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:27:47 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: swmodule.h,v $
 * Revision 2.0  2010/01/22 11:27:47  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.7  2009/05/13 11:53:07  CRuff
 * R: 1. Core lib 'oss' needs to pull in additional Makefile for VxWorks
 * M: 1a) Change type of SwModule::makefilePath to a list QStringList
 *    1b) added method SwModule::makefilePath_all() to get whole list of
 *        makefile paths
 *    1c) added method SwModule::addMakefilePath() to add an additional
 *        path to the list
 *
 * Revision 1.6  2009/04/27 17:57:55  CRuff
 * R: make kernel modules OSS and DBG configurable
 * M: in SwModuleType: add new type ModType::CoreLib
 *
 * Revision 1.5  2004/07/21 15:03:54  ufranke
 * cosmetics
 *
 * Revision 1.4  2004/03/12 11:53:06  dpfeuffer
 * cosmetics
 *
 * Revision 1.3  2004/01/09 08:20:32  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/06/03 10:16:50  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:36:29  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef SWMODULE_H
#define SWMODULE_H

#include <qstring.h>
#include <qlist.h>
#include <qstringlist.h>
#include <qmap.h>
#include <q3valuelist.h>

#include <Q3PtrList>

class SwModuleType {
 public:
	SwModuleType() {};
	SwModuleType( const QString &_typeName,
				  const QString &_comPath = QString::null,
				  const QString &_natPath = QString::null){
		typeName 	= _typeName;
		comPath		= _comPath;
		natPath		= _natPath;
	};

	QString	typeName;	  //! symbolic name of type e.g. "Low Level Driver"
	QString comPath;	  //! common path for type e.g. "DRIVERS/MDIS_LL"
	QString natPath;	  //! native path e.g. "SRC/IO/MDIS_LL"
};

//! class to list the available SwModule types
/*!
  Is only instanciated once as a the global object G_swModTypes.

  can be extended by OS specific types

  Usage:

  - to check if module type (int) exists, use types.find(type) or 
    type.contains(type) (see QMap doc)
  - to iterate through list of types use 

  \code
  SwModuleTypes::TypeMap::Iterator it;
  for ( it = types.begin(); it != types.end(); ++it ) {
      printf("%s\n", it.data().typeName.latin1());
  }
  \endcode
*/
class SwModuleTypes {
 public:

	//! datatype to map the module type (integer) to its description
	typedef QMap<int,SwModuleType> TypeMap;

	//! the type of the SW module (standard types)
	enum ModType { Other=0x1, LlDriver=0x2, BbDriver=0x4, CbDriver=0x8, 
				   LlTool=0x10, CoreLib=0x20, UserLib=0x40, ComTool=0x80, 
				   NativeDriver=0x100, NativeLib=0x200, NativeTool=0x400		   
	};

	SwModuleTypes();

	TypeMap::Iterator findString( const QString &str );
	int findStringVal( const QString &str );
	QString mapComToNatPath( const QString &comPath );
	//! map of available types
	TypeMap types;

};

extern SwModuleTypes *G_swModTypes;		//!< global SW module types

//! class to represent software module
/*!
  i.e. the code for a MDIS/BBIS/CBIS driver, API library, tools and examples

*/
class SwModule {
public:

	//! creates a software module
	/*!
	  \param type type of module (see ModType)
	  \param name name of resulting binary
	  \param makef pathname of makefile, (relative)
	  \param descr description of module

	  The initial use count is set to 0.
	*/
	SwModule( int type, const QString &name, const QString &makef, 
			  const QString &descr ) {
		_type = type; _modName=name, _makefilePath.append(makef), _description=descr;
		enabled = true; _useCount=0;
		internal = false;
		swapCapable = true;
	}

	//! get type of module
	int modType() const { return _type; }

	//! set type of module
	void setModType( int type ) { _type = type; }

	//! check if module is enabled for compilation
	bool isEnabled() const { return enabled; }
	//! set if module is enabled for compilation
	void setEnabled( bool isEn ) { enabled = isEn; }
	
	//! increment the module's use count
	void incUseCount() { _useCount++; }
	//! decrement the module's use count
	void decUseCount() { _useCount--; }
	//! get the module's use count
	int useCount() const { return _useCount; }
	
	//! get the module's name
	const QString &modName() const { return _modName; } ;
	//! get the module's makefile path (first element only)
	const QString &makefilePath() const { return _makefilePath[0]; }
	//! get the whole list of makefile paths
	const QStringList* makefilePath_all() const { return &_makefilePath; }
	//! get the module's description
	const QString &description() const { return _description; }
		
	//! get module internal flag
	bool isInternal() { return internal; }
	//! set module internal flag
	void setInternal( bool isInt ) { internal = isInt; }

	//! get module supports swapping flag
	bool isSwapCapable() { return swapCapable; }

	//! set module supports swapping flag
	void setSwapCapable( bool isCap ) { swapCapable = isCap; }

	//! Add \a osType to list of excluded OSes. \sa excludedOses 
	void excludeOs( int osType ){
		excludedOses << osType;
	}

	//! Add \a osType to list of included OSes. \sa includedOses 
	void includeOs( int osType ){
		includedOses << osType;
	}
	
	bool supportsOs( int osType );
	
	//! add lib to makefile path for this module
	void addMakefilePath( QString newPath ) { _makefilePath.append(newPath); }

protected:
	bool enabled;				//!< compile this component?
	int _useCount;				//!< number of devices referencing this module
	QString _modName;			//!< name of module (i.e. resulting binary)
	QStringList _makefilePath;		//!< pathname of makefile, (relative)
	QString _description;		//!< description of that module
	int _type;					//!< module type
	bool internal;				//!< module is used MEN internally
	bool swapCapable;			//!< LL-driver: swapping driver available

	//! These two lists flag the OSes supported by this SwModule 
	/*! 
	  they take OsFactory::OsType codes.
	  Initially both lists are empty. In this case the SwModule
	  supports \b all OSes. 

	  excludedOses flags which OSes are \b not supported.
	  includedOses flags which OSes \b are supported
	*/
	Q3ValueList<int> excludedOses; 
	Q3ValueList<int> includedOses; //!< see #excludedOses
	
};

//! class that holds a list of software modules
/*!
  A subclass of QPtrList that serves as a container for SwModule objects.

  The SwModule objects are owned by the SwModuleList. They will be deleted
  when the SwModuleList is deleted.

  
*/
class SwModuleList : public Q3PtrList<SwModule> {
public:
	SwModuleList();

	bool add( SwModule *mod, bool overwrite=true );
	void add( SwModuleList *lst );
	SwModule *find( int type );
	SwModule *find( int type, const QString &name );
	SwModule *findMake( int type, const QString &makefilePath );
};

class DepracatedComponent {
public:

	//! type of Depracated Component End of support/End of life
	enum DepracatedType { EOS=0x1, EOL=0x2 };

	DepracatedComponent( int type, const QString &component, const QString &description ) {
		_type = type; _component=component; _description = description; }
	//! get type of module
	int componentType() const { return _type; }
	//! get the module's name
	const QString &componentName() const { return _component; } ;
	//! get the module's description
	const QString &description() const { return _description; }
private:
	QString _component;		//!< name of _component / mdiswizard name of component
	QString _description;	//!< description of component / full Component name
	int _type;				//!< type of component EOL / EOS
};

class ComponentEosEolList : public Q3PtrList<DepracatedComponent> {
public:
	ComponentEosEolList();
	bool add(DepracatedComponent *component);
	DepracatedComponent *find( int type, const QString &name );
};

#endif

