#include "serverHost.h"
#include "lib_currencyAF/currencyType.h"
#include "lib_currencyAF/currencyData.h"
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>

using namespace CurrencyAF;
#define HOUR 60 * 60 * 1000

AFHostServer::AFHostServer(QObject *parent) : QNetworkAccessManager(parent),
    m_accessKey("?access_key=8704c7498e5dd8143220f38d6ea43db5"),
    // 33e9a96e437daadfc69cc9495422bb0b dec 0
    // 6b9823a710368c32401bc9515833dbab dec 0
    // 23b9da795607bc2ee760d46a38b61bf7 dec 0
    // 188173f0aec06a1e34121204a4baa613 dec 0
    // 024f408d3d8e4c4f2cebeac3f75638bf dec 0
    // 4193cf026fb7776e88d8cf13c50dc0f5 dec 0
    // 5c82b4e42057cf470451f9434235a9a9 dec 0
    // ecd8747c1d9397f99daa565c8ca66d4b dec 0
    // 03ab68e2d550b99f99ebea7d24daaef8 dec 0
    // b67a3fa3082d809762ad503262b29d17 dec 0
    // ee0a50e0c36f5feccc2990530d4ae429 dec 0
    // 57c0244a940aeb655c1f526298f56bb1 dec 0
    // c9aaa41ff0721a90514a5ac9b4dffae4 dec 0
    // d0e51fcc8d664430d69377bad04beff4 dec 0
    // d067366558778d3522528d90ce2e483d dec 0
    // 8704c7498e5dd8143220f38d6ea43db5 dec jan 0
    // d1f29be92416adc654f63cd524553d64 dec jan 0
    // 8ade05ca2d164730c6e69973e46c98e3 dec jan 0
    // 3932546593b313897b1677411570ccb3 dec jan 0
    // 3643433ebdb91f0c8672f0cefa5a169b dec jan 0
    // 48e2441a32913f0703dd52c8c7f2456f dec jan 0
    // 2f57d4724959e0cb1d4e21755bd04563 dec jan 0
    // c3ab98b9db57e385049019c3f6ee2f54 dec jan 0
    //
    // main: 496299ab5db8dc64e2c8f42bcff5d896
    m_url("http://data.fixer.io/api/"), actualRequest(0)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(HOUR);

    // if last update was more than 1 hour before, send it now
    if (Data::latestDateTime().secsTo(QDateTime::currentDateTime()) >= HOUR)
        updateLatest();
    if (Data::latestDate() != QDate::currentDate().addDays(-1))
        updateAfterDate(Data::latestDate());

    QDateTime nearDTime = QDateTime::currentDateTime().addMSecs(HOUR);
    nearDTime.time().setHMS(nearDTime.time().hour(), 0, 15);
    QTimer::singleShot(QDateTime::currentDateTime().secsTo(nearDTime), m_timer, static_cast<void (QTimer::*)()>(&QTimer::start));
    QObject::connect(this, &QNetworkAccessManager::finished, this, &AFHostServer::onRead);
    QObject::connect(m_timer, &QTimer::timeout, this, &AFHostServer::timeout);
}

void AFHostServer::updateLatest()
{
    QNetworkRequest request;
    actualRequest++;
    request.setUrl(QUrl(m_url + "latest" + m_accessKey));
    get(request);
}

void AFHostServer::updateAfterDate(const QDate &date)
{
    qDebug() << "Latest date:" << date;
    QDate it_date = date.addDays(1);
    while (it_date.toJulianDay() < QDate::currentDate().toJulianDay()){
        updateDate(it_date);
        it_date = it_date.addDays(1);
    }
}

void AFHostServer::updateDate(const QDate &date)
{
    QNetworkRequest request;
    actualRequest++;
    request.setUrl(QUrl(m_url + date.toString("yyyy-MM-dd") + m_accessKey));
    get(request);
}

void AFHostServer::onRead(QNetworkReply *reply)
{
    QJsonObject obj = QJsonDocument::fromJson(reply->readAll()).object();
    if (not obj.value("success").toBool()){
        qWarning() << "Success false";
        reply->deleteLater();
    }

    if (obj.value("success").toBool()) {
        QDate date = QDate::fromString(obj.value("date").toString(), "yyyy-MM-dd");
        QString base = obj.value("base").toString();
        QJsonObject rateObj = obj.value("rates").toObject();
        qDebug() << "read" << date << base << obj.contains("historical");

        if (not obj.value("historical").toBool())
            Data::updateLatest(rateObj, QDateTime::currentDateTime());
        else {
            QStringList possibleList = Type::allShort();
            for (auto m_short : rateObj.keys())
                if (possibleList.contains(m_short))
                    Data::add(date, m_short, rateObj.value(m_short).toDouble());
        }
    }

//    testEqual(rateObj);
    qDebug() << "Current last " << actualRequest << " request.";
    actualRequest--;
    if (actualRequest == 0){
        Data::save();
        qDebug() << "ALL END.";
    }
    reply->deleteLater();
}

void AFHostServer::timeout()
{
    updateLatest();
    if (Data::latestDate() != QDate::currentDate().addDays(-1))
        updateAfterDate(Data::latestDate());
}

void AFHostServer::testEqual(QJsonObject rateObj)
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
