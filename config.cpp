#include "config.h"
#include <QTextStream>

Config::Config() : ipAddress("10.30.25.35"), port(9866), bindPort(9888)
{
    QFile   file("conf.ini");

    if(file.exists())
    {
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                 return;

             QTextStream in(&file);
             in >> ipAddress >> port >> bindPort;
    }
}
