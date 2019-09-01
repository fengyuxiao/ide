#ifndef IDE_H
#define IDE_H

#include <QMainWindow>
#include <QImage>
#include <QMenu>
#include <QLabel>
#include <QMenuBar>
#include <QAction>
#include <QTextCharFormat>
#include <QComboBox>
#include <QSpinBox>
#include <QToolBar>
#include <QMouseEvent>
#include <QFontComboBox>
#include <QToolButton>
#include "showwidget.h"
#pragma execution_character_set("utf-8");

class IDE : public QMainWindow
{
    Q_OBJECT

public:
    IDE(QWidget *parent = nullptr);
    ~IDE();
    void createActions();
    void createToolBars();
    void createMenus();
    void loadFile(QString filename);
    void mergeFormat(QTextCharFormat);

private:

//菜单栏
    QMenu *fileMenu;                      //文件菜单
    QMenu *editMenu;                      //编辑菜单
    QMenu *searchMenu;                    //搜索菜单
    QMenu *viewMenu;                      //视图菜单
    QMenu *settingMenu;                   //设置菜单
    QMenu *toolMenu;                      //工具菜单
    QMenu *helpMenu;                      //帮助菜单

    QImage img;
    QString fileName;
    ShowWidget *showWidget;

//文件菜单栏
    QAction *openFileAction;               //打开
    QAction *newFileAction;                //新建
    QAction *saveAction;                   //保存
    QAction *saveAsAction;                 //另存为
    QAction *saveAllAction;                //全部保存
    QAction *closeAction;                  //关闭
    QAction *closeAllAction;               //全部关闭
    QAction *exitAction;                   //退出

//编辑菜单栏
    QAction *revokeAction;                 //撤销
    QAction *recoveryAction;               //恢复
    QAction *copyAction;                   //复制
    QAction *cutAction;                    //剪切
    QAction *pasteAction;                  //粘贴
    QAction *deleteAction;                 //删除
    QAction *allPickAction;                //全选

//搜索菜单栏
    QAction *seekAction;                   //查找
    QAction *replaceAction;                //替换

//帮助菜单栏
    QAction *helpDocAction;                //帮助文档
    QAction *aboutAction;                  //关于

//文件工具栏
    QToolBar *fileTool;                    //文件
    QToolBar *editTool;                    //编辑
    QToolBar *searchTool;                  //搜索
    QToolBar *helpTool;                    //帮助
};

#endif // IDE_H
