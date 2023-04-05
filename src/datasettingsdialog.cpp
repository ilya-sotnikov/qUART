#include "datasettingsdialog.h"
#include "serialtransceiver.h"

#include <qboxlayout.h>
#include <qdialogbuttonbox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qsettings.h>

using namespace Qt::Literals::StringLiterals;

DataSettingsDialog::DataSettingsDialog(QWidget *parent) : QDialog{ parent }
{
    auto layout{ new QVBoxLayout{ this } };

    auto buttonBox{ new QDialogButtonBox{ QDialogButtonBox::Cancel | QDialogButtonBox::Ok,
                                          Qt::Horizontal, this } };

    auto settingsBox{ new QGroupBox{ u"Data settings"_s } };
    auto settingsLayout{ new QGridLayout{ settingsBox } };

    auto dataFormatLabel{ new QLabel{ u"Data format"_s } };
    dataTypeBox->addItem(u"ASCII"_s, static_cast<int>(SerialTransceiver::DataTypes::ascii));
    dataTypeBox->addItem(u"unsigned 8 bit"_s, static_cast<int>(SerialTransceiver::DataTypes::u8));
    dataTypeBox->addItem(u"unsigned 16 bit"_s, static_cast<int>(SerialTransceiver::DataTypes::u16));
    dataTypeBox->addItem(u"unsigned 32 bit"_s, static_cast<int>(SerialTransceiver::DataTypes::u32));
    dataTypeBox->addItem(u"unsigned 64 bit"_s, static_cast<int>(SerialTransceiver::DataTypes::u64));
    dataTypeBox->addItem(u"signed 8 bit"_s, static_cast<int>(SerialTransceiver::DataTypes::i8));
    dataTypeBox->addItem(u"signed 16 bit"_s, static_cast<int>(SerialTransceiver::DataTypes::i16));
    dataTypeBox->addItem(u"signed 32 bit"_s, static_cast<int>(SerialTransceiver::DataTypes::i32));
    dataTypeBox->addItem(u"signed 64 bit"_s, static_cast<int>(SerialTransceiver::DataTypes::i64));
    dataTypeBox->addItem(u"float 32 bit"_s, static_cast<int>(SerialTransceiver::DataTypes::f32));
    dataTypeBox->addItem(u"double 64 bit"_s, static_cast<int>(SerialTransceiver::DataTypes::f64));
    settingsLayout->addWidget(dataFormatLabel, 0, 0);
    settingsLayout->addWidget(dataTypeBox, 0, 1);

    auto byteOrderLabel{ new QLabel{ u"Byte order (for numeric)"_s } };
    byteOrderBox->addItem(u"little-endian"_s, static_cast<int>(QDataStream::LittleEndian));
    byteOrderBox->addItem(u"big-endian"_s, static_cast<int>(QDataStream::BigEndian));
    settingsLayout->addWidget(byteOrderLabel, 1, 0);
    settingsLayout->addWidget(byteOrderBox, 1, 1);

    layout->addWidget(settingsBox, 0, Qt::AlignTop);
    layout->addWidget(buttonBox, 0, Qt::AlignBottom);

    setWindowTitle(u"Data settings"_s);
    setModal(true);

    loadSettings();

    connect(buttonBox, &QDialogButtonBox::accepted, this, &DataSettingsDialog::ok);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &DataSettingsDialog::cancel);
}

/**
 * @brief Updates the current settings according to the selected settings
 *
 */
void DataSettingsDialog::updateSettings()
{
    settings.dataType = dataTypeBox->currentData().value<SerialTransceiver::DataTypes>();
    settings.byteOrder = byteOrderBox->currentData().value<QDataStream::ByteOrder>();
}

/**
 * @brief Updates the combo box current index according to the data
 *
 * @tparam T Data type
 * @param comboBox QComboBox to be updated
 * @param data Data from QComboBox
 */
template<typename T>
void DataSettingsDialog::updateIndex(QComboBox *comboBox, T data)
{
    auto index{ comboBox->findData(data) };
    if (index != -1)
        comboBox->setCurrentIndex(index);
}

/**
 * @brief Loads settings from a file and checks the correct boxes
 *
 */
void DataSettingsDialog::loadSettings()
{
    QSettings settingsFile;
    settingsFile.beginGroup(u"data"_s);
    settings.dataType = static_cast<SerialTransceiver::DataTypes>(
            settingsFile.value(u"type"_s, static_cast<int>(SerialTransceiver::DataTypes::ascii))
                    .toInt());
    settings.byteOrder = static_cast<QDataStream::ByteOrder>(
            settingsFile.value(u"byteOrder"_s, QDataStream::LittleEndian).toInt());
    settingsFile.endGroup();

    updateIndex(dataTypeBox, static_cast<int>(settings.dataType));
    updateIndex(byteOrderBox, static_cast<int>(settings.byteOrder));
}

/**
 * @brief Saves settings to a file
 *
 */
void DataSettingsDialog::saveSettings()
{
    QSettings settingsFile;

    settingsFile.beginGroup(u"data"_s);
    settingsFile.setValue(u"type"_s, static_cast<int>(settings.dataType));
    settingsFile.setValue(u"byteOrder"_s, settings.byteOrder);
    settingsFile.endGroup();
}

/**
 * @brief Updates the current settings, saves them to a file and hides DataSettingsDialog
 *
 */
void DataSettingsDialog::ok()
{

    updateSettings();
    saveSettings();
    hide();
}

/**
 * @brief Hides DataSettingsDialog without changing anything
 *
 */
void DataSettingsDialog::cancel()
{
    hide();
}
