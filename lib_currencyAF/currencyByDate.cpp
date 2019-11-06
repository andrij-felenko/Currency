#include "currencyByDate.h"
#include <QtCore/QDateTime>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QtCore/QFile>
#include <QtCore/QTimeZone>

using namespace CurrencyAF;

ByDate::ByDate(QDir dataDir, QObject *parent) : QObject(parent),
      m_fileName("currencyByDate.json"), m_dir(dataDir)
{
    load();
    m_updater = new Updater(this);

    QObject::connect(m_updater, &Updater::result, this, &ByDate::add);
    QObject::connect(m_updater, &Updater::finish, this, &ByDate::save);

    m_updater->request();
}

void ByDate::load()
{
    QFile file(m_dir.absoluteFilePath(m_fileName));
    if (file.open(QIODevice::ReadOnly)){
        QJsonObject byte = QJsonDocument::fromJson(file.readAll()).object();
        QTime time = QTime::fromString(byte.value("time").toString(), "hh");
        QJsonArray data = byte.value("data").toArray();
        for (auto it : data){
            QDate date = QDate::fromString(it.toObject().value("date").toString(), "yyyy-MM-dd");
            QJsonObject currencies = it.toObject().value("currencies").toObject();
            for (auto currency : currencies.keys())
                add(date, currency, currencies.value(currency).toDouble());
        }
        file.close();
    }
    else
        qDebug() << "File can`t read" << m_fileName;
}

bool ByDate::isCurrent(QDate date, AFcontinent cont)
{
    Q_UNUSED(date)
    int shift = 0;
    switch (cont) {
    case AFcontinent::Oceania:       shift =  8; break;
    case AFcontinent::East_Asia:     shift =  6; break;
    case AFcontinent::West_Asia:     shift =  3; break;
    case AFcontinent::Europe:        shift =  0; break;
    case AFcontinent::Africa:        shift = -1; break;
    case AFcontinent::South_America: shift = -5; break;
    case AFcontinent::North_America: shift = -7; break;
    default:;
    }

    auto needTime = QDateTime(QDate::currentDate(), QTime(11 + shift, 0),
                              QTimeZone::utc());
    return needTime.secsTo(QDateTime::currentDateTime()) >= 0;
}

void ByDate::save()
{
    QFile file(m_dir.absoluteFilePath(m_fileName));
    if (file.open(QIODevice::Truncate | QIODevice::ReadWrite)){
        QList <QDate> dateList;
        for (auto it : m_list)
            if (!dateList.contains(it.m_date))
                dateList.push_back(it.m_date);
        QJsonArray array;
        for (auto date : dateList){
            QJsonObject dateObj;
            for (auto it : m_list)
                if (it.m_date == date)
                    dateObj.insert(it.m_short, it.m_value);
            QJsonObject subArray;
            subArray.insert("date", date.toString("yyyy-MM-dd"));
            subArray.insert("currencies", dateObj);
            array.push_back(subArray);
        }
        QJsonObject obj;
        obj.insert("data", array);
        obj.insert("time", QTime::currentTime().toString("hh"));
        file.write(QJsonDocument(obj).toJson());
        file.close();
    }
    else
        qDebug() << "File can`t create" << m_fileName;
}

void ByDate::add(QDate date, QString m_short, double m_value)
{                    
    DateValue tmp;
    tmp.m_date = date;
    tmp.m_short = m_short;
    tmp.m_value = m_value;
    m_list.push_back(tmp);
}

ByDate& ByDate::instance(QDir dataDir, QObject *parent)
{
    static ByDate ins(dataDir, parent);
    return ins;
}
