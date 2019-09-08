#include "ide.h"

//QsciScintilla作为QWidget的控件，需要添加该控件的头文件
#include <Qsci/qsciscintilla.h>
//以python语法作为例子，该语法分析器的头文件
#include <Qsci/qscilexerpython.h>
//设置代码提示功能，依靠QsciAPIs类实现
#include <Qsci/qsciapis.h>

#pragma execution_character_set("utf-8");

IDE::IDE(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("C语言集成开发环境"));
    showWidget = new ShowWidget(this);
    setCentralWidget(showWidget);

    this -> setWindowIcon(QIcon("title.ico"));
    this -> resize(QSize(1300,900));

    createActions();                            //创建动作
    createMenus();                              //创建菜单
    createToolBars();                           //创建工具栏

    if(img.load("image.png"))
    {
        showWidget -> imageLabel ->setPixmap(QPixmap::fromImage(img));
    }

    QsciScintilla *editor=new QsciScintilla(this);
    setCentralWidget(editor);
    //设置语法
    QsciLexerPython *textLexer = new QsciLexerPython;//创建一个词法分析器
    editor->setLexer(textLexer);//给QsciScintilla设置词法分析器
    editor->setMarginType(0,QsciScintilla::NumberMargin);//设置编号为0的页边显示行号。
    editor->setMarginLineNumbers(0,true);//对该页边启用行号
    editor->setMarginWidth(0,15);//设置页边宽度
    //代码提示
    QsciAPIs *apis = new QsciAPIs(textLexer);
    apis->add(QString("import"));
    apis->prepare();
    editor->setAutoCompletionSource(QsciScintilla::AcsAll);   //设置源，自动补全所有地方出现的
    editor->setAutoCompletionCaseSensitivity(true);   //设置自动补全大小写敏感
    editor->setAutoCompletionThreshold(1);    //设置每输入一个字符就会出现自动补全的提示
    editor->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,QsciScintilla::SC_CP_UTF8);//设置编码为UTF-8
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
    connect(openFileAction,SIGNAL(triggered()),this,SLOT(showOpenFile()));

    newFileAction = new QAction(QIcon("new.ico"),tr("新建"),this);                                  //新建文件
    newFileAction -> setShortcut(tr("Ctrl+N"));
    newFileAction -> setStatusTip(tr("新建一个文件"));

    saveAction = new QAction(QIcon("save.ico"),tr("保存"),this);                                    //保存
    saveAction -> setShortcut(tr("Ctrl+S"));
    saveAction -> setStatusTip(tr("保存当前正在编辑的文件"));

    saveAsAction = new QAction(QIcon("saveAs.ico"),tr("另存为"),this);                              //另存为
    saveAsAction -> setStatusTip(tr("将当前正在编辑的文本保存在新的文件中"));

    saveAllAction = new QAction(QIcon("saveAll.ico"),tr("全部保存"),this);                          //全部保存
    saveAllAction -> setShortcut(tr("Shift+Ctrl+S"));
    saveAllAction -> setStatusTip(tr("保存当前打开的所有文件"));

    closeAction = new QAction(tr("关闭"),this);                                                    //关闭
    closeAction -> setShortcut(tr("Ctrl+W"));
    closeAction -> setStatusTip(tr("关闭当前正在编辑的文件"));

    closeAllAction = new QAction(QIcon("closeAll.ico"),tr("全部关闭"),this);                        //全部关闭
    closeAllAction -> setShortcut(tr("Shift+Ctrl+W"));
    closeAllAction -> setStatusTip(tr("关闭当前打开的全部文件"));

    currentFileAction = new QAction(QIcon("currentFile.ico"),tr("最近打开文件"),this);              //最近打开文件
    currentFileAction -> setStatusTip(tr("显示最近所打开过的文件"));

    exitAction = new QAction(tr("退出"),this);                                                     //退出
    exitAction -> setShortcut(tr("Ctrl+Q"));
    exitAction -> setStatusTip(tr("退出集成开发环境"));
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));

//编辑菜单动作
    revokeAction = new QAction(QIcon("revoke.ico"),tr("撤销"),this);                               //撤销
    revokeAction -> setShortcut(tr("Ctrl+Z"));
    revokeAction -> setStatusTip(tr("撤销上一步进行的操作"));
    connect(revokeAction,SIGNAL(triggered()),showWidget -> text,SLOT(undo()));

    recoveryAction = new QAction(QIcon("recovery.ico"),tr("恢复"),this);                           //恢复
    recoveryAction -> setShortcut(tr("Shift+Ctrl+Z"));
    recoveryAction -> setStatusTip(tr("恢复上一步被撤销的的操作"));
    connect(recoveryAction,SIGNAL(triggered()),showWidget -> text,SLOT(redo()));

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
    deleteAction -> setStatusTip(tr("删除所选中的文本内容"));

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

//视图菜单动作
    showConsoleAction = new QAction(QIcon("console.ico"),tr("显示控制台"),this);                    //显示控制台
    showConsoleAction -> setStatusTip(tr("显示或关闭控制台"));

    showStatusAction = new QAction(QIcon("status.ico"),tr("显示状态栏"),this);                      //显示状态栏
    showStatusAction -> setStatusTip(tr("显示或关闭状态栏"));

    showToolsAction = new QAction(QIcon("tool.ico"),tr("显示工具栏"),this);                         //显示工具栏
    showToolsAction -> setStatusTip(tr("显示或关闭工具栏"));

    showSourceAction = new QAction(QIcon("source.ico"),tr("显示资源管理器"),this);                   //显示资源管理器
    showSourceAction -> setStatusTip(tr("显示或关闭资源管理器"));

    showFullScreenAction = new QAction(QIcon("fullScreen.ico"),tr("全屏显示"),this);                //全屏显示
    showFullScreenAction -> setShortcut(tr("Ctrl+F11"));
    showFullScreenAction -> setStatusTip(tr("将文本编辑页面全屏显示"));

//设置菜单动作
    chooseEOrCAction = new QAction(tr("中/英文显示切换"),this);                                      //中英文显示切换
    chooseEOrCAction -> setStatusTip(tr("切换中/英文显示界面"));

    fontSetAction = new QAction(QIcon("font.ico"),tr("字体设置"),this);                             //字体设置
    fontSetAction -> setStatusTip(tr("设置显示的字体和大小"));

//工具菜单工作
    compileAction = new QAction(QIcon("compile.ico"),tr("编译"),this);                             //编译
    compileAction -> setShortcut(tr("F9"));
    compileAction -> setStatusTip(tr("对当前编辑文件中的文本进行编译"));

    runAction = new QAction(QIcon("run.ico"),tr("运行"),this);                                     //运行
    runAction -> setShortcut(tr("F10"));
    runAction -> setStatusTip(tr("运行文本中的C语言源代码"));

    compileRunAction = new QAction(QIcon("compileRun.ico"),tr("编译运行"),this);                    //编译运行
    compileRunAction -> setShortcut(tr("F11"));
    compileRunAction -> setStatusTip(tr("对当前编辑文件中的文本进行编译并运行文本中的源代码"));

    compileAllAction = new QAction(QIcon("compileAll.ico"),tr("全部编译"),this);                    //全部编译
    compileAllAction -> setShortcut(tr("F12"));
    compileAllAction -> setStatusTip(tr("对所有打开文件中的文本进行编译"));

    startDebugAction = new QAction(QIcon("start.ico"),tr("开始调试"),this);                         //开始调试
    startDebugAction -> setShortcut(tr("F5"));
    startDebugAction -> setStatusTip(tr("对文本开始进行调试"));

    endDebugAction = new QAction(QIcon("end.ico"),tr("结束调试"),this);                             //结束调试
    endDebugAction -> setStatusTip(tr("关闭调试器"));

    setBreakAction = new QAction(QIcon("setBreak.ico"),tr("设置断点"),this);                        //设置断点
    setBreakAction -> setStatusTip(tr("在文本中添加一个断点"));

    cancelBreakAction = new QAction(QIcon("cancelBreak.ico"),tr("取消断点"),this);                  //取消断点
    cancelBreakAction -> setStatusTip(tr("取消一个在文本中设置的断点"));

    nextStepAction = new QAction(QIcon("next.ico"),tr("下一步"),this);                              //下一步
    nextStepAction -> setShortcut(tr("F7"));
    nextStepAction -> setStatusTip(tr("执行下一条语句"));

    turnNextBreakAction = new QAction(QIcon("turnNext.ico"),tr("跳到下一个断点"),this);              //跳到下一个断点
    turnNextBreakAction -> setStatusTip(tr("使程序运行到下一个断点所在行"));

    enterAction = new QAction(QIcon("enter"),tr("单步进入"),this);                                  //单步进入
    enterAction -> setStatusTip(tr("进入函数体内部"));

    jumpAction = new QAction(QIcon("jump"),tr("跳出"),this);                                       //跳出
    jumpAction -> setStatusTip(tr("从函数体内部跳出"));

    addWatchAction = new QAction(QIcon("add.ico"),tr("添加查看"),this);                             //添加查看
    addWatchAction -> setStatusTip(tr("添加需要查看的变量名"));

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
    fileMenu -> addAction(currentFileAction);
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
    viewMenu -> addAction(showConsoleAction);
    viewMenu -> addAction(showStatusAction);
    viewMenu -> addAction(showToolsAction);
    viewMenu -> addAction(showSourceAction);
    viewMenu -> addSeparator();
    viewMenu -> addAction(showFullScreenAction);

//设置菜单
    settingMenu = menuBar() -> addMenu(tr("设置"));
    settingMenu -> addAction(chooseEOrCAction);
    settingMenu -> addAction(fontSetAction);

//工具菜单
    toolMenu = menuBar() -> addMenu(tr("工具"));
    toolMenu -> addAction(compileAction);
    toolMenu -> addAction(runAction);
    toolMenu -> addAction(compileRunAction);
    toolMenu -> addAction(compileAllAction);
    toolMenu -> addSeparator();
    toolMenu -> addAction(startDebugAction);
    toolMenu -> addAction(endDebugAction);
    toolMenu -> addSeparator();
    toolMenu -> addAction(setBreakAction);
    toolMenu -> addAction(cancelBreakAction);
    toolMenu -> addSeparator();
    toolMenu -> addAction(nextStepAction);
    toolMenu -> addAction(turnNextBreakAction);
    toolMenu -> addAction(enterAction);
    toolMenu -> addAction(jumpAction);
    toolMenu -> addAction(addWatchAction);

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
    fileTool -> setMovable(false);

//编辑工具栏
    editTool = addToolBar("Edit");
    editTool -> addAction(revokeAction);
    editTool -> addAction(recoveryAction);
    editTool -> addAction(copyAction);
    editTool -> addAction(cutAction);
    editTool -> addAction(pasteAction);
    editTool -> setMovable(false);

//搜索工具栏
    searchTool = addToolBar("Search");
    searchTool -> addAction(seekAction);
    searchTool -> addAction(replaceAction);
    searchTool -> setMovable(false);

//视图工具栏
    viewTool = addToolBar("View");
    viewTool -> addAction(showConsoleAction);
    viewTool -> addAction(showStatusAction);
    viewTool -> addAction(showToolsAction);
    viewTool -> setMovable(false);

//设置工具栏
    settingTool = addToolBar("Setting");
    settingTool -> addAction(fontSetAction);
    settingTool -> setMovable(false);

//编译工具栏
    compileTool = addToolBar("Compile");
    compileTool -> addAction(compileAction);
    compileTool -> addAction(compileAllAction);
    compileTool -> addAction(runAction);
    compileTool -> addAction(compileRunAction);
    compileTool -> setMovable(false);

//调试工具栏
    debugTool = addToolBar("Debug");
    debugTool -> addAction(startDebugAction);
    debugTool -> addAction(endDebugAction);
    debugTool -> addAction(setBreakAction);
    debugTool -> addAction(cancelBreakAction);
    debugTool -> addAction(nextStepAction);
    debugTool -> addAction(turnNextBreakAction);
    debugTool -> addAction(enterAction);
    debugTool -> addAction(jumpAction);
    debugTool -> addAction(addWatchAction);
    debugTool -> setMovable(false);

//帮助工具栏
    helpTool = addToolBar("Help");
    helpTool -> addAction(helpDocAction);
    helpTool -> addAction(aboutAction);
    helpTool -> setMovable(false);

}

void IDE::showOpenFile()
{
    fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())
    {
        if(showWidget -> text -> document() -> isEmpty())
        {
            loadFile(fileName);
        }
    }
}

void IDE::loadFile(QString filename)
{
    printf("file name:%s\n",filename.data());
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream textStream(&file);
        while(!textStream.atEnd())
        {
            showWidget -> text -> append(textStream.readLine());
            printf("read line\n");
        }
        printf("end\n");
    }
}
