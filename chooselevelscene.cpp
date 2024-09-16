#include "chooselevelscene.h"
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include <QTimer>
#include <QLabel>
#include <QDebug>
#include <QSoundEffect>
ChooseLevelScene::ChooseLevelScene(QWidget *parent)
    : QMainWindow{parent}
{

    this->setFixedSize(320,588);

    this->setWindowIcon(QIcon(":/res/Coin0001.png"));

    this->setWindowTitle("选择关卡场景");

    //创建菜单栏
    QMenuBar* bar=menuBar();
    setMenuBar(bar);

    //创建开始菜单
    QMenu* startMenu = bar->addMenu("开始");

    //创建退出 菜单项
    QAction* actionQuit =startMenu->addAction("退出");

    connect(actionQuit,&QAction::triggered,[=](){

        this->close();
    });

    //返回按钮
    MyPushButton* backBtn = new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(),this->height()-backBtn->height());

    //准备关卡按钮的音效
    QSoundEffect* closeSound = new QSoundEffect(this);
    closeSound->setSource(QUrl::fromLocalFile(":/res/BackButtonSound.wav"));

    connect(backBtn,&QPushButton::clicked,[=](){
        //延时返回
        closeSound->play();
        QTimer::singleShot(200,this,[=](){
            emit this->chooseSceneBack();//发送关闭信号
        });

    });

    //准备关卡按钮的音效
    QSoundEffect* chooseSound = new QSoundEffect(this);
    chooseSound->setSource(QUrl::fromLocalFile(":/res/TapButtonSound.wav"));

    //创建选择关卡按钮
    for(int i=0;i<20;i++)
    {
        MyPushButton* menuBtn= new MyPushButton(":/res/LevelIcon.png");
        menuBtn->setParent(this);
        menuBtn->move(25+i%4*70,140+i/4*70);


        //监听每个按钮的点击事件
        connect(menuBtn,&MyPushButton::clicked,[=](){
            // QString str = QString("选择第 %1 关").arg(i+1);
            // qDebug()<<str;

            chooseSound->play();
            //进入游戏场景
            this->hide();
            play = new PlayScene(i+1);
            play->setGeometry(this->geometry());
            play->show();

            //监听关闭选择场景信号
            connect(play,&PlayScene::chooseSceneBack,[=](){
                this->setGeometry(play->geometry());
                delete play;
                play=nullptr;

                this->show();//重现关卡选择场景
            });
        });
        QLabel* label=new QLabel;
        label->setParent(this);
        label->setFixedSize(menuBtn->width(),menuBtn->height());
        label->setText(QString::number(i+1));
        label->move(25+i%4*70,140+i/4*70);

        //设置 label文字对齐方式 水平垂直居中
        label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        //设置 鼠标穿透属性
        label->setAttribute(Qt::WA_TransparentForMouseEvents);
    }


}

void ChooseLevelScene::paintEvent(QPaintEvent* )
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/OtherSceneBg.png");
    painter.drawPixmap(0,30,this->width(),this->height(),pix);


    pix.load(":/res/Title.png");
    painter.drawPixmap(this->width()*0.5-pix.width()*0.5,30,pix.width(),pix.height(),pix);
}
