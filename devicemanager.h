#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>
#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QAudioDeviceInfo>

class DeviceManager : public QObject
{
    Q_OBJECT
public:
    static DeviceManager *getInstance();
    bool getDevice(quint32 peerUID, QAudioInput **input, QAudioOutput **output);
    bool releaseDevice(quint32 peerUID);

private:
    explicit DeviceManager(QObject *parent = 0);
    void setup();

private:
    static DeviceManager *instance;
    QAudioInput *audioInput;
    QAudioOutput *audioOutput;
    quint32 peerUID;
};

#endif // DEVICEMANAGER_H
