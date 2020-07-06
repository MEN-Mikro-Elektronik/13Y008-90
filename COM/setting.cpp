/***************************************************************************
 */
/*!       \file  setting.cpp
 *       \brief  Implementation of configuration setting classes
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/07/12 10:07:05 $
 *    $Revision: 2.1 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: setting.cpp,v $
 * Revision 2.1  2010/07/12 10:07:05  dpfeuffer
 * R: access violation in mdiswiz_lib.lib under Windows with MDIS_CLINST
 * M: QComboBox replaced with Q3ComboBox
 *
 * Revision 2.0  2010/01/22 11:27:40  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.10  2009/04/27 17:54:20  CRuff
 * R: Cosmetics
 * M: only show ':' behind settings if description/help text is not empty
 *
 * Revision 1.9  2006/11/30 11:27:57  cs
 * added StringSetting class
 *
 * Revision 1.8  2004/07/21 15:00:05  ufranke
 * changed
 *  - currentCfg() by MAIN_GetCurrentCfg()
 *
 * Revision 1.7  2004/07/14 11:21:59  ufranke
 * added
 *  - info initialisation
 *
 * Revision 1.6  2004/06/25 13:37:13  ufranke
 * changed
 *  - setChoises() parameter _default added
 *
 * Revision 1.5  2004/01/09 08:20:26  kp
 * intermediate checkin
 *
 * Revision 1.4  2003/06/05 12:01:25  dschmidt
 * intermediate checkin
 *
 * Revision 1.3  2003/06/05 09:33:57  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/06/03 10:16:46  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:36:26  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <qobject.h>
#include <qlabel.h>
#include <q3vbox.h>
#include <qlayout.h>
#include <q3combobox.h>
#include <qlineedit.h>
#include <q3whatsthis.h>
#include <qpushbutton.h>
#include <q3buttongroup.h>
#include <qradiobutton.h>
#include <q3filedialog.h>
#include <qfileinfo.h>
#include <qmessagebox.h>
#include <q3wizard.h>
#include <qregexp.h>
#include <qvalidator.h>
#include <stdlib.h>

#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <Q3Button>

#include "wizdebug.h"
#include "propertiesdlg.h"
#include "hwcomponent.h"
#include "configuration.h"
#include "setting.h"
#include "util.h"
#include "vmsgbox.h"

extern QWidget *G_mainWindow;


//----------------------------------------------------------------------

//! creates a Setting object
/*!
  Its value is set to null and the setting is enabled.
*/
Setting::Setting( const QString &key )
{
	frm = 0;
	wiz = 0;
	tab = 0;
	lst = 0;
	_key = key;
	_value = QString();
	info = QString::null;
	_enabled = Enabled;
	_persist = true;
}

//! set a new value
/*!
  Set the new value and emit valueChanged().

  \remark The \a newVal is not checked for validity.

*/
void
Setting::setValue( const QVariant &newVal ) {
	_value = newVal;
	emit valueChanged( _value );
}

//! called whenever the setting's frame is destroyed
void
Setting::settingFrameDestroyed( SettingFrame *_frm )
{
	wDebug(("settingFrameDestroyed %s %p %p", (const char *)_key, frm, frm));
	WIZ_ASSERT( _frm == frm );
	frm = 0;
	wiz = 0;
	tab = 0;
}

//! make a copy of this object
Setting *
Setting::clone()
{
	Setting *setting = new Setting(key());
	copy( setting );
	return setting;
}

//! copy attributes of this object to \a setting
void
Setting::copy(Setting *setting)
{
	setting->_key = key();
	setting->_value = value();
	setting->_enabled = enabled();
	setting->_persist = persist();
}

//----------------------------------------------------------------------
//! creates a multiple choise setting object
MultipleChoiseSetting::MultipleChoiseSetting( QString key ) :
	Setting( key )
{
}

void
MultipleChoiseSetting::setEnabled( EnabledFlags enabled )
{
	if( frm ){
		if( wiz )
			wiz->setAppropriate( frm, enabled==Enabled );

		else if( tab )
			cb->setEnabled( enabled==Enabled );
	}
	Setting::setEnabled( enabled );
}

void
MultipleChoiseSetting::setValue( const QVariant &newVal )
{
	int idx;
	// check if it is a known environment
	if( (idx=choises.findIndex( newVal.toString() )) == -1 ){
		return;
	}
	if( frm ){
		if( wiz )
			buttonGroup->setButton( idx );

		else if ( tab )
			cb->setCurrentItem( idx );
	}
	Setting::setValue( newVal );
	emit indexChanged( idx );
}

//! get the currently selected index (0=first choise)
int
MultipleChoiseSetting::index()
{
	return choises.findIndex( value().toString() );
}

QWidget *
MultipleChoiseSetting::settingsTabWidget( QWidget *parent )
{
	WIZ_ASSERT( frm == 0 ); // assertion if frame already generated

	frm = new SettingFrame(parent);
	Q3VBoxLayout *vb = new Q3VBoxLayout(frm);

	QLabel *label;
	if (labelText() != "")
	{
		label = new QLabel( key() + ": " + labelText(), frm );
	}
	else
	{
		label = new QLabel( key(), frm );
	}
	vb->addWidget( label );

	Q3HBoxLayout* hb = new Q3HBoxLayout();
	vb->addLayout(hb);

	cb = new Q3ComboBox( false, frm );
	cb->insertStringList( choises );
	cb->setCurrentText(value().toString());

	hb->addWidget( cb );
	hb->addStretch(1);

	Q3WhatsThis::add( frm, infoText() );

	connect( cb, SIGNAL(activated(int)),
			 this, SLOT(slotCboxChange(int)));
	connect( frm, SIGNAL( destroyed( SettingFrame *)),
			 this, SLOT( settingFrameDestroyed( SettingFrame *) ));

	tab = parent;
	setEnabled( _enabled );
	return frm;
}

QWidget *
MultipleChoiseSetting::wizardPage( Q3Wizard *parent, QString *pageTitleP )
{
	WIZ_ASSERT( frm == 0 ); // assertion if frame already generated

	frm = new SettingFrame(parent);
	Q3VBoxLayout *vb = new Q3VBoxLayout(frm);
	frm->setMinimumWidth(520);
	frm->setMinimumHeight(270);
	QLabel *info = new QLabel( infoText(), frm );
	info->setWordWrap( true );
	vb->addStretch(1);
	vb->addWidget( info );

    buttonGroup = new Q3ButtonGroup( 1, Qt::Horizontal,
									QString::null, frm );

	vb->addStretch(1);
	vb->addWidget( buttonGroup );
	vb->addStretch(1);

	makeButtonGroup();
	if ( !QString::compare(this->key(),"WIZ_CDK", Qt::CaseInsensitive) ) {
		QString depracatedComponentTextLine0("<font color=\"black\">Note:</font>");
		QString depracatedComponentTextLine1("<font color=\"red\">EOL: No longer maintained (see supported_components.md)</font>");
		QLabel *label = new QLabel();
		label->setText(depracatedComponentTextLine0 + "<br>" + depracatedComponentTextLine1);
		vb->addWidget(label);
		vb->addStretch(1);
	}

	connect( buttonGroup, SIGNAL(clicked(int)),
			 this, SLOT(slotCboxChange(int)));

	connect( frm, SIGNAL( destroyed( SettingFrame *)),
			 this, SLOT( settingFrameDestroyed( SettingFrame *) ));

	*pageTitleP = wizPageTitle();
	wiz = parent;
	setEnabled( _enabled );
	return frm;
}

void
MultipleChoiseSetting::makeButtonGroup()
{
	for( int i=0; i < choises.count(); i++ ) {
		QString txt = choises[i];

		if( (i < choisesHelp.count()) && (choisesHelp[i] != "") )
			txt += ": " + choisesHelp[i];

		new QRadioButton( Util::breakString(txt, 50) , buttonGroup );
	}

	// activate currently selected button
	buttonGroup->setButton( index() );
}

void
MultipleChoiseSetting::setChoises( const QStringList &lst,
								   const QStringList &helpLst,
								   const QString     &_default  )
{
	choises = lst;
	choisesHelp = helpLst;
	int inList;

	if( frm ){
		if( buttonGroup ){

			foreach( QAbstractButton *obj, buttonGroup->findChildren<QAbstractButton*>()){
				// for each found object...
				wDebug(("setChoises: remove %p", obj ));
				buttonGroup->remove( obj );
				delete obj;
			}

			makeButtonGroup();
		}
	}

	if( !_default.isEmpty() )
	{
		wDebug(("setChoises: choises.find"));
		inList = choises.findIndex( _default );
		if( inList == -1 )
		{
			wDebug(("setChoises: choises.find !inList"));
			setValue( QVariant(choises[0]) );
		}
		else
		{
			wDebug(("setChoises: choises.find in list"));
			setValue( choises[inList] );
		}
	}
	else
	{
		wDebug(("setChoises: no default"));
	}

}


void
MultipleChoiseSetting::slotCboxChange( int idx )
{
	wDebug(("slotCboxChange %d", idx));
	QString errMsg;
	setValue( QVariant(choises[idx]));
	emit indexChanged( idx );
}

Setting *
MultipleChoiseSetting::clone()
{
	MultipleChoiseSetting *setting = new MultipleChoiseSetting(key());
	copy( setting );
	return setting;
}

//! copy attributes of this object to \a setting
void
MultipleChoiseSetting::copy(MultipleChoiseSetting *setting)
{
	Setting::copy( setting );
	setting->choises = choises;
}

//----------------------------------------------------------------------

//! creates a Directory setting object
/*!
  See DirectorySetting::Mode for mode values
*/
DirectorySetting::DirectorySetting( QString key, int _mode ) :
	Setting( key )
{
	mode = _mode;
}

void
DirectorySetting::setEnabled( EnabledFlags enabled )
{
	if( frm ){
		if( wiz ){
			wiz->setAppropriate( frm, enabled==Enabled );
			wDebug((" setAppropriate %p (%s) %d (is %d)", frm,
				   (const char *)key(),
				   enabled==Enabled, wiz->appropriate( frm )));
		}
		else if( tab ){
			edit->setEnabled( enabled==Enabled );
			button->setEnabled( enabled==Enabled );
		}
	}
	Setting::setEnabled( enabled );
}

//! set a new value for setting
/*!
  It allows any value to be set, even if it's invalid
*/
void
DirectorySetting::setValue( const QVariant &newVal )
{
	if( frm )
		edit->setText( newVal.toString() );

	Setting::setValue( newVal );
}

//! check if \a newVal is valid for this setting
/*!
  Depending on the current mode, this implementation checks if this is
  a file/directory and if it exists
*/
bool
DirectorySetting::validate( const QVariant &newVal, QString &errMsg )
{
	bool rv=true;
	QString expVal = lst->expandedValueOf(newVal.toString());
	QFileInfo fi( expVal );

	if( mode & MustExist ){

		if( ! fi.exists() ){
			errMsg += key() + ": \"" + expVal +	"\" doesn't exist\n";
			return false;
		}

		if( mode & Dir ){
			if( ! fi.isDir() ){
				errMsg += key() + ": \"" + expVal + "\" is not a directory\n";
				rv = false;
			}
		}

		if( mode & File ){
			if( ! fi.isFile() ){
				errMsg += key() + ": \"" + expVal +
					"\" is not a regular file\n";
			}
			rv = false;
		}
	}
	return rv;
}

QWidget *
DirectorySetting::settingsTabWidget( QWidget *parent )
{
	WIZ_ASSERT( frm == 0 ); // assertion if frame already generated

	frm = new SettingFrame(parent);
	Q3VBoxLayout *vb = new Q3VBoxLayout(frm);

	QLabel *label;
	if (labelText() != "")
	{
		label = new QLabel( key() + ": " + labelText(), frm );
	}
	else
	{ 
		label = new QLabel( key(), frm );
	}
	vb->addWidget( label );

	Q3HBoxLayout* hb = new Q3HBoxLayout();
	vb->addLayout(hb);

	edit = new QLineEdit( value().toString(), frm );
	hb->addWidget( edit );

	button = new QPushButton( "Browse", frm );
	hb->addWidget( button );

	connect( button, SIGNAL(clicked()), this, SLOT(slotBrowse()));
	//hb->addStretch(1);

	Q3WhatsThis::add( frm, infoText() );

	connect( frm, SIGNAL( destroyed( SettingFrame *)),
			 this, SLOT( settingFrameDestroyed( SettingFrame *) ));
	tab = parent;
	setEnabled( _enabled );
	return frm;
}

QWidget *
DirectorySetting::wizardPage( Q3Wizard *parent, QString *pageTitleP )
{
	WIZ_ASSERT( frm == 0 ); // assertion if frame already generated

	frm = new SettingFrame(parent);
	Q3VBoxLayout *vb = new Q3VBoxLayout(frm);

	QLabel *info = new QLabel( infoText(), frm );
	info->setWordWrap( true );
	vb->addStretch(1);
	vb->addWidget( info );
	vb->addStretch(1);

	Q3HBoxLayout* hb = new Q3HBoxLayout();
	vb->addLayout(hb);

	edit = new QLineEdit( value().toString(), frm );
	hb->addWidget( edit );

	button = new QPushButton( "Browse", frm );
	hb->addWidget( button );

	connect( button, SIGNAL(clicked()), this, SLOT(slotBrowse()));

	vb->addStretch(1);

	connect( frm, SIGNAL( destroyed( SettingFrame *)),
			 this, SLOT( settingFrameDestroyed( SettingFrame *) ));

	*pageTitleP = wizPageTitle();
	wiz = parent;
	setEnabled( _enabled );
	return frm;
}

//! called whenever user presses the "Browse" button
/*!
  Should bringup a QFileDialog to select the directory/file.
  After the user selected a directory/file, the validate function should
  be called and -- if failed -- tell the user about his error.
*/
void
DirectorySetting::slotBrowse()
{
	QString expandedVal, errMsg;

	if( frm == 0 ) return;

	// force all other settings to re-read their widgets in case
	// this setting contains macros refering to other settings.
	// errMsg will be ignored
	lst->applyAll( errMsg );


	expandedVal = lst->expandedValueOf( edit->text() );

	Q3FileDialog fdlg( G_mainWindow, "file dialog", TRUE );
	QString newVal;
	//QFileInfo fi( expandedVal );

	fdlg.setCaption( labelText() );


	// set mode of dialog
	switch( mode ){
	case Dir:
	case Dir|MustExist:
		fdlg.setMode( Q3FileDialog::Directory );
		break;
	case File|MustExist:
		fdlg.setMode( Q3FileDialog::ExistingFile );
		break;
	default:
	case File:
		fdlg.setMode( Q3FileDialog::AnyFile );
		break;
	}

	// set start dir/file of dialog
	switch( mode ){
	case Dir:
	case Dir|MustExist:
		fdlg.setDir( expandedVal );
	default: // file
		fdlg.setSelection( expandedVal );
	}

	// bring up dialog
	if ( fdlg.exec() == QDialog::Accepted ){
		QString errMsg;

		newVal = fdlg.selectedFile();

		// since newVal does not contain macros anymore, try to substitute
		// parts of the string with the macros that have been in the setting's
		// value before the user called this dialog
		QStringList macs = SettingList::macrosOf( value().toString() );

		for ( QStringList::Iterator it = macs.begin(); it != macs.end(); ++it){

			int idx;
			QString macVal = lst->resolveMacro( *it );
			wDebug(("slotBrowse: mac %s=%s", (const char *)*it,
				   (const char *)macVal ));

			// check if the macro's value is still a part of this string
			if( ( idx = newVal.find( macVal )) >= 0 ){

				// replace the macro's value with the macro name
				newVal = newVal.replace( idx, macVal.length(),
										 QString("$(") + *it + ")" );
			}
		}
		wDebug(("slotBrowse: subs. value %s\n", (const char *) newVal ));

		// set the new value
		if( validate( newVal, errMsg ) == false )

			VMsgBox::criticalOk(
				MAIN_GetCurrentCfg()->msgBoxCaption(),
				QString("Validation for ") + key() + " failed!",
				errMsg, frm );
		else
			setValue( newVal );
	}
}

void
DirectorySetting::slotApply( QString &errMsg )
{
	if( !frm )
		return;

	QVariant value = QVariant( edit->text());

	validate( value, errMsg );
	setValue( value );
}


Setting *
DirectorySetting::clone()
{
	DirectorySetting *setting = new DirectorySetting(key(),mode);
	copy( setting );
	return setting;
}

//! copy attributes of this object to \a setting
void
DirectorySetting::copy(DirectorySetting *setting)
{
	Setting::copy( setting );
	setting->mode = mode;
}

//----------------------------------------------------------------------
//! creates a integer setting object
IntSetting::IntSetting( QString key, int min, int max ) :
	Setting( key )
{
	validator = new QIntValidator( min, max, 0, 0 );
	_min = min;
	_max = max;
}

//! destructs IntSetting
IntSetting::~IntSetting()
{
	delete validator;
}

void
IntSetting::setEnabled( EnabledFlags enabled )
{
	if( frm ){
		if( wiz )
			wiz->setAppropriate( frm, enabled==Enabled );

		else if( tab )
			edit->setEnabled( enabled==Enabled );
	}
	Setting::setEnabled( enabled );
}

void
IntSetting::setValue( const QVariant &newVal )
{
	if( frm )
		edit->setText( newVal.toString() );

	Setting::setValue( newVal );
}

bool
IntSetting::validate( const QVariant &newVal, QString &errMsg )
{
	bool ok;
	int val;

	val = newVal.toInt( &ok );

	if( ok == false ){
		errMsg += key() + ": is not a valid integer.\n";
		return false;
	}
	if( val < _min ){
		errMsg += key() + QString(": value %1 is smaller than minimum (%2)").
			arg(val).arg(_min);
		return false;
	}
	if( val > _max ){
		errMsg += key() + QString(": value %1 is greater than maximum (%2)").
			arg(val).arg(_max);
		return false;
	}
	return true;
}

QWidget *
IntSetting::settingsTabWidget( QWidget *parent )
{
	WIZ_ASSERT( frm == 0 ); // assertion if frame already generated

	frm = new SettingFrame(parent);
	Q3VBoxLayout *vb = new Q3VBoxLayout(frm);

	QLabel *label;
	if (labelText() != "")
	{
		label = new QLabel( key() + ": " + labelText(), frm );
	}
	else
	{ 
		label = new QLabel( key(), frm );
	}
	vb->addWidget( label );

	edit = new QLineEdit( value().toString(), frm );
	edit->setValidator( validator );
	vb->addWidget( edit );

	Q3WhatsThis::add( frm, infoText() );

	connect( frm, SIGNAL( destroyed( SettingFrame *)),
			 this, SLOT( settingFrameDestroyed( SettingFrame *) ));

	tab = parent;
	setEnabled( _enabled );
	return frm;
}

QWidget *
IntSetting::wizardPage( Q3Wizard *parent, QString *pageTitleP )
{
	WIZ_ASSERT( frm == 0 ); // assertion if frame already generated

	frm = new SettingFrame(parent);
	Q3VBoxLayout *vb = new Q3VBoxLayout(frm);

	QLabel *info = new QLabel( infoText(), frm );
	info->setWordWrap( true );
	vb->addStretch(1);
	vb->addWidget( info );
	vb->addStretch(1);

	Q3HBoxLayout* hb = new Q3HBoxLayout();
	vb->addLayout(hb);

	edit = new QLineEdit( value().toString(), frm );
	edit->setValidator( validator );
	hb->addWidget( edit );

	vb->addStretch(1);

	connect( frm, SIGNAL( destroyed( SettingFrame *)),
			 this, SLOT( settingFrameDestroyed( SettingFrame *) ));

	*pageTitleP = wizPageTitle();
	wiz = parent;
	setEnabled( _enabled );
	return frm;

}


Setting *
IntSetting::clone()
{
	IntSetting *setting = new IntSetting(key(), _min, _max);
	copy( setting );
	return setting;
}

//! copy attributes of this object to \a setting
void
IntSetting::copy(IntSetting *setting)
{
	Setting::copy( setting );
}

void
IntSetting::slotApply( QString &errMsg )
{
	if( !frm )
		return;

	QVariant value = QVariant( edit->text());

	validate( value, errMsg );
	setValue( value );
}

//----------------------------------------------------------------------
//! creates a string setting object
StringSetting::StringSetting( QString key ) :
		Setting( key )
{
}

void
StringSetting::setEnabled( EnabledFlags enabled )
{
	if( frm ){
		if( wiz )
			wiz->setAppropriate( frm, enabled==Enabled );

		else if( tab )
			edit->setEnabled( enabled==Enabled );
	}
	Setting::setEnabled( enabled );
}

void
StringSetting::setValue( const QVariant &newVal )
{
	if( frm )
		edit->setText( newVal.toString() );

	Setting::setValue( newVal );
}

bool
StringSetting::validate( const QVariant &newVal, QString &errMsg )
{
	UTIL_UNREF_PARAM(newVal);
	UTIL_UNREF_PARAM(errMsg);
	return true;
}

QWidget *
StringSetting::settingsTabWidget( QWidget *parent )
{
	WIZ_ASSERT( frm == 0 ); // assertion if frame already generated

	frm = new SettingFrame(parent);
	Q3VBoxLayout *vb = new Q3VBoxLayout(frm);

	QLabel *label;
	if (labelText() != "")
	{
		label = new QLabel( key() + ": " + labelText(), frm );
	}
	else
	{ 
		label = new QLabel( key(), frm );
	}
	vb->addWidget( label );

	edit = new QLineEdit( value().toString(), frm );
	// edit->setValidator( validator ); ???
	vb->addWidget( edit );

	Q3WhatsThis::add( frm, infoText() );

	connect( frm, SIGNAL( destroyed( SettingFrame *)),
		 this, SLOT( settingFrameDestroyed( SettingFrame *) ));

	tab = parent;
	setEnabled( _enabled );
	return frm;
}

QWidget *
StringSetting::wizardPage( Q3Wizard *parent, QString *pageTitleP )
{
	WIZ_ASSERT( frm == 0 ); // assertion if frame already generated

	frm = new SettingFrame(parent);
	Q3VBoxLayout *vb = new Q3VBoxLayout(frm);

	QLabel *info = new QLabel( infoText(), frm );
	info->setWordWrap( true );
	vb->addStretch(1);
	vb->addWidget( info );
	vb->addStretch(1);

	Q3HBoxLayout* hb = new Q3HBoxLayout();
	vb->addLayout(hb);

	edit = new QLineEdit( value().toString(), frm );
	// edit->setValidator( validator ); ???
	hb->addWidget( edit );

	vb->addStretch(1);

	connect( frm, SIGNAL( destroyed( SettingFrame *)),
		 this, SLOT( settingFrameDestroyed( SettingFrame *) ));

	*pageTitleP = wizPageTitle();
	wiz = parent;
	setEnabled( _enabled );
	return frm;

}


Setting *
StringSetting::clone()
{
	StringSetting *setting = new StringSetting(key());
	copy( setting );
	return setting;
}

//! copy attributes of this object to \a setting
void
StringSetting::copy(StringSetting *setting)
{
	Setting::copy( setting );
}

void
StringSetting::slotApply( QString &errMsg )
{
	if( !frm )
		return;

	QVariant value = QVariant( edit->text());

	validate( value, errMsg );
	setValue( value );
}

//----------------------------------------------------------------------
//! constructs a SettingList object
SettingList::SettingList()
{
	setAutoDelete(true);
}

//! make a copy of all Setting objects and the list
/*!
  \return a pointer to the new list
*/
SettingList *
SettingList::clone()
{
	Setting *setting;
	SettingList *newList = new SettingList();

	for( setting=first(); setting; setting=next() ){
		Setting *newSetting = setting->clone();
		newList->append( newSetting );
		/*wDebug(("SettingList::clone %s = %s",
			   newSetting->key().latin1(),
			   newSetting->value().toString().latin1()));*/
	}
	return newList;
}

//! set the list pointer of all Setting objects in this list
void
SettingList::connectSettingsToList()
{
	Q3PtrListIterator<Setting> it(*this);
	Setting *setting;

	while( (setting = it.current() )){
		++it;
		setting->setList( this );
	}
}

//! take over data from another settinglist
/*!
  deletes all entries in the current object first.
  The takes over (copies) all entries from \a settingList into current list.
*/
void
SettingList::take( SettingList *settingList )
{
	Setting *setting;

	// delete all my entries
	while( (setting=first()) != 0 ){
		remove(setting);
	}

	// take new entries
	for( setting=settingList->first(); setting; setting=settingList->next() )
		append( setting->clone() );

	connectSettingsToList();
}

//! find the first occurance of an entry in the list with name \a key
/*!
  \return entry or 0 if not found
*/
Setting *
SettingList::find( const QString &key )
{
	Q3PtrListIterator<Setting> it(*this);
	Setting *setting;

	while( (setting = it.current() )){
		++it;
		if( setting->key() == key )
		{
			return setting;
		}
	}
	return 0;
}

//! take over values from another settinglist
/*!
  for every entry in \a settingList, find the corresponding entry in
  this object and take over the value, enabled and persist state
*/
void
SettingList::takeValues( SettingList *settingList )
{
	Setting *setting;
	Setting *thisSetting;

	// take new entries
	for( setting=settingList->first(); setting; setting=settingList->next() ){
		thisSetting = find( setting->key() );

		if( thisSetting ){
			wDebug(("\ntakeValues: setting=%s", setting->key().latin1() ));
			thisSetting->setPersist( setting->persist() );
			thisSetting->setEnabled( setting->enabled() );
			thisSetting->setValue( setting->value());
		}
	}
}


//! get value of \a str after expanding all macros
/*!
  This assumes that the setting's value is a QString.

  All contained macros must be there.

  Expansion is done recursively; i.e. if a macro contains other macros, they
  are expanded as well.

  If the macro name is not found in the SettingList it will attempt to
  find an environment variable with that name.

  If the string contains incorrectly terminated macros, e.g. "$(MACRO", it
  returns the resolved string up to this point

  \return expanded value of \a str

*/
QString
SettingList::expandedValueOf( QString str )
{
	QString rval, macName;
	int startIdx, endIdx;

	rval = str;

	// resolve macros
	while( (startIdx = rval.find( "$(" )) >= 0 ){

		if( (endIdx = rval.find( ')', startIdx )) < 0 )
			break;

		macName = rval.mid( startIdx+2, endIdx-startIdx-2 );

		wDebug(("%s found macro %s", (const char *)rval, (const char *)macName));

		// replace macro name in string with expanded macro
		rval = rval.replace( startIdx, endIdx-startIdx+1,
							 resolveMacro(macName) );
	}

	wDebug(("SettingList::expandedValueOf %s is %s",
		   (const char *)str, (const char *)rval));

	return rval;
}

//! get value of \a setting after expanding all macros
/*!
  This assumes that the setting's value is a QString.
*/
QString
SettingList::expandedValueOf( Setting *setting )
{
	WIZ_ASSERT(setting);

	return expandedValueOf( setting->value().toString() );
}

//! get a list of all macros contained in \a str
/*!
  e.g. when \a str is \c $(MAC1)/xxx/$(MAC2)yyy then this function
  will return a QStringlist containing two strings: \c MAC1 and \c MAC2.

  It will not descend recursively into submacros.
*/
QStringList
SettingList::macrosOf( QString str )
{
	QString macName, expMacro;
	QStringList rval;
	int startIdx=0, endIdx;

	while( (startIdx = str.find( "$(", startIdx )) >= 0 ){

		if( (endIdx = str.find( ')', startIdx )) < 0 )
			break; // end of macro not found. Abort

		// add macro name found to string list
		rval += str.mid( startIdx+2, endIdx-startIdx-2 );
		startIdx = endIdx + 1;
	}
	return rval;
}

//! get the value of macro \a macName
/*!
  \a macName can be the name of a Setting in this list or the name
  of an environment variable
*/
QString
SettingList::resolveMacro( QString macName )
{
	Setting *setting;
	QString expMacro;
	char *env;

	// expand found macro
	if( (setting = find( macName )) == 0 ){

		// macro not found, check if it's an environment variable
		if( (env = getenv( (const char *)macName )))
			expMacro = env;
		else
		{
			wDebug(("macro %s not found", (const char *)macName));
			expMacro = QString::null;
		}
	}
	else {
		expMacro = expandedValueOf( setting );
	}
	return expMacro;
}

//! call slotApply of all Setting objects in the list
/*!
  Will add lines to \a errMsg if any setting is invalid
*/
void
SettingList::applyAll( QString &errMsg )
{
	Q3PtrListIterator<Setting> it(*this);
	Setting *setting;

	while( (setting = it.current() )){
		++it;
		if( setting->enabled() == Setting::Enabled )
			setting->slotApply( errMsg );
	}
}

//----------------------------------------------------------------------

//! creates the configuration settings dialog without any tabs
SettingsDlg::SettingsDlg( QWidget *parent, Configuration *cfg ) :
	Q3TabDialog( parent, 0, true )
{
	config = cfg;
}

//! must be called after addTab() and before exec()
void
SettingsDlg::setButtons()
{
	setOkButton();
	setApplyButton();
	setCancelButton();
}




