#ifndef CURRENCYUPDATER_H
#define CURRENCYUPDATER_H

#include <QtNetwork/QAuthenticator>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include "currencyType.h"

namespace CurrencyAF {
class Updater : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit Updater(QObject *parent = nullptr);
    void request();
    void request(QDate date);

signals:
    void result(QDate, QString, double);
    void finish();

public slots:
    //

private:
    void onRead(QNetworkReply* reply);
    QString m_url;
    QString m_accesKey;
    QString m_latest;

    void testEqual(QJsonObject rateObj);
};
}

#endif // CURRENCYUPDATER_H
