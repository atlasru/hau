// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <QString>
#include <QColor>
namespace Parley {
QString designSystem(bool dark, bool amoled, bool translucent, const QColor& accent);
}
