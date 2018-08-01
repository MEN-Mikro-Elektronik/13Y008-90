/***************************************************************************
 */
/*!       \file  setting.h
 *       \brief  Classes for configuration settings
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/07/12 10:07:10 $
 *    $Revision: 2.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: setting.h,v $
 * Revision 2.1  2010/07/12 10:07:10  dpfeuffer
 * R: access violation in mdiswiz_lib.lib under Windows with MDIS_CLINST
 * M: QComboBox replaced with Q3ComboBox
 *
 * Revision 2.0  2010/01/22 11:27:43  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.7  2008/02/06 11:53:13  cs
 * changed:
 *   - DirectorySetting::edit from private to protected
 *
 * Revision 1.6  2006/11/30 11:27:59  cs
 * added StringSetting class
 *
 * Revision 1.5  2004/07/14 11:21:17  ufranke
 * added
 *  - setInfoText()
 *
 * Revision 1.4  2004/06/25 13:38:06  ufranke
 * changed
 *  - setChoises() parameter _default added
 *
 * Revision 1.3  2004/01/09 08:20:28  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/05/14 13:45:58  kp
 * misc
 *
 * Revision 1.1  2003/05/02 14:36:27  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef SETTING_H
#define SETTING_H


#include <q3tabdialog.h>
#include <qvariant.h>
#include <q3frame.h>
#include <q3memarray.h>

#include <Q3PtrList>

#include "wizexcept.h"
#include "util.h"

class Configuration;
class TargetOs;
class SettingFrame;
class Q3ComboBox;
class QLineEdit;
class QPushButton;
class Q3Wizard;
class SettingList;
class Q3ButtonGroup;
class QIntValidator;

//----------------------------------------------------------------------
//! Project/Configuration settings tab'ed dialog
/*!
  This dialog is typically created by TargetOs::settingsDlg().

  Normally it will provide a tab for the OS specific settings and a tab
  for the SwModule selection, see OsLinux::settingsDlg().

*/
class SettingsDlg : public Q3TabDialog
{
	Q_OBJECT
public:
	SettingsDlg( QWidget *parent, Configuration *cfg );
	void setButtons();
	//! return pointer to current configuration
	Configuration *getConfiguration() { return config; }
protected:
	Configuration *config;	//!< ptr to current configuration

signals:

protected slots:
};


//----------------------------------------------------------------------
//! Subclass of QFrame to be used in settings dialog and wizards
/*!
  This behaves essentially as QFrame, except it emits
  destroyed() when the frame is deleted
*/
class SettingFrame : public Q3Frame {
	Q_OBJECT
public:
	//! this construts a SettingFrame (nothing else than QFrame)
	SettingFrame( QWidget *parent=0, const char *name=0,
		Qt::WFlags f=0 ) : Q3Frame( parent, name, f ) { }

	//! destructs SettingFrame and emits destroyed()
	~SettingFrame(){
		emit destroyed( this );
	}
signals:
	//! this signal is emitted when this object is destroyed by destructor
	void destroyed( SettingFrame *frm );
};


//----------------------------------------------------------------------
//! Class to store and manipulate configuration settings
/*!
  To be used to store OS specific settings, such as compiler include
  paths, or the name of the compiler executive.

  This class has a \em key and a \em value; the value is a QVariant object
  and therefore can hold different data types such as \c int, \c QString
  or \c QStringList.

  The Setting object can be manipulated in the "new project wizard" or
  in the "project settings" tab. For both possibilities, this class
  provides methods to generate the widgets. See settingsTabWidget() and
  wizardPage(). Both functions may return \c 0 to indicate that the setting
  is not modifyable in either the wizard or the project settings tab.

  If they return non-null, they should really return a SettingFrame object,
  because these object signal their destruction via the the
  SettingFrame::destroyed() signal to tell the Setting that this widget
  is no longer valid.

  \remark Only one of these widgets can be active at the same time.

  This class is intended to be subclassed for each individual setting.

  \sa SettingList
*/
class Setting : public QObject {
	Q_OBJECT
public:
	//! flags to pass to setEnabled()
	enum EnabledFlags {
		Enabled,			//!< show and allow changes
		Disabled, 			//!< show, but don't allow changes
		Hide,				//!< hide from dialog
	};

	Setting( const QString &key );

	//! destructor
	virtual ~Setting() {}

	//! set a pointer back to the SettingList containg this setting
	void setList( SettingList *_lst ) { lst = _lst; }

	//! get the key name
	QString key() { return _key; }

	//! get value of setting
	QVariant value() { return _value; }

	//! get the "enabled" property. \sa EnabledFlags
	EnabledFlags enabled() { return _enabled; }

	//! get the "persist" property
	/*!
	  if true, this setting shall be saved/restored from/to the
	  configuration on disk
	*/
	bool persist() { return _persist; }

	//! set the "persist" property.
	/*!
	  The default of this property is true
	*/
	void setPersist(bool persist) { _persist = persist; }

	//--------------------------------------------------------------
	// the following routines should be reimplimented by subclasses

	//! set the "enabled" property.
	/*!
	  It shall enable or disable all widgets of the setting. \a mode
	  shall is one of the EnabledFlags.

	  - In the wizard, if mode is either Hide or Disabled,
	    it does not appear
	  - In the settings tab, if a widget is Disabled, it will be shown,
	    but user can't change it. If it's mode is Hide, it will not appear.
	*/
	virtual void setEnabled( EnabledFlags mode ) { _enabled = mode; }

	virtual void setValue( const QVariant &newVal );

	//! make widget for setting tab for this setting
	virtual QWidget *settingsTabWidget( QWidget *parent ){
		parent=parent;return 0;
	}

	//! make wizard page for this setting
	/*!
	  \return A page to be displayed in the "new configuration wizard"
	  \return \a *pageTitleP set to the page's title
	*/
	virtual QWidget *wizardPage( Q3Wizard *parent, QString *pageTitleP ) {
		parent = parent;
		pageTitleP = pageTitleP;
		return 0;
	}

	//! get title for wizard page
	virtual QString wizPageTitle() { return QString::null; }
	//! get title for label in settings tab
	virtual QString labelText() { return QString::null; }
	//! set tooltip text / info field in wizard and Makefile
	virtual void setInfoText(QString *infoNew ){ info = *infoNew; }
	//! get tooltip text / info field in wizard and Makefile
	virtual QString infoText() { return info; }

	virtual Setting *clone();
	void copy(Setting *setting);

	//! get the widget that is currently controlling the setting
	virtual QWidget *currentWidget() { return frm; }

	//! check if \a newVal is valid for this setting
	/*!
	  \return true if \a newVal is valid
	  If \a newVal is invalid, append lines to \a errMsg
	*/
	virtual bool validate( const QVariant &newVal, QString &errMsg ) {
		UTIL_UNREF_PARAM(errMsg);
		UTIL_UNREF_PARAM(newVal);
		return true;
	}

signals:
	//! emitted whenever the Setting object's value is changed
	void valueChanged( const QVariant &newValue );

public slots:
    //! update value from widgets
    /*!
	  should be called so that the Setting widgets can re-read the
	  underlying widgets (e.g. QLineEdit widgets). If the setting is
	  invalid, it should add lines to \a errMsg.
	  In any case, the Setting's value is changed.
	*/
    virtual void slotApply( QString &errMsg ) {
		UTIL_UNREF_PARAM(errMsg);
	};
    virtual void settingFrameDestroyed( SettingFrame *_frm );

protected:
	SettingFrame *frm;  //!< the created frame (0=none)
	Q3Wizard *wiz;		//!< reference to the QWizard dialog
	QWidget *tab;		//!< reference to the settings tab
	QString _key;		//!< name (ID) of setting
	QVariant _value;	//!< value of setting
	QString info;
	EnabledFlags _enabled;	//!< flags \sa EnabledFlags
	bool _persist;		//!< setting shall be saved/restored from/to disk
	SettingList *lst;	//!< list containing that setting
};

//----------------------------------------------------------------------
//! A variant of the Setting Class that features "Multiple-Choise" selections
/*!
  The current value can be retrieved either by value() (as a QString)
  or index(), that returns the currently selected item as an index.

  In the project wizard, user can select the choises by clicking of
  one of the radiobuttons.

  In the settings tab, the user can select one of the choises from a
  Q3ComboBox.

  \remark The \a choises and \a choisesHelp must be initialized by subclasses.
  \a choisesHelp is displayed in the wizard only
*/
class MultipleChoiseSetting : public Setting {
	Q_OBJECT
public:

	MultipleChoiseSetting( QString key );

	virtual void setEnabled( EnabledFlags enabled );
	virtual void setValue( const QVariant &newVal );

	virtual QWidget *settingsTabWidget( QWidget *parent );
	virtual QWidget *wizardPage( Q3Wizard *parent, QString *pageTitleP );
	virtual int index();

	virtual Setting *clone();
	void copy(MultipleChoiseSetting *setting);

	//! set list of choises from outside
    /*!
		  This works even when the widget has been created already
		  \param lst      list of strings
		  \param helpLst  list of help strings
		  \param _default 0 or a default string
    */
	virtual void setChoises( const QStringList &lst,
							 const QStringList &helpLst,
							 const QString     &_default );

signals:
	//! emitted when the setting is changed
	void indexChanged( int index );

protected:

private slots:
    void slotCboxChange( int idx );

protected:
	//! the list of possible choises valid for this setting
	/*!
	  must be initialized by subclasses!
	*/
	QStringList choises;

	//! help text for each choise
	/*!
	  Displayed in the wizard page only.
	  Must be initialized by subclasses!
	*/
	QStringList choisesHelp;

private:
	Q3ComboBox *cb;
	Q3ButtonGroup *buttonGroup;
    void makeButtonGroup();
};

//----------------------------------------------------------------------
//! A variant of the Setting Class that features directory or file selections
/*!
 */
class DirectorySetting : public Setting {
	Q_OBJECT
	public:
	//! The file selection mode.
	/*!
		Select either Dir or File. If this must exist, OR mode with
		MustExist.
	 */
		enum Mode { Dir=0x1, File=0x2, MustExist=0x100 };
		DirectorySetting( QString key, int mode );

		virtual void setEnabled( EnabledFlags enabled );
		virtual void setValue( const QVariant &newVal );
		virtual bool validate( const QVariant &newVal, QString &errMsg );

		virtual QWidget *settingsTabWidget( QWidget *parent );
		virtual QWidget *wizardPage( Q3Wizard *parent, QString *pageTitleP );

		virtual Setting *clone();
		void copy(DirectorySetting *setting);

	public slots:
		virtual void slotApply( QString &errMsg );

	protected:

	private slots:
		virtual void slotBrowse();

	protected:
		QLineEdit *edit;
	private:
		QPushButton *button;
		int mode;
};

//----------------------------------------------------------------------
//! A variant of the Setting Class that features decimal integer inputs
/*!
*/
class IntSetting : public Setting {
	Q_OBJECT
public:
	IntSetting( QString key, int min, int max );
	~IntSetting();

	virtual void setEnabled( EnabledFlags enabled );
	virtual void setValue( const QVariant &newVal );
	virtual bool validate( const QVariant &newVal, QString &errMsg );

	virtual QWidget *settingsTabWidget( QWidget *parent );
	virtual QWidget *wizardPage( Q3Wizard *parent, QString *pageTitleP );

	virtual Setting *clone();
	void copy(IntSetting *setting);

public slots:
    virtual void slotApply( QString &errMsg );

protected:

protected:
private:
	int _min, _max;
	QLineEdit *edit;
	QIntValidator *validator;
};

//----------------------------------------------------------------------
//! A variant of the Setting Class that features simple strings
/*!
 */
class StringSetting : public Setting {
	Q_OBJECT
public:
	StringSetting( QString key );

	virtual void setEnabled( EnabledFlags enabled );
	virtual void setValue( const QVariant &newVal );
	virtual bool validate( const QVariant &newVal, QString &errMsg );

	virtual QWidget *settingsTabWidget( QWidget *parent );
	virtual QWidget *wizardPage( Q3Wizard *parent, QString *pageTitleP );

	virtual Setting *clone();
	void copy(StringSetting *setting);

public slots:
	virtual void slotApply( QString &errMsg );

protected:
private slots:
protected:
private:
	QLineEdit *edit;
};

//----------------------------------------------------------------------
//! class that holds a list of Setting objects
/*!
  A subclass of QPtrList that serves as a container for Setting objects.

  The Setting objects are owned by the SettingList. They will be deleted
  when the SettingList is deleted.

  After adding Setting objects to the list, one must call either
  Setting::setList() or call connectSettingsToList().

  This class also contains some utility functions (expandedValueOf())
  to expand Makefile-like macro of a string. The macro names can be
  resolved from the settings win this list or by environment variables.

*/
class SettingList : public Q3PtrList<Setting> {
public:
	SettingList();
	SettingList *clone();
	void connectSettingsToList();
	void take( SettingList *settingList );
	void takeValues( SettingList *settingList );
	Setting *find( const QString &key );
	QString expandedValueOf( QString str );
	QString expandedValueOf( Setting *setting );
	QString resolveMacro( QString macName );
	static QStringList macrosOf( QString str );
	void applyAll( QString &errMsg );
private:
};
#endif





