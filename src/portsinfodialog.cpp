#include "portsinfodialog.h"

#include <qboxlayout.h>
#include <qlabel.h>
#include <qscrollarea.h>
#include <qserialportinfo.h>

using namespace Qt::Literals::StringLiterals;

PortsInfoDialog::PortsInfoDialog(QWidget *parent) : QDialog{ parent }
{
    const auto scroll{ new QScrollArea{ this } };
    const auto widget{ new QWidget{ this } };
    scroll->setWidget(widget);
    scroll->setWidgetResizable(true);
    const auto layout{ new QVBoxLayout{ widget } };

    const auto portsInfo{ QSerialPortInfo::availablePorts() };
    for (const auto &info : portsInfo) {
        const QString infoStr{
            u"Port: "_s + info.portName() + u"\n"_s + u"Location: "_s + info.systemLocation()
            + u"\n"_s + u"Manufacturer: "_s + info.manufacturer() + u"\n"_s + u"Serial number: "_s
            + info.serialNumber() + u"\n"_s + u"Vendor identifier: "_s
            + (info.hasVendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : u""_s)
            + u"\n"_s + u"Product identifier: "_s
            + (info.hasProductIdentifier() ? QString::number(info.productIdentifier(), 16) : u""_s)
            + u"\n"_s
        };
        const auto label{ new QLabel{ infoStr, this } };
        layout->addWidget(label);
    }

    const auto dialogLayout{ new QVBoxLayout{ this } };
    dialogLayout->addWidget(scroll);
    setWindowTitle(u"Ports info"_s);
}
