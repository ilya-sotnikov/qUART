#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SerialTransceiver.h"
#include "PortSettingsDialog.h"
#include "DataSettingsDialog.h"
#include "Plot.h"
#include "TextWidget.h"

#include <QMainWindow>
#include <QWidget>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow* ui;
    Plot* plot {nullptr};
    PortSettingsDialog* portSettingsDialog {nullptr};
    DataSettingsDialog* dataSettingsDialog {nullptr};
    SerialTransceiver* serialTransceiver {nullptr};
    TextWidget* chartTypeWidget {nullptr};
    QString createFileDialog(QFileDialog::AcceptMode acceptMode,
                             QString nameFilter, QString defaultSuffix);

private slots:
    void serialConnect();
    void serialDisconnect();
    void saveImage();
    void clearChart();
    void saveData();
    void openData();
    void updateDataSettingsDialog();
    void statusBarUpdateChartType();
};
#endif // MAINWINDOW_H
