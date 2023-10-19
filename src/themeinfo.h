#ifndef THEMEINFO_H
#define THEMEINFO_H

#include <qpalette.h>

namespace ThemeInfo {

inline auto windowColor() { return QPalette{}.color(QPalette::Window); }

inline auto windowTextColor() { return QPalette{}.color(QPalette::WindowText); }

inline bool isDarkTheme() {
    const QPalette defaultPalette;
    return defaultPalette.color(QPalette::WindowText).lightness()
            > defaultPalette.color(QPalette::Window).lightness();
}

} // ThemeInfo

#endif // THEMEINFO_H
