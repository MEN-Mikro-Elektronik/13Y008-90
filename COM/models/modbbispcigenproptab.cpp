/***************************************************************************
 */
/*!       \file  modbbispcigenproptab.cpp
 *       \brief  Properties tab for PCI/PCI carriers
 *      \author	 Klaus.Popp@men.de
 *		  $Date: 2016/02/20 16:26:43 $
 *    $Revision: 2.9 $
 *
 *      Project: mdiswiz
 *     Switches: -
 */
/*-------------------------------[ History ]---------------------------------
 *
 * $Log: modbbispcigenproptab.cpp,v $
 * Revision 2.9  2016/02/20 16:26:43  ts
 * R: BoxPCs based on SC24 need 2 SMBusses in MDIS wizard
 * M: 1. added member nrSmbBusses to class CpuDeviceSmb
 *    2. added parameter int nrSmbBusses to every constructor of CPUs with SMB
 *       controllers: 2 for SC24 (AMD based) Box PCs, 1 for every other CPU
 *
 * Revision 2.8  2014/08/22 15:56:26  dpfeuffer
 * R: inconsistent PC-MIP/PMC/PCI104/XMC and Chameleon usage
 * M: PC-MIP/PMC/PCI104/XMC and Chameleon usage completely revised
 *
 * Revision 2.7  2014/07/18 15:12:17  dpfeuffer
 * R: first slot must be 0 for PMCs
 * M: ModBbisPciGenPropTab::setProperties(): "for( int slt=1; .." changed to slt=0
 *
 * Revision 2.6  2011/02/15 11:37:14  CRuff
 * R: hint for compactPCI slot numbering needed in UI (Err&Imp entry)
 * M: added slot numbering description in slot label
 *
 * Revision 2.5  2010/07/15 08:57:50  CRuff
 * R: need version for kernel 2.6.34, but pci domains not fully implemented
 * M: hide the widgets that hold the pci domain
 *
 * Revision 2.4  2010/07/12 10:07:48  dpfeuffer
 * R: access violation in mdiswiz_lib.lib under Windows with MDIS_CLINST
 * M: QComboBox replaced with Q3ComboBox
 *
 * Revision 2.3  2010/07/05 18:09:33  CRuff
 * R: implementation of PCI Domains
 * M: 1. added widgets to ModBbisPciGenPropTab and ModBbisSmbPciGenPropTab
 *       for pciDomain handling
 *    2. added handling of pciDomain to setProperties() and applyProperties()
 *       in both classes
 *
 * Revision 2.2  2010/06/16 14:40:23  CRuff
 * R: handling of SMB_ALERT_POLL_FREQ and SMB_BUSY_WAIT descriptor keys needed
 * M: added handling for descriptor keys in ModBbisSmbPciGenPropTab methods
 *
 * Revision 2.1  2010/05/19 17:15:12  CRuff
 * R: 1. cosmetics
 *    2. continuing implementation of ModBbisSmbPciGen
 *    3. handle the linux variant as similar as possible to the general variant
 * M: 1a) change labels on ModBbisSmbPciGenPropTab
 *    2a) remove widgets for PCI settings from ModBbisSmbPciGenPropTab; these are
 *        not set by the user
 *    2b) hide bar and offset labels if device is not of type Z001
 *    2c) do not handle pci settings in setProperties() and updateProperties();
 *        they are never set by the user
 *    3. use ModBbisSmbPciGenProperties in class ModBbisSmbPciLinux and therefore
 *       in ModBbisSmbPciLinuxPropTab
 *
 * Revision 2.0  2010/01/22 11:29:15  dpfeuffer
 * R: Qt 3.3.1 doesn't support 64-bit build
 *    required for Windows MDIS_CLINST DLL.
 * M: MDIS Wizard ported to Qt 4.6
 *
 * Revision 1.5  2009/12/07 10:23:15  CRuff
 * R: Implementation not finished, but interrupted
 * M: Intermediate checkin
 *
 * Revision 1.4  2009/09/03 11:07:17  CRuff
 * R: preparation for SMB support
 * M: added properties implementation for ModBbisSmbPciGen and ModBbisSmbPciZ001
 *
 * Revision 1.3  2003/06/05 11:59:31  dschmidt
 * intermediate checkin
 *
 * Revision 1.2  2003/06/05 09:35:10  kp
 * intermediate checkin
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003-2010 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/
#include "modbbispcigenproptab.h"
#include <qlayout.h>
#include <q3vbox.h>
#include <qlabel.h>
#include <q3combobox.h>
#include <qspinbox.h>

#include <Q3HBoxLayout>
#include <Q3GridLayout>
#include <Q3VBoxLayout>
#include <Q3MemArray>

#include "propertiesdlg.h"
#include "wizdebug.h"
#include "wizexcept.h"
#include "resourcemng.h"
#include "modmdisgeneric.h"
#include "modbbissmbpcigen.h"

ModBbisPciGenPropTab::ModBbisPciGenPropTab(
	QWidget *parent,
	DevPropertiesDlg *_mainDlg,
	const char *name,
	Qt::WFlags f) :
	DevPropertiesTab(parent,_mainDlg,name,f)
{
	PciBusInterface *busIf;
	int row = 0;
	wDebug(("ModBbisPciGenPropTab::ModBbisPciGenPropTab"));
	
	WIZ_DYNAMIC_CAST( (getMainDlg()->getDevice()->getParent()), busIf,
					  PciBusInterface * );

	hasPciBusPath = busIf->hasPciBusPath();

	pciSlotCbox = 0;
	pciBusNoSpBox = 0;
	pciDevNoSpBox = 0;

	Q3VBoxLayout *vb = new Q3VBoxLayout( this, 10 );
	QLabel *lab = new QLabel( this );
	vb->addWidget( lab );
	
	Q3GridLayout *gl = new Q3GridLayout( vb, 3, 2 );
	vb->addLayout( gl );

	// Bus domain

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

	if( hasPciBusPath ){

		//
		// Bus interface bus path is known, simply ask for slot
		//
		lab->setText( QString("Enter the %1 slot number of this "
							  "carrier board").
					  arg(busIf->busIfTypeMap.keyToStr(
						  busIf->getBusIfType())));
		
		QLabel *lab1 = new QLabel("Slot:", this );
		if ( busIf->getBusIfType() == HwComponent::Cpci ) {
			lab1->setText("Slot (System Slot is 1, asc.)");
		}
		gl->addWidget( lab1, row, 0, Qt::AlignRight );
		
		pciSlotCbox = new Q3ComboBox( false, this );
		gl->addWidget( pciSlotCbox, row, 1, Qt::AlignLeft );
		row ++;

		// only one slot? ->grey it up
		if( busIf->getNbrOfSlots() == 1 ){
			lab1->setEnabled(false);
			pciSlotCbox->setEnabled(false);
		}
	}
	else {
		//
		// Bus interface bus path is unknown, ask for bus/device number
		//
		// QString("<font color=\"red\">Enter PCI bus number shown by 'lspci' command for this carrier!\n</font>)
		lab = new QLabel( QString("<font color=\"red\">Enter PCI bus nr. shown by 'lspci' command for this carrier!\n</font>"), this );
		vb->addWidget( lab );

		// Bus number
		lab = new QLabel( QString("PCI Bus Number:"), this );
		gl->addWidget( lab, row, 0, Qt::AlignRight );

		pciBusNoSpBox = new QSpinBox( 0, 255, 1, this );
		gl->addWidget( pciBusNoSpBox, row, 1, Qt::AlignLeft );
		row++;

		// Device number
		lab = new QLabel( QString("PCI Device Number:"), this );	
		gl->addWidget( lab, row, 0, Qt::AlignRight );

		pciDevNoSpBox = new QSpinBox( 0, 31, 1, this );
		gl->addWidget( pciDevNoSpBox, row, 1, Qt::AlignLeft );
		row++;	

		// PCI device number of PCIe endpoints is always 0, grey it up
		if( busIf->getIsPciE() ){
			lab->setEnabled(false);
			pciDevNoSpBox->setEnabled(false);
		}
	}
}

void
ModBbisPciGenPropTab::updateProperties()
{
	ModBbisPciGenProperties *_prop;
	WIZ_DYNAMIC_CAST( getMainDlg()->getProperties(), _prop,
					  ModBbisPciGenProperties *);

	if( hasPciBusPath ){
		_prop->slotNo = pciSlotCbox->currentText().toInt();
	}
	else{
		_prop->pciBusNo = pciBusNoSpBox->value();
		_prop->pciDevNo = pciDevNoSpBox->value();
	}
		
	_prop->pciDomainNo = pciDomNoSpBox->value();
}

void
ModBbisPciGenPropTab::setProperties()
{
	ModBbisPciGenProperties *_prop;
	WIZ_DYNAMIC_CAST( (getMainDlg()->getProperties()), _prop,
					  ModBbisPciGenProperties *);
					  
	PciBusInterface *busIf;
		WIZ_DYNAMIC_CAST( (getMainDlg()->getDevice()->getParent()), busIf,
						  PciBusInterface * );
	if( hasPciBusPath ){
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
	}
	else{
		pciBusNoSpBox->setValue( _prop->pciBusNo );
		pciDevNoSpBox->setValue( _prop->pciDevNo );
	}
	
	if (busIf && (_prop->pciDomainNo == 0) )
    	_prop->pciDomainNo = busIf->getPciDomainNo();

	pciDomNoSpBox->setValue( _prop->pciDomainNo );

}


ModBbisSmbPciGenPropTab::ModBbisSmbPciGenPropTab(
	QWidget *parent,
	DevPropertiesDlg *_mainDlg,
	const char *name,
	Qt::WFlags f) :
	DevPropertiesTab(parent,_mainDlg,name,f)
{
	pciBusPathLineEdit = 0;
	pciDevNoLineEdit = 0;
	pciFuncNoSpBox = 0;
	smBusNoSpBox = 0;
	barNoSpBox = 0;
	offsetLineEdit = 0;
	busyWaitLineEdit = 0;
	alertPollLineEdit = 0;
	ModBbisSmbPciGen* smbBbis = 0;

	Q3VBoxLayout *vb = new Q3VBoxLayout( this, 10 );
	QLabel *lab = new QLabel( this );
	vb->addWidget( lab );

	//
	// ask for PCI bus/device/function number
	//
	lab->setText( QString("Enter the settings for the SM bus "
						  "(No validity checks will be done!)" ));

	Q3GridLayout *gl = new Q3GridLayout( vb, 7, 2 );
	int row = 0;

	lab = new QLabel( QString("SMB Bus Number:"), this );
	gl->addWidget( lab, row, 0, Qt::AlignRight );

    smBusNoSpBox = new QSpinBox( 0, 255, 1, this );
	gl->addWidget( smBusNoSpBox, row, 1, Qt::AlignLeft );
	row++;
	
	lab = new QLabel( QString("Pci Domain Number:"), this );
	gl->addWidget( lab, row, 0, Qt::AlignRight );
	lab->setHidden(true);
	
    pciDomNoSpBox = new QSpinBox( 0, 31, 1, this );
	gl->addWidget( pciDomNoSpBox, row, 1, Qt::AlignLeft );
	row++;
	pciDomNoSpBox->setHidden(true);

	QLabel* barLab = new QLabel( QString("Bar:"), this );
	gl->addWidget( barLab, row, 0, Qt::AlignRight );

    barNoSpBox = new QSpinBox( -1, 5, 1, this );
	gl->addWidget( barNoSpBox, row, 1, Qt::AlignLeft );
	row++;

	QLabel* offsLab = new QLabel( QString("Offset (hex):"), this );
	gl->addWidget( offsLab, row, 0, Qt::AlignRight );

	offsetLineEdit = new QLineEdit("", this );
	gl->addWidget( offsetLineEdit, row, 1, Qt::AlignLeft );
	row++;
	
	// only show bar / offset widgets for z001 controllers
	WIZ_DYNAMIC_CAST( getMainDlg()->getDevice(), smbBbis, ModBbisSmbPciGen* );
	if (!(smbBbis->variant.contains("Z001")))
	{
		barLab->setHidden(true);
		barNoSpBox->setHidden(true);
		offsLab->setHidden(true);
		offsetLineEdit->setHidden(true);
	}
	
	QLabel* busyWaitLab = new QLabel( QString("Busy Wait Time (ms):"), this );
	gl->addWidget( busyWaitLab, row, 0, Qt::AlignRight );
		
	busyWaitLineEdit = new QLineEdit("100", this );
	gl->addWidget( busyWaitLineEdit, row, 1, Qt::AlignLeft );
	row++;
	
	QLabel* alertPollLab = new QLabel( QString("Alert Polling Frequency:"), this );
	gl->addWidget( alertPollLab, row, 0, Qt::AlignRight );
		
	alertPollLineEdit = new QLineEdit("0", this );
	gl->addWidget( alertPollLineEdit, row, 1, Qt::AlignLeft );
}

void
ModBbisSmbPciGenPropTab::setProperties()
{
	ModBbisSmbPciGenProperties *_prop;
	WIZ_DYNAMIC_CAST( (getMainDlg()->getProperties()), _prop,
					  ModBbisSmbPciGenProperties *);
					  
	// set the bus number
	smBusNoSpBox->setValue( _prop->smBusNo );
	pciDomNoSpBox->setValue(_prop->pciDomainNo );

	barNoSpBox->setValue( _prop->bar );
	offsetLineEdit->setText( QString::number(_prop->offs, 16) );
	busyWaitLineEdit->setText( QString::number(_prop->smbBusyWait) );
	alertPollLineEdit->setText( QString::number(_prop->smbPollAlertFreq) );
}

void
ModBbisSmbPciGenPropTab::updateProperties()
{
	
	ModBbisSmbPciGenProperties *_prop;
	wDebug(("ModBbisSmbPciGenPropTab::updateProperties"));
	WIZ_DYNAMIC_CAST( getMainDlg()->getProperties(), _prop,
					  ModBbisSmbPciGenProperties *);

	_prop->smBusNo = smBusNoSpBox->value();
	_prop->pciDomainNo = pciDomNoSpBox->value();

	// also set the smBusNo of the children
	Device* childDev;
	Q3PtrList<HwComponent> children = getMainDlg()->getDevice()->getChildren();
	for ( Q3PtrListStdIterator<HwComponent> it = children.begin(); it != children.end(); ++it ) {
		HwComponent* child = *it;
		if (child->firstChild() != 0) {
			WIZ_DYNAMIC_CAST( child->firstChild(), childDev, Device * );
			ModDescriptorProperties* newDevProp;
			WIZ_DYNAMIC_CAST( childDev->getProp(), newDevProp, ModDescriptorProperties * );
			DescriptorEntryDirectory* desc = &newDevProp->desc;

			DescriptorEntryUint32* smBusDesc = (DescriptorEntryUint32*)desc->find( "SMB_BUSNBR", DescriptorEntry::Uint32 );
			smBusDesc->setValue( _prop->smBusNo );
		}
	}

	_prop->bar = barNoSpBox->value();

	QString offsetString = offsetLineEdit->text();
	_prop->offs = offsetString.toInt(0, 16);
	
	QString busyWaitString = busyWaitLineEdit->text();
	_prop->smbBusyWait = busyWaitString.toInt();
	
	QString pollAlertString = alertPollLineEdit->text();
	_prop->smbPollAlertFreq = pollAlertString.toInt();
}


ModBbisSmbPciLinuxPropTab::ModBbisSmbPciLinuxPropTab(
	QWidget *parent,
	DevPropertiesDlg *_mainDlg,
	const char *name,
	Qt::WFlags f) :
	DevPropertiesTab(parent,_mainDlg,name,f)
{
	smBusNoSpBox = 0;

	Q3VBoxLayout *vb = new Q3VBoxLayout( this, 10 );
	QLabel *lab = new QLabel( this );
	vb->addWidget( lab );

	//
	// ask for PCI bus/device/function number
	//
	lab->setText( QString("Enter the settings for the SM bus "
						  "(No validity checks will be done!)" ));

	Q3GridLayout *gl = new Q3GridLayout( vb, 1, 2 );
	int row = 0;

	lab = new QLabel( QString("SMB Bus Number:"), this );
	gl->addWidget( lab, row, 0, Qt::AlignRight );

    smBusNoSpBox = new QSpinBox( 0, 255, 1, this );
	gl->addWidget( smBusNoSpBox, row, 1, Qt::AlignLeft );
	row++;

}

void
ModBbisSmbPciLinuxPropTab::setProperties()
{
	ModBbisSmbPciGenProperties *_prop;
	WIZ_DYNAMIC_CAST( (getMainDlg()->getProperties()), _prop,
					  ModBbisSmbPciGenProperties *);

	// create the bus path string
	smBusNoSpBox->setValue( _prop->smBusNo );

}

void
ModBbisSmbPciLinuxPropTab::updateProperties()
{
	ModBbisSmbPciGenProperties *_prop;
	wDebug(("ModBbisSmbPciLinuxPropTab::updateProperties"));
	WIZ_DYNAMIC_CAST( getMainDlg()->getProperties(), _prop,
					  ModBbisSmbPciGenProperties *);

	_prop->smBusNo = smBusNoSpBox->value();

	// also set the smBusNo of the children
	Device* childDev;
	Q3PtrList<HwComponent> children = getMainDlg()->getDevice()->getChildren();
	for ( Q3PtrListStdIterator<HwComponent> it = children.begin(); it != children.end(); ++it ) {
		HwComponent* child = *it;
		if (child->firstChild() != 0) {
			WIZ_DYNAMIC_CAST( child->firstChild(), childDev, Device * );
			ModDescriptorProperties* newDevProp;
			WIZ_DYNAMIC_CAST( childDev->getProp(), newDevProp, ModDescriptorProperties * );
			DescriptorEntryDirectory* desc = &newDevProp->desc;

			DescriptorEntryUint32* smBusDesc = (DescriptorEntryUint32*)desc->find( "SMB_BUSNBR", DescriptorEntry::Uint32 );
			smBusDesc->setValue( _prop->smBusNo );
		}
	}

}


