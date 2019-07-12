/***************************************************************************
 */
/*!       \file  modbbispcimmodproptab.cpp 
 *       \brief  Properties tab for PCI M-Module carriers
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2016/02/20 16:26:45 $
 *    $Revision: 2.10 $
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbispcimmodproptab.cpp,v $
 * Revision 2.10  2016/02/20 16:26:45  ts
 * R: BoxPCs based on SC24 need 2 SMBusses in MDIS wizard
 * M: 1. added member nrSmbBusses to class CpuDeviceSmb
 *    2. added parameter int nrSmbBusses to every constructor of CPUs with SMB
 *       controllers: 2 for SC24 (AMD based) Box PCs, 1 for every other CPU
 *
 * Revision 2.9  2014/08/22 15:56:33  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.8  2011/09/26 08:43:01  dpfeuffer
 * R: new bus interfaces required: CpciSer
 * M: modifications for CpciSer
 *
 * Revision 2.7  2011/02/15 11:37:37  CRuff
 * R: hint for compactPCI slot numbering needed in UI (Err&Imp entry)
 * M: added slot numbering description in slot label
 *
 * Revision 2.6  2010/07/15 08:57:52  CRuff
 * R: need version for kernel 2.6.34, but pci domains not fully implemented
 * M: hide the widgets that hold the pci domain
 *
 * Revision 2.5  2010/07/12 10:08:00  dpfeuffer
 * R: access violation in mdiswiz_lib.lib under Windows with MDIS_CLINST
 * M: QComboBox replaced with Q3ComboBox
 *
 * Revision 2.4  2010/07/06 08:26:19  CRuff
 * R: cosmetics
 * M: removed unused variable pciSlotLabel from ModBbisPciMmodPropTab constructor
 *
 * Revision 2.3  2010/07/05 18:16:17  CRuff
 * R: implementation of PCI Domains
 * M: 1. added widgets to ModBbisPciMmodPropTab for pciDomain handling
 *    2. added handling of pciDomain to setProperties() and applyProperties()
 *
 * Revision 2.2  2010/06/02 16:16:55  CRuff
 * R: changes from revision 2.1 not needed any longer
 * M: remove changes from revision 2.1
 *
 * Revision 2.1  2010/05/19 17:23:28  CRuff
 * R: 1. cascading the view items of chameleon BBIS and SMBPCI BBIS changes the
 *       given object hierarchy
 * M: 1. get the device properties of the current device instead of using the
 *       properties from MainDlg directly
 *
 * Revision 2.0  2010/01/22 11:29:24  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.5  2005/03/09 13:49:37  ub
 * cosmetics
 *
 * Revision 1.4  2004/06/08 17:26:07  kp
 * Initial Release for Linux 1.0.010-linux1
 *
 * Revision 1.3  2003/06/05 11:59:35  dschmidt
 * intermediate checkin
 *
 * Revision 1.2  2003/06/05 09:35:17  kp
 * intermediate checkin
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include <qlayout.h>
#include <q3vbox.h>
#include <qlabel.h>
#include <q3combobox.h>
#include <qspinbox.h>

#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>

#include "modbbispcimmodproptab.h"
#include "propertiesdlg.h"
#include "wizdebug.h"
#include "wizexcept.h"
#include "resourcemng.h"

ModBbisPciMmodPropTab::ModBbisPciMmodPropTab(
	QWidget *parent,
	DevPropertiesDlg *_mainDlg,
	const char *name, 
	Qt::WFlags f) : 
	DevPropertiesTab(parent,_mainDlg,name,f)
{
	PciBusInterface *busIf;

	int row = 0;
	wDebug(("ModBbisPciMmodPropTab::ModBbisPciMmodPropTab"));

	WIZ_DYNAMIC_CAST( (getMainDlg()->getDevice()->getParent()), busIf, 
					  PciBusInterface * );

	hasPciBusPath = busIf->hasPciBusPath();

	pciSlotCbox = 0;
	pciBusNoSpBox = 0;
	pciDevNoSpBox = 0;

	Q3VBoxLayout *vb = new Q3VBoxLayout( this, 10 );
	QLabel *lab = new QLabel( this );

	// Bus domain
	lab->setText( QString("Enter the %1 slot number of this "
						  "carrier board").
				  arg(busIf->busIfTypeMap.keyToStr(
					  busIf->getBusIfType())));
	vb->addWidget( lab );


	//Q3VBoxLayout *vb2 = new Q3VBoxLayout( vb );
	Q3GridLayout *gl = new Q3GridLayout( vb, 0, 2, 10);

	// add widgets for pci domain (only shown if necessary)

	QLabel* domLabel = new QLabel( QString("PCI Domain:"), this );
	gl->addWidget( domLabel, row, 0, Qt::AlignRight );
		
	pciDomNoSpBox = new QSpinBox( 0, 31, 1, this );
	gl->addWidget( pciDomNoSpBox, row, 1, Qt::AlignLeft );
	row++;	

	if( busIf->getPciDomainNoUnknown() == false ){
		domLabel->setHidden(true);
		pciDomNoSpBox->setHidden(true);
	}

	//
	// Bus interface bus path is known, simply ask for slot
	//
	QLabel *lab1 = new QLabel("Slot:", this );
	if ( busIf->getBusIfType() == HwComponent::Cpci ) {
		lab1->setText("Slot (System Slot is 1, asc.)");
	}

	gl->addWidget( lab1, row, 0, Qt::AlignLeft );
	pciSlotCbox = new Q3ComboBox( false, this );
	gl->addWidget( pciSlotCbox, row, 0, Qt::AlignRight );
	row++;


	pciBusSlotParameter = new QCheckBox(QString("Use BUS and DEVICE number "
			"instead of BUS_SLOT?"), this);

	gl->addWidget( pciBusSlotParameter, row, 0, Qt::AlignLeft );
	row++;

	// Bus interface bus path is unknown, ask for bus/device number
	pciBusNoInfo0 = new QLabel( QString(""), this );
	pciBusNoInfo0->setText( QString("Enter PCI settings: <font color=\"red\">use PCI bus nr. from 'lspci' command!</font>") );
	gl->addWidget( pciBusNoInfo0, row, 0, Qt::AlignLeft );
	row++;
	pciBusNoInfo1 = new QLabel( QString(""), this );
	pciBusNoInfo1->setText( QString("No validity check will be done") );
	gl->addWidget( pciBusNoInfo1, row, 0, Qt::AlignLeft );
	row++;

	// Bus number
	pciBusNoLabel = new QLabel( QString("PCI Bus Number:"), this );
	gl->addWidget( pciBusNoLabel, row, 0, Qt::AlignLeft );
	pciBusNoSpBox = new QSpinBox( 0, 255, 1, this );
	gl->addWidget( pciBusNoSpBox, row,  0, Qt::AlignCenter );
	row++;

	// Device number
	pciDevNoLabel = new QLabel( QString("PCI Device Number:"), this );
	gl->addWidget( pciDevNoLabel, row, 0, Qt::AlignLeft );
	pciDevNoSpBox = new QSpinBox( 0, 31, 1, this );
	gl->addWidget( pciDevNoSpBox, row, 0, Qt::AlignCenter);

	lab = new QLabel( QString(""), this );
	gl->addWidget( lab, row, 1, Qt::AlignLeft );

	// PCI device number of PCIe endpoints is always 0, grey it up
	if( busIf->getIsPciE() ){
		lab->setEnabled(false);
		pciDevNoSpBox->setEnabled(false);
	}

	// connect pciBusSlotParameter checkbox and
	connect(pciBusSlotParameter, SIGNAL(stateChanged(int)), this, SLOT(enablePciFields(int)));
	vb->addStretch( 10 );
}

void
ModBbisPciMmodPropTab::enablePciFields(int state)
{
	wDebug(("ModBbisPciMmodPropTab::enablePciFields"));
	if (state == QCheckBox::On) {
		wDebug(("pciSlotCbox->setEnabled(false);"));
		pciSlotCbox->setEnabled(false);
		pciBusNoInfo0->setHidden(false);
		pciBusNoInfo1->setHidden(false);
		pciBusNoLabel->setHidden(false);
		pciDevNoLabel->setHidden(false);
		pciBusNoSpBox->setHidden(false);
		pciDevNoSpBox->setHidden(false);
		pciBusNoSpBox->setEnabled(true);
		pciDevNoSpBox->setEnabled(true);
	} else {
		wDebug(("pciSlotCbox->setEnabled(true);"));
		pciSlotCbox->setEnabled(true);
		pciBusNoInfo0->setHidden(true);
		pciBusNoInfo1->setHidden(true);
		pciBusNoLabel->setHidden(true);
		pciDevNoLabel->setHidden(true);
		pciBusNoSpBox->setHidden(true);
		pciDevNoSpBox->setHidden(true);
		pciBusNoSpBox->setEnabled(false);
		pciDevNoSpBox->setEnabled(false);
	}
}

void
ModBbisPciMmodPropTab::updateProperties()
{
	ModBbisPciMmodProperties *_prop;

	wDebug(("ModBbisPciMmodPropTab::updateProperties"));


	WIZ_DYNAMIC_CAST( getMainDlg()->getProperties(), _prop, 
					  ModBbisPciMmodProperties *);

	_prop->useSlotNo=!pciBusSlotParameter->isChecked();
	_prop->slotNo = pciSlotCbox->currentText().toInt();
	_prop->pciBusNo = pciBusNoSpBox->value();
	_prop->pciDevNo = pciDevNoSpBox->value();

	if ( _prop->useSlotNo ) {
		_prop->pciBusNoIsDef = false;
		_prop->pciDevNoIsDef = false;
	}
	else {
		_prop->pciBusNoIsDef = true;
		_prop->pciDevNoIsDef = true;
	}

	_prop->pciDomainNo = pciDomNoSpBox->value();

}

void
ModBbisPciMmodPropTab::setProperties()
{
	ModBbisPciMmodProperties *_prop;

	WIZ_DYNAMIC_CAST( (getMainDlg()->getProperties()), _prop, 
					  ModBbisPciMmodProperties *);
					  
	PciBusInterface *busIf;
	WIZ_DYNAMIC_CAST( (getMainDlg()->getDevice()->getParent()), busIf, 
						PciBusInterface * );

	pciBusSlotParameter->setChecked( !_prop->useSlotNo );

	while( pciSlotCbox->count() )	// remove old items
		pciSlotCbox->removeItem(0);

	for( int slt=0; slt<100; slt++ ){
		HwComponent *owner;
		if( slt==_prop->slotNo ||
			busIf->pciSlots->isAvail( slt, 1, &owner ) ==
			ResourceMng::Avail ){
			wDebug(("isAVail %d", slt ));
			pciSlotCbox->insertItem(QString("%1").arg(slt));
		}
	}

	pciSlotCbox->setCurrentText(QString("%1").arg(_prop->slotNo ));

	pciBusNoSpBox->setValue( _prop->pciBusNo );
	pciDevNoSpBox->setValue( _prop->pciDevNo );

	if ( _prop->useSlotNo ) {
		pciSlotCbox->setEnabled(true);
		pciBusNoSpBox->setEnabled(false);
		pciDevNoSpBox->setEnabled(false);
		pciBusNoInfo0->setHidden(true);
		pciBusNoInfo1->setHidden(true);
		pciBusNoLabel->setHidden(true);
		pciDevNoLabel->setHidden(true);
		pciBusNoSpBox->setHidden(true);
		pciDevNoSpBox->setHidden(true);
	}
	else {
		pciSlotCbox->setEnabled(false);
		pciBusNoSpBox->setEnabled(true);
		pciDevNoSpBox->setEnabled(true);
		pciBusNoInfo0->setHidden(false);
		pciBusNoInfo1->setHidden(false);
		pciBusNoLabel->setHidden(false);
		pciDevNoLabel->setHidden(false);
		pciBusNoSpBox->setHidden(false);
		pciDevNoSpBox->setHidden(false);
	}

	if (busIf && (_prop->pciDomainNo == 0) ) {
    	_prop->pciDomainNo = busIf->getPciDomainNo();
    }
    	
	pciDomNoSpBox->setValue( _prop->pciDomainNo );
	
	wDebug(("ModBbisPciMmodPropTab::setProperties leave"));
}
