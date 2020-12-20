#include "mainwindow.h"
#include <qdebug.h>
#include <QToolBar>
#include <QtAlgorithms>
#include <iostream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle ("华中大校园地图导航");
    setWindowIcon(QPixmap("../data/华中科技大学.png"));
    this->setStyleSheet("background-color:#66ccff");
    dj = new MainWindow::DijkstraFindPath(); //√
    dj->CreateGraph ();//创建图的邻接表  //√
    scene = new QGraphicsScene;
    scene->setSceneRect (-838, -410, 1676,820);
    initScene();
    view = new QGraphicsView;
    view->setScene (scene);
    view->setMinimumSize (800, 800);
    view->show ();
    setCentralWidget (view);

    createAction ();
    createToolBar ();           //实现一个工具栏
    setMinimumSize (800, 800);  //设置最小尺寸
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);
    setFixedSize(1680,868);
}

void MainWindow::createToolBar ()
{
    QToolBar *toolBar = addToolBar ("Tool");

    const QString str1 ="QComboBox \{\
            border: none;   /* 边框 */\
            color: 	#FFB6C1;\
            background:  #FFFAFA;\
    }";
    const QString str2 = "QComboBox QAbstractItemView\
    {\
    background: #66ccff;\
    border-radius: 5px;\
    font: 20px \"微软雅黑\";\
    color: #ffffff;\
    outline:none;\
    }";
    startLabel = new QLabel(tr("起点:"));
    startLabel->setStyleSheet("color:#9932CC;");
    startComboBox = new ComboBox;
    endComboBox = new ComboBox;
    startComboBox->setEditable(true);
    startComboBox->setFixedWidth(400);
    startComboBox->setFixedHeight(28);
    startComboBox->setStyleSheet(str2+str1);
    endComboBox->setFixedWidth(400);
    endComboBox->setStyleSheet(str2+str1);
    endComboBox->setFixedHeight(28);
    startComboBox->clear();
    endComboBox->clear();
    connect(startComboBox, SIGNAL(keyEnterReleased()), this, SLOT(addStartComboBox()));
    connect(endComboBox,SIGNAL(keyEnterReleased()),this,SLOT(addEndComboBox()));

    endLabel = new QLabel(tr("终点: "));
    endComboBox->setEditable(true);
    endLabel->setStyleSheet("color:#9932CC;");
    connect (startComboBox, SIGNAL(activated(int)), this, SLOT(setStartStation()));
    connect (endComboBox, SIGNAL(activated(int)), this, SLOT(setEndStation()));
    toolBar->setStyleSheet("background-color:#E6E6FA");
    toolBar->addWidget (startLabel);
    toolBar->addWidget (startComboBox);
    toolBar->addSeparator ();
    toolBar->addWidget (endLabel);
    toolBar->addWidget (endComboBox);
    toolBar->addSeparator ();
    toolBar->addAction(findPathAction);
    toolBar->addSeparator ();
    toolBar->addAction(clearAction);
    toolBar->addSeparator ();
    toolBar->addAction(callMap);
    toolBar->addSeparator();
    toolBar->addAction(swiftAction);


}


MainWindow::DijkstraFindPath::DijkstraFindPath()
{
    mgraph.vexnum = 25;                        //初始化点数目
       for (int i = 0; i < mgraph.vexnum; i++) //初始化点编号
           mgraph.vexs.push_back (i);
       mgraph.arcnum = 32;                     //暂定
       for (int i = 0; i < mgraph.vexnum; i++) {
           for (int j = 0; j < mgraph.vexnum; j++) {
               if (i == j)
                   mgraph.arcs[i][j].adj = 0;
               else
                   mgraph.arcs[i][j].adj = INF;
   //            mgraph.arcs[i][j].info = "";
           }
       }
}

void MainWindow::DijkstraFindPath::CreateGraph ()
{

        mgraph.arcs[0][1].adj = mgraph.arcs[1][0].adj = 74;
        mgraph.arcs[0][6].adj = mgraph.arcs[6][0].adj = 46;
        mgraph.arcs[1][2].adj = mgraph.arcs[2][1].adj = 33;
        mgraph.arcs[1][18].adj = mgraph.arcs[18][1].adj = 30;
        mgraph.arcs[2][17].adj = mgraph.arcs[17][2].adj = 28;
        mgraph.arcs[3][4].adj = mgraph.arcs[4][3].adj = 56;
        mgraph.arcs[3][15].adj = mgraph.arcs[15][3].adj = 61;
        mgraph.arcs[3][17].adj = mgraph.arcs[17][3].adj = 65;
        mgraph.arcs[3][21].adj = mgraph.arcs[21][3].adj = 70;
        mgraph.arcs[4][5].adj = mgraph.arcs[5][4].adj = 28;
        mgraph.arcs[5][8].adj = mgraph.arcs[8][5].adj = 48;
        mgraph.arcs[6][16].adj = mgraph.arcs[16][6].adj = 74;
        mgraph.arcs[6][23].adj = mgraph.arcs[23][6].adj = 38;
        mgraph.arcs[7][10].adj = mgraph.arcs[10][7].adj = 48;
        mgraph.arcs[7][12].adj = mgraph.arcs[12][7].adj = 30;
        mgraph.arcs[8][21].adj = mgraph.arcs[21][8].adj = 47;
        mgraph.arcs[24][9].adj = mgraph.arcs[9][24].adj = 59;
        mgraph.arcs[10][11].adj = mgraph.arcs[11][10].adj = 38;
        mgraph.arcs[10][14].adj = mgraph.arcs[14][10].adj = 40;
        mgraph.arcs[12][15].adj = mgraph.arcs[15][12].adj = 41;
        mgraph.arcs[12][24].adj = mgraph.arcs[24][12].adj = 71;
        mgraph.arcs[13][14].adj = mgraph.arcs[14][13].adj = 25;
        mgraph.arcs[13][15].adj = mgraph.arcs[15][13].adj = 52;
        mgraph.arcs[13][19].adj = mgraph.arcs[19][13].adj = 43;
        mgraph.arcs[14][20].adj = mgraph.arcs[20][14].adj = 45;
        mgraph.arcs[15][16].adj = mgraph.arcs[16][15].adj = 51;
        mgraph.arcs[16][18].adj = mgraph.arcs[18][16].adj = 17;
        mgraph.arcs[17][18].adj = mgraph.arcs[18][17].adj = 42;
        mgraph.arcs[19][21].adj = mgraph.arcs[21][19].adj = 30;
        mgraph.arcs[20][22].adj = mgraph.arcs[22][20].adj = 39;
        mgraph.arcs[21][22].adj = mgraph.arcs[22][21].adj = 26;
        mgraph.arcs[23][24].adj = mgraph.arcs[24][23].adj = 50;
        QGraphicsPathItem *item = new QGraphicsPathItem();
        //初始化坐标
        Location.resize(25);
        for(int i =0;i!=25;++i){
            Location[i].resize(2);
        }
        Location[0][0]=152;Location[0][1]=-27;
        Location[1][0]=78;Location[1][1]=-34;
        Location[2][0]=45;Location[2][1]=-30;
        Location[3][0]=29;Location[3][1]=-123;
        Location[4][0]=-26;Location[4][1]=-112;
        Location[5][0]=-33;Location[5][1]=-140;
        Location[6][0]=159;Location[6][1]=-73;
        Location[7][0]=133;Location[7][1]=-170;
        Location[8][0]=-27;Location[8][1]=-188;
        Location[9][0]=261;Location[9][1]=-111;
        Location[10][0]=138;Location[10][1]=-204;
        Location[11][0]=176;Location[11][1]=-201;
        Location[12][0]=131;Location[12][1]=-126;
        Location[13][0]=93;Location[13][1]=-177;
        Location[14][0]=98;Location[14][1]=-215;
        Location[15][0]=90;Location[15][1]=-132;
        Location[16][0]=85;Location[16][1]= -95;
        Location[17][0]=39;Location[17][1]=-58;
        Location[18][0]=81;Location[18][1]=-64;
        Location[19][0]=50;Location[19][1]=-190;
        Location[20][0]=53;Location[20][1]=-223;
        Location[21][0]=20;Location[21][1]=-188;
        Location[22][0]=14;Location[22][1]= -222;
        Location[23][0]=197;Location[23][1]=-68;
        Location[24][0]=202;Location[24][1]=-118;

        //初始化names
        names.push_back({ "华中科技大学正门" });  names.push_back({ "华中科技大学南二门" });
        names.push_back({ "华中科技大学南三门" });  names.push_back({ "化学学院" });
        names.push_back({ "煤燃烧实验室" });  names.push_back({ "紫菘学生公寓" });
        names.push_back({ "计算机学院","电信学院" });  names.push_back({ "图书馆" });
        names.push_back({ "西一学生食堂" });  names.push_back({ "人文学院","法学院" });
        names.push_back({ "科技楼","友谊公寓"});  names.push_back({ "教工活动中心","印刷厂" });
        names.push_back({ "计算中心","华中教育网中心" });  names.push_back({ "电信收费中心","档案馆" });
        names.push_back({ "八号楼","招待所" });  names.push_back({ "静园" });
        names.push_back({ "南三楼","西五楼" });  names.push_back({ "西十二楼" });
        names.push_back({ "建筑学院" });  names.push_back({ "西5舍" });
        names.push_back({ "西边博士公寓" });  names.push_back({ "西6舍" });
        names.push_back({ "西学超市" });  names.push_back({ "图像防伪中心" });
        names.push_back({ "醉晚亭" });
}

void MainWindow::DijkstraFindPath::dijkstra (int startPos)
{
    for (int i = 0; i < mgraph.vexnum; i++) d[i] = INF;
    for (int i = 0; i < mgraph.vexnum; i++) used[i] = false;
    for (int i = 0; i < mgraph.vexnum; i++) prev[i] = -1;
    d[startPos] = 0;

    while (true) {
        int v = -1;
        for (int u = 0; u < mgraph.vexnum; u++) {
            if (!used[u] && (v == -1 || d[u] < d[v])) v = u;
        }

        if (v == -1) break;
        used[v] = true;

        for (int u = 0; u < mgraph.vexnum; u++) {
            if (d[u] > d[v] + mgraph.arcs[v][u].adj) {
                d[u] = d[v] + mgraph.arcs[v][u].adj;
                prev[u] = v;
            }
        }
    }
}

QVector<int> MainWindow::DijkstraFindPath::get_Path (int endPos)
{
    QVector<int> path;

    for ( ; endPos != -1; endPos = prev[endPos]) {
//        std::cout << "EndPos: " << endPos << ", ";
        path.push_back (endPos);
    }

    std::reverse(path.begin (), path.end ());

    return path;
}

void MainWindow::initScene ()
{
    m_timer.start(100,this);
    QGraphicsPixmapItem *item =
            scene->addPixmap((QPixmap("../data/map.jpg")));
//    item->setFlag (QGraphicsItem::ItemIsMovable);
    item->setPos(-838,-410);
        for(int i=0;i!=25;++i){
                    QString temp;
                    for(int j=0;j!=dj->names[i].size();++j){
                        temp+=dj->names[i][j];
                        if(j+1!=dj->names[i].size()){
                            temp+='\n';
                        }
                    }
                    QLabel *L=new QLabel();
                    L->setPixmap(QPixmap("../data/红点.png"));
                    L->setToolTip(temp);
                  //  L->show();
                    L->move(dj->Location[i][0]-4,dj->Location[i][1]-6);
                    scene->addWidget(L);
        }
}


MainWindow::~MainWindow()
{

}

void MainWindow::createAction ()
{
    findPathAction = new QAction(QIcon("../data/路线.png"),tr("搜索路径"), this);
    findPathAction->setShortcut (tr("Ctrl+F"));
    findPathAction->setStatusTip (tr("搜索路径"));
    connect (findPathAction, SIGNAL(triggered(bool)), this, SLOT(FindPath()));

    clearAction = new QAction(QIcon("../data/清屏.png"), tr("清理路径"), this);
    clearAction->setShortcut (tr("Ctrl+W"));
    clearAction->setStatusTip (tr("清理路径"));
    connect (clearAction, SIGNAL(triggered(bool)), this, SLOT(Clear()));

    callMap = new QAction(QIcon("../data/定位.png"), tr("调用测试地图"), this);
    callMap->setShortcut (tr("Ctrl+M"));
    callMap->setStatusTip (tr("调用测试地图"));
    connect (callMap, SIGNAL(triggered(bool)), this, SLOT(callOtherMap()));

    swiftAction=new QAction(QIcon("../data/切换.png"),tr("切换"),this);
    swiftAction->setStatusTip("切换");
    connect(swiftAction,SIGNAL(triggered(bool)),this,SLOT(swift()));
}



void MainWindow::setStart(int X, int Y) {
    startX = X; startY = Y;
//    qDebug() << X << ", " << Y;
}

void MainWindow::setEnd (int X, int Y)
{
    endX = X; endY = Y;
}

void MainWindow::setStartStation ()
{
    int i=startComboBox->currentIndex();
    setStart(dj->Location[i][0],dj->Location[i][1]);
}

void MainWindow::setEndStation ()
{
    int i=endComboBox->currentIndex();
    setEnd(dj->Location[i][0],dj->Location[i][1]);
}

void MainWindow::setNextPos (int index)
{
    setEnd(dj->Location[index][0],dj->Location[index][1]);
}

void MainWindow::FindPath ()
{
    //Demo 在图片上绘线 在原有基础上 (+700, +440);
    QVector<QPoint> v;
    if(rv!=-1&&lv!=-1){
        startX=dj->Location[rv][0];
        startY=dj->Location[rv][1];
        dj->dijkstra(rv);
        nextPath=dj->get_Path(lv);
    }
    else{
        auto x=startComboBox->currentText().toStdString();;
        int n1,n2;
        if(x[1]-'0'>=0&&x[1]-'0'<10){
          n1=x[1]-'0'+10*(x[0]-'0');
        }
        else
            n1=x[0]-'0';

        x=endComboBox->currentText().toStdString();
        if(x[1]-'0'>=0&&x[1]-'0'<10){
          n2=x[1]-'0'+10*(x[0]-'0');
        }
        else n2=x[0]-'0';
        startX=dj->Location[n2][0];
        startY=dj->Location[n2][1];
        dj->dijkstra (n2);
        nextPath = dj->get_Path (n1);
    }
    //准备绘制
    Clear ();
    //将路线绘制下来
    QGraphicsPathItem *item = new QGraphicsPathItem();

    QPen pen;
    pen.setWidth (4);
    pen.setColor (Qt::blue);
    item->setPen (pen);
    item->setFlag (QGraphicsItem::ItemIsPanel);


    for (int i = 1; i < nextPath.size (); i++) {
        qDebug() << nextPath[i] << " , ";
    }
    scene->addItem (item);

    QPainterPath pa;               //path

    pa.moveTo (startX , startY );

    //将路径用坐标存入到路径
    for (int i = 1; i < nextPath.size() ; i++) {
        setNextPos (nextPath[i]);
        pa.lineTo (endX , endY );
    }
    item->setPath (pa);

}

void MainWindow::Clear ()
{
    dfg=1;
    rx=lx=-1;
    QList<QGraphicsItem*> listItem = scene->items ();
    while (!listItem.empty ())
    {
        scene->removeItem (listItem.at (0));
        listItem.removeAt (0);
    }
    QGraphicsPixmapItem *item=
            scene->addPixmap (QPixmap("../data/map.jpg"));
    item->setPos (-838, -410);
    for(int i=0;i!=25;++i){
                QString temp;
                for(int j=0;j!=dj->names[i].size();++j){
                    temp+=dj->names[i][j];
                    if(j+1!=dj->names[i].size()){
                        temp+='\n';
                    }
                }
                QLabel *L=new QLabel();
                L->setPixmap(QPixmap("../data/红点.png"));
                L->setToolTip(temp);
                //L->show();
                L->move(dj->Location[i][0]-4,dj->Location[i][1]-6);
                scene->addWidget(L);
    }

    rv=-1;
    lv=-1;

}
void MainWindow:: swift(){
    stt=!stt;
}

void MainWindow::timerEvent(QTimerEvent *event){
    Q_UNUSED(event);
        if(dfg==1){
        if (event->timerId() == m_timer.timerId())
        {
            ++m_nStep;
            update();
        }
        else
        {
            timerEvent(event);
        }
        }
}


void MainWindow::mouseDoubleClickEvent(QMouseEvent *e){
    if(stt){
    int mx = e->x()-841;
    int my = e->y()-453;
    double min=500;
    int res=-1;
    for(int i=0;i!=25;++i){
           auto tt=(mx-dj->Location[i][0])*(mx-dj->Location[i][0])+(my-dj->Location[i][1])*(my-dj->Location[i][1]);
           if(tt<min){
               min=tt;
               res=i;
           }
     }
    if(lx==-1){
        lx=res;
    }
    else if(rx==-1){
        rx=res;
        QGraphicsPathItem *item = new QGraphicsPathItem();
        QPen pen;
        int nIndex = (m_nStep +1) % 16;
        QColor c;
        c.setHsv((15 - nIndex) * 16, 255, 191);
        pen.setWidth (4);
        pen.setColor(c);
        item->setPen (pen);
        item->setFlag (QGraphicsItem::ItemIsPanel);
        scene->addItem (item);
        QPainterPath pa;               //path
        pa.moveTo (dj->Location[lx][0], dj->Location[lx][1] );
        pa.lineTo (dj->Location[rx][0] , dj->Location[rx][1] );
        item->setPath (pa);
        dfg=1;
    }
    else {
        dfg=1;
        lx=rx;
        rx=res;
        QGraphicsPathItem *item = new QGraphicsPathItem();
        QPen pen;
        int nIndex = (m_nStep + rx) % 16;
        QColor c;
        c.setHsv((15 - nIndex) * 16, 255, 191);
        pen.setWidth (4);
        pen.setColor(c);
        item->setPen (pen);
        item->setFlag (QGraphicsItem::ItemIsPanel);
        scene->addItem (item);
        QPainterPath pa;               //path
        pa.moveTo (dj->Location[lx][0], dj->Location[lx][1] );
        pa.lineTo (dj->Location[rx][0] , dj->Location[rx][1] );
        item->setPath (pa);
        }
    }
}

//鼠标事件
void MainWindow::mousePressEvent (QMouseEvent *e)
{
    if(!stt){
    if(e->button() == Qt::LeftButton )
    {
        if(lv==-1){
        int mx = e->x()-841;
        int my = e->y()-453;
        double min=500;
        int res=-1;
        for(int i=0;i!=25;++i){
               auto tt=(mx-dj->Location[i][0])*(mx-dj->Location[i][0])+(my-dj->Location[i][1])*(my-dj->Location[i][1]);
               if(tt<min){
                   min==tt;
                   res=i;
               }
         }
        QString temp;
        if(res!=-1){
            for(int j=0;j!=dj->names[res].size();++j){
                temp+=dj->names[res][j];
                if(j+1!=dj->names[res].size())
                temp+="\n      ";
            }
            lv=res;
        }
            l = new QGraphicsSimpleTextItem();
            l->setText("起点："+temp);
            l->setPos(-800,300);
            QFont font = l->font();
            font.setPixelSize(20);  // 像素大小
            font.setItalic(true);  // 斜体
            font.setUnderline(false);  // 下划线
            l->setBrush(QBrush(QColor("#ff00ff")));
            l->setFont(font);
            scene->addItem(l);}
        else Clear();
    }
    else if(e->button() == Qt::RightButton )
    {
        if(rv==-1){
        int mx = e->x()-841;
        int my = e->y()-453;
        double min=500;
        int res=-1;
        for(int i=0;i!=25;++i){
               auto tt=(mx-dj->Location[i][0])*(mx-dj->Location[i][0])+(my-dj->Location[i][1])*(my-dj->Location[i][1]);
               if(tt<min){
                   min==tt;
                   res=i;
               }
         }
        QString temp;
        if(res!=-1){
            for(int j=0;j!=dj->names[res].size();++j){
                temp+=dj->names[res][j];
                if(j+1!=dj->names[res].size())
                temp+="\n      ";
            }
            rv=res;
        }
        r = new QGraphicsSimpleTextItem();
        r->setText("终点："+temp);
        r->setPos(-500,300);
        QFont font = r->font();
        font.setPixelSize(20);  // 像素大小
        font.setItalic(true);  // 斜体
        font.setUnderline(false);  // 下划线
        r->setBrush(QBrush(QColor("#ff00ff")));
        r->setFont(font);
        scene->addItem(r);}
        else Clear();
    }
  }
}
void MainWindow::callOtherMap ()
{
    mapWidget = new MapWidget;
    mapWidget->show ();
}


double MainWindow::similarity(const QString& s, const QVector<QString>&t){
    double res = 0;
    auto m = s.size();
    auto b = t.size();
    for (int c = 0; c != b; ++c) {
        double u = 0;
        auto n = t[c].size();
        int max = 0;
        int i = 0; int j = 0;
        while (i < m && j < n) {
            if (s[i] == t[c][j]) {
                ++i; ++j;
                max++;
            }
            else {
                ++j;
            }
        }
        if(max==m)
        if ((u = max*1.0 / m + max*1.0 / n) > res)  res = u;
    }
    return  res;
}

QVector<int> MainWindow::similarest(const QString& s, const QVector<QVector<QString>>v) {
    QVector<QPair<int, double>>t;
    QVector<int>res;
    for (int i = 0; i < v.size(); ++i) {
        double p;
        if ((p = similarity(s, v[i])) > 0) {
            t.push_back({ i, p });
        }
    }
    qSort(t.begin(), t.end(), [](QPair<int, double>t1, QPair<int, double>t2) {return t1.second > t2.second; });
    for (int i = 0; i != t.size(); ++i) {
        res.push_back(t[i].first);
    }
    return res;
}

 ComboBox::ComboBox(QWidget *parent) : QComboBox (parent)
{

}

void ComboBox:: keyReleaseEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
    {
        qDebug() << "enter released";
        emit keyEnterReleased();

    }
}
void MainWindow :: addStartComboBox(){
    QString nowStartText=startComboBox->currentText();
    startComboBox->clear();
     QVector<int> qViStart=similarest(nowStartText,dj->names);

   for(int i=0;i!=qViStart.size();i++){
       QString temp=QString::number(qViStart[i],10);
       temp+=":";
       for(int j=0;j!=dj->names[qViStart[i]].size();j++){
           temp+=dj->names[qViStart[i]][j];
           if(j+1!=dj->names[qViStart[i]].size()){
               temp+=" |";
           }
       }
       startComboBox->addItem((temp));
   }
}

void MainWindow::addEndComboBox(){
    QString nowEndText=endComboBox->currentText();
    endComboBox->clear();
    QVector<int> qViEnd=similarest(nowEndText,dj->names);
    for(int i=0;i!=qViEnd.size();i++){
        QString temp=QString::number(qViEnd[i],10);
        temp+=":";
        for(int j=0;j!=dj->names[qViEnd[i]].size();j++){
            temp+=dj->names[qViEnd[i]][j];
            if(j+1!=dj->names[qViEnd[i]].size()){
                temp+=" |";
            }
        }
        endComboBox->addItem((temp));
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    if(dfg==1&&rx!=-1&&lx!=-1){
    Q_UNUSED(event);
    QGraphicsPathItem *item = new QGraphicsPathItem();
    QPen pen;
    int nIndex = (m_nStep + 1) % 16;
    QColor c;
    c.setHsv((15 - nIndex) * 16, 255, 191);
    pen.setWidth (4);
    pen.setColor(c);
    item->setPen (pen);
    item->setFlag (QGraphicsItem::ItemIsPanel);
    scene->addItem (item);
    QPainterPath pa;               //path
    pa.moveTo (dj->Location[lx][0], dj->Location[lx][1] );
    pa.lineTo (dj->Location[rx][0] , dj->Location[rx][1] );
    item->setPath (pa);
    }
}
