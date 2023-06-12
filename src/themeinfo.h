#ifndef THEMEINFO_H
#define THEMEINFO_H

#include <qpalette.h>

class ThemeInfo
{
public:
    static bool isDarkTheme();
    static auto windowColor() { return QPalette{}.color(QPalette::Window); };
    static auto windowTextColor() { return QPalette{}.color(QPalette::WindowText); };
};

#endif // THEMEINFO_H
