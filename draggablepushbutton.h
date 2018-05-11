/*******************************************************
 *
 *  The draggable push button class header file
 *
 *  Written by Jari Korhonen, Shenzhen University, 2018
 *
 *******************************************************/

#ifndef DRAGGABLEPUSHBUTTON_H
#define DRAGGABLEPUSHBUTTON_H

#include <QPushButton>
#include <QMimeData>
#include <QDrag>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
//#include "myguiwidget.h"

/* MAIN WIDGET */
class DraggablePushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit DraggablePushButton(QWidget *parent, int thisNum);
    int rankNum() { return rankn; }
    void setRankNum(int rn) { rankn=rn; }

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private:
    QWidget *myParent;
    int buttonNum;
    int rankn;
};

#endif // DRAGGABLEPUSHBUTTON_H
