/***************************************************************************
 */
/*!       \file  execprogram.cpp
 *       \brief  Implementation of ExecProgram
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:26:01 $
 *    $Revision: 2.0 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: execprogram.cpp,v $
 * Revision 2.0  2010/01/22 11:26:01  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.6  2008/02/06 11:35:13  cs
 * R: reason of build termination showed always "finished"
 * M: handle ExecProgram::finishReason to also show "cancled" ...
 *
 * Revision 1.5  2004/06/08 17:25:17  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.4  2003/06/05 12:00:53  dschmidt
 * intermediate checkin
 *
 * Revision 1.3  2003/06/05 09:33:18  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/06/03 10:16:04  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:34:39  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#include <qapplication.h>
#include <qstyle.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <q3textedit.h>
#include <qfont.h>

#include <Q3HBoxLayout>
#include <Q3VBoxLayout>

#include "wizdebug.h"
#include "execprogram.h"
#include "util.h"

//! Creates the ExecProgram Dialog
/*!
  \param caption 		title text
  \param text	 		text to display above textview
  \param parent			parent widget (passed to QDialog)
  \param name			internal name (passed to QDialog)
  \param modal			true if modal dialog (passed to QDialog)
  \param f				window flags (passed to QDialog)
*/
ExecProgram::ExecProgram(
	const QString & caption,
	const QString & text,
	QWidget * parent,
	const char * name,
	bool modal,
	Qt::WFlags f ) :

	QDialog( parent, name, modal, f | Qt::WStyle_Customize |
			 Qt::WStyle_DialogBorder | Qt::WStyle_Title | Qt::WStyle_SysMenu)
{
	Q3VBoxLayout *vb = new Q3VBoxLayout( this, 10 );
	QFont font("Courier", 10 );

	UTIL_UNREF_PARAM(caption);

	// create the QProcess
	proc = new Q3Process( this, "proc" );
	connect( proc, SIGNAL(readyReadStdout()),
			 this, SLOT(slotReadStdout()));
	connect( proc, SIGNAL(readyReadStderr()),
			 this, SLOT(slotReadStderr()));
	connect( proc, SIGNAL(processExited()),
			 this, SLOT(slotProcessExited()));

	// create text label
	QLabel *label = new QLabel( text, this, "text" );
	vb->addWidget( label );

	// create text edit widget
	output = new Q3TextEdit( this, "output" );
	output->setReadOnly( true );
	output->setMinimumWidth(600);
	output->setMinimumHeight(200);
	output->setFont( font );
	output->setTextFormat( Qt::PlainText );
	vb->addWidget( output );

	Q3HBoxLayout *butHb = new Q3HBoxLayout();
	vb->addLayout( butHb );

	// create edit field
	cmdText = new QLineEdit();
	butHb->addWidget( cmdText );
	// create buttons
	sendCmd = new QPushButton("Send", this );
	butHb->addWidget( sendCmd );
	sendCmd->setEnabled( true );
	connect( sendCmd, SIGNAL(clicked()), this, SLOT(slotSendCmd()));

	okBut = new QPushButton("Close", this );
	butHb->addStretch(1);
	butHb->addWidget( okBut );
	okBut->setEnabled( false );
	connect( okBut, SIGNAL(clicked()), this, SLOT(accept()));

	cancelBut = new QPushButton("Cancel", this );
	butHb->addWidget( cancelBut );
	cancelBut->setEnabled( false );
	connect( cancelBut, SIGNAL(clicked()), this, SLOT(slotCancel()));

}

//! destructs the EcexProgram dialog
/*!
  You should not call it while the dialog is opened...
*/
ExecProgram::~ExecProgram()
{
	if( proc->isRunning())
		proc->tryTerminate();
}

//! starts the program
/*!

Tries to run a process for the command and arguments that were
specified with \a args. The command is searched in the path for executable
programs; you can also use an absolute path to the command.

The program's working directory is always the current directory.

If start of the program fails, an error message is written into the
text view window and user must acknowledge it.

\return TRUE if the process could be started, otherwise FALSE.
\sa processExited
*/

bool
ExecProgram::start( const QStringList & args )
{
	proc->setArguments( args );

	finishReason = QString( "finished" );
	if (proc->start() == false ){
		output->setColor( Qt::red );
		output->append( QString("Cannot start process: ") + args[0] + "\n" );
		okBut->setEnabled( true );
		return false;
	}
	cancelBut->setEnabled( true );
	return true;
}

//! get exit status of the running program
/*!
  \returns -1=still running, -2=killed or crashed >=0 exit status

  The result is undefined when the program hasn't been started yet
*/
int
ExecProgram::status()
{
	if( proc->isRunning() )
		return -1;
	if( !proc->normalExit() )
		return -2;
	return proc->exitStatus();
}


//! called when process terminates
void
ExecProgram::slotProcessExited()
{
	wDebug(("ExecProgram::slotProcessExited"));
	//slotReadStdout();
	//slotReadStderr();

	output->setColor( Qt::blue );
	output->insert( proc->arguments().join(" ") + " " + finishReason);

	okBut->setEnabled( true );
	cancelBut->setEnabled( false );
	cmdText->setEnabled( false );
	sendCmd->setEnabled( false );

}

//! called when user attempts to close the window
void
ExecProgram::done( int r )
{
	wDebug(("ExecProgram::done"));
	if( proc->isRunning() )
		return;
	QDialog::done(r);
	emit finished();
}

//! read the processes stdout
void
ExecProgram::slotReadStdout()
{
	output->setColor( Qt::black );
	output->insert( proc->readStdout() );
}

//! read the processes stderr
void
ExecProgram::slotReadStderr()
{
	output->setColor( Qt::red );
	output->insert( proc->readStderr() );
}

//! kill the program
void
ExecProgram::slotCancel()
{
	proc->tryTerminate();
	finishReason = QString( "canceled");
}

//! send data into script
void
ExecProgram::slotSendCmd()
{
	QString cmdTextQString=cmdText->text();
	cmdTextQString += "\n";
	proc->writeToStdin( cmdTextQString );
	cmdText->clear();
}


