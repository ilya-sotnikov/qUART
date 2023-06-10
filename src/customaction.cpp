#include "customaction.h"

using namespace Qt::Literals::StringLiterals;

/**
 * @brief Sets a shortcut and optionally updates the tooltip
 *
 * @param shortcut
 * @param showInTooltip
 */
void CustomAction::setShortcut(const QKeySequence &shortcut, bool showInTooltip)
{
    QAction::setShortcut(shortcut);
    if (showInTooltip)
        setToolTip(toolTip() + u" ("_s + shortcut.toString() + u")"_s);
}
