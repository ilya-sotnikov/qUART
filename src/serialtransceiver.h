#ifndef SERIALTRANSCEIVER_H
#define SERIALTRANSCEIVER_H

#include <qserialport.h>

class SerialTransceiver : public QObject
{
    Q_OBJECT

public:
    explicit SerialTransceiver(QObject *parent = nullptr);
    bool serialOpen();
    void serialClose();

    enum DataTypes { ascii, u8, u16, u32, u64, i8, i16, i32, i64, f32, f64 };
    Q_ENUM(DataTypes)

    auto errorString() const { return serialPort->errorString(); };
    auto setPortName(const QString &name) { return serialPort->setPortName(name); };
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
    void setDataType(DataTypes dataType)
    {
        bufferArray.clear();
        this->dataType = dataType;
    }
    void setByteOrder(QDataStream::ByteOrder byteOrder)
    {
        bufferArray.clear();
        this->byteOrder = byteOrder;
    }

    qint64 writeNumber(const QString &numString, bool isSigned) const;
    qint64 writeString(const QString &string, bool appendNewline) const;

private:
    QSerialPort *serialPort{ new QSerialPort{ this } };
    QByteArray bufferArray;
    DataTypes dataType{ DataTypes::ascii };
    QDataStream::ByteOrder byteOrder{ QDataStream::LittleEndian };

    template<typename T>
    void deserializeByteArray(QByteArray &byteArray, QList<qreal> &dataList);

private slots:
    void receiveData();

signals:
    void newDataAvailable(const QSharedPointer<const QList<qreal>> dataList) const;
};

#endif // SERIALTRANSCEIVER_H
