#include "themeinfo.h"

/**
 * @brief Checks if the current theme is dark
 *
 * @return True if the current theme is dark, otherwise false
 */
bool ThemeInfo::isDarkTheme()
{
    const QPalette defaultPalette;
    return defaultPalette.color(QPalette::WindowText).lightness()
            > defaultPalette.color(QPalette::Window).lightness();
}
