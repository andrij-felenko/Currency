#include "currencyUpdater.h"
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QDebug>

using namespace CurrencyAF;

Updater::Updater(QObject *parent) : QNetworkAccessManager(parent),
      m_url("http://data.fixer.io/api/"), m_accesKey("?access_key=33e9a96e437daadfc69cc9495422bb0b"),
      m_latest("latest")
{
    QObject::connect(this, &QNetworkAccessManager::finished, this, &Updater::onRead);
}

void Updater::request()
{
    QNetworkRequest request;
    request.setUrl(QUrl(m_url + m_latest + m_accesKey));
    get(request);
}

void Updater::request(QDate date)
{
    QNetworkRequest request;
    request.setUrl(QUrl(m_url + date.toString("yyyy-MM-dd") + m_accesKey));
    get(request);
}

void Updater::onRead(QNetworkReply *reply)
{
    QJsonObject obj = QJsonDocument::fromJson(reply->readAll()).object();
    QDate date = QDate::fromString(obj.value("date").toString(), "yyyy-MM-dd");
    QString base = obj.value("base").toString();
    QJsonObject rateObj = obj.value("rates").toObject();
    QStringList possibleList = Type::allShort();
    for (auto m_short : rateObj.keys())
        if (possibleList.contains(m_short))
            emit result(date, m_short, rateObj.value(m_short).toDouble());
    emit finish();

//    testEqual(rateObj);
}

void Updater::testEqual(QJsonObject rateObj)
{
    QStringList list = rateObj.keys();
    QStringList list2 = CurrencyAF::Type::allShort();
    list.sort();
    list2.sort();
    QStringList allList = list + list2;
    allList.removeDuplicates();
    qDebug() << "name" << list.length() << list2.length();
    for (auto it : allList)
        qDebug() << it << list.contains(it) << list2.contains(it);
    qDebug() << rateObj;
}
