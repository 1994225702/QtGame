#include "mainscene.h"
#include "ui_mainscene.h"
#include <QPainter>
#include "mypushbutton.h"
#include <QTimer>
#include <QSoundEffect> //多媒体音效
MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/res/Coin0001.png"));
    setWindowTitle("翻硬币");
    setFixedSize(320,588);
    connect(ui->actionExit,&QAction::triggered,[=](){
        this->close();
    });

    //准备开始按钮的音效
    QSoundEffect* startSound = new QSoundEffect(this);
    startSound->setSource(QUrl::fromLocalFile(":/res/TapButtonSound.wav"));

    MyPushButton* startBtn=new MyPushButton(":/res/MenuSceneStartButton.png");
    startBtn->setParent(this);
    startBtn->move(this->width()*0.5-startBtn->width()*0.5,this->height()*0.7);

    //实例化选择关卡场景
    chooseScene = new ChooseLevelScene;

    //监听关闭选择场景信号
    connect(chooseScene,&ChooseLevelScene::chooseSceneBack,[=](){
        chooseScene->hide();
        this->setGeometry(chooseScene->geometry());
        this->show();//重现主场景
    });


    connect(startBtn,&QPushButton::clicked,[=](){
        // qDebug()<<"start";
        startSound->play();
        startBtn->zoom1();
        // startBtn->zoom2();


        //延时进入到选择关卡场景
        QTimer::singleShot(200,this,[=](){
            //自身隐藏
            chooseScene->setGeometry(this->geometry());
            this->hide();
            //显示选择关卡场景
            chooseScene->show();
        });


    });
}


void MainScene::paintEvent(QPaintEvent* )
{
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    QPainter painter(this);
    painter.drawPixmap(0,30,this->width(),this->height(),pix);

    pix.load(":/res/Title.png");
    pix= pix.scaled(pix.width()*0.5,pix.height()*0.5);
    painter.drawPixmap(10,30,pix);
}
MainScene::~MainScene()
{
    delete ui;
}
