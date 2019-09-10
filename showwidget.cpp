#include "showwidget.h"
#include "ui_showwidget.h"

#include <QHBoxLayout>

showwidget::showwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::showwidget)
{
    ui->setupUi(this);
    mainLayout = new QHBoxLayout;
}

showwidget::~showwidget()
{
    delete ui;
}

void showwidget::goToLine()
{

}
