#include "textwidget.h"

#include <qboxlayout.h>

/**
 * @brief Constructs a new TextWidget object
 *
 * @param parent
 * @param text Text to be displayed in the status bar
 */
TextWidget::TextWidget(QWidget *parent, const QString &text)
    : QWidget{ parent }, label{ new QLabel{ text, this } }
{
    const auto layout{ new QHBoxLayout{ this } };
    layout->addWidget(label);
}
