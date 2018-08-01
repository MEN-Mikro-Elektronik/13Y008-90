/***************************************************************************
 */
/*!       \file  modbbisd203fam.h
 *       \brief  BBIS base class for D203 family PCI M-Module carriers
 *      \author	 Christian.Schuster@men.de
 *		  $Date: 2013/09/27 08:19:19 $
 *    $Revision: 2.1 $
 *
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbisd203fam.h,v $
 * Revision 2.1  2013/09/27 08:19:19  dpfeuffer
 * R: G204 support
 * M: modifications for G204 in ModBbisD203Family
 *
 * Revision 2.0  2010/01/22 11:28:57  dpfeuffer
 * R: copyright wrong
 * M: copyright fixed
 *
 * Revision 1.2  2007/02/23 12:20:37  DPfeuffer
 * ModBbisD203Family::createPropDlg4Win prototype added
 *
 * Revision 1.1  2006/11/30 11:28:16  cs
 * Initial Revision
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2006-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#ifndef MODBBISD203_H
#define MODBBISD203_H

#include "hwcomponent.h"
#include "descriptor.h"

class ResourceMng;

//! Device properties for D203 family
class ModBbisD203FamProperties : public ModBbisPciMmodProperties
{
public:
	int pxiTrigSrc, pxiTrigDst;
	int trigASrc[4], trigADst[4];
	int trigBSrc[4], trigBDst[4];

	int idCheck;
};

//! BBIS class for MEN D203 family
class ModBbisD203Family : public ModBbisPciMmod
{
public:
	enum Model { D203, F204, F205, G204, D203_A24, F204_A24, F205_A24, G204_A24 };
	ModBbisD203Family( Model model, BusIfType ifType=Cpci );
	virtual ApplyResult applyProperties( DeviceProperties *newProp,
										 QString &errMsg );
	virtual ApplyResult applyProperties( DescriptorEntryDirectory *devDesc,
										 QString &errMsg );
	// create another instance
	virtual Device *create(bool withSubDevs=true);
	//virtual QString getMainProperties();
	void createSpecialDesc( DescriptorEntryDirectory *parentDesc );
	SwModuleList *enumSwModules();
	virtual ~ModBbisD203Family();
protected:
	virtual DevPropertiesDlg *createPropDlg( QWidget *parent );
	virtual DevPropertiesDlg *createPropDlg4Win( QWidget *parent );
	virtual DeviceProperties *createProperties();
	virtual void copyProperties( const DeviceProperties *from,
								 DeviceProperties *to );
	//virtual void freeResources();

	virtual void createSlots();
	virtual int getSlots(){ return _numSlots; };
	int getDescMmSlot( DescriptorEntry * );
	Model _model;
	int _numSlots;
};


#endif /* MODBBISD203_H */










