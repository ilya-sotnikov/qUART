#include "mainwindow.h"

#include <QDebug>
#include <QIcon>
#include <QMessageBox>

/**
 * @brief Construct a new MainWindow object
 *
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{ parent },
      chart{ new Chart{ this } },
      portSettingsDialog{ new PortSettingsDialog{ this } },
      dataSettingsDialog{ new DataSettingsDialog{ this } },
      serialTransceiver{ new SerialTransceiver{ this } },
      chartTypeWidget{ new TextWidget{ this } },
      actionConnect{ new QAction{ "Connect", this } },
      actionDisconnect{ new QAction{ "Disconnect", this } },
      actionPortSettings{ new QAction{ "Port", this } },
      actionClear{ new QAction{ "Clear", this } },
      actionSaveImage{ new QAction{ "Save image", this } },
      actionSaveData{ new QAction{ "Save data", this } },
      actionOpenData{ new QAction{ "Open data", this } },
      actionDataSettings{ new QAction{ "Data", this } },
      actionChartType{ new QAction{ "Chart type", this } },
      actionResetZoom{ new QAction{ "Reset zoom", this } },
      actionHideMarker{ new QAction{ "Hide marker", this } },
      actionAppendToPlot{ new QAction{ "Append to chart", this } },
      actionAppendToSpectrum{ new QAction{ "Append to spectrum", this } },
      statusBar{ new QStatusBar{ this } }
{
    setMinimumSize(QSize{ 320, 240 });
    resize(QSize{ 800, 600 });
    setWindowTitle("qUART");
    setWindowIcon(QIcon{ ":app.png" });

    auto menuBar{ new QMenuBar{ this } };
    auto menuFile{ new QMenu{ "File", menuBar } };
    auto menuSettings{ new QMenu{ "Settings", menuBar } };
    setMenuBar(menuBar);

    setStatusBar(statusBar);

    actionConnect->setIcon(QIcon{ ":actionConnect.png" });
    actionDisconnect->setIcon(QIcon{ ":actionDisconnect.png" });
    actionClear->setIcon(QIcon{ ":actionClear.png" });
    actionChartType->setIcon(QIcon{ ":actionChartType.png" });
    actionResetZoom->setIcon(QIcon{ ":actionResetZoom.png" });
    actionHideMarker->setIcon(QIcon{ ":actionHideMarker.png" });
    actionAppendToPlot->setIcon(QIcon{ ":actionAppendToPlot.png" });
    actionAppendToPlot->setCheckable(true);
    actionAppendToPlot->setChecked(true);
    actionAppendToSpectrum->setIcon(QIcon{ ":actionAppendToSpectrum.png" });
    actionAppendToSpectrum->setCheckable(true);
    actionAppendToSpectrum->setChecked(true);

    auto toolBar{ new QToolBar{ this } };
    addToolBar(Qt::TopToolBarArea, toolBar);

    menuBar->addAction(menuFile->menuAction());
    menuBar->addAction(menuSettings->menuAction());
    menuFile->addAction(actionSaveData);
    menuFile->addAction(actionOpenData);
    menuFile->addAction(actionSaveImage);
    menuSettings->addAction(actionPortSettings);
    menuSettings->addAction(actionDataSettings);
    menuSettings->addSeparator();
    toolBar->addAction(actionConnect);
    toolBar->addAction(actionDisconnect);
    toolBar->addAction(actionClear);
    toolBar->addAction(actionChartType);
    toolBar->addAction(actionResetZoom);
    toolBar->addAction(actionHideMarker);
    toolBar->addAction(actionAppendToPlot);
    toolBar->addAction(actionAppendToSpectrum);

    auto windowLayout{ new QHBoxLayout{} };
    windowLayout->addWidget(chart);

    auto window{ new QWidget{} };
    window->setLayout(windowLayout);
    setCentralWidget(window);

    actionConnect->setEnabled(true);
    actionDisconnect->setEnabled(false);
    actionPortSettings->setEnabled(true);

    statusBar->addPermanentWidget(chartTypeWidget);
    if (chart->getChartType() == Chart::plot)
        chartTypeWidget->setText("Chart type: Plot");
    else if (chart->getChartType() == Chart::spectrum)
        chartTypeWidget->setText("Chart type: Spectrum");

    connect(serialTransceiver, &SerialTransceiver::newDataAvailable, chart, &Chart::addData);
    connect(actionConnect, &QAction::triggered, this, &MainWindow::serialConnect);
    connect(actionDisconnect, &QAction::triggered, this, &MainWindow::serialDisconnect);
    connect(actionPortSettings, &QAction::triggered, portSettingsDialog, &PortSettingsDialog::show);
    connect(actionDataSettings, &QAction::triggered, dataSettingsDialog, &DataSettingsDialog::show);
    connect(actionSaveImage, &QAction::triggered, this, &MainWindow::saveImage);
    connect(actionClear, &QAction::triggered, this, &MainWindow::clearChart);
    connect(actionChartType, &QAction::triggered, chart, &Chart::changeType);
    connect(actionChartType, &QAction::triggered, this, &MainWindow::statusBarUpdateChartType);
    connect(actionSaveData, &QAction::triggered, this, &MainWindow::saveData);
    connect(actionOpenData, &QAction::triggered, this, &MainWindow::openData);
    connect(actionResetZoom, &QAction::triggered, chart, &Chart::resetZoom);
    connect(actionHideMarker, &QAction::triggered, chart, &Chart::hideMarker);
    connect(chart, &Chart::pointSelected, this, &MainWindow::updateSelectedPoint);
    connect(actionAppendToPlot, &QAction::triggered, this,
            [this] { chart->appendToPlot = !chart->appendToPlot; });
    connect(actionAppendToSpectrum, &QAction::triggered, this,
            [this] { chart->appendToSpectrum = !chart->appendToSpectrum; });
    connect(dataSettingsDialog, &DataSettingsDialog::dataTypeChanged, this,
            [this](SerialTransceiver::DataTypes dataType) {
                if (static_cast<int>(dataType)
                    > static_cast<int>(SerialTransceiver::DataTypes::u64)) {
                    actionAppendToSpectrum->setChecked(false);
                    chart->appendToSpectrum = false;
                    actionAppendToSpectrum->setDisabled(true);
                } else {
                    actionAppendToSpectrum->setDisabled(false);
                }
            });

    serialConnect();
}

/**
 * @brief Destroy the MainWindow object
 *
 */
MainWindow::~MainWindow()
{
    delete serialTransceiver;
}

/**
 * @brief A helper function to create a file dialog with specified parameters
 *
 * This function can be used to create an open dialog or save dialog
 * with the file extension filter and default file suffix.
 * Returns the selected file name or empty string if none was selected.
 *
 * @param acceptMode
 * @param nameFilter
 * @param defaultSuffix
 * @return QString
 */
QString MainWindow::createFileDialog(QFileDialog::AcceptMode acceptMode, QString nameFilter,
                                     QString defaultSuffix)
{
    QFileDialog fileDialog{ this };
    fileDialog.setAcceptMode(acceptMode);
    fileDialog.setNameFilter(nameFilter);
    fileDialog.setDefaultSuffix(defaultSuffix);
    if (!fileDialog.exec() || fileDialog.selectedFiles().isEmpty())
        return "";
    else
        return fileDialog.selectedFiles().at(0);
}

/**
 * @brief Connect to the serial port with specified parameters from
 * PortSettingsDialog
 *
 * Additionally, it disables several actions which shouldn't be executed while
 * new data is received. If failed to connect, pop up a message box with an
 * error.
 *
 */
void MainWindow::serialConnect()
{
    const auto &serialSettings{ portSettingsDialog->getCurrentSettings() };
    const auto dataType{ dataSettingsDialog->getCurrentDataType() };
    serialTransceiver->setDataType(dataType);
    // serialTransceiver->setPortName(serialSettings.name);
    serialTransceiver->setPortName("/dev/pts/3");
    serialTransceiver->setBaudRate(serialSettings.baudRate);
    serialTransceiver->setDataBits(serialSettings.dataBits);
    serialTransceiver->setParity(serialSettings.parity);
    serialTransceiver->setStopBits(serialSettings.stopBits);
    serialTransceiver->setFlowControl(serialSettings.flowControl);
    if (serialTransceiver->serialOpen()) {
        actionConnect->setEnabled(false);
        actionDisconnect->setEnabled(true);
        actionPortSettings->setEnabled(false);
        actionDataSettings->setEnabled(false);
        actionOpenData->setEnabled(false);
        actionSaveData->setEnabled(false);
        actionSaveImage->setEnabled(false);
    } else {
        QMessageBox::critical(this, "Error", serialTransceiver->errorString());
    }
}

/**
 * @brief Disconnect from the serial port
 *
 * Additionally, it reenables several actions which are safe to execute
 * while the serial port is disconnected.
 *
 */
void MainWindow::serialDisconnect()
{
    serialTransceiver->serialClose();
    actionConnect->setEnabled(true);
    actionDisconnect->setEnabled(false);
    actionPortSettings->setEnabled(true);
    actionDataSettings->setEnabled(true);
    actionOpenData->setEnabled(true);
    actionSaveData->setEnabled(true);
    actionSaveImage->setEnabled(true);
}

/**
 * @brief Saves the chart to an png image
 *
 */
void MainWindow::saveImage()
{
    auto fileName{ createFileDialog(QFileDialog::AcceptSave, "Images (*.png)", "png") };
    if (fileName.isEmpty())
        return;
    QPixmap pixMap{ chart->grab() };
    pixMap.save(fileName, "PNG");
}

/**
 * @brief Clears the chart
 *
 */
void MainWindow::clearChart()
{
    chart->clear();
}

/**
 * @brief Saves data from the chart to a text file
 *
 * Saved data depends on the current chart type (chart or spectrum).
 *
 */
void MainWindow::saveData()
{
    auto fileName{ createFileDialog(QFileDialog::AcceptSave, "Text files (*.txt)", "txt") };
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        const auto dataList{ chart->getData() };
        QTextStream stream(&file);
        for (const auto &data : dataList)
            stream << QString::number(data) << "\n";
    }
}

/**
 * @brief Opens text file and plots data from it
 *
 * It reads raw data, so you must choose the correct chart type before opening
 * new data (chart or spectrum).
 *
 */
void MainWindow::openData()
{
    chart->clear();
    auto fileName = createFileDialog(QFileDialog::AcceptOpen, "Text files (*.txt)", "txt");
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QList<qreal> dataList;
        QTextStream stream(&file);
        while (!stream.atEnd())
            dataList.append(stream.readLine().toDouble());
        chart->addRawData(&dataList);
    }
}

/**
 * @brief Updates the status bar with the current chart type
 *
 */
void MainWindow::statusBarUpdateChartType()
{
    auto chartType{ chart->getChartType() };
    if (chartType == Chart::spectrum)
        chartTypeWidget->setText("Chart type: Spectrum");
    else if (chartType == Chart::plot)
        chartTypeWidget->setText("Chart type: Plot");
}

/**
 * @brief Updates the message in the status bar with the selected point
 *
 * @param point
 */
void MainWindow::updateSelectedPoint(const QPointF point)
{
    QString msg{ "x: " };
    msg += QString::number(point.x());
    msg += "     y: ";
    msg += QString::number(point.y());
    statusBar->showMessage(msg);
}
