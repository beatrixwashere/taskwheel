#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <unordered_map>

class QVariant;

class Settings {
public:
    static std::unordered_map<QString, QString> types;
    static QVariant get(QString name);
    static QStringList getKeys();
    static void read();
    static void set(QString name, QVariant val);
    static void write();
private:
    static std::unordered_map<QString, QVariant> settings;
    static void reset();
};

#endif // SETTINGS_H