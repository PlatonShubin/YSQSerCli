#ifndef SOCKETKSERVER_H
#define SOCKETKSERVER_H

#include <QTcpServer>
#include <QList>

#include "../YourSimpleClient/socketclientcore.h"
#include "../YourSimpleClient/datablock.h"

using namespace YourSimpleClient;

namespace YourSimpleServer {

class SocketServerCore : public QTcpServer
{
    Q_OBJECT

public:
    explicit SocketServerCore(QObject *parent = 0);
    ~SocketServerCore();

private slots:
    void onSocketDisconnected();
    void onBlockReceived(DataBlock &block);
    void execCommand(const DataBlock &cmdBlock, SocketClientCore *sender);

protected:
    void incomingConnection(qintptr sockDesc);

private:
    //клиентские сокеты, кто сейчас подключен
    QList<SocketClientCore *> *socketClients;
};

}

#endif // SOCKETKSERVER_H
