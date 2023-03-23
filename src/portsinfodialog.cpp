#include "portsinfodialog.h"

#include <qboxlayout.h>
#include <qlabel.h>
#include <qscrollarea.h>
#include <qserialportinfo.h>

/**
 * @brief Constructs a new PortsInfoDialog object
 *
 * @param parent
 */
PortsInfoDialog::PortsInfoDialog(QWidget *parent) : QDialog{ parent }
{
    auto scroll{ new QScrollArea{ this } };
    auto widget{ new QWidget{ this } };
    scroll->setWidget(widget);
    scroll->setWidgetResizable(true);
    auto layout{ new QVBoxLayout{ widget } };

    const auto portsInfo{ QSerialPortInfo::availablePorts() };
    for (const auto &info : portsInfo) {
        QString infoStr{
            "Port: " + info.portName() + "\n" + "Location: " + info.systemLocation() + "\n"
            + "Manufacturer: " + info.manufacturer() + "\n"
            + "Serial number: " + info.serialNumber() + "\n" + "Vendor identifier: "
            + (info.hasVendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : "")
            + "\n" + "Product identifier: "
            + (info.hasProductIdentifier() ? QString::number(info.productIdentifier(), 16) : "")
            + "\n"
        };
        auto label{ new QLabel{ infoStr, this } };
        layout->addWidget(label);
    }

    auto dialogLayout{ new QVBoxLayout{ this } };
    dialogLayout->addWidget(scroll);
    setWindowTitle("Ports info");
}
