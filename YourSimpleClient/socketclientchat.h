#ifndef SOCKETCLIENTCHAT_H
#define SOCKETCLIENTCHAT_H

#include <QHostAddress>
#include <QTime>
#include <QThread>

#include "socketclientcore.h"
#include "datablock.h"

namespace YourSimpleClient {

class SocketClientChat : QObject
{
    Q_OBJECT

public:
    enum ChatCommand {
        ProvideInfo = 1,
        DisplayMessage
    };

public:
    explicit SocketClientChat(QObject *parent = 0);
    ~SocketClientChat();

private slots:
    void onSocketDisconnected();
    void onBlockReceived(DataBlock block);

private:
    void execCommand(const DataBlock &cmdBlock);

    void displayMessage(const QString &msg);

    void sendInfo() const;
    void sendMessage(const QString &msg);

private:
    SocketClientCore *socketClient;
    QString chat;
};

}
#endif // SOCKETCLIENTCHAT_H
