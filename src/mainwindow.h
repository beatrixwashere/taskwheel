#ifndef MAINWINDOW_H
#define MAINWINDOW_H
 
#include <KXmlGuiWindow>
#include <vector>
#include "taskentry.h"

class KTextEdit;
class KJob;
class QTimer;
class QCloseEvent;
class QVBoxLayout;
class QPushButton;
class QScrollArea;

class MainWindow : public KXmlGuiWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    void openFileFromUrl(const QUrl& inputFileName);
private:
    QScrollArea* taskScroll;
    QWidget* taskList;
    QVBoxLayout* taskLayout;
    std::vector<TaskEntry*> taskEntries;
    QPushButton* newTask;
    KTextEdit* textArea;
    QString fileName;
    QTimer* taskTime;
    int secondsLeft;
    bool cancelQuit;
    QAction* makeAction(QString text, QIcon icon, QString name, QKeySequence keys);
    void setupActions();
    void setupWindow();
    void saveFileToDisk(const QString& outputFileName);
    void startTimer();
    void stopTimer();
    void updateTimer();
    void unsavedChanges();
private Q_SLOTS:
    void checkTimer();
    void newFile();
    void openFile();
    void pauseTimer();
    void resumeTimer();
    void saveFile();
    void saveFileAs();
    void spinWheel();
    void downloadFinished(KJob* job);
    void quitApplication();
protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H