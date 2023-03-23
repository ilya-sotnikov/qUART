#ifndef PORTSINFODIALOG_H
#define PORTSINFODIALOG_H

#include <qdialog.h>

/**
 * @brief A dialog with information about all available serial ports
 *
 */
class PortsInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PortsInfoDialog(QWidget *parent = nullptr);
};

#endif // PORTSINFODIALOG_H
