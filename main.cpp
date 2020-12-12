#include "mainwindow.h"
#include <iostream>
#include <QApplication>
#include "world.h"
#include "controller.h"
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    controller controller;
    return a.exec();

}
