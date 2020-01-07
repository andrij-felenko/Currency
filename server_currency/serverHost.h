#ifndef AF_CURRENCY_HOST_SERVER
#define AF_CURRENCY_HOST_SERVER

#include <QtCore/QTimer>
#include <QtNetwork/QAuthenticator>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

class AFHostServer : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit AFHostServer(QObject* parent = nullptr);

    void updateLatest();
    void updateDate(const QDate& date);
    void updateAfterDate(const QDate& date);

protected:
    void onRead(QNetworkReply* reply);
    void timeout();
    QString m_accessKey;
    QString m_url;
    QTimer* m_timer;
    int actualRequest;

    void testEqual(QJsonObject rateObj);
};

#endif // AF_CURRENCY_HOST_SERVER
