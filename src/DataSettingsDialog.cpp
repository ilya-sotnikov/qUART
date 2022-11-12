#include "DataSettingsDialog.h"

DataSettingsDialog::DataSettingsDialog(QWidget *parent) : QDialog{ parent }
{
    layout = new QVBoxLayout{ this };

    buttonBox = new QDialogButtonBox{ QDialogButtonBox::Cancel | QDialogButtonBox::Ok,
                                      Qt::Horizontal, this };

    buttonsLayout = new QVBoxLayout{ this };
    groupBox = new QGroupBox{ "Data format", this };
    button_u8 = new QRadioButton{ "unsigned 8 bit" };
    button_u16 = new QRadioButton{ "unsigned 16 bit" };
    button_u32 = new QRadioButton{ "unsigned 32 bit" };
    button_u64 = new QRadioButton{ "unsigned 64 bit" };
    button_i8 = new QRadioButton{ "signed 8 bit" };
    button_i16 = new QRadioButton{ "signed 16 bit" };
    button_i32 = new QRadioButton{ "signed 32 bit" };
    button_i64 = new QRadioButton{ "signed 64 bit" };
    button_f32 = new QRadioButton{ "float 32 bit" };
    button_f64 = new QRadioButton{ "double 64 bit" };
    buttonsLayout->addWidget(button_u8);
    buttonsLayout->addWidget(button_u16);
    buttonsLayout->addWidget(button_u32);
    buttonsLayout->addWidget(button_u64);
    buttonsLayout->addWidget(button_i8);
    buttonsLayout->addWidget(button_i16);
    buttonsLayout->addWidget(button_i32);
    buttonsLayout->addWidget(button_i64);
    buttonsLayout->addWidget(button_f32);
    buttonsLayout->addWidget(button_f64);
    groupBox->setLayout(buttonsLayout);

    hideAdditionalDataTypes();

    buttonGroup = new QButtonGroup{ groupBox };
    buttonGroup->addButton(button_u8, SerialTransceiver::u8);
    buttonGroup->addButton(button_u16, SerialTransceiver::u16);
    buttonGroup->addButton(button_u32, SerialTransceiver::u32);
    buttonGroup->addButton(button_u64, SerialTransceiver::u64);
    buttonGroup->addButton(button_i8, SerialTransceiver::i8);
    buttonGroup->addButton(button_i16, SerialTransceiver::i16);
    buttonGroup->addButton(button_i32, SerialTransceiver::i32);
    buttonGroup->addButton(button_i64, SerialTransceiver::i64);
    buttonGroup->addButton(button_f32, SerialTransceiver::f32);
    buttonGroup->addButton(button_f64, SerialTransceiver::f64);

    layout->addWidget(groupBox, 0, Qt::AlignTop);
    layout->addWidget(buttonBox, 0, Qt::AlignBottom);

    setWindowTitle("Data settings");
    setModal(true);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &DataSettingsDialog::ok);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &DataSettingsDialog::cancel);
}

void DataSettingsDialog::hideAdditionalDataTypes() const
{
    button_i8->hide();
    button_i16->hide();
    button_i32->hide();
    button_i64->hide();
    button_f32->hide();
    button_f64->hide();
    button_u8->click();
}

void DataSettingsDialog::showAdditionalDataTypes() const
{
    button_i8->show();
    button_i16->show();
    button_i32->show();
    button_i64->show();
    button_f32->show();
    button_f64->show();
}

void DataSettingsDialog::ok()
{
    currentDataType = static_cast<SerialTransceiver::DataTypes>(buttonGroup->checkedId());
    hide();
}

void DataSettingsDialog::cancel()
{
    hide();
}
