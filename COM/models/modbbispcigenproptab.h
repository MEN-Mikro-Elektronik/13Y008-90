#ifndef MODBBISPCIMGENPROPTAB_H
#define MODBBISPCIMGENPROPTAB_H

#include <Q3HBoxLayout>
#include <QLabel>

#include "propertiesdlg.h"
#include "modbbispcigen.h"
#include "modbbissmbpcigen.h"
#include <qspinbox.h>
#include <qlineedit.h>
class Q3HBoxLayout;
class QSpinBox;
class Q3ComboBox;
class QLabel;
class QCheckBox;

// 
// This dialog tab presents either the PCI slot number only
// or alternatively PCI bus number
// selected by the hasPciBusPath parameter
//
class ModBbisPciGenPropTab : public DevPropertiesTab {
	Q_OBJECT
public:
	ModBbisPciGenPropTab(
		QWidget *parent,
		DevPropertiesDlg *_mainDlg,
		const char *name=0, 
		Qt::WFlags f=0);

	virtual void updateProperties();
	virtual void setProperties();

private slots:
	void enablePciFields(int state);

private:
    bool hasPciBusPath; 
    QLabel *pciBusNoInfo0, *pciBusNoInfo1, *pciBusNoLabel, *pciDevNoLabel;
	Q3ComboBox *pciSlotCbox;
	QSpinBox  *pciBusNoSpBox, *pciDevNoSpBox, *pciDomNoSpBox;
	QCheckBox *pciBusSlotParameter;
};

// 
// This dialog tab presents the PCI slot number,
// together with the PCI device number and the 
// PCI function number
//
class ModBbisSmbPciGenPropTab : public DevPropertiesTab {
	//Q_OBJECT
public:
	ModBbisSmbPciGenPropTab(
		QWidget *parent,
		DevPropertiesDlg *_mainDlg,
		const char *name=0, 
		Qt::WFlags f=0);
		
	~ModBbisSmbPciGenPropTab() {
		if (pciBusPathLineEdit)
			delete(pciBusPathLineEdit);
		if (pciDevNoLineEdit)
			delete(pciDevNoLineEdit);
		if (pciFuncNoSpBox)
			delete(pciFuncNoSpBox);
		if (smBusNoSpBox)
			delete(smBusNoSpBox);
		if (barNoSpBox)
			delete(barNoSpBox);
		if (offsetLineEdit)
			delete(offsetLineEdit);
		if (busyWaitLineEdit)
			delete(busyWaitLineEdit);
		if (alertPollLineEdit)
			delete(alertPollLineEdit);
		}

	virtual void updateProperties();
 	virtual void setProperties();
	
private:
	
	QLineEdit  	*pciBusPathLineEdit;
	QLineEdit  	*pciDevNoLineEdit;
	QSpinBox  	*pciFuncNoSpBox;
	
	QSpinBox  	*smBusNoSpBox;
	QSpinBox	*pciDomNoSpBox;
	
	QSpinBox 	*barNoSpBox;
	QLineEdit 	*offsetLineEdit;
	QLineEdit	*busyWaitLineEdit;
	QLineEdit	*alertPollLineEdit;

	
	QLabel 		*pciBusPathLabel;
};

// 
// This dialog tab presents the PCI slot number,
// together with the PCI device number and the 
// PCI function number
//
class ModBbisSmbPciLinuxPropTab : public DevPropertiesTab {
	//Q_OBJECT
public:
	ModBbisSmbPciLinuxPropTab(
		QWidget *parent,
		DevPropertiesDlg *_mainDlg,
		const char *name=0, 
		Qt::WFlags f=0);
		
	~ModBbisSmbPciLinuxPropTab() {
		if (smBusNoSpBox)
			delete(smBusNoSpBox);
		}

	virtual void updateProperties();
 	virtual void setProperties();
	
private:

	QSpinBox  *smBusNoSpBox;
	
};

#endif









