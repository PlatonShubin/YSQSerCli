#include "socketclientcore.h"
#include "datablock.h"

using namespace YourSimpleClient;

void SocketClientCore::sendBlock(const DataBlock &block)
{
    QByteArray resBlock;
    QDataStream dataStream(&resBlock, QIODevice::WriteOnly);

    dataStream << (quint16)0 << block.receiver << block.sender;
    dataStream << block.command;
    resBlock.append(block.argument);
    dataStream.device()->seek(0); dataStream << (quint16)(resBlock.size() - sizeof(quint16));

    write(resBlock);
}

SocketClientCore::SocketClientCore(QObject *parent) : QTcpSocket(parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
}

void SocketClientCore::onSocketReadyRead()
{
    QDataStream dataStream(this);

    quint64 bytes = 0;

    while ((bytes = bytesAvailable()) > 0) //Пока в сокете что-то валяется
    {
        bytes = bytesAvailable();
        if (lastBlockSize == 0) //Если блок новый
        {
            if (bytes < sizeof(lastBlockSize))
                break; //Размер меньше 2 может быть ТОЛЬКО когда блок новый

            dataStream >> lastBlockSize;
        }

        if (bytes < lastBlockSize)
            break; //ждём полного блока

        DataBlock block(lastBlockSize, dataStream);

        lastBlockSize = 0; //Готовы принимать новый блок

        emit blockReceived(block);
    }
}
