/***************************************************************************
 */
/*!       \file  packagefile.cpp
 *       \brief  Implementation of package file XML parser
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2011/09/26 08:42:36 $
 *    $Revision: 2.2 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: packagefile.cpp,v $
 * Revision 2.2  2011/09/26 08:42:36  dpfeuffer
 * R:1. no MEN HW with PC104+ PCI (it is PCI-104)
 *   2. new bus interfaces required: StdPciE, CpciSer
 * M:1. Pc104PlusPci replaced with Pci104
 *   2. added StdPciE, CpciSer
 *
 * Revision 2.1  2011/02/14 13:55:30  CRuff
 * R: 1. add support for PP04
 * M: 1. in makeAutoId: also accept pci variant Pc104PlusPci
 *
 * Revision 2.0  2010/01/22 11:27:18  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.11  2009/08/14 13:25:12  CRuff
 * R: wrong implementation of chamV2 groups
 * M: makeAutoId(): extract several group members out of one chamv2grpmemid tag
 *
 * Revision 1.10  2009/05/13 11:47:17  CRuff
 * R: Native drivers were not handled correctly, which caused application crash
 * M: handle native drivers like chameleon devices (like before chamV2 changes);
 *    therefor add additional check for device type in PackageFile::makeDevice()
 *
 * Revision 1.9  2009/04/01 15:37:51  CRuff
 * R: 1.Chameleon V2 support
 * M: 1.a) extended function makeDevice: create ModMdisCham device in case it is
 *         a chameleon mdis device
 *      b) extended function makeAutoId: create DeviceChamV2AutoId if device
 *         supports chamv2
 *
 * Revision 1.8  2006/11/30 11:27:53  cs
 * PackageFile::makeAutoId() modified for HwComponent::Xmc
 *
 * Revision 1.7  2004/05/24 08:54:56  dpfeuffer
 * PackageFile::makeAutoId() modified for chamAutoId
 *
 * Revision 1.6  2004/01/09 08:20:19  kp
 * support BbSlotSpec
 * support SW module "internal" flag
 * support SW module "swap" flag
 * support SW module "os/notos" tags
 *
 * Revision 1.5  2003/06/05 12:01:16  dschmidt
 * intermediate checkin
 *
 * Revision 1.4  2003/06/05 09:33:47  kp
 * intermediate checkin
 *
 * Revision 1.3  2003/06/03 10:16:35  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/05/14 13:45:35  kp
 * activate setting when hidden attribute was fouhd
 *
 * Revision 1.1  2003/05/02 14:36:18  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <qfile.h>
#include <qregexp.h>

#include <Q3ValueList>

#include "wizdebug.h"
#include "packagefile.h"
#include "modelfactory.h"
#include "osfactory.h"
#include "models/modmdisgeneric.h"
#include "models/modmdischam.h"
#include "util.h"

//! create a PackageFile object
/*!
  This constructs a PackageFile class. To parse the file, call parse().
*/
PackageFile::PackageFile( const QString &_fileName )
{
	fileName = _fileName;
	doc = 0;
}


//! destructs package file object
PackageFile::~PackageFile()
{
	if( doc )
		delete doc;
}

//! Do basic parsing of a package XML file
/*!
  It opens the XML file and does basic parsing on it. Any
  errors will be reported to \a errMsg.

  \return true if XML syntax is ok

  \remark This function does not check the validity of all XML tags.
  It only checks if the tag nesting is ok
*/
bool
PackageFile::parse( QString &errMsg )
{
	QString domErrMsg;
	int domErrLine;

	errMsg = QString::null;


	QFile f( fileName );

	if ( !f.open( QIODevice::ReadOnly ) ){
		errMsg += "Cannot open " + fileName + "\n";
		return false;
	}

	doc = new QDomDocument( fileName );

	// convert XML file to QDom hierarchy
	if ( !doc->setContent( &f, &domErrMsg, &domErrLine ) ) {
		errMsg += QString("line %2: %3\n").arg(domErrLine).arg(domErrMsg);
		goto errexit;
	}

	pkgnode = findNode( *doc, "package", QDomNode::ElementNode );
	if( pkgnode.isNull() ){
		errMsg += "\"package\" tag not found";
		goto errexit;
	}
	f.close();

	return true;

 errexit:
	errMsg = QString("%1: %2").arg(fileName).arg(errMsg);
	delete doc;
	doc = 0;
	f.close();
	return false;
}

//! get a tag from XML file related to that package
/*
  possible tags are:
  - name
  - description
  - date
  - revision
  - docroot

  \return tag value (or QString::null if tag not found, parsing failed
  or not yet done)

  \sa parse
*/

QString
PackageFile::pkgTag( const QString &tagName )
{
	QDomNode n;
	if( !doc )
		return QString::null;

	n = findNode( pkgnode, tagName, QDomNode::ElementNode );
	if( n.isNull() )
		return QString::null;

	return nodeText(n);
}


//! Add all models contained in package file to \a factory
/*!
  Parses the entire package file. Any \em model that is found in the XML
  file is converted to a ModMdisGeneric device model.

  Any warning or error is put into \a errMsg (newline separated)

  \return the number of devices created
*/
int
PackageFile::toModelFactory( ModelFactory *factory, QString &errMsg )
{
	SwModuleList swModsGlobal;
	DescriptorEntryDirectory descGlobal("_global_");
	ModMdisGeneric *dev;
	int devCount = 0;

	wDebug(("PackageFile toModelFactory %s", fileName.latin1()));
	dbgSettingMods =
		(1<<DebugProperties::Driver) |
		(1<<DebugProperties::MdisKernel) |
		(1<<DebugProperties::Oss) |
		(1<<DebugProperties::Desc);

	errMsg = QString::null;
	errPf = fileName + ": ";

	if( !doc ){
		errMsg += errPf + "Invalid XML document\n";
		return 0;
	}

	// find the global descriptor entries
	errPf = fileName + ": global settings: ";
	makeDescriptor( pkgnode, &descGlobal, errMsg );

	// find global SW modules
	errPf = fileName + ": global SW modules: ";
	makeSwModules( pkgnode, &swModsGlobal, errMsg );

	// loop through the model list (find the modellist tag)
	QDomNode modelList = findNode( pkgnode, "modellist",
								   QDomNode::ElementNode );
	errPf = fileName + ": ";

	if( modelList.isNull() ){
		errMsg += errPf + "No \"modellist\" tag found\n";
		return 0;
	}

	QDomNode model = modelList.firstChild();

	while( !model.isNull() ){

		errPf = fileName + ": ";

		if( model.nodeName() == "model" ){
			dev = makeDevice( model, errMsg,
							  &swModsGlobal, &descGlobal );
			if( dev ){
				devCount++;
				factory->append( dev );
			}
			else
				errMsg += "*** Import failed\n";
		}
		model = model.nextSibling();
	}

	return devCount;
}

//! Build an ModMdisGeneric device from the <model> section
/*!
  If all mantadory tags found under node \a model, build a ModMdisGeneric
  device.

  \param swModsGlobal global software modules (added to model's modules)
  \param descGlobal global descriptor entries (added to model's descriptor)

  \return new device created or 0 if failed

  Any errors are put into errMsg

*/
ModMdisGeneric *
PackageFile::makeDevice( QDomNode model, QString &errMsg,
						 SwModuleList *swModsGlobal,
						 DescriptorEntryDirectory *descGlobal )
{
	QDomNode n = model.firstChild();
	QString modelName, driverName, description, devType, busIf, devNamePrefix;
	QString hwName, vendorName, intModName;
	QString subDevOffTblStr, hiddenStr = "no";
	QString mmodDataCap, mmodAddrCap;
	Q3MemArray<int> subDevOffTbl;
	HwComponent::BusIfType busIfType;
	ModMdisGeneric *dev = 0;
	DescriptorEntryDirectory modelDesc( "_model_" );
	SwModuleList *modelSwMods;
	DeviceAutoId *autoId;
	BbSlotSpecList *bbSlotSpecList;
	bool isNative;
	bool isChameleon = false;

	// parse the global attributes of the model
	while( !n.isNull() ){

		if( n.nodeName() == "vendor" )
			vendorName = nodeText(n);

		else if( n.nodeName() == "hwname" )
			hwName = nodeText(n);

		else if( n.nodeName() == "modelname" )
			modelName = nodeText(n);

		else if( n.nodeName() == "description" )
			description = nodeText(n);

		else if( n.nodeName() == "devtype" )
			devType = nodeText(n);

		else if( n.nodeName() == "busif" )
			busIf = nodeText(n);

		else if( n.nodeName() == "mmoddatacap" )
			mmodDataCap = nodeText(n);

		else if( n.nodeName() == "mmodaddrcap" )
			mmodAddrCap = nodeText(n);

		else if( n.nodeName() == "devnameprefix" )
			devNamePrefix = nodeText(n);

		else if( n.nodeName() == "subdevofftbl" )
			subDevOffTblStr = nodeText(n);

		n = n.nextSibling();
	}

	// validate global attributes
	if( hwName.isEmpty() ){
		errMsg += errPf + "Missing model \"hwname\" tag\n";
		return 0;
	}
	intModName = !modelName.isEmpty() ? modelName : hwName;

	errPf = fileName + ": model " + intModName + ": ";


	if( devType == "MDIS" )
		isNative = false;
	else if( devType == "NATIVE" )
		isNative = true;
	else {
		errMsg += errPf + "bad \"devtype\"" + devType + "\n";
		goto abort;
	}

	// check for bbslot tags
	bbSlotSpecList = makeBbSlotSpec( model, errMsg );

	if( bbSlotSpecList == 0 ){
		if( busIf.isEmpty() ){
			errMsg += errPf + "Missing \"busif\" tag\n";
			goto abort;
		}
		if( (busIfType = HwComponent::busIfTypeMap.strToKey( busIf )) ==
			HwComponent::InvalidBusIf ){
			errMsg += errPf + "Illegal \"busif\" tag " + busIf + "\n";
			goto abort;
		}
	}
	else
	{
		busIfType = HwComponent::LocalBus; // arbitrary

		QString bbModel = bbSlotSpecList->first()->_bbModel;
		if ( bbModel.contains( "CHAMELEON" ) ) {
			isChameleon = true;
		}
	}
	// also check for the bus interface type
	if (busIfType == HwComponent::ChameleonBus ) {
		isChameleon = true;
	}

	// convert subdevice offset table
	if( !subDevOffTblStr.isEmpty() ){
		int idx=0, val;
		bool ok;

		subDevOffTbl.resize(0);
		while( true ){
			QString elem = subDevOffTblStr.section( ',', idx, idx );

			if( elem.isEmpty() )
				break;
			val = elem.toInt( &ok, 16 );
			if( ok ){
				subDevOffTbl.resize(idx+1);
				subDevOffTbl[idx++] = val;
			}
			else {
				errMsg += errPf + "Illegal \"subdevofftbl\" value(s) " +
					subDevOffTblStr + "\n";
				break;
			}

		}
	}
	// create the model specific descriptor entries
	modelDesc.take( descGlobal );

	errPf = fileName + ": model " + intModName + " settings: ";
	makeDescriptor( model, &modelDesc, errMsg );

	// create model specific SwModules
	modelSwMods = new SwModuleList;
	modelSwMods->add( swModsGlobal );

	errPf = fileName + ": model " + intModName + " SW modules: ";
	makeSwModules( model, modelSwMods, errMsg );

	// locate the LL/Native driver module to set the driverName
	{
		SwModule *mod = modelSwMods->find( SwModuleTypes::LlDriver );

		if( !mod )
			mod = modelSwMods->find( SwModuleTypes::NativeDriver );

		if( !mod ){
			errMsg += errPf + "no LL or native driver specified\n";
			delete modelSwMods;
			goto abort;
		}
		driverName = mod->modName().upper(); // convert always to upper case
	}

	errPf = fileName + ": model " + intModName + " ";
	// check for autoid
	autoId = makeAutoId( model, &modelDesc, busIfType, errMsg );

	//-------------------------------
	// create the new device
	//-------------------------------
	if ( isChameleon ) {
		wDebug(( "create ModMdisCham device" ));
		dev = new ModMdisCham ( hwName, driverName, true, busIfType, autoId );
	}
	else {
		wDebug(( "create ModMdisGeneric device" ));
		dev = new ModMdisGeneric( hwName, driverName, true, busIfType );
	}
	dev->setDescriptor( &modelDesc );

	if( !modelName.isEmpty() )
		dev->setModelName( modelName );

	if( !vendorName.isEmpty() )
		dev->setVendorName( vendorName );

	if( !description.isEmpty() )
		dev->setDescription( description );

	if( !devNamePrefix.isEmpty() )
		dev->setDeviceNameTpl( devNamePrefix );

	dev->setSwModules( modelSwMods );

	if( subDevOffTbl.count() > 0 )
		dev->setSubDevOffTbl( subDevOffTbl );

	dev->setDebugSettingMods( dbgSettingMods );

	if( autoId )
		dev->setAutoId( autoId );

	if( bbSlotSpecList ){
		dev->addBbSlotSpec( bbSlotSpecList );
		delete bbSlotSpecList;
		bbSlotSpecList = 0;
	}

	dev->setNativeDevice( isNative );

	if( !mmodDataCap.isEmpty() ){
		HwComponent::MmodDataCaps caps;
		if( (caps = HwComponent::mmodDataMap.strToKey( mmodDataCap )) ==
			HwComponent::MmodDInvalid ){
			errMsg += errPf + "Illegal \"mmoddatacap\" tag " +
				mmodDataCap + "\n";
		}
		else
			dev->setMmodDataRequirements(caps);
	}

	if( !mmodAddrCap.isEmpty() ){
		HwComponent::MmodAddrCaps caps;
		if( (caps = HwComponent::mmodAddrMap.strToKey( mmodAddrCap )) ==
			HwComponent::MmodAInvalid ){
			errMsg += errPf + "Illegal \"mmodaddrcap\" tag " +
				mmodAddrCap + "\n";
		}
		else
			dev->setMmodAddrRequirements(caps);
	}
 abort:
	return dev;
}

//! Build descriptor entries from settingslist
/*!
  find "settingslist" node in \a parent node. Convert all "setting" and
  "settingsubdir" nodes below "settingslist" to Descriptor entries and
  put them in \a dir.

  Any errors are put into errMsg

*/
void
PackageFile::makeDescriptor( QDomNode parent, DescriptorEntryDirectory *dir,
							 QString &errMsg )
{
	QDomNode settingList = findNode( parent, "settinglist",
									 QDomNode::ElementNode);

	if( settingList.isNull() )
		return;

	makeDescriptorSubdir( settingList, dir, errMsg );
}

void
PackageFile::makeDescriptorSubdir( QDomNode parent,
								   DescriptorEntryDirectory *dir,
								   QString &errMsg )
{
	QDomNode n = parent.firstChild(), cn;
	DescriptorEntryDirectory *newDir;
	DescriptorEntry *e;
	DescriptorEntryUint32 *e32;
	DescriptorFactory dFact;
	QString ePath = dir->path().section("/",1);
	QString orgPf = errPf;
	QString hiddenStr;
	bool hiddenVal;

	for( ; ! n.isNull(); n = n.nextSibling() ){

		errPf = orgPf + ePath + ": ";

		if( n.nodeName() == "setting" ){
			QString eName, eType, eValue, eDefaultValue, eDescription;
			ulong eMinValue=0, eMaxValue=0xffffffff;

			// Descriptor entry found
			// the "setting" tag may have the "hidden" attribute
			hiddenStr = n.toElement().attribute( "hidden", "false" );

			if( hiddenStr == "false" )
				hiddenVal = false;
			else if( hiddenStr == "true" )
				hiddenVal = true;
			else {
				errMsg += errPf + "Illegal value for \"hidden\" attribue\n";
				continue;
			}


			// Lookup "name" (required)
			cn = findNode( n, "name", QDomNode::ElementNode );
			if( cn.isNull() ){
				errMsg += errPf + "Missing \"name\" tag for \"setting\"\n";
				continue;
			}
			eName = nodeText(cn);
			errPf = orgPf + ePath + "/" + eName + ": ";

			// Lookup "type" (required)
			cn = findNode( n, "type", QDomNode::ElementNode );
			if( cn.isNull() ){
				errMsg += errPf + "Missing \"type\" tag\n";
				continue;
			}
			eType = nodeText(cn);

			// Lookup "defaultvalue" (required)
			cn = findNode( n, "defaultvalue", QDomNode::ElementNode );
			if( cn.isNull() ){
				errMsg += errPf + "Missing \"defaultvalue\" tag\n";
				continue;
			}
			eDefaultValue = nodeText(cn);

			// Lookup "description" (optional)
			cn = findNode( n, "description", QDomNode::ElementNode );
			if( ! cn.isNull() )
				eDescription = nodeText(cn);

			// Lookup "value" (optional)
			cn = findNode( n, "value", QDomNode::ElementNode );
			if( ! cn.isNull() )
				eValue = nodeText(cn);

			// create descriptor entry
			// activate it when a "value" tag or hidden attribute was found
			e = dFact.create( eName, eType, eDefaultValue,
							  eDescription,
							  (!eValue.isEmpty() || hiddenVal) ? true : false );
			if( !e )
				errMsg += errPf + "Bad type or value\n";
			else {
				wDebug(("Created %s", e->path().latin1()));
				e->setHidden( hiddenVal );

				// do extended processing for U_INT32 entries
				if( e->getType() == DescriptorEntry::Uint32 ){
					e32 = (DescriptorEntryUint32 *)e;

					// Lookup "minValue" (optional)
					cn = findNode( n, "minvalue", QDomNode::ElementNode );
					if( ! cn.isNull() ){
						if( Util::getInt( nodeText(cn), &eMinValue, 0 )
							== false )
							errMsg += errPf + "Bad \"minvalue\" tag\n";
					}

					// Lookup "maxValue" (optional)
					cn = findNode( n, "maxvalue", QDomNode::ElementNode );
					if( ! cn.isNull() ){
						if( Util::getInt( nodeText(cn), &eMaxValue, 0 )
							== false )
							errMsg += errPf + "Bad \"maxvalue\" tag\n";
					}

					e32->setRange( eMinValue, eMaxValue );

					// Lookup and process "choises" (optional)
					QDomNode choises = findNode( n, "choises",
												 QDomNode::ElementNode);
					if( !choises.isNull() )
						makeDescChoises( choises, e32, errMsg );

				}

				if( !eValue.isEmpty() )
					e->setValue( eValue );

				dir->addChild( e );
			}

		}
		else if( n.nodeName() == "settingsubdir" ){
			// Descriptor subdirectory found
			cn = findNode( n, "name", QDomNode::ElementNode );

			if( cn.isNull() )
				errMsg += errPf + "Missing \"name\" tag for "
					"\"settingsubdir\"\n";
			else {
				int rangeStart =
					n.toElement().attribute( "rangestart", "0" ).toInt();
				int rangeEnd =
					n.toElement().attribute( "rangeend", "0" ).toInt();

				for( int subidx=rangeStart; subidx<=rangeEnd; subidx++ ){
					QString subDirName = nodeText(cn);

					if( rangeEnd > rangeStart )
						subDirName += QString("%1").arg(subidx);

					newDir = new DescriptorEntryDirectory( subDirName );
					wDebug(("newDir %s", (const char *)subDirName));
					dir->addChild( newDir );
					errPf = orgPf;
					makeDescriptorSubdir( n, newDir, errMsg );
					errPf = orgPf + ePath + ": ";
				}
			}
		}
		else if( n.nodeName() == "debugsetting" ){
			dbgSettingMods = 0;

			if( n.toElement().attribute( "drv", "true" ) == "true")
				dbgSettingMods |= (1<<DebugProperties::Driver);
			if( n.toElement().attribute( "mk", "true" ) == "true")
				dbgSettingMods |= (1<<DebugProperties::MdisKernel);
			if( n.toElement().attribute( "oss", "true" ) == "true")
				dbgSettingMods |= (1<<DebugProperties::Oss);
			if( n.toElement().attribute( "desc", "true" ) == "true")
				dbgSettingMods |= (1<<DebugProperties::Desc);
			if( n.toElement().attribute( "mbuf", "false" ) == "true")
				dbgSettingMods |= (1<<DebugProperties::Mbuf);
		}

	}

	errPf = orgPf;
}

//! Scan list of choises for multiple choise U_INT32 descriptor entries
/*!
  \param parent	node that contains choises list
  \param e		U_INT32 entry that shall receive choises
  \param errMsg	to add error messages
*/
void PackageFile::makeDescChoises( QDomNode parent,
								   DescriptorEntryUint32 *e,
								   QString &errMsg )
{
	QDomNode n = parent.firstChild(), cn;
	QString orgPf = errPf;

	for( ; ! n.isNull(); n = n.nextSibling() ){

		errPf = orgPf + "choises: ";

		if( n.nodeName() == "choise" ){
			ulong eValue;
			QString eDescription;

			// Choise found

			// Lookup "value" (required)
			cn = findNode( n, "value", QDomNode::ElementNode );
			if( cn.isNull() ){
				errMsg += errPf + "Missing \"value\" tag for \"choise\"\n";
				continue;
			}

			if( Util::getInt( nodeText(cn), &eValue, 0 ) == false )
				errMsg += errPf + "Bad \"value\" tag\n";

			// Lookup "description" (required)
			cn = findNode( n, "description", QDomNode::ElementNode );
			if( cn.isNull() ){
				errMsg += errPf + "Missing \"description\" tag\n";
				continue;
			}
			eDescription = nodeText(cn);

			// add the choise to the entrie's choises map
			wDebug(("%s add val %ld = %s", errPf.latin1(), eValue,
				   eDescription.latin1()));

			e->choises[eValue] = eDescription;
		}
	}
	errPf = orgPf;
}

//! Build SwModuleList entries from swmodulelist
/*!
  find "swmodulelist" node in \a parent node. Convert all "swmodule"
  nodes below "swmodulelist" to a SwModule object and
  put them in \a lst.

  Any errors are put into errMsg

*/
void
PackageFile::makeSwModules( QDomNode parent, SwModuleList *lst,
							QString &errMsg )
{
	QDomNode swlist = findNode( parent, "swmodulelist",
								QDomNode::ElementNode);

	QDomNode n, cn;
	QString orgPf = errPf;

	wDebug(("makeSwModules"));
	if( swlist.isNull() )
		return;
	wDebug(("makeSwModules found swlist"));

	for( n = swlist.firstChild(); ! n.isNull(); n = n.nextSibling() ){

		errPf = orgPf + ": ";

		if( n.nodeName() == "swmodule" ){
			QString eName, eType, eDescription, eMakefilePath;
			int eTypeCode;
			bool isInternal, isSwapCapable;

			wDebug(("makeSwModules found swmodule"));
			// swmodule entry found

			isInternal = (n.toElement().attribute( "internal", "false" )
						  == "true");

			isSwapCapable = (n.toElement().attribute( "swap", "true" )
							 == "true");

			// Lookup "name" (required)
			cn = findNode( n, "name", QDomNode::ElementNode );
			if( cn.isNull() ){
				errMsg += errPf + "Missing \"name\" tag for \"swmodule\"\n";
				continue;
			}
			eName = nodeText(cn);

			errPf = orgPf + " " + eName + ": ";

			// Lookup "type" (required)
			cn = findNode( n, "type", QDomNode::ElementNode );
			if( cn.isNull() ){
				errMsg += errPf + "Missing \"type\" tag\n";
				continue;
			}
			eType = nodeText(cn);
			eTypeCode = G_swModTypes->findStringVal( eType );
			if( eTypeCode == -1 ){
				errMsg += errPf + "Illegal type \"" + eType + "\"\n";
				continue;
			}

			// Lookup "description" (required)
			cn = findNode( n, "description", QDomNode::ElementNode );
			if( cn.isNull() ){
				errMsg += errPf + "Missing \"description\" tag\n";
				continue;
			}
			eDescription = nodeText(cn);

			// Lookup "makefilepath" (required)
			cn = findNode( n, "makefilepath", QDomNode::ElementNode );
			if( cn.isNull() ){
				errMsg += errPf + "Missing \"makefilepath\" tag\n";
				continue;
			}
			eMakefilePath = nodeText(cn);

			// create the SwModule and add it to list
			SwModule *mod = new SwModule( eTypeCode, eName, eMakefilePath,
										  eDescription );

			mod->setInternal( isInternal );
			mod->setSwapCapable( isSwapCapable );

			// lookup <os> and <notos> tags
			for( cn = n.firstChild(); ! cn.isNull(); cn = cn.nextSibling() ){

				if( cn.nodeName() == "os" || cn.nodeName() == "notos" ){
					QString osName = nodeText(cn);
					OsFactory::OsType osType;

					if( (osType = OsFactory::strToType( osName )) ==
						(OsFactory::OsType)-1 ){
						errMsg += errPf + "invalid \"os/notos\" value\n";
					}
					else {
						if( cn.nodeName() == "os" )
							mod->includeOs( osType );
						else
							mod->excludeOs( osType );
					}
				}
			}
			lst->add( mod );
		}
	}
	errPf = orgPf;

}

//! Build Device AutoId
/*!
  find "autoid" node in \a parent node.

  Further processing depends on \a busIfType:
  - For M-Modules, subtag "mmoduleid" must exist
  - For PCI-devices, check if the descriptor entries
    PCI_VENDOR_ID..PCI_SUBSYS_ID are existant in \a desc.
	If so, build the auto-id info based on that keys.

  Any errors are put into errMsg
  \return a new object describing auto-id or 0 if no or invalid auto-id found.
*/
DeviceAutoId *
PackageFile::makeAutoId( QDomNode parent, DescriptorEntryDirectory *desc,
						 HwComponent::BusIfType busIfType, QString &errMsg )
{
	QDomNode n = findNode( parent, "autoid", QDomNode::ElementNode );
	QDomNode cn;
	QString txt;
	DeviceAutoId *ai = 0;
	DeviceChamV2AutoId *chamV2ai = NULL;

	if( n.isNull() )
		return 0;

	wDebug(("autoid found"));
	switch( busIfType ){

	case HwComponent::Mmod:
	{
		ulong val;
		cn = findNode( n, "mmoduleid", QDomNode::ElementNode);
		if( cn.isNull() ){
			errMsg += errPf + "autoid: missing \"mmoduleid\" tag\n";
			break;
		}
		Util::getInt( nodeText(cn), &val, 0 );
		ai = new DeviceMmodAutoId( val );
		break;
	}
	case HwComponent::StdPci:
	case HwComponent::StdPciE:
	case HwComponent::Cpci:
	case HwComponent::CpciSer:
	case HwComponent::PcMip:
	case HwComponent::Pmc:
	case HwComponent::Xmc:
	case HwComponent::Pci104:
	{
		DescriptorEntryUint32 *de;

		// check for descripor keys PCI_xxx and use them as autoid info.
		DevicePciAutoId *pai = new DevicePciAutoId();

		if( (de = (DescriptorEntryUint32 *)
			 desc->find( "PCI_VENDOR_ID", DescriptorEntry::Uint32 )))
			pai->pciVendorId = de->getValue();

		if( (de = (DescriptorEntryUint32 *)
			 desc->find( "PCI_DEVICE_ID", DescriptorEntry::Uint32 )))
			pai->pciDeviceId = de->getValue();

		if( (de = (DescriptorEntryUint32 *)
			 desc->find( "PCI_SUBSYS_VENDOR_ID", DescriptorEntry::Uint32 )))
			pai->pciSubvenId = de->getValue();

		if( (de = (DescriptorEntryUint32 *)
			 desc->find( "PCI_SUBSYS_ID", DescriptorEntry::Uint32 )))
			pai->pciSubsysId = de->getValue();

		if( pai->pciVendorId == 0xFFFFFFFF || pai->pciDeviceId == 0xFFFFFFFF ){
			errMsg += errPf + "autoid: incomplete PCI parameters!\n";
			delete pai;
			pai = 0;
		}
		ai = pai;
		break;
	}
	default:
	{
		ulong id = 0;
		ulong idV2 = 0;
		ulong grpMemId = 0;

		// look for chameleon v2 id
		cn = findNode( n, "chamv2id", QDomNode::ElementNode);
		if ( cn.isNull() ){
			// package does not support chameleon v2
			// read original chamunitid
			cn = findNode( n, "chamunitid", QDomNode::ElementNode);
			if( cn.isNull() ){
				errMsg += errPf + "autoid: unsupported bustype or unsupported autoid\n";
				break;
			}
			else {
				// create original chameleon device
				Util::getInt( nodeText(cn), &id, 0 );
				ai = new DeviceChamAutoId( id );
				wDebug(("created original chameleon device: id=%lu", id));
				break;
			}
		}
		else {
			// found chamv2 device
			Util::getInt( nodeText(cn), &idV2, 0 );

			// read original chamId (for backwards compatibility
			cn = findNode( n, "chamunitid", QDomNode::ElementNode);
			if ( !cn.isNull() ){
				Util::getInt( nodeText(cn), &id, 0 );
			}

			// create chameleonV2 device
			chamV2ai = new DeviceChamV2AutoId( idV2, id );
			wDebug(("created chamV2 device: id=%lu, oldId=%lu", idV2, id));

			// read the group members if given (optional)
			cn = findNode( n, "chamv2grpmemid", QDomNode::ElementNode);
			if ( !cn.isNull() ) {
				QList<QString> grpMemIdList = QStringList::split(' ', nodeText(cn));
				wDebug(("member count: %d\n", grpMemIdList.count()));

				QList<QString>::iterator it = grpMemIdList.begin();
				while (it != grpMemIdList.end()) {
					Util::getInt((*it), &grpMemId, 0);

					chamV2ai->chamv2grpList->push_back( grpMemId );
					wDebug(("added group member: id=%lu", grpMemId));

					it++;
				}
			}

			ai = chamV2ai;

			break;
		}

	} // end default
	} // end switch
	return ai;
}

//! Build Device BbSlotSpec objects
/*!
  find all "bbslot" nodes in \a parent node and build a BbSlotSpec object
  list

  Any errors are put into \a errMsg
  \return BbSlotSpec list or null if no bbslot tags found
*/
BbSlotSpecList *
PackageFile::makeBbSlotSpec( QDomNode parent, QString &errMsg )
{
    BbSlotSpecList *lst = 0;

	QDomNode n = parent.firstChild();

	while( !n.isNull() ){
		QDomNode cn;
		QString bbisModel, bbisInstance;
		int slot = -1;
		bool ok = true;

		if( n.nodeName() == "bbslot" &&
			n.nodeType() == QDomNode::ElementNode ){

			// look for bbismodel
			cn = findNode( n, "bbismodel", QDomNode::ElementNode);
			if( !cn.isNull() )
				bbisModel = nodeText( cn );

			// look for bbisinstance
			cn = findNode( n, "bbisinstance", QDomNode::ElementNode);
			if( !cn.isNull() )
				bbisInstance = nodeText( cn );

			// look for slot
			cn = findNode( n, "slot", QDomNode::ElementNode);
			if( !cn.isNull() ){
				ulong val;
				Util::getInt( nodeText(cn), &val, 0 );
				slot = val;
			}

			// validate
			if( bbisModel == QString::null && bbisInstance == QString::null ){
				errMsg += errPf + "bbslot: must specify either bbmodel or "
					"bbisinstance\n";
				ok = false;
			}
			else if( bbisModel != QString::null &&
					 bbisInstance != QString::null ){
				errMsg += errPf + "bbslot: do not specify bbmodel and "
					"bbisinstance\n";
				ok = false;
			}
			if( ok ){
				BbSlotSpec *spec = new BbSlotSpec( bbisModel, bbisInstance,
												   slot );
				if( !lst )
					lst = new BbSlotSpecList;

				lst->add( spec );
			}
		}
		n = n.nextSibling();
	}
	return lst;
}


//! find in node \a parent the child node \a name with type \a type
/*!
  \return node found or a null node if not found
*/
QDomNode
PackageFile::findNode( QDomNode parent, QString name, QDomNode::NodeType type )
{
	QDomNode cn = parent.firstChild();

	while( !cn.isNull() ){
		if( cn.nodeName() == name && cn.nodeType() == type )
			return cn;
		cn = cn.nextSibling();
	}
	cn.clear();
	return cn;
}

//! find in node \a parent all child nodes \a name with type \a type
/*!
  \return list of nodes found or an empty list if not found
*/
Q3ValueList<QDomNode>
PackageFile::findNodes( QDomNode parent, QString name, QDomNode::NodeType type )
{
	Q3ValueList<QDomNode> nodeList;

	QDomNode cn = parent.firstChild();
	while( !cn.isNull() ){
		if( cn.nodeName() == name && cn.nodeType() == type )
		{
			nodeList.push_back(cn);
		}
		cn = cn.nextSibling();
	}


	return nodeList;
}

//! dump XML doc
void
PackageFile::dump()
{
	if( ! doc )
		return;
	wDebug(("=== DUMP of XML file %s ===", (const char *)fileName ));
	dumpNode( doc, 0 );
	wDebug(("=== ENDDUMP of XML file %s ===", (const char *)fileName ));
}

void
PackageFile::dumpNode( QDomNode *n, int indent )
{
	wDebug(("%*s%s type=%d text=\"%s\"", indent*2, "",
		   (const char *)n->nodeName(),
		   (int)n->nodeType(),
		   /*n->isText() ? (const char *)n->toText().data() : "<none>"*/
		   n->isCharacterData() ? (const char *)n->toCharacterData().data() : "<none cd>" ));

	//wDebug(("%*s%s",  indent*2, "", n->isElement() ? (const char *)n->toElement().text() : "<notext>"));
	QDomNode cn = n->firstChild();

	while( !cn.isNull() ){
		dumpNode( &cn, indent+1 );
		cn = cn.nextSibling();
	}
}

//! extract text from the Node \a n
/*!
  Collapses the string as specified in XML documentation:
  - replace newlines/tabs by white-space
  - replace multiple subsequent spaces by single space

  Note: Even if you insert XML char references (e.g. &#10;) they are
  replaced. There is currently no way to force a line feed.
*/
QString
PackageFile::nodeText( QDomNode n )
{
	QString s = n.toElement().text();

	//wDebug(("org=\"%s\"\n", s.latin1() ));
	// replace newlines/tabs by white-space
	s = s.replace( QRegExp("[\n\t]"), " " );
	// replace multiple subsequent spaces by single space
	s = s.replace( QRegExp(" +"), " " );
	//wDebug(("rep=\"%s\"\n", s.latin1() ));

	return s;
}







