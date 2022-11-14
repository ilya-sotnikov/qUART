#include "MainWindow.h"

#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{ parent },
      plot{ new Plot{ this } },
      portSettingsDialog{ new PortSettingsDialog{ this } },
      dataSettingsDialog{ new DataSettingsDialog{ this } },
      serialTransceiver{ new SerialTransceiver{ this } },
      chartTypeWidget{ new TextWidget{ this } }
{
    setMinimumSize(QSize{ 320, 240 });
    resize(QSize{ 800, 600 });
    actionConnect = new QAction{ "Connect", this };
    actionDisconnect = new QAction{ "Disconnect", this };
    actionPortSettings = new QAction{ "Port", this };
    actionClear = new QAction{ "Clear", this };
    actionSaveImage = new QAction{ "Save image", this };
    actionSaveData = new QAction{ "Save data", this };
    actionOpenData = new QAction{ "Open data", this };
    actionDataSettings = new QAction{ "Data", this };
    actionChartType = new QAction{ "Chart type", this };
    actionResetZoom = new QAction{ "Reset zoom", this };
    actionHideMarker = new QAction{ "Hide marker", this };

    centralWidget = new QWidget{ this };
    verticalLayout = new QVBoxLayout{ centralWidget };
    setCentralWidget(centralWidget);

    menuBar = new QMenuBar{ this };
    menuFile = new QMenu{ "File", menuBar };
    menuSettings = new QMenu{ "Settings", menuBar };
    setMenuBar(menuBar);

    statusBar = new QStatusBar{ this };
    setStatusBar(statusBar);

    toolBar = new QToolBar{ this };
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

    setWindowTitle("qUART");
    setCentralWidget(plot);

    actionConnect->setEnabled(true);
    actionDisconnect->setEnabled(false);
    actionPortSettings->setEnabled(true);

    statusBar->addPermanentWidget(chartTypeWidget);
    chartTypeWidget->setText("Chart type: Spectrum");

    connect(serialTransceiver, &SerialTransceiver::newDataAvailable, plot, &Plot::addData);
    connect(actionConnect, &QAction::triggered, this, &MainWindow::serialConnect);
    connect(actionDisconnect, &QAction::triggered, this, &MainWindow::serialDisconnect);
    connect(actionPortSettings, &QAction::triggered, portSettingsDialog, &PortSettingsDialog::show);
    connect(actionDataSettings, &QAction::triggered, dataSettingsDialog, &DataSettingsDialog::show);
    connect(actionSaveImage, &QAction::triggered, this, &MainWindow::saveImage);
    connect(actionClear, &QAction::triggered, this, &MainWindow::clearChart);
    connect(actionChartType, &QAction::triggered, plot, &Plot::changeType);
    connect(actionChartType, &QAction::triggered, this, &MainWindow::updateDataSettingsDialog);
    connect(actionChartType, &QAction::triggered, this, &MainWindow::statusBarUpdateChartType);
    connect(actionSaveData, &QAction::triggered, this, &MainWindow::saveData);
    connect(actionOpenData, &QAction::triggered, this, &MainWindow::openData);
    connect(actionResetZoom, &QAction::triggered, plot, &Plot::resetZoom);
    connect(actionHideMarker, &QAction::triggered, plot, &Plot::hideMarker);
    connect(plot, &Plot::pointSelected, this, &MainWindow::updateSelectedPoint);
}

MainWindow::~MainWindow()
{
    delete serialTransceiver;
}

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

void MainWindow::serialConnect()
{
    const auto &serialSettings{ portSettingsDialog->getCurrentSettings() };
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
        actionChartType->setEnabled(false);
    } else {
        QMessageBox::critical(this, "Error", serialTransceiver->errorString());
    }
}

void MainWindow::serialDisconnect()
{
    serialTransceiver->serialClose();
    actionConnect->setEnabled(true);
    actionDisconnect->setEnabled(false);
    actionPortSettings->setEnabled(true);
    actionDataSettings->setEnabled(true);
    actionOpenData->setEnabled(true);
    actionSaveData->setEnabled(true);
    actionChartType->setEnabled(true);
}

void MainWindow::saveImage()
{
    auto fileName{ createFileDialog(QFileDialog::AcceptSave, "Images (*.png)", "png") };
    if (fileName.isEmpty())
        return;
    QPixmap pixMap{ plot->grab() };
    pixMap.save(fileName, "PNG");
}

void MainWindow::clearChart()
{
    plot->clear();
}

void MainWindow::saveData()
{
    auto fileName{ createFileDialog(QFileDialog::AcceptSave, "Text files (*.txt)", "txt") };
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        const auto dataList{ plot->getData() };
        QTextStream stream(&file);
        for (const auto &data : dataList)
            stream << QString::number(data) << "\n";
    }
}

void MainWindow::openData()
{
    plot->clear();
    auto fileName = createFileDialog(QFileDialog::AcceptOpen, "Text files (*.txt)", "txt");
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QList<qreal> dataList;
        QTextStream stream(&file);
        while (!stream.atEnd())
            dataList.append(stream.readLine().toDouble());
        plot->addRawData(&dataList);
    }
}

void MainWindow::updateDataSettingsDialog()
{
    auto chartType{ plot->getChartType() };
    if (chartType == Plot::spectrum)
        dataSettingsDialog->hideAdditionalDataTypes();
    else if (chartType == Plot::plot)
        dataSettingsDialog->showAdditionalDataTypes();
}

void MainWindow::statusBarUpdateChartType()
{
    auto chartType{ plot->getChartType() };
    if (chartType == Plot::spectrum)
        chartTypeWidget->setText("Chart type: Spectrum");
    else if (chartType == Plot::plot)
        chartTypeWidget->setText("Chart type: Plot");
}

void MainWindow::updateSelectedPoint(const QPointF point)
{
    QString msg{ "x: " };
    msg += QString::number(point.x());
    msg += "     y: ";
    msg += QString::number(point.y());
    statusBar->showMessage(msg);
}