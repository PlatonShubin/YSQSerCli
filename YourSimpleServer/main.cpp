#include <QCoreApplication>
#include "socketservercore.h"

using namespace YourSimpleServer;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    SocketServerCore server;

    return app.exec();
}
