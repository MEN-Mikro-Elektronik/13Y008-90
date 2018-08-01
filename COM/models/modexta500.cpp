/***************************************************************************
 */
/*!       \file  modexta500.cpp
 *       \brief  Implementation of A500
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2014/08/22 15:57:33 $
 *    $Revision: 2.1 $
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modexta500.cpp,v $
 * Revision 2.1  2014/08/22 15:57:33  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.0  2010/01/22 11:30:53  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.2  2006/11/30 11:28:35  cs
 * added:
 *   - support for combined CPUs:
 *     create all Interfaces/BBISes in dedicated function
 *
 * Revision 1.1  2004/06/08 17:26:56  kp
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>

#include "modexta500.h"
#include "wizdebug.h"
#include "wizexcept.h"
#include "resourcemng.h"
#include "util.h"

// -----------------------------------------------------------------
//! Specialized PciBusInterface for A500 PCI slots
/*!
  overrides pciBusPath to get the bus path from the ESM PCI bus
*/
class A500PciIf : public PciBusInterface {
public:
	A500PciIf(int _minSlot, int _maxSlot,
		Q3MemArray<uchar> *pciPath,
		Q3MemArray<uchar> *_directIdTbl) :
	  PciBusInterface( PciOrCham, _minSlot, _maxSlot, pciPath, _directIdTbl, true )
	{
  	}

	//! determine PCI path from the ESM CPU board
	bool pciBusPath( int slotNo, Q3MemArray<uchar> *pciPath )
	{
		// only generate pciPath if not already set from base CPU
		if( !basePciPath.size() ){
			HwComponent *parent;
			PciBusInterface *cpuIf;

			parent = getParent();			// goto ModExtA500
			WIZ_ASSERT(parent);

			parent = parent->getParent();	// goto CPU busIf
			WIZ_ASSERT(parent);

			WIZ_DYNAMIC_CAST( parent, cpuIf, PciBusInterface * );

			// get buspath up to PMC/PCMIP bridge (connected to ESM IDSEL0)
			if( cpuIf->pciBusPath( 0, &basePciPath ) == false )
				return false;
		}

		return PciBusInterface::pciBusPath( slotNo, pciPath );
	}
};

class ModVmeIfA500 : public VmeBusInterface
{
public:
	ModVmeIfA500(){
		vmeA16Res->addRegion( 0x0000, 0x10000 );
		vmeA24Res->addRegion( 0x000000, 0x1000000 );
		vmeA32Res->addRegion( 0x00000000, 0xffffffff );
		vmeIrqVectorRes->addRegion( 0x80, 0x7e );
		setInstName( "VMEbus" );
	}

	virtual ulong supportedIrqLevels() {
		return 0xfe; // level 7-1
	}

	//! the VME driver always presents the VME interface in non-swapped mode
	virtual bool isEndianSwapping() {
		return false;
	}
};

// -----------------------------------------------------------------
//! Add A500 Interfaces and BBISes
extern void
ModExtA500IfsCreate( HwComponent *parentComp,
					 QString &hwName,
					 QString &descr,
					 ModExtA500::Mezzanines mezzanines,
					 Q3MemArray<uchar> *pciPath,
					 bool withSubDevs)
{
	BusInterface *busIf2;

	UTIL_UNREF_PARAM(withSubDevs);

	switch(mezzanines ){
	
	case ModExtA500::WithPcMips:
	{
		hwName += "a";
		descr += "with 3 PC-MIP slots";

		Q3MemArray<uchar> pciBusTbl(3);
		pciBusTbl[0] = 0xf;     
   		pciBusTbl[1] = 0xe;  
   		pciBusTbl[2] = 0xd;  

		busIf2 = new A500PciIf( 0, 2, pciPath, &pciBusTbl );	// for PCM/PCMIP slots
		busIf2->setInstName(QString("onboard PC-MIP"));
	}
		break;

	case ModExtA500::WithPmcs:
	{
		hwName += "c";
		descr += "with 2 PMC slots";

		Q3MemArray<uchar> pciBusTbl(2);
		pciBusTbl[0] = 0xc;     
   		pciBusTbl[1] = 0xb;  

		busIf2 = new A500PciIf( 0, 1, pciPath, &pciBusTbl );	// for PCM/PCMIP slots
		busIf2->setInstName(QString("onboard PMC"));

	}
		break;
	}


	BusInterface *busIf1 = new ModVmeIfA500();	// VME interface
	parentComp->addChild( busIf1 );

	parentComp->addChild( busIf2 );
}


// -----------------------------------------------------------------
// A500

//! Create an A500  device
/*!

*/
ModExtA500::ModExtA500(Mezzanines mezzanines, bool withSubDevs) :
	EsmCarrier( "A500")
{
	QString hwName = QString( "A500" );
	QString descr  = QString( "VME 6U Embedded system module carrier " );

	ModExtA500IfsCreate( this, hwName, descr, mezzanines, 0, withSubDevs);

	_mezzanines = mezzanines;

	setHwName( hwName );
	setDescription( descr );

}


Device *ModExtA500::create(bool withSubDevs)
{
	return new ModExtA500( _mezzanines, withSubDevs );
}


SwModuleList *
ModExtA500::enumSwModules()
{
	CpuExtDevice::enumSwModules();
	SwModuleList *addLst = enumSwModulesForExternalPackage( "PLDZ002" );

	if( lstSwModules == 0 && addLst )
		lstSwModules = new SwModuleList;

	if( lstSwModules && addLst )
		lstSwModules->add( addLst );

	return lstSwModules;
}


