#include "currencyData.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QJsonObject>

using namespace CurrencyAF;

void Data::save(FileType type)
{
    inst()->save(type);
}

void Data::saveAsJson()
{
    inst()->save(FileType::JsonAll);
}

void Data::add(QDate date, QString m_short, double m_value)
{
    inst()->add(date, m_short, m_value);
}

void Data::updateLatest(QJsonObject data, QDateTime dTime)
{
    inst()->updateLatest(data, dTime);
}

void Data::addCurrency(QString name)
{
    inst()->addCurrency(name);
}

void Data::addCurrency(QStringList list)
{
    inst()->addCurrency(list);
}

bool Data::foundDate(QDate date)
{
    return inst()->foundDate(date);
}

QDate Data::latestDate()
{
    return inst()->latestDate();
}

QDateTime Data::latestDateTime()
{
    return inst()->m_latestDateTime;
}

QList<DateValue> Data::getCurrency(QStringList list)
{
    QList <DateValue> c_list;
    for (const auto &it : inst()->m_list)
        for (auto sub : it.m_list)
            for (const auto &currency : list)
                if (sub.m_currency == Type::toEnum(currency))
                    inst()->add(c_list, QDate(it.m_year, it.m_month, sub.m_day),
                                sub.m_currency, sub.m_value);
    return c_list;
}

QList<DateValue> Data::getUpdate(QDate date, QStringList list)
{
    QList <DateValue> c_list;
    for (const auto &it : inst()->m_list)
        if (QDate(it.m_year, it.m_month, 1) > date)
            for (auto sub : it.m_list)
                for (const auto &currency : list)
                    if (sub.m_currency == Type::toEnum(currency))
                        if (QDate(it.m_year, it.m_month, sub.m_day) > date)
                            inst()->add(c_list, QDate(it.m_year, it.m_month, sub.m_day),
                                        sub.m_currency, sub.m_value);
    return c_list;
}

QList<ValueSingle> Data::getLatest()
{
    return inst()->m_latest;
}

ByDatePtr CurrencyAF::Data::inst()
{
    static ByDatePtr m_ptr;
    if (m_ptr.isNull())
        m_ptr = ByDatePtr::create(qApp);

    return m_ptr;
}
