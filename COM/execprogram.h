/***************************************************************************
 */
/*!       \file  execprogram.h
 *       \brief  Widget to show output of external process
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:26:04 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: execprogram.h,v $
 * Revision 2.0  2010/01/22 11:26:04  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.2  2008/02/06 11:36:07  cs
 * R: reason of build termination showed always "finished"
 * M: add ExecProgram::finishReason which can be set by OS specific functions
 *
 * Revision 1.1  2003/05/02 14:34:40  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:20:05  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef EXECPROGRAM_H
#define EXECPROGRAM_H

#include <qdialog.h>
#include <q3process.h>

class Q3TextEdit;
class QPushButton;

//! Widget to show output of external process
/*!

  This Qt widget opens a dialog, runs an external program and displays
  it's output into the widget.

  To start the program, call start() with the program name and it's
  arguments. Currently, you cannot pass anything to stdin of the program.

  All output is displayed in a QTextEdit widget. Stdout output is displayed
  in black while stderr output is displayed in red.

  User has to close the dialog with the "Close" button.

  When the user has closed the dialog, the finished() signal is emitted. You
  can then check for the exit status using status().

  Typical usage:
  \code
		QStringList args = "make";

		buildDlg = new ExecProgram(
			currentCfg()->msgBoxCaption(),
			"Building Project (make)",
			this,
			"buildDlg",
			false);
		buildDlg->show();

		buildDlg->start( args );
  \endcode
*/

class ExecProgram : public QDialog {
	Q_OBJECT
	friend class OsVxworksExecProgram;
public:
	ExecProgram( const QString & caption,
				 const QString & text,
				 QWidget * parent = 0,
				 const char * name = 0,
				 bool modal = false,
				 Qt::WFlags f = 0);
	~ExecProgram();

	bool start( const QStringList & args );

	int status();
	QString finishReason;

signals:
	//! signal is emitted when user pressed the "Close" button
	void finished();

protected slots:
    void done( int r );

private slots:
    void slotReadStdout();
    void slotReadStderr();
	void slotProcessExited();
	virtual void slotCancel();
private:
	Q3Process *proc;
	Q3TextEdit *output;
	QPushButton *okBut, *cancelBut;
};


#endif



