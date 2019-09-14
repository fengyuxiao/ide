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
#include <QShortcut>
#include <QWidget>
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
#include <QObject>
#include <QToolTip>
#include <QPainter>
#include <QMimeData>
#include <QClipboard>
#include <QUndoCommand>
#include <QDebug>
#include <QLineEdit>
#include <QPushButton>
#include <Qsci/qsciapis.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qsciscintilla.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createInit();                           //创建窗口
    createAction();                         //创建操作
    createMenuBar();                        //创建菜单栏
    createToolBar();                        //创建工具栏

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
    connect(tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(closeFileSlot()));
    setCentralWidget(tabWidget);

    fileNumber = 0;                                                                                 //更新窗口标签数量

    QsciScintilla *editor = new QsciScintilla;
    QString fileName = tr("New %1").arg(++fileNumber);
    tabWidget -> setCurrentIndex(tabWidget -> addTab(editor,fileName));
    tabWidget -> setCurrentWidget(editor);
    fileNameVector.append(fileName);

    QsciLexerCPP *textLexer = new QsciLexerCPP;                                       //创建一个词法分析器
    textLexer -> setColor(QColor(Qt:: yellow),QsciLexerCPP::CommentLine);             //设置自带的注释行为绿色
    editor -> setLexer(textLexer);                                                    //给QsciScintilla设置词法分析器

//代码提示
    QsciAPIs *apis = new QsciAPIs(textLexer);
    if(!apis -> load(QString(":/api.txt")))
    {
        QMessageBox::warning(this,QString("提示"),QString("读取文件失败"));
    }
    else
    {
        apis->prepare();
    }

    editor -> setAutoCompletionSource(QsciScintilla::AcsAll);                         //设置源，自动补全所有地方出现的
    editor -> setAutoCompletionCaseSensitivity(true);                                 //设置自动补全大小写敏感
    editor -> setAutoCompletionThreshold(1);                                          //设置每输入一个字符就会出现自动补全的提示

//行号显示区域
    editor -> setMarginType(0,QsciScintilla::NumberMargin);                           //设置编号为0的页边显示行号。
    editor -> setMarginLineNumbers(0,true);                                           //对该页边启用行号
    editor -> setMarginWidth(0,20);                                                   //设置页边宽度

//自动折叠区域
    editor -> setMarginType(3, QsciScintilla::SymbolMargin);
    editor -> setMarginLineNumbers(3, false);
    editor -> setMarginWidth(3, 15);
    editor -> setMarginSensitivity(3, true);

//设置自动缩进
    editor -> setAutoIndent(true);

//显示选中行号
    editor -> setCaretLineVisible(true);
    editor -> setCaretLineBackgroundColor(Qt::lightGray);

//显示行号背景颜色
    editor -> setMarginsBackgroundColor(Qt::gray);
    editor -> setBraceMatching(QsciScintilla::SloppyBraceMatch);                      //括号匹配

//设置编码为UTF-8
    editor->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,QsciScintilla::SC_CP_UTF8);

    QAction *action = new QAction(this);

//设置触发QAction对象的快捷操作.
    action->setShortcut(Qt::CTRL + Qt::Key_K);

//把这个QAction的对象加入到当前窗口中去.

    editor->addAction(action);
//连接信号与槽.连接好了以后，当你按下ctrl+s时，就会调用槽函数，也就是这里自定义的messageSlot()函数;

    connect(action,SIGNAL(triggered()),editor,SLOT(commentSlot()));


}

void MainWindow::openFileSlot()                                                     //打开文件槽函数
{

    QString fileName = QFileDialog::getOpenFileName(this,"Open File","","C/C++文件(*.c *.cpp *.h);;C文件(*.c *.h);;所有文件(*.*)");
    if(fileName == "")
    {
        QMessageBox::information(this,"Warning !","Please select a file !");
        return ;
    }
    else
    {
        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::warning(this,"Error !","Cannot open this file !");
            return;
        }
        else
        {
            if(!file.isReadable())
            {
                QMessageBox::warning(this,"Error !","This file is not readable !");
            }
            else
            {
                QString filetext = "";
                int indexNum = tabWidget->count();
                if(indexNum)
                {
                    filetext = EDITOR -> text();
                    if(filetext == "")
                    {
                        int index = tabWidget->currentIndex();
                        QString fileNamePro = QFileInfo(fileName).fileName();
                        tabWidget->setTabText(index,fileNamePro);
                        fileNameVector.pop_back();
                        fileNameVector.append(fileName);
                    }
                    else
                    {
                        QString fileNamePro = QFileInfo(fileName).fileName();
                        QsciScintilla *editor = new QsciScintilla;
                        tabWidget -> setCurrentIndex(tabWidget -> addTab(editor,fileNamePro));
                        tabWidget -> setCurrentWidget(editor);
                        fileNameVector.append(fileName);
                        fileNumber++;
                    }
                }
                else
                {
                    QString fileNamePro = QFileInfo(fileName).fileName();
                    QsciScintilla *editor = new QsciScintilla;
                    tabWidget -> setCurrentIndex(tabWidget -> addTab(editor,fileNamePro));
                    tabWidget -> setCurrentWidget(editor);
                    fileNameVector.append(fileName);
                    fileNumber++;
                }
                QTextStream textStream(&file);
                while(!textStream.atEnd())
                {
                    EDITOR -> setText(textStream.readAll());
                }
                file.close();
            }
        }
    }

}

bool MainWindow::closeFileSlot()                                                    //关闭文件槽函数
{

    int index = tabWidget -> currentIndex();
    closeFile(index);

}

void MainWindow::closeFile(int index)                                               //关闭文件实际操作函数
{

    saveFile(index);
    fileNameVector.remove(index);
    tabWidget -> removeTab(index);
    fileNumber--;
    return;

}

bool MainWindow::closeAllFileSlot()                                                 //全部关闭槽函数
{

    int indexNum = fileNumber;
    while(indexNum--)
    {
        saveFile(indexNum);
        fileNameVector.remove(indexNum);
        tabWidget -> removeTab(indexNum);
        fileNumber--;
    }

}

bool MainWindow::saveFileSlot()                                                     //保存槽函数
{

    int index = tabWidget -> currentIndex();
    saveFile(index);

}

void MainWindow::saveFile(int index)                                                //保存实际操作函数
{

    QString fileName = fileNameVector[index];
    QFileInfo fileInfo(fileName);
    if(!fileInfo.isFile())
    {
        this -> saveAsFile(index);
        return;
    }
    else
    {
        QFile *file = new QFile;
        file->setFileName(fileName);
        if(file -> open(QIODevice::WriteOnly))
        {
            QTextStream out(file);
            out << EDITOR -> text();
            file->close();
            delete file;
        }
        else
        {
            QMessageBox::information(this,"Error !","Please change your dir !");
            this->saveAsFileSlot();
            return;
        }
    }

}

bool MainWindow::saveAsFileSlot()                                                   //另存为槽函数
{

    int index = tabWidget -> currentIndex();
    saveAsFile(index);

}

void MainWindow::saveAsFile(int index)                                              //另存为实际操作函数
{

    QString fileName = QFileDialog::getSaveFileName(this,"Open File","","C++源文件(*.cpp);;C源文件(*.c);;所有文件(*.*)");
    if(fileName.isEmpty())
    {
        QMessageBox::information(this,"Warning !","Please select a file !");
        return;
    }

    QFile *file = new QFile;
    QString fileNamePro;
    file -> setFileName(fileName);
    if(file -> open(QIODevice::WriteOnly))
    {
        fileNamePro = fileName;
        fileNameVector.replace(index,fileNamePro);
        QString fileNameNow = QFileInfo(fileNamePro).fileName();
        tabWidget -> setTabText(index,fileNameNow);
        QTextStream out(file);
        out << EDITOR -> text();
        file->close();
        delete file;
    }
    else
    {
        QMessageBox::information(this,"Error !","Cannot save this file !");
        return;
    }

}

bool MainWindow::saveAllFileSlot()                                                  //全部保存槽函数
{

    int indexNum = fileNumber;
    while(indexNum --)
    {
        saveFile(indexNum);
    }
    QMessageBox::information(this,"Success!","Files have all saved !");

}

void MainWindow::newFileSlot()                                                      //新建文件槽函数
{

    QsciScintilla *editor = new QsciScintilla;
    QString fileName = tr("New %1").arg(++fileNumber);
    tabWidget -> setCurrentIndex(tabWidget -> addTab(editor,fileName));
    tabWidget -> setCurrentWidget(editor);
    fileNameVector.append(fileName);

    QsciLexerCPP *textLexer = new QsciLexerCPP;                                       //创建一个词法分析器
    textLexer -> setColor(QColor(Qt:: yellow),QsciLexerCPP::CommentLine);             //设置自带的注释行为绿色
    editor -> setLexer(textLexer);                                                    //给QsciScintilla设置词法分析器

//代码提示
    QsciAPIs *apis = new QsciAPIs(textLexer);
    if(!apis ->load(QString(":/api.txt")))
    {
        QMessageBox::warning(this,QString("提示"),QString("读取文件失败"));
    }
    else
    {
        apis->prepare();
    }

    editor -> setAutoCompletionSource(QsciScintilla::AcsAll);                         //设置源，自动补全所有地方出现的
    editor -> setAutoCompletionCaseSensitivity(true);                                 //设置自动补全大小写敏感
    editor -> setAutoCompletionThreshold(1);                                          //设置每输入一个字符就会出现自动补全的提示

//行号显示区域
    editor -> setMarginType(0,QsciScintilla::NumberMargin);                           //设置编号为0的页边显示行号。
    editor -> setMarginLineNumbers(0,true);                                           //对该页边启用行号
    editor -> setMarginWidth(0,20);                                                   //设置页边宽度

//自动折叠区域
    editor -> setMarginType(3, QsciScintilla::SymbolMargin);
    editor -> setMarginLineNumbers(3, false);
    editor -> setMarginWidth(3, 15);
    editor -> setMarginSensitivity(3, true);

//设置自动缩进
    editor -> setAutoIndent(true);

//显示选中行号
    editor -> setCaretLineVisible(true);
    editor -> setCaretLineBackgroundColor(Qt::lightGray);

//显示行号背景颜色
    editor -> setMarginsBackgroundColor(Qt::gray);
    editor -> setBraceMatching(QsciScintilla::SloppyBraceMatch);                      //括号匹配

//设置编码为UTF-8
    editor->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,QsciScintilla::SC_CP_UTF8);

    QAction *action = new QAction(this);
//设置触发QAction对象的快捷操作.

    action->setShortcut(Qt::CTRL + Qt::Key_K);
//把这个QAction的对象加入到当前窗口中去.

    editor->addAction(action);
//连接信号与槽.连接好了以后，当你按下ctrl+s时，就会调用槽函数，也就是这里自定义的messageSlot()函数;

    connect(action,SIGNAL(triggered()),editor,SLOT(commentSlot()));

}


void MainWindow::createAction()                                                     //创建操作
{

//文件操作定义
    openFileAction = new QAction(QIcon(":/images/open.ico"),tr("打开"),this);
    openFileAction -> setShortcut(QKeySequence::Open);
    openFileAction -> setStatusTip(tr("打开一个文件"));
    connect(openFileAction,SIGNAL(triggered()),this,SLOT(openFileSlot()));

    newFileAction = new QAction(QIcon(":/images/new.ico"),tr("新建"),this);
    newFileAction -> setShortcut(QKeySequence::New);
    newFileAction -> setStatusTip(tr("新建一个文件"));
    connect(newFileAction,SIGNAL(triggered()),this,SLOT(newFileSlot()));

    saveFileAction = new QAction(QIcon(":/images/save.ico"),tr("保存"),this);
    saveFileAction -> setShortcut(QKeySequence::Save);
    saveFileAction -> setStatusTip(tr("保存当前编辑页的文本到源地址"));
    connect(saveFileAction,SIGNAL(triggered()),this,SLOT(saveFileSlot()));

    saveAsFileAction = new QAction(QIcon(":/images/saveAs.ico"),tr("另存为"),this);
    saveAsFileAction -> setShortcut(QKeySequence::SaveAs);
    saveAsFileAction -> setStatusTip(tr("保存当前编辑页的文本到另一地址"));
    connect(saveAsFileAction,SIGNAL(triggered()),this,SLOT(saveAsFileSlot()));

    saveAllFileAction = new QAction(QIcon(":/images/saveAll.ico"),tr("全部保存"),this);
    saveAllFileAction -> setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    saveAllFileAction -> setStatusTip(tr("保存所有编辑页的文本"));
    connect(saveAllFileAction,SIGNAL(triggered()),this,SLOT(saveAllFileSlot()));

    closeFileAction = new QAction(QIcon(":/images/closeFile.ico"),tr("关闭"),this);
    closeFileAction -> setShortcut(QKeySequence::Close);
    closeFileAction -> setStatusTip(tr("关闭当前编辑页"));
    connect(closeFileAction,SIGNAL(triggered()),this,SLOT(closeFileSlot()));

    closeAllFileAction = new QAction(QIcon(":/images/close.ico"),tr("全部关闭"),this);
    closeAllFileAction -> setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_X);
    closeAllFileAction -> setStatusTip(tr("关闭所有的编辑页"));
    connect(closeAllFileAction,SIGNAL(triggered()),this,SLOT(closeAllFileSlot()));

    exitFileAction = new QAction(QIcon(":/images/exit.ico"),tr("退出"),this);
    exitFileAction -> setShortcut(QKeySequence::Quit);
    exitFileAction -> setStatusTip(tr("退出当前编辑器"));
    connect(exitFileAction,SIGNAL(triggered()),this,SLOT(close()));


//编辑操作定义
    undoAction = new QAction(QIcon(":/images/undo.png"),tr("撤销"),this);
    undoAction -> setShortcut(QKeySequence::Undo);
    undoAction -> setStatusTip(tr("撤回上一步进行的操作"));
    connect(undoAction,SIGNAL(triggered()),this,SLOT(undoSlot()));

    redoAction = new QAction(QIcon(":/images/redo.png"),tr("恢复"),this);
    redoAction -> setShortcut(QKeySequence::Redo);
    redoAction -> setStatusTip(tr("恢复上一步被撤销的操作"));
    connect(redoAction,SIGNAL(triggered()),this,SLOT(redoSlot()));

    copyAction = new QAction(QIcon(":/images/copy.ico"),tr("复制"),this);
    copyAction -> setShortcut(QKeySequence::Copy);
    copyAction -> setStatusTip(tr("复制选中文本到剪贴板"));
    connect(copyAction,SIGNAL(triggered()),this,SLOT(copySlot()));

    cutAction = new QAction(QIcon(":/images/cut.ico"),tr("剪切"),this);
    cutAction -> setShortcut(QKeySequence::Cut);
    cutAction -> setStatusTip(tr("复制选中文本到剪贴板并删除"));
    connect(cutAction,SIGNAL(triggered()),this,SLOT(cutSlot()));

    pasteAction = new QAction(QIcon(":/images/paste.ico"),tr("粘贴"),this);
    pasteAction -> setShortcut(QKeySequence::Paste);
    pasteAction -> setStatusTip(tr("将剪贴板内的文本粘贴到光标所在位置"));
    connect(pasteAction,SIGNAL(triggered()),this,SLOT(pasteSlot()));

    selectAllAction = new QAction(QIcon(":/images/select.png"),tr("全选"),this);
    selectAllAction -> setShortcut(QKeySequence::SelectAll);
    selectAllAction -> setStatusTip(tr("选中当前编辑页内所有的文本"));
    connect(selectAllAction,SIGNAL(triggered()),this,SLOT(selectAllSlot()));


//搜索操作定义
    searchAction = new QAction(QIcon(":/images/search.ico"),tr("查找/替换"),this);
    searchAction -> setStatusTip(tr("查找与输入文本相同的文本的位置并进行替换操作"));
    connect(searchAction,SIGNAL(triggered()),this,SLOT(searchSlot()));


//视图操作定义
    toolBarAction = new QAction(QIcon(":/images/tool.ico"),tr("显示工具栏"),this);
    toolBarAction -> setStatusTip(tr("显示或关闭工具栏"));
    connect(toolBarAction,SIGNAL(triggered()),this,SLOT(judgeToolBars()));


//工具操作定义
    compileAction = new QAction(QIcon(":/images/compile.ico"),tr("编译"),this);
    compileAction -> setShortcut(Qt::Key_F9);
    compileAction -> setStatusTip(tr("对当前编辑页内C语言源码进行编译"));
    connect(compileAction,SIGNAL(triggered()),this,SLOT(complieSlot()));

    runAction = new QAction(QIcon(":/images/run.ico"),tr("运行"),this);
    runAction -> setShortcut(Qt::Key_F10);
    runAction -> setStatusTip(tr("运行当前编辑页内C语言源码"));
    connect(runAction,SIGNAL(triggered()),this,SLOT(runSlot()));

    compileRunAction = new QAction(QIcon(":/images/compile.ico"),tr("编译运行"),this);
    compileRunAction -> setShortcut(Qt::Key_F11);
    compileRunAction -> setStatusTip(tr("编译并运行当前编辑页内C语言源码"));
    connect(compileRunAction,SIGNAL(triggered()),this,SLOT(complieRunSlot()));


//帮助操作定义
    helpDocAction = new QAction(QIcon(":/images/help.ico"),tr("帮助文档"),this);
    helpDocAction -> setStatusTip(tr("展示提示如何操作本软件的文本"));
    connect(helpDocAction,SIGNAL(triggered()),this,SLOT(helpDocSlot()));


    aboutAction = new QAction(QIcon(":/images/about.ico"),tr("关于"),this);
    aboutAction -> setStatusTip(tr("展示软件的版本信息及团队介绍"));
    connect(aboutAction,SIGNAL(triggered()),this,SLOT(aboutSlot()));


}

void MainWindow::createMenuBar()                                                    //创建菜单栏
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


//搜索菜单
    searchMenu = menuBar() -> addMenu(tr("搜索"));
    searchMenu -> addAction(searchAction);


//视图菜单
    viewMenu = menuBar() -> addMenu(tr("视图"));
    viewMenu -> addAction(toolBarAction);


//工具菜单
    toolMenu = menuBar() -> addMenu(tr("工具"));
    toolMenu -> addAction(runAction);
    toolMenu -> addAction(compileAction);
    toolMenu -> addAction(compileRunAction);


//帮助菜单
    helpMenu = menuBar() -> addMenu(tr("帮助"));
    helpMenu -> addAction(helpDocAction);
    helpMenu -> addAction(aboutAction);


}

void MainWindow::createToolBar()                                                    //创建工具栏
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


//搜索工具
    toolBar -> addAction(searchAction);


//编译工具
    toolBar -> addAction(runAction);
    toolBar -> addAction(compileAction);
    toolBar -> addAction(compileRunAction);
    toolBar -> addSeparator();


//调试工具
    toolBar -> addAction(runAction);
    toolBar -> addAction(compileAction);
    toolBar -> addAction(compileRunAction);
    toolBar -> addSeparator();


//帮助工具
    toolBar -> addAction(helpDocAction);
    toolBar -> addAction(aboutAction);


}

void MainWindow::judgeToolBars()                                                    //判断当前工具栏的显示状态
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

void MainWindow::hideToolBars()                                                     //如果当前工具栏是显示状态则隐藏
{

    toolBar -> setVisible(false);

}

void MainWindow::showToolBars()                                                     //如果当前工具栏是隐藏状态则显示
{

    toolBar -> setVisible(true);

}

void MainWindow::cutSlot()                                  //剪切槽函数
{

    EDITOR -> cut();

}

void MainWindow::copySlot()                                 //复制槽函数
{

    EDITOR -> copy();

}

void MainWindow::undoSlot()                                 //撤销槽函数
{

    EDITOR -> undo();

}

void MainWindow::redoSlot()                                 //恢复槽函数
{

    EDITOR -> redo();

}

void MainWindow::pasteSlot()                                //粘贴槽函数
{

    EDITOR -> paste();

}

void MainWindow::selectAllSlot()                            //全选槽函数
{

    EDITOR -> selectAll();

}

void MainWindow::searchSlot()                               //查找替换槽函数
{

    //窗口内容的构建
    QDialog *findDlg = new QDialog(this);
    QLineEdit *findLine = new QLineEdit(findDlg);
    QLineEdit *replaceLine = new QLineEdit(findDlg);
    QPushButton *findNext = new QPushButton(findDlg);
    QPushButton *replaceThis = new QPushButton(findDlg);


    //窗口布局设置
    findDlg -> setWindowTitle(" 查找  /  替换 ");
    findDlg -> resize(300,200);
    findDlg -> setMaximumSize(300,200);
    findDlg -> setMinimumSize(300,200);
    findLine -> move(25,50);
    replaceLine -> move(25,100);
    findLine -> resize(250,30);
    replaceLine -> resize(250,30);
    findLine -> setPlaceholderText("Input the word you want to search");
    replaceLine -> setPlaceholderText("Input the word you want to Replace with");


    //控件外观设置
    findNext -> move(60,150);
    replaceThis -> move(170,150);
    findNext -> resize(85,25);
    replaceThis -> resize(75,25);
    findNext -> setText("寻找下一个");
    replaceThis -> setText("替换");


    connect(findNext,&QPushButton::clicked,
            [=]()
            {
                QString Str = findLine->text();
                QString allText = editor->text();
                QString toBeReplace = replaceLine->text();
                qDebug()<<"Str:"<<Str;
                qDebug()<<"allText:"<<allText;
                editor->findFirst(Str,true,false,true,true);
            }
            );
    connect(replaceThis,&QPushButton::clicked,
            [=]()
            {
                QString Str = findLine->text();
                QString allText = editor->text();
                QString toBeReplace = replaceLine->text();
                qDebug()<<"Str:"<<Str;
                qDebug()<<"allText:"<<allText;
                editor -> replace(toBeReplace);
            }
            );

//查询功能实现
    findDlg -> show();
    findDlg -> exec();

}

void MainWindow::aboutSlot()                                    //关于槽函数
{
    QDialog helpDocWin1;

    QTextEdit *helpDocShow = new QTextEdit(&helpDocWin1);
    helpDocShow -> resize(600,400);
    helpDocWin1.resize(600,400);
    helpDocWin1.setMaximumSize(600,400);
    helpDocWin1.setMinimumSize(600,400);
    helpDocWin1.setWindowIcon(QIcon(":/images/Logo.ico"));
    helpDocWin1.setWindowTitle(tr("  About  us  "));

    //调节背景颜色
    QColor clr(255, 246, 240);
    QString strClr = clr.name();
    QString strStyleSheet = QString("background-color: %1").arg( strClr );
    helpDocShow->setStyleSheet( strStyleSheet );

    helpDocShow -> setReadOnly(true);
    helpDocShow -> setFontPointSize(15);
    helpDocShow -> setFontWeight(50);
    helpDocShow -> setAlignment(Qt::AlignCenter);
    helpDocShow -> setFont(QFont(tr("Cambria")));

    helpDocShow -> append("^_^");
    helpDocShow -> append("");
    helpDocShow -> append("");
    helpDocShow -> append("Welcome to our IDE:");
    helpDocShow -> append("");
    helpDocShow -> append("We are appreciate that you choose our product.");
    helpDocShow -> append("");
    helpDocShow -> append("Our IDE offers text editing ");
    helpDocShow -> append("");
    helpDocShow -> append("and compiling of C Programming Language.");
    helpDocShow -> append("");
    helpDocShow -> append("");helpDocShow->append("");
    helpDocShow -> append("STAFF:");
    helpDocShow -> append("");
    helpDocShow -> append("YueHun");
    helpDocShow -> append("");helpDocShow->append("Xu Zhaoliang");
    helpDocShow -> append("");helpDocShow->append("Feng Yuxiao");
    helpDocShow -> append("");helpDocShow->append("Wen GuangBing");
    helpDocShow -> append("");helpDocShow->append("Lv Jiaming");
    helpDocShow -> append("");helpDocShow->append("");
    helpDocShow -> append("");helpDocShow->append("We create this in ths summer of 2019");
    helpDocShow -> append("");helpDocShow->append("and hope that it can give you some convince.");
    helpDocShow -> append("");helpDocShow->append("");
    helpDocShow -> append("Thanks for your interviewing!");
    helpDocShow -> append("");helpDocShow->append("");helpDocShow->append("");

    helpDocShow->moveCursor(QTextCursor::Start);

    helpDocWin1.show();
    helpDocWin1.exec();

}

void MainWindow::helpDocSlot()                              //帮助槽函数
{
    QDialog helpDocWin1;

    QTextEdit *helpDocShow = new QTextEdit(&helpDocWin1);
    helpDocShow -> resize(600,400);
    helpDocWin1.resize(600,400);
    helpDocWin1.setMaximumSize(600,400);
    helpDocWin1.setMinimumSize(600,400);
    helpDocWin1.setWindowIcon(QIcon(":/images/Logo.ico"));
    helpDocWin1.setWindowTitle(tr("  HelpDoc  "));

    //调节背景颜色
    QColor clr(255, 246, 240);
    QString strClr = clr.name();
    QString strStyleSheet = QString("background-color: %1").arg( strClr );
    helpDocShow -> setStyleSheet( strStyleSheet );


    helpDocShow -> setReadOnly(true);
    helpDocShow -> setFontPointSize(15);
    helpDocShow -> setFontWeight(50);
    helpDocShow -> setFont(QFont(tr("Cambria")));

    helpDocShow -> append("Welcome to our IDE");
    helpDocShow -> setAlignment(Qt::AlignCenter);
    helpDocShow -> append("This page can help you to use our product");
    helpDocShow -> setAlignment(Qt::AlignCenter);
    helpDocShow -> append("");
    helpDocShow -> append("");
    helpDocShow -> append("文件菜单栏：");
    helpDocShow -> setAlignment(Qt::AlignLeft);
    helpDocShow -> append("");
    helpDocShow -> append("打开(Ctrl+O)：打开一个文件或项目");
    helpDocShow -> append("新建(Ctrl+N)：新建一个文件");
    helpDocShow -> append("保存(Ctrl+S)：保存当前编辑的文件");
    helpDocShow -> append("全部保存(Ctrl+Shift+S)：保存当前打开的所有文件");
    helpDocShow -> append("另存为：将当前正在编辑的文本保存在新的文件中");
    helpDocShow -> append("关闭(Ctrl+W)：关闭当前正在编辑的文件");
    helpDocShow -> append("全部关闭(Ctrl+Shift+W)：关闭当前打开的全部文件");
    helpDocShow -> append("最近打开文件：显示最近所打开过的所有文件");
    helpDocShow -> append("退出(Ctrl+Q)：退出集成开发环境");
    helpDocShow -> append("");
    helpDocShow -> append("编辑菜单栏：");
    helpDocShow -> append("撤销(Ctrl+Z)：撤销上一步进行的操作");
    helpDocShow -> append("恢复(Ctrl+Shift+Z)：恢复上一步被撤销的操作");
    helpDocShow -> append("复制(Ctrl+C)：复制选中的文本到粘贴板");
    helpDocShow -> append("剪切(Ctrl+X)：将选中的文本添加到粘贴板并从当前移除");
    helpDocShow -> append("粘贴(Ctrl+V)：将粘贴板中的文本粘贴到光标所在位置");
    helpDocShow -> append("删除(Del)：删除所选中的文本内容");
    helpDocShow -> append("全选(Ctrl+A)：选中当前编辑文本中的所有文本");
    helpDocShow -> append("");
    helpDocShow -> append("搜索菜单栏：");
    helpDocShow -> append("查找(Ctrl+F)：查找与选中或输入文本相同的文本并显示");
    helpDocShow -> append("替换(Ctrl+H)：将搜索到的文本替换为输入的文本");
    helpDocShow -> append("");
    helpDocShow -> append("视图菜单栏：");
    helpDocShow -> append("显示工具栏：显示或关闭工具栏");
    helpDocShow -> append("");
    helpDocShow -> append("工具菜单栏：");
    helpDocShow -> append("编译(F9)：对当前编辑文件中的文本进行编译");
    helpDocShow -> append("运行(F10)：运行文本中的C语言源代码");
    helpDocShow -> append("编译运行(F11)：对当前编辑文件中的文本进行编译并运行文本中的源代码");
    helpDocShow -> append("");
    helpDocShow -> append("帮助菜单栏：");
    helpDocShow -> append("帮助文档：打开记录软件使用方法的文档");
    helpDocShow -> append("关于：打开记录软件版本和团队信息的文档");

    helpDocShow->moveCursor(QTextCursor::Start);

    helpDocWin1.show();
    helpDocWin1.exec();

}

void MainWindow::commentSlot()
{
    QMessageBox::information(this, "z", "q");

//获取必要的参数
//空白行的处理

    bool commentEmptyLines = true;
//获取选择注释的开始位置
    int selectionStart = editor -> SendScintilla(QsciScintillaBase::SCI_GETSELECTIONSTART);

//获取选择注释的结束位置
    int selectionEnd = editor -> SendScintilla(QsciScintillaBase::SCI_GETSELECTIONEND);

//获取选择注释的当前位置
    int caretPosition = editor -> SendScintilla(QsciScintillaBase::SCI_GETCURRENTPOS);

//当前的光标位置 是否在选择内
    bool move_caret = caretPosition < selectionEnd;

//获取要注释的行号
    int selStartLine = editor -> SendScintilla(QsciScintillaBase::SCI_LINEFROMPOSITION,selectionStart);
    int selEndLine = editor -> SendScintilla(QsciScintillaBase::SCI_LINEFROMPOSITION,selectionEnd);

//计算注释行数
    int lines = selEndLine - selStartLine;

//处理结束行
    if((lines > 0)&& (selectionEnd == editor -> SendScintilla(QsciScintillaBase::SCI_POSITIONFROMLINE,selEndLine)))
    {
        selEndLine-- ;
    }

//记录操作，方便实现 redo undo功能
    editor -> SendScintilla(QsciScintillaBase::SCI_BEGINUNDOACTION);

//注释每一行
//遍历行
    for(int i = selStartLine; i <= selEndLine ; ++i)
    {
    //获取注释的行信息
        int lineStart  = editor -> SendScintilla(QsciScintillaBase::SCI_POSITIONFROMLINE,i);
        int lineIndent = editor -> SendScintilla(QsciScintillaBase::SCI_GETLINEINDENTPOSITION,i);
        int lineEnd    = editor -> SendScintilla(QsciScintillaBase::SCI_GETLINEENDPOSITION,i);
        if(lineIndent == lineEnd && !commentEmptyLines)
        {
            continue;
        }

//要注释的行前面的空格处理
        lineIndent = lineStart;
        int lineBufferSize = lineEnd - lineIndent +1;
        char *buf = new char[lineBufferSize];

//获取注释范围
        editor -> SendScintilla(QsciScintillaBase::SCI_GETTEXTRANGE, lineIndent, lineEnd,buf);

//添加注释 “//”
        editor -> SendScintilla(QsciScintillaBase::SCI_INSERTTEXT, lineIndent, "//");
    }

//完成记录
    editor -> SendScintilla(QsciScintillaBase::SCI_ENDUNDOACTION);

}

void MainWindow::closeEvent(QCloseEvent *event)                                     //重写关闭事件
{

    closeAllFileSlot();
    event -> accept();

}

void MainWindow::complieSlot()
{

    int index = tabWidget -> currentIndex();
    file_Name = fileNameVector[index];

    txt1.setWindowTitle(tr("Compile"));
    txt1.setWindowIcon(QIcon(":/images/Logo.ico"));
    txt1.resize(600,400);
    txt1.setMaximumSize(600,400);
    txt1.setMinimumSize(600,400);

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
    if(file_suffix != "c"&&file_suffix != "cpp")
    {
        txt1.setText("Error!无法编译此文件!");
        txt1.show();
    }
    else
    {
        QStringList name;
        name=file_Name.split(".");
        qDebug()<<name;
        qDebug()<<name[0];
        dealfileName=name[0].split("/");

        qDebug()<<dealfileName;
        QString  s = dealfileName.join("\\");
        qDebug()<<s;
        QString n="g++ "+s+"."+name[1]+" -o "+s+".exe";
        qDebug()<<n;
        const char *cstr;

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
        QByteArray qbt = p.readAllStandardError();
        QTextCodec* pTextCodec = QTextCodec::codecForName("System");
        assert(pTextCodec != nullptr);
        QString msg = pTextCodec->toUnicode(qbt);


        qDebug()<<msg;
        if(msg != nullptr)
        {
            txt1.setText(msg);
            txt1.show();
        }
        else
        {
            txt1.setText("Success!此文件编译成功!");
            txt1.show();
        }

    }

}

void MainWindow::complieRunSlot()
{

    int index = tabWidget -> currentIndex();
    file_Name = fileNameVector[index];

    dealfileName.clear();
    fileinfo = QFileInfo(file_Name);

    txt1.setWindowTitle(tr("compile_and_run"));

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

    txt1.setWindowTitle(tr("Compile"));
    txt1.setWindowIcon(QIcon(":/images/Logo.ico"));

    if(file_suffix != "c"&&file_suffix != "cpp")
    {
        txt1.setText("Sussess!无法编译此文件!");
        txt1.show();
    }
    else
    {
        QStringList name;
        name = file_Name.split(".");
        qDebug()<<name;
        qDebug()<<name[0];
        dealfileName = name[0].split("/");

        qDebug()<<dealfileName;
        QString  s = dealfileName.join("\\");
        qDebug()<<s;
        QString n="g++ "+s+"."+name[1]+" -o "+s+".exe";
        const char *cstr;
        QByteArray bytes = n.toLocal8Bit();
        cstr = bytes.data();
        qDebug()<<cstr;
        QString temp = "start "+s+".exe";
        const char *run_char;
        QByteArray bytes1 = temp.toLocal8Bit();
        run_char = bytes1.data();
        qDebug()<<run_char;
        QProcess p;
        p.start("cmd.exe",QProcess::ReadWrite);
        p.waitForStarted();
        p.write(cstr);
        p.write("\n");
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

void MainWindow::runSlot()
{

    int index = tabWidget -> currentIndex();
    file_Name = fileNameVector[index];

    txt1.setWindowTitle(tr("Compile"));
    txt1.setWindowIcon(QIcon(":/images/Logo.ico"));

    dealfileName.clear();
    QStringList name;
    name = file_Name.split(".");
    qDebug()<<name;
    qDebug()<<name[0];
    dealfileName = name[0].split("/");
    qDebug()<<dealfileName;
    QString  s=dealfileName.join("\\");
    qDebug()<<s;
    QString n = "dir /a "+s+"."+"exe";
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
    QByteArray qbt = p.readAllStandardError();
    QString msg = QString::fromLocal8Bit(qbt);
    qDebug()<<msg;
    if(msg != "")
    {
        txt1.setText(msg);
        txt1.show();
    }
    else
    {
        QString temp = "start "+s+"."+"exe";
        const char *run_char;
        QByteArray bytes1 = temp.toLocal8Bit();
        run_char = bytes1.data();
        qDebug()<<run_char;
        QProcess q;
        q.start("cmd.exe",QProcess::ReadWrite);
        q.waitForStarted();
        q.write(run_char);
        q.write("\n");
        q.closeWriteChannel();
        q.waitForFinished();
        p.kill();
        qDebug()<<q.readAllStandardOutput();
        QByteArray qbt = q.readAllStandardError();
        QString msg = QString::fromLocal8Bit(qbt);
        qDebug()<<msg;
    }

}
