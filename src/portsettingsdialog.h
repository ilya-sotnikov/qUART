#ifndef PORTSETTINGSDIALOG_H
#define PORTSETTINGSDIALOG_H

#include "portsinfodialog.h"

#include <qcombobox.h>
#include <qdialog.h>
#include <qserialport.h>

class PortSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PortSettingsDialog(QWidget *parent = nullptr);

    struct Settings
    {
        QString name;
        QSerialPort::BaudRate baudRate;
        QSerialPort::DataBits dataBits;
        QSerialPort::Parity parity;
        QSerialPort::StopBits stopBits;
        QSerialPort::FlowControl flowControl;
    };
    const auto &getCurrentSettings() const { return currentSettings; };

private:
    PortsInfoDialog *portsInfoDialog{ new PortsInfoDialog{ this } };
    Settings currentSettings;

    void fillSettings() const;
    void enumeratePorts() const;
    void checkCustomPath(const int index) const;
    void updateSettings();

    QComboBox *serialPortBox{ new QComboBox{ this } };
    QComboBox *baudRateBox{ new QComboBox{ this } };
    QComboBox *dataBitsBox{ new QComboBox{ this } };
    QComboBox *parityBox{ new QComboBox{ this } };
    QComboBox *stopBitsBox{ new QComboBox{ this } };
    QComboBox *flowControlBox{ new QComboBox{ this } };

private slots:
    void ok();
    void cancel();
};

#endif // PORTSETTINGSDIALOG_H
