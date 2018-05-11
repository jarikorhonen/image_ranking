/*******************************************************
 *
 *  The image widget class source file
 *
 *  This class basically just displays the test images
 *
 *
 *  Written by Jari Korhonen, Shenzhen University, 2018
 *
 *******************************************************/

#include "myimagewidget.h"

myImageWidget::myImageWidget(QWidget *parent) :
    QWidget(parent),
    image(),
    mainscrimage(),
    images()
{
    layout.addWidget(&image, 1, 1, 1, 1);
    layout.setContentsMargins(0,0,0,120);
    layout.setAlignment(Qt::AlignCenter);
    this->setStyleSheet("color: white; background-color: gray");
    this->setLayout(&layout);

    //int screenCount = QApplication::desktop()->screenCount();
    int primary = QApplication::desktop()->primaryScreen();
    int screen = primary;

    //grayimage = QImage(GRAYIMAGEPATH);

    /*if( screenCount==2 ) {
        screen = 1 - primary;*/

        QRect screenr = QApplication::desktop()->screenGeometry(primary);
        mainscrwidget.move(QPoint(screenr.x(), screenr.y()));
        resize(screenr.width(), screenr.height()-120);

        mainscrlayout.addWidget(&mainscrimage, 1, 1, 1, 1);
        mainscrlayout.setMargin(0);

        mainscrwidget.setLayout(&mainscrlayout);
        //QLayout::setAlignment (&mainscrlayout, Qt::AlignCenter );
        //mainscrlayout.setAlignment(Qt::AlignCenter);

        //mainscrimage.setPixmap( QPixmap::fromImage(grayimage) );
    //}

    QRect screenres = QApplication::desktop()->screenGeometry(screen);
    this->move(QPoint(screenres.x(), screenres.y()));

    //grayimage = QImage(GRAYIMAGEPATH);

    setAcceptDrops(true);

    return;
}

void myImageWidget::updateImage( int index )
{
    selectedImage = index;
    image.setPixmap( QPixmap::fromImage(images.value(selectedImage)) );
    return;
}

void myImageWidget::updateImages( QStringList list )
{
    images.clear();
    for(int i=0; i<list.count(); i++)
        images.append( QImage(list.at(i)) );
    this->updateImage(0);
    return;
}

void myImageWidget::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void myImageWidget::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}

void myImageWidget::dropEvent(QDropEvent *event)
{
    event->accept();
}

void myImageWidget::mouseMoveEvent(QMouseEvent *event)
{
    event->accept();
}

void myImageWidget::mousePressEvent(QMouseEvent *event)
{
    event->accept();
}
