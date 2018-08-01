/***************************************************************************
 */
/*!       \file  cfgmakefile.cpp 
 *       \brief  Makefile parser 
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/12/10 10:45:59 $
 *    $Revision: 2.2 $
 *
 *	 Used to parse Makefiles (convert it into MkFileTuples) and they other
 *	 way around (write MkFileTuples into Makefiles)
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: cfgmakefile.cpp,v $
 * Revision 2.2  2010/12/10 10:45:59  CRuff
 * R: Sporadic syntax errors during Makefile load
 * M: In LineToTuple(): initialize MkTuple before using it
 *
 * Revision 2.1  2010/05/19 14:45:00  CRuff
 * R: GetLine() crashes at end of device
 * M: add error handling in GetLine()
 *
 * Revision 2.0  2010/01/22 11:25:28  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.6  2009/04/27 17:25:01  CRuff
 * R: New project settings option UP / SMP for VxWorks
 * M: added handling for new Assignment type '+=' in methods write() and LineToTuple()
 *
 * Revision 1.5  2005/12/08 15:55:16  UFranke
 * added
 *  + Ifeq, Else
 *
 * Revision 1.4  2004/04/13 11:00:32  dpfeuffer
 * cosmetics
 *
 * Revision 1.3  2003/06/05 12:00:40  dschmidt
 * intermediate checkin
 *
 * Revision 1.2  2003/06/05 09:33:03  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:34:25  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/


#include <qregexp.h>
#include <q3textstream.h>

#include <Q3PtrList>

#include "wizdebug.h"
#include "cfgmakefile.h"

//! constructs a CfgMakefile with the filename \a fileName
CfgMakefile::CfgMakefile( const QString &_fileName )
{
	file.setName(_fileName);
}

//! read config makefile.
/*!
\throws CfgMakefile::CannotOpen if file cannot be opened.
\throws CfgMakefile::SyntaxError.

To write the Makefile use write() 
\return a list of MkFileTuple found in the makefile (without comments)
*/
Q3PtrList<MkFileTuple> *
CfgMakefile::read()
{
	Q3PtrList<MkFileTuple> *valList = new Q3PtrList<MkFileTuple>();

	valList->setAutoDelete(true);

	if ( file.open(QIODevice::ReadOnly) == false ) {    // file opened successfully
		throw CfgMakefile::CannotOpen(*this);
	}
	currentLine = 0;
	
	// scan entire file
	try {
		while( !file.atEnd() ){
			QString line;

			// read one line from file
			line = GetLine();
			//wDebug(("Line: <%s>", (const char *)line ));

			// ignore empty lines
			if( line == QString::null || line == "" )
				continue;

			// split line according to whitespace
			QStringList lst = QStringList::split( QRegExp("[ \\t]+"), line );
			//wDebug(("SPLITTED: %s", (const char *)lst.join("|")));

			// convert line to tuples (might throw 
			valList->append( LineToTuple( lst ));
		}
	}
	catch( CfgMakefile::Except &e ){
		e = e; // suppress compiler warning
		file.close();
		throw;		// pass exception to caller
	}
	file.close();

	return valList;
}

//! write config makefile
/*!
Writes all elements contained in \a valList to the configuration Makefile

throws CfgMakefile::CannotOpen if file cannot be opened.

handles assignment, include, comment tuples.
*/
void 
CfgMakefile::write( Q3PtrList<MkFileTuple> *valList )
{
	MkFileTuple *tup;

	if ( file.open(QIODevice::WriteOnly | QIODevice::Truncate) == false ) {
		throw CfgMakefile::CannotOpen(*this);
	}

	Q3TextStream outStream( &file );

	for( tup = valList->first(); tup; tup = valList->next() ){

		switch( tup->tag & MkFileTuple::TypeMask ){
		case MkFileTuple::None:
			break;

		case MkFileTuple::Assignment:
			if( tup->tag & MkFileTuple::Export )
				outStream << "export ";

			outStream << tup->key << " ";

			if( tup->tag & MkFileTuple::HardAssign )
				outStream << ":= ";
			else if (tup->tag & MkFileTuple::ExtendAssign )
				outStream << "+= ";
			else
				outStream << "= ";
			
			if( tup->val.count() > 1 )
				outStream << "\\\n\t" << tup->val.join(" \\\n\t");
			else
				outStream << tup->val.join(" ");

			outStream << "\n\n";
			break;

		case MkFileTuple::Include:
			outStream << "include " << tup->key << "\n\n";
			break;

		case MkFileTuple::Ifdef:
			outStream << "ifdef " << tup->key << "\n";
			break;

		case MkFileTuple::Ifndef:
			outStream << "ifndef " << tup->key << "\n";
			break;

		case MkFileTuple::Ifeq:
			outStream << "ifeq (" << tup->key << ")\n";
			break;

		case MkFileTuple::Else:
			outStream << "else\n";
			break;

		case MkFileTuple::Endif:
			outStream << "endif\n\n";
			break;

		case MkFileTuple::Comment:
			outStream << "# " << tup->val.join("\n# ");
			outStream << "\n\n";
			break;
		}
	}
	file.close();		
}



//
//! converts the splitted line \a lst into a MkFileTuple
/*!
 Knows the following line styles:

 - key = val1 val2 val3 ..
 - key := val1 val2 val3 ..
 - export key := val1 val2 val3 ..
 - include file

\throws CfgMakefile::SyntaxError
*/
MkFileTuple *
CfgMakefile::LineToTuple( QStringList &lst )
{
	QStringList::Iterator it;
	int idx=-1, i;
	MkFileTuple *tup = new MkFileTuple;
	tup->tag = MkFileTuple::None; // initialize

	for( it=lst.begin(), i=0; it != lst.end(); ++it, i++ ){
		if( *it == "=" ){
			tup->tag = MkFileTuple::Assignment;
			idx = i;
			break;
		}
		else if( *it == ":=" ){
			tup->tag = MkFileTuple::Assignment | MkFileTuple::HardAssign;
			idx = i;
			break;
		}
		else if ( *it == "+=" ) {
			tup->tag = MkFileTuple::Assignment | MkFileTuple::ExtendAssign;
			idx = i;
			break;
		}
	}

	// check for assignment beginning with export
	if( (tup->tag & MkFileTuple::TypeMask) == MkFileTuple::Assignment ){
		if( lst[0] == "export" && idx == 2 ){
			tup->tag |= MkFileTuple::Export;
			tup->key = lst[1];
		}
		else if( idx == 1 ){
			tup->key = lst[0];
		}
		else {
			delete tup;
			throw SyntaxError(*this, lst.join(" "));
		}
		// copy the values
		for( ++it ; it != lst.end(); ++it )
			tup->val += *it;
	}
	else if( lst[0] == "include" ){
		tup->tag = MkFileTuple::Include;
		tup->key = lst[1];
	}
	else if( lst[0] == "ifdef" ){
		tup->tag = MkFileTuple::Ifdef;
		tup->key = lst[1];
	}
	else if( lst[0] == "ifndef" ){
		tup->tag = MkFileTuple::Ifdef;
		tup->key = lst[1];
	}
	else if( lst[0] == "ifeq" ){
		tup->tag = MkFileTuple::Ifeq;
		tup->key = lst[1];
	}
	else if( lst[0] == "else" ){
		tup->tag = MkFileTuple::Ifdef;
	}
	else if( lst[0] == "endif" ){
		tup->tag = MkFileTuple::Ifdef;
	}
	else {
		delete tup;
		throw SyntaxError(*this, lst.join(" "));
	}
	//wDebug(("LineToTuple: key=%s tag=%x val=%s", 
	//	   (const char *)tup->key, tup->tag,(const char *) tup->val.join(" ")));


	return tup;
}


//! read in next line from file
/*! 
skip any comment
concatenate lines ending with '\'
strips any leading and trailing spaces
*/
QString 
CfgMakefile::GetLine()
{
	QString line;
	bool endOfLine=false;
	bool inComment=false;
	int c,c1;

	while( endOfLine==false && (c = file.getch()) != -1 ){
		
		switch(c){
		case 0xa:				/* end-of line chars */
		case 0xd:
			c1 = file.getch();		/* check for two-char end-of line */
			if( (c==0xa && c1 != 0xd) || 
			    (c==0xd && c1 != 0xa))

				/*
				 * Note: getch() returns -1 if the end of the I/O device has been reached
				 *       (no char was read therefore no ungetch() in this case!)
				 */
				if( c1 != -1 )
				file.ungetch(c1);	/* put char back to stream */

			currentLine++;

			if( line.right(1) != "\\" )	// check for continuation
				endOfLine=true;
			else 
				/* remove continuation symbol */
				line.truncate( line.length()-1 );
			break;
		case '#':				/* comment start */
			inComment=true;
			break;			

		default:
			if( !inComment ){
				line += c;		/* put char into line bufer */
			}
			break;
		}
	}

	return line.stripWhiteSpace();
}


