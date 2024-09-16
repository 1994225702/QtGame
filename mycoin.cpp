#include "mycoin.h"
#include <QDebug>
MyCoin::MyCoin(QString btnImg)
{
    QPixmap pix;
    bool ret=pix.load(btnImg);
    if(!ret)
    {
        QString str = QString("图片 %1 加载失败").arg(btnImg);
        qDebug()<<str;
        return;
    }
    this->setFixedSize(pix.width(),pix.height());
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(),pix.height()));

    //初始化定时器对象
    timer1 = new QTimer(this);
    timer2 = new QTimer(this);

    //监听正面翻反面的信号并翻金币
    connect(timer1,&QTimer::timeout,[=](){
        QPixmap pix;
        QString str = QString(":/res/Coin000%1.png").arg(this->minv++);
        pix.load(str);

        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("QPushButton{border:0px;}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));

        //判断
        if(this->minv>this->maxv)
        {
            this->minv=1;
            isAnimation=false;//结束动画
            timer1->stop();
        }
    });

    //监听反面翻正面的信号并翻金币
    connect(timer2,&QTimer::timeout,[=](){
        QPixmap pix;
        QString str = QString(":/res/Coin000%1.png").arg(this->maxv--);
        pix.load(str);

        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("QPushButton{border:0px;}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));

        //判断
        if(this->minv>this->maxv)
        {
            this->maxv=8;
            isAnimation=false;//结束动画
            timer2->stop();
        }
    });
}
void MyCoin::changeFlag()
{
    if(this->flag)
    {
        timer1->start(30);
        isAnimation=true;//开始做动画
        this->flag = false;
    }
    else
    {
        timer2->start(30);
        isAnimation=true;//开始做动画
        this->flag = true;
    }

}

void MyCoin::mousePressEvent(QMouseEvent *e)
{
    if(this->isAnimation||this->isWin)
        return;
    else
        QPushButton::mousePressEvent(e);
}
