#ifndef SETTINGSENTRY_H
#define SETTINGSENTRY_H

#include <QObject>

class QHBoxLayout;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QCheckBox;
class QLabel;
class QVariant;

class SettingsEntry {
public:
    SettingsEntry(QString name, bool val);
    ~SettingsEntry();
    QWidget* container;
    QSpinBox* settingValue;
    QCheckBox* settingCheck;
    QVariant getVal();
    void setVal(QVariant val);
private:
    QHBoxLayout* layout;
    QLabel* settingName;
    bool isNum;
};

#endif // SETTINGSENTRY_H