#include <QLineEdit>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QVariant>
#include <KLocalizedString>
#include "settingsentry.h"
#include "settings.h"

SettingsEntry::SettingsEntry(QString name, bool num) {
    container = new QWidget();
    layout = new QHBoxLayout();
    settingName = new QLabel();

    settingName->setText(name);

    layout->addWidget(settingName);
    if(num) {
        settingValue = new QSpinBox();
        settingValue->setMaximum(3600);
        layout->addWidget(settingValue);
    } else {
        settingCheck = new QCheckBox();
        layout->addWidget(settingCheck);
    }
    isNum = num;

    container->setLayout(layout);
}

QVariant SettingsEntry::getVal() {
    if(isNum) {
        return settingValue->value();
    } else {
        return settingCheck->isChecked();
    }
}

void SettingsEntry::setVal(QVariant val) {
    if(isNum) {
        settingValue->setValue(val.toInt());
    } else {
        settingCheck->setChecked(val.toBool());
    }
    Settings::set(settingName->text(), val);
}