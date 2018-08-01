/***************************************************************************
 */
/*!       \file  modcpua12.cpp
 *       \brief  Classes for model MEN A12 and derivates
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2014/08/22 15:56:40 $
 *    $Revision: 2.3 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modcpua12.cpp,v $
 * Revision 2.3  2014/08/22 15:56:40  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.2  2014/07/18 15:12:19  dpfeuffer
 * R: erroneous PMC support, missing ComExpress support, general maintenance
 * M: intermediate check-in during development
 *
 * Revision 2.1  2010/05/25 17:41:20  CRuff
 * R: wrong device numbers for onboard pmc devices
 * M: fixed device numbers for Pmc Bus Interface
 *
 * Revision 2.0  2010/01/22 11:29:38  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.11  2009/04/21 15:13:05  CRuff
 * R: support mezzanine slots (Pmc, PCMip)
 * M: ModCpuA12::ModCpuA12(): added Bus Interface for mezzanine slots with
 *    pci bus path
 *
 * Revision 1.10  2006/11/30 13:08:21  cs
 * added include file modbbisd203fam.h
 *
 * Revision 1.9  2004/08/24 09:45:32  kp
 * removed crazy wdebug
 *
 * Revision 1.8  2004/07/30 11:43:24  ufranke
 * added
 *  + busIf 1 name VME , busIf 2 name PCI local
 *
 * Revision 1.7  2004/06/08 17:26:11  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.6  2004/01/09 08:20:52  kp
 * intermediate checkin
 *
 * Revision 1.5  2003/06/11 12:21:55  kp
 * intermediate checkin
 *
 * Revision 1.4  2003/06/06 13:32:31  kp
 * cosmetics
 *
 * Revision 1.3  2003/06/05 09:35:19  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/06/03 10:17:59  kp
 * Initial Revision
 *
 * Revision 1.1  2003/05/02 14:33:22  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <Q3MemArray>

#include "modcpua12.h"
#include "wizdebug.h"
#include "resourcemng.h"
#include "modbbispcimmod.h"
#include "modbbisd203fam.h"
#include "util.h"

class ModVmeIfA12 : public VmeBusInterface
{
public:
	ModVmeIfA12();
	virtual ulong supportedIrqLevels();
};

class ModVmeIfA15 : public VmeBusInterface
{
public:
	ModVmeIfA15();
	virtual ulong supportedIrqLevels();
};

// -----------------------------------------------------------------
// CPU A12

//! Creates an A12 CPU device
/*!
  including the bus interfaces and, if \a withSubDevs is \c true, the
  BBIS devices
*/
ModCpuA12::ModCpuA12(SubModel submod,
					 Mezzanines mezzanines,
					 ExtensionBoard extensionBoard,
					 bool withSubDevs ):
	CpuDevice( QString(""), QString("A12"), false )
{
	BusInterface *busIf1, *busIf2, *busIf3=0;
	BusInterface *busIfObPCI1=0;
	BbisDevice *bbis1=0;
	QString hwName;
	QString descr;

	_submod = submod;
	_mezzanines = mezzanines;
	_extensionBoard = extensionBoard;

	lstCpuCores << Ppc603e;

	switch( _submod ){
	case A12: 	hwName += "A12"; 	descr = "VME "; break;
	case A15: 	hwName += "A15"; 	descr = "VME "; break;
	case D3: 	hwName += "D3"; 	descr = "CompactPCI "; break;
	case SC13: 	hwName += "SC13"; 	descr = "Single Board "; break;
	case KahluaBox: hwName += "KAHLUA_BOX"; descr = "Kahlua Box "; break;
	}

	descr += "PowerPC 824x CPU ";

	if( _submod == KahluaBox ){
		switch( _mezzanines ){
		case WithPcMips: 	hwName += "_PCMIP"; break;
		case WithMmods: 	hwName += "_MMOD"; break;
		case WithPmcs: 		hwName += "_PMC"; break;
		}
	}
	else {
		// A12, A15, D3, SC13
		switch( _mezzanines ){
		case WithPcMips: 	hwName += "a"; break;
		case WithMmods: 	hwName += "b"; break;
		case WithPmcs: 		hwName += "c"; break;
		}
	}

	switch( _mezzanines ){
	case WithPcMips: 	descr += "with 3 PC-MIP slots"; break;
	case WithMmods: 	descr += "with 3 M-Module slots"; break;
	case WithPmcs: 		descr += "with 2 PMC slots"; break;
	}

	switch( _extensionBoard ){
	case D201:
		hwName += "_D201"; descr += " and 4 M-Module slots on D201"; break;
	case D203:
		hwName += "_D203"; descr += " and 4 M-Module slots on D203"; break;
	default: break;
	}
	setHwName( hwName );
	setDescription(descr);

	// Create the bus interfaces
	if( _submod == A12 ) {
		busIf1 = new ModVmeIfA12();					// VME interface
		busIf1->setInstName( QString("VME"));
		addChild( busIf1 );
	}
	if( _submod == A15 ) {
		busIf1 = new ModVmeIfA15();					// VME interface
		busIf1->setInstName( QString("VME"));
		addChild( busIf1 );
	}

	{	// Interface for A12 BBISes
		Q3MemArray<uchar> pciBusPath(0);

		busIf2 = new PciBusInterface( LocalBus, 0, 0, 0x1d, &pciBusPath,
									  false );
		busIf2->setInstName( QString("PCI local"));
		busIf2->setEndianSwapping( true );
		addChild( busIf2 );
	}

	if( _submod == D3 || _submod == KahluaBox ){
		Q3MemArray<uchar> cpciBusPath(1);
		cpciBusPath[0] = 0x1e;

        // Interface for CompactPci
		// only slot 5 available on KahluaBox
		if( _submod == KahluaBox )
			busIf3 = new PciBusInterface( Cpci, 5, 5, 0xc, &cpciBusPath, true);
		else
			busIf3 = new PciBusInterface( Cpci, 2, 8, 0xf, &cpciBusPath, true);

		busIf3->setInstName( QString("CompactPci Bus"));
		busIf3->setEndianSwapping( true );
		addChild( busIf3 );
	}

	switch( _mezzanines ){
	case WithPcMips:
		{
			Q3MemArray<uchar> pciBusPath(1);
			pciBusPath[0] = 0x1d; // same for both board types
			Q3MemArray<uchar> pciBusTbl(3);
			
			switch( _submod ){
			case A12: 	
			case D3: 	
			case SC13: 
			case KahluaBox: 
				pciBusTbl[0] = 16;	// todo: or 0?           
    			pciBusTbl[1] = 17;	// todo: or 1?                 
    			pciBusTbl[2] = 18;	// todo: or 2?
				break;
		
			case A15: 	
				pciBusTbl[0] = 18;	// todo: or 0?            
    			pciBusTbl[1] = 17;	// todo: or 1?                  
    			pciBusTbl[2] = 16;	// todo: or 2? 			
				break;
				
			default:
				break;
			}
    		
			busIfObPCI1 = new PciBusInterface( PcMip, 0, 2,
                                  			   &pciBusPath, &pciBusTbl, true );
			busIfObPCI1->setInstName(QString("onboard PC-MIP"));
			busIfObPCI1->setEndianSwapping( true );
			addChild(busIfObPCI1);
			
			break;
		}
	case WithPmcs:
		{
			Q3MemArray<uchar> pciBusPath(1);
			pciBusPath[0] = 0x1d;        
			
			Q3MemArray<uchar> pciBusTbl(2);
			pciBusTbl[0] = 0x3;     
    		pciBusTbl[1] = 0x4;  

			busIfObPCI1 = new PciBusInterface( Pmc, 0, 1, &pciBusPath, &pciBusTbl, true );
			busIfObPCI1->setInstName(QString("onboard PMC"));
			busIfObPCI1->setEndianSwapping( true );
			addChild(busIfObPCI1);
			
			break;
		}
	case WithMmods:
		{
			break; // no pci interface for m-modules
		}
	}
	
	if( withSubDevs ){
		if( _mezzanines == WithMmods ){
			// Create the local BBISes
			bbis1 = new ModBbisA12();					// MMod-Bbis
			busIf2->addChild( bbis1 );
		}

		// Kahlua Box extension board
		if( _extensionBoard != NoExtension && busIf3 ){
			ModBbisPciMmod *extBbis = 0;

			if( _extensionBoard == D201 )
				extBbis = new ModBbisD201();
			else if( _extensionBoard == D203 )
				extBbis = new ModBbisD203Family( ModBbisD203Family::D203 );

			WIZ_ASSERT(extBbis);
			extBbis->setSlotNo( 5 );
			busIf3->addChild( extBbis );
		}
	}


}

Device *ModCpuA12::create(bool withSubDevs)
{
	return new ModCpuA12(_submod, _mezzanines, _extensionBoard, withSubDevs);
}

SwModuleList *
ModCpuA12::enumSwModules()
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
// BBIS A12

//! Creates an A12 BBIS device
/*!
  if \a withSubDevs is \c true, it also creates the QSPI MDIS device
*/
ModBbisA12::ModBbisA12(bool withSubDevs) :
	BbisDevice( QString("A12_OBMMODS"),
				QString("A12"), false, LocalBus )
{
	BbisSlot *bbSlot;
	//MdisDevice *mdDev;
	UTIL_UNREF_PARAM(withSubDevs);

	setInstName( QString("OB_MMODS") );
	setDescription("Pseudo device for onboard M-Modules slots");

	// create slots
	for( int slotNum=0; slotNum<3; slotNum++ ){
		bbSlot = new BbisSlot( slotNum, MmodD32, MmodA24 );
		addChild( bbSlot );
	}

	bbSlot = new BbisSlot( LocalBus, 0x1000 );	// QSPI
	bbSlot->setSlotName( "pseudo slot for QSPI" );
	bbSlot->setBbSlotSpecRequired( true );
	addChild( bbSlot );
}

SwModuleList *ModBbisA12::enumSwModules()
{
	if( lstSwModules == 0 ){
		lstSwModules = new SwModuleList;

		// always ignore byteswapping (to generate driver.mak)
		lstSwModules->add( makeStdDriverSwModule("A12", "", true ));
	}
	return lstSwModules;
}

//! as Device::getDriverName, but always returns non-swapped variant
QString
ModBbisA12::getDriverName( bool fullName, bool withSw)
{
	UTIL_UNREF_PARAM(withSw);

	return Device::getDriverName( fullName, false );
}

// -----------------------------------------------------------------
// VME Bus If A12

ModVmeIfA12::ModVmeIfA12() : VmeBusInterface()
{
	vmeA16Res->addRegion( 0x0000, 0x10000 );
	vmeA24Res->addRegion( 0x000000, 0x1000000 );
	vmeIrqVectorRes->addRegion( 0x80, 0x7e );
	setInstName( "VMEbus" );
}

ulong
ModVmeIfA12::supportedIrqLevels()
{
	return 0xfe; // level 7-1
}

// -----------------------------------------------------------------
// VME Bus If A15

ModVmeIfA15::ModVmeIfA15() : VmeBusInterface()
{
	vmeA16Res->addRegion( 0x0000, 0x10000 );
	vmeA24Res->addRegion( 0x000000, 0x1000000 );
	vmeA32Res->addRegion( 0x00000000, 0xffffffff );
	vmeIrqVectorRes->addRegion( 0x80, 0x7e );
	setInstName( "VMEbus" );
}

ulong
ModVmeIfA15::supportedIrqLevels()
{
	return 0xfe; // level 7-1
}






