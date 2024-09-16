#ifndef CHOOSELEVELSCENE_H
#define CHOOSELEVELSCENE_H

#include <QMainWindow>
#include "playscene.h"
class ChooseLevelScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit ChooseLevelScene(QWidget *parent = nullptr);


    //重写paintEvent
    void paintEvent(QPaintEvent* );

    //游戏场景对象指针
    PlayScene* play = nullptr;

signals:
    //自定义按下关闭按钮信号
    void chooseSceneBack();
};

#endif // CHOOSELEVELSCENE_H
