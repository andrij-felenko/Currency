import qbs
import "../../../qbs/test.qbs" as TestApp

TestApp {
    projectName: "currencyAF"
    property string organization: sourceDirectory
    cpp.defines: "organizationq=" + organization

    Depends { name: "lib_currencyAF" }
}
