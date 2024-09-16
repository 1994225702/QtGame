#ifndef MYCOIN_H
#define MYCOIN_H

#include <QPushButton>
#include <QTimer>
class MyCoin : public QPushButton
{
    Q_OBJECT
public:
   // explicit MyCoin(QWidget *parent = nullptr);
    //参数 图片路径
    MyCoin(QString btnImg);

    //金币属性
    int posX; //x坐标位置
    int posY; //y坐标位置
    bool flag; //正反标识

    void changeFlag();//翻
    QTimer* timer1;
    QTimer* timer2;
    int minv = 1;
    int maxv = 8;
    //执行动画标志
    bool isAnimation=false;

    //重写 按下
    void mousePressEvent(QMouseEvent *e);

    bool isWin=false;//判断胜利标志
signals:
};

#endif // MYCOIN_H
