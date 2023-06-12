#ifndef CUSTOMACTION_H
#define CUSTOMACTION_H

#include <qaction.h>

class CustomAction : public QAction
{
public:
    using QAction::QAction;
    using QAction::setShortcut;
    void setShortcut(const QKeySequence &shortcut, bool showInTooltip);
    void setIconWithTheme(const QString &fileName);
};

#endif // CUSTOMACTION_H
