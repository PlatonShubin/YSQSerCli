#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QTcpSocket>
#include <QDataStream>

namespace YourSimpleClient {

class DataBlock; //forward declaration

using ClientId = quint16;

enum ClientType : quint8 {
    None = 0,
    Server = 1,
    Client,
    Group
};

enum ClientStatus {
    Disonnected,
    Connecting,
    Connected
};

enum CoreCommand {
    ProvideInfo = 1,
    RequestInfo
};

class SocketClientCore : public QTcpSocket {
    Q_OBJECT

public:
    explicit SocketClientCore(QObject *parent = 0);
    ~SocketClientCore() = default;

    QString getName() const { return clientName; }
    void setName(const QString &name) { clientName = name; }

    ClientId getId() const { return clientId; }
    void setId(ClientId id) { clientId = id; }
    bool hasId() const { return clientId != 0; }

    ClientStatus getStatus() const { return clientStatus; }
    void setStatus(ClientStatus status) { clientStatus = status; }

signals:
    void blockReceived(DataBlock &block);

private slots:
    void onSocketReadyRead();

public:
    void sendBlock(const DataBlock &block);

private:
    quint16 lastBlockSize = 0;

    QString clientName;
    ClientId clientId = 0; //id not initialized
    ClientStatus clientStatus = Disonnected;
};

}

#endif // SOCKETCLIENT_H
