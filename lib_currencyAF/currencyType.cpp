#include "currencyType.h"

using namespace CurrencyAF;
// GET http://data.fixer.io/api/latest?access_key=33e9a96e437daadfc69cc9495422bb0b
// https://fixer.io/dashboard

Type &Type::instance()
{
    static Type ins;
    return ins;
}

QString Type::toShort(Enum type)
{
    for (auto it : instance().m_list)
        if (it.m_enum == type)
            return it.m_short;
    return "NaN";
}

QString Type::toShort(QString full)
{
    for (auto it : instance().m_list)
        if (it.m_full == full)
            return it.m_short;
    return "NaN";
}

QString Type::toFull(Type::Enum type)
{
    for (auto it : instance().m_list)
        if (it.m_enum == type)
            return it.m_full;
    return "NaN";
}

QString Type::toFull(QString m_short)
{
    for (auto it : instance().m_list)
        if (it.m_short == m_short)
            return it.m_full;
    return "NaN";
}

Type::Enum Type::fromShort(QString currency)
{
    for (auto it : instance().m_list)
        if (it.m_short == currency)
            return it.m_enum;
    return Enum::None;
}

AFlib::Enum::Continent Type::continent(QString name)
{
    for (auto it : instance().m_list)
        if (it.m_full == name || it.m_short == name)
            return it.m_continent;
    return AFcontinent::None;
}

AFlib::Enum::Continent Type::continent(Type::Enum type)
{
    for (auto it : instance().m_list)
        if (it.m_enum == type)
            return it.m_continent;
    return AFcontinent::None;
}

QStringList Type::allShort()
{
    QStringList list;
    for (auto it : instance().m_list)
        list.push_back(it.m_short);
    return list;
}

void Type::addAfrica(){
    AFcontinent cont = AFcontinent::Africa;
    add(Enum::Algerian_Dinar,       "Algerian Dinar",       "DZD", cont);
    add(Enum::Angolan_Kwanza,       "Angolan Kwanza",       "AOA", cont);
    add(Enum::Basotho_Loti,         "Basotho Loti",         "LSL", cont);
    add(Enum::Botswana_Pula,        "Botswana Pula",        "BWP", cont);
    add(Enum::Burundian_Franc,      "Burundian Franc",      "BIF", cont);
    add(Enum::Cape_Verdean_Escudo,  "Cape Verdean Escudo",  "CVE", cont);
    add(Enum::Central_African,      "Central African",      "XAF", cont);
    add(Enum::CFA_Franc,            "CFA Franc",            "XOF", cont);
    add(Enum::Comorian_Franc,       "Comorian Franc",       "KMF", cont);
    add(Enum::Congolese_Franc,      "Congolese Franc",      "CDF", cont);
    add(Enum::Djiboutian_Franc,     "Djiboutian Franc",     "DJF", cont);
    add(Enum::Egyptian_Pound,       "Egyptian Pound",       "EGP", cont);
    add(Enum::Eritrean_Nakfa,       "Eritrean Nakfa",       "ERN", cont);
    add(Enum::Ethiopian_Birr,       "Ethiopian Birr",       "ETB", cont);
    add(Enum::Gambian_Dalasi,       "Gambian Dalasi",       "GMD", cont);
    add(Enum::Ghanaian_Cedi,        "Ghanaian Cedi",        "GHS", cont);
    add(Enum::Guinean_Franc,        "Guinean Franc",        "GNF", cont);
    add(Enum::Kenyan_Shilling,      "Kenyan Shilling",      "KES", cont);
    add(Enum::Liberian_Dollar,      "Liberian Dollar",      "LRD", cont);
    add(Enum::Libyan_Dinar,         "Libyan Dinar",         "LYD", cont);
    add(Enum::Malagasy_Ariary,      "Malagasy Ariary",      "MGA", cont);
    add(Enum::Malawian_Kwacha,      "Malawian Kwacha",      "MWK", cont);
    add(Enum::Mauritanian_Ouguiya,  "Mauritanian Ouguiya",  "MRO", cont);
    add(Enum::Mauritian_Rupee,      "Mauritian Rupee",      "MUR", cont);
    add(Enum::Moroccan_Dirham,      "Moroccan Dirham",      "MAD", cont);
    add(Enum::Mozambican_Metical,   "Mozambican Metical",   "MZN", cont);
    add(Enum::Nigerian_Naira ,      "Nigerian Naira",       "NGN", cont);
    add(Enum::Namibian_Dollar,      "Namibian Dollar",      "NAD", cont);
    add(Enum::Rwandan_Franc,        "Rwandan Franc",        "RWF", cont);
    add(Enum::Saint_Helenian_Pound, "Saint Helenian Pound", "SHP", cont);
    add(Enum::Sao_Tomean_Dobra,     "Sao Tomean Dobra",     "STD", cont);
    add(Enum::Seychellois_Rupee,    "Seychellois Rupee",    "SCR", cont);
    add(Enum::Sierra_Leonean_Leone, "Sierra Leonean Leone", "SLL", cont);
    add(Enum::Somali_Shilling,      "Somali Shilling",      "SOS", cont);
    add(Enum::South_African_Rand,   "South African Rand",   "ZAR", cont);
    add(Enum::Sudanese_Pound,       "Sudanese Pound",       "SDG", cont);
    add(Enum::Swazi_Lilangeni,      "Swazi Lilangeni",      "SZL", cont);
    add(Enum::Tanzanian_Shilling,   "Tanzanian Shilling",   "TZS", cont);
    add(Enum::Tunisian_Dinar,       "Tunisian Dinar",       "TND", cont);
    add(Enum::Ugandan_Shilling,     "Ugandan Shilling",     "UGX", cont);
    add(Enum::Zambian_Kwacha,       "Zambian Kwacha",       "ZMW", cont);
    add(Enum::Zimbabwean_Dollar,    "Zimbabwean Dollar",    "ZWL", cont);
}

void Type::addEastAsia(){
    AFcontinent cont = AFcontinent::East_Asia;
    add(Enum::Bangladeshi_Taka,  "Bangladeshi Taka",   "BDT", cont);
    add(Enum::Bhutanese_Ngultrum,"Bhutanese Ngultrum", "BTN", cont);
    add(Enum::Bruneian_Dollar,   "Bruneian Dollar",    "BND", cont);
    add(Enum::Burmese_Kyat,      "Burmese Kyat",       "MMK", cont);
    add(Enum::Cambodian_Riel,    "Cambodian Riel",     "KHR", cont);
    add(Enum::Chinese_Yuan,      "Chinese Yuan",       "CNY", cont);
    add(Enum::Hong_Kong_Dollar,  "Hong Kong Dollar",   "HKD", cont);
    add(Enum::Japanese_Yen,      "Japanese Yen",       "JPY", cont);
    add(Enum::Indian_Rupee,      "Indian Rupee",       "INR", cont);
    add(Enum::Indonesian_Rupiah, "Indonesian Rupiah",  "IDR", cont);
    add(Enum::Kyrgyzstani_Som,   "Kyrgyzstani Som",    "KGS", cont);
    add(Enum::Lao_Kip,           "Lao Kip",            "LAK", cont);
    add(Enum::Macau_Pataca,      "Macau Pataca",       "MOP", cont);
    add(Enum::Malaysian_Ringgit, "Malaysian Ringgit",  "MYR", cont);
    add(Enum::Mongolian_Tughrik, "Mongolian Tughrik",  "MNT", cont);
    add(Enum::Nepalese_Rupee,    "Nepalese Rupee",     "NPR", cont);
    add(Enum::North_Korean_Won,  "North Korean Won",   "KPW", cont);
    add(Enum::Philippine_Piso,   "Philippine Piso",    "PHP", cont);
    add(Enum::Singapore_Dollar,  "Singapore Dollar",   "SGD", cont);
    add(Enum::South_Korean_Won,  "South Korean Won",   "KRW", cont);
    add(Enum::Sri_Lankan_Rupee,  "Sri Lankan Rupee",   "LKR", cont);
    add(Enum::Taiwan_New_Dollar, "Taiwan New Dollar",  "TWD", cont);
    add(Enum::Thai_Baht,         "Thai Baht",          "THB", cont);
    add(Enum::Vietnamese_Dong,   "Vietnamese Dong",    "VND", cont);
}

void Type::addWestAsia(){
    AFcontinent cont = AFcontinent::West_Asia;
    add(Enum::Afghan_Afghani,      "Afghan Afghani",      "AFN", cont);
    add(Enum::Armenian_Dram,       "Armenian Dram",       "AMD", cont);
    add(Enum::Azerbaijan_Manat,    "Azerbaijan Manat",    "AZN", cont);
    add(Enum::Bahraini_Dinar,      "Bahraini Dinar",      "BHD", cont);
    add(Enum::Emirati_Dirham,      "Emirati Dirham",      "AED", cont);
    add(Enum::Georgian_Lari,       "Georgian Lari",       "GEL", cont);
    add(Enum::Jordanian_Dinar,     "Jordanian Dinar",     "JOD", cont);
    add(Enum::Iraqi_Dinar,         "Iraqi Dinar",         "IQD", cont);
    add(Enum::Iranian_Rial,        "Iranian Rial",        "IRR", cont);
    add(Enum::Israeli_Shekel,      "Israeli Shekel",      "ILS", cont);
    add(Enum::Kazakhstani_Tenge,   "Kazakhstani Tenge",   "KZT", cont);
    add(Enum::Kuwaiti_Dinar,       "Kuwaiti Dinar",       "KWD", cont);
    add(Enum::Lebanese_Pound,      "Lebanese Pound",      "LBP", cont);
    add(Enum::Maldivian_Rufiyaa,   "Maldivian Rufiyaa",   "MVR", cont);
    add(Enum::Omani_Rial,          "Omani Rial",          "OMR", cont);
    add(Enum::Pakistani_Rupee,     "Pakistani Rupee",     "PKR", cont);
    add(Enum::Qatari_Riyal,        "Qatari Riyal",        "QAR", cont);
    add(Enum::Russian_Ruble,       "Russian Ruble",       "RUB", cont);
    add(Enum::Saudi_Arabian_Riyal, "Saudi Arabian Riyal", "SAR", cont);
    add(Enum::Syrian_Pound,        "Syrian Pound",        "SYP", cont);
    add(Enum::Tajikistani_Somoni,  "Tajikistani Somoni",  "TJS", cont);
    add(Enum::Turkish_Lira,        "Turkish Lira",        "TRY", cont);
    add(Enum::Turkmenistani_Manat, "Turkmenistani Manat", "TMT", cont);
    add(Enum::Uzbekistani_Som,     "Uzbekistani Som",     "UZS", cont);
    add(Enum::Yemeni_Rial,         "Yemeni Rial",         "YER", cont);
}

void Type::addEurope()
{
    AFcontinent cont = AFcontinent::Europe;
    add(Enum::Albanian_Lek,              "Albanian Lek",              "ALL", cont);
    add(Enum::Belarusian_Ruble,          "Belarusian Ruble",          "BYR", cont);
    add(Enum::Bosnian_Convertible_Marka, "Bosnian Convertible Marka", "BAM", cont);
    add(Enum::British_Pound,             "British Pound",             "GBP", cont);
    add(Enum::Bulgarian_Lev,             "Bulgarian Lev",             "BGN", cont);
    add(Enum::Croatian_Kuna,             "Croatian Kuna",             "HRK", cont);
    add(Enum::Czech_Koruna,              "Czech Koruna",              "CZK", cont);
    add(Enum::Danish_Krone,              "Danish Krone",              "DKK", cont);
    add(Enum::Dutch_Guilder,             "Dutch Guilder",             "ANG", cont);
    add(Enum::Euro,                      "Euro",                      "EUR", cont);
    add(Enum::Gibraltar_Pound,           "Gibraltar Pound",           "GIP", cont);
    add(Enum::Guernsey_Pound,            "Guernsey Pound",            "GGP", cont);
    add(Enum::Hungarian_Forint,          "Hungarian Forint",          "HUF", cont);
    add(Enum::Icelandic_Krona,           "Icelandic Krona",           "ISK", cont);
    add(Enum::Isle_of_Man_Pound,         "Isle of Man Pound",         "IMP", cont);
    add(Enum::Jersey_Pound,              "Jersey Pound",              "JEP", cont);
    add(Enum::Macedonian_Denar,          "Macedonian Denar",          "MKD", cont);
    add(Enum::Moldovan_Leu,              "Moldovan Leu",              "MDL", cont);
    add(Enum::New_Belarusian_Ruble,      "New Belarusian Ruble",      "BYN", cont);
    add(Enum::Norwegian_Krone,           "Norwegian Krone",           "NOK", cont);
    add(Enum::Polish_Zloty,              "Polish Zloty",              "PLN", cont);
    add(Enum::Romanian_Leu,              "Romanian Leu",              "RON", cont);
    add(Enum::Serbian_Dinar,             "Serbian Dinar",             "RSD", cont);
    add(Enum::Swedish_Krona,             "Swedish Krona",             "SEK", cont);
    add(Enum::Swiss_Franc,               "Swiss Franc",               "CHF", cont);
    add(Enum::Ukrainian_Hryvnia,         "Ukrainian Hryvnia",         "UAH", cont);
}

void Type::addNorthAmerica()
{
    AFcontinent cont = AFcontinent::North_America;
    add(Enum::Aruban_or_Dutch_Guilder,   "Aruban or Dutch Guilder",   "AWG", cont);
    add(Enum::Bahamian_Dollar,           "Bahamian Dollar",           "BSD", cont);
    add(Enum::Barbadian_or_Bajan_Dollar, "Barbadian or Bajan Dollar", "BBD", cont);
    add(Enum::Belizean_Dollar,           "Belizean Dollar",           "BZD", cont);
    add(Enum::Bermudian_Dollar,          "Bermudian Dollar",          "BMD", cont);
    add(Enum::Canadian_Dollar,           "Canadian Dollar",           "CAD", cont);
    add(Enum::Caymanian_Dollar,          "Caymanian Dollar",          "KYD", cont);
    add(Enum::Costa_Rican_Colon,         "Costa Rican Colon",         "CRC", cont);
    add(Enum::Cuban_Convertible_Peso,    "Cuban Convertible Peso",    "CUC", cont);
    add(Enum::Cuban_Peso,                "Cuban Peso",                "CUP", cont);
    add(Enum::Dominican_Peso,            "Dominican Peso",            "DOP", cont);
    add(Enum::East_Caribbean_Dollar,     "East Caribbean Dollar",     "XCD", cont);
    add(Enum::Guatemalan_Quetzal,        "Guatemalan Quetzal",        "GTQ", cont);
    add(Enum::Haitian_Gourde,            "Haitian Gourde",            "HTG", cont);
    add(Enum::Honduran_Lempira,          "Honduran Lempira",          "HNL", cont);
    add(Enum::Jamaican_Dollar,           "Jamaican Dollar",           "JMD", cont);
    add(Enum::Mexican_Peso,              "Mexican Peso",              "MXN", cont);
    add(Enum::Nicaraguan_Cordoba,        "Nicaraguan Cordoba",        "NIO", cont);
    add(Enum::Panamanian_Balboa,         "Panamanian Balboa",         "PAB", cont);
    add(Enum::Salvadoran_Colon,          "Salvadoran Colon",          "SVC", cont);
    add(Enum::Trinidadian_Dollar,        "Trinidadian Dollar",        "TTD", cont);
    add(Enum::US_Dollar,                 "US Dollar",                 "USD", cont);
}

void Type::addOceania()
{
    AFcontinent cont = AFcontinent::Oceania;
    add(Enum::Australian_Dollar,       "Australian Dollar",       "AUD", cont);
    add(Enum::CFP_Franc,               "CFP Franc",               "XPF", cont);
    add(Enum::Fijian_Dollar,           "Fijian Dollar",           "FJD", cont);
    add(Enum::New_Zealand_Dollar,      "New Zealand Dollar",      "NZD", cont);
    add(Enum::Ni_Vanuatu_Vatu,         "Ni-Vanuatu Vatu",         "VUV", cont);
    add(Enum::Papua_New_Guinean_Kina,  "Papua New Guinean Kina",  "PGK", cont);
    add(Enum::Samoan_Tala,             "Samoan Tala",             "WST", cont);
    add(Enum::Solomon_Islander_Dollar, "Solomon Islander Dollar", "SBD", cont);
    add(Enum::Tongan_Paanga,           "Tongan Pa\'anga",         "TOP", cont);
}

void Type::addSouthAmerica()
{
    AFcontinent cont = AFcontinent::South_America;
    add(Enum::Argentine_Peso,        "Argentine Peso",        "ARS", cont);
    add(Enum::Bolivian_Boliviano,    "Bolivian Bolíviano",    "BOB", cont);
    add(Enum::Brazilian_Real,        "Brazilian Real",        "BRL", cont);
    add(Enum::Chilean_Peso,          "Chilean Peso",          "CLP", cont);
    add(Enum::Colombian_Peso,        "Colombian Peso",        "COP", cont);
    add(Enum::Falkland_Island_Pound, "Falkland Island Pound", "FKP", cont);
    add(Enum::Guyanese_Dollar,       "Guyanese Dollar",       "GYD", cont);
    add(Enum::Paraguayan_Guarani,    "Paraguayan Guarani",    "PYG", cont);
    add(Enum::Peruvian_Sol,          "Peruvian Sol",          "PEN", cont);
    add(Enum::Surinamese_Dollar,     "Surinamese Dollar",     "SRD", cont);
    add(Enum::Venezuelan_Bolivar,    "Venezuelan Bolivar",    "VEF", cont);
    add(Enum::Uruguayan_Peso,        "Uruguayan Peso",        "UYU", cont);
}

Type::Type()
{
    addAfrica();
    addEastAsia();
    addWestAsia();
    addEurope();
    addNorthAmerica();
    addOceania();
    addSouthAmerica();

    add(Enum::Bitcoin,         "Bitcoin",         "BTC");
    add(Enum::IMF_Special,     "IMF Special",     "XDR");
    add(Enum::Gold_Ounce,      "Gold Ounce",      "XAU");
    add(Enum::Platinum_Ounce,  "Platinum Ounce",  "XPT");
    add(Enum::Palladium_Ounce, "Palladium Ounce", "XPD");
    add(Enum::Silver_Ounce,    "Silver Ounce",    "XAG");
}

void Type::add(Type::Enum type, QString full, QString short_, AFcontinent continent)
{
    TypeStruct temp;
    temp.m_enum = type;
    temp.m_full = full;
    temp.m_short = short_;
    temp.m_continent = continent;
    m_list.push_back(temp);
}
