/***************************************************************************
 */
/*!       \file  vmsgbox.h
 *       \brief  Enhanced Qt message box
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:28:17 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: vmsgbox.h,v $
 * Revision 2.0  2010/01/22 11:28:17  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.1  2003/05/02 14:36:39  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef VMSGBOX_H
#define VMSGBOX_H

#include <qdialog.h>

//! Enhanced Qt message box
/*!

  This Qt widget brings up a dialog box with an icon, title, a brief
  text and a detailed text. The detailed text is put into a scrollable
  window (QTextEdit).

  Currently, the dialog has only an Ok button.

  The example shows how to create and execute a VMsgBox:
  \code
  		VMsgBox mb( currentCfg()->msgBoxCaption(),
					QString("Warnings while loading components"),
					errMsg,
					VMsgBox::Warning,
					this);
		mb.exec();
  \endcode

  Alternatively, the static functions informationOk(), warningOk(), 
  criticalOk() can be used.
*/

class VMsgBox : public QDialog {
public:
	//! The icon to disply in the message box
    enum Icon { NoIcon = 0, Information = 1, Warning = 2, Critical = 3 };

	VMsgBox(const QString & caption, 
			const QString & text, 
			const QString & detailsText, 
			Icon icon, 
			QWidget * parent = 0, 
			const char * name = 0, 
			bool modal = TRUE, 
			Qt::WFlags f = 0 );


	//! executes a modal information box with ok button
	static void informationOk( const QString & caption, const QString & text, 
							   const QString & detailsText, 
							   QWidget * parent = 0 ) {
		VMsgBox mb( caption, text, detailsText, Information, parent );
		mb.exec();
	}

	//! executes a modal warning box with ok button
	static void warningOk( const QString & caption, const QString & text, 
						   const QString & detailsText, 
						   QWidget * parent = 0 ) {
		VMsgBox mb( caption, text, detailsText, Warning, parent );
		mb.exec();
	}

	//! executes a modal critical box with ok button
	static void criticalOk( const QString & caption, const QString & text, 
							const QString & detailsText, 
							QWidget * parent = 0 ) {
		VMsgBox mb( caption, text, detailsText, Critical, parent );
		mb.exec();
	}

};


#endif

