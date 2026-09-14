#include <QApplication>
#include <QAction>
#include <QSaveFile>
#include <QFileDialog>
#include <QTextStream>
#include <QByteArray>
#include <QStandardPaths>
#include <QTimer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QScrollArea>
#include <KTextEdit>
#include <KLocalizedString>
#include <KActionCollection>
#include <KStandardAction>
#include <KMessageBox>
#include <KIO/StoredTransferJob>
#include <string>
#include <sstream>
#include <vector>
#include "mainwindow.h"
#include "taskentry.h"

MainWindow::MainWindow(QWidget* parent) : KXmlGuiWindow(parent), fileName(QString()) {
    setupWindow();
    taskTime = new QTimer(this);
    secondsLeft = 0;
    cancelQuit = false;
    connect(taskTime, &QTimer::timeout, this, &MainWindow::updateTimer);
    setCentralWidget(taskScroll);
    setupActions();
}

MainWindow::~MainWindow() {
    taskList->hide();
    delete taskScroll;
    delete taskList;
    delete taskLayout;
    for(TaskEntry* te : taskEntries) {
        delete te;
    }
    delete newTask;
    delete textArea;
    delete taskTime;
}

void MainWindow::setupActions() {
    using namespace Qt::Literals::StringLiterals;

    QAction* spinAction = makeAction(i18n("&Spin"), QIcon::fromTheme(QIcon::ThemeIcon::Printer), u"Spin"_s, Qt::ALT | Qt::Key_S);
    QAction* checkAction = makeAction(i18n("&Check"), QIcon::fromTheme(QIcon::ThemeIcon::CameraVideo), u"Check"_s, Qt::ALT | Qt::Key_C);
    QAction* pauseAction = makeAction(i18n("&Pause"), QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackPause), u"Pause"_s, Qt::ALT | Qt::Key_P);
    QAction* resumeAction = makeAction(i18n("&Resume"), QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStart), u"Resume"_s, Qt::ALT | Qt::Key_R);

    connect(spinAction, &QAction::triggered, this, &MainWindow::spinWheel);
    connect(checkAction, &QAction::triggered, this, &MainWindow::checkTimer);
    connect(pauseAction, &QAction::triggered, this, &MainWindow::pauseTimer);
    connect(resumeAction, &QAction::triggered, this, &MainWindow::resumeTimer);

    KStandardAction::quit(this, &MainWindow::quitApplication, actionCollection());
    KStandardAction::open(this, &MainWindow::openFile, actionCollection());
    KStandardAction::save(this, &MainWindow::saveFile, actionCollection());
    KStandardAction::saveAs(this, &MainWindow::saveFileAs, actionCollection());
    KStandardAction::openNew(this, &MainWindow::newFile, actionCollection());

    setupGUI(Default, u"taskwheelui.rc"_s);
}

void MainWindow::setupWindow() {
    taskScroll = new QScrollArea();
    taskList = new QWidget();
    taskLayout = new QVBoxLayout();

    for(int i = 0; i < 1; i++) {
        taskEntries.push_back(new TaskEntry());
        taskLayout->addWidget(taskEntries[i]->container);
    }
    newTask = new QPushButton();
    newTask->setText(i18n("new task"));
    taskLayout->addWidget(newTask);
    textArea = new KTextEdit();
    taskLayout->addWidget(textArea);

    taskList->setLayout(taskLayout);
    taskList->setMinimumSize(960, 540);
    taskList->show();

    taskScroll->setWidget(taskList);
    taskScroll->setAlignment(Qt::AlignHCenter);
    taskScroll->setWidgetResizable(true);
}

QAction* MainWindow::makeAction(QString text, QIcon icon, QString name, QKeySequence keys) {
    QAction* action = new QAction(this);
    action->setText(text);
    action->setIcon(icon);
    actionCollection()->addAction(name, action);
    actionCollection()->setDefaultShortcut(action, keys);
    return action;
}

void MainWindow::checkTimer() {
    QString timerStatus;
    if(secondsLeft == 0) {
        timerStatus = i18n("no timer active");
    } else {
        timerStatus = QString::fromStdString("seconds left: " + std::to_string(secondsLeft));
    }

    KMessageBox::information(
        nullptr,
        timerStatus,
        i18n("timer"));
}

void MainWindow::newFile() {
    fileName.clear();
    textArea->clear();
}

void MainWindow::openFile() {
    openFileFromUrl(QFileDialog::getOpenFileUrl(this, i18n("open file")));
}

void MainWindow::openFileFromUrl(const QUrl& inputFileName) {
    if(!inputFileName.isEmpty()) {
        KIO::Job* job = KIO::storedGet(inputFileName);
        fileName = inputFileName.toLocalFile();
        connect(job, &KJob::result, this, &MainWindow::downloadFinished);
        job->exec();
    }
}

void MainWindow::pauseTimer() {
    if(!taskTime->isActive()) {
        KMessageBox::information(
            nullptr,
            i18n("no timer to pause"),
            i18n("timer"));
        return;
    }

    taskTime->stop();
    KMessageBox::information(
        nullptr,
        QString::fromStdString("timer paused at " + std::to_string(secondsLeft) + " seconds"),
        i18n("timer"));
}

void MainWindow::resumeTimer() {
    if(secondsLeft == 0) {
        KMessageBox::information(
            nullptr,
            i18n("no timer to resume"),
            i18n("timer"));
        return;
    }

    taskTime->start(1000);

    KMessageBox::information(
        nullptr,
        QString::fromStdString("timer resumed at " + std::to_string(secondsLeft) + " seconds"),
        i18n("timer"));
}

void MainWindow::saveFile() {
    if(!fileName.isEmpty()) {
        saveFileToDisk(fileName);
    } else {
        saveFileAs();
    }
}

void MainWindow::saveFileAs() {
    saveFileToDisk(QFileDialog::getSaveFileName(this, i18n("save file as")));
}

void MainWindow::saveFileToDisk(const QString& outputFileName) {
    if(!outputFileName.isNull()) {
        QSaveFile file(outputFileName);
        file.open(QIODevice::WriteOnly);

        QByteArray outputByteArray;
        outputByteArray.append(textArea->toPlainText().toUtf8());

        file.write(outputByteArray);
        file.commit();

        fileName = outputFileName;
    }
}

void MainWindow::spinWheel() {
    if(taskTime->isActive()) {
        auto messageBox = KMessageBox::questionTwoActions(
            nullptr,
            i18n("stop current timer?"),
            i18n("timer"),
            KStandardGuiItem::ok(), KStandardGuiItem::cancel());
        if(messageBox == KMessageBox::PrimaryAction) {
            stopTimer();
        } else {
            return;
        }
    }

    std::string txt = textArea->toPlainText().toStdString();
    std::stringstream ss(txt);
    std::vector<std::string> tasks;

    for(std::string token; std::getline(ss, token);) {
        std::size_t pos = token.find(' ');
        if(pos == -1) {
            continue;
        }
        std::string taskName = token.substr(0, pos);
        int taskWeight = stoi(token.substr(pos));
        while(taskWeight--) {
            tasks.push_back(taskName);
        }
    }

    QString selection;
    srand(time(NULL));
    if(tasks.size() > 0) {
        selection = QString::fromStdString(tasks[rand() % tasks.size()]);
    } else {
        selection = i18n("failed to select task");
    }

    auto messageBox = KMessageBox::questionTwoActions(
        nullptr,
        selection,
        i18n("selected task"),
        KStandardGuiItem::ok(), KStandardGuiItem::cancel());
    if(messageBox == KMessageBox::PrimaryAction) {
        startTimer();
    }
}

void MainWindow::startTimer() {
    secondsLeft = 600;
    taskTime->start(1000);

    KMessageBox::information(
        nullptr,
        i18n("started 10 minute timer"),
        i18n("timer"));
}

void MainWindow::stopTimer() {
    taskTime->stop();
    secondsLeft = 0;

    KMessageBox::information(
        nullptr,
        i18n("timer finished!"),
        i18n("timer"));
}

void MainWindow::updateTimer() {
    secondsLeft--;
    if(secondsLeft <= 0) {
        stopTimer();
    }
}

void MainWindow::downloadFinished(KJob* job) {
    if(job->error()) {
        KMessageBox::error(this, job->errorString());
        fileName.clear();
        return;
    }

    const KIO::StoredTransferJob* storedJob = qobject_cast<KIO::StoredTransferJob*>(job);

    if(storedJob) {
        textArea->setPlainText(QTextStream(storedJob->data(), QIODevice::ReadOnly).readAll());
    }
}

void MainWindow::unsavedChanges() {
    auto messageBox = KMessageBox::questionTwoActionsCancel(
        nullptr,
        i18n("save before closing?"),
        i18n("quit"),
        KStandardGuiItem::save(), KStandardGuiItem::dontSave());
    if(messageBox == KMessageBox::PrimaryAction) {
        saveFile();
    }
    if(messageBox == KMessageBox::Cancel) {
        cancelQuit = true;
    }
}

void MainWindow::quitApplication() {
    qApp->quit();
    delete this;
}

void MainWindow::closeEvent(QCloseEvent* event) {
    unsavedChanges();
    if(cancelQuit) {
        event->ignore();
        cancelQuit = false;
    } else {
        event->accept();
    }
}