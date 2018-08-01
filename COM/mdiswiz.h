/***************************************************************************
 */
/*!       \file  mdiswiz.h
 *       \brief  Classes for the mdiswiz GUI
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2013/01/21 10:27:30 $
 *    $Revision: 2.2 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: mdiswiz.h,v $
 * Revision 2.2  2013/01/21 10:27:30  ts
 * R: manual system configuration was often difficult
 * M: added support for automatic system scan
 *
 * Revision 2.1  2010/05/19 15:21:07  CRuff
 * R: 1. Implementation of SmbPciBbis on z001 controllers (fpga)
 * M: 1a) implement class BbisChamDeviceViewItem (inherits BbisDeviceViewItem)
 *    1b) make methods of BbisDeviceViewItem virtual to allow inheritance
 *
 * Revision 2.0  2010/01/22 11:26:33  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.9  2009/08/17 13:29:13  CRuff
 * R: VxWorks: CPU core was not updated correctly when CPU was changed
 * M: new signal introduced to notify configuration tabs about cpu change
 *
 * Revision 1.8  2008/07/23 11:49:18  gvarlet
 * R:1. No support for QNX
 *   2. When changing the CPU on a already setted up configuration, the application
 * 	 of changing was not done
 * M:1. Add support for QNX
 *   2. Make the MdiswizView::slotChangeCpu() function overloadable
 *
 * Revision 1.7  2005/07/15 15:47:03  ub
 * cosmetics
 *
 * Revision 1.6  2004/07/22 12:05:41  kp
 * added OS9
 *
 * Revision 1.5  2004/06/08 17:25:29  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.4  2003/06/11 12:21:34  kp
 * intermediate checkin
 *
 * Revision 1.3  2003/06/06 13:32:20  kp
 * slotHelp removed
 *
 * Revision 1.2  2003/06/03 10:16:19  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:36:05  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MDISWIZVIEW_H
#define MDISWIZVIEW_H

#include <qsplitter.h>
#include <qstring.h>
#include <qobject.h>
#include <qdatetime.h>
#include <qlist.h>
#include <q3listview.h>
#include <qmenubar.h>
#include <q3listview.h>
#include <q3mainwindow.h>

#include <QCloseEvent>
#include <QPixmap>
#include <QLabel>
#include <QMouseEvent>
#include <Q3PtrList>
#include <Q3PopupMenu>

#include "util.h"

class QAction;
class Q3ToolBar;
class Q3ListView;
class QLabel;
class QPainter;
class QColorGroup;
class Q3PopupMenu;
class Device;
class CpuDevice;
class BbisDevice;
class MdisDevice;
class BusInterface;
class BbisSlot;
class MdiswizView;
class ExecProgram;
class HwComponent;

// -----------------------------------------------------------------
//! GUI representation of a hardware component
/*!
  This object has exactly one Device associated and will be used in mdiswiz's
  main window to display the hierarchy of the components.
  
  It also provides a context dependent popup menu for right mouse clicks.

  Subclasses will setup a special pixmap for each entry for easier
  identification.

*/
class ComponentViewItem : public Q3ListViewItem
{
public:
    ComponentViewItem(  MdiswizView *_wizview, Q3ListView *parent, 
						const QString &name );
	ComponentViewItem(  MdiswizView *_wizview, Q3ListViewItem *parent, 
						const QString &name );
	virtual ~ComponentViewItem();

	//! get associated Device
	Device *getDevice() { return device; };

	// show popup Menu
	virtual void showPopup( const QPoint & point );

	//! get the popup menu
	Q3PopupMenu *getPopup() { return popup; };
	void refresh();

	//! get a list of interfaces to attach to 
	virtual void getAttachableIfNames(QStringList *) { };

	//! get a list of devices attacheable to \a hwcomp
	/*!	
	  hwComp must be either a bbSlot or busIf
	 */
	virtual void getAttachableDevs( HwComponent *hwComp, 
									   Q3PtrList<Device> *) {
		UTIL_UNREF_PARAM(hwComp);
	};

	virtual bool attachDev( HwComponent *hwComp, 
							 QString newDevName) {
		UTIL_UNREF_PARAM(hwComp);
		return false;
	};

protected:
	Device *device;			//!< associated device
	Q3PopupMenu *popup;		//!< the popup menu
	MdiswizView *wizView;	//!< link to the main window
private:
	void init( MdiswizView *_wizview, const QString &name );
    QPixmap *pix;
};

// -----------------------------------------------------------------
//! GUI representation of a CPU device
class CpuDeviceViewItem : public ComponentViewItem
{
public:
    CpuDeviceViewItem( MdiswizView *_wizview, Q3ListView *parent, 
					   const QString &name );
    CpuDeviceViewItem( MdiswizView *_wizview, Q3ListViewItem *parent, 
					   const QString &name );

	virtual void getAttachableIfNames(QStringList *);
	virtual void getAttachableDevs( HwComponent *hwComp, 
									Q3PtrList<Device> *);
	virtual bool attachDev( HwComponent *hwComp, 
							QString newDevName);

	// show popup Menu
	void showPopup( const QPoint & point );
	
	// add component to businterface (GUI) 
	bool addToBusIf( int id );

	void setDevice( CpuDevice *_device );
protected:
};

// -----------------------------------------------------------------
//! GUI representation of a BBIS device

class BbisDeviceViewItem : public ComponentViewItem
{
public:
	BbisDeviceViewItem(  MdiswizView *_wizview,
						 Q3ListViewItem *parent, const QString &name );
	virtual void setDevice( BbisDevice *_device );

	virtual void getAttachableIfNames(QStringList *);
	virtual void getAttachableDevs( HwComponent *hwComp, 
									Q3PtrList<Device> *);
	virtual bool attachDev( HwComponent *hwComp, 
							QString newDevName);

	// show popup Menu
	virtual void showPopup( const QPoint & point );

	// add component to bbis slot (GUI) 
	bool addToBbSlot( int id );

protected:
};


class BbisChamDeviceViewItem : public BbisDeviceViewItem
{
	public:
	BbisChamDeviceViewItem(  MdiswizView *_wizview,
						 Q3ListViewItem *parent, const QString &name ) :
		BbisDeviceViewItem( _wizview, parent, name ) {};
	
	virtual void setDevice( BbisDevice *_device );
	virtual void getAttachableIfNames(QStringList *);
	virtual void getAttachableDevs( HwComponent *hwComp, 
									Q3PtrList<Device> *);
		
	virtual bool attachDev( HwComponent *hwComp, 
							QString newDevName);
	
	// show popup Menu
	virtual void showPopup( const QPoint & point );
							
};

// -----------------------------------------------------------------
//! GUI representation of a MDIS device

class MdisDeviceViewItem : public ComponentViewItem
{
public:
	MdisDeviceViewItem(  MdiswizView *_wizview, 
						 Q3ListViewItem *parent, const QString &name );
	// show popup Menu
	void showPopup( const QPoint & point );
	void setDevice( MdisDevice *_device );
protected:
};

//! Subclass of QListView to change the double click behaviour
class MdisWizListView : public Q3ListView {
public:
	//! creates MdisWizListView (same parameters as QListView)
	MdisWizListView( QWidget * parent = 0, const char * name = 0, 
					 Qt::WFlags f = 0 ) : Q3ListView( parent, name, f ) {};
		
	//! overrides QListView method. 
	/*!
	  It is called when a double click is detected on an item
	  It just emits doubleClicked() without opening/closing a folder
	*/
	void contentsMouseDoubleClickEvent ( QMouseEvent * e ) {

		QPoint vp = contentsToViewport(e->pos());

		Q3ListViewItem * i = itemAt( vp );

		if ( !i || !i->isEnabled() )
			return;

		emit doubleClicked( i );
	}
	ComponentViewItem *findItemByInstName( const QString &name );

};

// -----------------------------------------------------------------
//! Class for GUI representation of the component hierarchy
/*!
  Contains also the main program logic
*/
class MdiswizView : public Q3MainWindow
{
    Q_OBJECT

public:
    MdiswizView( QWidget *parent = 0, const char *name = 0 );

	//! destructs MdiswizView
    ~MdiswizView() {}
	
	Q3ListView *getListView();
	bool loadConfig( const QString &cfgName );
	bool newConfig();
	bool scanSystem();
	static QString Version();
	ComponentViewItem *activeItem();

protected:
	void closeEvent( QCloseEvent* ce );
	int confirmClose();
	virtual void setupProjectActions();

    MdisWizListView *listView;		  //!< widget for component hierarchy	
	QMenuBar *menu;					  //!< main menu bar	
	Q3PopupMenu *projectMenu;		  //!< project menu
	Q3ToolBar *tb;					  //!< toolbar

	ComponentViewItem *popupMenuItem; //!< item whose popup menu is displayed
	bool cfgModified;				  //!< flags config has been modified
	QLabel *modifiedLabel;			  //!< status bar "modified" label
private:
	void cfgUnmodified();
	void changeCaption();
	void doBuild( const QString &title, const QStringList &args );
	
public slots:
	void slotNewConfig();
	void slotCfgModified();

protected slots:
	void slotRMB( Q3ListViewItem* Item, const QPoint & point, int );
	void slotLoadConfig();
	void slotSaveConfig();
	void slotSaveAsConfig();
	void slotCfgSettings();
	void slotAddToBusIf( int id );
	void slotAddToBbSlot( int id );
	void slotAddComponent();
	void slotDelete();
	void slotDefineCpu();
	void slotPropertiesDc( Q3ListViewItem *item );
	void slotProperties();
	void slotPkgInstall();
	void slotPkgReparse();
	void slotPkgShow();
	//void slotHelp();	
	void slotInfo();
	
	virtual void slotChangeCpu();
	
signals:
    void sigCpuChanged();
		
};

// -----------------------------------------------------------------
//! Subclass of MdiswizView to reimplement some methods for linux
class LinuxMdiswiz: public MdiswizView 
{
	Q_OBJECT

public:
	//! constructor: passes all args to MdiswizView
    LinuxMdiswiz( QWidget *parent = 0, const char *name = 0 ) : 
		MdiswizView( parent, name ) 
	{	
		buildDlg = 0; 
		setupProjectActions();
	};

protected:
	virtual void setupProjectActions();
	ExecProgram *buildDlg;			  //!< build program dialog if active	

private:
	void doBuild( const QString &title, const QStringList &args, int isScan );

protected slots:
	void slotBuild();
	void slotBuildInstall();
	void slotRebuild();
	void slotScan();
	void slotBuildFinished();
	void slotScanFinished();
};

// -----------------------------------------------------------------
//! Subclass of MdiswizView to reimplement some methods for QNX
class QnxMdiswiz: public MdiswizView 
{
	Q_OBJECT

public:
	//! constructor: passes all args to MdiswizView
    QnxMdiswiz( QWidget *parent = 0, const char *name = 0 ) : 
		MdiswizView( parent, name ) 
	{	
		buildDlg = 0; 
		setupProjectActions();
	};

protected:
	virtual void setupProjectActions();
	ExecProgram *buildDlg;			  //!< build program dialog if active	

private:
	void doBuild( const QString &title, const QStringList &args );

protected slots:
	void slotBuild();
	void slotRebuild();
	void slotBuildFinished();
	
	virtual void slotChangeCpu();
};

// -----------------------------------------------------------------
//! Subclass of MdiswizView to reimplement some methods for VxWorks
class VxworksMdiswiz: public MdiswizView
{
	Q_OBJECT

public:
	//! constructor: passes all args to MdiswizView
    VxworksMdiswiz( QWidget *parent = 0, const char *name = 0 ) :
		MdiswizView( parent, name )
	{
		buildDlg = 0;
		setupProjectActions();
	};

protected:
	virtual void setupProjectActions();
	ExecProgram *buildDlg;			  //!< build program dialog if active

private:
	void doBuild( const QString &title, const QStringList &args );

protected slots:
	void slotBuild();
	void slotRebuild();
	void slotBuildFinished();
};

// -----------------------------------------------------------------
//! Subclass of MdiswizView to reimplement some methods for OS9
class Os9Mdiswiz: public MdiswizView 
{
	Q_OBJECT

public:
	//! constructor: passes all args to MdiswizView
    Os9Mdiswiz( QWidget *parent = 0, const char *name = 0 ) : 
		MdiswizView( parent, name ) 
	{	
		buildDlg = 0; 
		setupProjectActions();
	};

protected:
	virtual void setupProjectActions();
	ExecProgram *buildDlg;			  //!< build program dialog if active	

private:
	void doBuild( const QString &title, const QStringList &args );

protected slots:
	void slotBuild();
	void slotRebuild();
	void slotBuildFinished();
};
#endif

