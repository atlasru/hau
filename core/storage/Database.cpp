#include "Database.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QUuid>

bool Database::open(const QString &path, QString &error) {
    close();
    connectionName_ = QUuid::createUuid().toString(QUuid::WithoutBraces);
    auto db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), connectionName_);
    db.setDatabaseName(path);
    if (!db.open()) { error = db.lastError().text(); db = {}; close(); return false; }
    if (!db.transaction()) { error = db.lastError().text(); db = {}; close(); return false; }
    QSqlQuery query(db);
    if (!query.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS schema_migrations (version INTEGER PRIMARY KEY, applied_at TEXT NOT NULL)"))) {
        error = query.lastError().text(); db.rollback(); query = {}; db = {}; close(); return false;
    }
    if (!query.exec(QStringLiteral("SELECT COALESCE(MAX(version), 0) FROM schema_migrations")) || !query.next()) {
        error = query.lastError().text(); db.rollback(); query = {}; db = {}; close(); return false;
    }
    const int version = query.value(0).toInt();
    if (version > 1) { error = QStringLiteral("Database schema is newer than this application"); db.rollback(); query = {}; db = {}; close(); return false; }
    if (version == 0 && !query.exec(QStringLiteral("INSERT INTO schema_migrations(version, applied_at) VALUES(1, strftime('%Y-%m-%dT%H:%M:%fZ','now'))"))) {
        error = query.lastError().text(); db.rollback(); query = {}; db = {}; close(); return false;
    }
    if (!db.commit()) { error = db.lastError().text(); db.rollback(); query = {}; db = {}; close(); return false; }
    query = {}; db = {};
    return true;
}

int Database::schemaVersion(QString &error) const {
    if (connectionName_.isEmpty()) { error = QStringLiteral("Database is closed"); return -1; }
    auto db = QSqlDatabase::database(connectionName_, false);
    QSqlQuery query(db);
    if (!query.exec(QStringLiteral("SELECT COALESCE(MAX(version), 0) FROM schema_migrations")) || !query.next()) {
        error = query.lastError().text(); return -1;
    }
    return query.value(0).toInt();
}

void Database::close() {
    if (connectionName_.isEmpty()) return;
    const auto name = connectionName_;
    { auto db = QSqlDatabase::database(name, false); if (db.isValid()) db.close(); }
    QSqlDatabase::removeDatabase(name);
    connectionName_.clear();
}
