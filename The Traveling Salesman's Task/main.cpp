#include "graph.h"
#include <windows.h>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Graph w;
    w.show();
    return a.exec();
}
