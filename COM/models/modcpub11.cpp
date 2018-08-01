/***************************************************************************
 */
/*!       \file  modcpub11.cpp 
 *       \brief  Classes for model MEN B11
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2014/08/22 15:56:57 $
 *    $Revision: 2.1 $
 *
 *  
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpub11.cpp,v $
 * Revision 2.1  2014/08/22 15:56:57  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.0  2010/01/22 11:29:48  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.5  2004/06/08 17:26:13  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.4  2003/06/11 12:22:00  kp
 * intermediate checkin
 *
 * Revision 1.3  2003/06/05 09:35:21  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/06/03 10:18:01  kp
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:33:24  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include "modcpub11.h"
#include "wizdebug.h"
#include "resourcemng.h"
#include "util.h"

class ModVmeIfB11 : public VmeBusInterface 
{
public:
	ModVmeIfB11();
	virtual ulong supportedIrqLevels();
};

// -----------------------------------------------------------------
// CPU B11

//! Creates an B11 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the 
  BBIS devices
*/
ModCpuB11::ModCpuB11( bool withSubDevs ): 
	CpuDevice( QString(""), QString("B11"), false )
{
	BusInterface *busIf1, *busIf2;
	QString hwName = "B11";
	QString descr = "VME PowerPC 824x CPU with PC-MIP slots";

	UTIL_UNREF_PARAM(withSubDevs);

	setHwName( hwName );
	setDescription(descr);	

	lstCpuCores << Ppc603e;

	// Create the bus interfaces
	busIf1 = new ModVmeIfB11();					// VME interface
	addChild( busIf1 );
	busIf1->setInstName( QString("VMEbus"));

	// Interface for B11 BBISes	
	busIf2 = new PciBusInterface( LocalBus, -1, -1, 0, 0, false );
	addChild( busIf2 );
	busIf2->setEndianSwapping( true );

	// Interface for PC-MIPs
	{
		BusInterface *busIfMezz;
		Q3MemArray<uchar> pciBusPath(0);

		Q3MemArray<uchar> pciBusTbl(2);
		pciBusTbl[0] = 0x1a;     
		pciBusTbl[1] = 0x1d;  

		busIfMezz = new PciBusInterface( PcMip, 0, 1,
                          				   &pciBusPath, &pciBusTbl, true );
		busIfMezz->setInstName(QString("onboard PC-MIP"));
		busIfMezz->setEndianSwapping( true );
		addChild(busIfMezz);
	}
}

Device *ModCpuB11::create(bool withSubDevs)
{
	return new ModCpuB11(withSubDevs);
}

SwModuleList *
ModCpuB11::enumSwModules()
{	
	CpuDevice::enumSwModules();
	SwModuleList *addLst = enumSwModulesForExternalPackage( "PLDZ002" );

	if( lstSwModules == 0 && addLst )
		lstSwModules = new SwModuleList;

	if( lstSwModules && addLst )
		lstSwModules->add( addLst );

	return lstSwModules;
}

// -----------------------------------------------------------------
// VME Bus If 

ModVmeIfB11::ModVmeIfB11() : VmeBusInterface()
{
	vmeA16Res->addRegion( 0x0000, 0x10000 );
	vmeA24Res->addRegion( 0x000000, 0x1000000 );
	vmeIrqVectorRes->addRegion( 0x80, 0x7e );
}

ulong
ModVmeIfB11::supportedIrqLevels()
{
	return 0xfe; // level 7-1
}





