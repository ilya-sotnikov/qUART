#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chart.h"
#include "datasettingsdialog.h"
#include "portsettingsdialog.h"
#include "serialtransceiver.h"
#include "textwidget.h"

#include <qmainwindow.h>
#include <qstatusbar.h>
#include <qfiledialog.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    Chart *chart{ new Chart{ this } };
    PortSettingsDialog *portSettingsDialog{ new PortSettingsDialog{ this } };
    DataSettingsDialog *dataSettingsDialog{ new DataSettingsDialog{ this } };
    SerialTransceiver *serialTransceiver{ new SerialTransceiver{ this } };
    TextWidget *chartTypeWidget{ new TextWidget{ this } };
    QStatusBar *statusBar{ new QStatusBar{ this } };

    QAction *actionConnect{ new QAction{ "Connect", this } };
    QAction *actionDisconnect{ new QAction{ "Disconnect", this } };
    QAction *actionPortSettings{ new QAction{ "Port", this } };
    QAction *actionClear{ new QAction{ "Clear", this } };
    QAction *actionSaveImage{ new QAction{ "Save image", this } };
    QAction *actionSaveData{ new QAction{ "Save data", this } };
    QAction *actionOpenData{ new QAction{ "Open data", this } };
    QAction *actionDataSettings{ new QAction{ "Data", this } };
    QAction *actionChartType{ new QAction{ "Chart type", this } };
    QAction *actionResetZoom{ new QAction{ "Reset zoom", this } };
    QAction *actionAppendToPlot{ new QAction{ "Append to chart", this } };
    QAction *actionAppendToSpectrum{ new QAction{ "Append to spectrum", this } };
    QLineEdit *sendNumLineEdit{ new QLineEdit };
    QCheckBox *sendSignedCheckBox{ new QCheckBox{ "Signed" } };

    QString createFileDialog(QFileDialog::AcceptMode acceptMode, QString nameFilter,
                             QString defaultSuffix);

private slots:
    void serialConnect();
    void serialDisconnect();
    void saveImage();
    void saveData();
    void openData();
    void chartTypeChanged();
    void updateSelectedPoint(const QPointF point);
};
#endif // MAINWINDOW_H
