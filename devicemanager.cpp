#include <QDebug>
#include "devicemanager.h"

DeviceManager *DeviceManager::instance = NULL;

DeviceManager::DeviceManager(QObject *parent) :
    QObject(parent)
{
    peerUID = 0;
    setup();
}

DeviceManager *DeviceManager::getInstance()
{
    if(instance == NULL)
        instance = new DeviceManager();
    return instance;
}

void DeviceManager::setup()
{
    QAudioFormat format;
    format.setFrequency(8000);
    format.setChannels(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(format)) {
        qWarning()<<"default format not supported try to use nearest";
        format = info.nearestFormat(format);
    }

    audioInput = new QAudioInput(format, this);
    audioOutput = new QAudioOutput(format, this);
}

bool DeviceManager::getDevice(quint32 peerUID, QAudioInput **input, QAudioOutput **output)
{
    if(this->peerUID > 0)
        return false;
    this->peerUID = peerUID;
    *input = audioInput;
    *output = audioOutput;
    return true;
}

bool DeviceManager::releaseDevice(quint32 peerUID)
{
    if(this->peerUID != peerUID)
        return false;
    this->peerUID = 0;
    return true;
}
