#include "socketclientcore.h"
#include "datablock.h"

using namespace YourSimpleClient;

void SocketClientCore::sendBlock(const DataBlock &block)
{
    QDataStream dataStream(this);

    dataStream << block;

}

SocketClientCore::SocketClientCore(QObject *parent) : QTcpSocket(parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
}

void SocketClientCore::onSocketReadyRead()
{
    QDataStream dataStream(this);
    DataBlock block;

    //for some reason if several blocks arrive too fast
    //readyRead() not emited several times
    while(bytesAvailable() > 0)
    {
        dataStream.startTransaction();

        dataStream >> block;

        //if a block wasn't full and we read past end
        if (!dataStream.commitTransaction())
            return; //then we wait for more data

        //Okay, here we send a non-const reference to the block.
        //According to the doc it should be safe even when the connection
        //is not direct. (I just don't know if it is true for non-const refs)
        emit blockReceived(block);
    }
}
