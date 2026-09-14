#include <QApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QUrl>
#include <KAboutData>
#include <KLocalizedString>
#include "mainwindow.h"

int main (int argc, char* argv[]) {
    using namespace Qt::Literals::StringLiterals;

    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("taskwheel");

    KAboutData aboutData(
        u"taskwheel"_s,
        i18n("taskwheel"),
        u"1.0"_s,
        i18n("choose a random task to work on for 10 minutes"),
        KAboutLicense::MIT,
        i18n("2026"),
        i18n("Educational application..."),
        u"https://github.com/beatrixwashere/taskwheel"_s,
        u"https://github.com/beatrixwashere/taskwheel/issues"_s);

    aboutData.addAuthor(
        i18n("beatrixwashere"),
        i18n("developer"),
        u"beamcg17@gmail.com"_s,
        u"https://beatrixwashere.neocities.org"_s,
        u"beatrixwashere"_s);

    KAboutData::setApplicationData(aboutData);

    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);
    parser.addPositionalArgument(u"file"_s, i18n("wheel to open"));

    MainWindow* window = new MainWindow();
    window->show();

    if (parser.positionalArguments().count() > 0) {
        window->openFileFromUrl(QUrl::fromUserInput(
            parser.positionalArguments().at(0),
            QDir::currentPath()));
    }

    return app.exec();
}
