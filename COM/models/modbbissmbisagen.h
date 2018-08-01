/***************************************************************************
 */
/*!       \file  modbbissmbisagen.h
 *       \brief  Generic class for SMB ISA BBIS
 *      \author	 Christine.Ruff@men.de
 *		  $Date: 2011/02/14 14:56:38 $
 *    $Revision: 2.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbissmbisagen.h,v $
 * Revision 2.1  2011/02/14 14:56:38  CRuff
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODBBISSMBISAGEN_H
#define MODBBISSMBISAGEN_H

#include <Q3MemArray>

#include "hwcomponent.h"
#include "descriptor.h"


//! Device properties for ModBbisSmbPciGen 
class ModBbisSmbIsaGenProperties : public DeviceProperties
{
public:
	int				deviceAddress;  //!< device address where the controller resides>
	int 			smBusNo;	    //!< SMBus number>
	int 			smbPollAlertFreq; //!< SMB alert polling frequency>
	int 			smbBusyWait;

#ifdef WIN_MDISWIZ_LIB
    QStringList     smbDevNameList;
#endif
	
};

//! class for generic SMB PCI BBIS devices
class ModBbisSmbIsaGen : public BbisDevice
{
public:
	ModBbisSmbIsaGen( const QString &variant=0, bool isIo=false );
	~ModBbisSmbIsaGen();
	
	virtual Device *create(bool withSubDevs=true);

    QString getMainProperties();
    SwModuleList* enumSwModules();
        
    void createSpecialDesc( DescriptorEntryDirectory *parentDesc );    
    ApplyResult applyProperties( DescriptorEntryDirectory *devDesc,
										 QString &errMsg );
	ApplyResult applyProperties( DeviceProperties *newProp, 
	                                     QString &errMsg );
	                                     
	QString getDriverName( bool fullName, bool withSw );
	 
	QString variant; 
	bool isIo;
	
protected:	
	
	virtual DeviceProperties *createProperties( void);
	virtual void copyProperties( const DeviceProperties *from,
								 DeviceProperties *to );
	virtual void freeResources();	                                   
};


#endif

