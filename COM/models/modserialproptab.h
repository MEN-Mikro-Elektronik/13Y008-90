#ifndef MODSERIALPROPTAB_H_
#define MODSERIALPROPTAB_H_

#include <Q3HBoxLayout>

#include "propertiesdlg.h"
#include "modbbispcimmod.h"
#include "modbbispcigen.h"

class Q3HBoxLayout;
class QSpinBox;
class Q3ComboBox;
class QCheckBox;
class QLabel;

//
// This dialog tab presents CPCI serial device properties
//
class ModSerialPropTab : public DevPropertiesTab {
	Q_OBJECT
public:
	ModSerialPropTab(
		QWidget *parent,
		DevPropertiesDlg *_mainDlg,
		const char *name=0,
		Qt::WFlags f=0, bool isBbisPciGen=false);
	~ModSerialPropTab() {};
	virtual void updateProperties();
	virtual void setProperties();

private slots:
	void enablePciFields(int state);

private:
	bool isBbisPciGenDev;
    bool hasPciBusPath;
    QLabel *pciBusNoInfo0, *pciBusNoInfo1, *pciBusNoLabel, *pciSlotLabel, *pciDevNoLabel;
    QCheckBox *pciBusSlotParameter;
    bool hasPciBusSlotParameter;
	QSpinBox  *pciBusNoSpBox,*pciDomNoSpBox, *pciDevNoSpBox;

};

#endif /* MODSERIALPROPTAB_H_ */
