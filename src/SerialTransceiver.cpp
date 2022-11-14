#include "SerialTransceiver.h"

// for some reason 0xff is always received twice on my PC
// can't reproduce on anything else
// define it if this problem occurs for you too
// #define BUG_0xff

SerialTransceiver::SerialTransceiver(QObject *parent)
    : QObject{ parent },
      serialPort{ new QSerialPort{ this } },
      timer{ new QTimer{ this } },
      dataList{ new QList<qreal> }
{
    connect(serialPort, &QSerialPort::readyRead, this, &SerialTransceiver::receiveData);
    connect(timer, &QTimer::timeout, this, &SerialTransceiver::timerTimeout);
}

SerialTransceiver::~SerialTransceiver()
{
    serialClose();
    delete dataList;
}

bool SerialTransceiver::serialOpen()
{
    bool isOpen{ serialPort->open(QIODevice::ReadWrite) };
    if (isOpen) {
        serialPort->clear();
        timer->start(200);
    }
    return isOpen;
}

void SerialTransceiver::serialClose()
{
    if (serialPort->isOpen())
        serialPort->close();
    if (timer->isActive())
        timer->stop();
    dataList->clear();
    bufferArray.clear();
}

template<typename T>
void SerialTransceiver::deserializeByteArray(QByteArray *byteArray)
{
#ifdef BUG_0xff
    for (qsizetype i = 0; i < byteArray->size(); ++i) {
        if (byteArray->at(i) == static_cast<char>(0xff))
            byteArray->remove(i++, 1);
    }
#endif

    byteArray->append(bufferArray);
    bufferArray.clear();

    qsizetype byteArraySize{ byteArray->size() };
    quint8 byteCnt{ static_cast<quint8>(byteArraySize % sizeof(T)) };

    bufferArray = byteArray->last(byteCnt);
    byteArray->chop(byteCnt);

    T data;
    QDataStream stream{ byteArray, QIODevice::ReadOnly };
    if (dataType == f32)
        stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    while (!stream.atEnd()) {
        stream >> data;
        dataList->append(data);
    }
}

void SerialTransceiver::receiveData()
{
    QByteArray byteArray{ serialPort->readAll() };

    switch (dataType) {
    case DataTypes::u8:
        deserializeByteArray<quint8>(&byteArray);
        break;
    case DataTypes::u16:
        deserializeByteArray<quint16>(&byteArray);
        break;
    case DataTypes::u32:
        deserializeByteArray<quint32>(&byteArray);
        break;
    case DataTypes::u64:
        deserializeByteArray<quint64>(&byteArray);
        break;
    case DataTypes::i8:
        deserializeByteArray<qint8>(&byteArray);
        break;
    case DataTypes::i16:
        deserializeByteArray<qint16>(&byteArray);
        break;
    case DataTypes::i32:
        deserializeByteArray<qint32>(&byteArray);
        break;
    case DataTypes::i64:
        deserializeByteArray<qint64>(&byteArray);
        break;
    case DataTypes::f32:
        deserializeByteArray<float>(&byteArray);
        break;
    case DataTypes::f64:
        deserializeByteArray<double>(&byteArray);
        break;
    }
}

void SerialTransceiver::timerTimeout()
{
    if (!dataList->isEmpty()) {
        emit newDataAvailable(dataList);
    }
}

void SerialTransceiver::setDataType(DataTypes dataType)
{
    this->dataType = dataType;
}

bool SerialTransceiver::setBaudRate(qint32 baudRate, QSerialPort::Directions directions)
{
    return serialPort->setBaudRate(baudRate, directions);
}
void SerialTransceiver::setPortName(const QString &name)
{
    serialPort->setPortName(name);
}
QString SerialTransceiver::errorString() const
{
    return serialPort->errorString();
}
bool SerialTransceiver::setFlowControl(QSerialPort::FlowControl flowControl)
{
    return serialPort->setFlowControl(flowControl);
}
bool SerialTransceiver::setStopBits(QSerialPort::StopBits stopBits)
{
    return serialPort->setStopBits(stopBits);
}
bool SerialTransceiver::setParity(QSerialPort::Parity parity)
{
    return serialPort->setParity(parity);
}
bool SerialTransceiver::setDataBits(QSerialPort::DataBits dataBits)
{
    return serialPort->setDataBits(dataBits);
}
