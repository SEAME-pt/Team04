#include "DataManager.hpp"

#include "instrument-cluster/mq/ZMQSubscriber.hpp"

DataManager::DataManager(QObject* parent) : QObject{parent}, context(1) {
    // ZeroMQ setup (in a separate thread)
    // move the task object to the thread BEFORE connecting any signal/slots
    thread = new QThread(this);
    std::unique_ptr<MQ::ZeroMQSocket> sub_socket =
        std::make_unique<MQ::ZeroMQSocket>(context, zmq::socket_type::sub);
    sub_socket->bind("ipc:///tmp/speed.ipc");
    worker = new ZmqSubscriber(std::move(sub_socket));
    worker->moveToThread(thread);

    connect(this, &DataManager::startZeroMQ, worker, &ZmqSubscriber::checkForMessages);
    connect(worker, &ZmqSubscriber::messageReceived, this, &DataManager::updateData);
    connect(worker, &ZmqSubscriber::batteryMessageReceived, this, &DataManager::setBattery);
    connect(worker, &ZmqSubscriber::temperatureMessageReceived, this, &DataManager::setTemperature);

    // automatically delete thread and task object when work is done:
    connect(thread, &QThread::finished, worker, &DataManager::deleteLater);  // Clean up
    connect(thread, &QThread::finished, this, &DataManager::deleteLater);

    // Start ZeroMQ
    thread->start();
    emit startZeroMQ();
}

DataManager::~DataManager() {
    qDebug() << "DataManager::shutting down context";
    context.shutdown();
    qDebug() << "DataManager::quitting thread";
    thread->quit();
    qDebug() << "DataManager::waiting thread finishes";
    thread->wait();  // Ensure thread finishes
    qDebug() << "DataManager::closing context";
    context.close();
    qDebug() << "DataManager::finished";
}

void DataManager::setSpeed(uint8_t speed) {
    if (m_speed != speed) {
        m_speed = speed;
        emit speedChanged();
    }
}

void DataManager::setRpm(uint16_t rpm) {
    if (m_rpm != rpm) {
        m_rpm = rpm;
        emit rpmChanged();
    }
}

void DataManager::setBattery(uint8_t battery) {
    if (m_battery != battery) {
        m_battery = battery;
        emit batteryChanged();
    }
}

void DataManager::setTemperature(uint8_t temperature) {
    if (m_temperature != temperature) {
        m_temperature = temperature;
        emit temperatureChanged();
    }
}

void DataManager::updateData(uint8_t speed, uint16_t rpm) {
    setSpeed(speed);
    setRpm(rpm);
}
