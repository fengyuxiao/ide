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
#include <QTextEdit>
#include <QTextStream>
#include <QTextCharFormat>
#include <QTextDocumentWriter>
#include <QTextDocumentFragment>
#include <QToolBar>
#include <QToolButton>
#include <QMenuBar>
#include <QSpinBox>
#include <QComboBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QColorDialog>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QTabWidget>
#include <QKeySequence>
#include <QKeySequenceEdit>
#include <QKeyEventTransition>
#include <QAction>
#include <QActionGroup>
#include <QEvent>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QDialog>
#include <QToolTip>
#include <QPainter>
#include <QMimeData>
#include <QClipboard>
#include <QUndoCommand>
#include <Qsci/qsciapis.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qsciscintilla.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createInit();                  //创建窗口
    createAction();                //创建操作
    createMenuBar();               //创建菜单栏
    createToolBar();               //创建工具栏
    b1.setText("选择文件");
    b1.setParent(this);
    b1.move(100,100);
    connect(&b1,&QPushButton::clicked,this,&MainWindow::getadress);
}
void MainWindow::getadress()
{
    file_Name = QFileDialog::getOpenFileName(this, tr("open file"), "C:/complie",  tr("Allfile(*.*);;mp3 file(*.mp3)"));
    txt1.setText(file_Name);
    txt1.show();
    qDebug()<<file_Name;

}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createInit()                                           //创建窗口
{
    setWindowTitle(tr("C语言集成开发环境 "));
    this -> resize(1300,900);                                           //设置窗口初始大小
    this -> setWindowIcon(QIcon(":/images/Logo.ico"));                  //设置窗口图标

    messageLabel = new QLabel;                                          //信息标签
    messageLabel -> setMinimumSize(150,20);
    messageLabel -> setFrameShape(QFrame::WinPanel);
    messageLabel -> setFrameShadow(QFrame::Sunken);
    ui -> statusBar -> addPermanentWidget(messageLabel);
    messageLabel -> setText(tr("   Welcome  To  Quasar  TextEditor（cpp）  v1.0.1   "));

    tabWidget = new QTabWidget;                                                                     //创建tabWidget
    tabWidget -> setMovable(false);                                                                 //设置Tab页不可移动
    tabWidget -> setTabsClosable(true);                                                             //设置Tab页可以关闭
    tabWidget -> setContextMenuPolicy(Qt::CustomContextMenu);

    setCentralWidget(tabWidget);                                                                    //用TabWidget填充窗口

    fileNumber = 1;                                                                                 //更新窗口标签数量
    editor = new QsciScintilla;
    tabWidget -> setCurrentIndex(tabWidget -> addTab(editor,tr("New %1").arg(fileNumber)));         //为当前标签设定数字标号
    tabWidget -> setCurrentWidget(editor);

}

bool MainWindow::saveFile()                                             //保存槽函数
{
    return saveFile(tabWidget -> currentIndex());
}

bool MainWindow::saveFile(int index)                                    //保存实际操作函数
{

    //QsciScintilla *EDITOR = static_cast<QsciScintilla *>(tabWidget -> widget(index));
    QString fileName = openFileName.at(index);
    if (!fileName.contains("/") && !fileName.contains("\\"))
    {
        return saveAsFile(index);
    }
    QFile file(fileName);
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //out << EDITOR -> text();
    QApplication::restoreOverrideCursor();
    return true;

}

bool MainWindow::saveAsFile()
{
    return saveAsFile(tabWidget -> currentIndex());
}

bool MainWindow::saveAsFile(int index)
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."), QString(),tr("Text Files(*.txt);;All Files (*)"));
    if(fileName.isEmpty())
    {
        return false;
    }

    openFileName.replace(index,fileName);
    tabWidget -> setTabText(index,QFileInfo(fileName).fileName());

    return saveFile(index);
}

void MainWindow::newFile()                                      //新建文件槽函数
{

    QsciScintilla *editor = new QsciScintilla;
    QString fileName = tr("New %1").arg(++fileNumber);
    openFileName << fileName;
    tabWidget -> setCurrentIndex(tabWidget -> addTab(editor,fileName));
    tabWidget -> setCurrentWidget(editor);

}


void MainWindow::createAction()                                 //创建操作
{

//文件操作定义
    openFileAction = new QAction(QIcon(":/images/open.ico"),tr("打开"),this);
    openFileAction -> setShortcut(QKeySequence::Open);
    connect(openFileAction,SIGNAL(triggered()),this,SLOT());

    newFileAction = new QAction(QIcon(":/images/new.ico"),tr("新建"),this);
    newFileAction -> setShortcut(QKeySequence::New);
    connect(newFileAction,SIGNAL(triggered()),this,SLOT(newFile()));

    saveFileAction = new QAction(QIcon(":/images/save.ico"),tr("保存"),this);
    saveFileAction -> setShortcut(QKeySequence::Save);
    connect(saveFileAction,SIGNAL(triggered()),this,SLOT(saveFile()));

    saveAsFileAction = new QAction(QIcon(":/images/saveAs.ico"),tr("另存为"),this);
    saveAsFileAction -> setShortcut(QKeySequence::SaveAs);
    connect(saveAsFileAction,SIGNAL(triggered()),this,SLOT(saveAsFile()));

    saveAllFileAction = new QAction(QIcon(":/images/saveAll.ico"),tr("全部保存"),this);
    saveAllFileAction -> setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);

    closeFileAction = new QAction(QIcon(":/images/closeFile.ico"),tr("关闭"),this);
    closeFileAction -> setShortcut(QKeySequence::Close);

    closeAllFileAction = new QAction(QIcon(":/images/close.ico"),tr("全部关闭"),this);
    closeAllFileAction -> setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_X);

    exitFileAction = new QAction(QIcon(":/images/exit.ico"),tr("退出"),this);
    exitFileAction -> setShortcut(QKeySequence::Quit);
    connect(exitFileAction,SIGNAL(triggered()),this,SLOT(close()));


//编辑操作定义
    undoAction = new QAction(QIcon(":/images/undo.png"),tr("撤销"),this);
    undoAction -> setShortcut(QKeySequence::Undo);

    redoAction = new QAction(QIcon(":/images/redo.png"),tr("恢复"),this);
    redoAction -> setShortcut(QKeySequence::Redo);

    copyAction = new QAction(QIcon(":/images/copy.ico"),tr("复制"),this);
    copyAction -> setShortcut(QKeySequence::Copy);

    cutAction = new QAction(QIcon(":/images/cut.ico"),tr("剪切"),this);
    cutAction -> setShortcut(QKeySequence::Cut);

    pasteAction = new QAction(QIcon(":/images/paste.ico"),tr("粘贴"),this);
    pasteAction -> setShortcut(QKeySequence::Paste);

    selectAllAction = new QAction(QIcon(":/images/select.png"),tr("全选"),this);
    selectAllAction -> setShortcut(QKeySequence::SelectAll);

    goToLineAction = new QAction(QIcon(":/images/gotoLine.ico"),tr("跳转到行"),this);
    goToLineAction -> setShortcut(Qt::CTRL + Qt::Key_G);


//设置操作定义
    fontTypeAction = new QAction(QIcon(":/images/fontType.ico"),tr("字体设置"),this);


//搜索操作定义
    searchAction = new QAction(QIcon(":/images/search.ico"),tr("查找"),this);

    replaceAction = new QAction(QIcon(":/images/replace.ico"),tr("替换"),this);


//视图操作定义
    toolBarAction = new QAction(QIcon(":/images/tool.ico"),tr("显示工具栏"),this);
    connect(toolBarAction,SIGNAL(triggered()),this,SLOT(judgeToolBars()));


//工具操作定义
    compileAction = new QAction(QIcon(":/images/compile.ico"),tr("编译"),this);
    compileAction -> setShortcut(Qt::Key_F9);
    connect(compileAction, SIGNAL(triggered()), this, SLOT(complie()));

    runAction = new QAction(QIcon(":/images/run.ico"),tr("运行"),this);
    runAction -> setShortcut(Qt::Key_F10);
    connect(runAction, SIGNAL(triggered()), this, SLOT(run()));

    compileRunAction = new QAction(QIcon(":/images/compile.ico"),tr("编译运行"),this);
    compileRunAction -> setShortcut(Qt::Key_F11);
    connect(compileRunAction, SIGNAL(triggered()), this, SLOT(complie_and_run()));
    compileAllAction = new QAction(QIcon(":/images/compileAll.ico"),tr("全部编译"),this);
    compileAllAction -> setShortcut(Qt::Key_F12);


//帮助操作定义
    helpDocAction = new QAction(QIcon(":/images/help.ico"),tr("帮助文档"),this);

    aboutAction = new QAction(QIcon(":/images/about.ico"),tr("关于"),this);


}

void MainWindow::createMenuBar()                                //创建菜单栏
{

//文件菜单
    fileMenu = menuBar() -> addMenu(tr("文件"));
    fileMenu -> addAction(openFileAction);
    fileMenu -> addAction(newFileAction);
    fileMenu -> addSeparator();
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
    editMenu -> addSeparator();
    editMenu -> addAction(goToLineAction);


//设置菜单
    settingMenu = menuBar() -> addMenu(tr("设置"));
    settingMenu -> addAction(fontTypeAction);


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


//帮助菜单
    helpMenu = menuBar() -> addMenu(tr("帮助"));
    helpMenu -> addAction(helpDocAction);
    helpMenu -> addAction(aboutAction);


}

void MainWindow::createToolBar()                                    //创建工具栏
{

    toolBar = addToolBar(tr("ToolBar"));
    toolBar -> setMovable(false);

//文件工具
    toolBar -> addAction(openFileAction);
    toolBar -> addAction(newFileAction);
    toolBar -> addAction(saveFileAction);
    toolBar -> addAction(saveAsFileAction);
    toolBar -> addAction(saveAllFileAction);
    toolBar -> addAction(closeAllFileAction);
    toolBar -> addSeparator();


//编辑工具
    toolBar -> addAction(undoAction);
    toolBar -> addAction(redoAction);
    toolBar -> addAction(copyAction);
    toolBar -> addAction(cutAction);
    toolBar -> addAction(pasteAction);
    toolBar -> addAction(selectAllAction);
    toolBar -> addSeparator();


//设置工具
    toolBar -> addAction(fontTypeAction);
    toolBar -> addSeparator();


//搜索工具
    toolBar -> addAction(searchAction);
    toolBar -> addAction(replaceAction);


//编译工具
    toolBar -> addAction(runAction);
    toolBar -> addAction(compileAction);
    toolBar -> addAction(compileRunAction);
    toolBar -> addAction(compileAllAction);
    toolBar -> addSeparator();


//调试工具
    toolBar -> addAction(runAction);
    toolBar -> addAction(compileAction);
    toolBar -> addAction(compileRunAction);
    toolBar -> addAction(compileAllAction);
    toolBar -> addSeparator();


//帮助工具
    toolBar -> addAction(helpDocAction);
    toolBar -> addAction(aboutAction);


}

void MainWindow::judgeToolBars()                                            //判断当前工具栏的显示状态
{
    static bool showToolsAction = true;
    if(showToolsAction)
    {
        showToolsAction = false;
        hideToolBars();
    }
    else
    {
        showToolsAction = true;
        showToolBars();
    }
}

void MainWindow::hideToolBars()                                             //如果当前工具栏是显示状态则隐藏
{
    toolBar -> setVisible(false);
}

void MainWindow::showToolBars()                                             //如果当前工具栏是隐藏状态则显示
{
    toolBar -> setVisible(true);
}
void MainWindow::complie()
{
    //QString file_Name;
 //   int index = tabWidget -> currentIndex();

    dealfileName.clear();
    fileinfo=QFileInfo(file_Name);
    //文件名
    file_name = fileinfo.fileName();
    //文件后缀
    file_suffix = fileinfo.suffix();
    //绝对路径
    file_path = fileinfo.absolutePath();
    qDebug()<<fileinfo;
    qDebug()<<file_name;
    qDebug()<<file_suffix;
    qDebug()<<file_path;
    if(file_suffix!="c"&&file_suffix!="cpp")
    {
        txt1.setText("无法编译此文件");
        txt1.show();
    }
    else {
        QStringList name;
        name=file_Name.split(".");
        qDebug()<<name;
        qDebug()<<name[0];
        dealfileName=name[0].split("/");

        qDebug()<<dealfileName;
        QString  s=dealfileName.join("\\");
        qDebug()<<s;
        QString n="g++ "+s+"."+name[1]+" -o "+s+".exe";
        qDebug()<<n;
        const char *cstr;
        //QString ‐> GBK
        QByteArray bytes = n.toLocal8Bit();
        cstr = bytes.data();

        qDebug()<<cstr;
        QProcess p;
        p.start("cmd.exe",QProcess::ReadWrite);
        p.waitForStarted();
        p.write(cstr);
        p.write("\n");
        p.closeWriteChannel();
        p.waitForFinished();
        p.kill();
        qDebug()<<p.readAllStandardOutput();
        QByteArray qbt=p.readAllStandardError();
        QTextCodec* pTextCodec = QTextCodec::codecForName("System");
        assert(pTextCodec != nullptr);
        QString msg = pTextCodec->toUnicode(qbt);
       // QString msg = QString::fromLocal8Bit(qbt);


        qDebug()<<msg;
        if(msg!=nullptr)
        {
            txt1.setText(msg);
            txt1.show();
        }
        else {
            txt1.setText("编译成功");
            txt1.show();
        }

    }

}
void MainWindow::run()
{
    dealfileName.clear();
    QStringList name;
    name=file_Name.split(".");
    qDebug()<<name;
    qDebug()<<name[0];
    dealfileName=name[0].split("/");
    qDebug()<<dealfileName;
    QString  s=dealfileName.join("\\");
    qDebug()<<s;
    QString n="dir /a "+s+"."+"exe";
    qDebug()<<n;
    const char *cstr;
    QByteArray bytes = n.toLocal8Bit();
    cstr = bytes.data();
    qDebug()<<cstr;
    //确认exe文件存在
    QProcess p;
    p.start("cmd.exe",QProcess::ReadWrite);
    p.waitForStarted();
    p.write(cstr);
    p.write("\n");
    p.closeWriteChannel();
    p.waitForFinished();
    p.kill();
    qDebug()<<p.readAllStandardOutput();
    QByteArray qbt=p.readAllStandardError();
    QString msg = QString::fromLocal8Bit(qbt);
    qDebug()<<msg;
    if(msg!="")
    {
        txt1.setText(msg);
        txt1.show();
    }
    else
    {
        QString temp="start "+s+"."+"exe";
        const char *run_char;
        QByteArray bytes1 = temp.toLocal8Bit();
        run_char = bytes1.data();
        qDebug()<<run_char;
        QProcess q;
        q.start("cmd.exe",QProcess::ReadWrite);
        q.waitForStarted();
        q.write(run_char);
        q.write(" \\?\n");
        q.closeWriteChannel();
        q.waitForFinished();
        p.kill();
        qDebug()<<q.readAllStandardOutput();
        QByteArray qbt=q.readAllStandardError();
        QString msg = QString::fromLocal8Bit(qbt);
        qDebug()<<msg;
    }
}

void MainWindow::complie_and_run()
{
    dealfileName.clear();
    fileinfo=QFileInfo(file_Name);
    //文件名
    file_name = fileinfo.fileName();
    //文件后缀
    file_suffix = fileinfo.suffix();
    //绝对路径
    file_path = fileinfo.absolutePath();
    qDebug()<<fileinfo;
    qDebug()<<file_name;
    qDebug()<<file_suffix;
    qDebug()<<file_path;
    if(file_suffix!="c"&&file_suffix!="cpp")
    {
        txt1.setText("无法编译此文件");
        txt1.show();
    }
    else {
        QStringList name;
        name=file_Name.split(".");
        qDebug()<<name;
        qDebug()<<name[0];
        dealfileName=name[0].split("/");

        qDebug()<<dealfileName;
        QString  s=dealfileName.join("\\");
        qDebug()<<s;
        QString n="g++ "+s+"."+name[1]+" -o "+s+".exe";
        const char *cstr;
        QByteArray bytes = n.toLocal8Bit();
        cstr = bytes.data();
        qDebug()<<cstr;
        QString temp="start "+s+".exe";
        const char *run_char;
        QByteArray bytes1 = temp.toLocal8Bit();
        run_char = bytes1.data();
        qDebug()<<run_char;
        QProcess p;
        p.start("cmd.exe",QProcess::ReadWrite);
        p.waitForStarted();
        p.write(cstr);
        p.write(" \\?\n");
        p.closeWriteChannel();
        p.waitForFinished();
        p.kill();
        qDebug()<<p.readAllStandardOutput();

        QProcess q;
        q.start("cmd.exe",QProcess::ReadWrite);
        q.waitForStarted();
        q.write(run_char);
        q.write("\n");
        q.closeWriteChannel();
        q.waitForFinished();
        q.kill();
    }

}
