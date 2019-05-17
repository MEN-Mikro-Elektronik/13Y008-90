/***************************************************************************
 */
/*!       \file  swmodulestab.cpp 
 *       \brief  Implementation of Sw modules tab
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:27:50 $
 *    $Revision: 2.0 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: swmodulestab.cpp,v $
 * Revision 2.0  2010/01/22 11:27:50  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.4  2003/06/05 12:01:29  dschmidt
 * intermediate checkin
 *
 * Revision 1.3  2003/06/05 09:34:02  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/06/03 10:16:52  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:36:31  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <q3tabdialog.h>
#include <qstringlist.h>
#include <qregexp.h>

#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <Q3PtrList>

#include "wizdebug.h"
#include "swmodulestab.h"
#include "configuration.h"
#include "vmsgbox.h"

// -----------------------------------------------------------------

//! creates an item in the dialog and associated \a mod with it
CfgSwModuleItem::CfgSwModuleItem( Q3ListViewItem *parent, const SwModule &mod ):
	Q3CheckListItem( parent, mod.modName(), Q3CheckListItem::CheckBox ), 
		swMod(mod)
{
	// make sure that the description contains no newlines. 
	// Truncate to 60 chars
	QStringList descLst = QStringList::split( '\n', mod.description() );
	QString desc = descLst.join( " " );

	if( desc.length() > 60 )
		desc = desc.left( 57 ) + "...";

	setText( 1, desc );
}

// -----------------------------------------------------------------

//! creates the Sw module configuration tab
/*!
  The list of module types to show is taken from what is present
  in G_swModTypes.
*/
CfgSwModuleTab::CfgSwModuleTab( QWidget *parent, const char *name, 
								Configuration *_cfg) :
	QWidget( parent, name, 0 )
{
	//QPushButton *but;

	cfg = _cfg;

	Q3VBoxLayout *vb = new Q3VBoxLayout(this,10);	

	QLabel *lab = new QLabel("Select the software modules you want to "
							 "include in your project", this );
	vb->addWidget(lab);

	Q3HBoxLayout* hb = new Q3HBoxLayout();
	vb->addLayout(hb);

	// label to display the currently selected item's properties
	propLabel = new QLabel(this,"proplabel");
	vb->addWidget( propLabel );

	listView = new Q3ListView( this );
	listView->setRootIsDecorated( false );
	listView->addColumn( "Module" );
	listView->addColumn( "Description" );
	listView->setSorting(-1);

	connect( listView, SIGNAL(currentChanged(Q3ListViewItem*)), 
			 this, SLOT( slotCurrentChanged(Q3ListViewItem*) ));

	hb->addWidget( listView );

	// add groups of SwModules
	SwModuleTypes::TypeMap::Iterator it;

	// do this backward to force "natural" ordering (we have set setSorting -1)
	for( it = G_swModTypes->types.end(); 
		 it != G_swModTypes->types.begin(); ){

		--it;

		// build displayed name. Make plural for names ending with "y"
		QString disName = it.data().typeName;
		disName = disName.replace( QRegExp("y$"), "ie") + "s";
			
		// create a directory "controller"
		Q3CheckListItem *controller = new Q3CheckListItem( 
			listView, 
			disName,
			Q3CheckListItem::Controller );

		// show all childrens
		controller->setOpen(true);

		// add items to controller
		addItems( controller, it.key() );
	}

}		


//
// creates a QCheckListItem for each swModule in lstSwModules of type
// modType in directory "controller"
void 
CfgSwModuleTab::addItems( Q3CheckListItem *controller, 
						  int modType )
{
	SwModule *mod;
	CfgSwModuleItem *item;
	Q3PtrList<SwModule> *lstSwModules = cfg->getLstSwModules();

	for( mod=lstSwModules->first(); mod; mod=lstSwModules->next()){

		if( mod->modType() == modType ){
			item = new CfgSwModuleItem( controller, *mod );
			item->setOn( mod->isEnabled() );
		}
	}
}

//!  Sets the state of the checkbox items according to state of sw module
void 
CfgSwModuleTab::enableItems()
{
	Q3ListViewItem *group, *_item;
	CfgSwModuleItem *item;
	SwModule *mod;

	for( group=listView->firstChild(); group; group=group->nextSibling() ){

		for( _item=group->firstChild(); _item; _item=_item->nextSibling() ){
			
			item = dynamic_cast<CfgSwModuleItem *>(_item);

			if( item ){
				// find the module in the configuration
				mod = cfg->getLstSwModules()->find( item->swMod.modType(),
													item->swMod.modName());
				WIZ_ASSERT( mod );
				item->setOn( mod->isEnabled() && !(mod->isInternal()) );
			}
		}
	}
				
}


//! called when Ok or Apply button pressed in main dialog
void
CfgSwModuleTab::slotApply()
{
	wDebug(("CfgSwModuleTab::slotApply"));
	CfgSwModuleItem *item;
	Q3ListViewItem *group, *_item;
	SwModule *mod;

	// update the swmodules in the configuration depending
	// on the state of the checkboxes
	for( group=listView->firstChild(); group; group=group->nextSibling() ){

		for( _item=group->firstChild(); _item; _item=_item->nextSibling() ){

			item = dynamic_cast<CfgSwModuleItem *>(_item);

			if( item ){
				// find the module in the configuration
				mod = cfg->getLstSwModules()->find( item->swMod.modType(),
													item->swMod.modName());
				WIZ_ASSERT( mod );
				mod->setEnabled( item->isOn() );
			}
		}
	}

	// check all sw modules of configuration if they can be compiled
	QString errMsg;

	if( cfg->validateSwModules( errMsg, true, true ) == false ){
		// update item state
		enableItems();
	}
}

//! called when user selects a different item in the list view
/*!
  updates text in bottom area to show description and makefile
*/
void 
CfgSwModuleTab::slotCurrentChanged( Q3ListViewItem *_item )
{
	CfgSwModuleItem *item = dynamic_cast<CfgSwModuleItem *>(_item);

	if( !item ) return;

	// show properties of swmodules in label
	propLabel->setText( item->swMod.modName() + 
						": " + item->swMod.description() +
						"\nMakefile: " + item->swMod.makefilePath());

}

