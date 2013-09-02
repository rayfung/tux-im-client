#include "config.h"
#include <QTextStream>

Config::Config() : ipAddress("127.0.0.1"), port(9866), bindPort(9888)
{
    QFile   file("config.ini");

    if(file.exists())
    {
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QTextStream in(&file);
        in >> ipAddress >> port >> bindPort;
    }
}
