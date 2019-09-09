#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QMenuBar>
#include <QToolBar>
#include <QFile>
#include <QFont>
#include <QComboBox>
#include <QAction>
#include <QActionGroup>
#include <QFontComboBox>
#include <QTextCharFormat>
#include <QtPrintSupport/QPrinter>

#include "showwidget.h"
#include "settings.h"
#include "texttab.h"

#define EDITOR   static_cast<TextTab *>(tabWidget->currentWidget())

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Settings *,QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    Settings *settings;
    QStringList openedFiles;
    QComboBox *comboStyle;
    QComboBox *comboSize;
    QFontComboBox *comboFont;
    QList<QAction *> recentFileActs;
    QActionGroup *openedFilesGrp;

    int newNumber;

//创建主窗口
    void createinit();                                              //初始化
    void createTab();                                               //初始化Tab编辑页
    void createMenuBar();                                           //初始化菜单栏
    void createToolBar();                                           //初始化工具栏
    void createAction();                                            //初始化操作

    bool maybeSave(int index);

    void closeDuplicate(int index);
    void closeEvent(QCloseEvent *event);
    void createNewTab(const QString& fileName, QFile& file);
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void currentCharFormatChanged(const QTextCharFormat &format);
    void colorChanged(const QColor &col);
    void fontChanged(const QFont &font);

    void showReadme();
    void updateActions();
    void refreshActions();
    void updateRecentFilesList();
    void updateComboStyle();
    void fillRecentFileActs();

    void hideToolBars();
    void showToolBars();

//菜单栏设计
    QMenu *fileMenu;                //文件菜单
    QMenu *editMenu;                //编辑菜单
    QMenu *settingMenu;             //设置菜单
    QMenu *searchMenu;              //搜索菜单
    QMenu *viewMenu;                //视图菜单
    QMenu *toolMenu;                //工具菜单
    QMenu *helpMenu;                //帮助菜单

//工具栏设计
    QToolBar *toolBar;              //工具栏

//操作设计
    //文件菜单操作
    QAction *openFileAction;                //打开文件
    QAction *newFileAction;                 //新建文件
    QAction *saveFileAction;                //保存
    QAction *saveAsFileAction;              //另存为
    QAction *saveAllFileAction;             //全部保存
    QAction *exportFileAction;              //导出
    QAction *closeFileAction;               //关闭
    QAction *closeAllFileAction;            //全部关闭
    QAction *exitFileAction;                //退出

    //编辑菜单操作
    QAction *undoAction;                //撤销
    QAction *redoAction;                //恢复
    QAction *copyAction;                //复制
    QAction *cutAction;                 //剪切
    QAction *pasteAction;               //粘贴
    QAction *selectAllAction;           //全选
    QAction *goToLineAction;            //跳转到行

    //设置菜单操作
    QAction *fontTypeAction;            //文本字体设置

    //搜索菜单操作
    QAction *searchAction;              //查找
    QAction *replaceAction;             //替换

    //视图菜单操作
    QAction *toolBarAction;             //工具栏显示

    //工具菜单操作
    QAction *runAction;                 //运行
    QAction *compileAction;             //编译
    QAction *compileRunAction;          //编译运行
    QAction *compileAllAction;          //全部编译
    QAction *startDebugAction;          //开始调试
    QAction *endDebugAction;            //结束调试
    QAction *setBreakAction;            //设置断点
    QAction *cancelBreakAction;         //取消断点
    QAction *nextLineAction;            //下一步
    QAction *nextBreakAction;           //跳到下一个断点
    QAction *enterAction;               //单步进入
    QAction *jumpAction;                //跳出
    QAction *addTrackAction;            //添加查看

    //帮助菜单操作
    QAction *helpDocAction;             //帮助文档
    QAction *aboutAction;               //关于

    //Tab文档
    QTabWidget *tabWidget;              //Tab编辑页

public slots:

private slots:
    void currentChanged(int index); //tab发生改变时执行的槽
    void modificationChanged(bool changed); //文档发生改变
    void selectionChanged();    //文档选中状态发生改变
    void openFile();    //打开文件
    void openFile(QString FileName);    //打开文件
    void newFile(); //新建文件
    bool fileSaveAs(int index); //文件另存为（保存指定文件）
    bool fileSave(int index);   //保存文件（保存指定文件）
    bool fileSaveAs();  //文件另存为（保存当前文件）
    bool fileSave();    //保存文件（保存当前文件）
    bool fileSaveAll(); //保存所有文件
    void fileClose(int index);   //关闭文件（指定文件）
    void fileClose();   //关闭文件（当前文件）
    void fileCloseAll();    //关闭所有文件

    void textFont(); //设置文本字体
    void textFontFamily(const QString& font); //设置文本字体（通过字体组合框）

    void judgeToolBars();

};

#endif // MAINWINDOW_H
