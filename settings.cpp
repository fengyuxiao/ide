#include "settings.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QApplication>

Settings::Settings()
{

    iniFile = QApplication::applicationDirPath();
    iniFile += iniFile.endsWith('/') ? "settings.ini" : "/settings.ini";
    createIniFile(iniFile);

    QSettings txsettings(iniFile, QSettings::IniFormat);

    txsettings.beginGroup("General");
    recentFiles = txsettings.value("recentFiles").toStringList();
    maxRecentFiles = txsettings.value("maxRecentFiles", 10).toInt();
    showReadme = txsettings.value("showReadme", false).toBool();
    mainWindowsGeometry = txsettings.value("mainWindowGeometry").toByteArray();
    mainWindowState = txsettings.value("mainWindoState").toByteArray();
    txsettings.endGroup();

    txsettings.beginGroup("Editor");
    fontFamily = txsettings.value("fontFamily", "CourierNew").toString();
    fontSize = txsettings.value("fontSize", 12).toInt();
    fontStyle = txsettings.value("fontStyle", "Normal").toString();
    showLineNumber = txsettings.value("showLineNumber", true).toBool();
    tabIndents = txsettings.value("tabIndents").toBool(); //false
    autoIndent = txsettings.value("autoIndent").toBool();
    backUnindent = txsettings.value("backUnindent").toBool();
    spaceTabs = txsettings.value("spaceTabs", true).toBool();
    indentSize = txsettings.value("indentSize", 4).toInt();
    tabSize = txsettings.value("tabSize", 4).toInt();
    whitespaces = txsettings.value("whitespaces").toBool();
    txsettings.endGroup();

}

Settings::~Settings()
{

    iniFile = QApplication::applicationDirPath();
    iniFile += iniFile.endsWith('/') ? "settings.ini" : "/settings.ini";
    createIniFile(iniFile);
    QSettings txsettings(iniFile, QSettings::IniFormat);

    txsettings.beginGroup("General");
    txsettings.setValue("recentFiles", recentFiles);
    txsettings.setValue("maxRecentFile", maxRecentFiles);
    txsettings.setValue("showReadme", showReadme);
    txsettings.setValue("mainWindowGeometry", mainWindowsGeometry);
    txsettings.setValue("mainWindoState", mainWindowState);
    txsettings.endGroup();

    txsettings.beginGroup("Editor");
    txsettings.setValue("fontFamily", fontFamily);
    txsettings.setValue("fontSize", fontSize);
    txsettings.setValue("fontStyle", fontStyle);
    txsettings.setValue("showLineNumber", showLineNumber);
    txsettings.setValue("tabIndents", tabIndents);
    txsettings.setValue("autoIndent", autoIndent);
    txsettings.setValue("backUnindent", backUnindent);
    txsettings.setValue("spaceTabs", spaceTabs);
    txsettings.setValue("indentSize", indentSize);
    txsettings.setValue("tabSize", tabSize);
    txsettings.setValue("whitespaces", whitespaces);
    txsettings.endGroup(); //Editor

    txsettings.beginGroup("Search&Replace");
    findHistory.sort();
    replaceHistory.sort();
    txsettings.setValue("findHistory", findHistory);
    txsettings.setValue("maxHistory", maxHistory);
    txsettings.setValue("replaceHistory", replaceHistory);
    txsettings.setValue("matchCase", matchCase);
    txsettings.setValue("regExp", regExp);
    txsettings.endGroup();

}

void Settings::resetting(int receiver)
{
    emit reread(receiver);
}

void Settings::createIniFile(QString iniFile)
{
    if(QFile::exists(iniFile))
    {
        return;
    }
    else
    {
        QFile file(iniFile);
        file.open(QIODevice::ReadWrite | QIODevice::Truncate);
        file.close();
    }
}
