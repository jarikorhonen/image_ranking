/*******************************************************
 *
 *  The GUI widget class header file
 *
 *  Written by Jari Korhonen, Shenzhen University, 2018
 *
 *******************************************************/

#ifndef MYGUIWIDGET_H
#define MYGUIWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QGraphicsSimpleTextItem>
#include <QComboBox>
#include <QPushButton>
#include <QCommandLinkButton>
#include <QGridLayout>
#include <QWidget>
#include <QDockWidget>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QMessageBox>
#include <QInputDialog>
#include <QGlobal.h>
#include <QDateTime.h>
#include <QSizePolicy>
#include <QMimeData>
#include <QDrag>
#include "myimagewidget.h"
#include "draggablepushbutton.h"

/* MAIN WIDGET */
class myGuiWidget : public QWidget
{
    Q_OBJECT
public:
    explicit myGuiWidget(QWidget *parent = 0);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QFile file[100];
    QString fname[100][100];
    qint8 m_numPracticeTests;
    qint8 m_numTests;
    qint8 m_numFiles[50];
    qint8 m_currentTest;
    qint8 order[50];
    qint8 image_permutation[50];
    qint8 testcase_permutation[50];
    bool m_randomOrder;
    bool seen[50];
    qint64 seed;
    qint8 m_currSelection;
    QDateTime dateTime;
    QDateTime startDateTime;
    QStringList selectedFiles;
    QFileDialog fileDialog;
    DraggablePushButton *caseButton[15];
    QPushButton *selectButton;
    QPushButton *upButton;
    QPushButton *downButton;
    QLabel *lblBest;
    QLabel *lblWorst;
    QGridLayout *layout;
    myImageWidget imageWidget; 
    QPoint dragStartPosition;
    QPushButton *draggedButton;

    void makeGui();
    void closeEvent(QCloseEvent *event);
    void parseConfigFile(QString configfilepath);
    void saveStartTime(QString resultfilepath);
    void saveCurrentSelections(QString resultsfilepath, QString csfilepath);
    void saveEndTime(QString resultfilepath);
    void makeImagePermutation(qint8 num, qint8 currtest);
    void makeTestcasePermutation(qint16 num);

signals:
    
public slots:
    void selectNextCase();
    void moveUp();
    void moveDown();
    void changeCurrent();
    void setImageLabels();
    
};

#endif // MYGUIWIDGET_H
