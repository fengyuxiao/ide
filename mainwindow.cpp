#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFont>
#include <QFontDialog>
#include <QFontDatabase>
#include <QFontComboBox>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintEngine>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QTextDocumentFragment>
#include <QTextStream>
#include <QTextCharFormat>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QColorDialog>
#include <QComboBox>
#include <QMessageBox>
#include <QTabWidget>
#include <QKeySequence>
#include <QActionGroup>
#include <QMimeData>
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createTab();
    createAction();
    createMenuBar();
    createToolBar();

    setCentralWidget(tabWidget);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createTab()
{
    tabWidget = new QTabWidget(this);
    tabWidget -> setMovable(true);
    tabWidget -> setTabsClosable(true);
    tabWidget -> setContextMenuPolicy(Qt::CustomContextMenu);
}

void MainWindow::createAction()
{

//文件操作定义
    openFileAction = new QAction(QIcon(),tr("打开"),this);
    openFileAction -> setShortcut(QKeySequence::Open);
    openFileAction -> setStatusTip(tr(""));
    connect(openFileAction,SIGNAL(triggered()),this,SLOT(openFile()));

    newFileAction = new QAction(QIcon(""),tr("新建"),this);
    newFileAction -> setShortcut(QKeySequence::New);
    newFileAction -> setStatusTip(tr(""));

    saveFileAction = new QAction(QIcon(""),tr("保存"),this);
    saveFileAction -> setShortcut(QKeySequence::Save);
    saveFileAction -> setStatusTip(tr(""));

    saveAsFileAction = new QAction(QIcon(""),tr("另存为"),this);
    saveAsFileAction -> setShortcut(QKeySequence::SaveAs);
    saveAsFileAction -> setStatusTip(tr(""));

    saveAllFileAction = new QAction(QIcon(""),tr("全部保存"),this);
    saveAllFileAction -> setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    saveAllFileAction -> setStatusTip(tr(""));

    closeFileAction = new QAction(QIcon(""),tr("关闭"),this);
    closeFileAction -> setShortcut(QKeySequence::Close);
    closeFileAction -> setStatusTip(tr(""));

    closeAllFileAction = new QAction(QIcon(""),tr("全部关闭"),this);
    closeAllFileAction -> setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_X);
    closeAllFileAction -> setStatusTip(tr(""));

    exitFileAction = new QAction(QIcon(""),tr("退出"),this);
    exitFileAction -> setShortcut(QKeySequence::Quit);
    exitFileAction -> setStatusTip(tr(""));

//编辑操作定义
    undoAction = new QAction(QIcon(""),tr("撤销"),this);
    undoAction -> setShortcut(QKeySequence::Undo);
    undoAction -> setStatusTip(tr(""));

    redoAction = new QAction(QIcon(""),tr("恢复"),this);
    redoAction -> setShortcut(QKeySequence::Redo);
    redoAction -> setStatusTip(tr(""));

    copyAction = new QAction(QIcon(""),tr("复制"),this);
    copyAction -> setShortcut(QKeySequence::Copy);
    copyAction -> setStatusTip(tr(""));

    cutAction = new QAction(QIcon(""),tr("剪切"),this);
    cutAction -> setShortcut(QKeySequence::Cut);
    cutAction -> setStatusTip(tr(""));

    pasteAction = new QAction(QIcon(""),tr("粘贴"),this);
    pasteAction -> setShortcut(QKeySequence::Paste);
    pasteAction -> setStatusTip(tr(""));

    selectAllAction = new QAction(QIcon(""),tr("全选"),this);
    selectAllAction -> setShortcut(QKeySequence::SelectAll);
    selectAllAction -> setStatusTip(tr(""));

    goToLineAction = new QAction(QIcon(""),tr("跳转到行"),this);
    goToLineAction -> setShortcut(Qt::CTRL + Qt::Key_G);
    goToLineAction -> setStatusTip(tr(""));

//设置操作定义
    fontColorAction = new QAction(QIcon(""),tr("文本颜色"),this);
    fontColorAction -> setStatusTip(tr(""));

    fontTypeAction = new QAction(QIcon(""),tr("文本字体"),this);
    fontTypeAction -> setStatusTip(tr(""));

    fontBoldAction = new QAction(QIcon(""),tr("粗体"),this);
    fontBoldAction -> setShortcut(Qt::CTRL + Qt::Key_B);
    fontBoldAction -> setStatusTip(tr(""));
    fontBoldAction -> setCheckable(true);

    fontItalicAction = new QAction(QIcon(""),tr("斜体"),this);
    fontItalicAction -> setShortcut(Qt::CTRL + Qt::Key_I);
    fontItalicAction -> setStatusTip(tr(""));
    fontItalicAction -> setCheckable(true);

    fontUnderlineAction = new QAction(QIcon(""),tr("下划线"),this);
    fontUnderlineAction -> setShortcut(Qt::CTRL + Qt::Key_U);
    fontUnderlineAction -> setStatusTip(tr(""));
    fontUnderlineAction -> setCheckable(true);

//搜索操作定义
    searchAction = new QAction(QIcon(""),tr("查找"),this);
    searchAction -> setStatusTip(tr(""));

    replaceAction = new QAction(QIcon(""),tr("替换"),this);
    replaceAction -> setStatusTip(tr(""));

//视图操作定义
    toolBarAction = new QAction(QIcon(""),tr("显示工具栏"),this);
    toolBarAction -> setStatusTip(tr(""));


//工具操作定义
    compileAction = new QAction(QIcon(""),tr("编译"),this);
    compileAction -> setShortcut(Qt::Key_F9);
    compileAction -> setStatusTip(tr(""));

    runAction = new QAction(QIcon(""),tr("运行"),this);
    runAction -> setShortcut(Qt::Key_F10);
    runAction -> setStatusTip(tr(""));

    compileRunAction = new QAction(QIcon(""),tr("编译运行"),this);
    compileRunAction -> setShortcut(Qt::Key_F11);
    compileRunAction -> setStatusTip(tr(""));

    compileAllAction = new QAction(QIcon(""),tr("全部编译"),this);
    compileAllAction -> setShortcut(Qt::Key_F12);
    compileAllAction -> setStatusTip(tr(""));

    startDebugAction = new QAction(QIcon(""),tr("开始调试"),this);
    startDebugAction -> setShortcut(Qt::Key_F5);
    startDebugAction -> setStatusTip(tr(""));

    endDebugAction = new QAction(QIcon(""),tr("结束调试"),this);
    endDebugAction -> setStatusTip(tr(""));

    setBreakAction = new QAction(QIcon(""),tr("设置断点"),this);
    setBreakAction -> setStatusTip(tr(""));

    cancelBreakAction = new QAction(QIcon(""),tr("取消断点"),this);
    cancelBreakAction -> setStatusTip(tr(""));

    nextLineAction = new QAction(QIcon(""),tr("下一步"),this);
    nextLineAction -> setShortcut(Qt::Key_F7);
    nextLineAction -> setStatusTip(tr(""));

    nextBreakAction = new QAction(QIcon(""),tr("跳到下一个断点"),this);
    nextBreakAction -> setStatusTip(tr(""));

    enterAction = new QAction(QIcon(""),tr("单步进入"),this);
    enterAction -> setStatusTip(tr(""));

    jumpAction = new QAction(QIcon(""),tr("跳出"),this);
    jumpAction -> setStatusTip(tr(""));

    addTrackAction = new QAction(QIcon(""),tr("添加查看"),this);
    addTrackAction -> setStatusTip(tr(""));

//帮助操作定义
    helpDocAction = new QAction(QIcon(""),tr("帮助文档"),this);
    helpDocAction -> setStatusTip(tr(""));

    aboutAction = new QAction(QIcon(""),tr("关于"),this);
    aboutAction -> setStatusTip(tr(""));

}

void MainWindow::createMenuBar()
{

//文件菜单
    fileMenu = menuBar() -> addMenu(tr("文件"));
    fileMenu -> addAction(openFileAction);
    fileMenu -> addAction(newFileAction);
    fileMenu -> addAction(saveFileAction);
    fileMenu -> addAction(saveAsFileAction);
    fileMenu -> addAction(saveAllFileAction);
    fileMenu -> addSeparator();
    fileMenu -> addAction(closeFileAction);
    fileMenu -> addAction(closeAllFileAction);
    fileMenu -> addSeparator();
    fileMenu -> addAction(exitFileAction);

//编辑菜单
    editMenu = menuBar() -> addMenu(tr("编辑"));
    editMenu -> addAction(undoAction);
    editMenu -> addAction(redoAction);
    editMenu -> addSeparator();
    editMenu -> addAction(copyAction);
    editMenu -> addAction(cutAction);
    editMenu -> addAction(pasteAction);
    editMenu -> addAction(selectAllAction);

//设置菜单
    settingMenu = menuBar() -> addMenu(tr("设置"));
    settingMenu -> addAction(fontBoldAction);
    settingMenu -> addAction(fontItalicAction);
    settingMenu -> addAction(fontUnderlineAction);
    settingMenu -> addSeparator();
    settingMenu -> addAction(fontTypeAction);
    settingMenu -> addAction(fontColorAction);

//搜索菜单
    searchMenu = menuBar() -> addMenu(tr("搜索"));
    searchMenu -> addAction(searchAction);
    searchMenu -> addAction(replaceAction);

//视图菜单
    viewMenu = menuBar() -> addMenu(tr("视图"));
    viewMenu -> addAction(toolBarAction);

//工具菜单
    toolMenu = menuBar() -> addMenu(tr("工具"));
    toolMenu -> addAction(runAction);
    toolMenu -> addAction(compileAction);
    toolMenu -> addAction(compileRunAction);
    toolMenu -> addAction(compileAllAction);
    toolMenu -> addSeparator();
    toolMenu -> addAction(startDebugAction);
    toolMenu -> addAction(endDebugAction);
    toolMenu -> addAction(setBreakAction);
    toolMenu -> addAction(cancelBreakAction);
    toolMenu -> addSeparator();
    toolMenu -> addAction(nextLineAction);
    toolMenu -> addAction(nextBreakAction);
    toolMenu -> addAction(enterAction);
    toolMenu -> addAction(jumpAction);
    toolMenu -> addAction(addTrackAction);

//帮助菜单
    helpMenu = menuBar() -> addMenu(tr("帮助"));
    helpMenu -> addAction(helpDocAction);
    helpMenu -> addAction(aboutAction);

}

void MainWindow::createToolBar()
{

    toolBar = new QToolBar(this);
    toolBar -> setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    fontBar = new QToolBar(this);
    fontBar -> setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    addToolBar(toolBar);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(fontBar);
    toolBar -> setMovable(false);
    fontBar -> setMovable(false);

    toolBar -> addAction(openFileAction);
    toolBar -> addAction(newFileAction);
    toolBar -> addAction(saveFileAction);
    toolBar -> addAction(saveAsFileAction);
    toolBar -> addAction(saveAllFileAction);
    toolBar -> addAction(closeAllFileAction);
    toolBar -> addSeparator();

    toolBar -> addAction(undoAction);
    toolBar -> addAction(redoAction);
    toolBar -> addAction(copyAction);
    toolBar -> addAction(cutAction);
    toolBar -> addAction(pasteAction);
    toolBar -> addAction(selectAllAction);
    toolBar -> addSeparator();

    toolBar -> addAction(fontBoldAction);
    toolBar -> addAction(fontItalicAction);
    toolBar -> addAction(fontUnderlineAction);
    toolBar -> addSeparator();

    toolBar -> addAction(searchAction);
    toolBar -> addAction(replaceAction);
    toolBar -> addSeparator();

    toolBar -> addAction(runAction);
    toolBar -> addAction(compileAction);
    toolBar -> addAction(compileRunAction);
    toolBar -> addAction(compileAllAction);
    toolBar -> addSeparator();

    toolBar -> addAction(helpDocAction);
    toolBar -> addAction(aboutAction);

}

void MainWindow::createNewTab(const QString &filename, QFile &file)
{

    openedFiles << filename;

}

void MainWindow::openFile()
{

    QStringList files;
    QString filename;
    files = QFileDialog::getOpenFileNames(this,tr("Open files..."),QString(),tr("All Files(*);;Text Files(*.txt)"));
    if(files.count())
    {
        for(int i=0;i<files.count();i++)
        {
            filename = files.at(i);
            if(QFile::exists(filename))
            {
                QFile file(filename);
                if(file.open(QFile::ReadOnly))
                {
                    if(openedFiles.contains(filename))
                    {
                        continue;
                    }
                    createNewTab(filename,file);
                }
            }
        }
    }
}

void MainWindow::openFile(QString filename)
{

}
