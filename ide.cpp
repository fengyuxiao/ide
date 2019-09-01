#include "ide.h"
#pragma execution_character_set("utf-8");

IDE::IDE(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("C语言集成开发环境"));
    showWidget = new ShowWidget(this);
    setCentralWidget(showWidget);

    this->setWindowIcon(QIcon("title.ico"));

    createActions();                            //创建动作
    createMenus();                              //创建菜单
    createToolBars();                           //创建工具栏

    if(img.load("image.png"))
    {
        showWidget -> imageLabel ->setPixmap(QPixmap::fromImage(img));
    }
}

IDE::~IDE()
{

}

void IDE::createActions()
{
//文件菜单动作
    openFileAction = new QAction(QIcon("open.ico"),tr("打开"),this);                                //打开文件
    openFileAction -> setShortcut(tr("Ctrl+O"));
    openFileAction -> setStatusTip(tr("打开一个文件或项目"));

    newFileAction = new QAction(QIcon("new.ico"),tr("新建"),this);                                  //新建文件
    newFileAction -> setShortcut(tr("Ctrl+N"));
    newFileAction -> setStatusTip(tr("新建一个文件或项目"));

    saveAction = new QAction(QIcon("save.ico"),tr("保存"),this);                                    //保存
    saveAction -> setShortcut(tr("Ctrl+S"));
    saveAction -> setStatusTip(tr("保存当前正在编辑的文件"));

    saveAsAction = new QAction(QIcon("saveAs.ico"),tr("另存为"),this);                              //另存为
    saveAsAction -> setStatusTip(tr("将当前正在编辑的文本保存在新的文件中"));

    saveAllAction = new QAction(QIcon("saveAll.ico"),tr("全部保存"),this);                          //全部保存
    saveAllAction -> setShortcut(tr("Shift+Ctrl+S"));
    saveAllAction -> setStatusTip(tr("保存当前打开的所有文件"));

    closeAction = new QAction(tr("关闭"),this);                                                     //关闭
    closeAction -> setShortcut(tr("Ctrl+W"));
    closeAction -> setStatusTip(tr("关闭当前正在编辑的文件"));

    closeAllAction = new QAction(QIcon("closeAll.ico"),tr("全部关闭"),this);                        //全部关闭
    closeAllAction -> setShortcut(tr("Shift+Ctrl+W"));
    closeAllAction -> setStatusTip(tr("关闭当前打开的全部文件"));

    exitAction = new QAction(tr("退出"),this);                                                     //退出
    exitAction -> setShortcut(tr("Ctrl+Q"));
    exitAction -> setStatusTip(tr("退出集成开发环境"));
    connect(exitAction,SIGNAL(triggered()),this,SLOT(exit()));

//编辑菜单动作
    revokeAction = new QAction(QIcon("revoke.ico"),tr("撤销"),this);                               //撤销
    revokeAction -> setShortcut(tr("Ctrl+Z"));
    revokeAction -> setStatusTip(tr("撤销上一步进行的操作"));

    recoveryAction = new QAction(QIcon("recovery.ico"),tr("恢复"),this);                           //恢复
    recoveryAction -> setShortcut(tr("Shift+Ctrl+Z"));
    recoveryAction -> setStatusTip(tr("恢复上一步被撤销的的操作"));

    copyAction = new QAction(QIcon("copy.ico"),tr("复制"),this);                                   //复制
    copyAction -> setShortcut(tr("Ctrl+C"));
    copyAction -> setStatusTip(tr("复制选中的文本到粘贴板"));
    connect(copyAction,SIGNAL(triggered()),showWidget -> text,SLOT(copy()));

    cutAction = new QAction(QIcon("cut.ico"),tr("剪切"),this);                                     //剪切
    cutAction -> setShortcut(tr("Ctrl+X"));
    cutAction -> setStatusTip(tr("将选中文本添加到粘贴板并从当前位置移除"));
    connect(cutAction,SIGNAL(triggered()),showWidget -> text,SLOT(cut()));

    pasteAction = new QAction(QIcon("paste.ico"),tr("粘贴"),this);                                 //粘贴
    pasteAction -> setShortcut(tr("Ctrl+V"));
    pasteAction -> setStatusTip(tr("将粘贴板中的文本粘贴到光标所在位置"));
    connect(pasteAction,SIGNAL(triggered()),showWidget -> text,SLOT(paste()));

    deleteAction = new QAction(tr("删除"),this);                                                   //删除
    deleteAction -> setShortcut(tr("Delete"));
    deleteAction -> setStatusTip(tr("删除当前光标所在位置前的一个字符"));

    allPickAction = new QAction(tr("全选"),this);                                                  //全选
    allPickAction -> setShortcut(tr("Ctrl+A"));
    allPickAction -> setStatusTip(tr("选中当前编辑文件中的所有文本"));

//搜索菜单动作
    seekAction = new QAction(QIcon("seek.ico"),tr("查找"),this);                                   //查找
    seekAction -> setShortcut(tr("Ctrl+F"));
    seekAction -> setStatusTip(tr("查找与选中或输入文本相同的文本并显示"));

    replaceAction = new QAction(QIcon("replace.ico"),tr("替换"),this);                             //替换
    replaceAction -> setShortcut(tr("Ctrl+H"));
    replaceAction -> setStatusTip(tr("将搜索到的文本替换为输入的文本"));

//帮助菜单动作
    helpDocAction = new QAction(QIcon("helpDoc.ico"),tr("帮助文档"),this);                          //帮助文档
    helpDocAction -> setStatusTip(tr("打开记录软件使用方法的文档"));

    aboutAction =new QAction(QIcon("about.ico"),tr("关于"),this);                                  //关于
    aboutAction -> setStatusTip(tr("打开记录软件版本和团队信息的文档"));

}

void IDE::createMenus()
{
//文件菜单
    fileMenu = menuBar() -> addMenu(tr("文件"));
    fileMenu -> addAction(openFileAction);
    fileMenu -> addAction(newFileAction);
    fileMenu -> addAction(saveAction);
    fileMenu -> addAction(saveAllAction);
    fileMenu -> addAction(saveAsAction);
    fileMenu -> addSeparator();
    fileMenu -> addAction(closeAction);
    fileMenu -> addAction(closeAllAction);
    fileMenu -> addSeparator();
    fileMenu -> addAction(exitAction);

//编辑菜单
    editMenu = menuBar() -> addMenu(tr("编辑"));
    editMenu -> addAction(revokeAction);
    editMenu -> addAction(recoveryAction);
    editMenu -> addSeparator();
    editMenu -> addAction(copyAction);
    editMenu -> addAction(cutAction);
    editMenu -> addAction(pasteAction);
    editMenu -> addAction(deleteAction);
    editMenu -> addAction(allPickAction);

//搜索菜单
    searchMenu = menuBar() -> addMenu(tr("搜索"));
    searchMenu -> addAction(seekAction);
    searchMenu -> addAction(replaceAction);

//视图菜单
    viewMenu = menuBar() -> addMenu(tr("视图"));

//设置菜单
    settingMenu = menuBar() -> addMenu(tr("设置"));

//工具菜单
    toolMenu = menuBar() -> addMenu(tr("工具"));

//帮助菜单
    helpMenu = menuBar() -> addMenu(tr("帮助"));
    helpMenu -> addAction(helpDocAction);
    helpMenu -> addAction(aboutAction);

}

void IDE::createToolBars()
{
//文件工具栏
    fileTool = addToolBar("File");
    fileTool -> addAction(openFileAction);
    fileTool -> addAction(newFileAction);
    fileTool -> addAction(saveAction);
    fileTool -> addAction(saveAllAction);
    fileTool -> addAction(saveAsAction);
    fileTool -> addAction(closeAllAction);

//编辑工具栏
    editTool = addToolBar("Edit");
    editTool -> addAction(revokeAction);
    editTool -> addAction(recoveryAction);
    editTool -> addAction(copyAction);
    editTool -> addAction(cutAction);
    editTool -> addAction(pasteAction);

//搜索工具栏
    searchTool = addToolBar("Search");
    searchTool -> addAction(seekAction);
    searchTool -> addAction(replaceAction);

//帮助工具栏
    helpTool = addToolBar("Help");
    helpTool -> addAction(helpDocAction);
    helpTool -> addAction(aboutAction);

}

