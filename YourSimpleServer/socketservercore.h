#ifndef SOCKETKSERVER_H
#define SOCKETKSERVER_H

#include <QTcpServer>
#include <QHash>
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

private:
    void execCommand(const DataBlock &cmdBlock, SocketClientCore *sender);
    void updateClientInfo(SocketClientCore *client, quint16 id, const QString &name);

protected:
    void incomingConnection(qintptr sockDesc);

private:
    QHash<quint16, SocketClientCore*> clients; //clients connected to the server
    QList<SocketClientCore *> noidClients; //clients without id
};

}

#endif // SOCKETKSERVER_H
