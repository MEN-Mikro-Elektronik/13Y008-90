/***************************************************************************
 */
/*!       \file  swmodulestab.h
 *       \brief  Dialog tab to select Sw modules to build
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:27:52 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: swmodulestab.h,v $
 * Revision 2.0  2010/01/22 11:27:52  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.2  2003/06/03 10:16:54  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:36:32  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef SWMODULESTAB_H
#define SWMODULESTAB_H

#include <q3listview.h>
#include <qwidget.h>
#include <qlist.h>

#include <QLabel>

#include "swmodule.h"

class Q3ListView;
class Q3TabDialog;
class QLabel;
class Configuration;

// -----------------------------------------------------------------
//! class for items in SwModule configuration dialog

class CfgSwModuleItem : public Q3CheckListItem {
public:
	CfgSwModuleItem( Q3ListViewItem *parent, const SwModule &mod );
					
	SwModule swMod;				//!< associated SwModule
};
	
// -----------------------------------------------------------------
//! Dialog tab to select Sw modules to build
/*!
  This tab can be included (depending on the target OS) in the configuration
  settings dialog.

  It can be used to let the user select which modules (drivers, examples etc.)
  to build.

  See OsLinux:settingsDlg() for an example.
*/
class CfgSwModuleTab : public QWidget {
	Q_OBJECT

public:
	CfgSwModuleTab( QWidget *parent, const char *name,
					Configuration *cfg);

public slots:
    void slotApply(); 
protected slots:
	void slotCurrentChanged( Q3ListViewItem *item );

private:
	void addItems( Q3CheckListItem *controller, int modType);
	void enableItems();
	Configuration *cfg;
	Q3ListView *listView;
	QLabel *propLabel;
};

#endif

