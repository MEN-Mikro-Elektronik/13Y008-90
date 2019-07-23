#include <qlayout.h>
#include <q3vbox.h>
#include <qlabel.h>
#include <q3combobox.h>
#include <qspinbox.h>

#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>

#include "modserialproptab.h"
#include "propertiesdlg.h"
#include "wizdebug.h"
#include "wizexcept.h"
#include "resourcemng.h"

ModSerialPropTab::ModSerialPropTab(
	QWidget *parent,
	DevPropertiesDlg *_mainDlg,
	const char *name,
	Qt::WFlags f, bool isBbisPciGen) :
	DevPropertiesTab(parent,_mainDlg,name,f)
{
	isBbisPciGenDev = isBbisPciGen;
	PciBusInterface *busIf;
	int row = 0;
	wDebug(("ModSerialPropTab::ModSerialPropTab"));

	WIZ_DYNAMIC_CAST( (getMainDlg()->getDevice()->getParent()), busIf,
					  PciBusInterface * );

	hasPciBusPath = busIf->hasPciBusPath();
	pciBusNoSpBox = 0;

	Q3VBoxLayout *vb = new Q3VBoxLayout( this, 10 );
	QLabel *lab = new QLabel( this );

	Q3GridLayout *gl = new Q3GridLayout( vb, 0, 2, 10);
	row++;

	lab->setText( QString("%1 slot number of this "
						  "carrier board is fixed").
				  arg(busIf->busIfTypeMap.keyToStr(
					  busIf->getBusIfType())));
	gl->addWidget( lab );

	pciSlotLabel = new QLabel( QString("SLOT: "), this );
	gl->addWidget( pciSlotLabel, row, 0, Qt::AlignLeft);
	row++;

	pciBusSlotParameter = new QCheckBox(QString("Use PCI Bus Number instead of Slot\n"
			                                    "specific PCI parameters (PCI_BUS_PATH)"), this);
	gl->addWidget( pciBusSlotParameter, row, 0, Qt::AlignLeft );
	row++;

	// Bus interface bus path is unknown, ask for bus/device number
	pciBusNoInfo0 = new QLabel( QString(""), this );
	pciBusNoInfo0->setText( QString("Determine the actual parameters of the PCI device\n(e.g. by using lspci command under Linux) and enter it.") );
	gl->addWidget( pciBusNoInfo0, row, 0, Qt::AlignLeft );
	row++;

	pciBusNoInfo1 = new QLabel( QString(""), this );
	pciBusNoInfo1->setText( QString("<font color=\"red\">No validity check will be done!</font>") );
	gl->addWidget( pciBusNoInfo1, row, 0, Qt::AlignLeft );
	row++;

	// Bus number
	pciBusNoLabel = new QLabel( QString("PCI Bus Number:"), this );
	gl->addWidget( pciBusNoLabel, row, 0, Qt::AlignLeft );
	pciBusNoSpBox = new QSpinBox( 0, 255, 1, this );
	gl->addWidget( pciBusNoSpBox, row,  0, Qt::AlignCenter );
	row++;

	// Device number - for serial devices it is greyed out
	pciDevNoLabel = new QLabel( QString("PCI Device Number:"), this );
	gl->addWidget( pciDevNoLabel, row, 0, Qt::AlignLeft );
	pciDevNoSpBox = new QSpinBox( 0, 31, 1, this );
	gl->addWidget( pciDevNoSpBox, row, 0, Qt::AlignCenter);

	pciDevNoLabel->setEnabled(false);
	pciDevNoSpBox->setEnabled(false);

	if ( !hasPciBusPath ) {
		hasPciBusSlotParameter=false;
		pciBusSlotParameter->setHidden(true);
		pciBusSlotParameter->setChecked(true);
	}
	else {
		hasPciBusSlotParameter=true;
		pciBusSlotParameter->setChecked(false);
	}

	connect(pciBusSlotParameter, SIGNAL(stateChanged(int)), this, SLOT(enablePciFields(int)));
	vb->addStretch( 10 );

}

void
ModSerialPropTab::enablePciFields(int state)
{
	wDebug(("ModSerialPropTab::enablePciFields"));
	if ( hasPciBusPath ) {
		if (state == QCheckBox::On) {
			wDebug(("state == QCheckBox::On"));
			if ( isBbisPciGenDev ) {
				ModBbisPciGenProperties *_prop;
				WIZ_DYNAMIC_CAST( getMainDlg()->getProperties(), _prop,
								  ModBbisPciGenProperties *);
				_prop->pciBusNoIsDef = true;
				_prop->pciDevNoIsDef = true;
			}
			else {
				ModBbisPciMmodProperties *_prop;
				WIZ_DYNAMIC_CAST( getMainDlg()->getProperties(), _prop,
								  ModBbisPciMmodProperties *);
				_prop->pciBusNoIsDef = true;
				_prop->pciDevNoIsDef = true;
			}
			pciBusNoInfo0->setHidden(false);
			pciBusNoInfo1->setHidden(false);
			pciBusNoLabel->setHidden(false);
			pciBusNoSpBox->setHidden(false);
			pciDevNoLabel->setHidden(false);
			pciDevNoSpBox->setHidden(false);
			}
		else {
			wDebug(("state == QCheckBox::Off"));
			if ( isBbisPciGenDev ) {
				ModBbisPciGenProperties *_prop;
				WIZ_DYNAMIC_CAST( getMainDlg()->getProperties(), _prop,
								  ModBbisPciGenProperties *);
				_prop->pciBusNoIsDef = false;
				_prop->pciDevNoIsDef = false;
			}
			else {
				ModBbisPciMmodProperties *_prop;
				WIZ_DYNAMIC_CAST( getMainDlg()->getProperties(), _prop,
								  ModBbisPciMmodProperties *);
				_prop->pciBusNoIsDef = false;
				_prop->pciDevNoIsDef = false;
			}
			pciBusNoInfo0->setHidden(true);
			pciBusNoInfo1->setHidden(true);
			pciBusNoLabel->setHidden(true);
			pciBusNoSpBox->setHidden(true);
			pciDevNoLabel->setHidden(true);
			pciDevNoSpBox->setHidden(true);
		}
	}
}

void
ModSerialPropTab::updateProperties()
{
	PciBusInterface *busIf;
	wDebug(("ModSerialPropTab::updateProperties"));

	WIZ_DYNAMIC_CAST( (getMainDlg()->getDevice()->getParent()), busIf,
					  PciBusInterface * );

	if ( isBbisPciGenDev ) {
		ModBbisPciGenProperties *_prop;
		WIZ_DYNAMIC_CAST( getMainDlg()->getProperties(), _prop,
						  ModBbisPciGenProperties *);
		_prop->pciBusNo = pciBusNoSpBox->value();
		_prop->pciDevNo = 0;
		if( hasPciBusSlotParameter && !pciBusSlotParameter->isChecked() ){
			_prop->pciBusNoIsDef = false;
			_prop->pciDevNoIsDef = false;
		}
		else {
			_prop->pciBusNoIsDef = true;
			_prop->pciDevNoIsDef = true;
		}
	}
	else {
		ModBbisPciMmodProperties *_prop;
		WIZ_DYNAMIC_CAST( getMainDlg()->getProperties(), _prop,
						  ModBbisPciMmodProperties *);
		_prop->pciBusNo = pciBusNoSpBox->value();
		_prop->pciDevNo = 0;
		if( hasPciBusSlotParameter && !pciBusSlotParameter->isChecked() ){
			_prop->pciBusNoIsDef = false;
			_prop->pciDevNoIsDef = false;
		}
		else {
			_prop->pciBusNoIsDef = true;
			_prop->pciDevNoIsDef = true;
		}
	}

	pciDevNoLabel->setEnabled(false);
	pciDevNoSpBox->setEnabled(false);
}

void
ModSerialPropTab::setProperties()
{
	wDebug(("ModSerialPropTab::setProperties"));
	PciBusInterface *busIf;
	bool useBusPath=true;

	WIZ_DYNAMIC_CAST( (getMainDlg()->getDevice()->getParent()), busIf,
						PciBusInterface * );

	if ( isBbisPciGenDev ) {
		ModBbisPciGenProperties *_prop;
		WIZ_DYNAMIC_CAST( getMainDlg()->getProperties(), _prop,
						  ModBbisPciGenProperties *);
		pciSlotLabel->setText(QString("Slot: %1").arg(_prop->slotNo ));
		pciBusNoSpBox->setValue( _prop->pciBusNo );
		_prop->pciDevNo = 0;
		useBusPath = (!_prop->usePciBusNoAndDevNo());
	}
	else {
		ModBbisPciMmodProperties *_prop;
		WIZ_DYNAMIC_CAST( getMainDlg()->getProperties(), _prop,
						  ModBbisPciMmodProperties *);
		pciSlotLabel->setText(QString("Slot: %1").arg(_prop->slotNo ));
		pciBusNoSpBox->setValue( _prop->pciBusNo );
		_prop->pciDevNo = 0;
		useBusPath = (!_prop->usePciBusNoAndDevNo());
	}

	if( hasPciBusSlotParameter && useBusPath ) {
		pciBusSlotParameter->setChecked(false);
		pciBusNoInfo0->setHidden(true);
		pciBusNoInfo1->setHidden(true);
		pciBusNoLabel->setHidden(true);
		pciBusNoSpBox->setHidden(true);
		pciDevNoLabel->setHidden(true);
		pciDevNoSpBox->setHidden(true);
	}
	else {
		pciBusSlotParameter->setChecked(true);
		pciBusNoInfo0->setHidden(false);
		pciBusNoInfo1->setHidden(false);
		pciBusNoLabel->setHidden(false);
		pciBusNoSpBox->setHidden(false);
		pciDevNoLabel->setHidden(false);
		pciDevNoSpBox->setHidden(false);
	}
}
