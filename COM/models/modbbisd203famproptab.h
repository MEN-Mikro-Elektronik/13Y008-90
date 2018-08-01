/***************************************************************************
 */
/*!       \file  modbbisd203famproptab.h
 *       \brief  D203 family PCI M-Module carriers configuration tab
 *      \author	 Christian.Schuster@men.de
 *		  $Date: 2010/07/12 10:07:44 $
 *    $Revision: 2.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbisd203famproptab.h,v $
 * Revision 2.1  2010/07/12 10:07:44  dpfeuffer
 * R: access violation in mdiswiz_lib.lib under Windows with MDIS_CLINST
 * M: QComboBox replaced with Q3ComboBox
 *
 * Revision 2.0  2010/01/22 11:29:02  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.1  2006/11/30 11:28:18  cs
 * Initial Revision
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2006-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODBBISD203FAMPROPTAB_H
#define MODBBISD203FAMPROPTAB_H

#include <Q3HBoxLayout>

#include "propertiesdlg.h"
#include "modbbisd203fam.h"

class Q3HBoxLayout;
class QSpinBox;
class Q3ComboBox;
class DipSwitch;
class TristateSwitch;
class TristateButton;


//
// This dialog tab presents PXI trigger line settings
//
class ModBbisD203FamPropTab : public DevPropertiesTab {
	//Q_OBJECT
public:
	ModBbisD203FamPropTab(
		int nSlots,
		QWidget *parent,
		DevPropertiesDlg *_mainDlg,
		const char *name=0,
		Qt::WFlags f=0);

	virtual void updateProperties();
	virtual void setProperties();

protected slots:
	TristateSwitch *triPxiWdg, *triWdgTrigASlot[4], *triWdgTrigBSlot[4];
	TristateSwitch *triSw1Wdg;

	int _nSlots;
private:



};

#endif /* MODBBISD203FAMPROPTAB_H */










