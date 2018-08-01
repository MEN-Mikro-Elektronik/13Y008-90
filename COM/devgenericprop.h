/***************************************************************************
 */
/*!       \file  devgenericprop.h
 *       \brief  Descriptor tree editor
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/07/12 10:06:18 $
 *    $Revision: 2.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: devgenericprop.h,v $
 * Revision 2.1  2010/07/12 10:06:18  dpfeuffer
 * R: access violation in mdiswiz_lib.lib under Windows with MDIS_CLINST
 * M: QComboBox replaced with Q3ComboBox
 *
 * Revision 2.0  2010/01/22 11:25:51  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.1  2003/05/02 14:34:34  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#ifndef DEVGENERICPROP_H
#define DEVGENERICPROP_H

#include <q3listview.h>

#include <Q3VBoxLayout>

#include "propertiesdlg.h"
#include "models/modmdisgeneric.h"

class Q3ListView;
class DevGenericPropTab;
class DescriptorEntry;
class DescriptorEntryUint32;
class DescriptorEntryDirectory;
class Q3VBoxLayout;
class QColorGroup;
class QPainter;
class QRadioButton;
class QLineEdit;
class Q3ComboBox;

//! class for entries in list view 
/*!
  Each object of this class has an associated DescriporEntry
*/
class DescViewItem : public Q3ListViewItem {
public:
	DescViewItem( DevGenericPropTab *_tab, Q3ListView *parent, 
				  DescriptorEntry *_descEntry );
	DescViewItem( DevGenericPropTab *_tab, DescViewItem *parent, 
				  DescriptorEntry *_descEntry );
    void update();
	void paintCell( QPainter *p, const QColorGroup &cg,
					int column, int width, int alignment );
	//! get associated DescriptorEntry
	DescriptorEntry *getDescEntry() { return descEntry; }
	//! set associated DescriptorEntry
	void setDescEntry( DescriptorEntry *e ) { descEntry = e; }
private:
	void init(DevGenericPropTab *_tab, DescriptorEntry *_descEntry );
	DescriptorEntry *descEntry;	
	DevGenericPropTab *tab;
};

//! A Qt widget for MDIS Descriptors
/*!
  This widget presents the descriptor entries in a QListView box. It is used
  by devices inherited from ModMdisGeneric.

  The user can double click on the entries to modify the descriptor values.
*/
class DevGenericPropTab : public DevPropertiesTab {
	Q_OBJECT
public:
	DevGenericPropTab( QWidget *parent,
					   DevPropertiesDlg *_mainDlg,
					   const char *name=0, 
					   Qt::WFlags f=0);

	virtual void updateProperties();
	virtual void setProperties();

protected slots:
    void slotDoubleClicked( Q3ListViewItem *_it );

private:
    void addDescDir( DescViewItem *parent, 
					 DescriptorEntryDirectory *dir,
					 bool inRootDir );
    Q3ListView *listView;
	Q3VBoxLayout *vb;
	
};

//! dialog to modify key values
/*!
  This dialog is created when an entry in DevGenericPropTab is double clicked
*/
class DescValueDlg : public QDialog {
	Q_OBJECT

public:
	DescValueDlg( DescriptorEntry *_descEntry, QWidget *parent );
   
protected slots:
    void slotOk();
	void slotCancel();
	void slotActiveToggled(bool);
	void slotUpdateValueWdg();

signals:

private:
	DescriptorEntry *descEntry;	
	DescriptorEntryUint32 *descEntry32;	

	QRadioButton *activeWdg, *deactiveWdg;
	QLineEdit *valueWdg;
	Q3ComboBox *valueCbox;
};
	
#endif



