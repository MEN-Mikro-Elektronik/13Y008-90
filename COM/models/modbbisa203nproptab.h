/***************************************************************************
 */
/*!       \file  modbbisa203nproptab.h
 *       \brief  A203N family VME configuration tab
 *      \author	 dieter.pfeuffer@men.de
 *		  $Date: 2010/07/12 10:07:29 $
 *    $Revision: 2.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbisa203nproptab.h,v $
 * Revision 2.1  2010/07/12 10:07:29  dpfeuffer
 * R: access violation in mdiswiz_lib.lib under Windows with MDIS_CLINST
 * M: QComboBox replaced with Q3ComboBox
 *
 * Revision 2.0  2010/01/22 11:28:48  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.1  2005/08/02 11:20:45  dpfeuffer
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODBBISA203NPROPTAB_H
#define MODBBISA203NPROPTAB_H

#include <QTimerEvent>
#include <Q3HBoxLayout>
#include <QLabel>

#include "propertiesdlg.h"
#include "modbbisa203n.h"
#include "hexspinbox.h"

class QSignalMapper;
class Q3HBoxLayout;
class InstNameWidget;
class QSpinBox;
class Q3ComboBox;
class QCheckBox;
class HexEditWidget;
class QRadioButton;
class DipSwitch;
class QLabel;


//! A203N family VME configuration tab
class ModBbisA203NPropTab : public DevPropertiesTab {
	Q_OBJECT
public:
	ModBbisA203NPropTab(int _nSlots, 
					   QWidget *parent,
					   DevPropertiesDlg *_mainDlg,
					   const char *name=0, 
					   Qt::WFlags f=0);
	~ModBbisA203NPropTab();

	virtual void updateProperties();
	virtual void setProperties();

protected slots:
	void slotChkBoxToggled( bool state );
	void slotChkBoxA32Toggled( bool state );
	void slotHexSpinChanged( int value );
	void slotVmeAddrTextChanged();
	void slotSpaceRbChanged( bool );
private:
	virtual void addrToSwitches(ulong adr);
	void timerEvent( QTimerEvent *e );

	int nSlots;
	int timerId;

#ifdef WIN_MDISWIZ_LIB
	QCheckBox *forceFoundChBox[4];
#endif
	Q3ComboBox *levelSpBox[4];
	QSpinBox *vectorSpBox[4];
	HexEditWidget *vmeAddWdg, *vmeA32AddWdg;
	QLabel *vmeA32AddWdgLab;
	QRadioButton *vmeA16Wdg, *vmeA24Wdg;
	QCheckBox *chkBoxA16, *chkBoxA32;
	HexSpinBox *hexSpin1Wdg, *hexSpin2Wdg, *hexSpin3Wdg;
};

#endif










