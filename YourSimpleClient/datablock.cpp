#include <QDebug>

#include "datablock.h"

using namespace YourSimpleClient;

DataBlock::DataBlock(quint8 com, const QByteArray &arg,
                     ClientType sendType, quint32 sendId,
                     ClientType recType, quint32 recId) :
    receiver(recType + (recId << 2)),
    sender(sendType + (sendId << 2)),
    command(com),
    argument(arg)
{
    ;
}

DataBlock::DataBlock(quint16 blockSize, QDataStream &dataStream)
{
    dataStream >> receiver;
    dataStream >> sender;
    dataStream >> command;

    int argSize = blockSize - sizeof(receiver) - sizeof(sender) - sizeof(command);
    char *argData = new char[argSize + 1];
    argData[argSize] = '\0';
    int read = dataStream.readRawData(argData, argSize);
    if (read < argSize) qDebug() << "Read less bytes than needed!";

    argument.append(argData); //всё остальное
}
