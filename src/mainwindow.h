#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chart.h"
#include "datasettingsdialog.h"
#include "portsettingsdialog.h"
#include "serialtransceiver.h"
#include "textwidget.h"
#include "customaction.h"

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

    CustomAction *actionConnect{ new CustomAction{ "Connect", this } };
    CustomAction *actionDisconnect{ new CustomAction{ "Disconnect", this } };
    CustomAction *actionPortSettings{ new CustomAction{ "Port", this } };
    CustomAction *actionClear{ new CustomAction{ "Clear", this } };
    CustomAction *actionSaveImage{ new CustomAction{ "Save Image", this } };
    CustomAction *actionSavePlot{ new CustomAction{ "Save Plot", this } };
    CustomAction *actionOpenPlot{ new CustomAction{ "Open Plot", this } };
    CustomAction *actionSaveSpectrum{ new CustomAction{ "Save Spectrum", this } };
    CustomAction *actionOpenSpectrum{ new CustomAction{ "Open Spectrum", this } };
    CustomAction *actionDataSettings{ new CustomAction{ "Data", this } };
    CustomAction *actionChartType{ new CustomAction{ "Chart type", this } };
    CustomAction *actionResetZoom{ new CustomAction{ "Reset zoom", this } };
    CustomAction *actionAppendToPlot{ new CustomAction{ "Append to plot", this } };
    CustomAction *actionAppendToSpectrum{ new CustomAction{ "Append to spectrum", this } };
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
