/***************************************************************************
 */
/*!       \file  descfile.cpp 
 *       \brief  Implementation of class DescFile
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:25:38 $
 *    $Revision: 2.0 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: descfile.cpp,v $
 * Revision 2.0  2010/01/22 11:25:38  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.6  2004/06/08 17:25:11  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.5  2004/04/13 11:00:34  dpfeuffer
 * cosmetics
 *
 * Revision 1.4  2003/06/05 12:00:44  dschmidt
 * intermediate checkin
 *
 * Revision 1.3  2003/06/05 09:33:07  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/05/14 13:42:45  kp
 * new param to descFile.write: headerTex
 *
 * Revision 1.1  2003/05/02 14:34:29  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:19:53  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#include "wizdebug.h"
#include "descfile.h"
#include "q3textstream.h"
#include <ctype.h>

static const int maxLineLength = 200;

// -----------------------------------------------------------------
// DescFile

//! creates a DescFile object with file name \a _fileName
/*
  \remark file is not opened before read() or write() is called
*/
DescFile::DescFile( const QString &_fileName )
{
	file.setName(_fileName);
	currentDir = 0;
}

//! read in descriptor file
/*!
  opens the descriptor file, scans it and builds the entire
  hierarchy of descriptors
  \throw DescFileExcept

  \return a DescriptorEntryDirectory object called "root" that contains
  all the descriptor entries from the parsed file
*/
DescriptorEntryDirectory *DescFile::read()
{
	char lineBuf[maxLineLength];
	char commentBuf[maxLineLength];
	QString comment;
	DescriptorEntryDirectory *rootEntry=0;

	if ( file.open(QIODevice::ReadOnly) == false ) {    // file opened successfully
		throw DescFileCannotOpen( *this );
	}

	rootEntry = new DescriptorEntryDirectory( QString("root"), QString::null );
	currentDir = rootEntry;

	currentLine = 0;
	comment = "";

	// scan entire file
	try {

		while( GetLine( lineBuf, commentBuf ) == true ){
			wDebug(("<%s> <%s>", lineBuf, commentBuf ));
			ParseLine( lineBuf );			
		}
	}
	catch( DescFileExcept &e ){
		e = e; // suppress compiler warning
		delete rootEntry;
		file.close();
		throw;		// pass exception to caller
	}
	file.close();

	return rootEntry;
}

//! prints all descriptors under \a root to the file
/*
  First prints \a headerText to file, which may consist of multiple lines 
  and must be formatted by caller (including comment '#')
*/
void 
DescFile::write( DescriptorEntryDirectory *root, const QString &headerText)
{
	if ( file.open( QIODevice::WriteOnly | QIODevice::Truncate ) == false ) {    
		throw DescFileCannotOpen( *this );
	}
	
	Q3TextStream outStream( &file );

	outStream << headerText;

	for( DescriptorEntry *e=root->firstChild(); e; e=root->nextChild())
		e->print( outStream, 0 );
	
	outStream << "# EOF\n";

	file.close();
}

//! get the file name of the DescFile object
QString DescFile::getFileName()
{
	return file.name();
}

//! get the current line while parsing descriptor file
int DescFile::getCurrentLine()
{
	return currentLine;
}

/********************************* GetLine **********************************
 *
 *  Description: Get line from input file
 *			   
 *	Get a single or multiple lines (when continuation sign '\' present) from
 *  the input file. 
 *
 *	Put any comment int commentBuf. 
 *  Remove any trailing white space
 *  End of line can be anything: CR, LF or CR-LF. 
 *  Return '\0' terminated line
 *			   
 *---------------------------------------------------------------------------
 *  Input......: lineBuf	buffer where input line will be stored
 *				 commentBuf	buffer where comment line will be stored
 *  Output.....: Return:	true=ok false=EOF 
 *  Globals....:
 *  Throws.....: DescFileExcept
 ****************************************************************************/
bool 
DescFile::GetLine( char *lineBuf, char *commentBuf )
{
	char *p = lineBuf;
	char *commentP = commentBuf;
	int c=-1, c1;
	bool endOfLine=false;
	int comment=0;
	int count=0, commentCount=0;

	while( endOfLine==false && (c = file.getch()) != -1 ){
		switch(c){
		case 0xa:				/* end-of line chars */
		case 0xd:
			c1 = file.getch();		/* check for two-char end-of line */
			if( (c==0xa && c1 != 0xd) || 
			    (c==0xd && c1 != 0xa))

				file.ungetch(c1);	/* put char back to stream */

			currentLine++;

			if( count==0 || p[-1] != '\\' )	/* check for continuation symbol */
				endOfLine=true;
			else {
				p--;			/* remove continuation symbol */
				count--;
			}
			break;
		case '#':				/* comment start */
			comment=2;
			break;			

		case '/':
			comment++;
			if( comment == 2 ){
				p--;			/* remove first '/' */
				count--;
				break;
			}
				
		default:
			if( comment < 2 ){
				if( ++count >= maxLineLength-1 )
					throw DescFileLineTooLong( *this );
				*p++ = c;		/* put char into line bufer */
			}
			else {
			  
				if( ++commentCount >= maxLineLength-1 )
					throw DescFileLineTooLong( *this );
				*commentP++ = c;		/* put char into line bufer */
			}
		}
	}

	/*--- remove trailing white space ---*/
	while( p != lineBuf ){
		if( p[-1]==' ' || p[-1]=='\t' )
			p--;
		else 
			break;
	}
	*p = '\0';
	*commentP = '\0';
	return c==-1 ? false:true;
}

/********************************* ParseLine ********************************
 *
 *  Description: Parse input line
 *			     Creates the 
 *			   
 *---------------------------------------------------------------------------
 *  Input......: lineBuf		current line, null terminated
 *  Output.....: Return			-
 *  Globals....: 
 *  Throws.....: DescFileExcept(SyntaxError), 	 
 ****************************************************************************/
void 
DescFile::ParseLine( char *lineBuf )
{
	char *p = lineBuf;
	char symbolBuf[100];
	DescriptorEntry *entry;
	QString keyName, typeStr, valueStr;
	DescriptorFactory dFact;

	/*---------------------------------+
	|  Search for object name or '}'   |
	+---------------------------------*/
	p = SkipWhite(p);
	if( *p == '\0' ) return;

	if( *p == '}' ){
		/*-------------------+
		|  End of directory  |
		+-------------------*/
		if( currentDir->getParent() == 0 )
			throw DescFileSyntaxError( QString("unexpected '}'"), *this);

		currentDir = (DescriptorEntryDirectory *)
			currentDir->getParent(); /* up one directory */

		p++;
		return;
	}

	/*---------------------------------------------------+
	|  Must be the name of a tag.                        |
	|  In csource mode, check if this is a valid C-name  |
	+---------------------------------------------------*/
	p = GetSymbol( p, symbolBuf, sizeof(symbolBuf) );

	/*--- check if valid C-symbol ---*/
	if( ! IsValidCSymbol( symbolBuf ))
		throw DescFileSyntaxError( QString("invalid symbol name %3").
								   arg(symbolBuf), *this );
	
	keyName = QString(symbolBuf);

	/*------------------------+
	|  Search for '=' or '{'  |
	+------------------------*/
	p = SkipWhite(p);

	if( *p=='{' ){
		DescriptorEntryDirectory *entryDir = 
			new DescriptorEntryDirectory( keyName, QString::null );

		entry = entryDir;

		p++;
		SkipWhite(p);
	}
	else if( *p=='=' ){
		/*--------------+
		|  Normal tag   |
		+--------------*/
		p++;					/* skip '=' */
		p = SkipWhite(p);

		/*--- get tag type ---*/
		p = GetSymbol( p, symbolBuf, sizeof(symbolBuf) );
		typeStr = QString(symbolBuf);
		
		p = SkipWhite(p);
		/*--- get value ---*/
		valueStr = QString(p);
		
		entry = dFact.create( keyName, typeStr, valueStr, QString::null, true);

		if( entry == 0 )
			throw DescFileSyntaxError( QString("invalid entry %1").
									   arg(lineBuf), *this );				
	}
	else {
		throw DescFileSyntaxError( QString("expected '=' or '{'"), *this );
	}
	
	// add new entry to list of children
	currentDir->addChild( entry );

	// enter into new directory
	if( entry->getType() == DescriptorEntry::Directory )
		currentDir = (DescriptorEntryDirectory *)entry;

}

/********************************* SkipWhite ********************************
 *
 *  Description: Skip any white space 
 *			   
 *  Do not advance behind '\0'			   
 *---------------------------------------------------------------------------
 *  Input......: p			ptr to start in line
 *  Output.....: Return		ptr to next non-white space
 *  Globals....: -
 ****************************************************************************/
char *
DescFile::SkipWhite( char *p )
{
	while( *p ){
		if( *p!=' ' && *p!='\t' )
			break;
		p++;
	}
	return p;
}

/********************************* GetSymbol ********************************
 *
 *  Description: Copy the symbol name from the line buffer
 *			   	
 *	copying takes place until a invalid character is detected
 *  Valid characters are a-z, A-Z, 0-9, _, /,  \
 *---------------------------------------------------------------------------
 *  Input......: p			ptr in line
 *				 buf		buffer where symbol will be copied to
 *				 len		length of buf
 *  Output.....: Return		ptr in input line behind symbol
 *				 buf		symbol name
 *  Globals....: -
 ****************************************************************************/
char *
DescFile::GetSymbol( char *p, char *buf, long len )
{
	while( *p && len>1){
		if( isalnum(*p) || *p=='_' || *p=='\\' || *p=='/' ){
			*buf++ = *p++;
			len--;
		}
		else
			break;
	}
	*buf = '\0';
	return p;
}

/********************************* IsValidCSymbol ****************************
 *
 *  Description: Checks if the symbol is a valid C-syntax
 *			   
 *			   
 *---------------------------------------------------------------------------
 *  Input......: p			ptr to symbol name
 *  Output.....: Return		0=not a vaild symbol 1=it is
 *  Globals....: -
 ****************************************************************************/
bool 
DescFile::IsValidCSymbol( char *p )
{
	if( !( isalpha(*p) || *p=='_')) return 0;
	p++;

	while( *p ){
		if( isalnum(*p) || *p=='_' )
			p++;
		else
			return 0;
	}
	return 1;
}


