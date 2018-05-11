/*******************************************************
 *
 *  The image widget class header file
 *
 *  Written by Jari Korhonen, Shenzhen University, 2018
 *
 *******************************************************/

#ifndef MYIMAGEWIDGET_H
#define MYIMAGEWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>

#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>

/* IMAGE WIDGET */
class myImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit myImageWidget(QWidget *parent = 0);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    int selectedImage;
    QGridLayout layout;
    QGridLayout mainscrlayout;
    QLabel image;
    QLabel mainscrimage;
    QImage grayimage;
    QVector<QImage> images;

    QWidget mainscrwidget;

//protected:
//    void keyPressEvent(QKeyEvent * e);

signals:

public slots:
    void updateImages( QStringList paths );
    void updateImage( int index );
};

#endif // MYIMAGEWIDGET_H
