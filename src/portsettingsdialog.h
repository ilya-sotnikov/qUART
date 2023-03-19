#ifndef PORTSETTINGSDIALOG_H
#define PORTSETTINGSDIALOG_H

#include "PortsInfoDialog.h"

#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSerialPort>
#include <QVBoxLayout>

class PortSettingsDialog : public QDialog {
  Q_OBJECT

public:
  explicit PortSettingsDialog(QWidget *parent = nullptr);

  struct Settings {
    QString name;
    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
    QSerialPort::FlowControl flowControl;
  };
  const auto &getCurrentSettings() const { return currentSettings; };

private:
  PortsInfoDialog *portsInfoDialog{nullptr};
  Settings currentSettings;

  void fillSettings() const;
  void enumeratePorts() const;
  void checkCustomPath(const int index) const;
  void updateSettings();

  QComboBox *serialPortBox{nullptr};
  QComboBox *baudRateBox{nullptr};
  QComboBox *dataBitsBox{nullptr};
  QComboBox *parityBox{nullptr};
  QComboBox *stopBitsBox{nullptr};
  QComboBox *flowControlBox{nullptr};

private slots:
  void ok();
  void cancel();
};

#endif // PORTSETTINGSDIALOG_H
