#ifndef DATAMANAGER_HPP
#define DATAMANAGER_HPP

#include <QtQml/qqml.h>

#include <QtCore/QObject>
#include <QtCore/QThread>

#include "instrument-cluster/mq/ZMQSubscriber.hpp"

class DataManager : public QObject {
    // NOLINTNEXTLINE(modernize-use-trailing-return-type)
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(uint8_t speed READ speed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(uint16_t rpm READ rpm WRITE setRpm NOTIFY rpmChanged)

   public:
    explicit DataManager(QObject* parent = nullptr);
    ~DataManager() override;

    [[nodiscard]] auto speed() const -> uint8_t { return m_speed; }
    void setSpeed(uint8_t speed);

    [[nodiscard]] auto rpm() const -> uint16_t { return m_rpm; }
    void setRpm(uint16_t rpm);

   private slots:
    void updateData(uint8_t speed, uint16_t rpm);

   signals:
    void speedChanged();
    void rpmChanged();
    void startZeroMQ();

   private:
    uint8_t m_speed = 0;
    uint16_t m_rpm = 0;
    QThread* thread;
    ZmqSubscriber* worker;
};

#endif
