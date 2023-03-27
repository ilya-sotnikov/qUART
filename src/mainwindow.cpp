#include "mainwindow.h"

/**
 * @brief Constructs a new MainWindow object
 *
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow{ parent }
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
    toolBar->addAction(actionAppendToPlot);
    toolBar->addAction(actionAppendToSpectrum);

    auto sidePanel{ new QGroupBox };
    sidePanel->setTitle("Side panel");

    auto sidePanelLayout{ new QVBoxLayout };

    auto lastPointsLabel{ new QLabel{ "Show last N points:" } };
    sidePanelLayout->addWidget(lastPointsLabel);
    auto lastPointsLineEdit{ new QLineEdit };
    lastPointsLineEdit->setToolTip("Empty means show all data");
    sidePanelLayout->addWidget(lastPointsLineEdit);

    auto sendNumLabel{ new QLabel{ "Send a number:" } };
    sidePanelLayout->addWidget(sendNumLabel);
    sendNumLineEdit->setToolTip("0x__ for hex, 0b__ for binary, Enter to send");
    sendNumLineEdit->setEnabled(false);
    sidePanelLayout->addWidget(sendNumLineEdit);
    sidePanelLayout->addWidget(sendSignedCheckBox);
    sendSignedCheckBox->setEnabled(false);

    sidePanelLayout->addStretch();
    sidePanel->setLayout(sidePanelLayout);

    auto windowLayout{ new QHBoxLayout };
    windowLayout->addWidget(sidePanel, 0);
    windowLayout->addWidget(chart, 1);

    auto window{ new QWidget };
    window->setLayout(windowLayout);
    setCentralWidget(window);

    actionConnect->setEnabled(true);
    actionDisconnect->setEnabled(false);
    actionPortSettings->setEnabled(true);

    statusBar->addPermanentWidget(chartTypeWidget);
    if (chart->getChartType() == Chart::ChartType::plot)
        chartTypeWidget->setText("Chart type: Plot");
    else if (chart->getChartType() == Chart::ChartType::spectrum)
        chartTypeWidget->setText("Chart type: Spectrum");

    connect(serialTransceiver, &SerialTransceiver::newDataAvailable, chart, &Chart::addData);
    connect(actionConnect, &QAction::triggered, this, &MainWindow::serialConnect);
    connect(actionDisconnect, &QAction::triggered, this, &MainWindow::serialDisconnect);
    connect(actionClear, &QAction::triggered, this, [this]() { chart->clear(); });
    connect(actionChartType, &QAction::triggered, chart, &Chart::changeType);
    connect(actionChartType, &QAction::triggered, this, &MainWindow::chartTypeChanged);
    connect(actionResetZoom, &QAction::triggered, chart, &Chart::resetZoom);
    connect(actionAppendToPlot, &QAction::triggered, this,
            [this] { chart->appendToPlot = !chart->appendToPlot; });
    connect(actionAppendToSpectrum, &QAction::triggered, this,
            [this] { chart->appendToSpectrum = !chart->appendToSpectrum; });

    connect(actionSaveData, &QAction::triggered, this, &MainWindow::saveData);
    connect(actionOpenData, &QAction::triggered, this, &MainWindow::openData);
    connect(actionSaveImage, &QAction::triggered, this, &MainWindow::saveImage);
    connect(actionPortSettings, &QAction::triggered, portSettingsDialog, &PortSettingsDialog::show);
    connect(actionDataSettings, &QAction::triggered, dataSettingsDialog, &DataSettingsDialog::show);

    connect(chart, &Chart::selectedPointChanged, this, &MainWindow::updateSelectedPoint);
    connect(dataSettingsDialog, &DataSettingsDialog::dataTypeChanged, this,
            [this](bool isUnsigned) {
                if (isUnsigned) {
                    actionAppendToSpectrum->setDisabled(false);
                } else {
                    actionAppendToSpectrum->setChecked(false);
                    chart->appendToSpectrum = false;
                    actionAppendToSpectrum->setDisabled(true);
                }
            });

    connect(lastPointsLineEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        bool ok;
        auto n{ text.toLongLong(&ok, 0) };

        if (ok)
            chart->setShowLastPoints(n);
        else
            chart->setShowLastPoints(-1);

        chart->updateChart();
    });

    connect(sendNumLineEdit, &QLineEdit::returnPressed, this, [this]() {
        auto numString{ sendNumLineEdit->text() };
        bool isSigned{ sendSignedCheckBox->isChecked() };
        serialTransceiver->writeNumber(numString, isSigned);
    });
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
 * @brief Connects to the serial port with specified parameters from PortSettingsDialog
 *
 * Additionally, it disables several actions which shouldn't be executed while
 * new data is received. If failed to connect, pop up a message box with an
 * error.
 *
 */
void MainWindow::serialConnect()
{
    const auto &serialSettings{ portSettingsDialog->getSettings() };
    const auto dataType{ dataSettingsDialog->getCurrentDataType() };
    serialTransceiver->setDataType(dataType);
    serialTransceiver->setPortName(serialSettings.name);
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
        sendNumLineEdit->setEnabled(true);
        sendSignedCheckBox->setEnabled(true);
    } else {
        QMessageBox::critical(this, "Error", serialTransceiver->errorString());
    }
}

/**
 * @brief Disconnects from the serial port
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
    sendNumLineEdit->setEnabled(false);
    sendSignedCheckBox->setEnabled(false);
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
 * @brief Saves data from the chart to a text file
 *
 * Saved data depends on the current chart type (chart or spectrum).
 *
 */
void MainWindow::saveData()
{
    auto fileName{ createFileDialog(QFileDialog::AcceptSave, "Text files (*.txt)", "txt") };
    if (fileName.isEmpty())
        return;
    QFile file{ fileName };
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        const auto dataList{ chart->getData() };
        QTextStream stream(&file);
        for (const auto &data : dataList)
            stream << QString::number(data) << "\n";
    }
}

/**
 * @brief Opens the text file and plots data from it
 *
 * It reads raw data, so you must choose the correct chart type before opening
 * new data (chart or spectrum).
 *
 */
void MainWindow::openData()
{
    auto fileName{ createFileDialog(QFileDialog::AcceptOpen, "Text files (*.txt)", "txt") };
    if (fileName.isEmpty())
        return;
    QFile file{ fileName };
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        chart->clear();
        QList<qreal> dataList;
        QTextStream stream(&file);
        while (!stream.atEnd())
            dataList.append(stream.readLine().toDouble());
        chart->setRawData(dataList);
    }
}

/**
 * @brief Updates the status bar
 *
 */
void MainWindow::chartTypeChanged()
{
    const auto chartType{ chart->getChartType() };
    if (chartType == Chart::ChartType::plot)
        chartTypeWidget->setText("Chart type: Plot");
    else if (chartType == Chart::ChartType::spectrum)
        chartTypeWidget->setText("Chart type: Spectrum");
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
