/*******************************************************
 *
 *  The main source file
 *
 *  Just the entry point to the test software
 *
 *
 *  Written by Jari Korhonen, Shenzhen University, 2018
 *
 *******************************************************/

#include <stdio.h>
#include <iostream>
#include <QApplication>
#include <QWidget>
#include "myguiwidget.h"

int main(int argv, char **args)
{
    QApplication app(argv, args);

    myGuiWidget gui;

    /* Execute */
    gui.show();

    return app.exec();
}
