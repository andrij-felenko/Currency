#ifndef CURRENTCURRENCYDATA_H
#define CURRENTCURRENCYDATA_H

#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QObject>
#include "currencyType.h"
#include "currencyByDate.h"

namespace CurrencyAF { class Data; }

class CurrencyAF::Data
{
public:
    static void setPluginName(QString name);

//    static QJsonObject collectCurrency(QString name);
//    static QJsonArray collectCurrency(QStringList names);
//    static QJsonObject collectLatest();
//    static std::optional <double> get(QString name, QDate date = QDate::currentDate());

    static void save();
    static void saveAsJson();
    static void add(QDate date, QString m_short, double m_value);
    static void updateLatest(QJsonObject data, QDateTime dTime = QDateTime::currentDateTime());
//    static void removeDate(QDate date);

    static void addCurrency(QString name);
    static void addCurrency(QStringList list);
//    static void removeCurrency(QString name);
    static bool foundDate(QDate date);
    static QDate latestDate();
    static QDateTime latestDateTime();

    static QList <ValueSingle> getLatest();
    static QList <DateValue> getCurrency(QStringList list);
    static QList <DateValue> getUpdate(QDate date, QStringList list);

private:
    explicit Data() = default;
    Data(Data const&) = delete;
    static ByDatePtr inst();
    void operator =(Data const&) = delete;
};

#endif // CURRENTCURRENCYDATA_H
