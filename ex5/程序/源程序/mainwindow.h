#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<mapwidget.h>
#include <QMainWindow>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QVector>
#include <QMouseEvent>
#include <QDialog>
#include <QPixmap>
#include <QGridLayout>
#include <QLineEdit>
#include <QBasicTimer>
#include <QFileDialog>
#include <QHBoxLayout>

class QMyLabel;
class ComboBox : public QComboBox
{
    friend class MainWindow;
    Q_OBJECT
public:
    explicit ComboBox(QWidget* parent = nullptr);

protected:

    void keyReleaseEvent(QKeyEvent *e);
signals:
    void keyEnterReleased();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void createToolBar();
    void createAction();
    void setStart(int X, int Y);
    void setEnd(int X, int Y);
    void setNextPos (int index);
    void initScene();
public slots:
    void setStartStation();
    void setEndStation();
    void FindPath();
    void Clear();
    void callOtherMap();
    void swift();
    void addStartComboBox();
    void addEndComboBox();
    double similarity(const QString& s, const QVector<QString>&t);
    QVector<int>similarest(const QString& s, const QVector<QVector<QString>>v);
private:
    MapWidget *mapWidget;
    QLabel *startLabel;
    QLabel *endLabel;
    QAction *findPathAction;
    QAction *clearAction;
    QAction *callMap;
    QAction*swiftAction;
    QGraphicsScene *scene;
    QGraphicsView *view;
    bool stt=0;
    int startX, startY, endX, endY;
    QVector<int> nextPath;

    /*
     * 图的实现，和最短路径算法声明
     */
    struct ArcCell{    //弧信息
        int adj;       //对无权图有1，0表示是否相邻，对带权图，则为权值类型
    };


    //内部类
    static const int MAX_VERTEX_NUM = 50;
    static const int INF = 999999;

    struct MGraph{
        QVector<int> vexs;                                    //顶点集合
        //临接矩阵
        ArcCell arcs[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
        int vexnum;                                           //顶点数
        int arcnum;                                           //边数
    };

    class DijkstraFindPath
    {
    public:
        DijkstraFindPath();
        MGraph mgraph;
        void CreateGraph();

        int prev[MAX_VERTEX_NUM];  //最短路上的前驱顶点
        int d[MAX_VERTEX_NUM];     //表示边e = (u,v)的权值(不存在时为INF,不过d[i][i]=0)
        bool used[MAX_VERTEX_NUM]; //已经使用过的图
        void dijkstra(int startPos);      //求从起点startPos出发到各个顶点的最短距离
        QVector<int> get_Path(int endPos);//到顶点endPos的最短路
        QVector<QVector<int>>Location;
        QVector<QVector<QString>>names;
    };

    ComboBox *startComboBox;
    ComboBox *endComboBox;
    DijkstraFindPath *dj;

//鼠标事件
protected:
    void mouseDoubleClickEvent (QMouseEvent *e);
    void mousePressEvent(QMouseEvent*e);
public:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
private:
    QString strPath;
    QLabel *label;
    QGraphicsSimpleTextItem *l;
    QGraphicsSimpleTextItem *r;
    int lv=-1;
    int rv=-1;
    int lx=-1;
    int rx=-1;
    QBasicTimer m_timer;
    int m_nStep=0;
    int dfg=0;
};
#endif // MAINWINDOW_H
