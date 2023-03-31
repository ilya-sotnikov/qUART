#include "datasettingsdialog.h"
#include "serialtransceiver.h"

#include <qboxlayout.h>
#include <qdialogbuttonbox.h>
#include <qgroupbox.h>
#include <qsettings.h>

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

    QSettings settingsFile;
    settingsFile.beginGroup("data");
    dataType = settingsFile.value("type", static_cast<int>(SerialTransceiver::DataTypes::ascii))
                       .value<SerialTransceiver::DataTypes>();

    byteOrder = settingsFile.value("byteOrder", QDataStream::LittleEndian)
                        .value<QDataStream::ByteOrder>();
    settingsFile.endGroup();

    auto dataFormatButtonsLayout{ new QVBoxLayout{} };
    auto dataFormatGroupBox{ new QGroupBox{ "Data format", this } };

    auto button_ascii{ new QRadioButton{ "ASCII" } };
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

    dataFormatButtonsLayout->addWidget(button_ascii);
    dataFormatButtonsLayout->addWidget(button_u8);
    dataFormatButtonsLayout->addWidget(button_u16);
    dataFormatButtonsLayout->addWidget(button_u32);
    dataFormatButtonsLayout->addWidget(button_u64);
    dataFormatButtonsLayout->addWidget(button_i8);
    dataFormatButtonsLayout->addWidget(button_i16);
    dataFormatButtonsLayout->addWidget(button_i32);
    dataFormatButtonsLayout->addWidget(button_i64);
    dataFormatButtonsLayout->addWidget(button_f32);
    dataFormatButtonsLayout->addWidget(button_f64);
    dataFormatGroupBox->setLayout(dataFormatButtonsLayout);

    dataTypeButtonGroup->addButton(button_ascii,
                                   static_cast<int>(SerialTransceiver::DataTypes::ascii));
    dataTypeButtonGroup->addButton(button_u8, static_cast<int>(SerialTransceiver::DataTypes::u8));
    dataTypeButtonGroup->addButton(button_u16, static_cast<int>(SerialTransceiver::DataTypes::u16));
    dataTypeButtonGroup->addButton(button_u32, static_cast<int>(SerialTransceiver::DataTypes::u32));
    dataTypeButtonGroup->addButton(button_u64, static_cast<int>(SerialTransceiver::DataTypes::u64));
    dataTypeButtonGroup->addButton(button_i8, static_cast<int>(SerialTransceiver::DataTypes::i8));
    dataTypeButtonGroup->addButton(button_i16, static_cast<int>(SerialTransceiver::DataTypes::i16));
    dataTypeButtonGroup->addButton(button_i32, static_cast<int>(SerialTransceiver::DataTypes::i32));
    dataTypeButtonGroup->addButton(button_i64, static_cast<int>(SerialTransceiver::DataTypes::i64));
    dataTypeButtonGroup->addButton(button_f32, static_cast<int>(SerialTransceiver::DataTypes::f32));
    dataTypeButtonGroup->addButton(button_f64, static_cast<int>(SerialTransceiver::DataTypes::f64));
    dataTypeButtonGroup->button(static_cast<int>(dataType))->click();

    auto byteOrderButtonsLayout{ new QVBoxLayout{} };
    auto byteOrderGroupBox{ new QGroupBox{ "Byte order (for numeric)", this } };

    auto buttonLittleEndian{ new QRadioButton{ "little-endian" } };
    auto buttonBigEndian{ new QRadioButton{ "big-endian" } };

    byteOrderButtonsLayout->addWidget(buttonLittleEndian);
    byteOrderButtonsLayout->addWidget(buttonBigEndian);
    byteOrderGroupBox->setLayout(byteOrderButtonsLayout);

    byteOrderButtonGroup->addButton(buttonLittleEndian,
                                    static_cast<int>(QDataStream::LittleEndian));
    byteOrderButtonGroup->addButton(buttonBigEndian, static_cast<int>(QDataStream::BigEndian));
    byteOrderButtonGroup->button(static_cast<int>(byteOrder))->click();

    layout->addWidget(dataFormatGroupBox, 0, Qt::AlignTop);
    layout->addWidget(byteOrderGroupBox, 0, Qt::AlignTop);
    layout->addWidget(buttonBox, 0, Qt::AlignBottom);

    setWindowTitle("Data settings");
    setModal(true);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &DataSettingsDialog::ok);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &DataSettingsDialog::cancel);
}

/**
 * @brief Gets the selected data type and hides DataSettingsDialog
 *
 */
void DataSettingsDialog::ok()
{
    QSettings settingsFile;

    dataType = static_cast<SerialTransceiver::DataTypes>(dataTypeButtonGroup->checkedId());
    byteOrder = static_cast<QDataStream::ByteOrder>(byteOrderButtonGroup->checkedId());

    settingsFile.beginGroup("data");
    settingsFile.setValue("type", static_cast<int>(dataType));
    settingsFile.setValue("byteOrder", byteOrder);
    settingsFile.endGroup();

    hide();
}

/**
 * @brief Hides DataSettingsDialog without changing the current data type
 *
 */
void DataSettingsDialog::cancel()
{
    dataTypeButtonGroup->button(static_cast<int>(dataType))->click();
    hide();
}
