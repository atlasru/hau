#pragma once
#include <QObject>
#include <QString>
#include "core/profile/ProfileStore.h"

class AppViewModel final : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString profileName READ profileName NOTIFY changed)
    Q_PROPERTY(QString toxId READ toxId NOTIFY changed)
    Q_PROPERTY(QString connectionStateText READ connectionStateText NOTIFY changed)
    Q_PROPERTY(int connectionState READ connectionState NOTIFY changed)
    Q_PROPERTY(QString initializationError READ initializationError NOTIFY changed)
public:
    explicit AppViewModel(QObject *parent = nullptr) : QObject(parent) {}
    QString profileName() const { return name_; }
    QString toxId() const { return toxId_; }
    QString connectionStateText() const;
    int connectionState() const { return state_; }
    QString initializationError() const { return error_; }
    void setProfile(const Profile &profile);
    void setIdentity(const QString &id);
    void setState(int state);
    void setError(const QString &error);
    Q_INVOKABLE void copyToxId();
signals:
    void changed();
private:
    QString name_;
    QString toxId_;
    QString error_;
    int state_ = 0;
};
