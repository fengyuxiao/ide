#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "showwidget.h"

#include <QFont>
#include <QFontDialog>
#include <QFontDatabase>
#include <QFontComboBox>
#include <QPlainTextEdit>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintEngine>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QTextDocumentFragment>
#include <QTextDocumentWriter>
#include <QTextStream>
#include <QTextEdit>
#include <QTextCharFormat>
#include <QMenuBar>
#include <QToolBar>
#include <QToolButton>
#include <QSpinBox>
#include <QFileDialog>
#include <QColorDialog>
#include <QComboBox>
#include <QMessageBox>
#include <QTabWidget>
#include <QKeySequence>
#include <QKeySequenceEdit>
#include <QKeyEvent>
#include <QKeyEventTransition>
#include <QActionGroup>
#include <QAction>
#include <QMimeData>
#include <QClipboard>
#include <QUndoCommand>
#include <QDialog>
#include <QEvent>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QFont>
#include <QMenu>
#include <QToolTip>
#include <QAction>
#include <QPainter>

MainWindow::MainWindow(Settings *settings,QWidget *parent) :
    QMainWindow(parent), settings(settings),
  ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createinit();                                                                     //初始化tabwidget属性
    createTab();                                                                      //创建tabwidget
    createAction();                                                                   //创建各个操作
    createMenuBar();                                                                  //创建菜单栏
    createToolBar();                                                                  //创建工具栏

    currentChanged(-1);                                                               //在tabwidget上建立可编辑text
    currentChanged(0);

    createEditor();                                                                   //建立编辑操作的connect
    setCentralWidget(tabWidget);

    restoreGeometry(settings->mainWindowsGeometry);                                   //保存用户的主界面设置
    restoreState(settings->mainWindowState);

    setWindowTitle(tr("C语言集成开发环境"));
    this -> setWindowIcon(QIcon("Logo.ico"));                                         //为软件设置图标
    this -> resize(1300,900);

    messageLabel = new QLabel;                                                         //信息标签
    messageLabel -> setMinimumSize(150,20);
    messageLabel -> setFrameShape(QFrame::WinPanel);
    messageLabel -> setFrameShadow(QFrame::Sunken);
    ui -> statusBar -> addPermanentWidget(messageLabel);
    messageLabel -> setText(tr("   Welcome  To  Quasar  v1.0.1   "));
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
    connect(tabWidget,SIGNAL(currentChanged(int)),this,SLOT(currentChanged(int)));
    connect(tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(fileClose(int)));

}

void MainWindow::createinit()
{

    toolBar = new QToolBar(this);
    fontBar = new QToolBar(this);
    toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    fontBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    addToolBar(toolBar);
    toolBar -> setMovable(false);
    addToolBarBreak();
    addToolBar(fontBar);

    comboFont = new QFontComboBox(fontBar);                                   //设置主窗口字体等属性
    comboFont->setCurrentFont(QFont(settings->fontFamily));
    fontBar->addWidget(comboFont);
    comboStyle = new QComboBox(fontBar);
    comboStyle->addItem("Normal");
    comboStyle->addItem("Bold");
    comboStyle->addItem("Italic");
    comboStyle->addItem("Bold Italic");
    comboStyle->setCurrentIndex(comboStyle->findText(settings->fontStyle));
    fontBar->addWidget(comboStyle);
    comboSize = new QComboBox(fontBar);
    comboSize->setEditable(true);

    QFontDatabase db;
    foreach(int size, db.standardSizes())
        comboSize->addItem(QString::number(size));
    comboSize->setCurrentIndex(comboSize->findText(tr("%1").arg(settings->fontSize)));

    fontBar->addWidget(comboSize);
    fontBar -> setVisible(false);

}


void MainWindow::createAction()
{

//文件操作定义
    openFileAction = new QAction(QIcon(":/images/open.ico"),tr("打开"),this);
    openFileAction -> setShortcut(QKeySequence::Open);
    openFileAction -> setStatusTip(tr("打开选中的文件并显示文本内容"));
    connect(openFileAction,SIGNAL(triggered()),this,SLOT(openFile()));

    newFileAction = new QAction(QIcon(":/images/new.ico"),tr("新建"),this);
    newFileAction -> setShortcut(QKeySequence::New);
    newFileAction -> setStatusTip(tr("新建一个文件并编辑文本内容"));
    connect(newFileAction,SIGNAL(triggered()),this,SLOT(newFile()));

    saveFileAction = new QAction(QIcon(":/images/save.ico"),tr("保存"),this);
    saveFileAction -> setShortcut(QKeySequence::Save);
    saveFileAction -> setStatusTip(tr("保存当前正在编辑页的文本到原文件"));
    connect(saveFileAction,SIGNAL(triggered()),this,SLOT(fileSave()));

    saveAsFileAction = new QAction(QIcon(":/images/saveAs.ico"),tr("另存为"),this);
    saveAsFileAction -> setShortcut(QKeySequence::SaveAs);
    saveAsFileAction -> setStatusTip(tr("保存当前正在编辑页的文本到指定文件"));
    connect(saveAsFileAction,SIGNAL(triggered()),this,SLOT(fileSaveAs()));

    saveAllFileAction = new QAction(QIcon(":/images/saveAll.ico"),tr("全部保存"),this);
    saveAllFileAction -> setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    saveAllFileAction -> setStatusTip(tr("保存当前所有编辑页的文本到原文件"));
    connect(saveAllFileAction,SIGNAL(triggered()),this,SLOT(fileSaveAll()));

    closeFileAction = new QAction(QIcon(""),tr("关闭"),this);
    closeFileAction -> setShortcut(QKeySequence::Close);
    closeFileAction -> setStatusTip(tr("关闭当前正在编辑的编辑页"));
    connect(closeFileAction,SIGNAL(triggered()),this,SLOT(fileClose()));

    closeAllFileAction = new QAction(QIcon(":/images/close.ico"),tr("全部关闭"),this);
    closeAllFileAction -> setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_X);
    closeAllFileAction -> setStatusTip(tr("关闭当前打开的所有编辑页"));
    connect(closeAllFileAction,SIGNAL(triggered()),this,SLOT(fileCloseAll()));

    exitFileAction = new QAction(QIcon(":/images/exit.ico"),tr("退出"),this);
    exitFileAction -> setShortcut(QKeySequence::Quit);
    exitFileAction -> setStatusTip(tr("退出当前正在使用的文本编辑器"));
    connect(exitFileAction,SIGNAL(triggered()),this,SLOT(close()));

//编辑操作定义
    undoAction = new QAction(QIcon(":/images/undo.png"),tr("撤销"),this);
    undoAction -> setShortcut(QKeySequence::Undo);
    undoAction -> setStatusTip(tr("将文本内容变为上一次操作前的状态"));

    redoAction = new QAction(QIcon(":/images/redo.png"),tr("恢复"),this);
    redoAction -> setShortcut(QKeySequence::Redo);
    redoAction -> setStatusTip(tr("取消上一个撤销操作"));

    copyAction = new QAction(QIcon(":/images/copy.ico"),tr("复制"),this);
    copyAction -> setShortcut(QKeySequence::Copy);
    copyAction -> setStatusTip(tr("将选中文本添加到剪切板"));

    cutAction = new QAction(QIcon(":/images/cut.ico"),tr("剪切"),this);
    cutAction -> setShortcut(QKeySequence::Cut);
    cutAction -> setStatusTip(tr("将选中文本删除并添加到剪切板"));

    pasteAction = new QAction(QIcon(":/images/paste.ico"),tr("粘贴"),this);
    pasteAction -> setShortcut(QKeySequence::Paste);
    pasteAction -> setStatusTip(tr("在光标后添加剪切板内内容/用剪贴板内本文替换选中文本"));

    selectAllAction = new QAction(QIcon(":/images/select.png"),tr("全选"),this);
    selectAllAction -> setShortcut(QKeySequence::SelectAll);
    selectAllAction -> setStatusTip(tr("将当前编辑页内全部文本选中"));

    goToLineAction = new QAction(QIcon(""),tr("跳转到行"),this);
    goToLineAction -> setShortcut(Qt::CTRL + Qt::Key_G);
    goToLineAction -> setStatusTip(tr("将光标跳转到输入的行号位置"));


//设置操作定义
    fontTypeAction = new QAction(QIcon(":/images/fontType.ico"),tr("文本字体"),this);
    fontTypeAction -> setStatusTip(tr("更改当前编辑页中的文本属性（字体、大小、颜色）"));
    connect(fontTypeAction,SIGNAL(triggered()),SLOT(textFont()));
    connect(comboFont, SIGNAL(activated(QString)),SLOT(textFontFamily(QString)));
    connect(comboStyle,SIGNAL(activated(int)),SLOT(textStyle(int)));
    connect(comboStyle, SIGNAL(activated(QString)),SLOT(updateTextStyleActs(QString)));
    connect(comboSize, SIGNAL(activated(QString)), SLOT(textSize(QString)));

    fontBAction = new QAction(tr("粗体"),this);
    fontIAction = new QAction(tr("斜体"),this);
    fontUAction = new QAction(tr("下划线"),this);
    fontCAction = new QAction(tr("颜色"),this);

//搜索操作定义
    searchAction = new QAction(QIcon(":/images/search.ico"),tr("查找"),this);
    searchAction -> setStatusTip(tr("查找与输入文本相同的所有文本内容"));

    replaceAction = new QAction(QIcon(":/images/replace.ico"),tr("替换"),this);
    replaceAction -> setStatusTip(tr("将查找到的文本替换为指定文本内容"));

//视图操作定义
    toolBarAction = new QAction(QIcon(":/images/tool.ico"),tr("显示工具栏"),this);
    toolBarAction -> setStatusTip(tr("显示/隐藏工具栏"));
    connect(toolBarAction,SIGNAL(triggered()),this,SLOT(judgeToolBars()));


//工具操作定义
    compileAction = new QAction(QIcon(":/images/compile.ico"),tr("编译"),this);
    compileAction -> setShortcut(Qt::Key_F9);
    compileAction -> setStatusTip(tr("编译当前编辑页内的源代码"));

    runAction = new QAction(QIcon(":/images/run.ico"),tr("运行"),this);
    runAction -> setShortcut(Qt::Key_F10);
    runAction -> setStatusTip(tr("运行当前编辑页的源代码"));

    compileRunAction = new QAction(QIcon(":/images/compile.ico"),tr("编译运行"),this);
    compileRunAction -> setShortcut(Qt::Key_F11);
    compileRunAction -> setStatusTip(tr("编译并运行当前编辑页的源代码"));

    compileAllAction = new QAction(QIcon(":/images/compileAll.ico"),tr("全部编译"),this);
    compileAllAction -> setShortcut(Qt::Key_F12);
    compileAllAction -> setStatusTip(tr("编译所有编辑页内的源代码"));

    startDebugAction = new QAction(QIcon(":/images/start.ico"),tr("开始调试"),this);
    startDebugAction -> setShortcut(Qt::Key_F5);
    startDebugAction -> setStatusTip(tr("启动调试器对编辑页源代码调试"));

    endDebugAction = new QAction(QIcon(":/images/end.ico"),tr("结束调试"),this);
    endDebugAction -> setStatusTip(tr("关闭调试器结束调试"));

    setBreakAction = new QAction(QIcon(":/images/set.ico"),tr("设置断点"),this);
    setBreakAction -> setStatusTip(tr("设置断点，程序执行到断点处停下"));

    cancelBreakAction = new QAction(QIcon(":/images/cancel.ico"),tr("取消断点"),this);
    cancelBreakAction -> setStatusTip(tr("取消之前设置的断点"));

    nextLineAction = new QAction(QIcon(":/images/nextL.ico"),tr("下一步"),this);
    nextLineAction -> setShortcut(Qt::Key_F7);
    nextLineAction -> setStatusTip(tr("执行下一行的程序语句"));

    nextBreakAction = new QAction(QIcon(":/images/nextB.ico"),tr("跳到下一个断点"),this);
    nextBreakAction -> setStatusTip(tr("执行到下一个设置的断点处"));

    enterAction = new QAction(QIcon(""),tr("单步进入"),this);
    enterAction -> setStatusTip(tr("进入函数体内部"));

    jumpAction = new QAction(QIcon(""),tr("跳出"),this);
    jumpAction -> setStatusTip(tr("跳出当前函数体"));

    addTrackAction = new QAction(QIcon(":/images/addT.ico"),tr("添加查看"),this);
    addTrackAction -> setStatusTip(tr("添加需要监视的对象"));

//帮助操作定义
    helpDocAction = new QAction(QIcon(":/images/help.ico"),tr("帮助文档"),this);
    helpDocAction -> setStatusTip(tr("显示帮助文档辅助用户使用本软件"));

    aboutAction = new QAction(QIcon(":/images/about.ico"),tr("关于"),this);
    aboutAction -> setStatusTip(tr("显示关于文档向用户介绍软件的版本信息及制作团队信息"));

}

void MainWindow::createMenuBar()
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

//设置菜单
    settingMenu = menuBar() -> addMenu(tr("设置"));
    settingMenu -> addSeparator();
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

    toolBar -> addAction(searchAction);
    toolBar -> addAction(replaceAction);
    toolBar -> addSeparator();

    toolBar -> addAction(runAction);
    toolBar -> addAction(compileAction);
    toolBar -> addAction(compileRunAction);
    toolBar -> addAction(compileAllAction);
    toolBar -> addSeparator();

    toolBar -> addAction(startDebugAction);
    toolBar -> addAction(endDebugAction);
    toolBar -> addAction(setBreakAction);
    toolBar -> addAction(cancelBreakAction);
    toolBar -> addAction(nextLineAction);
    toolBar -> addAction(nextBreakAction);
    toolBar -> addAction(addTrackAction);
    toolBar -> addSeparator();

    toolBar -> addAction(helpDocAction);
    toolBar -> addAction(aboutAction);

}

//设置菜单


void MainWindow::judgeToolBars()                         //判断当前工具栏的显示状态
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

void MainWindow::hideToolBars()                         //如果当前工具栏是显示状态则隐藏
{
    toolBar -> setVisible(false);
}

void MainWindow::showToolBars()                         //如果当前工具栏是隐藏状态则显示
{
    toolBar -> setVisible(true);
}

void MainWindow::closeEvent(QCloseEvent *event)                 //重新定义关闭事件，若只剩一个编辑页则关闭操作变为保存至文件并删除编辑区文本
{
    for (int i = 0; i < tabWidget->count(); i++)
    {
        tabWidget->setCurrentIndex(i);
        if (!maybeSave(i))
        {
            event->ignore();
            return;
        }
    }
    event->accept();
}

void MainWindow::modificationChanged(bool changed)                              //在文本发生改变时更新部分操作的操作内容
{
    QString str = tabWidget->tabText(tabWidget->currentIndex());
    if (str[str.length() - 1] == '*')
    {
        if (!changed)
            str.resize(str.length() - 1);
    }
    else if (changed)
    {
        str += '*';
    }
    tabWidget->setTabText(tabWidget->currentIndex(), str);                      //更新EDITOR的状态
    refreshActions();                                                           //更新save\copy\cut\paste\undo\redo操作
}

void MainWindow::selectionChanged()                                             //当有文本被选中时更新EDITOR的状态
{

    QTextCharFormat fmt=EDITOR->currentCharFormat();
    currentCharFormatChanged(fmt);
}


void MainWindow::currentChanged(int index)                                       //在Tab发生改变时触发的槽
{
    if (index == -1)
    {
        newNumber = 0;
        newFile();
        updateTextStyleActs(settings->fontStyle);                                //更新文本的属性设置
        return;
    }
    updateActions();                                                             //更新各项操作
    setWindowIcon(QIcon("Logo.ico"));
    setWindowTitle(tr("TextTab(%1)").arg(openedFiles.at(index)));
}

void MainWindow::createNewTab(const QString &fileName, QFile& file)              //创建新的tabwidget用于打开文件功能，记录所打开的文件名称并显示
{
    openedFiles << fileName;
    TextTab *texttab = new TextTab(settings);
    tabWidget->addTab(texttab, QFileInfo(fileName).fileName());                  //建立texttab类
    QByteArray data = file.readAll();
    texttab->setPlainText(QString::fromLocal8Bit(data));
    tabWidget->setCurrentWidget(texttab);                                        //使用texttab类来更新tabwidget当前的布局
}

bool MainWindow::maybeSave(int index)                                            //向用户提示当前文本是否需要保存（关闭未保存文本时）
{
    TextTab *textab = static_cast<TextTab *>(tabWidget->widget(index));
    QString fileName = openedFiles.at(index);
    if (!textab->document()->isModified())
        return true;
    if (fileName.startsWith(QLatin1String(":/")))
        return true;
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("TextTab"),tr("The document has been modified.\n"
                                                      "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);          //提供保存、不保存直接关闭、取消关闭操作
    if (ret == QMessageBox::Save)
        return fileSave(index);
    else if (ret == QMessageBox::Cancel)
        return false;
    return true;
}

void MainWindow::updateRecentFilesList()                                        //更新settings中最近打开的文件列表
{

    int index = tabWidget->currentIndex();
    QString fileName = openedFiles.at(index);
    settings->recentFiles.removeAll(fileName);
    settings->recentFiles.prepend(fileName);
    if (settings->recentFiles.size() > settings->maxRecentFiles)
        settings->recentFiles.removeLast();
}

void MainWindow::refreshActions()                                           //更新action各个操作的状态
{

#ifndef QT_NO_CLIPBOARD
    if (const QMimeData *md = QApplication::clipboard()->mimeData())
        pasteAction->setEnabled(md->hasText());
#endif

}

void MainWindow::updateActions()                                                                //更新操作状态
{
    connect(EDITOR, SIGNAL(modificationChanged(bool)), SLOT(modificationChanged(bool)), Qt::UniqueConnection);
    connect(EDITOR,SIGNAL(selectionChanged()),SLOT(selectionChanged()));
    refreshActions();
    updateRecentFilesList();
}

void MainWindow::closeDuplicate(int index)                                      //如果打开已经打开的文件则关闭之前打开的文件
{
    QString fileName = openedFiles.at(index);
    for (int i = 0; i < openedFiles.count(); i++)
    {
        if (openedFiles.at(i) == fileName && i != index)
        {
            openedFiles.removeAt(i);
            tabWidget->removeTab(i);
        }
    }
    int currIndex = openedFiles.indexOf(fileName);
    tabWidget->setCurrentIndex(currIndex);
    setWindowTitle(tr("TextTab(%1)").arg(fileName));
}

void MainWindow::mergeFormatOnWordOrSelection(const QTextCharFormat &format)            //设置文本的格式（字体、颜色、大小）
{
    QTextCursor cursor = EDITOR->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    EDITOR->mergeCurrentCharFormat(format);
}

void MainWindow::openFile(QString fileName)                                           //打开指定文件
{
    int index = openedFiles.indexOf(fileName);
    if (index != -1)
        tabWidget->setCurrentIndex(index);
    else
    {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly))
            createNewTab(fileName, file);
    }
}

void MainWindow::openFile()                                                          //打开文件
{
    QStringList files;
    files = QFileDialog::getOpenFileNames(this, tr("Open files..."), QString(),
                                          tr("All Files(*);;Text Files(*.txt)"));
    QString fileName;
    if (files.count())
    {
        for (int i = 0; i < files.count(); i++)
        {
            fileName = files.at(i);
            if (QFile::exists(fileName))
            {
                QFile file(fileName);
                if (file.open(QFile::ReadOnly))
                {
                    if (openedFiles.contains(fileName))
                        continue;
                    createNewTab(fileName, file);
                }
            }
        }
    }
}

void MainWindow::newFile()                                                                   //新建文件
{
    QString fileName = tr("New");
    openedFiles << fileName;
    tabWidget->setCurrentIndex(tabWidget->addTab(new TextTab(settings), fileName));
}

bool MainWindow::fileSaveAs(int index)                                                          //文件另存为保存初次保存的文件
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."), QString(),
                                              tr("Plain text Files(*.txt);;All Files (*)"));
    if (fn.isEmpty())
        return false;
    if (!fn.endsWith(".txt", Qt::CaseInsensitive))
        fn += ".txt";

    openedFiles.replace(index, fn);
    tabWidget->setTabText(index, QFileInfo(fn).fileName());
    return fileSave(index);
}

bool MainWindow::fileSave(int index)                                                           //保存文件保存初次保存的文件
{
    TextTab *texttab = static_cast<TextTab *>(tabWidget->widget(index));
    QString fileName = openedFiles.at(index);
    if (!fileName.contains("/") && !fileName.contains("\\"))
        return fileSaveAs(index);
    QTextDocumentWriter writer(fileName);
    bool success = writer.write(texttab->document());
    if (success)
    {
        texttab->document()->setModified(false);
        tabWidget->setCurrentWidget(texttab);
        setWindowTitle(tr("NotePad(%1)").arg(fileName));
    }
    closeDuplicate(index);
    return success;
}

bool MainWindow::fileSaveAs()                                                                   //文件另存为保存当前文件
{
    return fileSaveAs(tabWidget->currentIndex());
}

bool MainWindow::fileSave()                                                                     //文件保存保存当前文件
{
    return fileSave(tabWidget->currentIndex());
}

bool MainWindow::fileSaveAll()                                                                  //全部保存保存当前打开的所有文件
{
    bool success = true;
    for (int i = 0; i < tabWidget->count(); i++)
    {
        tabWidget->setCurrentIndex(i);
        success = fileSave(i);
    }
    return success;
}


void MainWindow::fileClose(int index)                                                              //关闭指定文件
{
    if (maybeSave(index))
    {
        if (openedFiles.count() == 1)
        {
            newFile();
            openedFiles.removeAt(0);
            tabWidget->removeTab(0);
        }
        else
        {
            openedFiles.removeAt(index);
            tabWidget->removeTab(index);
        }
    }
}

void MainWindow::fileClose()                                                                      //关闭当前文件
{
    fileClose(tabWidget->currentIndex());
}

void MainWindow::fileCloseAll()
{
    while (tabWidget->count() >= 1)
    {
        if (maybeSave(tabWidget->currentIndex()))
        {
            if (openedFiles.count() == 1)
            {
                newFile();
                openedFiles.removeAt(0);
                tabWidget->removeTab(0);
                break;
            }
            else
            {
                openedFiles.removeAt(tabWidget->currentIndex());
                tabWidget->removeTab(tabWidget->currentIndex());
            }
        }
        else
            break;
    }
}

void MainWindow::textColor()                                                     //设置文本颜色
{
    QColor col = QColorDialog::getColor(Qt::black, this);
    if (!col.isValid())
        return;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);
    colorChanged(col);
}

void MainWindow::textFontFamily(const QString& font)                             //字体选择框
{
    QTextCharFormat fmt;
    fmt.setFontFamily(font);
    currentCharFormatChanged(fmt);
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::textFont()                                                      //设置文本字体
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, EDITOR->font(),this);
    if (!ok)
        return;
    QTextCharFormat fmt;
    fmt.setFont(font);
    mergeFormatOnWordOrSelection(fmt);
    fontChanged(font);
}

void MainWindow::textSize(const QString &size)                                  //设置字体大小
{
    qreal pointSize = size.toFloat();
    if (size.toFloat() > 0)
    {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        comboSize->setCurrentIndex(comboSize->findText(size));
        mergeFormatOnWordOrSelection(fmt);
    }
}

void MainWindow::colorChanged(const QColor &col)
{
    QPixmap pix(16, 16);
    pix.fill(col);
    fontCAction->setIcon(pix);
}

void MainWindow::fontChanged(const QFont &font)
{
    comboFont->setCurrentIndex(comboFont->findText(QFontInfo(font).family()));
    comboSize->setCurrentIndex(
                comboSize->findText(QString::number(font.pointSize())));
    fontBAction->setChecked(font.bold());
    fontIAction->setChecked(font.italic());
    fontUAction->setChecked(font.underline());
    updateComboStyle();
}

void MainWindow::updateComboStyle()
{
    if (fontBAction->isChecked() && fontIAction->isChecked())
        comboStyle->setCurrentIndex(comboStyle->findText(tr("Bold Italic")));
    else if (fontBAction->isChecked() && !fontIAction->isChecked())
        comboStyle->setCurrentIndex(comboStyle->findText(tr("Bold")));
    else if (!fontBAction->isChecked() && fontIAction->isChecked())
        comboStyle->setCurrentIndex(comboStyle->findText(tr("Italic")));
    else
        comboStyle->setCurrentIndex(comboStyle->findText(tr("Normal")));
}

void MainWindow::textStyle()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(fontBAction->isChecked() ? QFont::Bold : QFont::Normal);
    fmt.setFontItalic(fontIAction->isChecked());
    fmt.setFontUnderline(fontUAction->isChecked());
    mergeFormatOnWordOrSelection(fmt);
    fontChanged(fmt.font());
}

void MainWindow::updateTextStyleActs(QString style)
{
    if (style == "Normal")
    {
        fontBAction->setChecked(false);
        fontIAction->setChecked(false);
    }
    else if (style == "Bold")
    {
        fontBAction->setChecked(true);
        fontIAction->setChecked(false);
    }
    else if (style == "Italic")
    {
        fontBAction->setChecked(false);
        fontIAction->setChecked(true);
    }
    else
    {
        fontBAction->setChecked(true);
        fontIAction->setChecked(true);
    }
}

void MainWindow::textStyle(int styleIndex)
{
    QTextCharFormat fmt;
    if (styleIndex == 0)
    {
        fmt.setFontItalic(false);
        fmt.setFontWeight(QFont::Normal);
    }
    else if (styleIndex == 1)
    {
        fmt.setFontItalic(false);
        fmt.setFontWeight(QFont::Bold);
    }
    else if (styleIndex == 2)
    {
        fmt.setFontItalic(true);
        fmt.setFontWeight(QFont::Normal);
    }
    else
    {
        fmt.setFontItalic(true);
        fmt.setFontWeight(QFont::Bold);
    }
    mergeFormatOnWordOrSelection(fmt);
    fontChanged(fmt.font());
}

void MainWindow::currentCharFormatChanged(const QTextCharFormat &format)
{
    fontChanged(format.font());
    colorChanged(format.foreground().color());
}

void MainWindow::createEditor()
{

    connect(undoAction,SIGNAL(triggered()),this,SLOT(undo()));
    connect(redoAction,SIGNAL(triggered()),this,SLOT(redo()));
    connect(copyAction,SIGNAL(triggered()),this,SLOT(copy()));
    connect(cutAction,SIGNAL(triggered()),this,SLOT(cut()));
    connect(pasteAction,SIGNAL(triggered()),this,SLOT(paste()));
    connect(selectAllAction,SIGNAL(triggered()),this,SLOT(selectAll()));

}
