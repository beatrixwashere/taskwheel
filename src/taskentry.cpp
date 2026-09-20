#include <QLineEdit>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <KLocalizedString>
#include "taskentry.h"

TaskEntry::TaskEntry() {
    container = new QWidget();
    layout = new QHBoxLayout();
    taskName = new QLineEdit();
    taskWeight = new QSpinBox();
    taskActive = new QCheckBox();

    layout->addWidget(taskActive);
    layout->addWidget(taskName);
    layout->addWidget(taskWeight);
    
    container->setLayout(layout);
    container->setMinimumSize(960, 30);
    container->show();
}

TaskEntry::~TaskEntry() {
    container->hide();
    /*
    delete container;
    delete layout;
    delete taskName;
    delete taskWeight;
    delete rmButton;
    */
}

bool TaskEntry::getActive() {
    return taskActive->isChecked();
}

QString TaskEntry::getName() {
    return taskName->text();
}

int TaskEntry::getWeight() {
    return taskWeight->value();
}

void TaskEntry::setName(QString text) {
    return taskName->setText(text);
}

void TaskEntry::setWeight(int w) {
    return taskWeight->setValue(w);
}