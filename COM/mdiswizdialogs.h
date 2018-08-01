/***************************************************************************
 */
/*!       \file  mdiswizdialogs.h
 *       \brief  Common mdiswiz dialogs
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/10/26 09:08:36 $
 *    $Revision: 2.2 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: mdiswizdialogs.h,v $
 * Revision 2.2  2010/10/26 09:08:36  CRuff
 * R: 1. application crash at add component action if no interfaces available
 * M: 1. added member function BeginnersAddDialog::numParentComponents that
 *        returns the number of interface parents in the system
 *
 * Revision 2.1  2010/07/12 10:06:31  dpfeuffer
 * R: access violation in mdiswiz_lib.lib under Windows with MDIS_CLINST
 * M: QComboBox replaced with Q3ComboBox
 *
 * Revision 2.0  2010/01/22 11:26:38  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.3  2005/07/15 15:47:07  ub
 * column width in CPU select dialog made variable
 *
 * Revision 1.2  2004/06/08 17:25:34  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.1  2003/05/02 14:36:07  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MDISWIZDIALOGS_H
#define MDISWIZDIALOGS_H

#include <qdialog.h>
#include <qlayout.h>
#include <qlabel.h>
#include <q3combobox.h>
#include <q3vbox.h>
#include <qpushbutton.h>
#include <q3listview.h>

#include <Q3HBoxLayout>
#include <Q3VBoxLayout>

class MdisWizListView;
class ComponentViewItem;
class HwComponent;

//! add component widget
/*!
  It is used by AddComponentDialog but can be also used as a normal
  widget (e.g. in the wizard)
*/
class AddComponentWidget : public QWidget {
	Q_OBJECT
public:
	//! creates the add component dialog
	AddComponentWidget(QWidget *parent,
					   const char *name,
					   const QString &text) : QWidget( parent, name )
	{
        Q3VBoxLayout *vb = new Q3VBoxLayout(this,10);

		if( !text.isEmpty() )
			vb->addWidget( new QLabel( text, this ));

		listBox = new Q3ListView( this, "listView" );
		listBox->setMinimumHeight( 200 );
		listBox->addColumn( "Model" );
		listBox->addColumn( "Description" );
		listBox->setColumnWidthMode( 0, Q3ListView::Maximum );
		listBox->setAllColumnsShowFocus( true );

		vb->addWidget( listBox );
	}

	//! add an entry to the list of selectable components
	void addItem( const QString &compStr, const QString &description )
	{
		(void) new Q3ListViewItem( listBox, compStr, description );
	}

	//! clear list of selectable components
	void clearItems()
	{
		listBox->clear();
	}

	//! get the selected item's text[0] or 0 if none selected
	QString result()
	{
		if( listBox->currentItem() )
			return listBox->currentItem()->text(0);
		return 0;
	}
private:
	Q3ListView *listBox;
};

//! add component dialog
class AddComponentDialog : public QDialog {
	Q_OBJECT
public:
	//! creates the add component dialog
	AddComponentDialog(QWidget *parent,
					   const char *name,
					   const QString &text,
					   bool modal=TRUE,
					   Qt::WFlags f=0) : QDialog(parent,name,modal,f)
    {

        Q3VBoxLayout* vb = new Q3VBoxLayout(this,10);
		QPushButton *but;

		wdg = new AddComponentWidget( this, 0, text );
		vb->addWidget( wdg );

		Q3HBoxLayout *hb = new Q3HBoxLayout();
		vb->addLayout( hb );
		hb->addStretch(1);

		but = new QPushButton("OK", this);
		hb->addWidget( but );
		connect( but, SIGNAL(clicked()), this, SLOT(accept()) );

		but = new QPushButton("Cancel", this);
		hb->addWidget( but );
		connect( but, SIGNAL(clicked()), this, SLOT(reject()) );
    }


	//! add an entry to the list of selectable components
	void addItem( const QString &compStr, const QString &description )
	{
		wdg->addItem( compStr, description );
	}

	//! get the selected item's text[0] or 0 if none selected
	QString result()
	{
		return wdg->result();
	}

private:
	AddComponentWidget *wdg;

};

//! beginners add component dialog
/*!
 invoked from menu or toolbar
*/

class BeginnersAddDialog : public QDialog {
	Q_OBJECT
public:
	//! creates the add component dialog
	BeginnersAddDialog(
		MdisWizListView *_wizView,
		ComponentViewItem *activeItem,
		QWidget *parent,
		const char *name=0,
		bool modal=TRUE,
		Qt::WFlags f=0);

	//! get the parent device (the device to add the new comp. to)
	ComponentViewItem *getParentCvi() { return parentCvi; }
	HwComponent *getParentHwComp() { return parentHwComp; }

	//! get the selected device name to add or 0 if none selected
	QString result()
	{
		return addCompWdg->result();
	}
	
	//! return the number of available parent components
	int numParentComponents();

protected slots:
	void slotParentCompChanged( int idx );
	void slotParentIfChanged( int idx );

protected:
	QPushButton *okBut;
	Q3ComboBox *parentCompWdg;
	Q3ComboBox *parentIfWdg;
	AddComponentWidget *addCompWdg;
	MdisWizListView *wizView;

	ComponentViewItem *parentCvi;
	HwComponent *parentHwComp;

private:
	void getAllParentComps();
};

#endif

