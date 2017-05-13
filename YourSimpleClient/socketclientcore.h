#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QTcpSocket>
#include <QDataStream>

namespace YourSimpleClient {

struct DataBlock; //forward declaration

enum ClientType {
    Server = 1,
    Client,
    Group,
    All = Client //все - это клиент с id 0
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
    quint16 getId() const { return clientId; }
    void setId(quint16 id) { clientId = id; }
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
    quint16 clientId = 0; //id not initialized
    ClientStatus clientStatus = Disonnected;
};

}

#endif // SOCKETCLIENT_H
