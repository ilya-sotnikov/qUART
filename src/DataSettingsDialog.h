#ifndef DATASETTINGSDIALOG_H
#define DATASETTINGSDIALOG_H

#include "SerialTransceiver.h"

#include <QDialog>

namespace Ui {
class DataSettingsDialog;
}

class DataSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataSettingsDialog(QWidget* parent = nullptr);
    ~DataSettingsDialog();
    SerialTransceiver::DataTypes getCurrentDataType() { return currentDataType; }
    void hideAdditionalDataTypes() const;
    void showAdditionalDataTypes() const;

private:
    Ui::DataSettingsDialog *ui;
    SerialTransceiver::DataTypes currentDataType {SerialTransceiver::u8};

private slots:
    void ok();
    void cancel();
};

#endif // DATASETTINGSDIALOG_H
