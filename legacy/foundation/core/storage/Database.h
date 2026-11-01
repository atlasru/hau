#pragma once
#include <QString>
#include <QSqlDatabase>

class Database {
public:
    bool open(const QString &path, QString &error);
    void close();
    ~Database() { close(); }
    Database(const Database &) = delete;
    Database &operator=(const Database &) = delete;
    Database() = default;
    int schemaVersion(QString &error) const;
private:
    QString connectionName_;
};
