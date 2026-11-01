#include "AppViewModel.h"
#include <QClipboard>
#include <QGuiApplication>

QString AppViewModel::connectionStateText() const {
    if (!error_.isEmpty()) return QStringLiteral("Error");
    if (state_ == 2) return QStringLiteral("Connected");
    if (state_ == 1) return QStringLiteral("Connecting");
    return QStringLiteral("Disconnected");
}
void AppViewModel::setProfile(const Profile &profile) { name_ = profile.name; emit changed(); }
void AppViewModel::setIdentity(const QString &id) { toxId_ = id; emit changed(); }
void AppViewModel::setState(int state) { state_ = state; emit changed(); }
void AppViewModel::setError(const QString &error) { error_ = error; state_ = 0; emit changed(); }
void AppViewModel::copyToxId() {
    if (!toxId_.isEmpty()) QGuiApplication::clipboard()->setText(toxId_);
}
