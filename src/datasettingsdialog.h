#ifndef DATASETTINGSDIALOG_H
#define DATASETTINGSDIALOG_H

#include "serialtransceiver.h"

#include <qdialog.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>

/**
 * @brief The dialog to select a data type
 *
 * Available data types depend on the current chart type.
 * Chart (u8, u16, u32, u64, i8, i16, i32, i64, f32, f64).
 * Spectrum (u8, u16, u32, u64)
 *
 */
class DataSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataSettingsDialog(QWidget *parent = nullptr);

    auto getCurrentDataType() const { return currentDataType; }

private:
    SerialTransceiver::DataTypes currentDataType{ SerialTransceiver::DataTypes::u8 };
    QButtonGroup *buttonGroup{ new QButtonGroup{ this } };

    QRadioButton *button_u8{ new QRadioButton{ "unsigned 8 bit" } };
    QRadioButton *button_u16{ new QRadioButton{ "unsigned 16 bit" } };
    QRadioButton *button_u32{ new QRadioButton{ "unsigned 32 bit" } };
    QRadioButton *button_u64{ new QRadioButton{ "unsigned 64 bit" } };
    QRadioButton *button_i8{ new QRadioButton{ "signed 8 bit" } };
    QRadioButton *button_i16{ new QRadioButton{ "signed 16 bit" } };
    QRadioButton *button_i32{ new QRadioButton{ "signed 32 bit" } };
    QRadioButton *button_i64{ new QRadioButton{ "signed 64 bit" } };
    QRadioButton *button_f32{ new QRadioButton{ "float 32 bit" } };
    QRadioButton *button_f64{ new QRadioButton{ "double 64 bit" } };

private slots:
    void ok();
    void cancel();

signals:
    void dataTypeChanged(bool isUnsigned);
};

#endif // DATASETTINGSDIALOG_H
