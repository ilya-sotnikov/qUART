#include "datasettingsdialog.h"

#include <QButtonGroup>

/**
 * @brief Construct a new DataSettingsDialog object
 *
 * @param parent
 */
DataSettingsDialog::DataSettingsDialog(QWidget *parent)
    : QDialog{ parent }, buttonGroup{ new QButtonGroup{ this } }

{
    auto layout{ new QVBoxLayout{ this } };

    auto buttonBox{ new QDialogButtonBox{ QDialogButtonBox::Cancel | QDialogButtonBox::Ok,
                                          Qt::Horizontal, this } };

    auto buttonsLayout{ new QVBoxLayout{} };
    auto groupBox{ new QGroupBox{ "Data format", this } };

    auto button_u8{ new QRadioButton{ "unsigned 8 bit" } };
    auto button_u16{ new QRadioButton{ "unsigned 16 bit" } };
    auto button_u32{ new QRadioButton{ "unsigned 32 bit" } };
    auto button_u64{ new QRadioButton{ "unsigned 64 bit" } };
    auto button_i8{ new QRadioButton{ "signed 8 bit" } };
    auto button_i16{ new QRadioButton{ "signed 16 bit" } };
    auto button_i32{ new QRadioButton{ "signed 32 bit" } };
    auto button_i64{ new QRadioButton{ "signed 64 bit" } };
    auto button_f32{ new QRadioButton{ "float 32 bit" } };
    auto button_f64{ new QRadioButton{ "double 64 bit" } };

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
    buttonGroup->button(currentDataType)->click();

    layout->addWidget(groupBox, 0, Qt::AlignTop);
    layout->addWidget(buttonBox, 0, Qt::AlignBottom);

    setWindowTitle("Data settings");
    setModal(true);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &DataSettingsDialog::ok);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &DataSettingsDialog::cancel);
}

/**
 * @brief Get the selected data type and hide DataSettingsDialog
 *
 */
void DataSettingsDialog::ok()
{
    auto previousDataType{ currentDataType };
    currentDataType = static_cast<SerialTransceiver::DataTypes>(buttonGroup->checkedId());
    if (previousDataType != currentDataType)
        emit dataTypeChanged(currentDataType);
    hide();
}

/**
 * @brief Hide DataSettingsDialog without changing the current data type
 *
 */
void DataSettingsDialog::cancel()
{
    buttonGroup->button(currentDataType)->click();
    hide();
}
