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
    QAction *actionSaveImage{ new QAction{ "Save Image", this } };
    QAction *actionSavePlot{ new QAction{ "Save Plot", this } };
    QAction *actionOpenPlot{ new QAction{ "Open Plot", this } };
    QAction *actionSaveSpectrum{ new QAction{ "Save Spectrum", this } };
    QAction *actionOpenSpectrum{ new QAction{ "Open Spectrum", this } };
    QAction *actionDataSettings{ new QAction{ "Data", this } };
    QAction *actionChartType{ new QAction{ "Chart type", this } };
    QAction *actionResetZoom{ new QAction{ "Reset zoom", this } };
    QAction *actionAppendToPlot{ new QAction{ "Append to plot", this } };
    QAction *actionAppendToSpectrum{ new QAction{ "Append to spectrum", this } };
    QLineEdit *sendNumLineEdit{ new QLineEdit{ this } };
    QCheckBox *sendSignedCheckBox{ new QCheckBox{ "Signed" } };
    QLineEdit *sendStringLineEdit{ new QLineEdit{ this } };
    QCheckBox *sendStringNewlineCheckBox{ new QCheckBox{ "Newline" } };
    QLineEdit *updateIntervalLineEdit{ new QLineEdit{ QString::number(chart->getUpdateInterval()),
                                                      this } };
    QCheckBox *logScaleCheckBox{ new QCheckBox{ "Log scale" } };
    QCheckBox *autoscaleCheckboxX{ new QCheckBox{ "X" } };
    QCheckBox *autoscaleCheckboxY{ new QCheckBox{ "Y" } };

    QString createFileDialog(const QFileDialog::AcceptMode acceptMode, const QString &nameFilter,
                             const QString &defaultSuffix);
    std::unique_ptr<QTextStream, std::function<void(QTextStream *)>>
    openFileStream(const QString &fileName, QIODevice::OpenMode flags);

private slots:
    void serialConnect();
    void serialDisconnect() const;
    void saveImage();
    void savePlotData();
    void openPlotData();
    void saveSpectrumData();
    void openSpectrumData();
    void chartTypeChanged() const;
    void updateSelectedPoint(const QPointF point) const;
};
#endif // MAINWINDOW_H
