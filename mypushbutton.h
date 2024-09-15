#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    //explicit MyPushButton(QWidget *parent = nullptr);
    //normalImg代表正常显示的按钮
    //pressImg代表按下后显示的图片，默认为空
    MyPushButton(QString normalImg,QString pressImg="");//声明和实现不能同时有默认值

    QString normalImgPath;
    QString pressedImgPath;

    //弹跳特效
    void zoom1();//向下跳
    //重写按钮 按下和释放事件
    void mousePressEvent(QMouseEvent *e);

    void mouseReleaseEvent(QMouseEvent *e);
signals:
};

#endif // MYPUSHBUTTON_H
