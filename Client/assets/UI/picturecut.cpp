#include "picturecut.h"
#include "ui_picturecut.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QCursor>

PictureCut::PictureCut(std::string path, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PictureCut)
{
    ui->setupUi(this);
    picPath = path;
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    ui->picBox->installEventFilter(this);
    rec = new QWidget(ui->box);
    rec->setVisible(false);
    rec->setStyleSheet("border: 3px solid blue;");
    rec->setWindowFlags(Qt::WindowStaysOnTopHint);
    rec->installEventFilter(this);


    QImage *img = new QImage(QString(path.c_str()));
    oriPic = img;
    showPicture(img);
}

PictureCut::~PictureCut()
{
    delete ui;
}

void PictureCut::showPicture(QImage *img)
{
    qDebug()<<img->height();
    qDebug()<<img->width();
    if(338.0 * img->width() >= 600.0 * img->height())
    {
        ratio = 600.0 / img->width();
    }
    else
    {
        ratio = 338.0 / img->height();
    }
    QPixmap pixmap = QPixmap::fromImage(*img);
    pixmap = pixmap.scaled(int(img->width() * ratio), int(img->height() * ratio));
    qDebug() << pixmap.width() <<pixmap.height();
    ui->box->setGeometry(x_center - pixmap.width() / 2, y_center - pixmap.height() / 2, pixmap.width(), pixmap.height());
    ui->picBox->setGeometry(0, 0, ui->box->width(), ui->box->height());
    ui->picBox->setPixmap(pixmap);
}

bool PictureCut::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->picBox || obj == rec)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            start.x = QCursor::pos().x() - this->x() - ui->box->x();
            start.y = QCursor::pos().y() - this->y() - ui->box->y();
            qDebug("here");

        }
        else if(event->type() == QEvent::MouseMove)
        {
            rec->setVisible(true);
            qDebug() << QCursor::pos().x() - this->x();
            end.x = QCursor::pos().x() - this->x() - ui->box->x();
            end.y = QCursor::pos().y() - this->y() - ui->box->y();
            qDebug("move");
            qDebug() <<start.x<<" "<<start.y<<" "<<end.x-start.x<<" "<<end.y-start.y;
            int width = std::min(end.x-start.x, end.y-start.y);
            int limit = std::min(ui->box->width() - start.x, ui->box->height() - start.y);
            width = std::min(width, limit);
            rec->setGeometry(QRect(start.x, start.y, width, width));
            selectedPart.x = start.x;
            selectedPart.y = start.y;
            selectedPart.w = width;
            qDebug() << rec->x() << " " << rec->y();
        }
        return QMainWindow::eventFilter(obj, event);
    }
    else
    {
        return QMainWindow::eventFilter(obj, event);
    }
}

void PictureCut::on_btnCancel_clicked()
{
    close();
}

void PictureCut::on_btnConfirm_clicked()
{
    QImage resultPic = oriPic->copy(int(selectedPart.x / ratio), int(selectedPart.y / ratio), int(selectedPart.w / ratio), int(selectedPart.w  / ratio));
    QString picExt= QString::fromStdString(picPath.substr(picPath.find_last_of(".")));
    QDir dir;
    if(!dir.exists("./assets/")) dir.mkdir("./assets/");
    resultPic.save("./assets/head"+picExt);
    Ui::headImage result;
    result.image = resultPic;
    result.path = QFileInfo("./assets/head"+picExt).absoluteFilePath().toStdString();
    emit getSelectedPicture(result);
    close();
}
