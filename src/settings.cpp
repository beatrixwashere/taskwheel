#include <QSettings>
#include <QVariant>
#include <KLocalizedString>
#include <KMessageBox>
#include "settings.h"

using namespace Qt::Literals::StringLiterals;

std::unordered_map<QString, QVariant> Settings::settings = {};
std::unordered_map<QString, QString> Settings::types = {};
std::unordered_map<QString, QString> Settings::names = {};

QVariant Settings::get(QString name) {
    return settings[name];
}

QStringList Settings::getKeys() {
    QStringList list;
    for(auto& [k, v] : settings) {
        list.append(k);
    }
    return list;
}

void Settings::read() {
    reset();
    QSettings settingsFile(u"beatrixwashere"_s, u"taskwheel"_s);
    for(QString s : settingsFile.allKeys()) {
        if(settings.count(s)) {
            set(s, settingsFile.value(s));
        }
    }
}

void Settings::reset() {
    settings.clear();
    auto f = [](QString s, QVariant val, QString t, QString n){set(s, val); types[s] = t; names[s] = n;};
    f(u"systray/enabled"_s, true, u"bool"_s, i18n("system tray: enabled"));
    f(u"tasks/decrement"_s, true, u"bool"_s, i18n("tasks: decrement"));
    f(u"tasks/saveOnClose"_s, false, u"bool"_s, i18n("tasks: save on close"));
    f(u"timer/length"_s, 600, u"int"_s, i18n("timer: length (seconds)"));
}

void Settings::set(QString name, QVariant val) {
    settings[name] = val;
}

void Settings::write() {
    QSettings settingsFile(u"beatrixwashere"_s, u"taskwheel"_s);
    for(auto& [k, v] : settings) {
        settingsFile.setValue(k, v);
    }
}