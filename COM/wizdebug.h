/***************************************************************************
 */
/*!       \file  wizdebug.h
 *       \brief  Debug macros 
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:28:19 $
 *    $Revision: 2.0 $
 *
 *     Switches: WIZDEBUG
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: wizdebug.h,v $
 * Revision 2.0  2010/01/22 11:28:19  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.4  2004/01/09 08:20:40  kp
 * intermediate checkin
 *
 * Revision 1.3  2003/06/06 13:32:29  kp
 * wDebug( _x_ ) must use arg
 *
 * Revision 1.2  2003/06/05 12:01:34  dschmidt
 * intermediate checkin
 *
 * Revision 1.1  2003/06/05 09:34:19  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef WIZDEBUG_H
#define WIZDEBUG_H

#ifdef WIZDEBUG
# define wDebug( _x_ ) qDebug _x_
#else
//! Debug output macro
/*!
 if \c WIZDEBUG switch is set at compilation time, outputs the arguments
 using qDebug(). If \c WIZDEBUG is not defined, this macro is a no-op
*/
# define wDebug( _x_ )
#endif

#endif
