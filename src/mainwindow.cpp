#include "mainwindow.h"

using namespace Qt::Literals::StringLiterals;

MainWindow::MainWindow(QWidget *parent) : QMainWindow{ parent }
{
    setMinimumSize(QSize{ 800, 600 });
    setWindowTitle(u"qUART"_s);
    setWindowIcon(QIcon{ u":app.png"_s });

    const auto menuBar{ new QMenuBar{ this } };
    const auto menuFile{ new QMenu{ u"File"_s, menuBar } };
    const auto menuSettings{ new QMenu{ u"Settings"_s, menuBar } };
    setMenuBar(menuBar);

    setStatusBar(statusBar);

    actionConnect->setIconWithTheme(u":actionConnect.png"_s);
    actionConnect->setShortcut(QKeySequence{ u"Alt+C"_s }, true);
    actionDisconnect->setIconWithTheme(u":actionDisconnect.png"_s);
    actionDisconnect->setShortcut(QKeySequence{ u"Alt+D"_s }, true);
    actionClear->setIconWithTheme(u":actionClear.png"_s);
    actionClear->setShortcut(QKeySequence{ u"Ctrl+L"_s }, true);
    actionChartType->setIconWithTheme(u":actionChartType.png"_s);
    actionChartType->setShortcut(QKeySequence{ u"Ctrl+T"_s }, true);
    actionResetZoom->setIconWithTheme(u":actionResetZoom.png"_s);
    actionResetZoom->setShortcut(QKeySequence{ u"Ctrl+R"_s }, true);
    actionAppendToPlot->setIconWithTheme(u":actionAppendToPlot.png"_s);
    actionAppendToPlot->setCheckable(true);
    actionAppendToPlot->setChecked(true);
    actionAppendToSpectrum->setIconWithTheme(u":actionAppendToSpectrum.png"_s);
    actionAppendToSpectrum->setCheckable(true);
    actionAppendToSpectrum->setChecked(true);

    const auto toolBar{ new QToolBar{ this } };
    addToolBar(Qt::TopToolBarArea, toolBar);

    menuBar->addAction(menuFile->menuAction());
    menuBar->addAction(menuSettings->menuAction());
    menuFile->addAction(actionSavePlot);
    menuFile->addAction(actionOpenPlot);
    menuFile->addAction(actionSaveSpectrum);
    menuFile->addAction(actionOpenSpectrum);
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

    const auto sidePanel{ new QGroupBox };
    sidePanel->setTitle(u"Side panel"_s);

    const auto sidePanelLayout{ new QVBoxLayout };

    const auto lastPointsLabel{ new QLabel{ u"Show last N points:"_s } };
    sidePanelLayout->addWidget(lastPointsLabel);
    const auto lastPointsLineEdit{ new QLineEdit };
    lastPointsLineEdit->setToolTip(u"Empty means show all data"_s);
    sidePanelLayout->addWidget(lastPointsLineEdit);

    const auto updateIntervalLabel{ new QLabel{ u"Update interval (ms):"_s } };
    sidePanelLayout->addWidget(updateIntervalLabel);
    sidePanelLayout->addWidget(updateIntervalLineEdit);

    sidePanelLayout->addWidget(logScaleCheckBox);

    const auto autoscaleLabel{ new QLabel{ u"Autoscale:"_s } };
    sidePanelLayout->addWidget(autoscaleLabel);
    const auto autoscaleLayout{ new QHBoxLayout{} };
    autoscaleLayout->addWidget(autoscaleCheckboxX);
    autoscaleCheckboxX->setChecked(chart->getAutoScaleX());
    autoscaleCheckboxY->setChecked(chart->getAutoScaleY());
    autoscaleLayout->addWidget(autoscaleCheckboxY);
    sidePanelLayout->addLayout(autoscaleLayout);

    const auto sendNumLabel{ new QLabel{ u"Send a number:"_s } };
    sidePanelLayout->addWidget(sendNumLabel);
    sendNumLineEdit->setToolTip(u"0x__ for hex, 0b__ for binary, Enter to send"_s);
    sendNumLineEdit->setEnabled(false);
    sidePanelLayout->addWidget(sendNumLineEdit);
    sidePanelLayout->addWidget(sendSignedCheckBox);
    sendSignedCheckBox->setEnabled(false);

    const auto sendStringLabel{ new QLabel{ u"Send a string:"_s } };
    sidePanelLayout->addWidget(sendStringLabel);
    sendStringLineEdit->setToolTip(u"Enter to send"_s);
    sendStringLineEdit->setEnabled(false);
    sidePanelLayout->addWidget(sendStringLineEdit);
    sidePanelLayout->addWidget(sendStringNewlineCheckBox);
    sendStringNewlineCheckBox->setEnabled(false);
    sendStringNewlineCheckBox->setCheckState(Qt::Checked);

    sidePanelLayout->addStretch();
    sidePanel->setLayout(sidePanelLayout);

    const auto windowLayout{ new QHBoxLayout };
    windowLayout->addWidget(sidePanel, 0);
    windowLayout->addWidget(chart, 1);

    const auto window{ new QWidget };
    window->setLayout(windowLayout);
    setCentralWidget(window);

    actionConnect->setEnabled(true);
    actionDisconnect->setEnabled(false);
    actionPortSettings->setEnabled(true);

    statusBar->addPermanentWidget(chartTypeWidget);
    if (chart->getChartType() == Chart::ChartType::plot)
        chartTypeWidget->setText(u"Chart type: Plot"_s);
    else if (chart->getChartType() == Chart::ChartType::spectrum)
        chartTypeWidget->setText(u"Chart type: Spectrum"_s);

    connect(serialTransceiver, &SerialTransceiver::newDataAvailable, chart, &Chart::addData);
    connect(actionConnect, &QAction::triggered, this, &MainWindow::serialConnect);
    connect(actionDisconnect, &QAction::triggered, this, &MainWindow::serialDisconnect);
    connect(actionClear, &QAction::triggered, this, [this]() { chart->clear(); });
    connect(actionChartType, &QAction::triggered, chart, &Chart::changeType);
    connect(actionChartType, &QAction::triggered, this, &MainWindow::chartTypeChanged);
    connect(actionResetZoom, &QAction::triggered, chart, &Chart::resetZoom);
    connect(actionAppendToPlot, &QAction::toggled, chart, &Chart::setAppendToPlot);
    connect(actionAppendToSpectrum, &QAction::toggled, chart, &Chart::setAppendToPlot);

    connect(actionSavePlot, &QAction::triggered, this, &MainWindow::savePlotData);
    connect(actionOpenPlot, &QAction::triggered, this, &MainWindow::openPlotData);
    connect(actionSaveSpectrum, &QAction::triggered, this, &MainWindow::saveSpectrumData);
    connect(actionOpenSpectrum, &QAction::triggered, this, &MainWindow::openSpectrumData);
    connect(actionSaveImage, &QAction::triggered, this, &MainWindow::saveImage);
    connect(actionPortSettings, &QAction::triggered, portSettingsDialog,
            &PortSettingsDialog::showDialog);
    connect(actionDataSettings, &QAction::triggered, dataSettingsDialog,
            &DataSettingsDialog::showDialog);

    connect(chart, &Chart::selectedPointChanged, this, &MainWindow::updateSelectedPoint);

    connect(lastPointsLineEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        bool ok;
        const auto n{ text.toLongLong(&ok, 0) };

        if (ok)
            chart->setShowLastPoints(n);
        else
            chart->setShowLastPoints(-1);
    });

    connect(sendNumLineEdit, &QLineEdit::returnPressed, this, [this]() {
        const auto numString{ sendNumLineEdit->text() };
        const bool isSigned{ sendSignedCheckBox->isChecked() };
        serialTransceiver->writeNumber(numString, isSigned);
    });

    connect(sendStringLineEdit, &QLineEdit::returnPressed, this, [this]() {
        const auto string{ sendStringLineEdit->text() };
        const bool appendNewline{ sendStringNewlineCheckBox->isChecked() };
        serialTransceiver->writeString(string, appendNewline);
    });

    connect(updateIntervalLineEdit, &QLineEdit::textChanged, this, [this]() {
        bool ok;
        const auto ms{ updateIntervalLineEdit->text().toInt(&ok) };
        if (ok)
            chart->setUpdateInterval(ms);
    });

    connect(chart, &Chart::autoscaleChanged, this,
            [this](const bool autoscaleX, const bool autoscaleY) {
                autoscaleCheckboxX->setChecked(autoscaleX);
                autoscaleCheckboxY->setChecked(autoscaleY);
            });

    connect(logScaleCheckBox, &QCheckBox::stateChanged, chart, &Chart::setLogarithmic);
    connect(autoscaleCheckboxX, &QCheckBox::stateChanged, chart, &Chart::setAutoscaleX);
    connect(autoscaleCheckboxY, &QCheckBox::stateChanged, chart, &Chart::setAutoscaleY);
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
    const auto dataSettings{ dataSettingsDialog->getSettings() };
    serialTransceiver->setDataType(dataSettings.dataType);
    serialTransceiver->setByteOrder(dataSettings.byteOrder);
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
        actionOpenPlot->setEnabled(false);
        actionSavePlot->setEnabled(false);
        actionOpenSpectrum->setEnabled(false);
        actionSaveSpectrum->setEnabled(false);
        actionSaveImage->setEnabled(false);
        sendNumLineEdit->setEnabled(true);
        sendSignedCheckBox->setEnabled(true);
        sendStringLineEdit->setEnabled(true);
        sendStringNewlineCheckBox->setEnabled(true);
    } else {
        QMessageBox::warning(this, u"Warning"_s, serialTransceiver->errorString());
    }
}

/**
 * @brief Disconnects from the serial port
 *
 * Additionally, it reenables several actions which are safe to execute
 * while the serial port is disconnected.
 *
 */
void MainWindow::serialDisconnect() const
{
    serialTransceiver->serialClose();
    actionConnect->setEnabled(true);
    actionDisconnect->setEnabled(false);
    actionPortSettings->setEnabled(true);
    actionDataSettings->setEnabled(true);
    actionOpenPlot->setEnabled(true);
    actionSavePlot->setEnabled(true);
    actionOpenSpectrum->setEnabled(true);
    actionSaveSpectrum->setEnabled(true);
    actionSaveImage->setEnabled(true);
    sendNumLineEdit->setEnabled(false);
    sendSignedCheckBox->setEnabled(false);
    sendStringLineEdit->setEnabled(false);
    sendStringNewlineCheckBox->setEnabled(false);
}

/**
 * @brief Saves the chart to an png image
 *
 */
void MainWindow::saveImage()
{
    auto fileName{ createFileDialog(QFileDialog::AcceptSave, u"Images (*.png)"_s, u"png"_s) };
    if (fileName.isEmpty())
        return;
    QPixmap pixMap{ chart->grab() };
    pixMap.save(fileName, "PNG");
}

/**
 * @brief A helper function to create a file dialog with specified parameters
 *
 * This function can be used to create an open dialog or save dialog
 * with the file extension filter and default file suffix.
 *
 * @param acceptMode
 * @param nameFilter
 * @param defaultSuffix
 * @return QString The selected file name or empty string if none was selected

 */
QString MainWindow::createFileDialog(QFileDialog::AcceptMode acceptMode, const QString &nameFilter,
                                     const QString &defaultSuffix)
{
    QFileDialog fileDialog{ this };
    fileDialog.setAcceptMode(acceptMode);
    fileDialog.setNameFilter(nameFilter);
    fileDialog.setDefaultSuffix(defaultSuffix);
    if (!fileDialog.exec() || fileDialog.selectedFiles().isEmpty())
        return u""_s;
    else
        return fileDialog.selectedFiles().at(0);
}

/**
 * @brief A helper function to open a file and create a QTextStream on it
 *
 * It uses a custom deleter which deletes the attached file when the stream is deleted
 *
 * @param fileName
 * @param fileFlags
 * @return std::unique_ptr to QTextStream or nullptr if can't open a file
 */
std::unique_ptr<QTextStream, std::function<void(QTextStream *)>>
MainWindow::openFileStream(const QString &fileName, QIODevice::OpenMode fileFlags)
{
    const auto file{ new QFile{ fileName } };
    const auto fileDeleter{ [file](QTextStream *stream) {
        delete stream;
        file->deleteLater();
    } };
    if (!file->open(fileFlags)) {
        QMessageBox::warning(this, u"Warning"_s, file->errorString());
        file->deleteLater();
        return { nullptr };
    } else {
        return { new QTextStream{ file }, fileDeleter };
    }
}

/**
 * @brief Saves data from the chart to a text file
 *
 * Saved data depends on the current chart type (chart or spectrum).
 *
 */
void MainWindow::savePlotData()
{
    const auto fileName{ createFileDialog(QFileDialog::AcceptSave, u"Text files (*.txt)"_s,
                                          u"txt"_s) };
    if (fileName.isEmpty())
        return;

    auto stream{ openFileStream(fileName, QIODevice::WriteOnly | QIODevice::Text) };
    if (stream.get() == nullptr)
        return;

    const auto dataList{ chart->getPlotData() };
    for (const auto &data : *dataList)
        *stream << QString::number(data.mainValue()) << u"\n"_s;
}

void MainWindow::saveSpectrumData()
{
    const auto fileName{ createFileDialog(QFileDialog::AcceptSave, u"Text files (*.txt)"_s,
                                          u"txt"_s) };
    if (fileName.isEmpty())
        return;

    auto stream{ openFileStream(fileName, QIODevice::WriteOnly | QIODevice::Text) };
    if (stream.get() == nullptr)
        return;

    const auto dataList{ chart->getSpectrumData() };
    const auto dataEnd{ dataList->constEnd() };
    for (auto it{ dataList->constBegin() }; it != dataEnd; ++it)
        *stream << QString::number(it->mainKey()) << u" "_s << QString::number(it->mainValue())
                << u"\n"_s;
}

/**
 * @brief Opens the text file and displays plot data from it
 *
 */
void MainWindow::openPlotData()
{
    const auto fileName{ createFileDialog(QFileDialog::AcceptOpen, u"Text files (*.txt)"_s,
                                          u"txt"_s) };
    if (fileName.isEmpty())
        return;

    auto stream{ openFileStream(fileName, QIODevice::ReadOnly | QIODevice::Text) };
    if (stream.get() == nullptr)
        return;

    chart->clear();
    const auto dataList{ QSharedPointer<QList<qreal>>::create() };
    bool ok;
    qreal num;
    const auto warningMsg{ u"Can't read plot data at line "_s };
    quint64 lineCount{ 0 };
    while (!stream->atEnd()) {
        num = stream->readLine().toDouble(&ok);
        if (!ok) {
            QMessageBox::warning(this, u"Warning"_s, warningMsg + QString::number(lineCount));
            return;
        }

        dataList->append(num);

        ++lineCount;
    }
    chart->addData(dataList);
}

/**
 * @brief Opens the text file and displays spectrum data from it
 *
 */
void MainWindow::openSpectrumData()
{
    const auto fileName{ createFileDialog(QFileDialog::AcceptOpen, u"Text files (*.txt)"_s,
                                          u"txt"_s) };
    if (fileName.isEmpty())
        return;

    auto stream{ openFileStream(fileName, QIODevice::ReadOnly | QIODevice::Text) };
    if (stream.get() == nullptr)
        return;

    chart->clear();
    QCPGraphDataContainer data;
    QStringList xy;
    qreal x;
    qreal y;
    bool ok;
    quint64 lineNumber{ 0 };
    const auto warningMsg{ u"Can't read spectrum data at line "_s };
    while (!stream->atEnd()) {
        xy = stream->readLine().split(QRegularExpression{ u"\\s+"_s }, Qt::SkipEmptyParts);
        if (xy.empty() || (xy.size() < 2)) {
            QMessageBox::warning(this, u"Warning"_s, warningMsg + QString::number(lineNumber));
            return;
        }

        x = xy.at(0).toDouble(&ok);
        if (!ok) {
            QMessageBox::warning(this, u"Warning"_s, warningMsg + QString::number(lineNumber));
            return;
        }

        y = xy.at(1).toDouble(&ok);
        if (!ok) {
            QMessageBox::warning(this, u"Warning"_s, warningMsg + QString::number(lineNumber));
            return;
        }

        data.add(QCPGraphData{ x, y });

        ++lineNumber;
    }
    chart->setRawSpectrumData(data);
}

/**
 * @brief Updates the status bar chart type indicator (plot or spectrum)
 *
 */
void MainWindow::chartTypeChanged() const
{
    const auto chartType{ chart->getChartType() };
    if (chartType == Chart::ChartType::plot)
        chartTypeWidget->setText(u"Chart type: Plot"_s);
    else if (chartType == Chart::ChartType::spectrum)
        chartTypeWidget->setText(u"Chart type: Spectrum"_s);
}

/**
 * @brief Updates the message in the status bar with the selected point
 *
 * @param point
 */
void MainWindow::updateSelectedPoint(const QPointF point) const
{
    QString msg{ u"x: "_s };
    msg += QString::number(point.x());
    msg += u"     y: "_s;
    msg += QString::number(point.y());
    statusBar->showMessage(msg);
}
