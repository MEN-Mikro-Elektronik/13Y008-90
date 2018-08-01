/***************************************************************************
 */
/*!       \file  util.h
 *       \brief  Misc utiliy functions
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:28:12 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: util.h,v $
 * Revision 2.0  2010/01/22 11:28:12  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.4  2006/11/30 11:28:07  cs
 * added fileCopy member
 *
 * Revision 1.3  2005/07/15 15:47:20  ub
 * added function to set environment
 *
 * Revision 1.2  2003/06/03 10:17:00  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:36:36  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef UTIL_H
#define UTIL_H

#ifdef WIN32
	#define UTIL_UNREF_PARAM(P)	(P)
#else
	#define UTIL_UNREF_PARAM(P)
#endif

class QString;
class QStringList;
class QRegExp;

//! Class with misc utiliy functions
/*!

  This class contains misc utiliy functions. All functions are static.
*/

class Util {
public:
	static QString breakString( const QString &inStr, uint len=80);
	static bool fileGrep( const QString &fileName, const QRegExp &expr,
						  QStringList *results );
	static bool fileCopy( const QString &srcFileName,
						  const QString &dstFileName,
						  QString &errMsg);
	static void addPath( const QString &env, const QString &newElem );
	static bool getInt( const QString &str, ulong *rvP, int *bsP );
	static QString joinPath( const QString &p1, const QString &p2 );
	static void setEnv( const QString &varName, const QString &value );
};


#endif


