#ifndef TASKENTRY_H
#define TASKENTRY_H

#include <QObject>

class QHBoxLayout;
class QLineEdit;
class QSpinBox;
class QPushButton;

class TaskEntry {
public:
    TaskEntry();
    ~TaskEntry();
    QWidget* container;
    QHBoxLayout* layout;
    QPushButton* rmButton;
    QString getName();
    int getWeight();
private:
    QLineEdit* taskName;
    QSpinBox* taskWeight;
};

#endif // TASKENTRY_H