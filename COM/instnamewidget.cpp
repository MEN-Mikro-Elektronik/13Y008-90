/***************************************************************************
 */
/*!       \file  instnamewidget.cpp 
 *       \brief  A Qt widget to enter instance names of HwComponents
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:26:23 $
 *    $Revision: 2.0 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: instnamewidget.cpp,v $
 * Revision 2.0  2010/01/22 11:26:23  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.3  2009/12/07 09:09:42  CRuff
 * R: SMB devices have long default device names
 * M: increase max string length from 12 to 15
 *
 * Revision 1.2  2003/06/05 09:33:28  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:34:46  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

#include "instnamewidget.h"

//! Create an InstNameWidget
InstNameWidget::InstNameWidget( QWidget *parent, const char *name) :
	QLineEdit( parent, name )
{
	setMaxLength(15);

}
