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

public slots:
    void showDialog()
    {
        updateIndexes();
        show();
    }

private:
    Settings settings;
    QComboBox *dataTypeBox{ new QComboBox{ this } };
    QComboBox *byteOrderBox{ new QComboBox{ this } };

    void saveSettings() const;
    void loadSettings();
    void updateSettings();
    template<typename T>
    void updateIndex(QComboBox *const comboBox, const T data) const;
    void updateIndexes() const;

private slots:
    void ok();
    void cancel();
};

#endif // DATASETTINGSDIALOG_H
