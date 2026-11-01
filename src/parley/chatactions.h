// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
class QMenu;
class ChatId;
class Settings;
namespace Parley {
void addChatActions(QMenu& menu, const ChatId& id, Settings& settings);
}
