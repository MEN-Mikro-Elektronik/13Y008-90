/***************************************************************************
 */
/*!       \file  cfgmakefile.h
 *       \brief  Makefile parser 
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:25:31 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: cfgmakefile.h,v $
 * Revision 2.0  2010/01/22 11:25:31  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.3  2009/04/27 17:26:17  CRuff
 * R: New project settings option UP / SMP for VxWorks
 * M: added new assignment type MkFileTuple::ExtendAssign ('+=')
 *
 * Revision 1.2  2005/12/08 15:54:34  UFranke
 * added
 *  + Ifeq, Else
 *
 * Revision 1.1  2003/05/02 14:34:26  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:19:49  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef CFGMAKEFILE_H
#define CFGMAKEFILE_H

#include <qstringlist.h>
#include <qlist.h>
#include <qfile.h>

#include <Q3PtrList>

#include "wizexcept.h"

//! class to maintain a key and a list of corresponding values (String list)
class MkFileTuple {
public:
	/*!
	 * - For assignments: key value
	 * - For includes: include file name
	 * - For ifdef/ifndef: var name
	 */
	QString key;			
	QStringList val;	//!< for assignments and comments

	//! values for tag member
	enum TagType { None=0, Assignment=0x1, Include=0x2, 
				   Comment=0x3, Ifdef=0x4, Ifndef=0x5, Endif=0x6,
				   Ifeq=0x7, Else=0x8,
				   TypeMask=0xf, Export=0x80, HardAssign=0x100, ExtendAssign=0x200};
	int tag;			//!< type of the tuple
};

//! class to parse and generate a configuration's Makefile
class CfgMakefile {
public:
	CfgMakefile( const QString &_fileName );

	// read makefile
	Q3PtrList<MkFileTuple> *read();

	// write makefile
	void write( Q3PtrList<MkFileTuple> *valList );

	//! returns the filename of the current Makefile
	QString getFileName() { return file.name(); }

	//! returns the line number within the current Makefile
	int getLine() { return currentLine; }

	// exceptions
	class Except : public WizExcept {
	public:
		Except() {}
	};

	class CannotOpen : public Except {
	public:
		CannotOpen( CfgMakefile &cfgMakefile ) {
			msg = QString("%1: Cannot open").arg(cfgMakefile.getFileName());
		}
	};

	class SyntaxError : public Except {
	public:
		SyntaxError( CfgMakefile &cfgMakefile, QString line ) {
			msg = QString("%1: Syntax Error in line %2: %3").
				arg(cfgMakefile.getFileName()).arg(cfgMakefile.getLine()).
				arg(line);
		}
	};


protected:
	QFile file;				//!< file currently opened for reading/writing
	int currentLine;		//!< current line while parsing file

	QString GetLine();
	MkFileTuple *LineToTuple( QStringList &lst );
};


#endif

