#ifndef PORTSETTINGSDIALOG_H
#define PORTSETTINGSDIALOG_H

#include "PortsInfoDialog.h"

#include <QtWidgets>
#include <QDialog>
#include <QSerialPort>

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
    PortsInfoDialog *portsInfoDialog{ nullptr };
    Settings currentSettings;

    void fillSettings() const;
    void enumeratePorts() const;
    void checkCustomPath(const int index) const;
    void updateSettings();

    QVBoxLayout *layout{ nullptr };
    QDialogButtonBox *buttonBox{ nullptr };
    QGroupBox *selectBox{ nullptr };
    QGridLayout *controlsLayout{ nullptr };

    QLabel *portsInfoLabel{ nullptr };
    QPushButton *portsInfoButton{ nullptr };
    QLabel *serialPortLabel{ nullptr };
    QComboBox *serialPortBox{ nullptr };
    QLabel *baudRateLabel{ nullptr };
    QComboBox *baudRateBox{ nullptr };
    QLabel *dataBitsLabel{ nullptr };
    QComboBox *dataBitsBox{ nullptr };
    QLabel *parityLabel{ nullptr };
    QComboBox *parityBox{ nullptr };
    QLabel *stopBitsLabel{ nullptr };
    QComboBox *stopBitsBox{ nullptr };
    QLabel *flowControlLabel{ nullptr };
    QComboBox *flowControlBox{ nullptr };

private slots:
    void ok();
    void cancel();
};

#endif // PORTSETTINGSDIALOG_H
