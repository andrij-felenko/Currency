#include "currencyHttpServer.h"
#include <AFcurrency/AfCurrencyData>
#include <AFbase/AfFunction>

#include <QtCore/QCoreApplication>
#include <QtCore/QDataStream>
#include <QtCore/QJsonDocument>
#include <QtCore/QTimer>

using namespace CurrencyAF;
#define HOUR 60 * 60 * 1000

CurrencyHttpServer::CurrencyHttpServer(QObject *parent) : QObject(parent)
{
    m_fixerServer = new AFHostServer(this);

    m_server = new QHttpServer(this);
    init();
}

void CurrencyHttpServer::init()
{
    m_server->route(getServerKey(RequestType::Currency), &CurrencyHttpServer::currency);
    m_server->route(getServerKey(RequestType::Latest),   &CurrencyHttpServer::latest);
    m_server->route(getServerKey(RequestType::Update),   &CurrencyHttpServer::update);
}

int CurrencyHttpServer::listen(const QHostAddress &address, quint16 port)
{
    return m_server->listen(address, port);
}

QByteArray CurrencyHttpServer::currency(const QHttpServerRequest &request)
{
    qDebug() << "Call CurrencyHttpServer::currency";
    QDataStream stream(request.body());
    QStringList list;
    stream >> list;
    qDebug() << list;

    QByteArray ret;
    QDataStream retStream(&ret, QIODevice::WriteOnly);
    retStream << RequestType::Currency << Data::getCurrency(list);
    qDebug() << "CurrencyHttpServer::currency";
    qDebug() << request.body();
    qDebug() << ret;
    return ret;
}

QByteArray CurrencyHttpServer::latest()
{
    qDebug() << "Call CurrencyHttpServer::latest";
    QByteArray ret;
    QDataStream retStream(&ret, QIODevice::WriteOnly);
    retStream << RequestType::Latest << Data::latestDateTime() << Data::getLatest();
    return ret;
}

QByteArray CurrencyHttpServer::update(const QHttpServerRequest &request)
{
    qDebug() << "Call CurrencyHttpServer::update";
    QDataStream stream(request.body());
    QStringList list;
    QDate date;
    stream >> date >> list;
    qDebug() << date << list;

    QByteArray ret;
    QDataStream retStream(&ret, QIODevice::WriteOnly);
    retStream << RequestType::Update << Data::getUpdate(date, list);
    qDebug() << "CurrencyHttpServer::update";
    qDebug() << request.body();
    qDebug() << ret;
    return ret;
}
