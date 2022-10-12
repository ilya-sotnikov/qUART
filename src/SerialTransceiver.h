#ifndef SERIALTRANSCEIVER_H
#define SERIALTRANSCEIVER_H

#include <QObject>
#include <QDataStream>
#include <QSerialPort>
#include <QPointF>
#include <QTimer>

class SerialTransceiver : public QSerialPort
{
    Q_OBJECT

public:
    explicit SerialTransceiver(QObject* parent = nullptr);
    ~SerialTransceiver();
    bool serialOpen();
    void serialClose();

    enum DataTypes {
        u8, u16, u32, u64,
        i8, i16, i32, i64,
        f32, f64
    };
    Q_ENUM(DataTypes)
    void setDataType(DataTypes dataType);

private:
    QTimer* timer {nullptr};
    QList<qreal>* dataList {nullptr};
    DataTypes dataType {DataTypes::u8};
    QByteArray bufferArray;

    template <typename T>
    void deserializeByteArray(QByteArray* byteArray);

private slots:
    void receiveData();
    void timerTimeout();

signals:
    void newDataAvailable(QList<qreal>* dataList);
};

#endif // SERIALTRANSCEIVER_H
