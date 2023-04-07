#include "portsettingsdialog.h"

#include <qboxlayout.h>
#include <qdialogbuttonbox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qserialportinfo.h>
#include <qsettings.h>

using namespace Qt::Literals::StringLiterals;

PortSettingsDialog::PortSettingsDialog(QWidget *parent) : QDialog{ parent }
{
    const auto layout{ new QVBoxLayout{ this } };
    const auto buttonBox{ new QDialogButtonBox{ QDialogButtonBox::Cancel | QDialogButtonBox::Ok,
                                                Qt::Horizontal, this } };

    const auto settingsBox{ new QGroupBox{ u"Serial port configuration"_s } };

    const auto settingsLayout{ new QGridLayout{ settingsBox } };

    const auto portsInfoLabel{ new QLabel{ u"Ports info"_s } };
    settingsLayout->addWidget(portsInfoLabel, 0, 0);
    const auto portsInfoButton{ new QPushButton{ u"Show"_s } };
    settingsLayout->addWidget(portsInfoButton, 0, 1);

    const auto serialPortLabel{ new QLabel{ u"Serial port"_s } };
    settingsLayout->addWidget(serialPortLabel, 1, 0);
    settingsLayout->addWidget(serialPortBox, 1, 1);

    const auto baudRateLabel{ new QLabel{ u"Baud rate"_s } };
    settingsLayout->addWidget(baudRateLabel, 2, 0);
    settingsLayout->addWidget(baudRateBox, 2, 1);

    const auto dataBitsLabel{ new QLabel{ u"Data bits"_s } };
    settingsLayout->addWidget(dataBitsLabel, 3, 0);
    settingsLayout->addWidget(dataBitsBox, 3, 1);

    const auto parityLabel{ new QLabel{ u"Parity"_s } };
    settingsLayout->addWidget(parityLabel, 4, 0);
    settingsLayout->addWidget(parityBox, 4, 1);

    const auto stopBitsLabel{ new QLabel{ u"Stop bits"_s } };
    settingsLayout->addWidget(stopBitsLabel, 5, 0);
    settingsLayout->addWidget(stopBitsBox, 5, 1);

    const auto flowControlLabel{ new QLabel{ u"Flow control"_s } };
    settingsLayout->addWidget(flowControlLabel, 6, 0);
    settingsLayout->addWidget(flowControlBox, 6, 1);

    layout->addWidget(settingsBox, 0, Qt::AlignTop);
    layout->addWidget(buttonBox, 0, Qt::AlignBottom);

    setWindowTitle(u"Port settings"_s);
    setModal(true);

    connect(portsInfoButton, &QPushButton::pressed, portsInfoDialog, &PortsInfoDialog::show);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &PortSettingsDialog::ok);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &PortSettingsDialog::cancel);
    connect(serialPortBox, &QComboBox::currentIndexChanged, this,
            &PortSettingsDialog::checkCustomPath);
    enumeratePorts();
    fillSettings();
    loadSettings();
}

/**
 * @brief Fills PortSettingsDialog settings
 *
 */
void PortSettingsDialog::fillSettings() const
{
    baudRateBox->addItem(u"1200"_s, QSerialPort::Baud1200);
    baudRateBox->addItem(u"2400"_s, QSerialPort::Baud2400);
    baudRateBox->addItem(u"4800"_s, QSerialPort::Baud4800);
    baudRateBox->addItem(u"9600"_s, QSerialPort::Baud9600);
    baudRateBox->addItem(u"19200"_s, QSerialPort::Baud19200);
    baudRateBox->addItem(u"38400"_s, QSerialPort::Baud38400);
    baudRateBox->addItem(u"57600"_s, QSerialPort::Baud57600);
    baudRateBox->addItem(u"115200"_s, QSerialPort::Baud115200);

    dataBitsBox->addItem(u"5"_s, QSerialPort::Data5);
    dataBitsBox->addItem(u"6"_s, QSerialPort::Data6);
    dataBitsBox->addItem(u"7"_s, QSerialPort::Data7);
    dataBitsBox->addItem(u"8"_s, QSerialPort::Data8);

    parityBox->addItem(u"None"_s, QSerialPort::NoParity);
    parityBox->addItem(u"Even"_s, QSerialPort::EvenParity);
    parityBox->addItem(u"Odd"_s, QSerialPort::OddParity);
    parityBox->addItem(u"Mark"_s, QSerialPort::MarkParity);
    parityBox->addItem(u"Space"_s, QSerialPort::SpaceParity);

    stopBitsBox->addItem(u"1"_s, QSerialPort::OneStop);
#ifdef Q_OS_WIN
    stopBitsBox->addItem(u"1.5"_s, QSerialPort::OneAndHalfStop);
#endif
    stopBitsBox->addItem(u"2"_s, QSerialPort::TwoStop);

    flowControlBox->addItem(u"None"_s, QSerialPort::NoFlowControl);
    flowControlBox->addItem(u"RTS/CTS"_s, QSerialPort::HardwareControl);
    flowControlBox->addItem(u"XON/XOFF"_s, QSerialPort::SoftwareControl);
}

/**
 * @brief Enumerates available serial ports
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
    serialPortBox->addItem(u"Custom"_s);
}

/**
 * @brief Allows to type in a custom serial port name
 *
 * @param index An index to check
 */
void PortSettingsDialog::checkCustomPath(const int index) const
{
    const auto isCustomPath{ serialPortBox->itemData(index).isNull() };
    serialPortBox->setEditable(isCustomPath);
    if (isCustomPath)
        serialPortBox->clearEditText();
}

/**
 * @brief Updates the current settings according to the selected settings
 *
 */
void PortSettingsDialog::updateSettings()
{
    settings.name = serialPortBox->currentText();
    settings.baudRate = baudRateBox->currentData().value<QSerialPort::BaudRate>();
    settings.dataBits = dataBitsBox->currentData().value<QSerialPort::DataBits>();
    settings.parity = parityBox->currentData().value<QSerialPort::Parity>();
    settings.stopBits = stopBitsBox->currentData().value<QSerialPort::StopBits>();
    settings.flowControl = flowControlBox->currentData().value<QSerialPort::FlowControl>();
}

/**
 * @brief Updates the combo box current index according to the data
 *
 */
template<typename T>
void PortSettingsDialog::updateIndex(QComboBox *const comboBox, const T data) const
{
    const auto index{ comboBox->findData(data) };
    if (index != -1)
        comboBox->setCurrentIndex(index);
}

/**
 * @brief Updates indexes of all combo boxes
 *
 */
void PortSettingsDialog::updateIndexes() const
{
    const auto portNameIndex{ serialPortBox->findData(settings.name) };
    if (portNameIndex == -1) {
        serialPortBox->setEditable(true);
        serialPortBox->setEditText(settings.name);
    } else {
        serialPortBox->setCurrentIndex(portNameIndex);
    }

    updateIndex(baudRateBox, settings.baudRate);
    updateIndex(dataBitsBox, settings.dataBits);
    updateIndex(parityBox, settings.parity);
    updateIndex(stopBitsBox, settings.stopBits);
    updateIndex(flowControlBox, settings.flowControl);
}

/**
 * @brief Loads settings from a file
 *
 */
void PortSettingsDialog::loadSettings()
{
    QSettings settingsFile;

    settingsFile.beginGroup(u"port"_s);

    settings.name = settingsFile.value(u"name"_s, u""_s).toString();
    settings.baudRate = static_cast<QSerialPort::BaudRate>(
            settingsFile.value(u"baudRate"_s, u"9600"_s).toInt());
    settings.dataBits =
            static_cast<QSerialPort::DataBits>(settingsFile.value(u"dataBits"_s, u"8"_s).toInt());
    settings.parity =
            static_cast<QSerialPort::Parity>(settingsFile.value(u"parity"_s, u"0"_s).toInt());
    settings.stopBits =
            static_cast<QSerialPort::StopBits>(settingsFile.value(u"stopBits"_s, u"1"_s).toInt());
    settings.flowControl = static_cast<QSerialPort::FlowControl>(
            settingsFile.value(u"flowControl"_s, u"0"_s).toInt());

    settingsFile.endGroup();
}

/**
 * @brief Saves serial port settings to a file
 *
 */
void PortSettingsDialog::saveSettings() const
{
    QSettings settingsFile;

    settingsFile.beginGroup(u"port"_s);
    settingsFile.setValue(u"name"_s, settings.name);
    settingsFile.setValue(u"baudRate"_s, settings.baudRate);
    settingsFile.setValue(u"dataBits"_s, settings.dataBits);
    settingsFile.setValue(u"parity"_s, settings.parity);
    settingsFile.setValue(u"stopBits"_s, settings.stopBits);
    settingsFile.setValue(u"flowControl"_s, settings.flowControl);
    settingsFile.endGroup();
}

/**
 * @brief Updates the current settings, saves them to a file and hides PortSettingsDialog
 *
 */
void PortSettingsDialog::ok()
{
    updateSettings();
    saveSettings();
    hide();
}

/**
 * @brief Hides PortSettingsDialog without updating the current settings
 *
 */
void PortSettingsDialog::cancel()
{
    hide();
}
