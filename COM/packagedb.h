/***************************************************************************
 */
/*!       \file  packagedb.h
 *       \brief  Parse all XML model files in a directory
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:27:16 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: packagedb.h,v $
 * Revision 2.0  2010/01/22 11:27:16  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.2  2003/06/03 10:16:33  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:36:17  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef PACKAGEDB_H
#define PACKAGEDB_H

#include <qdir.h>

class ModelFactory;
// -----------------------------------------------------------------
//! Parser for all XML model files in a directory
/*!
  This allows to scan a directory for all \c "*.xml" files. Each
  XML file is assumed to be a model package file as defined by PackageFile.

  The parse() method does the real work. 
*/
class PackageDb {
public:
	PackageDb( const QString &dirName );

	bool parse( ModelFactory *factory );
	void showPackages();
private:
	QDir dir;
};

#endif
