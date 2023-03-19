#include "SerialTransceiver.h"

// See SerialTransceiver::deserializeByteArray
// #define BUG_0xff

/**
 * @brief Construct a new SerialTransceiver object
 * 
 * @param parent 
 */
SerialTransceiver::SerialTransceiver(QObject *parent)
    : QObject{parent}
    , serialPort{new QSerialPort{this}}
    , timer{new QTimer{this}}
    , dataList{new QList<qreal>}
{
    connect(serialPort, &QSerialPort::readyRead, this, &SerialTransceiver::receiveData);
    connect(timer, &QTimer::timeout, this, &SerialTransceiver::timerTimeout);
}

/**
 * @brief Destroy the SerialTransceiver object and its data list
 * 
 */
SerialTransceiver::~SerialTransceiver()
{
    serialClose();
    delete dataList;
}

/**
 * @brief Open the serial port and return connection status (bool)
 * 
 * It also starts the timer which will overflow every 200 msec to limit the chart update rate.
 *
 * @return true 
 * @return false 
 */
bool SerialTransceiver::serialOpen()
{
    bool isOpen{serialPort->open(QIODevice::ReadWrite)};
    if (isOpen) {
        serialPort->clear();
        timer->start(200);
    }
    return isOpen;
}

/**
 * @brief Close the serial port, clear all received data and stop the timer if it's active
 * 
 */
void SerialTransceiver::serialClose()
{
    if (serialPort->isOpen())
        serialPort->close();
    if (timer->isActive())
        timer->stop();
    dataList->clear();
    bufferArray.clear();
}

/**
 * @brief Deserialize byte array according to the current data type
 * 
 * On my previous Linux machine (both Arch and Debian) there was a nasty bug.
 * If a serial port has received 0xff, it always had a duplicate next to it.
 * I've tried multiple port settings, virtual serial ports, the bug was still there.
 * Can't reproduce it on Windows but decided to leave a hacky solution.
 * I don't know if it's a bug in Qt.
 * If you have this problem, define BUG_0xff at the top of this file.
 * 
 * @tparam T 
 * @param byteArray 
 */
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

    qsizetype byteArraySize{byteArray->size()};
    quint8 byteCnt{static_cast<quint8>(byteArraySize % sizeof(T))};

    bufferArray = byteArray->last(byteCnt);
    byteArray->chop(byteCnt);

    T data;
    QDataStream stream{byteArray, QIODevice::ReadOnly};
    if (dataType == f32)
        stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    while (!stream.atEnd()) {
        stream >> data;
        dataList->append(data);
    }
}

/**
 * @brief Receive data according to the current data type
 * 
 */
void SerialTransceiver::receiveData()
{
    QByteArray byteArray{serialPort->readAll()};

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

/**
 * @brief Emit the newDataAvailable signal on timerTimeout if there's new data
 * 
 * It gives raw pointer to received data, the received side is expected
 * to plot this data and clear the received data list.
 * 
 */
void SerialTransceiver::timerTimeout()
{
    if (!dataList->isEmpty()) {
        emit newDataAvailable(dataList);
    }
}

/**
 * @brief Set the current data type
 * 
 * @param dataType 
 */
void SerialTransceiver::setDataType(DataTypes dataType)
{
    this->dataType = dataType;
}

/**
 * @brief Set the current baud rate
 * 
 * @param baudRate 
 * @param directions 
 * @return true 
 * @return false 
 */
bool SerialTransceiver::setBaudRate(qint32 baudRate, QSerialPort::Directions directions)
{
    return serialPort->setBaudRate(baudRate, directions);
}

/**
 * @brief Set the current port name
 * 
 * @param name 
 */
void SerialTransceiver::setPortName(const QString &name)
{
    serialPort->setPortName(name);
}

/**
 * @brief Set the current flow control
 * 
 * @param flowControl 
 * @return true 
 * @return false 
 */
bool SerialTransceiver::setFlowControl(QSerialPort::FlowControl flowControl)
{
    return serialPort->setFlowControl(flowControl);
}

/**
 * @brief Set the current stop bits
 * 
 * @param stopBits 
 * @return true 
 * @return false 
 */
bool SerialTransceiver::setStopBits(QSerialPort::StopBits stopBits)
{
    return serialPort->setStopBits(stopBits);
}

/**
 * @brief Set the current parity
 * 
 * @param parity 
 * @return true 
 * @return false 
 */
bool SerialTransceiver::setParity(QSerialPort::Parity parity)
{
    return serialPort->setParity(parity);
}

/**
 * @brief Set the current data bits
 * 
 * @param dataBits 
 * @return true 
 * @return false 
 */
bool SerialTransceiver::setDataBits(QSerialPort::DataBits dataBits)
{
    return serialPort->setDataBits(dataBits);
}

/**
 * @brief Get the current error string of the serial port
 * 
 * @return QString 
 */
QString SerialTransceiver::errorString() const
{
    return serialPort->errorString();
}