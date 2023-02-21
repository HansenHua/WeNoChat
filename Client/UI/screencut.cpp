#include "screencut.h"
#include "ui_screencut.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QTimer>
#include <QDebug>
#include <QPainter>
#include <QFileInfo>
#include <QFileDialog>
#include <QPushButton>

ScreenCut::ScreenCut(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenCut)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowState(Qt::WindowActive | Qt::WindowFullScreen);
    this->setMouseTracking(true);
    screenPic = new QPixmap;
    toolBox = new QWidget(this);

    QPushButton *button1 = new QPushButton(toolBox);
    button1->setGeometry(23, 2, 36, 36);
    button1->setIconSize(QSize(36, 36));
    button1->setIcon(QIcon(":/new/icon/check.jpg"));
    button1->setCursor(Qt::PointingHandCursor);

    QPushButton *button2 = new QPushButton(toolBox);
    button2->setGeometry(82, 2, 36, 36);
    button2->setIconSize(QSize(36, 36));
    button2->setIcon(QIcon(":/new/icon/save.jpg"));
    button2->setCursor(Qt::PointingHandCursor);

    QPushButton *button3 = new QPushButton(toolBox);
    button3->setGeometry(141, 2, 36, 36);
    button3->setIconSize(QSize(36, 36));
    button3->setIcon(QIcon(":/new/icon/cross.jpg"));
    button3->setCursor(Qt::PointingHandCursor);

    toolBox->setVisible(false);
    leftUpPoint.x = 0;
    leftUpPoint.y = 0;
    rightDownPoint.x = 0;
    rightDownPoint.y = 0;
    adjustStartPoint.x = 0;
    adjustStartPoint.y = 0;
    state = beforeCut;
    menu = new QMenu;
    QAction *action1 = new QAction("完成截图");
    QAction *action2 = new QAction("截图区域另存为");
    QAction *action3 = new QAction("退出截图");
    menu->addAction(action1);
    menu->addAction(action2);
    menu->addAction(action3);
    connect(action1, SIGNAL(triggered(bool)), this, SLOT(completeCut()));
    connect(button1, SIGNAL(clicked(bool)), this, SLOT(completeCut()));
    connect(action2, SIGNAL(triggered(bool)), this, SLOT(saveCutPic()));
    connect(button2, SIGNAL(clicked(bool)), this, SLOT(saveCutPic()));
    connect(action3, SIGNAL(triggered(bool)), this, SLOT(cancelCut()));
    connect(button3, SIGNAL(clicked(bool)), this, SLOT(cancelCut()));
}

ScreenCut::~ScreenCut()
{
    delete ui;
}

void ScreenCut::completeCut()
{
    int x = leftUpPoint.x;
    int y = leftUpPoint.y;
    int w = rightDownPoint.x - x;
    int h = rightDownPoint.y - y;
    emit gotScreenCut(screenPic->copy(x, y, w, h).toImage());
    close();
}

void ScreenCut::saveCutPic()
{
    QString savePath = QFileDialog::getSaveFileName(NULL, "请选择截图存储路径", "", "png(*.png)");
    QFileInfo fileInfo = QFileInfo(savePath);
    QString suffix = fileInfo.suffix();
    int x = leftUpPoint.x;
    int y = leftUpPoint.y;
    int w = rightDownPoint.x - x;
    int h = rightDownPoint.y - y;
    screenPic->copy(x, y, w, h).save(savePath, suffix.toStdString().c_str());
    qDebug()<<savePath;
    qDebug()<<suffix;
}

void ScreenCut::cancelCut()
{
    emit cancel();
    close();
}

void ScreenCut::cut()
{
    QTimer::singleShot(1000, this, [&](){
        qDebug("get cut");
        QScreen* screen = QApplication::primaryScreen();
        *screenPic = screen->grabWindow(QApplication::desktop()->winId(), 0, 0, this->width(), this->height());
        qDebug("got cut");
        show();
    });
}

void ScreenCut::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        emit cancel();
        close();
    }
}

void ScreenCut::paintEvent(QPaintEvent *)
{
    qDebug("paint");
    int x = leftUpPoint.x;
    int y = leftUpPoint.y;
    int w = rightDownPoint.x - x;
    int h = rightDownPoint.y - y;

    int leftRightMargin = this->width() - leftUpPoint.x;
    int upMargin = leftUpPoint.y;
    int downMargin = this->height() - rightDownPoint.y;

    toolBox->setStyleSheet("background-color: white");

    if (state != beforeCut)
    {
        toolBox->setVisible(true);
        int geoX = x, geoY = y + h + 5;
        if (leftRightMargin < 200)
        {
            geoX = this->width() - 200;
        }
        if (downMargin < 45)
        {
            if(upMargin >= 45)
            {
                geoY = leftUpPoint.y - 45;
            }
            else
            {
                geoY = leftUpPoint.y;
            }
        }
        toolBox->setGeometry(geoX, geoY, 200, 40);
    }


    QPainter painter(this);
    qDebug("painting");
    painter.drawPixmap(0, 0, *showScreenPic);

    qDebug()<<x<<" "<<y<<" "<<w<<" "<<h<<" ";
    if (w != 0 && h != 0) {
        painter.drawPixmap(x, y, screenPic->copy(x, y, w, h));
    }

    QPen pen;
    pen.setColor(Qt::green);
    pen.setWidth(2);
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    painter.drawRect(x, y, w, h);

    pen.setColor(Qt::yellow);
    painter.setPen(pen);
    painter.drawText(x + 2, y - 8, tr("截图范围：( %1 x %2 ) - ( %3 x %4 )  图片大小：( %5 x %6 )")
                     .arg(x).arg(y).arg(x + w).arg(y + h).arg(w).arg(h));
    qDebug("paint over");
}

void ScreenCut::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<"mouse press";
    if (state == beforeCut)
    {
        state = cutting;
        startPoint.x = event->pos().x();
        startPoint.y = event->pos().y();
        endPoint.x = event->pos().x();
        endPoint.y = event->pos().y();
        leftUpPoint.x = std::min(startPoint.x, endPoint.x);
        leftUpPoint.y = std::min(startPoint.y, endPoint.y);
        rightDownPoint.x = std::max(startPoint.x, endPoint.x);
        rightDownPoint.y = std::max(startPoint.y, endPoint.y);
    }
    else if(state == hadCut)
    {
        int x = event->pos().x();
        int y = event->pos().y();
        state = getCursorState(x, y);
        adjustStartPoint.x = x;
        adjustStartPoint.y = y;
    }
}

void ScreenCut::mouseMoveEvent(QMouseEvent *event)
{
    this->setCursor(Qt::ArrowCursor);
    if (state == cutting)
    {
        qDebug("mouse move");

        endPoint.x = event->pos().x();
        endPoint.y = event->pos().y();

        leftUpPoint.x = std::min(startPoint.x, endPoint.x);
        leftUpPoint.y = std::min(startPoint.y, endPoint.y);
        rightDownPoint.x = std::max(startPoint.x, endPoint.x);
        rightDownPoint.y = std::max(startPoint.y, endPoint.y);
    }
    else if (state == hadCut)
    {
        int x = event->x();
        int y = event->y();
        int cursorState = getCursorState(x, y);
        switch (cursorState) {
        case adjustLeftUp: case adjustRightDown: this->setCursor(Qt::SizeFDiagCursor); break;
        case adjustLeftDown: case adjustRightUp: this->setCursor(Qt::SizeBDiagCursor); break;
        case adjustLeft: case adjustRight: this->setCursor(Qt::SizeHorCursor); break;
        case adjustUp: case adjustDown: this->setCursor(Qt::SizeVerCursor); break;
        case adjustPosition: this->setCursor(Qt::SizeAllCursor); break;
        default: this->setCursor(Qt::ArrowCursor); break;
        }
    }
    else
    {
        int leftMargin = -leftUpPoint.x;
        int rightMargin = this->width() - rightDownPoint.x;
        int upMargin = -leftUpPoint.y;
        int downMargin = this->height() - rightDownPoint.y;
        switch (state) {
        case adjustLeftUp:
            leftUpPoint.x = std::min(event->pos().x(), rightDownPoint.x - 50);
            leftUpPoint.y = std::min(event->pos().y(), rightDownPoint.y - 50);
            break;
        case adjustRightDown:
            rightDownPoint.x = std::max(event->pos().x(), leftUpPoint.x + 50);
            rightDownPoint.y = std::max(event->pos().y(), leftUpPoint.y + 50);
            break;
        case adjustLeftDown:
            leftUpPoint.x = std::min(event->pos().x(), rightDownPoint.x - 50);
            rightDownPoint.y = std::max(event->pos().y(), leftUpPoint.y + 50);
            break;
        case adjustRightUp:
            rightDownPoint.x = std::max(event->pos().x(), leftUpPoint.x + 50);
            leftUpPoint.y = std::min(event->pos().y(), rightDownPoint.y - 50);
            break;
        case adjustLeft:
            leftUpPoint.x = std::min(event->pos().x(), rightDownPoint.x - 50);
            break;
        case adjustRight:
            rightDownPoint.x = std::max(event->pos().x(), leftUpPoint.x + 50);
            break;
        case adjustUp:
            leftUpPoint.y = std::min(event->pos().y(), rightDownPoint.y - 50);
            break;
        case adjustDown:
            rightDownPoint.y = std::max(event->pos().y(), leftUpPoint.y + 50);
            break;
        case adjustPosition:
            adjustDelta.x = event->pos().x() - adjustStartPoint.x;
            adjustDelta.y = event->pos().y() - adjustStartPoint.y;
            qDebug() << adjustDelta.x << adjustDelta.y << "move";
            adjustDelta.x = std::min(std::max(leftMargin, adjustDelta.x), rightMargin);
            adjustDelta.y = std::min(std::max(upMargin, adjustDelta.y), downMargin);
            leftUpPoint.x += adjustDelta.x;
            leftUpPoint.y += adjustDelta.y;
            rightDownPoint.x += adjustDelta.x;
            rightDownPoint.y += adjustDelta.y;
            adjustStartPoint.x = event->pos().x();
            adjustStartPoint.y = event->pos().y();
            break;
        default: this->setCursor(Qt::ArrowCursor); break;
        }
    }
    this->update();
}

void ScreenCut::mouseReleaseEvent(QMouseEvent *)
{
    qDebug("mouse release");
    if (state == cutting)
    {
        rightDownPoint.x = std::max(rightDownPoint.x, leftUpPoint.x + 50);
        rightDownPoint.y = std::max(rightDownPoint.y, leftUpPoint.y + 50);
    }
    state = hadCut;

}

void ScreenCut::showEvent(QShowEvent *)
{
    qDebug() << "start show";
    QPixmap pix(this->width(), this->height());
    pix.fill((QColor(0, 0, 0, 100)));
    showScreenPic = new QPixmap(*screenPic);
    QPainter p(showScreenPic);
    p.drawPixmap(0, 0, pix);
    qDebug() << "end show";
}

void ScreenCut::contextMenuEvent(QContextMenuEvent *)
{
    if(state == hadCut)
    {
        this->setCursor(Qt::ArrowCursor);
        menu->exec(cursor().pos());
    }
}

int ScreenCut::getCursorState(int x, int y)
{
    if(leftUpPoint.x - 5 <= x && x <= rightDownPoint.x + 5 &&
            leftUpPoint.y - 5 <= y && y <= rightDownPoint.y + 5)
    {
        if (std::abs(x - leftUpPoint.x) <= 5 && std::abs(y - leftUpPoint.y) <= 5)
        {
            return adjustLeftUp;
        }
        else if (std::abs(x - rightDownPoint.x) <= 5 && std::abs(y - rightDownPoint.y) <= 5)
        {
            return adjustRightDown;
        }
        else if (std::abs(x - leftUpPoint.x) <= 5 && std::abs(y - rightDownPoint.y) <= 5)
        {
            return adjustLeftDown;
        }
        else if (std::abs(x - rightDownPoint.x) <= 5 && std::abs(y - leftUpPoint.y) <= 5)
        {
            return adjustRightUp;
        }
        else if (std::abs(x - leftUpPoint.x) <= 5)
        {
            return adjustLeft;
        }
        else if (std::abs(x - rightDownPoint.x) <= 5)
        {
            return adjustRight;
        }
        else if (std::abs(y - leftUpPoint.y) <= 5)
        {
            return adjustUp;
        }
        else if (std::abs(y - rightDownPoint.y) <= 5)
        {
            return adjustDown;
        }
        else
        {
            return adjustPosition;
        }
    }

    else
    {
        return Normal;
    }
}
