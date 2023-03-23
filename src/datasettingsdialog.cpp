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

/**
 * @brief Hides additional data types (i8, i16, i32, i64, f32, f64)
 *
 * This function is needed because a spectrum only supports unsigned int types
 */
void DataSettingsDialog::hideAdditionalDataTypes()
{
    button_i8->hide();
    button_i16->hide();
    button_i32->hide();
    button_i64->hide();
    button_f32->hide();
    button_f64->hide();

    // a hack to resize the dialog
    show();
    hide();

    button_u8->click();
}

/**
 * @brief Shows additional data types (i8, i16, i32, i64, f32, f64)
 *
 * This function is needed because a spectrum only supports unsigned int types
 * If the chart is in plot mode, all data types are available
 *
 */
void DataSettingsDialog::showAdditionalDataTypes() const
{
    button_i8->show();
    button_i16->show();
    button_i32->show();
    button_i64->show();
    button_f32->show();
    button_f64->show();
}
