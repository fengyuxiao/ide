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
#include <QActionGroup>
#include <QMimeData>
#include <QClipboard>
#include <QUndoCommand>

MainWindow::MainWindow(Settings *settings,QWidget *parent) :
    QMainWindow(parent), settings(settings),
  ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createinit();
    createTab();
    createAction();
    createMenuBar();
    createToolBar();

    setCentralWidget(tabWidget);

    setWindowTitle(tr("C语言集成开发环境"));
    this -> setWindowIcon(QIcon("Logo.ico"));
    this -> resize(1300,900);

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
    addToolBar(toolBar);
    toolBar -> setMovable(false);
}

bool MainWindow::fileSave()
{
    return fileSave(tabWidget->currentIndex());
}

void MainWindow::createAction()
{

//文件操作定义
    openFileAction = new QAction(QIcon(":/images/open.ico"),tr("打开"),this);
    openFileAction -> setShortcut(QKeySequence::Open);
    openFileAction -> setStatusTip(tr(""));
    connect(openFileAction,SIGNAL(triggered()),this,SLOT(openFile()));

    newFileAction = new QAction(QIcon(":/images/new.ico"),tr("新建"),this);
    newFileAction -> setShortcut(QKeySequence::New);
    newFileAction -> setStatusTip(tr(""));
    connect(newFileAction,SIGNAL(triggered()),this,SLOT(newFile()));

    saveFileAction = new QAction(QIcon(":/images/save.ico"),tr("保存"),this);
    saveFileAction -> setShortcut(QKeySequence::Save);
    saveFileAction -> setStatusTip(tr(""));
    connect(saveFileAction,SIGNAL(triggered()),this,SLOT(fileSave()));

    saveAsFileAction = new QAction(QIcon(":/images/saveAs.ico"),tr("另存为"),this);
    saveAsFileAction -> setShortcut(QKeySequence::SaveAs);
    saveAsFileAction -> setStatusTip(tr(""));
    connect(saveAsFileAction,SIGNAL(triggered()),this,SLOT(fileSaveAs()));

    saveAllFileAction = new QAction(QIcon(":/images/saveAll.ico"),tr("全部保存"),this);
    saveAllFileAction -> setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    saveAllFileAction -> setStatusTip(tr(""));
    connect(saveAllFileAction,SIGNAL(triggered()),this,SLOT(fileSaveAll()));

    closeFileAction = new QAction(QIcon(""),tr("关闭"),this);
    closeFileAction -> setShortcut(QKeySequence::Close);
    closeFileAction -> setStatusTip(tr(""));
    connect(closeFileAction,SIGNAL(triggered()),this,SLOT(fileClose()));

    closeAllFileAction = new QAction(QIcon(":/images/close.ico"),tr("全部关闭"),this);
    closeAllFileAction -> setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_X);
    closeAllFileAction -> setStatusTip(tr(""));
    connect(closeAllFileAction,SIGNAL(triggered()),this,SLOT(fileCloseAll()));

    exitFileAction = new QAction(QIcon(":/images/exit.ico"),tr("退出"),this);
    exitFileAction -> setShortcut(QKeySequence::Quit);
    exitFileAction -> setStatusTip(tr(""));
    connect(exitFileAction,SIGNAL(triggered()),this,SLOT(close()));

//编辑操作定义
    undoAction = new QAction(QIcon(":/images/undo.png"),tr("撤销"),this);
    undoAction -> setShortcut(QKeySequence::Undo);
    undoAction -> setStatusTip(tr(""));
    connect(undoAction,SIGNAL(triggered()),this,SLOT(undo()));

    redoAction = new QAction(QIcon(":/images/redo.png"),tr("恢复"),this);
    redoAction -> setShortcut(QKeySequence::Redo);
    redoAction -> setStatusTip(tr(""));
    connect(redoAction,SIGNAL(triggered()),this,SLOT(redo()));

    copyAction = new QAction(QIcon(":/images/copy.ico"),tr("复制"),this);
    copyAction -> setShortcut(QKeySequence::Copy);
    copyAction -> setStatusTip(tr(""));
    connect(copyAction,SIGNAL(triggered()),this,SLOT(copy()));

    cutAction = new QAction(QIcon(":/images/cut.ico"),tr("剪切"),this);
    cutAction -> setShortcut(QKeySequence::Cut);
    cutAction -> setStatusTip(tr(""));
    connect(cutAction,SIGNAL(triggered()),this,SLOT(cut()));

    pasteAction = new QAction(QIcon(":/images/paste.ico"),tr("粘贴"),this);
    pasteAction -> setShortcut(QKeySequence::Paste);
    pasteAction -> setStatusTip(tr(""));
    connect(pasteAction,SIGNAL(triggered()),this,SLOT(paste()));

    selectAllAction = new QAction(QIcon(":/images/select.png"),tr("全选"),this);
    selectAllAction -> setShortcut(QKeySequence::SelectAll);
    selectAllAction -> setStatusTip(tr(""));
    connect(selectAllAction,SIGNAL(triggered()),this,SLOT(selectAll()));

    goToLineAction = new QAction(QIcon(""),tr("跳转到行"),this);
    goToLineAction -> setShortcut(Qt::CTRL + Qt::Key_G);
    goToLineAction -> setStatusTip(tr(""));

//设置操作定义
    fontTypeAction = new QAction(QIcon(":/images/fontType.ico"),tr("文本字体"),this);
    fontTypeAction -> setStatusTip(tr(""));
    connect(fontTypeAction,SIGNAL(triggered()),SLOT(textFont()));

//搜索操作定义
    searchAction = new QAction(QIcon(":/images/search.ico"),tr("查找"),this);
    searchAction -> setStatusTip(tr(""));

    replaceAction = new QAction(QIcon(":/images/replace.ico"),tr("替换"),this);
    replaceAction -> setStatusTip(tr(""));

//视图操作定义
    toolBarAction = new QAction(QIcon(":/images/tool.ico"),tr("显示工具栏"),this);
    toolBarAction -> setStatusTip(tr(""));
    connect(toolBarAction,SIGNAL(triggered()),this,SLOT(judgeToolBars()));


//工具操作定义
    compileAction = new QAction(QIcon(":/images/compile.ico"),tr("编译"),this);
    compileAction -> setShortcut(Qt::Key_F9);
    compileAction -> setStatusTip(tr(""));

    runAction = new QAction(QIcon(":/images/run.ico"),tr("运行"),this);
    runAction -> setShortcut(Qt::Key_F10);
    runAction -> setStatusTip(tr(""));

    compileRunAction = new QAction(QIcon(":/images/compile.ico"),tr("编译运行"),this);
    compileRunAction -> setShortcut(Qt::Key_F11);
    compileRunAction -> setStatusTip(tr(""));

    compileAllAction = new QAction(QIcon(":/images/compileAll.ico"),tr("全部编译"),this);
    compileAllAction -> setShortcut(Qt::Key_F12);
    compileAllAction -> setStatusTip(tr(""));

    startDebugAction = new QAction(QIcon(":/images/start.ico"),tr("开始调试"),this);
    startDebugAction -> setShortcut(Qt::Key_F5);
    startDebugAction -> setStatusTip(tr(""));

    endDebugAction = new QAction(QIcon(":/images/end.ico"),tr("结束调试"),this);
    endDebugAction -> setStatusTip(tr(""));

    setBreakAction = new QAction(QIcon(":/images/set.ico"),tr("设置断点"),this);
    setBreakAction -> setStatusTip(tr(""));

    cancelBreakAction = new QAction(QIcon(":/images/cancel.ico"),tr("取消断点"),this);
    cancelBreakAction -> setStatusTip(tr(""));

    nextLineAction = new QAction(QIcon(":/images/nextL.ico"),tr("下一步"),this);
    nextLineAction -> setShortcut(Qt::Key_F7);
    nextLineAction -> setStatusTip(tr(""));

    nextBreakAction = new QAction(QIcon(":/images/nextB.ico"),tr("跳到下一个断点"),this);
    nextBreakAction -> setStatusTip(tr(""));

    enterAction = new QAction(QIcon(""),tr("单步进入"),this);
    enterAction -> setStatusTip(tr(""));

    jumpAction = new QAction(QIcon(""),tr("跳出"),this);
    jumpAction -> setStatusTip(tr(""));

    addTrackAction = new QAction(QIcon(":/images/addT.ico"),tr("添加查看"),this);
    addTrackAction -> setStatusTip(tr(""));

//帮助操作定义
    helpDocAction = new QAction(QIcon(":/images/help.ico"),tr("帮助文档"),this);
    helpDocAction -> setStatusTip(tr(""));

    aboutAction = new QAction(QIcon(":/images/about.ico"),tr("关于"),this);
    aboutAction -> setStatusTip(tr(""));

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

void MainWindow::textFontFamily(const QString& font)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(font);
    currentCharFormatChanged(fmt);
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::textFont()
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


void MainWindow::fontChanged(const QFont &font)
{
    comboFont->setCurrentIndex(comboFont->findText(QFontInfo(font).family()));
    comboSize->setCurrentIndex(
                comboSize->findText(QString::number(font.pointSize())));
}


void MainWindow::currentCharFormatChanged(const QTextCharFormat &format)
{
    fontChanged(format.font());
}

//文件菜单
void MainWindow::closeEvent(QCloseEvent *event)
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

//文档发生改变
void MainWindow::modificationChanged(bool changed)
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
    tabWidget->setTabText(tabWidget->currentIndex(), str);
}

void MainWindow::selectionChanged()
{
    QTextCharFormat fmt=EDITOR->currentCharFormat();
    currentCharFormatChanged(fmt);
}


void MainWindow::currentChanged(int index)
{
    if (index == -1)
    {
        newNumber = 0;
        newFile();
        return;
    }
    setWindowIcon(QIcon(tr(":images/notepad.png")));
    setWindowTitle(tr("NotePad(%1)").arg(openedFiles.at(index)));
}

//创建新的Tab（用于打开文件）
void MainWindow::createNewTab(const QString& fileName, QFile& file)
{
    openedFiles << fileName;
    TextTab *notePad = new TextTab(settings);
    tabWidget->addTab(notePad, QFileInfo(fileName).fileName());
    QByteArray data = file.readAll();
    notePad->setPlainText(QString::fromLocal8Bit(data));
    tabWidget->setCurrentWidget(notePad);
}

bool MainWindow::maybeSave(int index)
{
    TextTab *notePad = static_cast<TextTab *>(tabWidget->widget(index));
    QString fileName = openedFiles.at(index);
    if (!notePad->document()->isModified())
        return true;
    if (fileName.startsWith(QLatin1String(":/")))
        return true;
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("NotePad"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret == QMessageBox::Save)
        return fileSave(index);
    else if (ret == QMessageBox::Cancel)
        return false;
    return true;
}

void MainWindow::closeDuplicate(int index)
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
    setWindowTitle(tr("NotePad(%1)").arg(fileName));
}

void MainWindow::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = EDITOR->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    EDITOR->mergeCurrentCharFormat(format);
}

void MainWindow::openFile(QString fileName)
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

void MainWindow::openFile()
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

void MainWindow::newFile()
{
    QString fileName = tr("New %1").arg(++newNumber);
    openedFiles << fileName;
    tabWidget->setCurrentIndex(
                tabWidget->addTab(new TextTab(settings), fileName));
}

bool MainWindow::fileSaveAs(int index)
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

bool MainWindow::fileSave(int index)
{
    TextTab *notePad = static_cast<TextTab *>(tabWidget->widget(index));
    QString fileName = openedFiles.at(index);
    if (!fileName.contains("/") && !fileName.contains("\\"))
        return fileSaveAs(index);
    QTextDocumentWriter writer(fileName);
    bool success = writer.write(notePad->document());
    if (success)
    {
        notePad->document()->setModified(false);
        tabWidget->setCurrentWidget(notePad);
        setWindowTitle(tr("NotePad(%1)").arg(fileName));
    }
    closeDuplicate(index);
    return success;
}

bool MainWindow::fileSaveAs()
{
    return fileSaveAs(tabWidget->currentIndex());
}

bool MainWindow::fileSaveAll()
{
    bool success = true;
    for (int i = 0; i < tabWidget->count(); i++)
    {
        tabWidget->setCurrentIndex(i);
        success = fileSave(i);
    }
    return success;
}

void MainWindow::fileClose(int index)
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

void MainWindow::fileClose()
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

void MainWindow::judgeToolBars()
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

void MainWindow::hideToolBars()
{
    toolBar -> setVisible(false);
}

void MainWindow::showToolBars()
{
    toolBar -> setVisible(true);
}
