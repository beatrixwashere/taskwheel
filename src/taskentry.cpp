#include <QLineEdit>
#include <QHBoxLayout>
#include <QSpinBox>
#include "taskentry.h"

TaskEntry::TaskEntry() {
    container = new QWidget();
    layout = new QHBoxLayout();
    taskName = new QLineEdit();
    taskWeight = new QSpinBox();
    layout->addWidget(taskName);
    layout->addWidget(taskWeight);
    container->setLayout(layout);
    container->show();
}

TaskEntry::~TaskEntry() {
    container->hide();
    delete container;
    delete layout;
    delete taskName;
    delete taskWeight;
}

QString TaskEntry::getName() {
    return taskName->text();
}

int TaskEntry::getWeight() {
    return taskWeight->value();
}