#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow{parent},
    ui{new Ui::MainWindow},
    plot{new Plot{this}},
    portSettingsDialog{new PortSettingsDialog{this}},
    dataSettingsDialog{new DataSettingsDialog{this}},
    serialTransceiver{new SerialTransceiver{this}},
    chartTypeWidget{new TextWidget{this}}
{
    ui->setupUi(this);

    setCentralWidget(plot);

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionPortSettings->setEnabled(true);

    ui->statusBar->addPermanentWidget(chartTypeWidget);
    chartTypeWidget->setText("Chart type: Spectre");

    connect(serialTransceiver, &SerialTransceiver::newDataAvailable, plot, &Plot::addData);
    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::serialConnect);
    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::serialDisconnect);
    connect(ui->actionPortSettings, &QAction::triggered, portSettingsDialog, &PortSettingsDialog::show);
    connect(ui->actionDataSettings, &QAction::triggered, dataSettingsDialog, &DataSettingsDialog::show);
    connect(ui->actionSaveImage, &QAction::triggered, this, &MainWindow::saveImage);
    connect(ui->actionClear, &QAction::triggered, this, &MainWindow::clearChart);
    connect(ui->actionChartType, &QAction::triggered, plot, &Plot::changeType);
    connect(ui->actionChartType, &QAction::triggered, this, &MainWindow::updateDataSettingsDialog);
    connect(ui->actionChartType, &QAction::triggered, this, &MainWindow::statusBarUpdateChartType);
    connect(ui->actionSaveData, &QAction::triggered, this, &MainWindow::saveData);
    connect(ui->actionOpenData, &QAction::triggered, this, &MainWindow::openData);
    connect(ui->actionResetZoom, &QAction::triggered, plot, &Plot::resetZoom);
}

MainWindow::~MainWindow()
{
    delete serialTransceiver;
    delete ui;
}

QString MainWindow::createFileDialog(QFileDialog::AcceptMode acceptMode, QString nameFilter, QString defaultSuffix)
{
        QFileDialog fileDialog{this};
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
    const auto& serialSettings {portSettingsDialog->getCurrentSettings()};
    const auto dataType {dataSettingsDialog->getCurrentDataType()};
    serialTransceiver->setDataType(dataType);
    serialTransceiver->setPortName(serialSettings.name);
    serialTransceiver->setBaudRate(serialSettings.baudRate);
    serialTransceiver->setDataBits(serialSettings.dataBits);
    serialTransceiver->setParity(serialSettings.parity);
    serialTransceiver->setStopBits(serialSettings.stopBits);
    serialTransceiver->setFlowControl(serialSettings.flowControl);
    if (serialTransceiver->serialOpen()) {
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionPortSettings->setEnabled(false);
        ui->actionDataSettings->setEnabled(false);
        ui->actionOpenData->setEnabled(false);
        ui->actionSaveData->setEnabled(false);
        ui->actionChartType->setEnabled(false);
    } else {
        QMessageBox::critical(this, "Error", serialTransceiver->errorString());
    }
}

void MainWindow::serialDisconnect()
{
    serialTransceiver->serialClose();
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionPortSettings->setEnabled(true);
    ui->actionDataSettings->setEnabled(true);
    ui->actionOpenData->setEnabled(true);
    ui->actionSaveData->setEnabled(true);
    ui->actionChartType->setEnabled(true);
}

void MainWindow::saveImage()
{
    auto fileName {createFileDialog(QFileDialog::AcceptSave, "Images (*.png)", "png")};
    if (fileName.isEmpty())
        return;
    QPixmap pixMap {plot->grab()};
    pixMap.save(fileName, "PNG");
}

void MainWindow::clearChart()
{
    plot->clear();
}

void MainWindow::saveData()
{
    auto fileName {createFileDialog(QFileDialog::AcceptSave, "Text files (*.txt)", "txt")};
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        const auto dataList {plot->getData()};
        QTextStream stream(&file);
        for (const auto& data : dataList)
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
    auto style {plot->getCurveStyle()};
    if (style == QwtPlotCurve::Sticks)
        dataSettingsDialog->hideAdditionalDataTypes();
    else if (style == QwtPlotCurve::Lines)
        dataSettingsDialog->showAdditionalDataTypes();
}

void MainWindow::statusBarUpdateChartType()
{
    auto style {plot->getCurveStyle()};
    if (style == QwtPlotCurve::Sticks)
        chartTypeWidget->setText("Chart type: Spectre");
    else if (style == QwtPlotCurve::Lines)
        chartTypeWidget->setText("Chart type: Plot");
}
