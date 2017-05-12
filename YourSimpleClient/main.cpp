#include <QCoreApplication>
#include "socketclientchat.h"

using namespace YourSimpleClient;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    SocketClientChat chat;

    return app.exec();
}
