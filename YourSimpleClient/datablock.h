#ifndef DATABLOCK_H
#define DATABLOCK_H

#include <QDataStream>

#include "socketclientcore.h"

namespace YourSimpleClient {

class DataBlock
{
public:
    DataBlock() = default;
    DataBlock(quint8 com, const QByteArray &arg);

    //setters
    DataBlock &toServer();
    DataBlock &fromServer();
    DataBlock &toClient(ClientId id);
    DataBlock &fromClient(ClientId id);
    DataBlock &toAll() { return toClient(0); }
    //DataBlock &toGroup();
    //DataBlock &fromGroup();

    //getters
    quint8 command() const { return _command; }
    const QByteArray &argument() const { return _argument; }
    ClientType senderType() const { return _senderType; }
    ClientId senderId() const { return _senderId; }
    ClientType receiverType() const { return _receiverType; }
    ClientId receiverId() const { return _receiverId; }

    //
    bool isForId(ClientId id) const { return receiverId() == id; }
    bool isFromId(ClientId id) const { return senderId() == id; }
    bool isForServer() const { return receiverType() == Server; }
    bool isFromServer() const { return senderType() == Server; }
    bool isForClient() const { return receiverType() == Client; }
    bool isFromClient() const { return senderType() == Client; }
    bool isForAll() const { return isForClient() && isForId(0); }

private:
    quint8 _command = 0;
    QByteArray _argument;
    ClientType _senderType = None;
    ClientId _senderId = 0;
    ClientType _receiverType = None;
    ClientId _receiverId = 0;

    friend QDataStream &operator <<(QDataStream &out, const DataBlock &b);
    friend QDataStream &operator >>(QDataStream &in, DataBlock &b);
};

QDataStream &operator <<(QDataStream &out, const DataBlock &b);
QDataStream &operator >>(QDataStream &in, DataBlock &b);

}


#endif // DATABLOCK_H
