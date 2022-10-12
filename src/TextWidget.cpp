#include "TextWidget.h"

#include <QHBoxLayout>

TextWidget::TextWidget(QWidget* parent, const QString& text) :
    QWidget{parent},
    label{new QLabel(text, this)}
{
    QHBoxLayout* layout {new QHBoxLayout};
    layout->addWidget(label);
    setLayout(layout);
}

void TextWidget::setText(const QString& text) const
{
    label->setText(text);
}
