#include "playscene.h"
#include <QDebug>
#include <QMenuBar>
#include <QPainter>
#include <QTimer>
#include "mypushbutton.h"
#include <QLabel>
#include <QPropertyAnimation>
#include "dataconfig.h"
#include <QSoundEffect>
PlayScene::PlayScene(int levelNum)
{
    // QString str=QString("进入了第 %1 关").arg(levelNum);
    // qDebug() << str;
    this->levelIndex=levelNum;

    //初始化游戏场景
    this->setFixedSize(320,588);
    this->setWindowIcon(QIcon(":/res/Coin0001.png"));
    this->setWindowTitle("翻金币ing");
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

    //准备返回按钮的音效
    QSoundEffect* closeSound = new QSoundEffect(this);
    closeSound->setSource(QUrl::fromLocalFile(":/res/BackButtonSound.wav"));

    //准备翻金币的音效
    QSoundEffect* flipSound = new QSoundEffect(this);
    flipSound->setSource(QUrl::fromLocalFile(":/res/ConFlipSound.wav"));

    //准备胜利的音效
    QSoundEffect* winSound = new QSoundEffect(this);
    winSound->setSource(QUrl::fromLocalFile(":/res/LevelWinSound.wav"));



    connect(backBtn,&QPushButton::clicked,[=](){
        //延时返回
        closeSound->play();
        QTimer::singleShot(200,this,[=](){
            emit this->chooseSceneBack();//发送关闭信号
        });

    });


    //显示当前关卡数
    QLabel* label= new QLabel;
    label->setParent(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    QString str=QString("Level: %1").arg(this->levelIndex);
    label->setFont(font);
    label->setText(str);
    label->setGeometry(30,this->height()-50,120,50);


    dataConfig config;
    //初始化每个关卡的二维数组
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            this->gameArray[i][j]=config.mData[this->levelIndex][i][j];

    //胜利图片显示
    QLabel* winLabel = new QLabel;
    QPixmap tmpPix;
    tmpPix.load(":/res/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->setParent(this);
    winLabel->move((this->width()-tmpPix.width())*0.5,-tmpPix.height());

    //显示金币背景图片
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            //绘制背景图片
            QPixmap pix(":/res/BoardNode.png");
            QLabel* label = new QLabel;
            label->setGeometry(0,0,pix.width(),pix.height());
            label->setPixmap(pix);
            label->setParent(this);
            label->move(57+i*50,200+j*50);

            QString str;
            if(gameArray[i][j]==1)
                str=":/res/Coin0001.png";
            else
                str=":/res/Coin0008.png";
            MyCoin* coin=new MyCoin(str);
            coin->setParent(this);
            coin->move(59+i*50,204+j*50);
            //设置金币属性
            coin->posX=i;
            coin->posY=j;
            coin->flag=this->gameArray[i][j];

            coinBtn[i][j]=coin;//方便后期判断周围金币


            connect(coin,&MyCoin::clicked,[=](){

                // flipSound->setLoopCount(10);//循环次数 -1则无限循环
                flipSound->play();

                //处理小BUG 点击将所有按钮禁止掉 后续再解禁
                for(int i=0;i<4;i++)
                    for(int j=0;j<4;j++)
                    {
                        coinBtn[i][j]->isWin=true;
                    }


                coin->changeFlag();
                this->gameArray[i][j]=this->gameArray[i][j]==0?1:0;

                QTimer::singleShot(300,this,[=](){
                    //翻周围金币

                    if(coin->posX+1<=3)
                    {
                        coinBtn[coin->posX+1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX+1][coin->posY]=this->gameArray[coin->posX+1][coin->posY]==0?1:0;
                    }
                    if(coin->posX-1>=0)
                    {
                        coinBtn[coin->posX-1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX-1][coin->posY]=this->gameArray[coin->posX-1][coin->posY]==0?1:0;
                    }
                    if(coin->posY+1<=3)
                    {
                        coinBtn[coin->posX][coin->posY+1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY+1]=this->gameArray[coin->posX][coin->posY+1]==0?1:0;
                    }
                    if(coin->posY-1>=0)
                    {
                        coinBtn[coin->posX][coin->posY-1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY-1]=this->gameArray[coin->posX][coin->posY-1]==0?1:0;
                    }

                    //周围金币翻完后解禁
                    for(int i=0;i<4;i++)
                        for(int j=0;j<4;j++)
                        {
                            coinBtn[i][j]->isWin=false;
                        }
                    //在完成所有翻转动作后 判断是否胜利
                    this->isWin=true;
                    for(int i=0;i<4;i++)
                        for(int j=0;j<4;j++)
                        {
                            if(coinBtn[i][j]->flag==false)
                            {
                                this->isWin=false;
                                break;
                            }
                        }
                    if(this->isWin==true)
                    {
                        winSound->setLoopCount(QSoundEffect::Infinite);
                        winSound->play();
                        qDebug()<<"Win";

                        for(int i=0;i<4;i++)
                            for(int j=0;j<4;j++)
                            {
                                coinBtn[i][j]->isWin=true;
                            }

                        //移动胜利图片
                        QPropertyAnimation* animation = new QPropertyAnimation(winLabel,"geometry");
                        animation->setDuration(1000);
                        animation->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
                        animation->setEndValue(QRect(winLabel->x(),winLabel->y()+175,winLabel->width(),winLabel->height()));
                        animation->setEasingCurve(QEasingCurve::OutBounce);
                        animation->start();
                    }
                });
                //判断胜利不能放在此处    原因是该语句在执行时因周围金币还没翻转完成故判断条件有误


            });

        }
    }
}


void PlayScene::paintEvent(QPaintEvent* )
{
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    QPainter painter(this);
    painter.drawPixmap(0,30,this->width(),this->height(),pix);

    pix.load(":/res/Title.png");
    pix= pix.scaled(pix.width()*0.5,pix.height()*0.5);
    painter.drawPixmap(10,30,pix);
}
