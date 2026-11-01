#pragma once
#include "core/profile/ProfileStore.h"
#include <QThread>
#include <atomic>

class ToxService final : public QThread {
    Q_OBJECT
public:
    explicit ToxService(Profile profile, QObject *parent = nullptr, bool enableBootstrap = true);
    ~ToxService() override;
    void stop();
signals:
    void identityReady(const QString &toxId);
    void networkStateChanged(int state); // 0 offline, 1 connecting, 2 online
    void failure(const QString &message);
protected:
    void run() override;
private:
    Profile profile_;
    std::atomic_bool stopping_{false};
    bool enableBootstrap_;
};
