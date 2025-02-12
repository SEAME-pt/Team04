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
    Q_PROPERTY(float speed READ speed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(float rpm READ rpm WRITE setRpm NOTIFY rpmChanged)
    Q_PROPERTY(uint8_t battery READ battery WRITE setBattery NOTIFY batteryChanged)
    Q_PROPERTY(uint8_t temperature READ temperature WRITE setTemperature NOTIFY temperatureChanged)

   public:
    explicit DataManager(QObject* parent = nullptr);
    ~DataManager() override;

    [[nodiscard]] auto speed() const -> float { return m_speed; }
    void setSpeed(float speed);

    [[nodiscard]] auto rpm() const -> float { return m_rpm; }
    void setRpm(float rpm);

    [[nodiscard]] auto battery() const -> uint8_t { return m_battery; }

    [[nodiscard]] auto temperature() const -> uint8_t { return m_temperature; }

   private slots:
    void updateData(float speed, float rpm);
    void setTemperature(uint8_t temperature);
    void setBattery(uint8_t battery);

   signals:
    void speedChanged();
    void rpmChanged();
    void batteryChanged();
    void temperatureChanged();
    void startZeroMQ();

   private:
    float m_speed = 0;
    float m_rpm = 0;
    uint8_t m_battery = 0;
    uint8_t m_temperature = 0;
    QThread* thread;
    ZmqSubscriber* worker;
    zmq::context_t context;
};

#endif
