/***************************************************************************
 */
/*!       \file  modbbisa201proptab.h
 *       \brief  A201 family VME configuration tab
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/07/12 10:07:19 $
 *    $Revision: 2.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbisa201proptab.h,v $
 * Revision 2.1  2010/07/12 10:07:19  dpfeuffer
 * R: access violation in mdiswiz_lib.lib under Windows with MDIS_CLINST
 * M: QComboBox replaced with Q3ComboBox
 *
 * Revision 2.0  2010/01/22 11:28:38  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.2  2004/10/07 11:13:38  dpfeuffer
 * - modified for windows "Force Found" option
 *
 * Revision 1.1  2003/05/02 14:33:10  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:26:24  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:17:48  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODBBISA201PROPTAB_H
#define MODBBISA201PROPTAB_H

#include <QTimerEvent>
#include <Q3HBoxLayout>

#include "propertiesdlg.h"
#include "modbbisa201.h"

class QSignalMapper;
class Q3HBoxLayout;
class InstNameWidget;
class QSpinBox;
class Q3ComboBox;
class QCheckBox;
class HexEditWidget;
class QRadioButton;
class DipSwitch;


//! A201 family VME configuration tab
class ModBbisA201PropTab : public DevPropertiesTab {
	Q_OBJECT
public:
	ModBbisA201PropTab(int _nSlots, 
					   QWidget *parent,
					   DevPropertiesDlg *_mainDlg,
					   const char *name=0, 
					   Qt::WFlags f=0);
	~ModBbisA201PropTab();

	virtual void updateProperties();
	virtual void setProperties();

protected slots:
    void slotDipChanged( int grpId, int butId, bool state );
	void slotVmeAddrTextChanged();
	void slotSpaceRbChanged( bool );
private:
	virtual ulong switchesToAddr();
	virtual void addrToSwitches(ulong adr);
	virtual bool switchA16isOn();
	virtual void switchA16setOn(bool on);
	void timerEvent( QTimerEvent *e );
	

	int nSlots;
	int timerId;

#ifdef WIN_MDISWIZ_LIB
	QCheckBox *forceFoundChBox[4];
#endif
	Q3ComboBox *levelSpBox[4];
	QSpinBox *vectorSpBox[4];
	HexEditWidget *vmeAddWdg;
	QRadioButton *vmeA16Wdg, *vmeA24Wdg;
	DipSwitch *dip1Wdg, *dip2Wdg;
};

#endif










