/***************************************************************************
 */
/*!       \file  propertiesdlg.h
 *       \brief  Device properties dialog
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/07/12 10:07:03 $
 *    $Revision: 2.2 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: propertiesdlg.h,v $
 * Revision 2.2  2010/07/12 10:07:03  dpfeuffer
 * R: access violation in mdiswiz_lib.lib under Windows with MDIS_CLINST
 * M: QComboBox replaced with Q3ComboBox
 *
 * Revision 2.1  2010/05/19 16:38:05  CRuff
 * R: 1. Cpu devices have onboad smb controllers, chameleon BBISes have z001
 *       smb controllers
 *    2. SmbPciBbis has to be added to device tree if activate smb checkbox is
 *       checked
 * M: 2. added methods hideOnboardSmbWidgets() and hideChamSmbWidgets()
 *    3. added signals activateOnboardSmb and activateChamSmb
 *
 * Revision 2.0  2010/01/22 11:27:33  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.3  2009/12/07 08:57:46  CRuff
 * R: add SMBus support
 * M: add new tab DevSmbTab to handle SMBus settings
 *
 * Revision 1.2  2003/06/03 10:16:44  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:36:21  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef PROPERTIESDLG_H
#define PROPERTIESDLG_H

#include <qdialog.h>
#include <q3tabdialog.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qlabel.h>

class DeviceProperties;
class InstNameWidget;
class Device;
class Q3ComboBox;
class MdiswizView;

//----------------------------------------------------------------------
//! Device Properties tab'ed dialog
/*!
  This dialog is used to show and/or modify a device's properties. It is 
  typically created by Device::createPropDlg() (or by subclasses of Device).

  This is a tab'ed dialog. It has initally only the "Generic" tab, containing
  device, model and driver name.

  The specific devices can add further tabs using addTab(). 
  
  Note that setButtons() has to be called after creating the tabs:
  \code
	dp = new DevPropertiesDlg( parent, this );
	dp->addTab(...);
	dp->setButtons();
  \endcode
*/  
class DevPropertiesDlg : public Q3TabDialog 
{
	Q_OBJECT
public:
	enum { Ok=1, Cancel=0 };	//!< dialog exec() return values
	DevPropertiesDlg( QWidget *parent, Device *dev, bool generalTab=TRUE );

	//! destructs DevPropertiesDlg
	virtual ~DevPropertiesDlg() {}	
	void setButtons();
	virtual void setProperties( DeviceProperties *_prop );

	//! get pointer to device property object
    DeviceProperties *getProperties() { return prop; };
	Device *getDevice();

protected:
	DeviceProperties *prop;	//!< points to properties within device obj
	Device *device;			//!< connected device
signals:
    //! for users of DevPropertiesDlg, emitted when Ok or Apply pressed
	void apply();		
	void sigNewProps();	//!< emitted when *prop changed
	void sigUpProps();	//!< emitted to udate *prop from widgets
private:
	void updateCaption();
	bool debugTabAdded;
protected slots:
    void slotApply();
};


//----------------------------------------------------------------------
//! abstract base class for device property tabs
/*!
  Devices that need to add special tabs to the properties dialog need
  to subclass this class.
*/
class DevPropertiesTab : public QWidget {
	Q_OBJECT
public:
	DevPropertiesTab( QWidget *parent, DevPropertiesDlg *_mainDlg, 
					  const char *name=0, Qt::WFlags f=0);
	//! destructs DevPropertiesTab
	virtual ~DevPropertiesTab() {}

	//! update device properties from widgets
	virtual void updateProperties() = 0;
	//! update widgets from properties
	virtual void setProperties() = 0;
protected:
	DevPropertiesDlg *getMainDlg();
private:
	DevPropertiesDlg *mainDlg;
protected slots:
    void slotNewProps();	// new properties have been applied
	void slotUpProps();		// update properties from widgets
};

//----------------------------------------------------------------------
//! general device property tab 
class DevGeneralTab : public DevPropertiesTab {
public:
	DevGeneralTab( QWidget *parent, DevPropertiesDlg *_mainDlg,
				   const char *name=0, Qt::WFlags f=0);
	
	virtual void updateProperties();
	virtual void setProperties();	
private:
	InstNameWidget *instNameWidget;
};

//----------------------------------------------------------------------
//! debug settings property tab 
class DevDebugTab : public DevPropertiesTab {
public:
	DevDebugTab( QWidget *parent, DevPropertiesDlg *_mainDlg,
				 const char *name=0, Qt::WFlags f=0);
	
	virtual void updateProperties();
	virtual void setProperties();	
private:
	Q3ComboBox *dbgLevCbox[10];
	enum DbgLevels { ErrorsOnly=0xc0008000, Basic=0xc0008001, 
					 Verbose=0xc0008003, VeryVerbose=0xc0008007 };
};

//----------------------------------------------------------------------
//! smb settings property tab 
class DevSmbTab : public DevPropertiesTab {
	Q_OBJECT
public:
	DevSmbTab( bool isCham, QWidget *parent, DevPropertiesDlg *_mainDlg,
				 const char *name=0, Qt::WFlags f=0);
	
	virtual void updateProperties();
	virtual void setProperties();
	
	void hideOnboardSmbWidgets(bool hide);
	void hideChamSmbWidgets(bool hide);
	
signals:
    //! emitted whenever the smb properties are updated 
    void activateOnboardSmb( bool active, MdiswizView* wizView );
    void activateChamSmb( bool active, int numControllers, MdiswizView* wizView );
	
private slots:
	void enableSpinBox(int state);
			
private:
	QCheckBox*	obSmbActiveChkBox;
	QCheckBox*	z001SmbActiveChkBox;
	
	QLabel* 	numZ001Label;
	QSpinBox*	numZ001SmbSpinBox;
	
};

#endif


