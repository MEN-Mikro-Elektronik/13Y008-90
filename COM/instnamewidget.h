/***************************************************************************
 */
/*!       \file  instnamewidget.h
 *       \brief  A Qt widget to enter instance names of HwComponents
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:26:25 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: instnamewidget.h,v $
 * Revision 2.0  2010/01/22 11:26:25  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.1  2003/05/02 14:34:47  dschmidt
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:20:12  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef INSTNAME_WIDGET_H
#define INSTNAME_WIDGET_H

#include <qlineedit.h>


//! A Qt widget to enter instance names of HwComponents
/*!
  \todo has almost no functionality
*/
class InstNameWidget : public QLineEdit {
public:
	InstNameWidget( QWidget *parent, const char *name = 0 );
private:
};

#endif
