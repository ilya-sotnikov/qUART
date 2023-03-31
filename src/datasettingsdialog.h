#ifndef DATASETTINGSDIALOG_H
#define DATASETTINGSDIALOG_H

#include "serialtransceiver.h"

#include <qdialog.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>

/**
 * @brief The dialog to select a data type
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
    SerialTransceiver::DataTypes dataType{ SerialTransceiver::DataTypes::ascii };
    QDataStream::ByteOrder byteOrder{ QDataStream::LittleEndian };
    QButtonGroup *dataTypeButtonGroup{ new QButtonGroup{ this } };
    QButtonGroup *byteOrderButtonGroup{ new QButtonGroup{ this } };

private slots:
    void ok();
    void cancel();
};

#endif // DATASETTINGSDIALOG_H
