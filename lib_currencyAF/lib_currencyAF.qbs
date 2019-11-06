import qbs
import "../../../qbs/lib_static.qbs" as LibStaticT

LibStaticT {
    libraryName: "currencyAF"

    Depends { name: "lib_baseAF" }
    Depends { name: "Qt.network" }
}
