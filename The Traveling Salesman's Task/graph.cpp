#include "graph.h"
#include <QTextStream>
#include <QMessageBox>
#include "priority_queue.h"

bool flag=false;
bool flagvoy=false;
int curr_index = 0;

Graph::Graph(QWidget *parent) : QGraphicsView(parent){
    scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-500, -600, 1000, 1120);

    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(800, 900);

    connected = true;
    DFS.clear();
    BFS.clear();
}

void Graph::itemMoved(){
    if(!timerId){
        timerId = startTimer(1000 / 25);
    }
}

void Graph::timerEvent(QTimerEvent *event){
    Q_UNUSED(event);
    const QList<QGraphicsItem *> items = scene -> items();
    for(Vertex *temp : qAsConst(vertexList)){
        temp->calculate();
    }
    bool itemsMoved = false;
    for(Vertex *temp : qAsConst(vertexList)){
        if(temp->preliminaryPosition()){
            itemsMoved = true;
        }
    }

    if(!itemsMoved){
        killTimer(timerId);
        timerId = 0;
    }
}

void Graph::drawBackground(QPainter *painter, const QRectF &rect){
    Q_UNUSED(rect);

    QRectF sceneRect = this->sceneRect();
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);
    createTabWidget(rect);
}

void Graph::createTabWidget(const QRectF &rect){
    Q_UNUSED(rect);

    QRectF sceneRect = this -> sceneRect();

    QTabWidget *tab = new QTabWidget();
    tab->setGeometry(sceneRect.left() + 1, sceneRect.top(), sceneRect.width() - 1, sceneRect.height()/9);

    QWidget *vertexTab = new QWidget;
    QWidget *edgeTab = new QWidget;
    QWidget *graphTab = new QWidget;
    QWidget *voyTab = new QWidget;

    tab->setAttribute(Qt::WA_StyledBackground);

    QFont textfont = QFont();
    textfont.setPointSize(12);
    tab -> setFont(textfont);
    tab->setIconSize(QSize(30, 35));

    tab->addTab(vertexTab, tr("Вершины"));
    tab->addTab(edgeTab, tr("Рёбра"));
    tab->addTab(voyTab, tr("Коммивояжёр"));
    tab->addTab(graphTab, tr("Граф"));

    QHBoxLayout *vertexTabLayout = new QHBoxLayout;
    QPushButton *addVertexButton = new QPushButton();
    addVertexButton->setText(tr("Добавить вершину"));
    addVertexButton->setFont(textfont);
    addVertexButton->setMaximumSize(QSize(300, 50));
    addVertexButton->setStyleSheet("QPushButton:selected, QPushButton:hover{color: darkBlue;} "
                                   "QPushButton:pressed{background-color: lightGreen;} "
                                   "QPushButton{height: 30px; width: 30px; background-color: Yellow; border-right, border-bottom: 4px solid Gray;}");
    connect(addVertexButton, &QPushButton::clicked, this, &Graph::insertVertex);

    QPushButton *eraseVertexButton = new QPushButton();
    eraseVertexButton->setText(tr("Удалить вершину"));
    eraseVertexButton->setFont(textfont);
    eraseVertexButton->setMaximumSize(QSize(300, 50));
    eraseVertexButton->setStyleSheet("QPushButton:selected, QPushButton:hover{color: darkBlue;} "
                                   "QPushButton:pressed{background-color: lightGreen;} "
                                   "QPushButton{height: 30px; width: 30px; background-color: Yellow; border-right, border-bottom: 4px solid Gray;}");

    connect(eraseVertexButton, &QPushButton::clicked, this, &Graph::createEraseVertexWindow);

    vertexTabLayout->addWidget(addVertexButton);
    vertexTabLayout->addWidget(eraseVertexButton);

    QHBoxLayout *edgeTabLayout = new QHBoxLayout;
    QPushButton *addEdgeButton = new QPushButton();
    addEdgeButton->setText(tr("Добавить ребро"));
    addEdgeButton->setFont(textfont);
    addEdgeButton->setMaximumSize(QSize(200, 50));
    addEdgeButton->setStyleSheet("QPushButton:selected, QPushButton:hover{color: darkBlue;} "
                                 "QPushButton:pressed{background-color: lightGreen;} "
                                 "QPushButton{height: 30px; width: 30px; background-color: Yellow; border-right, border-bottom: 4px solid Gray;}");
    connect(addEdgeButton, &QPushButton::clicked, this, &Graph::createAddEdgeWindow);

    QPushButton *updateWeightButton = new QPushButton();
    updateWeightButton->setText(tr("Обновить вес ребра"));
    updateWeightButton->setFont(textfont);
    updateWeightButton->setMaximumSize(QSize(200, 50));
    updateWeightButton->setStyleSheet("QPushButton:selected, QPushButton:hover{color: darkBlue;} "
                                  "QPushButton:pressed{background-color: lightGreen;} "
                                  "QPushButton{height: 30px; width: 30px; background-color: Yellow; border-right, border-bottom: 4px solid Gray;}");
    connect(updateWeightButton, &QPushButton::clicked, this, &Graph::createUpdateWeightWindow);



    QPushButton *eraseEdgeButton = new QPushButton();
    eraseEdgeButton->setText(tr("Удалить ребро"));
    eraseEdgeButton->setFont(textfont);
    eraseEdgeButton->setMaximumSize(QSize(200, 50));
    eraseEdgeButton->setStyleSheet("QPushButton:selected, QPushButton:hover{color: darkBlue;} "
                                 "QPushButton:pressed{background-color: lightGreen;} "
                                 "QPushButton{height: 30px; width: 30px; background-color: Yellow; border-right, border-bottom: 4px solid Gray;}");
    connect(eraseEdgeButton, &QPushButton::clicked, this, &Graph::createEraseEdgeWindow);

    edgeTabLayout->addWidget(addEdgeButton);
    edgeTabLayout->addWidget(updateWeightButton);
    edgeTabLayout->addWidget(eraseEdgeButton);

    QHBoxLayout *graphTabLayout = new QHBoxLayout;

    QToolButton *functionButton = new QToolButton();
    functionButton->setText(tr("Методы"));
    functionButton->setFont(textfont);
    functionButton->setMaximumSize(QSize(150, 50));
    functionButton->setStyleSheet("QToolButton:selected, QToolButton:hover{color: darkBlue;} "
                             "QToolButton:pressed{background-color: lightGreen;} "
                             "QToolButton{height: 30px; width: 30px; background-color: Yellow; border-right, border-bottom: 4px solid Gray;}");
    QMenu *menu = new QMenu;
    menu->setFixedWidth(200);

    QAction *dfs = new QAction(tr("Обход в глубину"), this);
    dfs->setStatusTip(tr("Обход в глубину"));
    connect(dfs, &QAction::triggered, this, &Graph::createDFSWindow);
    menu->addAction(dfs);
    menu->addSeparator();

    QAction *bfs = new QAction(tr("Обход в ширину"), this);
    bfs->setStatusTip(tr("Обход в ширину"));
    connect(bfs, &QAction::triggered, this, &Graph::createBFSWindow);
    menu->addAction(bfs);
    menu->addSeparator();

    QAction *dijkstra = new QAction(tr("Алгоритм Дейкстры"), this);
    dijkstra->setStatusTip(tr("Алгоритм Дейкстры"));
    connect(dijkstra, &QAction::triggered, this, &Graph::createDijkstraWindow);
    menu->addAction(dijkstra);
    menu->addSeparator();

    functionButton->setPopupMode(QToolButton::InstantPopup);
    functionButton->setMenu(menu);

    QPushButton *resetButton = new QPushButton();
    resetButton->setText(tr("Сбросить"));
    resetButton->setFont(textfont);
    resetButton->setMaximumSize(QSize(150, 50));
    resetButton->setStyleSheet("QPushButton:selected, QPushButton:hover{color: darkBlue;} "
                             "QPushButton:pressed{background-color: lightGreen;} "
                             "QPushButton{height: 30px; width: 30px; background-color: Yellow; border-right, border-bottom: 4px solid Gray;}");
    connect(resetButton, &QPushButton::clicked, this, &Graph::reset);

    QPushButton *clearButton = new QPushButton();
    clearButton->setText(tr("Очистить"));
    clearButton->setFont(textfont);
    clearButton->setMaximumSize(QSize(150, 50));
    clearButton->setStyleSheet("QPushButton:selected, QPushButton:hover{color: darkBlue;} "
                             "QPushButton:pressed{background-color: lightGreen;} "
                             "QPushButton{height: 30px; width: 30px; background-color: Yellow; border-right, border-bottom: 4px solid Gray;}");
    connect(clearButton, &QPushButton::clicked, this, &Graph::clear);

    QPushButton *functionsInfoButton = new QPushButton();
    functionsInfoButton->setText(tr("Информация о методах"));
    functionsInfoButton->setFont(textfont);
    functionsInfoButton->setMaximumSize(QSize(200, 50));
    functionsInfoButton->setStyleSheet("QPushButton:selected, QPushButton:hover{color: darkBlue;} "
                             "QPushButton:pressed{background-color: lightGreen;} "
                             "QPushButton{height: 30px; width: 30px; background-color: Yellow; border-right, border-bottom: 4px solid Gray;}");
    connect(functionsInfoButton, &QPushButton::clicked, this, &Graph::getFunctionsInfo);

    QPushButton *matrixInfoButton = new QPushButton();
    matrixInfoButton->setText(tr("Матрица смежности"));
    matrixInfoButton->setFont(textfont);
    matrixInfoButton->setMaximumSize(QSize(200, 50));
    matrixInfoButton->setStyleSheet("QPushButton:selected, QPushButton:hover{color: darkBlue;} "
                             "QPushButton:pressed{background-color: lightGreen;} "
                             "QPushButton{height: 30px; width: 30px; background-color: Yellow; border-right, border-bottom: 4px solid Gray;}");
    connect(matrixInfoButton, &QPushButton::clicked, this, &Graph::getMatrixInfo);

    QHBoxLayout *voyTabLayout = new QHBoxLayout;
    QPushButton *findShortPath = new QPushButton();
    findShortPath->setText(tr("Кратчайший путь"));
    findShortPath->setFont(textfont);
    findShortPath->setMaximumSize(QSize(220, 50));
    findShortPath->setStyleSheet("QPushButton:selected, QPushButton:hover{color: darkBlue;} "
                                 "QPushButton:pressed{background-color: lightGreen;} "
                                 "QPushButton{height: 30px; width: 30px; background-color: Yellow; border-right, border-bottom: 4px solid Gray;}");

    QLineEdit *shortestPath=new QLineEdit;
    shortestPath->setMaximumSize(QSize(600, 50));

    connect(findShortPath,&QPushButton::clicked,this,[this,shortestPath](){createTravellingSalesmanWindow(shortestPath);});

    QPushButton *clearLineButton = new QPushButton();
    clearLineButton->setText(tr("Очистить"));
    clearLineButton->setFont(textfont);
    clearLineButton->setMaximumSize(QSize(220, 50));
    clearLineButton->setStyleSheet("QPushButton:selected, QPushButton:hover{color: darkBlue;} "
                                 "QPushButton:pressed{background-color: lightGreen;} "
                                 "QPushButton{height: 30px; width: 30px; background-color: Yellow; border-right, border-bottom: 4px solid Gray;}");
    connect(clearLineButton,&QPushButton::clicked,this,[this,shortestPath](){clearLine(shortestPath);});
    connect(clearLineButton,&QPushButton::clicked,this,&Graph::reset);

    graphTabLayout->addWidget(functionButton);
    graphTabLayout->addWidget(resetButton);
    graphTabLayout->addWidget(clearButton);
    graphTabLayout->addWidget(functionsInfoButton);
    graphTabLayout->addWidget(matrixInfoButton);
    voyTabLayout->addWidget(findShortPath);
    voyTabLayout->addWidget(shortestPath);
    voyTabLayout->addWidget(clearLineButton);

    vertexTab->setLayout(vertexTabLayout);
    edgeTab->setLayout(edgeTabLayout);
    graphTab->setLayout(graphTabLayout);
    voyTab->setLayout(voyTabLayout);

    scene->addWidget(tab);

}

int Graph::getVertexNum(){
    return vertexList.size();
}

void Graph::scaleView(qreal scaleFactor){
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if(factor < 0.07 || factor > 100){
        return;
    }
    scale(scaleFactor, scaleFactor);
}

void Graph::insertVertex(){
    int vertexIndex = vertexList.size();
    vertexList.append(new Vertex(this, vertexIndex));
    vertexList[vertexIndex] -> setPos(0, 50);
    scene->addItem(vertexList[vertexIndex]);
    vertexNum++;
}

void Graph::updateIndex(){
    for(int i{0}; i < vertexList.size(); i++){
        vertexList[i] -> setVertexIndex(i);
    }
}

void Graph::eraseVertex(int vertexIndex){
    Vertex *temp = vertexList[vertexIndex];
    for(Edge *edge : temp->getEdges()){
        eraseEdge(vertexIndex, edge->destVertex()->getVertexIndex());
    }
    temp->clearEdge();
    scene->removeItem(temp);
    edgeNum -= vertexList[vertexIndex]->getEdges().size()*2;
    vertexList.erase(vertexList.begin() + vertexIndex);
    updateIndex();
    vertexNum--;
}

void Graph::getFunctionsInfo()
{
    int rows = vertexList.size() + 8;
    int cols = vertexList.size() + 1;
    QTableWidget *infoWindow = new QTableWidget(rows, cols);

    QString dfs = "Обход в глубину с индекса ";
    if(!DFS.empty()){
        dfs += QString::number(DFS[0]);
        for(int i{0}; i < DFS.size(); i++){
            infoWindow->setItem(0, i+1, new QTableWidgetItem(QString::number(DFS[i])));
        }
    }
    infoWindow->setItem(0, 0, new QTableWidgetItem(dfs));

    QString bfs = "Обход в ширину с индекса ";
    if(!BFS.empty()){
        bfs += QString::number(BFS[0]);
        for(int i{0}; i < BFS.size(); i++){
            infoWindow->setItem(1, i+1, new QTableWidgetItem(QString::number(BFS[i])));
        }
    }
    infoWindow->setItem(1, 0, new QTableWidgetItem(bfs));

    QString dijkstra1 = "Алгоритм Дейкстры с индекса ";
    if(flag==true && dijkstraSignal()!=-1)
    {
        QVector<int> k=dijkstra(dijkstraSignal());
        dijkstra1+= QString::number(dijkstraSignal());
        for(int i{0}; i < k.size(); i++){
            infoWindow->setItem(2, i+1, new QTableWidgetItem(QString::number(k[i])));
        }
    }
    infoWindow->setItem(2, 0, new QTableWidgetItem(dijkstra1));

    infoWindow->setWindowTitle("Информация о методах");
    infoWindow->setColumnWidth(0, 175);
    infoWindow->setFixedSize(QSize(700, 800));
    infoWindow->show();
}

void Graph::getMatrixInfo(){
    int rows = vertexList.size() + 8;
    int cols = vertexList.size() + 1;
    QTableWidget *infoWindow = new QTableWidget(rows, cols);

    infoWindow->setItem(0, 0, new QTableWidgetItem("Матрица смежности"));
    infoWindow->item(0, 0)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    for(int i{0}; i < vertexNum; i++){
        infoWindow->setItem(0, i+1, new QTableWidgetItem(QString::number(i)));
        infoWindow->item(0, i+1)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    }

    for(int i{0}; i < vertexNum; i++){
        infoWindow->setItem(1+i, 0, new QTableWidgetItem(QString::number(i)));
        infoWindow->item(1+i, 0)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        for(int j{0}; j < vertexNum; j++){
            infoWindow->setItem(1+i, 1+j, new QTableWidgetItem("0"));
            infoWindow->item(1+i, 1+j)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        }
        for(Edge *edge : vertexList[i]->getEdges()){
            int pos = edge->destVertex()->getVertexIndex();
            infoWindow->setItem(1+i, 1+pos, new QTableWidgetItem(QString::number(edge->getWeightofEdge())));
            infoWindow->item(1+i, 1+pos)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        }
        infoWindow->setColumnWidth(i+1, 40);
    }
    infoWindow->setWindowTitle("Матрица смежности");
    infoWindow->setColumnWidth(0, 200);
    infoWindow->setFixedSize(QSize(600, 700));
    infoWindow->show();
}

void Graph::insertEdge(int source, int dest, double weight){
    if(checkAdjacent(source, dest)){
        return;
    } else {
        scene->addItem(new Edge(vertexList[source], vertexList[dest], weight));
        scene->addItem(new Edge(vertexList[dest], vertexList[source], weight));
        edgeNum += 2;
    }
}

void Graph::updateWeight(int source, int dest, double weight){
    if(!checkAdjacent(source, dest)){
        return;
    }
    vertexList[source]->updateEdgeWeight(dest, weight);
    vertexList[dest]->updateEdgeWeight(source, weight);
}

void Graph::eraseEdge(int source, int dest){
    if(!checkAdjacent(source, dest)){
        scene->addText(tr("Не соединены"));
        return;
    }
    for(Edge *edge : vertexList[source]->getEdges()){
        if(edge -> destVertex() ->getVertexIndex() == dest){
            vertexList[source]->eraseEdge(dest);
            scene->removeItem(edge);
        }
    }
    for(Edge *edge : vertexList[dest]->getEdges()){
        if(edge -> destVertex() ->getVertexIndex() == source){
            vertexList[dest]->eraseEdge(source);
            scene->removeItem(edge);
        }
    }
    edgeNum -= 2;
}

bool Graph::checkAdjacent(int source, int dest){
    return vertexList[source] -> pathExist(dest);
}

int Graph::getDegree(int vertexIndex){
    return vertexList[vertexIndex]->getVertexDegree();
}

void Graph::clear(){
    while(!vertexList.empty()){
        eraseVertex(0);
    }
}

void Graph::clearLine(QLineEdit * line)
{
    line->clear();
}

void Graph::checkConnected(){
    connected = true;
    for(Vertex *vertex : vertexList){
        bool visited = false;
        if(vertex->getVertexColor() == "black"){
            visited = true;
        }
        connected &= visited;
    }
}

void Graph::reset(){
    if(vertexList.size() == 0){
        return;
    }
    for(int i{0}; i < vertexList.size(); i++){
        vertexList[i] -> setColor("white");
        for(Edge *edge : vertexList[i]->getEdges()){
            edge -> setColor("white");
        }
    }
}

void Graph::DepthTraversal(int vertexIndex, QVector<bool> &visited){
    QStack<int> dfsStack;
    dfsStack.push(vertexIndex);
    while(!dfsStack.empty()){
        int temp = dfsStack.top();
        Vertex *tempVertex = vertexList[temp];
        dfsStack.pop();
        if(!visited[temp]){
            visited[temp] = true;
            vertexList[temp]->setColor("black");
            DFS.push_back(temp);
            delay(100);
        }
        for(Edge *tempEdge : tempVertex->getEdges()){
            if(!visited[tempEdge -> destVertex() -> getVertexIndex()]){
                tempEdge->destVertex()->setColor("gray");
                delay(140);
                dfsStack.push(tempEdge->destVertex()->getVertexIndex());
            }
        }
    }
}

void Graph::dfs(int vertexIndex){
    DFS.clear();
    QVector<bool> visited(vertexList.size(), false);
    DepthTraversal(vertexIndex, visited);
    checkConnected();
}

void Graph::BreadthTraversal(int vertexIndex, QVector<bool> &visited){
    QQueue<int> bfsQueue;
    bfsQueue.push_back(vertexIndex);
    while(!bfsQueue.empty()){
        int temp = bfsQueue.front();

        Vertex *tempVertex = vertexList[temp];
        bfsQueue.pop_front();
        if(!visited[temp]){
            visited[temp] = true;
            tempVertex->setColor("black");
            BFS.push_back(temp);
            delay(100);
        }
        for(Edge *tempEdge : tempVertex->getEdges()){
            if(!visited[tempEdge -> destVertex() -> getVertexIndex()]){
                tempEdge->destVertex()->setColor("gray");
                bfsQueue.push_back(tempEdge->destVertex()->getVertexIndex());
                delay(140);
            }
        }
    }
}

void Graph::bfs(int vertexIndex){
    BFS.clear();
    QVector<bool> visited(vertexList.size(), false);
    BreadthTraversal(vertexIndex, visited);
    checkConnected();
}

QVector<int> Graph::dijkstra(int vertexIndex){
    int Adj_matrix[vertexNum][vertexNum];
    for(int i=0;i<vertexNum;i++){
        for(int j=0;j<vertexNum;j++){
            if(i==j) Adj_matrix[i][j] = 0;
            else Adj_matrix[i][j] = 100000000;
        }
    }
    PriorityQueue<int> queue;
    for(int i=0;i<vertexNum;i++){
        queue.enqueue(Adj_matrix[vertexIndex][i],i);
    }
    while(queue.count()>0){
        int Curr_vertex = queue.dequeue();
        int edge_count = vertexList[Curr_vertex]->getEdges().count();
        for(int i=0;i<edge_count;i++){
            Edge* e = vertexList[Curr_vertex]->getEdges()[i];
            Vertex* dest = e->destVertex();
            if(Adj_matrix[vertexIndex][Curr_vertex]+e->getWeightofEdge()<Adj_matrix[vertexIndex][dest->getVertexIndex()]){
                int old = Adj_matrix[vertexIndex][dest->getVertexIndex()];
                Adj_matrix[vertexIndex][dest->getVertexIndex()] = Adj_matrix[vertexIndex][Curr_vertex]+e->getWeightofEdge();
                queue.update(old,dest->getVertexIndex(),Adj_matrix[vertexIndex][dest->getVertexIndex()]);
            }
        }
    }
    QVector<int> new_vector;
    for(int i=0;i<vertexNum;i++){
        new_vector.append(Adj_matrix[vertexIndex][i]);
    }
    return new_vector;
}

int getHighScore(QVector<QVector<int>> matrix, int posi, int posj,int n){
    int minj = matrix[posi][0];
    int mini = matrix[0][posj];
    for(int j=1;j<n;j++)
    {
        if(j!=posj && matrix[posi][j]<minj)
        {
            minj = matrix[posi][j];
        }
    }
    for(int i=1;i<n;i++)
    {
        if(i!=posi && matrix[i][posj]<mini)
        {
            mini = matrix[i][posj];
        }
    }
    return mini + minj;
}

void Graph::travellingSalesman(int vertexIndex,QLineEdit *line)
{
    //Сброс цвета у стартового индекса
    for(int i=0;i<vertexList.size();i++)
    {
        vertexList[i]->setColor("white");
    }
    //Сброс цвета маршрута
    for(Vertex* v:vertexList)
    {
        for(Edge* e: v->getEdges())
        {
            e->setColor("white");
        }
    }
    QVector<QPair<int,int>> res;//результат,куда будет записываться путь в виде пар,например (1,2),(3,4),4(2) и т.д
    QVector<QVector<int>> matrix;//матрица смежности
    //Инициализация матрицы смежности
    for(int i=0;i<vertexNum;i++)
    {
        QVector<int> vec;
        matrix.push_back(vec);
        for(int j=0;j<vertexNum;j++)
        {
            matrix[i].push_back(0);
        }
    }
    int minRow[vertexNum];//массив минимального значения по строкам
    int minCol[vertexNum];//массив минимального значения по столбцам
    int HighScoreMatrix[vertexNum][vertexNum];//Матрица оценок
    //Заполняем матрицу оценок
    for(int i=0;i<vertexNum;i++)
    {
        for(int j=0;j<vertexNum;j++)
        {
            HighScoreMatrix[i][j]=-1;
        }
    }
    //Заполняем матрицу смежности
    for(int i=0;i<vertexNum;i++)
    {
        for(int j=0;j<vertexNum;j++)
        {
            if(i==j || !vertexList[i]->pathExist(j))
            {
                matrix[i][j]=100000000;
            }
            else
            {
                matrix[i][j]=vertexList[i]->getEdge(j);
            }
        }
    }
    //Цикл,если после первого обхода не получится составить кратчайший путь
    for(int k=0;k<vertexNum;k++)
    {
        //Заполняем матрицу оценок
        for(int i=0;i<vertexNum;i++)
        {
            for(int j=0;j<vertexNum;j++)
            {
                HighScoreMatrix[i][j]=-1;
            }
        }
        //Ищем минимальное значение в каждой строке
        for(int i=0;i<vertexNum;i++)
        {
            minRow[i]=matrix[i][0];
            for(int j=1;j<vertexNum;j++)
            {
                if(matrix[i][j]<minRow[i])
                {
                    minRow[i]=matrix[i][j];
                }
            }
            if(minRow[i]==100000000)
            {
                minRow[i]=0;
            }
        }
        //Редукция
        for(int i=0;i<vertexNum;i++)
        {
            for(int j=0;j<vertexNum;j++)
            {
                if(matrix[i][j]!=100000000)
                {
                    matrix[i][j]-=minRow[i];
                }
            }
        }
        //Ищем максимальное значение в каждом столбце
        for(int i=0;i<vertexNum;i++)
        {
            minCol[i]=matrix[0][i];
            for(int j=1;j<vertexNum;j++)
            {
                if(matrix[j][i]<minCol[i])
                {
                    minCol[i]=matrix[j][i];
                }
            }
            if(minCol[i]==100000000)
            {
                minCol[i]=0;
            }
        }
        //Редукция
        for(int i=0;i<vertexNum;i++)
        {
            for(int j=0;j<vertexNum;j++)
            {
                if(matrix[j][i]!=100000000)
                {
                    matrix[j][i]-=minCol[i];
                }
            }
        }
        //Ищем оценку для каждого нуля
        for(int i=0;i<vertexNum;i++)
        {
            for(int j=0;j<vertexNum;j++)
            {
                if(matrix[i][j]==0)
                {
                    HighScoreMatrix[i][j] = getHighScore(matrix,i,j,vertexNum);
                }
            }
        }
        //Координаты нуля с наибольшей оценкой
        int maxNull = -1;
        int maxNulli = 0;
        int maxNullj = 0;
        //Ищем наибольшую оценку
        for(int i=0;i<vertexNum;i++)
        {
            for(int j=0;j<vertexNum;j++)
            {
                if(HighScoreMatrix[i][j]!=-1 && HighScoreMatrix[i][j]>maxNull)
                {
                    maxNull = HighScoreMatrix[i][j];
                    maxNulli = i;
                    maxNullj = j;
                }
            }
        }
        //Редукция
        for(int i=0;i<vertexNum;i++)
        {
            matrix[i][maxNullj] = 100000000;
        }
        for(int j=0;j<vertexNum;j++)
        {
            matrix[maxNulli][j] = 100000000;
        }
        matrix[maxNullj][maxNulli] = 100000000;
        QPair<int,int> p;
        p.first=maxNulli;
        p.second=maxNullj;
        res.push_back(p);
    }
    //Вектор,в котором в правильном порядке выстраивается кратчайший путь
    int path[vertexNum+1];
    for(int i=0;i<vertexNum+1;i++)
    {
        path[i]=0;
    }
    path[0]=vertexIndex;
    int curr = vertexIndex;
    for(int i=0;i<vertexNum;i++)
    {
        for(int j=0;j<vertexNum;j++)
        {
            if(res[j].first==curr)
            {
                curr = res[j].second;
                path[i+1]=curr;
                break;
            }
        }
    }
    vertexList[vertexIndex]->setColor("red");
    //Цвет ребер кратчайшего пути меняется на красный
    for(int i=0;i<vertexNum;i++)
    {
        for(Edge *e: vertexList[path[i]]->getEdges())
        {
            if(e->destVertex()==vertexList[path[i+1]])
            {
                e->setColor("red");
            }
        }
    }
    //Переменная,в которую запишется сумма кратчайшего пути
    int sum=0;
    for(int i=0;i<vertexNum;i++)
    {
        sum+=vertexList[path[i]]->getEdge(path[i+1]);
    }
    QString str;
    str+= QString::number(path[0]);
    for(int i=1;i<vertexNum+1;i++)
    {
        str+= " -> " + QString::number(path[i]);
    }
    str += ". Итоговый путь составил " + QString::number(sum);
    line->setText(str);
}

void Graph::delay(int time){
    clock_t now = clock();
    while(clock() - now < time);
}

void Graph::createEraseVertexWindow(){
    window = new QWidget;
    input1 = new QLineEdit;
    QLabel *label = new QLabel;
    label->setFrameStyle(QFrame::Box | QFrame::Plain);
    QPushButton *okButton = new QPushButton(tr("OK"));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(new QLabel(tr("Индекс удаляемой вершины:")), 0, 0);
    layout->addWidget(input1, 0, 1);
    layout->addWidget(okButton, 1, 1, Qt::AlignRight);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    window->setLayout(layout);
    window->setWindowTitle("Удаление вершины");

    connect(okButton, &QPushButton::clicked, this, &Graph::eraseVertexSignal);
    window->show();
}

void Graph::eraseVertexSignal(){
    int srcIndex = input1->text().toInt();
    window -> close();
    if(srcIndex >= vertexList.size()){
        return;
    }
    eraseVertex(srcIndex);
}

void Graph::createAddEdgeWindow(){
    window = new QWidget;
    input1 = new QLineEdit;
    input2 = new QLineEdit;
    input3 = new QLineEdit;
    QLabel *label = new QLabel;
    label->setFrameStyle(QFrame::Box | QFrame::Plain);
    QPushButton *okButton = new QPushButton(tr("OK"));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(new QLabel(tr("Индекс исходной вершины:")), 0, 0);
    layout->addWidget(input1, 0, 1);
    layout->addWidget(new QLabel(tr("Индекс конечной вершины:")), 1, 0);
    layout->addWidget(input2, 1, 1);
    layout->addWidget(new QLabel(tr("Вес ребра:")), 2, 0);
    layout->addWidget(input3, 2, 1);
    layout->addWidget(okButton, 3, 1, Qt::AlignRight);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    window->setLayout(layout);
    window->setWindowTitle("Добавление ребра");

    connect(okButton, &QPushButton::clicked, this, &Graph::addEdgeSignal);
    window->show();
}

void Graph::addEdgeSignal(){
    int srcIndex = input1->text().toInt();
    int destIndex = input2->text().toInt();
    double edgeWeight = input3->text().toDouble();
    window->close();
    if(srcIndex >= vertexList.size() || destIndex >= vertexList.size() || edgeWeight <= 0){
        return;
    }
    insertEdge(srcIndex, destIndex, edgeWeight);
}

void Graph::createEraseEdgeWindow(){
    window = new QWidget;
    input1 = new QLineEdit;
    input2 = new QLineEdit;
    QLabel *label = new QLabel;
    label->setFrameStyle(QFrame::Box | QFrame::Plain);
    QPushButton *okButton = new QPushButton(tr("OK"));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(new QLabel(tr("Индекс исходной вершины:")), 0, 0);
    layout->addWidget(input1, 0, 1);
    layout->addWidget(new QLabel(tr("Индекс конечной вершины:")), 1, 0);
    layout->addWidget(input2, 1, 1);
    layout->addWidget(okButton, 2, 1, Qt::AlignRight);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    window->setLayout(layout);
    window->setWindowTitle("Удаление ребра");

    connect(okButton, &QPushButton::clicked, this, &Graph::eraseEdgeSignal);
    window->show();
}

void Graph::eraseEdgeSignal(){
    int srcIndex = input1->text().toInt();
    int destIndex = input2->text().toInt();
    window->close();
    if(srcIndex >= vertexList.size() || destIndex >= vertexList.size()){
        return;
    }
    eraseEdge(srcIndex, destIndex);
}

void Graph::createUpdateWeightWindow(){
    window = new QWidget;
    input1 = new QLineEdit;
    input2 = new QLineEdit;
    input3 = new QLineEdit;
    QLabel *label = new QLabel;
    label->setFrameStyle(QFrame::Box | QFrame::Plain);
    QPushButton *okButton = new QPushButton(tr("OK"));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(new QLabel(tr("Индекс исходной вершины:")), 0, 0);
    layout->addWidget(input1, 0, 1);
    layout->addWidget(new QLabel(tr("Индекс конечной вершины:")), 1, 0);
    layout->addWidget(input2, 1, 1);
    layout->addWidget(new QLabel(tr("Вес ребра:")), 2, 0);
    layout->addWidget(input3, 2, 1);
    layout->addWidget(okButton, 3, 1, Qt::AlignRight);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    window->setLayout(layout);
    window->setWindowTitle("Обновление веса ребра");

    connect(okButton, &QPushButton::clicked, this, &Graph::updateWeightSignal);
    window->show();
}

void Graph::updateWeightSignal(){
    int srcIndex = input1->text().toInt();
    int destIndex = input2->text().toInt();
    double edgeWeight = input3->text().toDouble();
    window->close();
    if(srcIndex >= vertexList.size() || destIndex >= vertexList.size() || edgeWeight <= 0){
        return;
    }
    updateWeight(srcIndex, destIndex, edgeWeight);
}

void Graph::createDFSWindow(){
    window = new QWidget;
    input1 = new QLineEdit;
    QLabel *label = new QLabel;
    label->setFrameStyle(QFrame::Box | QFrame::Plain);
    QPushButton *okButton = new QPushButton(tr("OK"));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(new QLabel(tr("Индекс стартовой вершины:")), 0, 0);
    layout->addWidget(input1, 0, 1);
    layout->addWidget(okButton, 1, 1, Qt::AlignRight);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    window->setLayout(layout);
    window->setWindowTitle("Обход в глубину");

    connect(okButton, &QPushButton::clicked, this, &Graph::dfsSignal);
    window->show();
}

void Graph::dfsSignal(){
    int index = input1->text().toInt();
    window->close();
    if(index >= vertexList.size()){
        return;
    }
    dfs(index);
}

void Graph::createBFSWindow(){
    window = new QWidget;
    input1 = new QLineEdit;
    QLabel *label = new QLabel;
    label->setFrameStyle(QFrame::Box | QFrame::Plain);
    QPushButton *okButton = new QPushButton(tr("OK"));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(new QLabel(tr("Индекс стартовой вершины:")), 0, 0);
    layout->addWidget(input1, 0, 1);
    layout->addWidget(okButton, 1, 1, Qt::AlignRight);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    window->setLayout(layout);
    window->setWindowTitle("Обход в ширину");

    connect(okButton, &QPushButton::clicked, this, &Graph::bfsSignal);
    window->show();
}

void Graph::createDijkstraWindow()
{
    window = new QWidget;
    input1 = new QLineEdit;
    QLabel *label = new QLabel;
    label->setFrameStyle(QFrame::Box | QFrame::Plain);
    QPushButton *okButton = new QPushButton(tr("OK"));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(new QLabel(tr("Индекс стартовой вершины:")), 0, 0);
    layout->addWidget(input1, 0, 1);
    layout->addWidget(okButton, 1, 1, Qt::AlignRight);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    window->setLayout(layout);
    window->setWindowTitle("Алгоритм Дейкстры");

    connect(okButton, &QPushButton::clicked, this, &Graph::dijkstraSignal);
    window->show();
}

void Graph::createTravellingSalesmanWindow(QLineEdit* line)
{
    window = new QWidget;
    input1 = new QLineEdit;
    QLabel *label = new QLabel;
    label->setFrameStyle(QFrame::Box | QFrame::Plain);
    QPushButton *okButton = new QPushButton(tr("OK"));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(new QLabel(tr("Индекс стартовой вершины:")), 0, 0);
    layout->addWidget(input1, 0, 1);
    layout->addWidget(okButton, 1, 1, Qt::AlignRight);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    window->setLayout(layout);
    window->setWindowTitle("Задача Коммивояжёра");
    connect(okButton,&QPushButton::clicked,this,[this,line](){TravellingSalesmanSignal(line);});
    window->show();
}

void Graph::bfsSignal(){
    int index = input1->text().toInt();
    window->close();
    if(index >= vertexList.size()){
        return;
    }
    bfs(index);
}

int Graph::dijkstraSignal()
{
    flag=true;
    int index=input1->text().toInt();
    window->close();
    if(index >= vertexList.size()){
        return -1;
    }
    return index;
}

int Graph::TravellingSalesmanSignal(QLineEdit* line)
{
    flagvoy=true;
    int index=input1->text().toInt();
    window->close();
    if(index >= vertexList.size()){
        return -1;
    }
    curr_index = index;
    travellingSalesman(index,line);
    return index;
}
