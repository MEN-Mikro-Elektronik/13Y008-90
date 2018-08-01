/***************************************************************************
 */
/*!       \file  util.cpp
 *       \brief  Implementation of misc standalone utility functions
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:28:10 $
 *    $Revision: 2.0 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: util.cpp,v $
 * Revision 2.0  2010/01/22 11:28:10  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.9  2006/11/30 11:28:05  cs
 * added fileCopy member
 *
 * Revision 1.8  2005/07/15 15:47:17  ub
 * added function to set environment
 *
 * Revision 1.7  2004/07/30 11:45:53  ufranke
 * added
 *  - stdlib.h for Visual C++.net Standard
 *
 * Revision 1.6  2004/04/13 11:01:26  dpfeuffer
 * cosmetics
 *
 * Revision 1.5  2004/01/09 08:20:38  kp
 * cosmetics
 *
 * Revision 1.4  2003/06/05 12:01:32  dschmidt
 * intermediate checkin
 *
 * Revision 1.3  2003/06/05 09:34:04  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/06/03 10:16:58  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:36:35  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#include <qfile.h>
#include <qfileinfo.h>
#include <qstring.h>
#include <qregexp.h>
#include <qstringlist.h>

#include <Q3TextStream>
#include <QDateTime>

#include "wizdebug.h"
#include "util.h"
#include "stdlib.h"
#include "wizexcept.h"

#ifndef WIN32
# include <sys/types.h>
# include <utime.h>
#else
# include <sys/utime.h>
# define utime _utime
# define utimbuf _utimbuf
#endif

#ifdef WIN32
#include "Windows.h"
#endif


//! break a string into lines with a max length of \a len at word boundaries
/*!
  line seperators in \a inStr will be still be present in the output
*/
QString
Util::breakString( const QString &inStr, uint len)
{
	QString outStr;
	QStringList inLines = QStringList::split( "\n", inStr, true );

	for ( QStringList::Iterator it = inLines.begin(); it != inLines.end();
		  ++it ) {

		QString outLine;
		QStringList line = QStringList::split(QRegExp("[ \t]+"), *it );
		uint lineLen = 0;

		for ( QStringList::Iterator wit = line.begin(); wit != line.end();
		  ++wit ) {
			if( lineLen + (*wit).length() > len ){
				outLine += "\n" + *wit;
				lineLen = ((*wit).length());
			}
			else {
				if( !outLine.isEmpty() ){
					outLine += " " + *wit;
					lineLen += ((*wit).length()) + 1;
				}
				else {
					outLine = *wit;
					lineLen = ((*wit).length());
				}
			}
		}

		outStr += outLine + "\n";
    }

	return outStr.left(outStr.length()-1); // remove last \n
}

//! Perform "grep" on a file
/*!
  Reads the file \a fileName line-by-line and checks each line if it
  matches the regular expression \a expr.

  All matching lines are added to the passed QStringList \a results

  \return true=file opened and parsed successfully, false=error reading file

*/
bool
Util::fileGrep( const QString &fileName, const QRegExp &expr,
				QStringList *results )
{
	QFile file;

	// open file
	file.setName( fileName );
	if( file.open( QIODevice::ReadOnly) == false )
		return false;

	Q3TextStream t( &file );        // use a text stream
	QString line;

	while ( !t.eof() ) {        	// until end of file...
		line = t.readLine();       	// line of text excluding '\n'

		//wDebug(("line: %s", line.latin1() ));
		if( line.find( expr ) >= 0 ){ // check if it line matches expr
			*results += line;
			//wDebug(("match: %s", line.latin1() ));
		}
	}

	file.close();
	return true;
}

//! Add element to path-like environment variable
/*!
  Adds \a newElem to the environment variable \a env if this element is
  not already present in \a env.

  The new element is always inserted at beginning of elements
*/
void
Util::addPath( const QString &env, const QString &newElem )
{
	char *envValCString=NULL;
	QString val;

	if( (envValCString = getenv( (const char *)env )) ){
		val = envValCString;

		// break up path list into elements
		QStringList elems = QStringList::split( ':', val );

		// remove old entry (if any)
		elems.remove( newElem );

		// add new element at beginning of string
		elems.prepend( newElem );

		val = elems.join(":");
	}
	else {
		// environment var did not exist. create it
		val = newElem;
	}
	setEnv( env, val);
	//wDebug(("Util::addPath %s newval=%s", (const char *)env, (const char *)val));
}


//! convert string to integer
/*!
  string may be preceeded by 0x/0X for hex numbers or % for binary numbers

  \return true if conversion successfull, \a *rvP contains converted value,
  \a *bsP contains the scanned base (2,10,16) (if *bsP is nonzero)

  Scans string until end of string, blanks are not allowed
*/
bool
Util::getInt( const QString &str, ulong *rvP, int *bsP )
{
	int bs, idx=0;
	ulong newVal=0, tmp;
	QChar c;

	if( str.startsWith("0x") || str.startsWith("0X") ){
		bs = 16;
		idx += 2;
	}
	else if( str.startsWith("%") ){
		bs = 2;
		idx++;
	}
	else
		bs = 10;

	//wDebug(("Util::getInt str %s bs %d", (const char *)str, bs ));
	for( ; (c = str.data()[idx]) != QChar::Null; idx++ ){
		if( c >= '0' && c <= '9' )
			tmp = c.toAscii() - '0';
		else if( c >= 'a' && c <= 'f' )
			tmp = c.toAscii() - 'a' + 10;
		else if( c >= 'A' && c <= 'F' )
			tmp = c.toAscii() - 'A' + 10;
		else
			goto ex;

		if( (int)tmp >= bs )
			goto ex;

		newVal = (newVal*bs) + tmp;
	}
	*rvP = newVal;
	if( bsP )
		*bsP = bs;
	return true;
 ex:
	return false;
}

//! concat two file/directory path elements
/*!
  Handle cases where path separator is already included at end
  of \a p1 or beginning of \a p2
  \return concatenated path
*/
QString
Util::joinPath( const QString &p1, const QString &p2 )
{
	if( p1 == QString::null || p1 == "" )
		return p2;

	if( p1.endsWith("/") || p2.startsWith("/" ))
		return p1 + p2;

	return p1 + "/" + p2;
}

#ifdef WIN32
//! convert a QString to windows unicode. buf must be large enough!
static LPCTSTR toUnicode( const QString &str, LPTSTR buf )
{
	for( int i=0; i<str.length()+1; i++ )
		buf[i] = str.at(i).unicode();
	return buf;
}

#endif // WIN32


//! Set environment variable
/*!
*/
void
Util::setEnv( const QString &varName, const QString &value )
{
#ifdef WIN32

	TCHAR varBuf[1000];
	TCHAR valBuf[1000];

	toUnicode( varName, varBuf );
	toUnicode( value, valBuf );

	WIZ_ASSERT( SetEnvironmentVariable( varBuf, valBuf ) );

#else // WIN32
	// easy under Linux...
	setenv( (const char *)varName, (const char *)value, true );
#endif
}

//! copy srcFileName to dstFileName
/*
  \a srcFileName and \a dstFileName must be paths (relative or absolute)
  Maintain file dates
*/
bool
Util::fileCopy(
	const QString &srcFileName,
	const QString &dstFileName,
	QString &errMsg)
{
	QFile srcFile( srcFileName );
	QFile dstFile( dstFileName );
	QByteArray copyBuf( 4096 );
	Q_LONG nRead, nWritten = 0;
	bool ok = true;

	wDebug(("Util::fileCopy %s %s", srcFileName.latin1(), dstFileName.latin1() ));

	if( srcFile.open( QIODevice::Unbuffered | QIODevice::ReadOnly ) == false ){
		errMsg += QString("Can't open ") + srcFileName + " for reading\n";
		return false;
	}
	if( dstFile.open( QIODevice::Unbuffered | QIODevice::WriteOnly ) == false ){
		errMsg += QString("Can't open ") + dstFileName + " for writing\n";
		return false;
	}

	// copy files
	while( (nRead = srcFile.readBlock( copyBuf.data(), copyBuf.size() )) > 0 ){
		nWritten = dstFile.writeBlock( copyBuf, nRead );
		if( nWritten < 0 )
			break;
	}

	if( nRead < 0 ){
		errMsg += QString("Error reading ") + srcFileName + "\n";
		ok = false;
	}
	if( nWritten < 0 ){
		errMsg += QString("Error writing ") + dstFileName + "\n";
		ok = false;
	}
	srcFile.close();
	dstFile.close();


	// reset file dates
	{
		// get source time
		QFileInfo fi( srcFileName );
		QDateTime srcDate = fi.lastModified();

		wDebug((" srcDate: %s %d",
			   srcDate.toString("yyyy-MM-dd hh:mm:ss").latin1(),
			   srcDate.toTime_t()));

		// set dest time
		{
			struct utimbuf ub;
			ub.actime = ub.modtime = srcDate.toTime_t();
			char dstF[200];
			strncpy( dstF, dstFileName, sizeof(dstF));

			if( utime( dstF, &ub ) < 0 ){
				errMsg += "Couldn't set date/time on %s\n", dstFileName + "\n";
				ok = false;
			}
		}
	}
	return ok;
}



