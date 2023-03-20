#include "textwidget.h"

#include <QHBoxLayout>

/**
 * @brief Construct a new TextWidget object
 *
 * @param parent
 * @param text
 */
TextWidget::TextWidget(QWidget *parent, const QString &text)
    : QWidget{ parent }, label{ new QLabel{ text, this } }
{
    QHBoxLayout *layout{ new QHBoxLayout };
    layout->addWidget(label);
    setLayout(layout);
}

/**
 * @brief Set widget text
 *
 * @param text
 */
void TextWidget::setText(const QString &text) const
{
    label->setText(text);
}
