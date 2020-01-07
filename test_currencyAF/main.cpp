#include <QtCore/QCoreApplication>
#include "lib_currencyAF/currencyByDate.h"
#include <QtCore>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
////    CurrencyAF::ByDate::instance();
//    QDateTime local(QDateTime::currentDateTime());
//    QDateTime UTC(local.toTimeSpec(Qt::UTC));
//    qDebug() << "Local time is:" << local;
//    qDebug() << "UTC time is:" << UTC;
//    int offst = local.timeZone().offsetFromUtc(QDateTime::currentDateTime());
//    qDebug() << "No difference between times:" << local.secsTo(UTC.toUTC())
//             << local.timeZone()
//             << local.timeZone().utc()
//             << local.timeZone().offsetFromUtc(QDateTime::currentDateTime())
//             << offst / 60 / 60;
//    int u = 0;
//    AFlib::Continent cont = AFlib::Continent::Oceania;
//    switch (cont) {
//    case AFlib::Continent::Oceania:       u =  8; break;
//    case AFlib::Continent::East_Asia:     u =  6; break;
//    case AFlib::Continent::West_Asia:     u =  3; break;
//    case AFlib::Continent::Europe:        u =  0; break;
//    case AFlib::Continent::Africa:        u = -1; break;
//    case AFlib::Continent::South_America: u = -5; break;
//    case AFlib::Continent::North_America: u = -7; break;
//    default:;
//    }
//    auto dd = QDateTime(QDate::currentDate(), QTime(12 + u, 0), QTimeZone::utc());
//    qDebug() << dd;
//    qDebug() << dd.secsTo(local);
//    qDebug() << bool(dd.secsTo(local) >= 0);

    CurrencyAF::ByDate date;
    return app.exec();
}
