#ifndef GRAPH_H
#define GRAPH_H

#include <vertex.h>
#include <edge.h>

#include <ctime>
#include <iomanip>

#include <QMainWindow>
#include <QGraphicsItem>
#include <QVector>
#include <QStack>
#include <QQueue>
#include <QPointF>
#include <QGraphicsView>
#include <QWidget>
#include <QStyleOptionGraphicsItem>
#include <QKeyEvent>
#include <QPushButton>
#include <QHBoxLayout>
#include <QStyle>
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QWidget>
#include <QTableWidget>
#include <QApplication>
#include <QGroupBox>
#include <QToolButton>
#include <QMenu>

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <sstream>

class Vertex;
class Edge;
class Graph;

class Graph : public QGraphicsView {
    Q_OBJECT

public:
    Graph(QWidget *parent = nullptr);
    void itemMoved();
    int getVertexNum();

protected:
    void timerEvent(QTimerEvent *event) override;

    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void createTabWidget(const QRectF &rect);
    void createMenuBar();
    void scaleView(qreal scaleFactor);

    void updateIndex();
    void insertVertex();
    void eraseVertex(int vertexIndex);
    void insertEdge(int source, int dest, double weight);
    void updateWeight(int source, int dest, double weight);
    void eraseEdge(int source, int dest);
    bool checkAdjacent(int source, int dest);
    int getDegree(int vertexIndex);
    void clear();
    void clearLine(QLineEdit*);
    void checkConnected();

    void reset();

    void getFunctionsInfo();
    void getMatrixInfo();

    void DepthTraversal(int vertexIndex, QVector<bool> &visited);
    void dfs(int vertexIndex);

    void BreadthTraversal(int vertexIndex, QVector<bool> &visited);
    void bfs(int vertexIndex);

    void travellingSalesman(int,QLineEdit*);

    void delay(int time);

    void createEraseVertexWindow();
    void createAddEdgeWindow();
    void createEraseEdgeWindow();
    void createUpdateWeightWindow();
    void createDFSWindow();
    void createBFSWindow();
    void createDijkstraWindow();
    void createTravellingSalesmanWindow(QLineEdit*);

private slots:
    void eraseVertexSignal();
    void addEdgeSignal();
    void eraseEdgeSignal();
    void updateWeightSignal();
    void dfsSignal();
    void bfsSignal();
    int dijkstraSignal();
    int TravellingSalesmanSignal(QLineEdit*);

private:
    unsigned int edgeNum=0;
    int vertexNum;
    bool connected;

    QVector<int> DFS;
    QVector<int> BFS;
    QVector<int> dijkstra(int);

    QVector<Vertex *> vertexList;
    QGraphicsScene *scene;
    int timerId = 0;

    QWidget *window;
    QLineEdit *input1;
    QLineEdit *input2;
    QLineEdit *input3;

};

#endif // GRAPH_H
