#ifndef AF_CURRENCY_HTTP_SERVER
#define AF_CURRENCY_HTTP_SERVER

#include <QtCore/QByteArray>
#include "serverHost.h"
#include <QtHttpServer/QHttpServer>

class CurrencyHttpServer final : public QObject
{
    Q_OBJECT
public:
    explicit CurrencyHttpServer(QObject* parent = nullptr);
    ~CurrencyHttpServer() = default;

    void init();
    int listen(const QHostAddress &address = QHostAddress::Any, quint16 port = 0);

private:
    static QByteArray currency(const QHttpServerRequest &request);
    static QByteArray latest();
    static QByteArray update(const QHttpServerRequest &request);

    QHttpServer* m_server;
    AFHostServer* m_fixerServer;
};

#endif // AF_CURRENCY_HTTP_SERVER
