#include "serialtransceiver.h"

#include <qregularexpression.h>
#include <type_traits>

template<typename T, typename U>
static constexpr auto CanTypeFitValue(const U value);

template<typename T>
static void writeSmallestToByteArray(QByteArray &byteArray, T num);

// See SerialTransceiver::deserializeByteArray
// #define BUG_0xff

/**
 * @brief Constructs a new SerialTransceiver object
 *
 * @param parent
 */
SerialTransceiver::SerialTransceiver(QObject *parent) : QObject{ parent }
{
    connect(serialPort, &QSerialPort::readyRead, this, &SerialTransceiver::receiveData);
}

/**
 * @brief Destroys the SerialTransceiver object and closes the serial port
 *
 */
SerialTransceiver::~SerialTransceiver()
{
    serialClose();
}

/**
 * @brief Opens the serial port and returns connection status (bool)
 *
 * @return true
 * @return false
 */
bool SerialTransceiver::serialOpen()
{
    bool isOpen{ serialPort->open(QIODevice::ReadWrite) };
    if (isOpen) {
        serialPort->clear();
    }
    return isOpen;
}

/**
 * @brief Closes the serial port
 *
 */
void SerialTransceiver::serialClose()
{
    if (serialPort->isOpen())
        serialPort->close();
    bufferArray.clear();
}

/**
 * @brief Deserializes byte array according to the current data type
 *
 * On my previous Linux machine (both Arch and Debian) there was a nasty bug.
 * If a serial port has received 0xff, it always had a duplicate next to it.
 * I've tried multiple port settings, virtual serial ports, the bug was still
 * there. Can't reproduce it on Windows but decided to leave a hacky solution.
 * I don't know if it's a bug in Qt.
 * If you have this problem, define BUG_0xff at the top of this file.
 *
 * @tparam T
 * @param byteArray
 */
template<typename T>
void SerialTransceiver::deserializeByteArray(QByteArray &byteArray, QList<qreal> &dataList)
{
#ifdef BUG_0xff
    for (qsizetype i = 0; i < byteArray->size(); ++i) {
        if (byteArray->at(i) == static_cast<char>(0xff))
            byteArray->remove(i++, 1);
    }
#endif

    byteArray.append(bufferArray);
    bufferArray.clear();

    qsizetype byteArraySize{ byteArray.size() };
    quint8 byteCnt{ static_cast<quint8>(byteArraySize % sizeof(T)) };

    bufferArray = byteArray.last(byteCnt);
    byteArray.chop(byteCnt);

    T data;
    QDataStream dataStream{ &byteArray, QIODevice::ReadOnly };
    dataStream.setByteOrder(byteOrder);
    if (dataType == DataTypes::f32)
        dataStream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    while (!dataStream.atEnd()) {
        dataStream >> data;
        dataList.append(data);
    }
}

/**
 * @brief Receives data according to the current data type
 *
 */
void SerialTransceiver::receiveData()
{
    QByteArray byteArray{ serialPort->readAll() };

    QList<qreal> dataList;

    switch (dataType) {
    case DataTypes::u8:
        deserializeByteArray<quint8>(byteArray, dataList);
        break;
    case DataTypes::u16:
        deserializeByteArray<quint16>(byteArray, dataList);
        break;
    case DataTypes::u32:
        deserializeByteArray<quint32>(byteArray, dataList);
        break;
    case DataTypes::u64:
        deserializeByteArray<quint64>(byteArray, dataList);
        break;
    case DataTypes::i8:
        deserializeByteArray<qint8>(byteArray, dataList);
        break;
    case DataTypes::i16:
        deserializeByteArray<qint16>(byteArray, dataList);
        break;
    case DataTypes::i32:
        deserializeByteArray<qint32>(byteArray, dataList);
        break;
    case DataTypes::i64:
        deserializeByteArray<qint64>(byteArray, dataList);
        break;
    case DataTypes::f32:
        deserializeByteArray<float>(byteArray, dataList);
        break;
    case DataTypes::f64:
        deserializeByteArray<double>(byteArray, dataList);
        break;
    case DataTypes::ascii: {
        auto containsSeparator{ false };

        byteArray.prepend(bufferArray);
        bufferArray.clear();

        for (auto byte : byteArray) {
            if (QChar::isSpace(byte))
                containsSeparator = true;
        }
        if (!containsSeparator)
            bufferArray.prepend(byteArray);
        else {
            double num;
            bool ok;
            auto stringList{ QString{ byteArray }.split(QRegularExpression{ "\\s+" },
                                                        Qt::SkipEmptyParts) };
            for (auto str : stringList) {
                num = str.toDouble(&ok);
                if (ok)
                    dataList.append(num);
            }
        }
        break;
    }
    }

    if (!dataList.isEmpty()) {
        auto dataShared{ QSharedPointer<QList<qreal>>::create(dataList) };
        emit newDataAvailable(dataShared);
    }
}

template<typename T, typename U>
static constexpr auto CanTypeFitValue(const U value)
{
    return ((value > static_cast<U>(0)) == (static_cast<T>(value) > static_cast<T>(0)))
            && (static_cast<U>(static_cast<T>(value)) == value);
}

template<typename T>
static void writeSmallestToByteArray(QByteArray &byteArray, T num, QDataStream::ByteOrder byteOrder)
{
    auto dataStream{ QDataStream{ &byteArray, QIODeviceBase::WriteOnly } };
    dataStream.setByteOrder(byteOrder);

    if (std::is_signed_v<T>) {
        if (CanTypeFitValue<qint8>(num))
            dataStream << static_cast<qint8>(num);
        else if (CanTypeFitValue<qint16>(num))
            dataStream << static_cast<qint16>(num);
        else if (CanTypeFitValue<qint32>(num))
            dataStream << static_cast<qint32>(num);
        else
            dataStream << num;
    } else {
        if (CanTypeFitValue<quint8>(num))
            dataStream << static_cast<quint8>(num);
        else if (CanTypeFitValue<quint16>(num))
            dataStream << static_cast<quint16>(num);
        else if (CanTypeFitValue<quint32>(num))
            dataStream << static_cast<quint32>(num);
        else
            dataStream << num;
    }
}

qint64 SerialTransceiver::writeNumber(const QString &numString, bool isSigned)
{
    QByteArray byteArray;
    qint64 numSigned;
    quint64 numUnsigned;
    bool ok;

    if (isSigned)
        numSigned = numString.toLongLong(&ok, 0);
    else
        numUnsigned = numString.toULongLong(&ok, 0);

    if (ok) {
        if (isSigned)
            writeSmallestToByteArray(byteArray, numSigned, byteOrder);
        else
            writeSmallestToByteArray(byteArray, numUnsigned, byteOrder);

        return serialPort->write(byteArray);
    } else {
        return 0;
    }
}

qint64 SerialTransceiver::writeString(const QString &string, bool appendNewline)
{
    auto byteArray{ string.toUtf8() };

    if (appendNewline)
        byteArray.append('\n');

    return serialPort->write(byteArray);
}
