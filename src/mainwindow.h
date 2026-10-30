#ifndef MAINWINDOW_H
#define MAINWINDOW_H
 
#include <KXmlGuiWindow>
#include <QSystemTrayIcon>
#include <vector>
#include <unordered_map>
#include "taskentry.h"
#include "settingsentry.h"

class KTextEdit;
class KJob;
class QTimer;
class QCloseEvent;
class QVBoxLayout;
class QPushButton;
class QScrollArea;
class QVariant;

class MainWindow : public KXmlGuiWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void openFileFromUrl(const QUrl& inputFileName);
private:
    QScrollArea* taskScroll;
    QWidget* taskList;
    QVBoxLayout* taskLayout;
    std::vector<TaskEntry*> taskEntries;
    QWidget* footerButtons;
    QHBoxLayout* footerLayout;
    QPushButton* addTaskButton;
    QPushButton* resetWeightsButton;
    QPushButton* checkWeightedButton;
    QSystemTrayIcon* systray;
    QWidget* settingsList;
    QVBoxLayout* settingsLayout;
    std::unordered_map<QString, SettingsEntry*> settingsEntries;
    QString fileName;
    QTimer* taskTime;
    int secondsLeft;
    bool cancelQuit;
    bool fullQuit;
    TaskEntry* addTask();
    void checkWeighted();
    void createList(QString text);
    void kInfo(QString title, QString body);
    QAction* makeAction(QString text, QIcon icon, QString name, QKeySequence keys);
    void moveTask(TaskEntry* task, int delta);
    void openSettings();
    void removeTask(TaskEntry* task);
    void resetWeights();
    void setupActions();
    void setupSettings();
    void setupTray();
    void setupWindow();
    void saveFileToDisk(const QString& outputFileName);
    void startTimer();
    void trayClicked(QSystemTrayIcon::ActivationReason reason);
    void updateTimer();
    void unsavedChanges();
    void writeSetting(QString name);
private Q_SLOTS:
    void checkTimer();
    void newFile();
    void openFile();
    void pauseTimer();
    void resumeTimer();
    void saveFile();
    void saveFileAs();
    void spinWheel();
    void stopTimer();
    void downloadFinished(KJob* job);
    void quitApplication();
protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H