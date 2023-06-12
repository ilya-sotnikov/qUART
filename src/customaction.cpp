#include "customaction.h"

#include <qfileinfo.h>
#include <qpalette.h>

using namespace Qt::Literals::StringLiterals;

static bool isDarkTheme();

/**
 * @brief Checks if the current theme is dark
 *
 * @return True if the current theme is dark, otherwise false
 */
static bool isDarkTheme()
{
    const QPalette defaultPalette;
    return defaultPalette.color(QPalette::WindowText).lightness()
            > defaultPalette.color(QPalette::Window).lightness();
}

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

/**
 * @brief Sets a black or white icon depending on the theme
 *
 * @param fileName
 */
void CustomAction::setIconWithTheme(const QString &fileName)
{
    const QFileInfo file{ fileName };
    const auto name{ file.baseName() };
    const auto extension{ file.suffix() };
    const auto fileNameWithTheme{ name + (isDarkTheme() ? u"White."_s : u"Black."_s) + extension };
    setIcon(QIcon{ fileNameWithTheme });
}
