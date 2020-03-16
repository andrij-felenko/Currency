#ifndef CURRENTCURRENCYBYDATE_H
#define CURRENTCURRENCYBYDATE_H

#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QTimer>
#include <QtNetwork/QAuthenticator>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include "currencyType.h"

namespace CurrencyAF {
    class ByDate;
    class Updater;
    typedef QSharedPointer <ByDate> ByDatePtr;

    enum class RequestType {
        Latest = 0xCE8000,
        Update,
        Currency,
    };
    QString getServerKey(const RequestType type);
    QUrl getServerLink(const RequestType type);

    struct ValueSingle {
        Type::Enum m_currency;
        double m_value;
    };

    struct DateValueSingle : ValueSingle {
        ushort m_day;
    };

    struct DateValue {
        QList <DateValueSingle> m_list;
        ushort m_year;
        ushort m_month;
    };

    enum class FileType {
        ByteMonthly,
        ByteAll,
        JsonAll,
        AsAll,
    };
}

class CurrencyAF::ByDate : public QObject
{
    Q_OBJECT
public:
    explicit ByDate(QObject* parent = nullptr);

    void clear();

    static QString serverLink;
    static QString pluginName;
    static bool ignoreUpdater;

    // single
    void add(QDate date, QString m_short, double m_value);
    void add(QDate date, Type::Enum m_currency, double m_value);
    void add(QList <DateValue>& list, QDate date, Type::Enum m_currency, double m_value);
    std::optional <double> get(QString name, QDate date = QDate::currentDate());

    // currencies list
    void addCurrency(QString name);
    void addCurrency(QStringList list);
    void removeCurrency(QString name);
    void updateCurrency(const QStringList &list);

    // date
    bool foundDate(QDate date) const;
    void removeDate(QDate date);
    QDate latestDate() const;

    // load
    void load(FileType type = FileType::ByteAll);
    // save
    void save(FileType type = FileType::ByteAll);

    // latest
    QJsonObject collectLatest();
    void addLatest(QString m_short, double m_value);
    void addLatest(Type::Enum m_currency, double m_value);
    void updateLatest(QJsonObject data, QDateTime dTime = QDateTime::currentDateTime());

    // server requests
    void getServerLatest();
    void getServerCurrency(QString currency);
    void getServerCurrencies(QStringList currencies);
    void getServerUpdate(QDate last, QStringList currencies);

private:
    ByDate(ByDate const&) = delete;

    QJsonArray collectJsonCurrency(QStringList names);
    QJsonObject collectJsonCurrency(QString name);

    // FIXME remove it to by date
    bool isCurrent(QDate date, AFlib::Continent cont);
    
    QDateTime m_latestDateTime;
    QStringList m_currencyList;
    QList <DateValue> m_list;
    QList <ValueSingle> m_latest;
    
    QDir m_dir;
    QString m_fileName;
    QString date_format;

    // server private
    QTimer* m_timer;
    void timeOut();
    void onRead(QNetworkReply* reply);
    QNetworkAccessManager* m_server;

    friend class Data;
};

QDataStream &operator << (QDataStream& d, const QList <CurrencyAF::ValueSingle>& data);
QDataStream &operator >> (QDataStream& d,       QList <CurrencyAF::ValueSingle>& data);

QDataStream &operator << (QDataStream& d, const QList <CurrencyAF::DateValueSingle>& data);
QDataStream &operator >> (QDataStream& d,       QList <CurrencyAF::DateValueSingle>& data);

QDataStream &operator << (QDataStream& d, const QList <CurrencyAF::DateValue>& data);
QDataStream &operator >> (QDataStream& d,       QList <CurrencyAF::DateValue>& data);

QDataStream &operator << (QDataStream& d, const CurrencyAF::RequestType& type);
QDataStream &operator >> (QDataStream& d,       CurrencyAF::RequestType& type);

#endif // CURRENTCURRENCYBYDATE_H
