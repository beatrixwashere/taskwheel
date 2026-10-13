#include <QSettings>
#include <QVariant>
#include <KMessageBox>
#include "settings.h"

using namespace Qt::Literals::StringLiterals;

std::unordered_map<QString, QVariant> Settings::settings = {};
std::unordered_map<QString, QString> Settings::types = {};

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
    auto f = [](QString s, QVariant val, QString t){set(s, val); types[s] = t;};
    f(u"systray/enabled"_s, true, u"bool"_s);
    f(u"tasks/decrement"_s, true, u"bool"_s);
    f(u"tasks/saveOnClose"_s, false, u"bool"_s);
    f(u"timer/length"_s, 600, u"int"_s);
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