#include "textwidget.h"

#include <qboxlayout.h>

/**
 * @brief Constructs a new TextWidget object
 *
 * @param parent
 * @param text
 */
TextWidget::TextWidget(QWidget *parent, const QString &text)
    : QWidget{ parent }, label{ new QLabel{ text, this } }
{
    QHBoxLayout *layout{ new QHBoxLayout{ this } };
    layout->addWidget(label);
}
