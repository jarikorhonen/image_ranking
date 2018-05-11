/*******************************************************
 *
 *  The draggable push button class source file
 *
 *  This class implements the functionality for the
 *  draggable push buttons that are used to define
 *  the rank order of the test images
 *
 *
 *  Written by Jari Korhonen, Shenzhen University, 2018
 *
 *******************************************************/

#include "draggablepushbutton.h"
#include "myguiwidget.h"

DraggablePushButton::DraggablePushButton(QWidget * parent, int num)
{
    myParent = parent;
    buttonNum = num;
    rankn = num;

    QSizePolicy sp_retain = sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);
    setSizePolicy(sp_retain);

    setAcceptDrops(true);
}

void DraggablePushButton::mousePressEvent(QMouseEvent *event)
{
    click();
    mouseMoveEvent(event);
}

void DraggablePushButton::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;

    if (!isChecked())
        return;

    QDrag *drag = new QDrag(this);
    QPixmap pixmap = this->grab();
    QMimeData *mimeData = new QMimeData;


    drag->setPixmap(pixmap);
    drag->setHotSpot(event->pos());
    drag->setMimeData(mimeData);

    rankn = buttonNum;

    setHidden(true);
    repaint();

    Qt::DropAction dropAction = drag->exec(Qt::MoveAction);

    // Delete later, if there is no drop event.
    if( dropAction == Qt::IgnoreAction ){
       drag->deleteLater();
    }

    setHidden(false);
    ((myGuiWidget *)myParent)->setImageLabels();

}

void DraggablePushButton::dropEvent(QDropEvent *event)
{
    DraggablePushButton *sourceButton = (DraggablePushButton *)event->source();

    if (sourceButton == nullptr){
        event->ignore();
        return;
    }

    if(buttonNum<sourceButton->rankNum()){
        for(int i=sourceButton->rankNum()-buttonNum; i>0; i--)
            ((myGuiWidget *)myParent)->moveUp();

    } else if (buttonNum>sourceButton->rankNum()){
        for(int i=buttonNum-sourceButton->rankNum(); i>0; i--)
            ((myGuiWidget *)myParent)->moveDown();

    }

    setHidden(false);
    sourceButton->setHidden(false);
    repaint();

    event->acceptProposedAction();
}

void DraggablePushButton::dragEnterEvent(QDragEnterEvent *event)
{
    DraggablePushButton *sourceButton = (DraggablePushButton *)event->source();

    if (sourceButton == nullptr){
        event->ignore();
        return;
    }

    if (buttonNum<sourceButton->rankNum()){
        for(int i=sourceButton->rankNum()-buttonNum; i>0; i--)
            ((myGuiWidget *)myParent)->moveUp();

    } else if (buttonNum>sourceButton->rankNum()){
        for(int i=buttonNum-sourceButton->rankNum(); i>0; i--)
            ((myGuiWidget *)myParent)->moveDown();

    }

    setHidden(true);
    sourceButton->setRankNum(buttonNum);

    event->accept();
}

void DraggablePushButton::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}
