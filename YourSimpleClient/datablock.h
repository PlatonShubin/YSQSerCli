#ifndef DATABLOCK_H
#define DATABLOCK_H

#include <QDataStream>

#include "socketclientcore.h"

namespace YourSimpleClient {

struct DataBlock
{

    DataBlock(quint8 com, const QByteArray &arg,
              ClientType sendType, quint32 sendId,
              ClientType recType = ClientType::Server, quint32 recId = 0);

    DataBlock(quint16 blockSize, QDataStream &dataStream);

    quint32 receiver = 0;
    quint32 sender = 0;
    quint8 command = 0;
    QByteArray argument;

};

}
#endif // DATABLOCK_H
