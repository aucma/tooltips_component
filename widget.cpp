#include "widget.h"
#include "ui_widget.h"
#include "tooltipswidget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 1
    tooltipsInfo* tooltipinfo = new tooltipsInfo;
    tooltipinfo->m_direction = tooltipsInfo::left; //箭头的方向
    tooltipinfo->m_content = "测试测试测试";
    tooltipswidget::showtooltips(ui->pushButton,  tooltipinfo);

    //2
    tooltipswidget::showtooltips(ui->pushButton_2,  "哈哈哈");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{

}

