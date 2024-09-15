#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include "mycoin.h"
class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
    // explicit PlayScene(QWidget *parent = nullptr);

    PlayScene(int levelNum);

    int levelIndex; //内部成员属性 记录所选的关卡

    //重写paintEvent
    void paintEvent(QPaintEvent* );

    int gameArray[4][4]; //二维数组 维护每个关卡的具体数据
    MyCoin* coinBtn[4][4];//存放各个按钮地址 实时更新;

    bool isWin=false;//判断胜利标志
signals:
    //自定义按下关闭按钮信号
    void chooseSceneBack();
};

#endif // PLAYSCENE_H
