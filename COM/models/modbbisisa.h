/***************************************************************************
 */
/*!       \file  modbbisisa.h
 *       \brief  BBIS class for the ISA devices
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2010/01/22 11:29:07 $
 *    $Revision: 2.0 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbisisa.h,v $
 * Revision 2.0  2010/01/22 11:29:07  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.4  2008/10/23 18:05:08  channoyer
 * R: During build we get an error: extra qualification
 * M: Remove extra qualification
 *
 * Revision 1.3  2006/08/04 11:50:11  ub
 * added property "irqNumber" to ModBbisIsa for use in ModBbisMscan5200
 *
 * Revision 1.2  2003/06/05 09:35:06  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/06/03 10:17:48  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODBBISISA_H
#define MODBBISISA_H

#include "hwcomponent.h"
#include "descriptor.h"

class ResourceMng;

//! Device properties for ISA family 
class ModBbisIsaProperties : public DeviceProperties
{
public:
	ulong devAddr;		//!< device address (IO or mem)
	ulong devAddrSize; 	//!< device address size in bytes
	long  irqNumber;    //!< number of IRQ (used in Mscan5200 BBIS)
};

//! BBIS class for ISA
class ModBbisIsa : public BbisDevice
{
public:
	ModBbisIsa( bool isIo);

	virtual ~ModBbisIsa();
	Device *create(bool withSubDevs);

	virtual ApplyResult applyProperties( DeviceProperties *newProp, 
										 QString &errMsg );
	virtual ApplyResult applyProperties( DescriptorEntryDirectory *devDesc,
										 QString &errMsg );
	virtual QString getMainProperties(); 
	void createSpecialDesc( DescriptorEntryDirectory *parentDesc );
	SwModuleList *enumSwModules();

	// ISA special calls
	void setIsaAddress( ulong devAddr, ulong devAddrSize );
	void setIrqNumber( long irqNumber );

protected:	
	//virtual DevPropertiesDlg *createPropDlg( QWidget *parent );
	virtual DeviceProperties *createProperties();
	virtual void copyProperties( const DeviceProperties *from,
								 DeviceProperties *to );
	virtual void freeResources();
	bool _isIo;			//!< true if address is in IO space
};


#endif


