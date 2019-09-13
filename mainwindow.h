#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFont>
#include <QFile>
#include <QLabel>
#include <QEvent>
#include <QDebug>
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QShortcut>
#include <QLineEdit>
#include <QComboBox>
#include <QKeyEvent>
#include <QTabWidget>
#include <QStatusBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <QMainWindow>
#include <QActionGroup>
#include <QFontComboBox>
#include <QTextCharFormat>
#include <Qsci/qsciapis.h>
#include <QContextMenuEvent>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qsciscintilla.h>


#define EDITOR static_cast<QsciScintilla *>(tabWidget->currentWidget())


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

    void saveFile(int index);                                       //保存实际操作函数
    void saveAsFile(int index);                                     //另存为实际操作函数
    void closeFile(int index);                                      //关闭文件实际操作函数

    QVector<QString> fileNameVector;                                //存储对应标签页对应的文件名

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

//搜索菜单操作
    QAction *searchAction;                          //查找替换

//视图菜单操作
    QAction *toolBarAction;                         //工具栏显示

//工具菜单操作
    QAction *runAction;                             //运行
    QAction *compileAction;                         //编译
    QAction *compileRunAction;                      //编译运行

//帮助菜单操作
    QAction *helpDocAction;                         //帮助文档
    QAction *aboutAction;                           //关于

//菜单栏设计
    QMenu *fileMenu;                                //文件菜单
    QMenu *editMenu;                                //编辑菜单
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


private slots:
    void newFileSlot();                                 //新建文件槽函数
    void openFileSlot();                                //打开文件槽函数

    bool saveFileSlot();                                //保存槽函数
    bool saveAsFileSlot();                              //另存为槽函数
    bool saveAllFileSlot();                             //全部保存槽函数
    bool closeFileSlot();                               //关闭文件槽函数
    bool closeAllFileSlot();                            //全部关闭槽函数

    void cutSlot();                                     //剪切槽函数
    void undoSlot();                                    //撤销槽函数
    void redoSlot();                                    //恢复槽函数
    void copySlot();                                    //复制槽函数
    void pasteSlot();                                   //粘贴槽函数
    void selectAllSlot();                               //全选槽函数

    void judgeToolBars();                               //工具栏显示

    void aboutSlot();                                   //关于槽函数
    void searchSlot();                                  //查找替换槽函数
    void helpDocSlot();                                 //帮助文档槽函数

    void comment();


};

#endif // MAINWINDOW_H
