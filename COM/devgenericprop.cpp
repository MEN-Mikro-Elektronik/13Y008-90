/***************************************************************************
 */
/*!       \file  devgenericprop.cpp 
 *       \brief  Implementation of descriptor tree editor
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/07/12 10:06:11 $
 *    $Revision: 2.1 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: devgenericprop.cpp,v $
 * Revision 2.1  2010/07/12 10:06:11  dpfeuffer
 * R: access violation in mdiswiz_lib.lib under Windows with MDIS_CLINST
 * M: QComboBox replaced with Q3ComboBox
 *
 * Revision 2.0  2010/01/22 11:25:48  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.5  2008/10/27 12:29:05  channoyer
 * R: Program name should be "MDIS Wizard" when displayed to the user
 * M: Call setCaption with G_progName constant
 *
 * Revision 1.4  2004/07/21 14:59:40  ufranke
 * changed
 *  - currentCfg() by MAIN_GetCurrentCfg()
 *
 * Revision 1.3  2003/06/05 12:00:48  dschmidt
 * intermediate checkin
 *
 * Revision 1.2  2003/06/05 09:33:11  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:34:33  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <q3header.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qlineedit.h>
#include <q3combobox.h>
#include <qpushbutton.h>
#include <qinputdialog.h>
#include <q3groupbox.h>
#include <qmessagebox.h>
#include <q3buttongroup.h>
#include <qradiobutton.h>

#include <Q3HBoxLayout>
#include <Q3Frame>
#include <Q3VBoxLayout>

#include "wizdebug.h"
#include "devgenericprop.h"
#include "descriptor.h"
#include "configuration.h"

//---------------------------------------------------------------------
//! construct DescViewItem that will be in the root directory
DescViewItem::DescViewItem( DevGenericPropTab *_tab, Q3ListView *parent, 
							DescriptorEntry *_descEntry ) :
	Q3ListViewItem( parent )
{
	init( _tab, _descEntry );
}

//! construct DescViewItem that will be inserted under \a parent
DescViewItem::DescViewItem( DevGenericPropTab *_tab, DescViewItem *parent, 
							DescriptorEntry *_descEntry ) :
	Q3ListViewItem( parent )
{
	init( _tab, _descEntry );
}

void 
DescViewItem::init(DevGenericPropTab *_tab, DescriptorEntry *_descEntry )
{
	descEntry = _descEntry;
	tab = _tab;	
	// show all childrens
	setOpen(true);
	update();
}

//! update key name and it's value in case descriptor entry changed
void 
DescViewItem::update()
{
	setText(0, descEntry->getKey());

	QString text1 = descEntry->valueToString();

	if( descEntry->getType() == DescriptorEntry::Uint32 ){
		DescriptorEntryUint32 *e32 = (DescriptorEntryUint32 *)descEntry;
		DescriptorEntryUint32::ChoisesMap::Iterator it;

		if( e32->isMultiChoise() ){
			it = e32->choises.find( e32->getValue() );
			text1 += " = " + it.data();
		}
	}
	setText(1, text1);
}

//! overwrite QListViewItem's paintCell to draw text in different colors
void 
DescViewItem::paintCell( QPainter *p, const QColorGroup &cg,
						 int column, int width, int alignment )
{
    QColorGroup _cg( cg );
    QColor c = _cg.text();

    if ( ! descEntry->isActive() )
        _cg.setColor( QColorGroup::Text, QColor("darkgrey") );

    Q3ListViewItem::paintCell( p, _cg, column, width, alignment );

    _cg.setColor( QColorGroup::Text, c );
}

//---------------------------------------------------------------------
//! constructs the descriptor editor tab
DevGenericPropTab::DevGenericPropTab( 
	QWidget *parent,
	DevPropertiesDlg *_mainDlg,
	const char *name, 
	Qt::WFlags f) :
	DevPropertiesTab( parent, _mainDlg, name, f )
{
	vb = new Q3VBoxLayout( this );
	listView = 0;
}



void
DevGenericPropTab::updateProperties()
{
}

void
DevGenericPropTab::addDescDir( DescViewItem *parent, 
							   DescriptorEntryDirectory *dir,
							   bool inRootDir )
{
	DescViewItem *it;

	// add children in reverse order to show it in natural order in QListView
	for( DescriptorEntry *entry = dir->getChildren().last();
		 entry;
		 entry = dir->getChildren().prev()){

		wDebug(("DevGenericPropTab::addDescDir: %s hidden=%d", 
			   (const char *)entry->getKey(), entry->isHidden()));

		if( !entry->isHidden() ){
			if( inRootDir )
				it = new DescViewItem( this, listView, entry );
			else
				it = new DescViewItem( this, parent, entry );
			
			
			if( entry->getType() == DescriptorEntry::Directory )
				addDescDir( it, (DescriptorEntryDirectory *)entry, false );
		}
	}					
}

// recreates the listview and all items
void
DevGenericPropTab::setProperties()
{
	wDebug(("DevGenericPropTab::setProperties"));

	ModDescriptorProperties *prop;
	WIZ_DYNAMIC_CAST( getMainDlg()->getProperties(), prop,
					  ModDescriptorProperties * );

	if( listView == 0 ){
		listView = new Q3ListView( this );
		listView->setSelectionMode( Q3ListView::Single );
		listView->header()->setClickEnabled( FALSE );
		listView->setRootIsDecorated( TRUE ); // level 0 entries have lines
		listView->setSorting(-1); // don't sort list

		connect(listView, SIGNAL( doubleClicked( Q3ListViewItem *)), 
				this, SLOT( slotDoubleClicked( Q3ListViewItem *) ));

		listView->addColumn( "Key" );
		listView->addColumn( "Value" );	

		vb->addWidget( listView );
	}
	else {
		// clear listview
		listView->clear();
	}

	wDebug(("DevGenericPropTab::createItems"));
	// create items
	addDescDir( 0, &prop->desc, true );
}

//!called when descriptor item double clicked
/*!
  bring up dialog to modify descriptor value
*/
void 
DevGenericPropTab::slotDoubleClicked( Q3ListViewItem *_it )
{
	DescViewItem *it = (DescViewItem *)_it;
	DescriptorEntry *entry, *orgEntry;
	DescriptorEntryDirectory *Dir;

	// no dialog for directories
	if( it->getDescEntry()->getType() == DescriptorEntry::Directory )
		return;

	// make a copy of the descriptor entry. Do modifications on this object
	orgEntry = it->getDescEntry();

	//wDebug(("DevGenericPropTab::slotDoubleClicked orgEntry=%p value=%s",
	//	   orgEntry, (const char *)orgEntry->valueToString()));

	Dir = orgEntry->getParent();
	entry = orgEntry->clone();
	
	DescValueDlg dlg( entry, this );

	if( dlg.exec() == QDialog::Accepted ){
		//wDebug(("dlg accepted NEW VALUE=%s", 
		//	   (const char *)entry->valueToString()));

		// replace the original entry by the new one
		it->setDescEntry( entry );
		it->update();

		Dir->removeChild( orgEntry );
		Dir->addChild( entry );

	}
	else {
		wDebug(("dlg rejected"));
		// discard changes
		delete entry;
	}
} 

//---------------------------------------------------------------------

//! create dialog to modify descriptor value
DescValueDlg::DescValueDlg( DescriptorEntry *_descEntry, QWidget *parent ) :
	QDialog( parent, 0, true )
{
	extern const QString	G_progName;     //!< main application name
	QLabel *lab;
	QPushButton *but;
	Q3VBoxLayout *vb = new Q3VBoxLayout( this, 10 );

	valueCbox = 0;
	valueWdg = 0;
	descEntry32 = 0;

	wDebug(("DescValueDlg::DescValueDlg %p %s", _descEntry, 
		   (const char *)_descEntry->getKey()));
	descEntry = _descEntry;

	setCaption(G_progName);
	vb->addSpacing(15);

	// Label Text
	lab = new QLabel( QString("Modify value of descriptor key \"%1\":").arg(
		descEntry->path().section("/",1)), this );
	vb->addWidget( lab );
	vb->addSpacing(10);

	// Radio Buttons to select active state
	Q3ButtonGroup *bg1 = new Q3ButtonGroup( 1, Qt::Horizontal, 
										  ""/*"Avtivate Descriptor"*/, this);
	bg1->setFrameStyle( Q3Frame::NoFrame );
	vb->addWidget( bg1 );
    activeWdg = new QRadioButton( "Activate key, use value entered below", 
								  bg1 );
    deactiveWdg = new QRadioButton( 
		"Deactivate key, use driver internal defaults", bg1 );

	connect( activeWdg, SIGNAL( toggled(bool)), this, 
			 SLOT(slotActiveToggled(bool)));

	// Value text widget
	Q3HBoxLayout *hb1 = new Q3HBoxLayout(vb);

	lab = new QLabel( QString("%1 value:").arg(descEntry->typeStr()), this );
	hb1->addWidget( lab );

	if( descEntry->getType() == DescriptorEntry::Uint32 )
		descEntry32 = (DescriptorEntryUint32 *)descEntry;

	// check if this is a multiple choise U_INT32 entry
	if( descEntry32 != 0 && descEntry32->isMultiChoise() ){
		valueCbox = new Q3ComboBox( this );
 
         DescriptorEntryUint32::ChoisesMap::Iterator it;

		 for ( it = descEntry32->choises.begin(); 
			   it != descEntry32->choises.end(); ++it ) {

			 QString cbText = descEntry32->anyValueToString( it.key() );
			 cbText += " = " + it.data();

			 valueCbox->insertItem( cbText );
		 }
		 hb1->addWidget( valueCbox );
	}
	else {
		// not a multiple choise U_INT32 entry
		valueWdg = new QLineEdit( this );	
		hb1->addWidget( valueWdg );
	}


	vb->addSpacing(15);

	// help text
	lab = new QLabel( this );
	lab->setAlignment( lab->alignment() );
	lab->setWordWrap( true );
	vb->addWidget( lab );

	lab->setText( descEntry->getHelpText().isEmpty() ? 
				  QString("[No help available]") :
				  descEntry->getHelpText() );

	vb->addSpacing(15);

	// add Buttons
	Q3HBoxLayout *hb2 = new Q3HBoxLayout(vb);
	
	hb2->addStretch(1);
	but = new QPushButton(tr("OK"), this);
	hb2->addWidget( but );
	connect( but, SIGNAL( clicked()), this, SLOT(slotOk()));

	hb2->addSpacing(10);
	but = new QPushButton(tr("Cancel"), this);
	hb2->addWidget( but );
	connect( but, SIGNAL( clicked()), this, SLOT(slotCancel()));
	
	// performing this some lines above results in a coredump...
	if( descEntry->isActive() )
		activeWdg->setChecked( true );
	else
		deactiveWdg->setChecked( true );

	slotUpdateValueWdg();
}

//! update value widget's text and state
void DescValueDlg::slotUpdateValueWdg()
{
	if( valueWdg ){
		// edit widget
		valueWdg->setText( descEntry->valueToString( !descEntry->isActive() ));

		// draw value widget in grey if not enabled
		valueWdg->setEnabled( descEntry->isActive() );

		if( descEntry->isActive() )
			valueWdg->setFocus();
	}
	else {
		// combo box
		WIZ_ASSERT( descEntry32 );
		WIZ_ASSERT( valueCbox );

		ulong val = descEntry->isActive() ? 
			descEntry32->getValue() : descEntry32->getDefaultValue();

		// find text for current value
		int idx;
		DescriptorEntryUint32::ChoisesMap::Iterator it = 
			descEntry32->choises.begin();

		for( idx=0; it != descEntry32->choises.end(); ++it, ++idx ) {
			if( it.key() == val )
				break;
		}
		valueCbox->setCurrentItem( idx );

		// draw value widget in grey if not enabled
		valueCbox->setEnabled( descEntry->isActive() );

		if( descEntry->isActive() )
			valueCbox->setFocus();
	}
}

//! called when OK button pressed
void DescValueDlg::slotOk()
{
	// attempt to set new value
	if( valueWdg ){
		// if using TextEdit
		if( descEntry->setValue( valueWdg->text()) == true )
			emit done( Accepted );
		else {
			QString errMsg = QString("Illegal value: %1").
				arg(valueWdg->text());

			QMessageBox mb( MAIN_GetCurrentCfg()->msgBoxCaption(), errMsg,
							QMessageBox::Critical,
							QMessageBox::Ok | QMessageBox::Default,
							Qt::NoButton,	
							Qt::NoButton,
							this );
			mb.exec();			
		}
	}
	else {
		// if using ComboxBox
		WIZ_ASSERT( descEntry32 );
		WIZ_ASSERT( valueCbox );

		// find the value corresponding to the cbox current index
		int idx = valueCbox->currentItem();

		DescriptorEntryUint32::ChoisesMap::Iterator it = 
			descEntry32->choises.begin();

		while( idx-- )
			it++;

		descEntry32->setValue( it.key() );
		emit done( Accepted );
	}
}

//! called when Cancel button pressed
void DescValueDlg::slotCancel()
{
	emit done( Rejected );
}

//! called when ACTIVE radiobuttons changed
void DescValueDlg::slotActiveToggled(bool state)
{
	descEntry->setActive( state );
	slotUpdateValueWdg();
}


