/***************************************************************************
 */
/*!       \file  vmsgbox.cpp 
 *       \brief  Implementation of VMsgBox
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:28:14 $
 *    $Revision: 2.0 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: vmsgbox.cpp,v $
 * Revision 2.0  2010/01/22 11:28:14  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.2  2003/06/05 09:34:06  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:36:38  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#include <qapplication.h>
#include <qstyle.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <q3textedit.h>

#include <QPixmap>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>

#include "wizdebug.h"
#include "vmsgbox.h"

//! Creates the verbose message box
/*!
  \param caption 		title text
  \param text	 		brief text
  \param detailsText	detailed text, newline separated
  \param icon			icon to display
  \param parent			parent widget
  \param name			internal name
  \param modal			true if modal dialog
  \param f				window flags
*/
VMsgBox::VMsgBox(
	const QString & caption, 
	const QString & text, 
	const QString & detailsText, 
	Icon icon, 
	QWidget * parent, 
	const char * name, 
	bool modal, 
	Qt::WFlags f ) :
	QDialog( parent, name, modal, f | Qt::WStyle_Customize | 
			 Qt::WStyle_DialogBorder | Qt::WStyle_Title | Qt::WStyle_SysMenu)
{
	Q3VBoxLayout *vb = new Q3VBoxLayout( this, 10 );

	Q3HBoxLayout *hb = new Q3HBoxLayout();
	vb->addLayout( hb );

	// Label for icon
	QLabel *iconLabel = new QLabel( this, "icon" );
    QPixmap pm;

    switch ( icon ) {
    case Information:

	pm = QApplication::style()->standardPixmap(QStyle::SP_MessageBoxInformation);
        break;
    case Warning:
	pm = QApplication::style()->standardPixmap(QStyle::SP_MessageBoxWarning);
        break;
    case Critical:
	pm = QApplication::style()->standardPixmap(QStyle::SP_MessageBoxCritical);
        break;
    default:
	break;
    }
	iconLabel->setPixmap( pm );
	hb->addWidget( iconLabel );

	// text label
	QLabel *label = new QLabel( text, this, "icon" );
	hb->addWidget( label );
	hb->addStretch(1);

	Q3TextEdit *textEdit = new Q3TextEdit( detailsText, QString::null, this );
	textEdit->setReadOnly( true );
	textEdit->setMinimumWidth(400);
	textEdit->setMinimumHeight(50);
	vb->addWidget( textEdit );

	Q3HBoxLayout *butHb = new Q3HBoxLayout();
	vb->addLayout( butHb );

	QPushButton *okBut = new QPushButton("OK", this );
	butHb->addStretch(1);
	butHb->addWidget( okBut );
	butHb->addStretch(1);
	connect( okBut, SIGNAL(clicked()), this, SLOT(accept()));

	setCaption(caption);

	okBut->setAutoDefault(true);
	okBut->setFocus();
}


