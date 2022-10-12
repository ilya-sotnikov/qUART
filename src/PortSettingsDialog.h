#ifndef PORTSETTINGSDIALOG_H
#define PORTSETTINGSDIALOG_H

#include "PortsInfoDialog.h"

#include <QDialog>
#include <QSerialPort>

namespace Ui {
class PortSettingsDialog;
}

class PortSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PortSettingsDialog(QWidget *parent = nullptr);
    ~PortSettingsDialog();

    struct Settings {
        QString name;
        QSerialPort::BaudRate baudRate;
        QSerialPort::DataBits dataBits;
        QSerialPort::Parity parity;
        QSerialPort::StopBits stopBits;
        QSerialPort::FlowControl flowControl;
    };
    const auto& getCurrentSettings() const { return currentSettings; };

private:
    Ui::PortSettingsDialog *ui;
    PortsInfoDialog* portsInfoDialog {nullptr};
    Settings currentSettings;

    void fillSettings() const;
    void enumeratePorts() const;
    void checkCustomPath(const int index) const;
    void updateSettings();


private slots:
    void ok();
    void cancel();
};

#endif // PORTSETTINGSDIALOG_H
