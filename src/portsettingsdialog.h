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
    const auto &getSettings() const { return settings; };

private:
    PortsInfoDialog *portsInfoDialog{ new PortsInfoDialog{ this } };
    Settings settings;
    QComboBox *serialPortBox{ new QComboBox{ this } };
    QComboBox *baudRateBox{ new QComboBox{ this } };
    QComboBox *dataBitsBox{ new QComboBox{ this } };
    QComboBox *parityBox{ new QComboBox{ this } };
    QComboBox *stopBitsBox{ new QComboBox{ this } };
    QComboBox *flowControlBox{ new QComboBox{ this } };

    void fillSettings() const;
    void enumeratePorts() const;
    void checkCustomPath(const int index) const;
    void updateSettings();
    void loadSettings();
    void saveSettings();
    template<typename T>
    void updateIndex(QComboBox *comboBox, T data);

private slots:
    void ok();
    void cancel();
};

#endif // PORTSETTINGSDIALOG_H
