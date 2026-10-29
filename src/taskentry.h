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
    void setActive(bool a);
    void setName(QString text);
    void setWeight(int w);
private:
    QCheckBox* taskActive;
    QLineEdit* taskName;
    QSpinBox* taskWeight;
};

#endif // TASKENTRY_H