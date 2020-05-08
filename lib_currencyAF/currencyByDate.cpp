#include "currencyByDate.h"
#include <AFbase/AfDir>
#include <AFbase/AfFile>
#include <AFbase/AfEnum>
#include <AFbase/AfServerSettings>

#include <QtCore/QDebug>
#include <QtCore/QDataStream>
#include <QtCore/QDateTime>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QTimeZone>

using namespace CurrencyAF;
bool ByDate::ignoreUpdater = false;
QString ByDate::pluginName = "Currency";
QString ByDate::serverLink = QString("127.0.0.1:55555");

uint _hour = 60 * 60 * 1000;
QString _all_data = "all_file";
QString _by_month = "by_month";
QString _currency = "currency";
QString _all_currency = "all_currency";
QString _latest = "latest";

ByDate::ByDate(QObject* parent) : QObject(parent),
    date_format("yyyy-MM-dd")
{
    clear();

    if (ignoreUpdater)
        load(FileType::ByteMonthly);
    if (m_list.isEmpty() || not ignoreUpdater)
        load();
    save(FileType::ByteMonthly);
    save(FileType::ByteAll);
//    m_latestDateTime = QDateTime::currentDateTime();// (QDate(1998, 1, 1), QTime(0, 0));

    m_server = new QNetworkAccessManager(this);
    QObject::connect(m_server, &QNetworkAccessManager::finished, this, &ByDate::onRead);

    if (m_currencyList.isEmpty()){
        addCurrency(Type::toShort(Type::Enum::Euro));
        addCurrency(Type::toShort(Type::Enum::US_Dollar));
        addCurrency(Type::toShort(Type::Enum::Japanese_Yen));
        addCurrency(Type::toShort(Type::Enum::British_Pound));
    }

    if (not ignoreUpdater) {
        m_timer = new QTimer(this);
        m_timer->setInterval(_hour);

        // if last update was more than 1 hour before, send it now
        if (m_latestDateTime.secsTo(QDateTime::currentDateTime()) >= _hour)
            getServerLatest();
        if (latestDate() != QDate::currentDate().addDays(-1))
            getServerUpdate(latestDate(), m_currencyList);

        QDateTime nearDTime = QDateTime::currentDateTime().addMSecs(_hour);
        nearDTime.time().setHMS(nearDTime.time().hour(), 0, 15);
        QTimer::singleShot(QDateTime::currentDateTime().secsTo(nearDTime), m_timer, static_cast<void (QTimer::*)()>(&QTimer::start));
    }

    QTimer::singleShot(10000, [=](){ save(); });
}

void ByDate::clear()
{
    m_list.clear();
    m_latest.clear();
    m_currencyList.clear();
    m_latestDateTime = QDateTime(QDate(1998, 1, 1), QTime(0, 0));
}

// single
void ByDate::add(QDate date, QString m_short, double m_value)
{
    add(date, Type::toEnum(m_short), m_value);
}

void ByDate::add(QDate date, Type::Enum m_currency, double m_value)
{
    add(m_list, date, m_currency, m_value);
}

void ByDate::add(QList<DateValue> &list, QDate date, Type::Enum m_currency, double m_value)
{
    DateValueSingle tmp;
    tmp.m_day = ushort(date.day());
    tmp.m_value = m_value;
    tmp.m_currency = m_currency;

    for (auto it  = list.begin(); it != list.end(); ++it)
        if (it->m_year == date.year())
            if (it->m_month == date.month()){
                for (auto sub : it->m_list)
                    if (sub.m_day == date.day())
                        if (sub.m_currency == m_currency)
                            return;
                it->m_list.push_back(tmp);
                return;
            }

    DateValue newDate;
    newDate.m_year = ushort(date.year());
    newDate.m_month = ushort(date.month());
    newDate.m_list.push_back(tmp);

    list.push_back(newDate);
}

std::optional <double> ByDate::get(QString name, QDate date)
{
    for (auto it : m_list){
        if (it.m_year != date.year())
            continue;

        if (it.m_month != date.month())
            continue;

        for (auto sub : it.m_list){
            if (sub.m_day != date.day())
                continue;

            if (sub.m_currency != Type::toEnum(name))
                continue;

            return sub.m_value;
        }
    }
    return std::nullopt;
}

// currencies list
void ByDate::addCurrency(QString name)
{
    auto currency = Type::toShort(name);
    if (not Type::allShort().contains(currency)){
        qDebug() << QString("Currency %1 not found").arg(name);
        return;
    }

    m_currencyList.push_back(currency);
    m_currencyList.removeDuplicates();
    getServerCurrency(currency);
}

void ByDate::addCurrency(QStringList list)
{
    for (auto it : list)
        addCurrency(it);
}

void ByDate::removeCurrency(QString name)
{
    if (not m_currencyList.contains(name)){
        qDebug() << QString("%1 not found in currency list.").arg(name);
        return;
    }

    m_currencyList.removeOne(name);
    for (auto it : m_list)
        it.m_list.erase(std::remove_if(it.m_list.begin(),
                                       it.m_list.end(),
                                       [name](DateValueSingle s){
                                           return s.m_currency == Type::toEnum(name);
                                       }),
                        it.m_list.end());
    save();
}

void ByDate::updateCurrency(const QStringList& list)
{
    for (auto it : m_currencyList)
        if (not list.contains(it))
            removeCurrency(it);

    for (auto it : list)
        if (not m_currencyList.contains(it))
            addCurrency(it);
}

// date
bool ByDate::foundDate(QDate date) const
{
    for (auto it : m_list)
        if (it.m_year == date.year())
            if (it.m_month == date.month())
                for (auto sub : it.m_list)
                    if (sub.m_day == date.day())
                        return true;

    qDebug() << "Not found date" << date;
    return false;
}

void ByDate::removeDate(QDate date)
{
    m_list.erase(std::remove_if(m_list.begin(), m_list.end(),
                                [date](DateValue value){
                                    return value.m_year == date.year() && value.m_month == date.month();
                                }),
            m_list.end());
}

QDate ByDate::latestDate() const
{
    QDate last = QDate::currentDate().addDays(-1);
    while (last != QDate(1999, 1, 1)){
        if (foundDate(last))
            break;
        last = last.addDays(-1);
    }
    return last;
}

// load
void ByDate::load(FileType type)
{
    qDebug() << "Begin load:" << QDateTime::currentDateTime();
    switch (type){
    case FileType::ByteAll: {
        AFfile file(QStringList { _currency, _all_data, _all_currency }, AFlib::FileType::Data);
        qDebug() << file.dPath() << file.dType();
        if (file.openRead())
            file >> m_latestDateTime >> m_latest >> m_list >> m_currencyList;
        break;
    }
    case FileType::JsonAll: {
        AFfile file(QStringList { _currency, _all_data, _all_currency }, AFlib::FileType::Json);
        if (file.openRead()) {
            QJsonObject byte = QJsonDocument::fromJson(file.readAll()).object();
            QJsonObject latest = byte.value("latest").toObject();
            updateLatest(latest.value("currencies").toObject(),
                         QDateTime(QDate::fromString(latest.value("date").toString(), date_format),
                                   QTime::fromString(latest.value("time").toString(), "hh:mm:ss")));
            m_currencyList = byte.value("list").toVariant().toStringList();
            QJsonArray data = byte.value("data").toArray();
            int i = 0;
            int len = data.count();
            for (auto it = data.begin(); it != data.end(); ++it){
                qDebug() << i << "/" << len << "   " << int(i*100 / len) << "%";
                i++;
                auto current = it->toObject();
                QString currency = current.value("currency").toString();
                QJsonObject subObj = current.value("data").toObject();
                for (auto sub = subObj.begin(); sub != subObj.end(); ++sub)
                    add(QDate::fromString(sub.key(), date_format), currency, sub.value().toDouble());
            }
            m_currencyList.removeDuplicates();
        }
        break;
    }
    case FileType::ByteMonthly: {
        QStringList fileList = AFfile::getFullDir(QStringList { _currency, _by_month }, false).entryList({"*.afd"});
        for (auto it : fileList){
            QString fileName = it.left(it.length() - 4);
            AFfile file(QStringList { _currency, _by_month, fileName }, AFlib::FileType::Data);
            if (not file.openRead())
                continue;

            // add to m_list
            QList <DateValueSingle> tmp;
            ushort date;
            file >> tmp >> date;
            DateValue single;
            qDebug() << "date load:" << date;
            single.m_list = tmp;
            single.m_year = date / 12;
            single.m_month = date % 12 + 1;
            m_list.push_back(single);
        }
        load(FileType::Latest);
        break;
    }
    case FileType::Latest: {
        AFfile file(QStringList { _currency, _latest }, AFlib::FileType::Data);
        if (not file.openRead())
            break;

        QList <DateValue> tmp;
        file >> tmp >> m_latest >> m_currencyList;
        m_list += tmp;
        break;
    }
    case FileType::AsAll: {
        load(FileType::ByteAll);
        load(FileType::JsonAll);
        load(FileType::ByteMonthly);
    }
    }
    qDebug() << "End load:" << QDateTime::currentDateTime();
}

// save
void ByDate::save(FileType type)
{
    qDebug() << "Begin save:" << QDateTime::currentDateTime();
    switch (type) {
    case FileType::ByteAll: {
        AFfile file(QStringList { _currency, _all_data, _all_currency }, AFlib::FileType::Data);
        if (file.openWrite())
            file << m_latestDateTime << m_latest << m_list << m_currencyList;
        break;
    }
    case FileType::JsonAll: {
        qDebug() << "Json save start";
        QTime time = QTime::currentTime();
        AFfile file(QStringList { _currency, _all_data, _all_currency }, AFlib::FileType::Json);
        if (file.openWrite()){
            QJsonObject obj;
            obj.insert("latest", collectLatest());
            obj.insert("data", collectJsonCurrency(m_currencyList));
            qDebug() << "data.list" << obj.value("data").toArray().count()
                     << m_list.length() << m_currencyList.length();
            obj.insert("list", QJsonArray::fromStringList(m_currencyList));
            file.writeAll(QJsonDocument(obj).toJson());
        }

        qDebug() << "Time to JSON save:" << time.msecsTo(QTime::currentTime()) << "msec";
        break;
    }
    case FileType::ByteMonthly: {
        QTime time = QTime::currentTime();
        // archive list
        QList <DateValue> m_latestList;
        for (auto it : m_list){
            if (it.m_month == QDate::currentDate().month())
                if (it.m_year == QDate::currentDate().year())
                    continue;

            AFfile file(QStringList { _currency, _by_month, it.dateStr() }, AFlib::FileType::Data);
            if (file.openWrite()){
                qDebug() << "date" << it.m_year * 12 + it.m_month - 1;
                file << it.m_list << ushort(it.m_year * 12 + it.m_month - 1);
            }
        }
        save(FileType::Latest);
        qDebug() << "Time to save:" << time.msecsTo(QTime::currentTime()) << "msec";
        break;
    }
    case FileType::Latest: {
        QList <DateValue> m_latestList;
        for (auto it : m_list){
            if (it.m_month == QDate::currentDate().month())
                if (it.m_year == QDate::currentDate().year()){
                    m_latestList.push_back(it);
                    continue;
                }
        }
        AFfile file(QStringList { _currency, _latest }, AFlib::FileType::Data);
        if (file.openWrite())
            file << m_latestList << m_latest << m_currencyList;
        break;
    }
    case FileType::AsAll: {
        save(FileType::ByteAll);
        save(FileType::JsonAll);
        save(FileType::ByteMonthly);
    }
    }

    qDebug() << "End save:" << QDateTime::currentDateTime();
}

// latest
QJsonObject ByDate::collectLatest()
{
    QJsonObject obj;
    obj.insert("date", m_latestDateTime.date().toString(date_format));
    obj.insert("time", m_latestDateTime.time().toString("hh:mm:ss"));
    QJsonObject currencies;
    for (auto it : m_latest)
        currencies.insert(Type::toShort(it.m_currency), it.m_value);

    obj.insert("currencies", currencies);
    return obj;
}

void ByDate::addLatest(QString m_short, double m_value)
{
    addLatest(Type::toEnum(m_short), m_value);
}

void ByDate::addLatest(Type::Enum m_currency, double m_value)
{
    for (auto it = m_latest.begin(); it != m_latest.end(); ++it)
        if (it->m_currency == m_currency){
            it->m_value = m_value;
            return;
        }

    ValueSingle tmp;
    tmp.m_currency = m_currency;
    tmp.m_value = m_value;
    m_latest.push_back(tmp);
}

void ByDate::updateLatest(QJsonObject data, QDateTime dTime)
{
    m_latest.clear();
    for (auto it = data.begin(); it != data.end(); ++it)
        addLatest(it.key(), it.value().toDouble());
    m_latestDateTime = dTime;
}

QJsonArray ByDate::collectJsonCurrency(QStringList names)
{
    QJsonArray array;
    for (auto it : names)
        array.push_back(collectJsonCurrency(it));
    return array;
}

QJsonObject ByDate::collectJsonCurrency(QString name)
{
    QJsonObject obj;
    if (name.length() != 3)
        name = Type::toShort(name);
    obj.insert("currency", name);

    QJsonObject data;
    for (auto it : m_list)
        for (auto sub : it.m_list)
            if (sub.m_currency == Type::toEnum(name))
                data.insert(QDate(it.m_year, it.m_month, sub.m_day).toString(date_format), sub.m_value);

    obj.insert("data", data);

    return obj;
}

// FIXME remove it to by date
bool ByDate::isCurrent(QDate date, AFlib::Continent cont)
{
    Q_UNUSED(date)
    int shift = 0;
    switch (cont) {
    case AFlib::Continent::Oceania:       shift =  8; break;
    case AFlib::Continent::East_Asia:     shift =  6; break;
    case AFlib::Continent::West_Asia:     shift =  3; break;
    case AFlib::Continent::Europe:        shift =  0; break;
    case AFlib::Continent::Africa:        shift = -1; break;
    case AFlib::Continent::South_America: shift = -5; break;
    case AFlib::Continent::North_America: shift = -7; break;
    default:;
    }

    auto needTime = QDateTime(QDate::currentDate(), QTime(11 + shift, 0),
                              QTimeZone::utc());
    return needTime.secsTo(QDateTime::currentDateTime()) >= 0;
}

// Server --------------------------------------------------------------
void ByDate::getServerLatest()
{
    if (ignoreUpdater)
        return;

    QNetworkRequest request;
    request.setUrl(getServerLink(RequestType::Latest));
    m_server->get(request);
}

void ByDate::getServerCurrency(QString currency)
{
    if (ignoreUpdater)
        return;

    getServerCurrencies(QStringList(currency));
}

void ByDate::getServerCurrencies(QStringList currencies)
{
    if (ignoreUpdater)
        return;

    QByteArray data;
    QDataStream dataStreamWrite(&data, QIODevice::WriteOnly);
    dataStreamWrite << currencies;

    QNetworkRequest request;
    request.setRawHeader("Content-Type", "octet-stream;");
    request.setRawHeader("Content-Length", QByteArray::number(data.size()));

    request.setUrl(getServerLink(RequestType::Currency));
    m_server->post(request, data);
}

void ByDate::getServerUpdate(QDate last, QStringList currencies)
{
    if (ignoreUpdater)
        return;

    QByteArray data;
    QDataStream dataStreamWrite(&data, QIODevice::WriteOnly);
    dataStreamWrite << last << currencies;

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream;");
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    request.setUrl(getServerLink(RequestType::Update));
    m_server->post(request, data);
    qDebug() << data;
}

void ByDate::timeOut()
{
    getServerLatest();
    if (latestDate() != QDate::currentDate().addDays(-1))
        getServerUpdate(latestDate(), m_currencyList);
}

void ByDate::onRead(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError){
        qDebug() << "Request has error" << reply->errorString() << reply->url();
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    QDataStream stream(data);
    RequestType type;
    stream >> type;

    switch (type){
    case RequestType::Latest: {
        stream >> m_latestDateTime >> m_latest;
        qDebug() << "read Latest:" << m_latestDateTime;
        break;
    }
    case RequestType::Update:
    case RequestType::Currency: {
        QList <DateValue> list;
        stream >> list;
        for (auto it : list)
            for (auto sub : it.m_list)
                add(QDate(it.m_year, it.m_month, sub.m_day), sub.m_currency, sub.m_value);
        break;
    }
    }

    save();
    reply->deleteLater();
}
// end server ----------------------------------------------------------

QDataStream &operator <<(QDataStream &d, const QList<ValueSingle> &data)
{
    d << data.length();
    for (auto it : data)
        d << ushort(it.m_currency) << it.m_value;
    return d;
}

QDataStream &operator >>(QDataStream &d, QList<ValueSingle> &data)
{
    int count;
    ushort m_currency;
    CurrencyAF::ValueSingle single;

    d >> count;
    for (int i = 0; i < count; i++){
        d >> m_currency >> single.m_value;
        single.m_currency = static_cast <CurrencyAF::Type::Enum> (m_currency);
        data.push_back(single);
    }
    return d;
}

QDataStream &operator <<(QDataStream &d, const QList<DateValueSingle> &data)
{
    d << data.length();
    for (auto it : data)
        d << it.m_day << ushort(it.m_currency) << it.m_value;
    return d;
}

QDataStream &operator >>(QDataStream &d, QList<DateValueSingle> &data)
{
    int count;
    ushort m_currency;
    CurrencyAF::DateValueSingle single;

    d >> count;
    for (int i = 0; i < count; i++){
        d >> single.m_day >> m_currency >> single.m_value;
        single.m_currency = static_cast <Type::Enum> (m_currency);
        data.push_back(single);
    }
    return d;
}

QDataStream &operator <<(QDataStream &d, const QList<DateValue> &data)
{
    d << data.length();
    for (auto it : data)
        d << it.m_list << ushort(it.m_month - 1 + 12 * it.m_year);
    return d;
}

QDataStream &operator >>(QDataStream &d, QList<DateValue> &data)
{
    int count;
    ushort p_date;
    CurrencyAF::DateValue single;

    d >> count;
    for (int i = 0; i < count; ++i){
        d >> single.m_list >> p_date;
        single.m_year = p_date / 12;
        single.m_month = p_date % 12 + 1;
        data.push_back(single);

        single.m_list.clear();
    }
    return d;
}

QDataStream &operator << (QDataStream &d, const RequestType &type)
{
    return d << static_cast <ushort> (type);
}

QDataStream &operator >> (QDataStream &d, RequestType &type)
{
    ushort retShort;
    d >> retShort;
    type = static_cast <RequestType> (retShort);
    return d;
}

QUrl CurrencyAF::getServerLink(const RequestType type)
{
    QString key = afServerSettings()->getUrl(ByDate::pluginName, ByDate::serverLink);
    key.append(getServerKey(type));
    return QUrl::fromUserInput(key);
}

QString CurrencyAF::getServerKey(const RequestType type)
{
    QString key = "/";
    switch (type) {
    case RequestType::Latest:   key += "latest";
    case RequestType::Update:   key += "update";
    case RequestType::Currency: key += "currency";
    }
    return key;
}


QString DateValue::dateStr() const
{
    QString ret = QString::number(m_year);
    ret += "_";
    if (m_month < 10)
        ret += "0";
    ret += QString::number(m_month);
    return ret;
}
