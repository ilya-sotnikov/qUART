#ifndef DATASETTINGSDIALOG_H
#define DATASETTINGSDIALOG_H

#include "SerialTransceiver.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QVBoxLayout>

/**
 * @brief The dialog to select a data type
 * 
 */
class DataSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataSettingsDialog(QWidget *parent = nullptr);
    SerialTransceiver::DataTypes getCurrentDataType() { return currentDataType; }
    void hideAdditionalDataTypes() const;
    void showAdditionalDataTypes() const;

private:
    SerialTransceiver::DataTypes currentDataType{SerialTransceiver::u8};

    QRadioButton *button_u8{nullptr};
    QRadioButton *button_u16{nullptr};
    QRadioButton *button_u32{nullptr};
    QRadioButton *button_u64{nullptr};
    QRadioButton *button_i8{nullptr};
    QRadioButton *button_i16{nullptr};
    QRadioButton *button_i32{nullptr};
    QRadioButton *button_i64{nullptr};
    QRadioButton *button_f32{nullptr};
    QRadioButton *button_f64{nullptr};
    QButtonGroup *buttonGroup{nullptr};

private slots:
    void ok();
    void cancel();
};

#endif // DATASETTINGSDIALOG_H
