#include "DataManager.hpp"

#include "mq/ZMQSubscriber.hpp"

DataManager::DataManager(QObject* parent) : QObject{parent} {
    // ZeroMQ setup (in a separate thread)
    // move the task object to the thread BEFORE connecting any signal/slots
    thread = new QThread(this);
    worker = new ZmqSubscriber("ipc:///tmp/speed.ipc");
    worker->moveToThread(thread);

    connect(this, &DataManager::startZeroMQ, worker, &ZmqSubscriber::checkForMessages);
    connect(worker, &ZmqSubscriber::messageReceived, this, &DataManager::updateData);

    // automatically delete thread and task object when work is done:
    connect(thread, &QThread::finished, worker, &DataManager::deleteLater);  // Clean up
    connect(thread, &QThread::finished, this, &DataManager::deleteLater);

    // Start ZeroMQ
    thread->start();
    emit startZeroMQ();
}

DataManager::~DataManager() {
    qDebug() << "DataManager::closing threads";
    thread->quit();
    thread->wait();  // Ensure thread finishes
    qDebug() << "DataManager::quitting";
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

void DataManager::updateData(uint8_t speed, uint16_t rpm) {
    setSpeed(speed);
    setRpm(rpm);
}
