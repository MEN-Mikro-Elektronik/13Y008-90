/***************************************************************************
 */
/*!       \file  modmdischamproptab.h
 *       \brief  chameleon mdis properties tab
 *      \author	 Christine.Ruff@men.de
 *		  $Date: 2010/01/22 11:32:34 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modmdischamproptab.h,v $
 * Revision 2.0  2010/01/22 11:32:34  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.2  2009/04/21 15:30:39  CRuff
 * R: cosmetics
 * M: added revision history macro in file header
 *
 * Revision 1.1  2009/04/01 14:19:58  CRuff
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODMDISCHAMPROPTAB_H_
#define MODMDISCHAMPROPTAB_H_

#include "propertiesdlg.h"
#include <qlabel.h>
#include <qlineedit.h>
#include <qstring.h>
#include <q3valuelist.h>
#include <qlayout.h>

#include <Q3VBoxLayout>
#include <Q3GridLayout>

#include "hwcomponent.h"
#include "setting.h"

//! A Qt widget for MDIS Chameleon devices
/*!
  This widget allows to set the chameleon properties for a chameleon mdis device, such as
  the instance numbers and group id of its member(s). It is used
  by devices inherited from ModMdisGeneric.

  When the instance numbers are set, they are checked for uniqueness.
*/
class ModMdisChamPropTab : public DevPropertiesTab {
	Q_OBJECT
public:
	//! Constructor
	ModMdisChamPropTab(
		QWidget *parent,
		DevPropertiesDlg *_mainDlg,
		const char *name=0,
		Qt::WFlags f=0);

	//! Destructor
	virtual ~ModMdisChamPropTab()
	{
		/*if (label_heading)
			delete label_heading;*/
		if (label_instanceNr)
			delete label_instanceNr;
		if (lineEdit_instanceNr)
			delete lineEdit_instanceNr;
		if (label_grpID)
			delete label_grpID;
		if (lineEdit_grpID)
			delete lineEdit_grpID;
		/*if (layoutGrid)
			delete layoutGrid;
		if (spacer)
			delete spacer;*/

		if (list_grpMemLabels)
		{
			for (int y=0; y<list_grpMemLabels->size(); y++)
			{
				delete *(list_grpMemLabels->at(y));
			}
			list_grpMemLabels->clear();
			delete list_grpMemLabels;
		}

		if (list_grpMemLineEdits)
		{
			for (int z=0; z<list_grpMemLineEdits->size(); z++)
			{
				delete *(list_grpMemLineEdits->at(z));
			}
			list_grpMemLineEdits->clear();
			delete list_grpMemLineEdits;
		}
	}

	virtual void updateProperties();
	virtual void setProperties();

protected slots:
private:

	void showWarningMsg( QString &errMsg );

	QLabel*    label_heading;

	QLabel*    label_instanceNr;
	QLineEdit* lineEdit_instanceNr;

	QLabel*    label_grpID;
	QLineEdit* lineEdit_grpID;

	Q3ValueList<QLabel*>* list_grpMemLabels;
	Q3ValueList<QLineEdit*>* list_grpMemLineEdits;

	QSpacerItem* spacer;
	Q3GridLayout* layoutGrid;
	Q3VBoxLayout* vlayout;

};




#endif /*MODMDISCHAMPROPTAB_H_*/
