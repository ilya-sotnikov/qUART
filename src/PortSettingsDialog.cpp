#include "PortSettingsDialog.h"
#include "ui_PortSettingsDialog.h"

#include <QSerialPortInfo>

PortSettingsDialog::PortSettingsDialog(QWidget* parent) :
    QDialog{parent},
    ui{new Ui::PortSettingsDialog},
    portsInfoDialog{new PortsInfoDialog{this}}
{
    ui->setupUi(this);
    setModal(true);
    connect(ui->portsInfoButton, &QPushButton::pressed, portsInfoDialog, &PortsInfoDialog::show);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &PortSettingsDialog::ok);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &PortSettingsDialog::cancel);
    connect(ui->serialPortBox, &QComboBox::currentIndexChanged, this, &PortSettingsDialog::checkCustomPath);
    enumeratePorts();
    fillSettings();
    updateSettings();
}

PortSettingsDialog::~PortSettingsDialog()
{
    delete ui;
}

void PortSettingsDialog::fillSettings() const
{
    ui->serialPortBox->setCurrentIndex(0);

    ui->baudRateBox->addItem(QStringLiteral("1200"), QSerialPort::Baud1200);
    ui->baudRateBox->addItem(QStringLiteral("2400"), QSerialPort::Baud2400);
    ui->baudRateBox->addItem(QStringLiteral("4800"), QSerialPort::Baud4800);
    ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudRateBox->addItem(QStringLiteral("57600"), QSerialPort::Baud57600);
    ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->baudRateBox->setCurrentIndex(3);

    ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->dataBitsBox->setCurrentIndex(3);

    ui->parityBox->addItem(QStringLiteral("None"), QSerialPort::NoParity);
    ui->parityBox->addItem(QStringLiteral("Even"), QSerialPort::EvenParity);
    ui->parityBox->addItem(QStringLiteral("Odd"), QSerialPort::OddParity);
    ui->parityBox->addItem(QStringLiteral("Mark"), QSerialPort::MarkParity);
    ui->parityBox->addItem(QStringLiteral("Space"), QSerialPort::SpaceParity);
    ui->parityBox->setCurrentIndex(0);

    ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBitsBox->addItem(QStringLiteral("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);
    ui->stopBitsBox->setCurrentIndex(0);

    ui->flowControlBox->addItem(QStringLiteral("None"), QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem(QStringLiteral("RTS/CTS"), QSerialPort::HardwareControl);
    ui->flowControlBox->addItem(QStringLiteral("XON/XOFF"), QSerialPort::SoftwareControl);
    ui->flowControlBox->setCurrentIndex(0);
}

void PortSettingsDialog::enumeratePorts() const
{
    ui->serialPortBox->clear();
    const auto ports = QSerialPortInfo::availablePorts();
    for (const auto &port : ports) {
        ui->serialPortBox->addItem(port.portName(), port.portName());
    }
    ui->serialPortBox->addItem("Custom");
}

void PortSettingsDialog::checkCustomPath(const int index) const
{
    const bool isCustomPath = ui->serialPortBox->itemData(index).isNull();
    ui->serialPortBox->setEditable(isCustomPath);
    if (isCustomPath)
        ui->serialPortBox->clearEditText();
}

void PortSettingsDialog::updateSettings()
{
    currentSettings.name = ui->serialPortBox->currentText();
    currentSettings.baudRate = ui->baudRateBox->currentData().value<QSerialPort::BaudRate>();
    currentSettings.dataBits = ui->dataBitsBox->currentData().value<QSerialPort::DataBits>();
    currentSettings.parity = ui->dataBitsBox->currentData().value<QSerialPort::Parity>();
    currentSettings.stopBits = ui->dataBitsBox->currentData().value<QSerialPort::StopBits>();
    currentSettings.flowControl = ui->dataBitsBox->currentData().value<QSerialPort::FlowControl>();
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

