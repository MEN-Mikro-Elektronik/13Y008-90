#ifndef MODBBISPCIMMODPROPTAB_H
#define MODBBISPCIMMODPROPTAB_H

#include <Q3HBoxLayout>

#include "propertiesdlg.h"
#include "modbbispcimmod.h"

class Q3HBoxLayout;
class QSpinBox;
class Q3ComboBox;
class QCheckBox;
class QLabel;


// 
// This dialog tab presents either the PCI slot number only
// or alternatively PCI bus number and device number
// selected by the hasPciBusPath parameter
//
class ModBbisPciMmodPropTab : public DevPropertiesTab {
	Q_OBJECT
public:
	ModBbisPciMmodPropTab(
		QWidget *parent,
		DevPropertiesDlg *_mainDlg,
		const char *name=0, 
		Qt::WFlags f=0);
	~ModBbisPciMmodPropTab() {};
	virtual void updateProperties();
	virtual void setProperties();

private slots:
	void enablePciFields(int state);

private:
    bool hasPciBusPath;
    QLabel *pciBusNoInfo0, *pciBusNoInfo1, *pciBusNoLabel, *pciDevNoLabel;
    QCheckBox *pciBusSlotParameter;
	Q3ComboBox *pciSlotCbox;
	QSpinBox  *pciBusNoSpBox, *pciDevNoSpBox, *pciDomNoSpBox;
	
};

#endif









