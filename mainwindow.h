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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QStringList openedFiles;

//创建主窗口
    void createTab();                                           //初始化Tab编辑页
    void createMenuBar();                                       //初始化菜单栏
    void createToolBar();                                       //初始化工具栏
    void createAction();                                        //初始化操作
    void createNewTab(const QString & filename, QFile & file);      //创建新Tab页

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
    QToolBar *fontBar;              //文本设置栏

//操作设计
    //文件菜单操作
    QAction *openFileAction;                //打开文件
    QAction *newFileAction;                 //新建文件
    QAction *saveFileAction;                //保存
    QAction *saveAsFileAction;              //另存为
    QAction *saveAllFileAction;             //全部保存
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
    QAction *fontColorAction;           //文本颜色设置
    QAction *fontBoldAction;            //文本粗体设置
    QAction *fontItalicAction;          //文本斜体设置
    QAction *fontUnderlineAction;       //文本下划线设置

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
    void openFile();                            //打开文件
    void openFile(QString filename);

};

#endif // MAINWINDOW_H
