#include "portsettingsdialog.h"

#include <qboxlayout.h>
#include <qdialogbuttonbox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qserialportinfo.h>

/**
 * @brief Construct a new PortSettingsDialog object
 *
 * @param parent
 */
PortSettingsDialog::PortSettingsDialog(QWidget *parent) : QDialog{ parent }
{
    auto layout{ new QVBoxLayout{ this } };
    auto buttonBox{ new QDialogButtonBox{ QDialogButtonBox::Cancel | QDialogButtonBox::Ok,
                                          Qt::Horizontal, this } };

    auto selectBox{ new QGroupBox{ "Serial port configuration", this } };

    auto controlsLayout{ new QGridLayout{ selectBox } };

    auto portsInfoLabel{ new QLabel{ "Ports info", selectBox } };
    controlsLayout->addWidget(portsInfoLabel, 0, 0, 1, 1);
    auto portsInfoButton{ new QPushButton{ "Show", selectBox } };
    controlsLayout->addWidget(portsInfoButton, 0, 1, 1, 1);

    auto serialPortLabel{ new QLabel{ "Serial port", selectBox } };
    controlsLayout->addWidget(serialPortLabel, 1, 0, 1, 1);
    controlsLayout->addWidget(serialPortBox, 1, 1, 1, 1);

    auto baudRateLabel{ new QLabel{ "Baud rate", selectBox } };
    controlsLayout->addWidget(baudRateLabel, 2, 0, 1, 1);
    controlsLayout->addWidget(baudRateBox, 2, 1, 1, 1);

    auto dataBitsLabel{ new QLabel{ "Data bits", selectBox } };
    controlsLayout->addWidget(dataBitsLabel, 3, 0, 1, 1);
    controlsLayout->addWidget(dataBitsBox, 3, 1, 1, 1);

    auto parityLabel{ new QLabel{ "Parity", selectBox } };
    controlsLayout->addWidget(parityLabel, 4, 0, 1, 1);
    controlsLayout->addWidget(parityBox, 4, 1, 1, 1);

    auto stopBitsLabel{ new QLabel{ "Stop bits", selectBox } };
    controlsLayout->addWidget(stopBitsLabel, 5, 0, 1, 1);
    controlsLayout->addWidget(stopBitsBox, 5, 1, 1, 1);

    auto flowControlLabel{ new QLabel{ "Flow control", selectBox } };
    controlsLayout->addWidget(flowControlLabel, 6, 0, 1, 1);
    controlsLayout->addWidget(flowControlBox, 6, 1, 1, 1);

    layout->addWidget(selectBox, 0, Qt::AlignTop);
    layout->addWidget(buttonBox, 0, Qt::AlignBottom);

    setWindowTitle("Port settings");
    setModal(true);

    connect(portsInfoButton, &QPushButton::pressed, portsInfoDialog, &PortsInfoDialog::show);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &PortSettingsDialog::ok);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &PortSettingsDialog::cancel);
    connect(serialPortBox, &QComboBox::currentIndexChanged, this,
            &PortSettingsDialog::checkCustomPath);
    enumeratePorts();
    fillSettings();
    updateSettings();
}

/**
 * @brief Fill PortSettingsDialog settings and select the most common ones
 *
 */
void PortSettingsDialog::fillSettings() const
{
    serialPortBox->setCurrentIndex(0);
    baudRateBox->addItem(QStringLiteral("1200"), QSerialPort::Baud1200);
    baudRateBox->addItem(QStringLiteral("2400"), QSerialPort::Baud2400);
    baudRateBox->addItem(QStringLiteral("4800"), QSerialPort::Baud4800);
    baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    baudRateBox->addItem(QStringLiteral("57600"), QSerialPort::Baud57600);
    baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    baudRateBox->setCurrentIndex(3);
    dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    dataBitsBox->setCurrentIndex(3);
    parityBox->addItem(QStringLiteral("None"), QSerialPort::NoParity);
    parityBox->addItem(QStringLiteral("Even"), QSerialPort::EvenParity);
    parityBox->addItem(QStringLiteral("Odd"), QSerialPort::OddParity);
    parityBox->addItem(QStringLiteral("Mark"), QSerialPort::MarkParity);
    parityBox->addItem(QStringLiteral("Space"), QSerialPort::SpaceParity);
    parityBox->setCurrentIndex(0);

    stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    stopBitsBox->addItem(QStringLiteral("1.5"), QSerialPort::OneAndHalfStop);
#endif
    stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);
    stopBitsBox->setCurrentIndex(0);
    flowControlBox->addItem(QStringLiteral("None"), QSerialPort::NoFlowControl);
    flowControlBox->addItem(QStringLiteral("RTS/CTS"), QSerialPort::HardwareControl);
    flowControlBox->addItem(QStringLiteral("XON/XOFF"), QSerialPort::SoftwareControl);
    flowControlBox->setCurrentIndex(0);
}

/**
 * @brief Enumerate available serial ports
 *
 * Note that if you connect a new serial port you must restart the program.
 *
 */
void PortSettingsDialog::enumeratePorts() const
{
    serialPortBox->clear();
    const auto ports{ QSerialPortInfo::availablePorts() };
    for (const auto &port : ports) {
        serialPortBox->addItem(port.portName(), port.portName());
    }
    serialPortBox->addItem("Custom");
}

/**
 * @brief Allow to type in a custom serial port name
 *
 * @param index
 */
void PortSettingsDialog::checkCustomPath(const int index) const
{
    const auto isCustomPath{ serialPortBox->itemData(index).isNull() };
    serialPortBox->setEditable(isCustomPath);
    if (isCustomPath)
        serialPortBox->clearEditText();
}

/**
 * @brief Update the current settings according to the selected
 *
 */
void PortSettingsDialog::updateSettings()
{
    currentSettings.name = serialPortBox->currentText();
    currentSettings.baudRate = baudRateBox->currentData().value<QSerialPort::BaudRate>();
    currentSettings.dataBits = dataBitsBox->currentData().value<QSerialPort::DataBits>();
    currentSettings.parity = parityBox->currentData().value<QSerialPort::Parity>();
    currentSettings.stopBits = stopBitsBox->currentData().value<QSerialPort::StopBits>();
    currentSettings.flowControl = flowControlBox->currentData().value<QSerialPort::FlowControl>();
}

/**
 * @brief Update the current settings according to the selected and hide
 * PortSettingsDialog
 *
 */
void PortSettingsDialog::ok()
{
    updateSettings();
    hide();
}

/**
 * @brief Hide PortSettingsDialog without updating the current settings
 *
 */
void PortSettingsDialog::cancel()
{
    hide();
}
