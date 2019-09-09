#ifndef TEXTTAB_H
#define TEXTTAB_H

#include <QPlainTextEdit>
#include <QTextBlock>
#include <QEvent>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QContextMenuEvent>
#include <QAction>

#include "settings.h"

#define FOLDBOXRECT(cy) QRect(lineNumWidth + foldBoxIndent, cy - foldBoxWidth / 2, foldBoxWidth, foldBoxWidth)
#define FULLRESIZE      resizeEvent(new QResizeEvent(QSize(0, 0), size()))
#define FONTWIDTH       fontMetrics().width(QLatin1Char('X'))

class TextTab: public QPlainTextEdit
{
    Q_OBJECT

public:
    //block 类型
    enum BlockState
    {
        End = 1,
        Begin = 2,
        String = 4,
        Comment = 8,
        Nested = 16,
        Folded = 32,
        Error = 64,
        Rehighligh = 128,
        Empty = -1
    };

    const static int StateShift = 8;

public:
    TextTab(Settings *, QWidget *parent = 0);
    ~TextTab();

public slots:
    void gotoLine(int lineNumber);
    int search(QString, bool, bool, bool);
    void replace(QString, QString, bool, bool, bool);
    void replaceAll(QString, QString, bool, bool);

private slots:

    void deleteSelected()
    {
        textCursor().removeSelectedText();
    }

    void blockCountChanged(int count)
    {
        lineNumDigits = qMax(2, QString::number(count).length());
    }

    void contentsChange(int, int, int);
    void ensureCursorVisible();
    void resetting(int = (Settings::Init | Settings::Editor));

private:
    bool eventFilter(QObject *, QEvent *);

    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void contextMenuEvent(QContextMenuEvent *);
    void extraAreaPaintEvent();
    void extraAreaMouseEvent(QMouseEvent *);

    void convertCase(bool toUpper = true)
    {
        textCursor().insertText(
                    toUpper ?
                        textCursor().selectedText().toUpper() :
                        textCursor().selectedText().toLower());
    }

    QTextBlock pointedBlock;
    QTextBlock findBlockByY(int);
    QWidget *extraArea;

    int lineNumDigits;
    int lineNumWidth;
    int foldBoxWidth;
    int foldBoxIndent;
    int foldBoxLength;
    int setBlockState(QTextBlock&);

    Settings *settings;
};

#endif // TEXTTAB_H
