// SPDX-License-Identifier: GPL-3.0-or-later
#include "designsystem.h"

QString Parley::designSystem(bool dark, bool amoled, bool translucent, const QColor& accent)
{
    // Opaque fallback avoids GPU blur and preserves text contrast on every backend.
    const QString surface = amoled ? QStringLiteral("#000000") : dark ? QStringLiteral("#151820") : QStringLiteral("#f5f5fc");
    const QString card = dark ? QStringLiteral("#202530") : QStringLiteral("#ffffff");
    const QString text = dark ? QStringLiteral("#f2f2fc") : QStringLiteral("#202231");
    const QString secondary = dark ? QStringLiteral("#b7becf") : QStringLiteral("#555f77");
    const QString border = dark ? QStringLiteral("#3a4254") : QStringLiteral("#d5dbea");
    const QString hover = dark ? QStringLiteral("#30384b") : QStringLiteral("#e9eafa");
    QString qss = QStringLiteral(R"QSS(
QMainWindow, QDialog, QDialog#LoginScreen, QMessageBox, QWidget#centralwidget, QWidget#contentWidget { background: %1; color: %3; }
QWidget { selection-background-color: %6; selection-color: %1; }
QLabel, QCheckBox, QRadioButton, QGroupBox { color: %3; background-color: transparent; }
QLabel#statusMessageLabelObj { color: %4; }
QWidget#accountRail { background: %2; border: 1px solid %5; border-radius: 24px; padding: 8px; }
QStackedWidget#stackedWidget, QWidget#newPage, QWidget#loginPage { background: transparent; }
QLabel#parleyTitle { font-size: 38px; font-weight: 600; letter-spacing: -1px; }
QLabel#parleyTagline { font-size: 20px; color: %4; }
QLabel#createTitle, QLabel#loginTitle { font-size: 28px; font-weight: 600; }
QLabel#createHint, QLabel#loginHint, QLabel#localHint, QLabel#passwordHint { color: %4; }
QPushButton#createAccountButton, QPushButton#loginButton { background: %6; color: %1; min-height: 30px; font-weight: 600; border: 0; }
QDialog#LoginScreen QPushButton#importButton { background: %2; color: %3; border: 1px solid %5; border-radius: 14px; padding: 8px 12px; }
QProgressBar#passStrengthMeter { background: %5; border: 0; border-radius: 3px; }
QProgressBar#passStrengthMeter::chunk { background: %6; border-radius: 3px; }
QFrame#statusPanel, QWidget#statusHead { background: %2; border-radius: 22px; }
QWidget#tooliconsZone { background: %2; border: 1px solid %5; border-radius: 24px; }
QPushButton, QToolButton { color: %3; background: %2; border: 1px solid %5; border-radius: 14px; padding: 8px 12px; min-height: 24px; }
QPushButton:hover, QToolButton:hover { background: %7; border-color: %6; }
QPushButton:pressed, QToolButton:pressed, QPushButton:checked { background: %7; border: 2px solid %6; }
QPushButton:focus, QToolButton:focus, QLineEdit:focus, QComboBox:focus { border: 2px solid %6; }
QPushButton:disabled, QToolButton:disabled { color: %4; border-color: %5; }
QLineEdit, QTextEdit, QPlainTextEdit, QSpinBox, QDoubleSpinBox, QComboBox { color: %3; background: %2; border: 1px solid %5; border-radius: 12px; padding: 7px; selection-background-color: %6; }
QComboBox QAbstractItemView, QListWidget, QTableView, QTreeView { color: %3; background: %2; alternate-background-color: %1; border: 1px solid %5; border-radius: 12px; }
QGroupBox { border: 1px solid %5; border-radius: 18px; margin-top: 16px; padding: 16px 10px 10px; }
QGroupBox::title { subcontrol-origin: margin; left: 18px; padding: 0 6px; }
QMenu { color: %3; background: %2; border: 1px solid %5; border-radius: 16px; padding: 8px; }
QMenu::item { padding: 9px 18px; border-radius: 9px; }
QMenu::item:selected { background: %7; }
QMenu::separator { height: 1px; background: %5; margin: 6px; }
QTabWidget::pane { border: 0; background: %1; }
QTabBar::tab { color: %4; background: %2; border-radius: 12px; padding: 10px 16px; margin: 3px; }
QTabBar::tab:selected { color: %3; background: %7; border-bottom: 2px solid %6; }
QLabel#unreadBadge { color: %1; background: %6; border-radius: 10px; padding: 3px 7px; font-weight: 600; min-width: 12px; }
QScrollBar:vertical { background: transparent; width: 12px; }
QScrollBar::handle:vertical { background: %5; border-radius: 5px; min-height: 32px; margin: 2px; }
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
QToolTip { color: %3; background: %2; border: 1px solid %5; padding: 8px; border-radius: 8px; }
)QSS");
    qss = qss.arg(surface, card, text, secondary, border, accent.name(), hover);
    if (translucent && !amoled) {
        qss += QStringLiteral("QWidget#tooliconsZone { background: qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 %1,stop:1 %2); border-top-color: %3; }").arg(card, surface, border);
    }
    return qss;
}
