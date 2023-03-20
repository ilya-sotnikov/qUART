#ifndef DATASETTINGSDIALOG_H
#define DATASETTINGSDIALOG_H

#include "serialtransceiver.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QVBoxLayout>

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
    SerialTransceiver::DataTypes getCurrentDataType() { return currentDataType; }
    void hideAdditionalDataTypes() const;
    void showAdditionalDataTypes() const;

private:
    SerialTransceiver::DataTypes currentDataType{ SerialTransceiver::u8 };

    QButtonGroup *buttonGroup{ nullptr };

private slots:
    void ok();
    void cancel();

signals:
    void dataTypeChanged(SerialTransceiver::DataTypes dataType);
};

#endif // DATASETTINGSDIALOG_H
