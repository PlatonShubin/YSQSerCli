#include "socketservercore.h"

using namespace YourSimpleServer;

SocketServerCore::SocketServerCore(QObject *parent) : QTcpServer(parent)
{
    if (listen(QHostAddress::Any, 27016))
        qDebug() << "Server successfully started on port 27016";
}

SocketServerCore::~SocketServerCore()
{
    qDeleteAll(clients.begin(), clients.end()); //questionable
    qDeleteAll(noidClients.begin(), noidClients.end()); //questionable
}

void SocketServerCore::incomingConnection(qintptr sockDesc)
{
    SocketClientCore *sockClient = new SocketClientCore(this);

    if (!sockClient->setSocketDescriptor(sockDesc))
        qDebug() << "Error. Socket descriptor not set.";

    noidClients.append(sockClient); //client has no id yet

    qDebug() << QString("New incoming connection from %1:%2").arg(sockClient->peerAddress().toString().remove(0, 7)).arg(sockClient->peerPort());

    //Слот будет вызываться при готовности сокета к чтению из него.
    connect(sockClient, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
    connect(sockClient, SIGNAL(blockReceived(DataBlock &)), this, SLOT(onBlockReceived(DataBlock &)));
}

void SocketServerCore::onSocketDisconnected()
{
    SocketClientCore *sockClient = (SocketClientCore *)sender();

    qDebug() << QString("Client %1:%2 disconnected").arg(sockClient->peerAddress().toString().remove(0, 7)).arg(sockClient->peerPort());

    //remove from container
    if (!sockClient->hasId()) noidClients.removeOne(sockClient);
    else clients.remove(sockClient->getId());

    sockClient->deleteLater(); //as doc warns
}

void SocketServerCore::onBlockReceived(DataBlock &block)
{
    if (!block.receiver) //strange block
        return;

    if (block.receiver & ClientType::Server) //block is for the server
    {
        qDebug() << "New message to server from client" << (block.sender >> 2); //сдвигаем на два бита, чтоб узнать id.

        execCommand(block, (SocketClientCore *)sender());
    }
    else if (block.receiver & ClientType::Client) //block is for the client(s)
    {
        quint16 receiverId = block.receiver >> 2;

        if (receiverId == 0) //block is for every client
        {
            for (QHash<quint16, SocketClientCore *>::const_iterator
                 i = clients.constBegin();
                 i != clients.constEnd(); ++i) //with id only
            {
                if (block.sender >> 2 == i.value()->getId())
                    continue; //не посылать блок тому, кто его отправил

                block.receiver = ClientType::Client;
                block.receiver += i.value()->getId() << 2;
                i.value()->sendBlock(block);
            }

            qDebug() << "Block was sent to every client";

            //TODO: сделать, чтоб не надо было всё это удалять
            //delete block; //вроде сделано, теперь блок копируется
        }
        else if (clients.contains(receiverId))
        {
            clients[receiverId]->sendBlock(block);
            qDebug() << "Block was sent to the client" << receiverId;
        }
        else qDebug() << "No client with such id";
    }
    else if (block.receiver & ClientType::Group)
    {
        //not implemented yet
    }

}

void SocketServerCore::execCommand(const DataBlock &cmdBlock, SocketClientCore *client)
{
    switch (cmdBlock.command) {
    case CoreCommand::ProvideInfo:

        updateClientInfo(client, cmdBlock.sender >> 2,
                         QString::fromUtf8(cmdBlock.argument));

        break;
    default:
        break;
    }
}

void SocketServerCore::updateClientInfo(SocketClientCore *client, quint16 id, const QString &name)
{
    client->setId(id);
    client->setName(name);
    qDebug() << "Name for id" << client->getId() << "is now" << client->getName();

    if (noidClients.contains(client)) //if new without an id
    {
        noidClients.removeOne(client);
        clients[id] = client;
    }
}
