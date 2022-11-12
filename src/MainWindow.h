#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SerialTransceiver.h"
#include "PortSettingsDialog.h"
#include "DataSettingsDialog.h"
#include "Plot.h"
#include "TextWidget.h"

#include <QMainWindow>
#include <QtWidgets>
#include <QWidget>
#include <QFileDialog>

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

    QAction *actionConnect;
    QAction *actionDisconnect;
    QAction *actionPortSettings;
    QAction *actionClear;
    QAction *actionSaveImage;
    QAction *actionSaveData;
    QAction *actionOpenData;
    QAction *actionDataSettings;
    QAction *actionChartType;
    QAction *actionResetZoom;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuSettings;
    QStatusBar *statusBar;
    QToolBar *toolBar;

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
