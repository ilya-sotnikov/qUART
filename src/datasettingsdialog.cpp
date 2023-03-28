#include "datasettingsdialog.h"
#include "serialtransceiver.h"

#include <qboxlayout.h>
#include <qdialogbuttonbox.h>
#include <qgroupbox.h>
#include <qsizepolicy.h>

/**
 * @brief Constructs a new DataSettingsDialog object
 *
 * @param parent
 */
DataSettingsDialog::DataSettingsDialog(QWidget *parent) : QDialog{ parent }

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
    auto button_ascii{ new QRadioButton{ "ASCII" } };

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
    buttonsLayout->addWidget(button_ascii);
    groupBox->setLayout(buttonsLayout);

    buttonGroup->addButton(button_u8, static_cast<int>(SerialTransceiver::DataTypes::u8));
    buttonGroup->addButton(button_u16, static_cast<int>(SerialTransceiver::DataTypes::u16));
    buttonGroup->addButton(button_u32, static_cast<int>(SerialTransceiver::DataTypes::u32));
    buttonGroup->addButton(button_u64, static_cast<int>(SerialTransceiver::DataTypes::u64));
    buttonGroup->addButton(button_i8, static_cast<int>(SerialTransceiver::DataTypes::i8));
    buttonGroup->addButton(button_i16, static_cast<int>(SerialTransceiver::DataTypes::i16));
    buttonGroup->addButton(button_i32, static_cast<int>(SerialTransceiver::DataTypes::i32));
    buttonGroup->addButton(button_i64, static_cast<int>(SerialTransceiver::DataTypes::i64));
    buttonGroup->addButton(button_f32, static_cast<int>(SerialTransceiver::DataTypes::f32));
    buttonGroup->addButton(button_f64, static_cast<int>(SerialTransceiver::DataTypes::f64));
    buttonGroup->addButton(button_ascii, static_cast<int>(SerialTransceiver::DataTypes::ascii));
    buttonGroup->button(static_cast<int>(currentDataType))->click();

    layout->addWidget(groupBox, 0, Qt::AlignTop);
    layout->addWidget(buttonBox, 0, Qt::AlignBottom);

    setWindowTitle("Data settings");
    setModal(true);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &DataSettingsDialog::ok);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &DataSettingsDialog::cancel);
}

/**
 * @brief Gets the selected data type and hides DataSettingsDialog
 *
 * If the selected data type is not unsigned appending data to the spectrum is not possible
 *
 */
void DataSettingsDialog::ok()
{
    auto previousDataType{ currentDataType };
    bool isUnsigned{ false };
    currentDataType = static_cast<SerialTransceiver::DataTypes>(buttonGroup->checkedId());

    if (previousDataType != currentDataType) {
        if (buttonGroup->checkedId() > static_cast<int>(SerialTransceiver::DataTypes::u64))
            isUnsigned = false;
        else
            isUnsigned = true;

        emit dataTypeChanged(isUnsigned);
    }

    hide();
}

/**
 * @brief Hides DataSettingsDialog without changing the current data type
 *
 */
void DataSettingsDialog::cancel()
{
    buttonGroup->button(static_cast<int>(currentDataType))->click();
    hide();
}
