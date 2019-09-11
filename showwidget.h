#ifndef SHOWWIDGET_H
#define SHOWWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include <QHBoxLayout>

#pragma execution_character_set("utf-8")

namespace Ui {
class showwidget;
}

class showwidget : public QWidget
{
    Q_OBJECT

public:
    explicit showwidget(QWidget *parent = nullptr);
    ~showwidget();

    QHBoxLayout *mainLayout;

signals:
    void goToLine(int LineNum);           //跳转到行

private slots:
    void goToLine();                    //跳转到行

private:
    Ui::showwidget *ui;
};

#endif // SHOWWIDGET_H
