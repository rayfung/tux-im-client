#ifndef CONFIG_H
#define CONFIG_H

#include <QFile>
#include <QString>

class Config
{
public:
    Config();

    QString ipAddress;
    quint16 port;
};

#endif // CONFIG_H
