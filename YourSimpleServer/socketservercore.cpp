#include "socketservercore.h"

using namespace YourSimpleServer;

SocketServerCore::SocketServerCore(QObject *parent) : QTcpServer(parent)
{
    socketClients = new QList<SocketClientCore *>();

    if (listen(QHostAddress::Any, 27016))
        qDebug() << "Server successfully started on port 27016";
}

SocketServerCore::~SocketServerCore()
{
    delete socketClients;
}

void SocketServerCore::incomingConnection(qintptr sockDesc)
{
    SocketClientCore *sockClient = new SocketClientCore(this);

    if (!sockClient->setSocketDescriptor(sockDesc))
        qDebug() << "Error. Socket descriptor not set.";

    socketClients->append(sockClient);

    qDebug() << QString("New incoming connection from %1:%2").arg(sockClient->peerAddress().toString().remove(0, 7)).arg(sockClient->peerPort());

    //Слот будет вызываться при готовности сокета к чтению из него.
    connect(sockClient, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
    connect(sockClient, SIGNAL(blockReceived(DataBlock &)), this, SLOT(onBlockReceived(DataBlock &)));
}

void SocketServerCore::onSocketDisconnected()
{
    SocketClientCore *sockClient = (SocketClientCore *)sender();

    qDebug() << QString("Client %1:%2 disconnected").arg(sockClient->peerAddress().toString().remove(0, 7)).arg(sockClient->peerPort());

    socketClients->removeOne(sockClient);
}

void SocketServerCore::onBlockReceived(DataBlock &block)
{
    if (!block.receiver)
        return;

    if (block.receiver & ClientType::Server)
    {
        qDebug() << "New message to server from client" << (block.sender >> 2); //сдвигаем на два бита, чтоб узнать id.

        execCommand(block, (SocketClientCore *)sender());
    }
    else if (block.receiver & ClientType::Client)
    {
        quint32 receiverId = block.receiver >> 2;

        if (receiverId == 0)
        {
            for (int i = 0; i < socketClients->size(); i++)
            {
                if (block.sender >> 2 == socketClients->at(i)->getId())
                    continue; //не посылать блок тому, кто его отправил

                block.receiver = ClientType::Client;
                block.receiver += socketClients->at(i)->getId() << 2;
                socketClients->at(i)->sendBlock(block);
            }

            qDebug() << "Block was sent to every client";

            //TODO: сделать, чтоб не надо было всё это удалять
            //delete block; //вроде сделано, теперь блок копируется
        }
        else for (int i = 0; i < socketClients->size(); i++) {
            if (socketClients->at(i)->getId() == receiverId)
            {
                socketClients->at(i)->sendBlock(block);
                qDebug() << "Block was sent to client" << receiverId;

                break;
            }
        }
    }
    else if (block.receiver & ClientType::Group)
    {
        //not implemented yet
    }

}

void SocketServerCore::execCommand(const DataBlock &cmdBlock, SocketClientCore *sender)
{
    switch (cmdBlock.command) {
    case CoreCommand::ProvideInfo:
        sender->setId(cmdBlock.sender >> 2);
        sender->setName(QString::fromUtf8(cmdBlock.argument));
        qDebug() << "Name for id" << sender->getId() << "is now" << sender->getName();
        break;
    default:
        break;
    }
}
