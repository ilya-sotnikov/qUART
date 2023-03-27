#ifndef SERIALTRANSCEIVER_H
#define SERIALTRANSCEIVER_H

#include <qserialport.h>
#include <qtimer.h>

class SerialTransceiver : public QObject
{
    Q_OBJECT

public:
    explicit SerialTransceiver(QObject *parent = nullptr);
    ~SerialTransceiver();
    bool serialOpen();
    void serialClose();

    enum class DataTypes { u8, u16, u32, u64, i8, i16, i32, i64, f32, f64 };
    Q_ENUM(DataTypes)

    auto errorString() const { return serialPort->errorString(); };
    auto setDataType(DataTypes dataType) { this->dataType = dataType; };
    auto setPortName(const QString &name) { serialPort->setPortName(name); };
    auto setDataBits(QSerialPort::DataBits dataBits) { return serialPort->setDataBits(dataBits); };
    auto setParity(QSerialPort::Parity parity) { return serialPort->setParity(parity); };
    auto setStopBits(QSerialPort::StopBits stopBits) { return serialPort->setStopBits(stopBits); }
    auto setFlowControl(QSerialPort::FlowControl flowControl)
    {
        return serialPort->setFlowControl(flowControl);
    };
    auto setBaudRate(qint32 baudRate,
                     QSerialPort::Directions directions = QSerialPort::AllDirections)
    {
        return serialPort->setBaudRate(baudRate, directions);
    };

    qint64 writeNumber(const QString &numString, bool isSigned);

private:
    QSerialPort *serialPort{ new QSerialPort{ this } };
    QTimer *timer{ new QTimer{ this } };
    QList<qreal> dataList{};
    QByteArray bufferArray;
    DataTypes dataType{ DataTypes::u8 };

    template<typename T>
    void deserializeByteArray(QByteArray &byteArray);

private slots:
    void receiveData();
    void timerTimeout();

signals:
    void newDataAvailable(QList<qreal> &dataList);
};

#endif // SERIALTRANSCEIVER_H
