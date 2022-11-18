#include "PortSettingsDialog.h"

#include <QSerialPortInfo>

PortSettingsDialog::PortSettingsDialog(QWidget *parent)
    : QDialog{parent}
    , portsInfoDialog{new PortsInfoDialog{this}}
{
    layout = new QVBoxLayout{this};
    buttonBox = new QDialogButtonBox{QDialogButtonBox::Cancel | QDialogButtonBox::Ok,
                                     Qt::Horizontal,
                                     this};

    selectBox = new QGroupBox{"Serial port configuration", this};

    controlsLayout = new QGridLayout{selectBox};

    portsInfoLabel = new QLabel{"Ports info", selectBox};
    controlsLayout->addWidget(portsInfoLabel, 0, 0, 1, 1);
    portsInfoButton = new QPushButton{"Show", selectBox};
    controlsLayout->addWidget(portsInfoButton, 0, 1, 1, 1);

    serialPortLabel = new QLabel{"Serial port", selectBox};
    controlsLayout->addWidget(serialPortLabel, 1, 0, 1, 1);
    serialPortBox = new QComboBox{selectBox};
    controlsLayout->addWidget(serialPortBox, 1, 1, 1, 1);

    baudRateLabel = new QLabel{"Baud rate", selectBox};
    controlsLayout->addWidget(baudRateLabel, 2, 0, 1, 1);
    baudRateBox = new QComboBox{selectBox};
    controlsLayout->addWidget(baudRateBox, 2, 1, 1, 1);

    dataBitsLabel = new QLabel{"Data bits", selectBox};
    controlsLayout->addWidget(dataBitsLabel, 3, 0, 1, 1);
    dataBitsBox = new QComboBox{selectBox};
    controlsLayout->addWidget(dataBitsBox, 3, 1, 1, 1);

    parityLabel = new QLabel{"Parity", selectBox};
    controlsLayout->addWidget(parityLabel, 4, 0, 1, 1);
    parityBox = new QComboBox{selectBox};
    controlsLayout->addWidget(parityBox, 4, 1, 1, 1);

    stopBitsLabel = new QLabel{"Stop bits", selectBox};
    controlsLayout->addWidget(stopBitsLabel, 5, 0, 1, 1);
    stopBitsBox = new QComboBox{selectBox};
    controlsLayout->addWidget(stopBitsBox, 5, 1, 1, 1);

    flowControlLabel = new QLabel{"Flow control", selectBox};
    controlsLayout->addWidget(flowControlLabel, 6, 0, 1, 1);
    flowControlBox = new QComboBox{selectBox};
    controlsLayout->addWidget(flowControlBox, 6, 1, 1, 1);

    layout->addWidget(selectBox, 0, Qt::AlignTop);
    layout->addWidget(buttonBox, 0, Qt::AlignBottom);

    setWindowTitle("Port settings");
    setModal(true);

    connect(portsInfoButton, &QPushButton::pressed, portsInfoDialog, &PortsInfoDialog::show);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &PortSettingsDialog::ok);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &PortSettingsDialog::cancel);
    connect(serialPortBox,
            &QComboBox::currentIndexChanged,
            this,
            &PortSettingsDialog::checkCustomPath);
    enumeratePorts();
    fillSettings();
    updateSettings();
}

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

void PortSettingsDialog::enumeratePorts() const
{
    serialPortBox->clear();
    const auto ports = QSerialPortInfo::availablePorts();
    for (const auto &port : ports) {
        serialPortBox->addItem(port.portName(), port.portName());
    }
    serialPortBox->addItem("Custom");
}

void PortSettingsDialog::checkCustomPath(const int index) const
{
    const bool isCustomPath = serialPortBox->itemData(index).isNull();
    serialPortBox->setEditable(isCustomPath);
    if (isCustomPath)
        serialPortBox->clearEditText();
}

void PortSettingsDialog::updateSettings()
{
    currentSettings.name = serialPortBox->currentText();
    currentSettings.baudRate = baudRateBox->currentData().value<QSerialPort::BaudRate>();
    currentSettings.dataBits = dataBitsBox->currentData().value<QSerialPort::DataBits>();
    currentSettings.parity = dataBitsBox->currentData().value<QSerialPort::Parity>();
    currentSettings.stopBits = dataBitsBox->currentData().value<QSerialPort::StopBits>();
    currentSettings.flowControl = dataBitsBox->currentData().value<QSerialPort::FlowControl>();
}

void PortSettingsDialog::ok()
{
    updateSettings();
    hide();
}

void PortSettingsDialog::cancel()
{
    hide();
}
