// SPDX-License-Identifier: GPL-3.0-or-later
#include "chatactions.h"
#include "src/core/chatid.h"
#include "src/persistence/settings.h"
#include <QAction>
#include <QDateTime>
#include <QMenu>

void Parley::addChatActions(QMenu& menu, const ChatId& id, Settings& settings)
{
    const auto current = settings.getChatState(id);
    const auto add = [&](const QString& label, auto change) {
        auto* action = menu.addAction(label);
        const ChatIdPtr copy(id.clone().release());
        QObject::connect(action, &QAction::triggered, &menu, [copy, &settings, change] {
            auto state = settings.getChatState(*copy);
            change(state);
            settings.setChatState(*copy, state);
        });
    };
    add(current.pinOrder ? QObject::tr("Unpin chat") : QObject::tr("Pin chat"), [](ChatState& state) {
        state.pinOrder = state.pinOrder ? 0 : QDateTime::currentMSecsSinceEpoch();
        state.deleted = false;
    });
    add(current.archived ? QObject::tr("Unarchive chat") : QObject::tr("Archive chat"), [](ChatState& state) {
        state.archived = !state.archived;
        state.deleted = false;
    });
    add(current.muted ? QObject::tr("Unmute chat") : QObject::tr("Mute chat"), [](ChatState& state) {
        state.muted = !state.muted;
    });
    menu.addSeparator();
}
