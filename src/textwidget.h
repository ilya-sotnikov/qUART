#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H

#include <qlabel.h>

/**
 * @brief Text widget to display in the status bar
 *
 */
class TextWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TextWidget(QWidget *parent = nullptr, const QString &text = "");
    void setText(const QString &text) const { label->setText(text); };

private:
    QLabel *label{ nullptr };
};

#endif // TEXTWIDGET_H
