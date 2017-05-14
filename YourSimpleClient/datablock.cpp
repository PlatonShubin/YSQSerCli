#include <QDebug>

#include "datablock.h"

using namespace YourSimpleClient;

DataBlock::DataBlock(quint8 com, const QByteArray &arg) :
    _command(com), _argument(arg)
{
    ;
}

DataBlock &DataBlock::toServer()
{
    _receiverType = Server;
    _receiverId = 0;

    return *this;
}

DataBlock &DataBlock::fromServer()
{
    _senderType = Server;
    _senderId = 0;

    return *this;
}

DataBlock &DataBlock::toClient(ClientId id)
{
    _receiverType = Client;
    _receiverId = id;

    return *this;
}

DataBlock &DataBlock::fromClient(ClientId id)
{
    _senderType = Client;
    _senderId = id;

    return *this;
}

QDataStream &YourSimpleClient::operator <<(QDataStream &out, const DataBlock &b)
{
    //Here we use Qt serialization model and transaction mechanism
    //It really needs much performance testing
    out << b._command << b._argument;
    out << (quint8 &)b._senderType << b._senderId;
    out << (quint8 &)b._receiverType << b._receiverId;

    return out;
}

QDataStream &YourSimpleClient::operator >>(QDataStream &in, DataBlock &b)
{
    //Here we use Qt serialization model and transaction mechanism
    //It really needs much performance testing
    //This code is particularly unoptimised
    //Due to possible transaction rollbacks
    in >> b._command >> b._argument;
    in >> (quint8 &)b._senderType >> b._senderId;
    in >> (quint8 &)b._receiverType >> b._receiverId;

    return in;
}
