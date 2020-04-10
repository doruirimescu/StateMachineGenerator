#include "mainwindow.h"
#include "scribblearea.h"

#include <QApplication>
#include <QColorDialog>
#include <QFileDialog>
#include <QImageWriter>
#include <QInputDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QCloseEvent>
#include <QList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), scribbleArea(new ScribbleArea(this))
{
    setCentralWidget(scribbleArea);

    createActions();
    createMenus();

    setWindowTitle(tr("State Machine Generator"));
    resize(500, 500);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
        event->accept();
    else
        event->ignore();
    delete scribbleArea;
}

void MainWindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open File"), QDir::currentPath());
    }
}

void MainWindow::save()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void MainWindow::penColor()
{
    QColorDialog * c = new QColorDialog(this);
    c->setStandardColor(0,0xE0E0E0);//To add some nice colors
    c->setStandardColor(1,0xF5F5F5);

    QColor newColor = c->getColor(scribbleArea->penColor(),this,"Title",QColorDialog::DontUseNativeDialog);

    if (newColor.isValid())
        scribbleArea->setPenColor(newColor);
    delete c;
}

void MainWindow::stateColor()
{
    QColor newColor = QColorDialog::getColor(scribbleArea->penColor());
    if (newColor.isValid())
        scribbleArea->setStateColor(newColor);
}
void MainWindow::changeGrid()
{
    int newSize = 3;
    while( newSize % 10 != 0 )
    {
        newSize = QInputDialog::getInt( this,"Change grid",
                                        "Enter a new grid size multiple of 10", scribbleArea->getGridSize() );
    }
    scribbleArea->setGridSize( newSize  );
    scribbleArea->clearImage();
}
void MainWindow::changeRadius( )
{
    int newRad = 3;
    while( newRad % 10 != 0 )
    {
        newRad = QInputDialog::getInt(this,"Edit state radius",
                                      "Enter a new radius, multiple 10", scribbleArea->getStateRadius() );
    }
    scribbleArea->setStateRadius( newRad );
}
void MainWindow::placeState()
{//Place state

    scribbleArea->pState = true;
    scribbleArea->eState = false;
    scribbleArea->pAction = false;
    scribbleArea->eAction = false;
    //changeRadius();

}

void MainWindow::placeAction()
{/* Place action behavior */
    scribbleArea->pAction = true;
    scribbleArea->pState = false;
    scribbleArea->eState = false;
    scribbleArea->eAction = false;
}
void MainWindow::editState()
{
    scribbleArea->eState = true;
    qInfo()<<"Edit state";
    QApplication::setOverrideCursor(Qt::PointingHandCursor);
}
void MainWindow::editAction()
{
    scribbleArea->eAction = true;
    qInfo()<<"Edit Action";
}
void MainWindow::penWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Scribble"),
                                        tr("Select pen width:"),
                                        scribbleArea->penWidth(),
                                        1, 50, 1, &ok);
    if (ok)
        scribbleArea->setPenWidth(newWidth);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Scribble"),
            tr("<p>The <b>Scribble</b> example shows how to use QMainWindow as the "
               "base widget for an application, and how to reimplement some of "
               "QWidget's event handlers to receive the events generated for "
               "the application's widgets:</p><p> We reimplement the mouse event "
               "handlers to facilitate drawing, the paint event handler to "
               "update the application and the resize event handler to optimize "
               "the application's appearance. In addition we reimplement the "
               "close event handler to intercept the close events before "
               "terminating the application.</p><p> The example also demonstrates "
               "how to use QPainter to draw an image in real time, as well as "
               "to repaint widgets.</p>"));
}
void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    const QList<QByteArray> imageFormats = QImageWriter::supportedImageFormats();
    for (const QByteArray &format : imageFormats)
    {
        QString text = tr("%1...").arg(QString::fromLatin1(format).toUpper());

        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, &QAction::triggered, this, &MainWindow::save);
        saveAsActs.append(action);
    }

    printAct = new QAction(tr("&Print..."), this);
    connect(printAct, &QAction::triggered, scribbleArea, &ScribbleArea::print);

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, &QAction::triggered, this, &MainWindow::close);

    penColorAct = new QAction(tr("&Pen Color..."), this);
    connect(penColorAct, &QAction::triggered, this, &MainWindow::penColor);

    penWidthAct = new QAction(tr("Pen &Width..."), this);
    connect(penWidthAct, &QAction::triggered, this, &MainWindow::penWidth);

    clearScreenAct = new QAction(tr("&Clear Screen"), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, &QAction::triggered,
            scribbleArea, &ScribbleArea::clearImage);

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);

    changeGridAct = new QAction(tr("Change &grid size"), this);
    connect(changeGridAct, &QAction::triggered, this, &MainWindow::changeGrid);

    changeRadiusAct = new QAction(tr("Change state &radius"), this);
    connect(changeRadiusAct, &QAction::triggered, this, &MainWindow::changeRadius);

    /* Add state machine drawing */
    stateColorAct = new QAction(tr("&State Color..."), this);
    connect(stateColorAct, &QAction::triggered, this, &MainWindow::stateColor);

    /*Add state machine actions*/
    placeStateAct = new QAction(tr("Place state"), this);
    placeStateAct->setShortcut(tr("S"));
    connect(placeStateAct, &QAction::triggered, this, &MainWindow::placeState);

    editStateAct = new QAction(tr("Edit state"), this);
    editStateAct->setShortcut(tr("Ctrl+S"));
    connect(editStateAct, &QAction::triggered, this, &MainWindow::editState);

    placeActionAct = new QAction(tr("Toggle place Action"), this);
    placeActionAct->setShortcut(tr("A"));
    connect(placeActionAct, &QAction::triggered, this, &MainWindow::placeAction);

    editActionAct = new QAction(tr("Edit Action"), this);
    editActionAct->setShortcut(tr("Ctrl+A"));
    connect(editActionAct, &QAction::triggered, this, &MainWindow::editAction);
}

void MainWindow::createMenus()
{
    saveAsMenu = new QMenu(tr("&Save As"), this);
    for (QAction *action : qAsConst(saveAsActs))
        saveAsMenu->addAction(action);

    /* Create File menu */
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    /* Create option menu */
    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addAction(changeGridAct);
    optionMenu->addAction(changeRadiusAct);
    optionMenu->addAction(stateColorAct);
    optionMenu->addSeparator();
    optionMenu->addAction(clearScreenAct);

    /* Create help menu */
    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    /* Create state menu */
    stateMenu= new QMenu(tr("&State machine"),this);
    stateMenu->addAction(placeStateAct);
    stateMenu->addAction(editStateAct);
    stateMenu->addAction(placeActionAct);
    stateMenu->addAction(editActionAct);

    /* Add menus to menu bar */
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);
    menuBar()->addMenu(stateMenu);
}

bool MainWindow::maybeSave()
{
    if (scribbleArea->isModified()) {
       QMessageBox::StandardButton ret;
       ret = QMessageBox::warning(this, tr("Scribble"),
                          tr("The image has been modified.\n"
                             "Do you want to save your changes?"),
                          QMessageBox::Save | QMessageBox::Discard
                          | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return saveFile("png");
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(QString::fromLatin1(fileFormat.toUpper()))
                               .arg(QString::fromLatin1(fileFormat)));
    if (fileName.isEmpty())
        return false;
    return true;
}
