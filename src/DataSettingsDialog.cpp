#include "DataSettingsDialog.h"
#include "ui_DataSettingsDialog.h"

DataSettingsDialog::DataSettingsDialog(QWidget* parent) :
    QDialog{parent},
    ui{new Ui::DataSettingsDialog}
{
    ui->setupUi(this);
    setModal(true);
    hideAdditionalDataTypes();
    ui->buttonGroup->addButton(ui->button_u8,  SerialTransceiver::u8);
    ui->buttonGroup->addButton(ui->button_u16, SerialTransceiver::u16);
    ui->buttonGroup->addButton(ui->button_u32, SerialTransceiver::u32);
    ui->buttonGroup->addButton(ui->button_u64, SerialTransceiver::u64);
    ui->buttonGroup->addButton(ui->button_i8,  SerialTransceiver::i8);
    ui->buttonGroup->addButton(ui->button_i16, SerialTransceiver::i16);
    ui->buttonGroup->addButton(ui->button_i32, SerialTransceiver::i32);
    ui->buttonGroup->addButton(ui->button_i64, SerialTransceiver::i64);
    ui->buttonGroup->addButton(ui->button_f32, SerialTransceiver::f32);
    ui->buttonGroup->addButton(ui->button_f64, SerialTransceiver::f64);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &DataSettingsDialog::ok);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &DataSettingsDialog::cancel);
}

DataSettingsDialog::~DataSettingsDialog()
{
    delete ui;
}

void DataSettingsDialog::hideAdditionalDataTypes() const
{
    ui->button_i8->hide();
    ui->button_i16->hide();
    ui->button_i32->hide();
    ui->button_i64->hide();
    ui->button_f32->hide();
    ui->button_f64->hide();
    ui->button_u8->click();
}

void DataSettingsDialog::showAdditionalDataTypes() const
{
    ui->button_i8->show();
    ui->button_i16->show();
    ui->button_i32->show();
    ui->button_i64->show();
    ui->button_f32->show();
    ui->button_f64->show();
}

void DataSettingsDialog::ok()
{
    currentDataType = static_cast<SerialTransceiver::DataTypes>(ui->buttonGroup->checkedId());
    hide();
}

void DataSettingsDialog::cancel()
{
    hide();
}
