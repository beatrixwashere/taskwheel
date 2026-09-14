#ifndef TASKENTRY_H
#define TASKENTRY_H

#include <KXmlGuiWindow>

class QHBoxLayout;
class QLineEdit;
class QSpinBox;

class TaskEntry {
public:
    TaskEntry();
    QWidget* container;
    QString getName();
    int getWeight();
private:
    QHBoxLayout* layout;
    QLineEdit* taskName;
    QSpinBox* taskWeight;
};

#endif // TASKENTRY_H