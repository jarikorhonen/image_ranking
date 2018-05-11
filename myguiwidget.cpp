/*******************************************************
 *
 *  The main GUI widget class source file
 *
 *  Most of the functionality of the test software is
 *  here: it creates GUI, reads the test setup from
 *  the config file, writes the results in the result
 *  file etc. etc.
 *
 *
 *  Written by Jari Korhonen, Shenzhen University, 2018
 *
 *******************************************************/

#include "myguiwidget.h"

//
//   Change the hardcoded paths to the preferred ones
//
#define CONFIGFILEPATH "f:\\rank_config.txt"
#define RESULT_LOG_FILE "f:\\rank_log.txt"
#define RESULT_CS_FILE "f:\\rank_result.csv"
#define NUM_PRACTICE_TESTS 1

/* MAIN WIDGET */
myGuiWidget::myGuiWidget(QWidget *parent) :
    QWidget(parent),
    selectedFiles(),
    fileDialog(),
    imageWidget()
{
    m_randomOrder = false;
    seed = 0;
    bool ok;
    QString text = QInputDialog::getText(this,
                                         ("Image order randomization"),
                                         ("Give seed number:"),
                                         QLineEdit::Normal,
                                         QString::null,&ok);
    if (ok && !text.isEmpty()){
        m_randomOrder = true;
        seed = text.toInt();
        qsrand(seed);
    }

    layout = NULL;
    m_currentTest = 0;
    m_numPracticeTests = NUM_PRACTICE_TESTS;

    parseConfigFile(CONFIGFILEPATH);

    imageWidget.showFullScreen();
    makeGui();
    saveStartTime(QString(RESULT_LOG_FILE));

    setAcceptDrops(true);
}

void myGuiWidget::makeGui(){

    // make a layout with all the buttons
    qint8 i;

    // first destroy the old layout (if any)
    if(layout != NULL){
        while(layout->count() > 0) {
            QWidget* widget = layout->itemAt(0)->widget();
            layout->removeWidget(widget);
            delete widget;
        }
        delete layout;
    }

    QRect screenr = QApplication::desktop()->screenGeometry(-1);

    resize(screenr.width(), 150);
    move(QPoint(0, screenr.height()-150));

    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    setSizePolicy(sizePolicy);
    setMinimumSize(QSize(screenr.width(), 150));
    setMaximumSize(QSize(screenr.width(), 150));

    // make grid layout
    layout = new QGridLayout();
    layout->setHorizontalSpacing(20);
    layout->setContentsMargins(50, 10, 50, 10);
    selectedFiles.clear();

    // permute to show images in random order
    makeImagePermutation(m_numFiles[testcase_permutation[m_currentTest]],
                         m_currentTest);

    // set "the best" label
    lblBest = new QLabel("The Best");

    QFont font = lblBest->font();
    font.setPointSize(14);
    font.setBold(true);
    lblBest->setFont(font);
    lblBest->setStyleSheet("color: rgb(100,100,100)");

    lblBest->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(lblBest, 1, 1, 1, 1);


    // make new buttons
    for(i=0; i<m_numFiles[testcase_permutation[m_currentTest]]; i++) {

        // make a button for each image
        caseButton[i] = new DraggablePushButton(this, i);
        layout->addWidget(caseButton[i], 1, i+2, 1, 1);
        caseButton[i]->setCheckable(true);
        caseButton[i]->setStyleSheet("color: white; background-color: rgb(150,100,100)");

        // set large bolded font
        caseButton[i]->setFont(font);

        selectedFiles << fname[testcase_permutation[m_currentTest]][image_permutation[i]];
        order[i]=i;
        seen[i]=false;
    }

    lblWorst = new QLabel("The Worst");
    lblWorst->setFont(font);
    lblWorst->setStyleSheet("color: rgb(100,100,100)");
    lblWorst->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(lblWorst, 1, i+2, 1, 1);

    selectButton = new QPushButton("&Next Case");
    selectButton->setStyleSheet("color: rgb(80,80,80); background-color: gray");
    selectButton->setFont(font);

    layout->addWidget(selectButton, 2, i+6, 1, 1);

    // select button disabled until all the images have been seen
    selectButton->setEnabled(false);

    setLayout(layout);

    QObject::connect(selectButton, SIGNAL(clicked()), this, SLOT(selectNextCase()));

    m_currSelection = 0;
    setImageLabels();

    imageWidget.updateImages( selectedFiles );

    for(i=0; i<m_numFiles[testcase_permutation[m_currentTest]]; i++)
        QObject::connect(caseButton[i], SIGNAL(clicked()), this, SLOT(changeCurrent()));

    setPalette( QPalette( Qt::gray ) );
    setAutoFillBackground( true );
    setWindowFlags( Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );

    draggedButton = nullptr;
}

void myGuiWidget::makeImagePermutation(qint8 num, qint8 currtest){

    qint8 i;

    // initialize as image_permutation[] = {0,1,2, ... ,num-1}
    for(i=0; i<num; i++)
        image_permutation[i]=i;

    // if no randomization, just return
    if(!m_randomOrder || m_numPracticeTests>currtest)
        return;

    for(i=0; i<num-1; i++){

        // swap values with the current and random remaining position
        qint8 pickPos = qrand() % (num - i) + i;
        qint8 pickPosVal = image_permutation[pickPos];
        image_permutation[pickPos] = image_permutation[i];
        image_permutation[i] = pickPosVal;
    }
}

void myGuiWidget::makeTestcasePermutation(qint16 num){

    qint8 i;

    // initialize as testcase_permutation[] = {0,1,2, ... ,num-1}
    for(i=0; i<num; i++)
        testcase_permutation[i]=i;

    // if no randomization, just return
    if(!m_randomOrder)
        return;

    // practice tests are shown in fixed order
    for(i=m_numPracticeTests;i<num-1;i++){

        // swap values with the current and random remaining position
        qint8 pickPos = qrand() % (num - i) + i;
        qint8 pickPosVal = testcase_permutation[pickPos];
        testcase_permutation[pickPos] = testcase_permutation[i];
        testcase_permutation[i] = pickPosVal;
    }

}


void myGuiWidget::selectNextCase(void){

    // save the result in a file
    saveCurrentSelections(QString(RESULT_LOG_FILE),QString(RESULT_CS_FILE));

    // go to the next case
    if(m_currentTest<m_numTests-1){

        m_currentTest++;
        makeGui();
    }
    // or if in the end of the test, just quit
    else{
        // show a "test finished" message
        saveEndTime(QString(RESULT_LOG_FILE));
        QMessageBox msgBox;
        msgBox.setText("The test is finished. Thank you!");
        msgBox.exec();

        if(layout != NULL){
            while(layout->count() > 0) {
                QWidget* widget = layout->itemAt(0)->widget();
                layout->removeWidget(widget);
                delete widget;
            }
            delete layout;
            this->hide();
        }

        // it's time to leave the scene
        exit(0);
    }
    return;
}

void myGuiWidget::closeEvent(QCloseEvent *event){
    imageWidget.close();
}

void myGuiWidget::setImageLabels(){

    QString list;

    // set the labels in image selection buttons
    for(int i=0; i<m_numFiles[testcase_permutation[m_currentTest]]; i++){

        QString text;
        list = QString(fname[testcase_permutation[m_currentTest]][order[i]].split("\\").last());
        char ch = 'A'+order[i];
        text.setNum(i+1);
        text = text + ". Image " + ch;
        //text = text + ". " + list;
        if(seen[order[i]]){
            //text = text + "  *";
            caseButton[i]->setStyleSheet("color: white; background-color: gray");
        }
        else
            caseButton[i]->setStyleSheet("color: white; background-color: rgb(150,100,100)");

        caseButton[i]->setText(text);
        caseButton[i]->setVisible(true);
        caseButton[i]->setAcceptDrops(true);
    }
}

void myGuiWidget::moveUp(){

    // move the currently selected image upwards in ranking
    int i=0;
    while(order[i]!=m_currSelection && i<m_numFiles[testcase_permutation[m_currentTest]])
        ++i;
    if(i>0){
        order[i]=order[i-1];
        order[i-1]=m_currSelection;
        caseButton[i-1]->setChecked(true);
        caseButton[i]->setChecked(false);
    }
    setImageLabels();
}

void myGuiWidget::moveDown(){

    // move the currently selected image downwards in ranking
    int i=0;
    while(order[i]!=m_currSelection && i<m_numFiles[testcase_permutation[m_currentTest]])
        ++i;
    if(i<m_numFiles[testcase_permutation[m_currentTest]]-1){
        order[i]=order[i+1];
        order[i+1]=m_currSelection;
        caseButton[i+1]->setChecked(true);
        caseButton[i]->setChecked(false);
    }
    setImageLabels();
}

void myGuiWidget::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void myGuiWidget::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}

void myGuiWidget::dropEvent(QDropEvent *event)
{
    event->accept();
}

void myGuiWidget::mouseMoveEvent(QMouseEvent *event)
{
    event->accept();
}

void myGuiWidget::mousePressEvent(QMouseEvent *event)
{
    event->accept();
}

void myGuiWidget::changeCurrent(){

    // change the current image selection
    qint8 i;

    for(i=0; i<m_numFiles[testcase_permutation[m_currentTest]]; i++){
        if(caseButton[i]->isChecked() && order[i] != m_currSelection){
            m_currSelection = order[i];
            caseButton[i]->setChecked(true);
            break;
        }
    }
    for(i=0; i<m_numFiles[testcase_permutation[m_currentTest]]; i++)
        if(order[i] != m_currSelection)
            caseButton[i]->setChecked(false);
        else
            caseButton[i]->setChecked(true);

    seen[m_currSelection]=true;
    setImageLabels();

    //imageWidget.hide();
    imageWidget.updateImage( m_currSelection );

    imageWidget.showFullScreen();

    // cannot go to the next case before all images have been seen
    if(!selectButton->isEnabled()) {
        for(i=0; i<m_numFiles[testcase_permutation[m_currentTest]]; i++)
            if(!seen[i])
                return;
        selectButton->setEnabled(true);
        selectButton->setStyleSheet("QPushButton { background-color : gray; color : white; }");
    }
}

void myGuiWidget::parseConfigFile(QString configfilepath)
{

    // parse the config file
    QFile cfgfile(configfilepath);
    if(!cfgfile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox msgBox;
        msgBox.setText("Could not open config file. Sorry!");
        msgBox.exec();
        exit(0);
    }
    QTextStream in(&cfgfile);

    m_numTests=0;
    m_numFiles[0]=0;
    bool delim_flag=true;
    QString line;

    while(!in.atEnd())
    {
        line = in.readLine(256);
        QFile f(line); // try if the file exists
        if(f.exists())
        {
            fname[m_numTests][m_numFiles[m_numTests]] = line;
            m_numFiles[m_numTests]++;
            delim_flag=false;
        }
        else if(delim_flag==false)
        {
            if(line[0]!='#'){ // comments in config file start with #
                delim_flag = true;
                m_numTests++;
                m_numFiles[m_numTests]=0;
            }
        }
    }
    if(delim_flag==false)
        m_numTests++;

    // permute to show test cases in (pseudo)random order
    makeTestcasePermutation(m_numTests);

    cfgfile.close();
}

void myGuiWidget::saveStartTime(QString resultfilepath){

    // save the current selections in a results file
    QFile resultfile(resultfilepath);
    if(!resultfile.open(QIODevice::Append | QIODevice::Text))
        return;
    QTextStream out(&resultfile);

    //get current date and time
    dateTime = QDateTime::currentDateTime();
    startDateTime = QDateTime::currentDateTime();
    QString dateTimeString = dateTime.toString();
    out << "Test started: " << dateTimeString << "\n\n";
    resultfile.close();
}

void myGuiWidget::saveCurrentSelections(QString logfilepath, QString csfilepath){

    // save the current selections in a results file
    QFile logfile(logfilepath);
    if(!logfile.open(QIODevice::Append | QIODevice::Text))
        return;
    QFile csfile(csfilepath);
    if(!csfile.open(QIODevice::Append | QIODevice::Text))
        return;
    QTextStream out(&logfile);
    QTextStream out_cs(&csfile);
    out << "test case " << testcase_permutation[m_currentTest]+1 << " ========\n";

    if(testcase_permutation[m_currentTest]>=NUM_PRACTICE_TESTS)
        out_cs << seed << "," << testcase_permutation[m_currentTest]+1-NUM_PRACTICE_TESTS;

    for(qint8 i=0; i<m_numFiles[testcase_permutation[m_currentTest]]; i++){
        char ch = 'A'+order[i];
        out << i+1 << ". Image " << ch << " " <<
               fname[testcase_permutation[m_currentTest]][image_permutation[order[i]]] << "\n";
        if(testcase_permutation[m_currentTest]>=NUM_PRACTICE_TESTS)
            out_cs << "," << image_permutation[order[i]]+1;
    }
    if(testcase_permutation[m_currentTest]>=NUM_PRACTICE_TESTS)
        out_cs << "\n";

    // save current time
    QTime time = QTime::currentTime();
    QString timeString = time.toString();
    int secs = dateTime.secsTo(QDateTime::currentDateTime());
    out << "\nTest ended: " << timeString << "; time spent: " <<
           QTime(0,secs/60,secs%60,0).toString() << "\n\n";
    dateTime = QDateTime::currentDateTime();

    logfile.close();
    csfile.close();
}

void myGuiWidget::saveEndTime(QString resultfilepath){

    // save the current selections in a results file
    QFile resultfile(resultfilepath);
    if(!resultfile.open(QIODevice::Append | QIODevice::Text))
        return;
    QTextStream out(&resultfile);

    //get current date and time
    QTime time = QTime::currentTime();
    QString timeString = time.toString();
    int secs = startDateTime.secsTo(QDateTime::currentDateTime());
    out << "\nAll test cases ended at " << timeString << "; total time spent: " <<
           QTime(0,secs/60,secs%60,0).toString() << "\n\n";
    resultfile.close();
}
