/***************************************************************************
 */
/*!       \file  packagefile.h
 *       \brief  Parse XML model file
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:27:21 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: packagefile.h,v $
 * Revision 2.0  2010/01/22 11:27:21  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.5  2009/04/01 15:41:06  CRuff
 * R: 1. Chameleon V2 support
 * M: 1. Added help function findNodes(): returns all child dom nodes which
 *       have the given name and type
 *
 * Revision 1.4  2004/01/09 08:20:21  kp
 * support BbSlotSpec
 *
 * Revision 1.3  2003/06/03 10:16:38  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/05/14 13:45:42  kp
 * misc
 *
 * Revision 1.1  2003/05/02 14:36:19  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef PACKAGEFILE_H
#define PACKAGEFILE_H

#include <qstring.h>
#include <qstringlist.h>
#include <qdom.h>
#include <q3valuelist.h>

#include "hwcomponent.h"

class ModelFactory;
class ModMdisGeneric;
class DescriptorEntryDirectory;
class DescriptorEntryUint32;
class SwModule;
class SwModuleList;

// -----------------------------------------------------------------
//! Parse SW packages and models from XML files
/*!
  The PackageFile class is used to create ModMdisGeneric devices from
  the data contained in XML files. 

  A package describes a set of files, typically a SW package for an
  MDIS low level driver. Each SW package contains one or more package
  XML files.  Mdiswiz parses those files on startup to build it's list
  of available models.

  See MEN packages specification PackageDesc-Spec1.doc for more info.


*/
class PackageFile {
public:
	PackageFile( const QString &_fileName);
	~PackageFile();

	bool parse( QString &errMsg );

	QString pkgTag( const QString &tagName );

	int toModelFactory( ModelFactory *factory, QString &errMsg );
	void dump();
private:
	QDomNode findNode( QDomNode parent, QString name, 
					   QDomNode::NodeType type );
    Q3ValueList<QDomNode> findNodes( QDomNode parent, QString name, 
    								QDomNode::NodeType type );

	ModMdisGeneric *makeDevice( QDomNode _model, QString &errMsg,
								SwModuleList *swModsGlobal,
								DescriptorEntryDirectory *descGlobal );
	void makeDescriptor( QDomNode parent, DescriptorEntryDirectory *dir, 
						 QString &errMsg );
	void makeSwModules( QDomNode parent, SwModuleList *lst, 
						QString &errMsg );
	void makeDescriptorSubdir( QDomNode parent, 
							   DescriptorEntryDirectory *dir, 
							   QString &errMsg );
	void makeDescChoises( QDomNode parent, 
						  DescriptorEntryUint32 *e, 
						  QString &errMsg );
	DeviceAutoId *makeAutoId( QDomNode parent, DescriptorEntryDirectory *desc, 
							  HwComponent::BusIfType busIfType, 
							  QString &errMsg );
	BbSlotSpecList *makeBbSlotSpec( QDomNode parent, QString &errMsg );


	void dumpNode( QDomNode *n, int indent );
	QString nodeText( QDomNode n );
	QDomDocument *doc;
	QString fileName;
	QDomNode pkgnode;			//!< "package" node
	QString errPf;				//!< prefix for error messages
	ulong dbgSettingMods;		//!< modules that have a debug level
};

#endif
