/***************************************************************************
 */
/*!       \file  modelfactory.cpp
 *       \brief  CPU, BBIS and MDIS model factory
 *      \author  Klaus.Popp@men.de
 *        $Date: 2018/06/20 11:34:57 $
 *    $Revision: 2.17 $
 *
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modelfactory.cpp,v $
 * Revision 2.17  2018/06/20 11:34:57  dpfeuffer
 * R: F26L introduction
 * M: add F26L support (implementation merged from M.Moese Git commit)
 *
 * Revision 2.16  2016/02/20 16:27:25  ts
 * R: BoxPCs based on SC24 need 2 SMBusses in MDIS wizard
 * M: 1. added member nrSmbBusses to class CpuDeviceSmb
 *    2. added parameter int nrSmbBusses to every constructor of CPUs with SMB
 *       controllers: 2 for SC24 (AMD based) Box PCs, 1 for every other CPU
 *
 * Revision 2.15  2015/05/26 16:44:15  ts
 * R: gituser autocheckin: updated source
 *
 * Revision 2.14  2014/08/22 15:56:09  dpfeuffer
 * R:1. CC10, CB30, CB70 introduction
 *   2. some unnecessary models
 * M:1. CC10, CB30, CB70 models added
 *   3. useless models removed
 *
 * Revision 2.13  2014/07/18 15:11:57  dpfeuffer
 * R: erroneous PMC support, missing ComExpress support, general maintenance
 * M: intermediate check-in during development
 *
 * Revision 2.12  2014/06/12 15:26:10  ts
 * R: new CPU F22P could not be selected
 * M: added new model F22P to modelfactory
 *
 * Revision 2.11  2014/06/06 14:14:43  ts
 * R: new CPUs G22 and F75P were missing as a choice
 * M: added them to model factory
 *
 * Revision 2.10  2013/11/27 10:34:10  ts
 * R: F213 PMC carrier was not available in the wizard
 * M: added F213 class
 *
 * Revision 2.9  2013/09/27 08:19:12  dpfeuffer
 * R: G204 support
 * M: G204 and G204_A24 models added
 *
 * Revision 2.8  2013/03/04 13:02:47  dpfeuffer
 * R: A21 support
 * M: A21 models added
 *
 * Revision 2.7  2012/12/20 13:18:27  ts
 * R: new cPCI card F223 was introduced
 * M: added Instance of ModBBISPciGen as F223
 *
 * Revision 2.6  2011/09/28 09:07:19  dpfeuffer
 * R: wrong CPU board name F50
 * M: F50 replaced by F50P
 *
 * Revision 2.5  2011/09/26 08:42:28  dpfeuffer
 * R: new models: COMPACTPCI_SERIAL_PC, COMPACTPCI_PLUSIO_PC, F19P, F21P, G20,
 *     SC24, F207, G215
 * M: new model classes added
 *
 * Revision 2.4  2011/02/14 13:52:27  CRuff
 * R: 1a) new models ModBbisSmbIsaGen and ModBbisChamIsa
 *    1b) model ModBbisSmbPciGen needed in IO variant
 * M: 1. added instances to Bbis model factory
 *    1a) ModBbisSmbIsaGen (variant 16Z001), ModBbisSmbIsaGen (16Z001_IO),
 *        ModBbisChamIsa
 *    1b) ModBbisSmbPciGen ( variant IO )
 *
 * Revision 2.3  2010/06/02 17:05:58  CRuff
 * R: support new models
 * M: added models ModCpuXm1, ModCpuXm2, ModCpuXm50, ModCpuF50, ModCpuF11S,
 *    ModCpuMm1, ModCpuMm50 and ModExtF503
 *
 * Revision 2.2  2010/05/20 15:45:25  CRuff
 * R: generic PCI BBIS needed for Windows (err & imp entry 77)
 * M: add generic PCI BBIS model to model list
 *
 * Revision 2.1  2010/05/19 15:25:47  CRuff
 * R: 1. moved ModCpuA19 and ModCpuA20 to new file
 *    2. different smb controllers are implemented by different variants
 * M: 1. added include of modcpua19.h
 *    2. for each variant of ModBbisSmbPciGen, add an instance to Bbis factory
 *
 * Revision 2.0  2010/01/22 11:26:40  dpfeuffer
 * R: ModBbisSmbPciZ001 class not fully implemented
 * M: commented out: append( new ModBbisSmbPciZ001());
 *
 * Revision 1.19  2009/09/03 10:56:25  CRuff
 * R: preparation for SMB support
 * M: added new BBIS models ModBbisSmbPciGen and ModBbisSmbPciZ001
 *
 * Revision 1.18  2009/08/14 15:11:08  CRuff
 * R: no support for P512
 * M: add support for P512 PMC
 *
 * Revision 1.17  2009/04/21 16:44:53  CRuff
 * R: P18 not supported in this version
 * M: remove P18
 *
 * Revision 1.16  2009/04/21 15:33:58  CRuff
 * R: support A17, A19, A20, D7, D9, F19P, F215, F216, P599, P506, P507
 * M: added A17, A19, A20, D7, D9, F19P, F215, F216, P599, P506, P507
 *    and supporting mezzanine bbisses
 *
 * Revision 1.15  2007/10/05 16:27:59  cs
 * added:
 *   + EM1N/A, EM9, F12N, F18
 *
 * Revision 1.14  2006/11/30 11:27:44  cs
 * added:
 *   + cpu models A13, A14, D6, EM7N, EM8,
 *                F11, F11N, F12, F13, F14, F15, F17, F500
 *   + BBIS models D203_A24, F204_A24, F205_A24, F208, F210, D6
 *
 * Revision 1.13  2006/08/04 11:50:07  ub
 * added parameter irqNumber to ModBbisMscan5200
 *
 * Revision 1.12  2006/07/27 13:43:25  ub
 * Added: Models for PP1 and Mscan5200
 *
 * Revision 1.11  2005/12/07 16:55:14  ub
 * added model for PP1 BBIS
 *
 * Revision 1.10  2005/08/02 11:20:39  dpfeuffer
 * added A203N
 *
 * Revision 1.9  2005/07/15 15:47:09  ub
 * added models for EM1, EM2, EM3, EM5, EM7, PP1, F9, EC01N, F206, F502
 *
 * Revision 1.8  2005/03/09 13:48:37  ub
 * new model "ModBbisF206"
 *
 * Revision 1.7  2004/06/08 17:25:36  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.6  2004/05/14 17:14:26  UFranke
 * added
 *  + F6 CPU
 *
 * Revision 1.5  2004/01/09 08:20:06  kp
 * added F2/F7
 *
 * Revision 1.4  2003/06/11 12:21:39  kp
 * intermediate checkin
 *
 * Revision 1.3  2003/06/05 09:33:36  kp
 * intermediate checkin
 *
 * Revision 1.2  2003/06/03 10:16:23  kp
 * intermediate checkin
 *
 * Revision 1.1  2003/05/02 14:36:08  dschmidt
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
// Implementation of modelfactory classes

#include "modelfactory.h"
#include "wizdebug.h"
#include "modbbisa201.h"
#include "modbbisa203n.h"
#include "modbbischam.h"
#include "modbbisisa.h"
#include "modbbispcigen.h"
#include "modbbispcimmod.h"
#include "modbbisd203fam.h"
#include "modcpua11.h"
#include "modcpua12.h"
#include "modcpua17.h"
#include "modcpua19.h"
#include "modcpua21.h"
#include "modcpub11.h"
#include "modcpud2.h"
#include "modcpud4.h"
#include "modcpud6.h"
#include "modcpuesm.h"
#include "modcpupp1.h"
#include "modcpuf1n.h"
#include "modcpuf2.h"
#include "modcpuf6.h"
#include "modcpuf7.h"
#include "modcpuf9.h"
#include "modcpuf14.h"
#include "modcpuf19p.h"
#include "modcpug20.h"
#include "modcpucpcipc.h"
#include "modcpucpciserpc.h"
#include "modcpupc.h"
#include "modexta500.h"
#include "modextec01.h"
#include "modextf501.h"
#include "modmdisgeneric.h"
#include "modbbissmbpcigen.h"
#include "modbbissmbisagen.h"
#include "modbbischamisa.h"
#include "modcpuesmexpr.h"
#include "modextf503.h"
#include "modcpuesmini.h"
#include "modcpusc24.h"
#include "modcpucomexpr.h"
#include "modextcomexpr.h"

// -----------------------------------------------------------------
// ModelFactory

//! creates an empty ModelFactory
ModelFactory::ModelFactory()
{
    lstModels.setAutoDelete(true);
}

//! get first model or 0 if no first model
/*!
  \sa next()
*/
Device *ModelFactory::first()
{
    return lstModels.first();
}

//! get next model or 0 if no next model
/*!
  \sa first()
*/
Device *ModelFactory::next()
{
    return lstModels.next();
}

//! append \a dev to the list of models
/*!
  If a model of the same model name already exists, it is replaced with the
  new one.
*/
void ModelFactory::append( Device *dev )
{
    Device *oldDev;

    if( (oldDev = find( dev->getModelName() )) != 0 )
        lstModels.remove( oldDev );

    lstModels.append( dev );
}

//! find model by model name
/*!
  It compares \a modelName to every model's Device::getModelName().
  \return pointer to device or 0 if not found
*/
Device *ModelFactory::find( const QString &modelName )
{
    for( Device *dev=first(); dev; dev=next())
        if( dev->getModelName() == modelName )
            return dev;
    return 0;
}

// -----------------------------------------------------------------
// CpuModelFactory

//! create the CpuModelFactory
CpuModelFactory::CpuModelFactory() : ModelFactory()
{
    wDebug(("CpuModelFactory"));
    // add all known CPUs here
    append( new ModCpuPc());
    append( new ModCpuCpciPc());
    append( new ModCpuCpciPcPlusIo());
    append( new ModCpuCpciSerPc());

    append( new ModCpuA11( false ));

    append( new ModCpuA12( ModCpuA12::A12, ModCpuA12::WithPcMips,
                                     ModCpuA12::NoExtension, false ));
    append( new ModCpuA12( ModCpuA12::A12, ModCpuA12::WithMmods,
                                     ModCpuA12::NoExtension, false ));
    append( new ModCpuA12( ModCpuA12::A12, ModCpuA12::WithPmcs,
                                     ModCpuA12::NoExtension, false ));
    append( new ModCpuA12( ModCpuA12::D3, ModCpuA12::WithPcMips,
                                     ModCpuA12::NoExtension, false ));
    append( new ModCpuA12( ModCpuA12::D3, ModCpuA12::WithMmods,
                                     ModCpuA12::NoExtension, false ));
    append( new ModCpuA12( ModCpuA12::D3, ModCpuA12::WithPmcs,
                                     ModCpuA12::NoExtension, false ));
    append( new ModCpuA12( ModCpuA12::SC13, ModCpuA12::WithPcMips,
                                     ModCpuA12::NoExtension, false ));
    append( new ModCpuA12( ModCpuA12::SC13, ModCpuA12::WithMmods,
                                     ModCpuA12::NoExtension, false ));
    append( new ModCpuA12( ModCpuA12::SC13, ModCpuA12::WithPmcs,
                                     ModCpuA12::NoExtension, false ));
    append( new ModCpuA12( ModCpuA12::A15, ModCpuA12::WithPcMips,
                                     ModCpuA12::NoExtension, false ));
    append( new ModCpuA12( ModCpuA12::A15, ModCpuA12::WithMmods,
                                     ModCpuA12::NoExtension, false ));
    append( new ModCpuA12( ModCpuA12::A15, ModCpuA12::WithPmcs,
                                     ModCpuA12::NoExtension, false ));
    append( new ModCpuA12( ModCpuA12::KahluaBox,
                           ModCpuA12::WithMmods,
                           ModCpuA12::NoExtension, false ));
    append( new ModCpuA12( ModCpuA12::KahluaBox,
                           ModCpuA12::WithPcMips,
                           ModCpuA12::NoExtension, false ));
    append( new ModCpuA12( ModCpuA12::KahluaBox,
                           ModCpuA12::WithPmcs,
                           ModCpuA12::NoExtension, false ));

    append( new ModCpuA13( ModExtA500::WithPmcs, false ));
    append( new ModCpuA13( ModExtA500::WithPcMips, false ));
    append( new ModCpuA14( ModExtA500::WithPmcs, false ));
    append( new ModCpuA14( ModExtA500::WithPcMips, false ));
    append( new ModCpuA17( false ));
    append( new ModCpuA19( false ));
    append( new ModCpuA20( false ));
    append( new ModCpuA21( ModCpuA21::A21Msi, ModCpuA21::WithMmods, false ));
    append( new ModCpuA21( ModCpuA21::A21, ModCpuA21::WithMmods, false ));
    append( new ModCpuA21( ModCpuA21::A21, ModCpuA21::WithPmcs, false ));
    append( new ModCpuB11(false ));
    append( new ModCpuD2(false) );
    append( new ModCpuD4( ModCpuD4::WithPmcs, false) );
    append( new ModCpuD4( ModCpuD4::WithPcMips, false) );
    append( new ModCpuD6( false) );
    append( new ModCpuD7( false) );
    append( new ModCpuD9( false) );
    append( new ModCpuEm1( false ));
    append( new ModCpuEm1N(true));
    append( new ModCpuEm1A(true));
    append( new ModCpuEm2(false));
    append( new ModCpuEm3(false));
    append( new ModCpuEm04(false));
    append( new ModCpuEm5(false));
    append( new ModCpuEm7(false));
    append( new ModCpuEm7N(true));
    append( new ModCpuEm8(false));
    append( new ModCpuEm9(false));
    append( new ModCpuPp1(false));
    append( new ModCpuF1N(false));
    append( new ModCpuF2(false));
    append( new ModCpuF6(false));
    append( new ModCpuF7(false));
    append( new ModCpuF9(false));
    append( new ModCpuF11(false));
    append( new ModCpuF11N(false));
    append( new ModCpuF11S(false));
    append( new ModCpuF12(false));
    append( new ModCpuF12N(false));
    append( new ModCpuF13(false));
    append( new ModCpuF14(false));
    append( new ModCpuF15(false));
    append( new ModCpuF17(false));
    append( new ModCpuF18(false));
    append( new ModCpuF19P(false));
    append( new ModCpuF21P(false));
    append( new ModCpuF22P(false));
    append( new ModCpuF23P(false));
    append( new ModCpuF26L(false));
    append( new ModCpuF27P(false));
    append( new ModCpuF50P(false));
    append( new ModCpuF75P(false));
    append( new ModCpuG20(false));
    append( new ModCpuG22(false));
    append( new ModCpuG23(false));
    append( new ModCpuG25A(false));
    append( new ModCpuMm1(false));
    append( new ModCpuMm50(false));
    append( new ModCpuXm1(false));
    append( new ModCpuXm2(false));
    append( new ModCpuXm50(false));
    append( new ModCpuBx50x(false));
    append( new ModCpuDc15(false));
    append( new ModCpuBx70x(false));
    append( new ModCpuCb30c(false));
    append( new ModCpuCb70c(false));
    append( new ModCpuCc10( ModCpuCc10::CC10, false));
    append( new ModCpuCc10( ModCpuCc10::CC10C, false));

    // extension boards
    append( new ModExtA500( ModExtA500::WithPcMips ));
    append( new ModExtA500( ModExtA500::WithPmcs ));
    append( new ModExtEc01());
    append( new ModExtEc01n());
    append( new ModExtF500());
    append( new ModExtF501());
    append( new ModExtF502());
    append( new ModExtF503(false));
    append( new ModExtComExpr(false));
}

// -----------------------------------------------------------------
// BbisModelFactory

//! create the BbisModelFactory
BbisModelFactory::BbisModelFactory() : ModelFactory()
{
    wDebug(("BbisModelFactory"));
    // generic pci bbis
    append( new ModBbisPciGen()); 
    // add all known Carrier Boards here
    append( new ModBbisA201());
    append( new ModBbisA203N());
    append( new ModBbisA302());
    append( new ModBbisB201());
    append( new ModBbisB202());
    append( new ModBbisC203());
    append( new ModBbisC204());
    append( new ModBbisD201());
    append( new ModBbisD202());
    append( new ModBbisD203Family( ModBbisD203Family::D203 ));
    append( new ModBbisD203Family( ModBbisD203Family::D203_A24 ));
    append( new ModBbisD203Family( ModBbisD203Family::F204 ));
    append( new ModBbisD203Family( ModBbisD203Family::F204_A24 ));
    append( new ModBbisD203Family( ModBbisD203Family::F205 ));
    append( new ModBbisD203Family( ModBbisD203Family::F205_A24 ));
    append( new ModBbisD203Family( ModBbisD203Family::G204, HwComponent::CpciSer ));
    append( new ModBbisD203Family( ModBbisD203Family::G204_A24, HwComponent::CpciSer ));
    append( new ModBbisD302());
    append( new ModBbisF201());
    append( new ModBbisF202());
    append( new ModBbisF203());
    append( new ModBbisF207());
    append( new ModBbisF223());

    append( new ModBbisCham4Mezz());

    // onboard BBIS
    append( new ModBbisA11(false));
    append( new ModBbisA12(false));
    append( new ModBbisA21(false));
    append( new ModBbisPci4Mezz(false));
    append( new ModBbisPp1ObPci104(false));

    append( new ModBbisIsa( false )); // MEM
    append( new ModBbisIsa( true ));  // IO

    append( new ModBbisD2(false));

    append( new ModBbisF2(false));
    append( new ModBbisF7(false));

    append( new ModBbisMscan5200("unknown", 0, -1));

    append( new ModBbisCham());
    append( new ModBbisCham("PCITBL"));
    //ts@men  these are customerspecific BBIS used only for MSI A21 variant (Mahr)
    append( new ModBbisCham("PCITBL_MSI"));
    append( new ModBbisCham("PCITBL_MSI_ENABLE"));
    append( new ModBbisChamIsa() );

    append( new ModBbisSmbPciGen("ICH"));
    append( new ModBbisSmbPciGen("FCH"));
    append( new ModBbisSmbPciGen("SCH"));
    append( new ModBbisSmbPciGen("16Z001"));
    append( new ModBbisSmbPciGen("16Z001", true));
    append( new ModBbisSmbPciGen("NAT"));

    append( new ModBbisSmbIsaGen("16Z001"));
    append( new ModBbisSmbIsaGen("16Z001", true));
}

// -----------------------------------------------------------------
// MdisModelFactory

//! create the MdisModelFactory
MdisModelFactory::MdisModelFactory() : ModelFactory()
{
    // add all known Device Models here
}
