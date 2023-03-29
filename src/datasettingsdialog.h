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

    auto getDataType() const { return dataType; }
    auto getByteOrder() const { return byteOrder; }

private:
    SerialTransceiver::DataTypes dataType{ SerialTransceiver::DataTypes::u8 };
    QDataStream::ByteOrder byteOrder{ QDataStream::LittleEndian };
    QButtonGroup *dataTypeButtonGroup{ new QButtonGroup{ this } };
    QButtonGroup *byteOrderButtonGroup{ new QButtonGroup{ this } };

private slots:
    void ok();
    void cancel();

signals:
    void dataTypeChanged(bool isUnsigned);
};

#endif // DATASETTINGSDIALOG_H
