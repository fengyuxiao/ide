#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QStringList>
#include <QString>
#include <QByteArray>
#include <QSettings>

#pragma execution_character_set("utf-8")

class Settings : public QObject
{
    Q_OBJECT
public:
    enum Receiver
    {
        Init = 1,                   //0000001
        Editor = 2,                 //0000010
        Highlighter = 4             //0000100
    };

    Settings();
    ~Settings();

    QString iniFile;
    QString fontFamily;
    QString fontStyle;
    QStringList findHistory;
    QStringList replaceHistory;
    QStringList recentFiles;
    QByteArray mainWindowsGeometry;
    QByteArray mainWindowState;

    bool showLineNumber;
    bool tabIndents;
    bool autoIndent;
    bool backUnindent;
    bool spaceTabs;
    bool showReadme;
    bool whitespaces;
    bool matchCase;
    bool regExp;
    int maxHistory;
    int indentSize;
    int tabSize;
    int fontSize;
    int maxRecentFiles;

private:
    void createIniFile(QString iniFile);                //创建配置文件

signals:
    void reread(int);

public slots:
    void resetting(int receiver);

};

#endif // SETTINGS_H
