#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>

#include "SerialTransceiver.h"
#include "PortSettingsDialog.h"
#include "DataSettingsDialog.h"
#include "Plot.h"
#include "TextWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Plot *plot{ nullptr };
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
    QWidget *centralWidget{ nullptr };
    QVBoxLayout *verticalLayout{ nullptr };
    QMenuBar *menuBar{ nullptr };
    QMenu *menuFile{ nullptr };
    QMenu *menuSettings{ nullptr };
    QStatusBar *statusBar{ nullptr };
    QToolBar *toolBar{ nullptr };

private slots:
    void serialConnect();
    void serialDisconnect();
    void saveImage();
    void clearChart();
    void saveData();
    void openData();
    void updateDataSettingsDialog();
    void statusBarUpdateChartType();
    void updateSelectedPoint(const QPointF point);
};
#endif // MAINWINDOW_H
