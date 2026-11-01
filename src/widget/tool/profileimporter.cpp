/* SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright © 2015-2019 by The qTox Project Contributors
 * Copyright © 2024-2026 The TokTok team.
 */

#include "profileimporter.h"

#include "src/core/core.h"
#include "src/persistence/paths.h"

#include "src/persistence/profilelocker.h"
#include <QSaveFile>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>

/**
 * @class ProfileImporter
 * @brief This class provides the ability to import profile.
 * @note This class can be used before @a Nexus instance will be created,
 * consequently it can't use @a GUI class. Therefore it should use QMessageBox
 * to create dialog forms.
 */
ProfileImporter::ProfileImporter(Paths& paths_, QWidget* parent)
    : QWidget(parent)
    , paths{paths_}
{
}

/**
 * @brief Show a file dialog. Selected file will be imported as Tox profile.
 * @return True, if the import was successful. False otherwise.
 */
bool ProfileImporter::importProfile()
{
    const QString title = tr("Import profile", "import dialog title");
    const QString filter = tr("Tox save file (*.tox)", "import dialog filter");
    const QString dir = QDir::homePath();

    // TODO: Change all QFileDialog instances across project to use
    // this instead of Q_NULLPTR. Possibly requires >Qt 5.9 due to:
    // https://bugreports.qt.io/browse/QTBUG-59184
    const QString path = QFileDialog::getOpenFileName(Q_NULLPTR, title, dir, filter);

    return importProfile(path);
}

/**
 * @brief Asks the user a question with Yes/No choices.
 * @param title Title of question window.
 * @param message Text in question window.
 * @return True if the answer is positive, false otherwise.
 */
bool ProfileImporter::askQuestion(QString title, QString message)
{
    const QMessageBox::Icon icon = QMessageBox::Warning;
    QMessageBox box(icon, title, message, QMessageBox::NoButton, this);
    QPushButton* pushButton1 = box.addButton(QApplication::tr("Yes"), QMessageBox::AcceptRole);
    QPushButton* pushButton2 = box.addButton(QApplication::tr("No"), QMessageBox::RejectRole);
    box.setDefaultButton(pushButton2);
    box.setEscapeButton(pushButton2);

    box.exec();
    return box.clickedButton() == pushButton1;
}

/**
 * @brief Try to import Tox profile.
 * @param path Path to Tox profile.
 * @return True, if the import was successful. False otherwise.
 */
bool ProfileImporter::importProfile(const QString& path)
{
    if (path.isEmpty())
        return false;

    const QFileInfo info(path);
    if (!info.exists()) {
        QMessageBox::warning(this, tr("File doesn't exist"), tr("Profile doesn't exist"),
                             QMessageBox::Ok);
        return false;
    }

    const QString profile = info.completeBaseName();

    if (info.suffix() != "tox") {
        QMessageBox::warning(this, tr("Ignoring non-Tox file", "popup title"),
                             tr("Warning: You have chosen a file that is not a "
                                "Tox save file; ignoring.",
                                "popup text"),
                             QMessageBox::Ok);
        return false; // ignore importing non-tox file
    }

    const QString settingsPath = paths.getSettingsDirPath();
    const QString profilePath = QDir(settingsPath).filePath(profile + Core::TOX_EXT);

    if (ProfileLocker::getCurLockName() == profile || !ProfileLocker::isLockable(profile, paths)) {
        QMessageBox::warning(this, tr("Profile in use"), tr("Switch accounts before importing over this profile."));
        return false;
    }
    if (QFileInfo(profilePath).exists()) {
        QMessageBox::warning(this, tr("Profile already exists"),
            tr("Rename the import file to a new profile name. Existing accounts are never overwritten, so their history cannot be mixed with a different identity."));
        return false;
    }

    QFile source(path);
    QSaveFile target(profilePath);
    target.setDirectWriteFallback(false);
    if (!source.open(QIODevice::ReadOnly) || source.size() <= 0 || source.size() > 32 * 1024 * 1024) {
        QMessageBox::warning(this, tr("Import failed"), tr("Profile is empty, too large, or unreadable."));
        return false;
    }
    const auto bytes = source.readAll();
    if (source.error() != QFile::NoError || !target.open(QIODevice::WriteOnly)
        || target.write(bytes) != bytes.size() || !target.commit()) {
        QMessageBox::warning(this, tr("Import failed"), tr("Cannot commit profile. Existing identity was preserved."));
        return false;
    }
    QFile::setPermissions(profilePath, QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    // no good way to update the ui from here... maybe we need a Widget:refreshUi() function...
    // such a thing would simplify other code as well I believe
    QMessageBox::information(this, tr("Profile imported"),
                             tr("%1.tox was successfully imported").arg(profile), QMessageBox::Ok);

    return true; // import successful
}
