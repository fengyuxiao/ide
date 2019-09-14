#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFont>
#include <QFile>
#include <QLabel>
#include <QEvent>
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QComboBox>
#include <QKeyEvent>
#include <QTabWidget>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QActionGroup>
#include <QFontComboBox>
#include <QTextCharFormat>
#include <Qsci/qsciapis.h>
#include <QContextMenuEvent>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qsciscintilla.h>
#include<QProcess>
#include<QFileDialog>
#include<string>
#include<QMessageBox>
#include <QDebug>
#include<QPushButton>
#include <QTextCodec>
class QsciScintilla;

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

    int fileNumber;                                                 //记录打开的文件数量

    void createInit();                                              //创建窗口
    void createAction();                                            //创建操作
    void createMenuBar();                                           //创建菜单栏
    void createToolBar();                                           //创建工具栏

    void hideToolBars();                                            //隐藏工具栏
    void showToolBars();                                            //显示工具栏

    QStringList openFileName;                                       //存储打开文件的名字
    QPushButton b1;
//操作设计

//文件菜单操作
    QAction *openFileAction;                        //打开文件
    QAction *newFileAction;                         //新建文件
    QAction *saveFileAction;                        //保存
    QAction *saveAsFileAction;                      //另存为
    QAction *saveAllFileAction;                     //全部保存
    QAction *exportFileAction;                      //导出
    QAction *closeFileAction;                       //关闭
    QAction *closeAllFileAction;                    //全部关闭
    QAction *exitFileAction;                        //退出

//编辑菜单操作
    QAction *undoAction;                            //撤销
    QAction *redoAction;                            //恢复
    QAction *copyAction;                            //复制
    QAction *cutAction;                             //剪切
    QAction *pasteAction;                           //粘贴
    QAction *selectAllAction;                       //全选
    QAction *goToLineAction;                        //跳转到行

//设置菜单操作
    QAction *fontTypeAction;                        //文本字体设置


//搜索菜单操作
    QAction *searchAction;                          //查找
    QAction *replaceAction;                         //替换

//视图菜单操作
    QAction *toolBarAction;                         //工具栏显示

//工具菜单操作
    QAction *runAction;                             //运行
    QAction *compileAction;                         //编译
    QAction *compileRunAction;                      //编译运行
    QAction *compileAllAction;                      //全部编译


//帮助菜单操作
    QAction *helpDocAction;                         //帮助文档
    QAction *aboutAction;                           //关于

//菜单栏设计
    QMenu *fileMenu;                                //文件菜单
    QMenu *editMenu;                                //编辑菜单
    QMenu *settingMenu;                             //设置菜单
    QMenu *searchMenu;                              //搜索菜单
    QMenu *viewMenu;                                //视图菜单
    QMenu *toolMenu;                                //工具菜单
    QMenu *helpMenu;                                //帮助菜单

//工具栏设计
    QToolBar *toolBar;                              //工具栏

//Tab页设计
    QTabWidget *tabWidget;                          //Tab页

//标签信息
    QLabel *messageLabel;                           //标签

//Qsci编辑区
    QsciScintilla *editor;                          //Qsci编辑区
//编译
    QString file_Name;
    QStringList dealfileName;
    QString file_suffix,file_name,file_path;
    QFileInfo fileinfo;
    QMessageBox txt1;

    QMessageBox txt2;
private slots:
    void newFile();                                 //新建文件槽函数

    bool saveFile();                                //保存槽函数
    bool saveAsFile();                              //另存为槽函数

    bool saveFile(int index);                       //保存实际操作函数
    bool saveAsFile(int index);                     //另存为实际操作函数

    void judgeToolBars();                           //工具栏显示
//编译
    void complie();
    void run();
    void complie_and_run();
    void getadress();

};

#endif // MAINWINDOW_H
