/***************************************************************************
 */
/*!       \file  modmdischamproptab.h
 *       \brief  chameleon mdis configuration tab
 *      \author	 Christine.Ruff@men.de
 *		  $Date: 2011/09/26 09:18:01 $
 *    $Revision: 2.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modmdischamproptab.cpp,v $
 * Revision 2.1  2011/09/26 09:18:01  dpfeuffer
 * R: base class for ModBbisCham (PCI) and ModBbisChamIsa required
 * M: ModBbisCham replaced with ModBbisChamBase
 *
 * Revision 2.0  2010/01/22 11:31:16  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.5  2009/08/14 14:35:14  CRuff
 * R: not clear which device identifier is shown in properties tab
 * M: Indicate which id is shown: Mod Id or Device Id
 *
 * Revision 1.4  2009/05/14 08:44:22  CRuff
 * R: Qt warning when opening the chameleon device properties dialog
 * M: ModMdisChamPropTab::ModMdisChamPropTab(): create grid layout without parent
 *
 * Revision 1.3  2009/05/11 16:32:26  CRuff
 * R: 1. Device identifications in groups done with index, not instance number
 * M: 1a) ModMdisChamPropTab::ModMdisChamPropTab(): Removed instance nr
 *        edit field of main group device;
 *        list index of group members instead of instance numbers (not editable)
 *    1b) ModMdisChamPropTab::updateProperties(): no more validity check for
 *        instance number
 *    1c) ModMdisChamPropTab::setProperties(): for group members, show the index
 *        instead of the instance number in the dialog
 *
 * Revision 1.2  2009/04/21 15:30:02  CRuff
 * R: Cosmetics
 * M: cleared file header
 *
 * Revision 1.1  2009/04/01 15:49:39  CRuff
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2009-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODMDISCHAMPROPTAB_CPP_
#define MODMDISCHAMPROPTAB_CPP_

#include "modmdischamproptab.h"
#include "modmdischam.h"
#include "modbbischam.h"
#include "wizexcept.h"
#include "wizdebug.h"
#include "vmsgbox.h"
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3VBoxLayout>
#include <QLabel>
#include <Q3ValueList>

//----------------------------------------------------------------------
//! create a mdis chameleon properties tab
/*!
  adds one tab, the "Chameleon" tab.

  \param parent parent widget
  \param _mainDlg pointer to main dialog
  \param name internal name
  \param f window flags
*/
ModMdisChamPropTab::ModMdisChamPropTab(
	QWidget *parent,
	DevPropertiesDlg *_mainDlg,
	const char *name, 
	Qt::WFlags f) : 
	DevPropertiesTab(parent,_mainDlg,name,f)
{
	wDebug(("enter ModMdisChamPropTab::ModMdisChamPropTab"));
	
	// get the device properties
	DeviceProperties* p = getMainDlg()->getDevice()->getProp();
	ModMdisChamProperties* _prop;
	WIZ_DYNAMIC_CAST( p, _prop, ModMdisChamProperties* );
	
	// create GUI layout
	vlayout = new Q3VBoxLayout(this, 8);
	layoutGrid = new Q3GridLayout(1, 2, 8);
	spacer = new QSpacerItem(0,10, QSizePolicy::Minimum, QSizePolicy::Ignored);

	// Hint: device name / device id
	QString idName = "Device Id";
	if (_prop->getType() == ModMdisChamProperties::ChamOriginal) {
		idName = "Mod Id";
	}
	label_heading = new QLabel(this);
	QString headingText = QString("Properties for device %1 (%3: '%2'):                            ")
						.arg(getMainDlg()->getDevice()->getInstName())
						.arg(_prop->deviceIdent.devId)
						.arg(idName);
	label_heading->setText(headingText);

	vlayout->addWidget(label_heading);
	vlayout->addLayout(layoutGrid);
	vlayout->addItem(spacer);
	
	bool isGroup = false;
	if ( ( _prop->devType == ModMdisChamProperties::ChamV2) && 
	     ( !_prop->grpMemList->empty()) )
		isGroup = true;

	// instance Number
	// if group, device is identified via group id! no instance nr necessary
	label_instanceNr = new QLabel(this);
	label_instanceNr->setText("Instance Number");
	layoutGrid->addWidget(label_instanceNr, 0, 0);
	lineEdit_instanceNr = new QLineEdit(this);
	lineEdit_instanceNr->setText("");
	layoutGrid->addWidget(lineEdit_instanceNr, 0, 1);
	if ( isGroup )
	{
		label_instanceNr->setHidden(true);
		lineEdit_instanceNr->setHidden(true);
	}


	// if there is a group defined for this instance, add the group number and
	// the group member instances
	list_grpMemLabels = NULL;
	list_grpMemLineEdits = NULL;
	label_grpID = NULL;
	lineEdit_grpID = NULL;
	if ( _prop->devType == ModMdisChamProperties::ChamV2 ) 
	{
		if ( isGroup )
		{
			// this device is main device of a group
			int numGroupMem = _prop->grpMemList->size();
			layoutGrid->expand(1 + numGroupMem, 2);
			
			// add the group nr
			label_grpID = new QLabel(this);
			label_grpID->setText("Group ID: ");
			layoutGrid->addWidget(label_grpID, 0, 0);
			lineEdit_grpID = new QLineEdit(this);
			lineEdit_grpID->setText("");
			layoutGrid->addWidget(lineEdit_grpID, 0, 1);
			
			// create the lists for group widgets
			list_grpMemLabels = new Q3ValueList<QLabel*>();
			list_grpMemLineEdits = new Q3ValueList<QLineEdit*>();
			
			// add the instance numbers of the group members
			for ( int i=0; i<_prop->grpMemList->size(); i++ )
			{
        		DeviceChamIdent grpMemIdent = *(_prop->grpMemList->at(i));
        		
        		QLabel* label_grpMem = new QLabel(this);
        		QString grpMemInstLabelText = QString("Index of member '%1' in group:").arg(grpMemIdent.devId);
        		label_grpMem->setText(grpMemInstLabelText);
        		list_grpMemLabels->push_back(label_grpMem);
        		layoutGrid->addWidget(label_grpMem, 1+i, 0);
        		
        		QLineEdit* lineEdit_grpMem = new QLineEdit(this);
        		lineEdit_grpMem->setText("");
        		lineEdit_grpMem->setEnabled(false);
        		layoutGrid->addWidget(lineEdit_grpMem, 1+i, 1);
        		list_grpMemLineEdits->push_back(lineEdit_grpMem);
        	}
		} // end if !grpMemList->empty()
	} // end if chamV2	
	
	wDebug(("leave ModMdisChamPropTab::ModMdisChamPropTab"));
}

//! show Warning dialog with given text
void 
ModMdisChamPropTab::showWarningMsg( QString &errMsg )
{
	VMsgBox::informationOk( "Device Properties",
							"Error while reading chameleon device properties",
							errMsg,
							getMainDlg() );
}

//! update the properties for the corresponding mdis device
/*!
  Read the given chameleon properties from the dialog tab and writes them
  to the device properties. 
  Thereby, the instance numbers and group ids are checked for uniqueness.
  If duplicate ids occur, the user is informed by a warning dialog. 
  Called when the apply button (or OK button) is pressed.
*/
void 
ModMdisChamPropTab::updateProperties()
{
	wDebug(("enter ModMdisChamPropTab::updateProperties()\n"));
	QString errMsg = "";
	
	// get the device properties
	DeviceProperties* p = getMainDlg()->getProperties();
	ModMdisChamProperties* _prop;
	WIZ_DYNAMIC_CAST( p, _prop, ModMdisChamProperties* );
	
	// reset the internal list of used instance numbers
	HwComponent* dev = getMainDlg()->getDevice()->getParent()->getParent();
	ModBbisChamBase* bbisDev;
	WIZ_DYNAMIC_CAST(dev, bbisDev, ModBbisChamBase* );
	bbisDev->clearInstanceTrack();
	ModMdisCham* myDevice;
	WIZ_DYNAMIC_CAST( getMainDlg()->getDevice(), myDevice, ModMdisCham* );
	bbisDev->setOtherInstances( myDevice );
	
	bool isGroup = false;
	if ( ( _prop->devType == ModMdisChamProperties::ChamV2) && 
	     ( !_prop->grpMemList->empty()) )
		isGroup = true;
		
	// save instance nr for this device
	
	// get contents of the instance nr field
	bool conversionSuccess = true;
	if ( !isGroup )
	{
		QString instNumText = lineEdit_instanceNr->text();
		instNumText = instNumText.stripWhiteSpace();
		int instNr = -1;
		if (instNumText.length() == 0 )
		{
			errMsg += QString("ERROR: Please set instance number for this device.\n\n");
		}
		else
		{
			int res = instNumText.toInt( &conversionSuccess );
			if ( !conversionSuccess )
			{	
				errMsg += QString("ERROR: instance number '%1' is not a valid integer. Please correct!\n\n").arg(instNumText.ascii());
			}
			else
			{
				instNr = res;
				wDebug(("set instance Nr '%s'\n", instNumText.ascii()));
			}
		}
		
		// check if this instance number can be set
		if ( bbisDev->addInstance( _prop->deviceIdent.devId, instNr ) )
		{
			// set the instance nr 
			_prop->deviceIdent.instanceNr = instNr;
		}
		else
		{  
			errMsg += QString("Could not add instance number '%1' for device '%2'\n").arg(instNr).arg(_prop->deviceIdent.devId);
			errMsg += "A device with this instance number already exists in this chameleon device\n\n";
			
			_prop->deviceIdent.instanceNr = instNr; // leave the value as is (to avoid removing correct values)
		}
	}
	else
	{
		// set the group settings if necessary

		wDebug(("start setting group id"));
		QString grpIDText = lineEdit_grpID->text();
		grpIDText = grpIDText.stripWhiteSpace();
		int res;
		if ( grpIDText.length() == 0 )
		{
			errMsg += QString("ERROR: Please set group ID for this device. \n\n");
			
			_prop->grpId = 0;
		}
		else
		{
			conversionSuccess = false;
			res = grpIDText.toInt( &conversionSuccess );
			if ( !conversionSuccess )
			{
				errMsg += QString("ERROR: group id '%1' is not a valid integer. Please correct!\n\n").arg(grpIDText.ascii());
				
				_prop->grpId = 0;
			}
			else
			{
				if ( bbisDev->addGroup( res ) ) {
					_prop->grpId = res;
					wDebug(("set group id '%s'\n", grpIDText.ascii()));
				} else {
					errMsg += QString("ERROR: a group with id '%1' already exists in chameleon device. Please select another one!\n\n").arg(grpIDText.ascii()); 
					_prop->grpId = 0;
				}
			}
		}
		
		// read the group member instance numbers
		// group members are now identified with index in group, not instance nr!
		// instance is not editible; no need to read it 
		/*WIZ_ASSERT( list_grpMemLineEdits != NULL );
		uint numGroupMem = list_grpMemLineEdits->size();
		for (uint i=0; i<numGroupMem; i++)
		{
			int grpMemInst = 0;
			QLineEdit* currGrpMem = *(list_grpMemLineEdits->at(i));
			
			WIZ_ASSERT( currGrpMem );
			WIZ_ASSERT( (_prop->grpMemList != NULL ) &&
						(_prop->grpMemList->size() > i) );
						
			QString grpMemInstText = currGrpMem->text();
			grpMemInstText = grpMemInstText.stripWhiteSpace();
			
			if ( grpMemInstText.length() == 0 )
			{
				errMsg += QString("ERROR: Please set the instance number for group member '%1'.\n\n")
							.arg((*(_prop->grpMemList->at(i))).devId);
				
				(*(_prop->grpMemList->at(i))).instanceNr = -1;
			}
			else
			{
				conversionSuccess = false;
				grpMemInst = grpMemInstText.toInt( &conversionSuccess );
				if ( !conversionSuccess )
				{						
					errMsg += QString("ERROR: instance Nr '%1' is not a valid integer. Please correct!\n\n").arg(grpMemInstText.ascii());
					
					(*(_prop->grpMemList->at(i))).instanceNr = -1;
				}
				else
				{
					if ( bbisDev->addInstance( (*(_prop->grpMemList->at(i))).devId, grpMemInst ) )
					{
						(*(_prop->grpMemList->at(i))).instanceNr = grpMemInst;
						wDebug(("set group mem id '%s'\n", grpMemInstText.ascii()));
					}
					else
					{
						errMsg += QString( "Could not add instance number '%1' for device '%2'\n")
											  	.arg(grpMemInst).arg((*(_prop->grpMemList->at(i))).devId);
						errMsg += "A device with this instance number already exists in this chameleon device\n\n";
						
						(*(_prop->grpMemList->at(i))).instanceNr = grpMemInst; // leave the value as is (to avoid removing correct values)
					}
				}
			}
		} // end for all group members
		*/
	} // end isGroup
	
	if ( errMsg != "" )
	{
		showWarningMsg( errMsg );
	}
	
	wDebug(("leave ModMdisChamPropTab::updateProperties()\n"));
}

//! update the property widgets with the device property values
/*!
  Read the chameleon properties from the chameleon mdis device and writes them
  to the dialog widgets. 
  
  Called when a property dialog for an mdis chameleon device is opened.
*/
void 
ModMdisChamPropTab::setProperties()
{
	wDebug(("enter ModMdisChamPropTab::setProperties()\n"));
	
	// get the device properties
	DeviceProperties* p = getMainDlg()->getProperties();
	ModMdisChamProperties* _prop;
	WIZ_DYNAMIC_CAST( p, _prop, ModMdisChamProperties* );
	
	bool isGroup = false;
	if ( ( _prop->devType == ModMdisChamProperties::ChamV2) && 
	     ( !_prop->grpMemList->empty()) )
		isGroup = true;

	// fill in the values to the widgets
	if ( !isGroup )
	{
		QString instNrText = "";
		int instNr = _prop->deviceIdent.instanceNr;
		if ( instNr >= 0 )
		{
			instNrText = QString("%1").arg(instNr);
		}
		lineEdit_instanceNr->setText(instNrText);
	}
	else
	{
		// set group id if necessary
		if ( _prop->grpId != 0 ) 
		{
			QString grpIDText = QString("%1").arg(_prop->grpId);
			lineEdit_grpID->setText(grpIDText);
		}
		else
		{
			lineEdit_grpID->setText("");
		}
		
		// set indices for group members
		uint numGroupMem = _prop->grpMemList->size();
		WIZ_ASSERT( list_grpMemLineEdits );
		for (uint i=0; i<numGroupMem; i++)
		{
			int grpMemInstNr = (*(_prop->grpMemList->at(i))).index;
			wDebug(("index for grp mem %d: %d", i, grpMemInstNr));
			WIZ_ASSERT( (uint)(list_grpMemLineEdits->size()) > i );
			QLineEdit* currLineEdit = *(list_grpMemLineEdits->at(i));
			if ( grpMemInstNr >= 0 )
				currLineEdit->setText(QString("%1").arg(grpMemInstNr));
			else
				currLineEdit->setText("");
		}
	} // end chamv2 device
	wDebug(("leave ModMdisChamPropTab::setProperties()\n"));
}

#endif /*MODMDISCHAMPROPTAB_CPP_*/


