#include <QtCore/QCoreApplication>
#include "currencyByDate.h"
#include <QtCore>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
//    CurrencyAF::ByDate::instance();
    QDateTime local(QDateTime::currentDateTime());
    QDateTime UTC(local.toTimeSpec(Qt::UTC));
    qDebug() << "Local time is:" << local;
    qDebug() << "UTC time is:" << UTC;
    int offst = local.timeZone().offsetFromUtc(QDateTime::currentDateTime());
    qDebug() << "No difference between times:" << local.secsTo(UTC.toUTC())
             << local.timeZone()
             << local.timeZone().utc()
             << local.timeZone().offsetFromUtc(QDateTime::currentDateTime())
             << offst / 60 / 60;
    int u = 0;
    AFcontinent cont = AFcontinent::Oceania;
    switch (cont) {
    case AFcontinent::Oceania:       u =  8; break;
    case AFcontinent::East_Asia:     u =  6; break;
    case AFcontinent::West_Asia:     u =  3; break;
    case AFcontinent::Europe:        u =  0; break;
    case AFcontinent::Africa:        u = -1; break;
    case AFcontinent::South_America: u = -5; break;
    case AFcontinent::North_America: u = -7; break;
    default:;
    }
    auto dd = QDateTime(QDate::currentDate(), QTime(12 + u, 0), QTimeZone::utc());
    qDebug() << dd;
    qDebug() << dd.secsTo(local);
    qDebug() << bool(dd.secsTo(local) >= 0);
    return app.exec();
}
