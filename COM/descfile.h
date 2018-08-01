/***************************************************************************
 */
/*!       \file  descfile.h
 *       \brief  MDIS descriptor file parsing and generation
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:25:40 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: descfile.h,v $
 * Revision 2.0  2010/01/22 11:25:40  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.2  2003/05/14 13:42:58  kp
 * new param to descFile.write: headerTex
 *
 * Revision 1.1  2003/05/02 14:34:30  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:19:54  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef DESCFILE_H
#define DESCFILE_H

#include "qfile.h"
#include "descriptor.h"
#include "wizexcept.h"

class DescFile;

// -----------------------------------------------------------------
//! class to parse and generate MDIS descriptor files
/*!
  This class is assigned a file name through it's constructor.

  The descriptor file can be parsed using the read() method. 
  The read() function converts the file into a hierarchy of ::Descriptor 
  objects.
  
  The opposite tranformation is done by the write() method
*/
class DescFile {
public:
	DescFile( const QString &_fileName );

	DescriptorEntryDirectory *read();
	void write( DescriptorEntryDirectory *_root, const QString &headerText);
	QString getFileName();
	int getCurrentLine();

protected:	
	QFile file;				//!< currently opened file
	int currentLine;		//!< current line while parsing file
private:
	bool GetLine( char *lineBuf, char *commentBuf );
	void ParseLine( char *lineBuf );
	bool IsValidCSymbol( char *p );
	char *SkipWhite( char *p );
	char *GetSymbol( char *p, char *buf, long len );


	DescriptorEntryDirectory *currentDir;
	
};				  

// -----------------------------------------------------------------
//! general exception generated by DescFile

class DescFileExcept : public WizExcept {
public:	
};

//! Exception that is thrown when syntax error detected while parsing descriptor file
class DescFileSyntaxError : public DescFileExcept {
public:
	//! creates an DescFileSyntaxError exception object
	DescFileSyntaxError( const QString &_msg, DescFile &descFile) {
		msg = QString("%1: Syntax Error in line %2: %3").
			arg(descFile.getFileName()).
			arg(descFile.getCurrentLine()).
			arg(_msg);
	}
};

//! Exception that is thrown when descriptor file can't be opened
class DescFileCannotOpen : public DescFileExcept {
public:
	//! creates an DescFileCannotOpen exception object
	DescFileCannotOpen( DescFile &descFile) {
		msg = QString("%1: Cannot open").arg(descFile.getFileName());
	}
};

//! Exception that is thrown when when line in descriptor file is too long
class DescFileLineTooLong : public DescFileExcept {
public:
	//! creates an DescFileLineTooLong exception object
	DescFileLineTooLong( DescFile &descFile) {
		msg = QString("%1: Line %2 too long").
			arg(descFile.getFileName()).
			arg(descFile.getCurrentLine());
	}
};

#endif

