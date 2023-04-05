#ifndef DATASETTINGSDIALOG_H
#define DATASETTINGSDIALOG_H

#include "serialtransceiver.h"

#include <qcombobox.h>
#include <qdialog.h>

/**
 * @brief A dialog with data settings (data type and byte order)
 *
 */
class DataSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataSettingsDialog(QWidget *parent = nullptr);

    struct Settings
    {
        QDataStream::ByteOrder byteOrder{ QDataStream::LittleEndian };
        SerialTransceiver::DataTypes dataType{ SerialTransceiver::DataTypes::ascii };
    };

    auto getSettings() const { return settings; };

private:
    Settings settings;
    QComboBox *dataTypeBox{ new QComboBox{ this } };
    QComboBox *byteOrderBox{ new QComboBox{ this } };

    void saveSettings();
    void loadSettings();
    void updateSettings();
    template<typename T>
    void updateIndex(QComboBox *comboBox, T data);

private slots:
    void ok();
    void cancel();
};

#endif // DATASETTINGSDIALOG_H
