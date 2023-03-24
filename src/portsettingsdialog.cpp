#include "portsettingsdialog.h"

#include <qboxlayout.h>
#include <qdialogbuttonbox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qserialportinfo.h>
#include <qsettings.h>

/**
 * @brief Constructs a new PortSettingsDialog object
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
    loadSettings();
    updateSettings();
}

/**
 * @brief Fills PortSettingsDialog settings
 *
 */
void PortSettingsDialog::fillSettings() const
{
    baudRateBox->addItem(QStringLiteral("1200"), QSerialPort::Baud1200);
    baudRateBox->addItem(QStringLiteral("2400"), QSerialPort::Baud2400);
    baudRateBox->addItem(QStringLiteral("4800"), QSerialPort::Baud4800);
    baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    baudRateBox->addItem(QStringLiteral("57600"), QSerialPort::Baud57600);
    baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);

    dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);

    parityBox->addItem(QStringLiteral("None"), QSerialPort::NoParity);
    parityBox->addItem(QStringLiteral("Even"), QSerialPort::EvenParity);
    parityBox->addItem(QStringLiteral("Odd"), QSerialPort::OddParity);
    parityBox->addItem(QStringLiteral("Mark"), QSerialPort::MarkParity);
    parityBox->addItem(QStringLiteral("Space"), QSerialPort::SpaceParity);

    stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    stopBitsBox->addItem(QStringLiteral("1.5"), QSerialPort::OneAndHalfStop);
#endif
    stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    flowControlBox->addItem(QStringLiteral("None"), QSerialPort::NoFlowControl);
    flowControlBox->addItem(QStringLiteral("RTS/CTS"), QSerialPort::HardwareControl);
    flowControlBox->addItem(QStringLiteral("XON/XOFF"), QSerialPort::SoftwareControl);
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
    serialPortBox->addItem("Custom");
}

/**
 * @brief Allows to type in a custom serial port name
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
void PortSettingsDialog::updateIndex(QComboBox *comboBox, T data)
{
    auto index{ comboBox->findData(data) };
    if (index != -1)
        comboBox->setCurrentIndex(index);
}

/**
 * @brief Loads settings from a file and checks the correct boxes
 *
 */
void PortSettingsDialog::loadSettings()
{
    QSettings settingsFile;

    settingsFile.beginGroup("port");

    settings.name = settingsFile.value("name", "").toString();
    settings.baudRate =
            static_cast<QSerialPort::BaudRate>(settingsFile.value("baudRate", "9600").toInt());
    settings.dataBits =
            static_cast<QSerialPort::DataBits>(settingsFile.value("dataBits", "8").toInt());
    settings.parity = static_cast<QSerialPort::Parity>(settingsFile.value("parity", "0").toInt());
    settings.stopBits =
            static_cast<QSerialPort::StopBits>(settingsFile.value("stopBits", "1").toInt());
    settings.flowControl =
            static_cast<QSerialPort::FlowControl>(settingsFile.value("flowControl", "0").toInt());

    auto portNameIndex{ serialPortBox->findData(settings.name) };
    if (portNameIndex == -1) {
        serialPortBox->setEditable(true);
        serialPortBox->setEditText(settings.name);
    } else {
        serialPortBox->setCurrentIndex(portNameIndex);
    }

    settingsFile.endGroup();

    updateIndex(baudRateBox, settings.baudRate);
    updateIndex(dataBitsBox, settings.dataBits);
    updateIndex(parityBox, settings.parity);
    updateIndex(stopBitsBox, settings.stopBits);
    updateIndex(flowControlBox, settings.flowControl);
}

/**
 * @brief Saves serial port settings to a file
 *
 */
void PortSettingsDialog::saveSettings()
{
    QSettings settingsFile;

    settingsFile.beginGroup("port");
    settingsFile.setValue("name", settings.name);
    settingsFile.setValue("baudRate", settings.baudRate);
    settingsFile.setValue("dataBits", settings.dataBits);
    settingsFile.setValue("parity", settings.parity);
    settingsFile.setValue("stopBits", settings.stopBits);
    settingsFile.setValue("flowControl", settings.flowControl);
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
