#ifndef PICTURECUT_H
#define PICTURECUT_H

#include <QMainWindow>
#include <QWidget>

namespace Ui {
class PictureCut;
struct headImage
{
    QImage image;
    std::string path;
};
}
Q_DECLARE_METATYPE(Ui::headImage)

class PictureCut : public QMainWindow
{
    Q_OBJECT

public:
    explicit PictureCut(std::string path, QWidget *parent = 0);
    ~PictureCut();

protected:
    bool eventFilter(QObject *, QEvent *);

private slots:
    void on_btnCancel_clicked();

    void on_btnConfirm_clicked();

private:
    struct point
    {
        int x, y;
    };
    struct rect
    {
        int x, y, w;
    };

    enum geoLimit{widthLimit = 600, heightLimit = 338};
    enum geoCenter{x_center = 350, y_center = 219};
    Ui::PictureCut *ui;
    std::string picPath;
    point start;
    point end;
    rect selectedPart;
    QWidget *rec;
    double ratio;
    QImage *oriPic;

    void showPicture(QImage *img);

signals:
    void getSelectedPicture(Ui::headImage);

};

#endif // PICTURECUT_H
