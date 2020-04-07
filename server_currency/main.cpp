#include <QtCore/QCoreApplication>
#include <AFcurrency/AfCurrencyData>
#include <AFbase/AfDir>

#include "currencyHttpServer.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    app.setOrganizationName("AF");
    app.setApplicationName("server_currency");

    ushort port = 55555;
    for (auto it : app.arguments())
        if (it.left(5).toLower() == "port:"){
            port = it.right(it.length() - 5).toUShort();
            break;
        }

    AFlib::afDir()->cpDirectory(":/data", AFlib::afDir()->storage().path() + "/currency");

    CurrencyAF::ByDate::ignoreUpdater = true;
    CurrencyAF::Data::addCurrency(CurrencyAF::Type::allShort());

    CurrencyHttpServer server(&app);
    server.listen(QHostAddress::Any, port);
    qDebug() << QCoreApplication::translate("CurrencyServer", "Running on http://127.0.0.1:%1/").arg(port);

    return app.exec();
}
