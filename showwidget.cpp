#include "showwidget.h"
#include "ui_showwidget.h"

showwidget::showwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::showwidget)
{
    ui->setupUi(this);
}

showwidget::~showwidget()
{
    delete ui;
}

void showwidget::goToLine()
{

}
