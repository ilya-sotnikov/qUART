#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>

#include "chart.h"
#include "datasettingsdialog.h"
#include "portsettingsdialog.h"
#include "serialtransceiver.h"
#include "textwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Chart *chart{ nullptr };
    PortSettingsDialog *portSettingsDialog{ nullptr };
    DataSettingsDialog *dataSettingsDialog{ nullptr };
    SerialTransceiver *serialTransceiver{ nullptr };
    TextWidget *chartTypeWidget{ nullptr };

    QString createFileDialog(QFileDialog::AcceptMode acceptMode, QString nameFilter,
                             QString defaultSuffix);

    QAction *actionConnect{ nullptr };
    QAction *actionDisconnect{ nullptr };
    QAction *actionPortSettings{ nullptr };
    QAction *actionClear{ nullptr };
    QAction *actionSaveImage{ nullptr };
    QAction *actionSaveData{ nullptr };
    QAction *actionOpenData{ nullptr };
    QAction *actionDataSettings{ nullptr };
    QAction *actionChartType{ nullptr };
    QAction *actionResetZoom{ nullptr };
    QAction *actionHideMarker{ nullptr };
    QAction *actionAppendToPlot{ nullptr };
    QAction *actionAppendToSpectrum{ nullptr };
    QStatusBar *statusBar{ nullptr };

private slots:
    void serialConnect();
    void serialDisconnect();
    void saveImage();
    void clearChart();
    void saveData();
    void openData();
    void statusBarUpdateChartType();
    void updateSelectedPoint(const QPointF point);
};
#endif // MAINWINDOW_H
