// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <QtGlobal>

// Stored inside the profile's encrypted settings, never in global UI settings.
struct ChatState
{
    bool archived = false;
    bool muted = false;
    bool deleted = false;
    qint64 pinOrder = 0;
    int unread = 0;
    bool operator==(const ChatState&) const = default;
};
