/***************************************************************************
 */
/*!       \file  xmldump.cpp 
 *       \brief  dump XML file as directory tree
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:28:24 $
 *    $Revision: 2.0 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: xmldump.cpp,v $
 * Revision 2.0  2010/01/22 11:28:24  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.5  2004/05/10 15:21:03  UFranke
 * fixed
 *  - return type for attrText
 *
 * Revision 1.4  2004/04/13 15:04:17  dpfeuffer
 * cosmetics
 *
 * Revision 1.3  2004/04/13 11:01:29  dpfeuffer
 * cosmetics
 *
 * Revision 1.2  2004/03/12 11:53:39  dpfeuffer
 * -attr option added
 *
 * Revision 1.1  2004/01/09 08:20:42  kp
 * -dir <xmldir> option added
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#include <qapplication.h>
#include <qdatetime.h>
#include <qdom.h>
#include <qfile.h>
#include <qregexp.h>
#include <stdio.h>

#include "wizdebug.h"
#include "configuration.h"
#include "targetos.h"
#include "util.h"


extern TargetOs *G_os;					//!< target OS object
bool G_attr = FALSE;

static void usage()
{
	const QString helpText = 
		"Usage: mdiswiz -xmldump <xmlfile> [-dir <xmldir> -attr]\n"
		"Function: Dump XML file as directory tree\n"
		" <xmlfile>   filename of package XML file without path\n"
		"Options:\n"
		" -dir <xmldir>: look in specified <xmldir> instead of OS specific default dir\n"
		" -attr        : dump also XML tag attributes\n";

	printf( "%s\n", helpText.latin1() );
}

static QString formatText( QString s )
{
	// replace newlines/tabs by white-space
	s = s.replace( QRegExp("[\n\t]"), " " );
	// replace multiple subsequent spaces by single space
	s = s.replace( QRegExp(" +"), " " );

	return s;
}

static QString nodeText( QDomNode *n )
{
	QString s = n->toElement().text();
	return formatText( s );
}

static void attrText( QDomNode *n, QString locPath )
{
	QString name = n->toAttr().name();
	QString value = n->toAttr().value();

	name = formatText( name );
	value = formatText( value );

	printf("%s/ATTR/%s \"%s\"\n",  
		locPath.latin1(), name.latin1(), value.latin1() );
}

static void dumpAttr( QDomNode *n, QString locPath )
{
	// extract the attributes
	QDomNamedNodeMap nnm = n->attributes();

	for( int i=0; i<nnm.count(); i++ ){
		QDomNode *na = &nnm.item( i ); 
		attrText(na, locPath);
	}
}

static void dumpNode( QString xPath, QDomNode *n )
{
	QString locPath = Util::joinPath( xPath, n->nodeName() );
	QDomAttr a;

	if( n->isElement() && n->firstChild().isText() ){
		printf("%s \"%s\"\n",  
			   locPath.latin1(),
			   nodeText(n).latin1() );
	}

	// dump attributes (optional)
	if( G_attr )
		dumpAttr( n, locPath );
	
	QDomNode cn = n->firstChild();

	while( !cn.isNull() ){
		dumpNode( locPath, &cn );
		cn = cn.nextSibling();
	}
}

//! dump XML file as directory tree
/*
  Usage: mdiswiz -xmldump xmlfile [-dir xmldir]
 
  Options:
    -dir <xmldir>: look in specified xmldir instead of OS specific default dir\n
	-attr        : dump also XML tag attributes\n";

  Dumps the entire XML hierarchy in the following form:

  \verbatim
  package/name "13m06706"
  package/description "MDIS4 driver package for MEN M67 M-Module"
  package/date "2003-05-15"
  package/revision "1.1"
  package/docroot "DRIVERS/MDIS_LL/M067/DRIVER/DOC/index.html"
  package/modellist/model/vendor "MEN"
  package/modellist/model/hwname "M67"
  package/modellist/model/modelname "M67_D32"
  package/modellist/model/description "M67 Driver using D32 access"
  ...
  \endverbatim

  XML tag attributes are silently ignored per default.
  If the -attr option is specified, then all XML tag attributes
  will also be dumped: 

  \verbatim
  ...
  package/settinglist/setting/ATTR/hidden "true"
  package/settinglist/setting/name "IRQ_ENABLE"
  package/settinglist/setting/description "enable irq after init"
  package/settinglist/setting/type "U_INT32"
  package/settinglist/setting/value "1"
  package/settinglist/setting/defaultvalue "0"
  ...	
  package/swmodulelist/swmodule/ATTR/internal "true"
  package/swmodulelist/swmodule/name "canopen_ser_test"
  package/swmodulelist/swmodule/description "Final inspection test program for the CANOPEN driver"
  package/swmodulelist/swmodule/type "Driver Specific Tool"
  package/swmodulelist/swmodule/makefilepath "CANOPEN/TEST/CANOPEN_SER_TEST/COM/program.mak"  ...
  ...
  \endverbatim
*/

int xmlDump( int argc, char **argv )
{
	if( argc < 1 ){
		usage();
		return 1;
	}

	QString xmlDir(G_os->pkgDbPath());
	QString str;

	// optional specified xml dir?
	for( int n=1; n<argc; n++ ){
		str = QString(argv[n]);

		if( str == QString("-dir") ){
			if( ++n < argc )
				xmlDir = QString(argv[n]);
			else{
				usage();
				return 1;
			}
		}
		else if( str == QString("-attr") ){
			G_attr = TRUE;
		}
		else{
			usage();
			return 1;
		}
	}

	QString xmlName(argv[0]);
	QString xmlPath = xmlDir + "/" + xmlName;
	QFile f( xmlPath );
	QDomDocument *doc = 0;
	QString domErrMsg;
	QString errMsg;
	int domErrLine;

	if ( !f.open( QIODevice::ReadOnly ) ){
		errMsg += "Cannot open " + xmlPath;
		goto ERREXIT;
	}

	doc = new QDomDocument( xmlPath );

	// convert XML file to QDom hierarchy
	if ( !doc->setContent( &f, &domErrMsg, &domErrLine ) ) {
		errMsg += QString("line %2: %3\n").arg(domErrLine).arg(domErrMsg);
		goto ERREXIT;
	}

	// Dump node hierarchy
	{
		QDomNode cn = doc->firstChild();

		// find the document root
		while( !cn.isNull() ){

			if( cn.isElement() ){
				dumpNode( 0, &cn );
				break;
			}
			cn = cn.nextSibling();
			
		}
	}
	
	return 0;
 ERREXIT:
	fprintf(stderr,"*** %s\n", errMsg.latin1());
	f.close();
	if( doc ) 
		delete doc;
	return 1;
}


