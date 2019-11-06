#ifndef CURRENTCURRENCYBYDATE_H
#define CURRENTCURRENCYBYDATE_H

#include <QtCore/QDir>
#include <QtCore/QObject>
#include "currencyUpdater.h"

namespace CurrencyAF {
class ByDate : public QObject
{
    Q_OBJECT
public:
    static ByDate& instance(QDir dataDir = QDir::current(), QObject *parent = nullptr);

signals:

public slots:

private:
    ByDate(QDir dataDir = QDir::current(), QObject *parent = nullptr);
    ByDate(ByDate const&) = delete;
    void operator =(ByDate const&) = delete;
    Updater* m_updater;

    void load();
    bool isCurrent(QDate date, AFcontinent cont);

    struct DateValue {
        QDate m_date;
        QString m_short;
        double m_value;
    };
    QList <DateValue> m_list;
    QString m_fileName;
    QDir m_dir;

private slots:
    void save();
    void add(QDate date, QString m_short, double m_value);
};
}

#endif // CURRENTCURRENCYBYDATE_H
