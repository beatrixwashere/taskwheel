#ifndef TASKENTRY_H
#define TASKENTRY_H

#include <QObject>

class QHBoxLayout;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QCheckBox;

class TaskEntry {
public:
    TaskEntry();
    ~TaskEntry();
    QWidget* container;
    QHBoxLayout* layout;
    QPushButton* rmButton;
    bool getActive();
    QString getName();
    int getWeight();
    void setName(QString text);
    void setWeight(int w);
private:
    QLineEdit* taskName;
    QSpinBox* taskWeight;
    QCheckBox* taskActive;
};

#endif // TASKENTRY_H