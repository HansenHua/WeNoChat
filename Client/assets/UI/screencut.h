#ifndef SCREENCUT_H
#define SCREENCUT_H

#include <QWidget>
#include <QLabel>
#include <QKeyEvent>
#include <QMenu>

namespace Ui {
class ScreenCut;
}

class ScreenCut : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenCut(QWidget *parent = 0);
    ~ScreenCut();
    void cut();

protected:
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void showEvent(QShowEvent *);
    void contextMenuEvent(QContextMenuEvent *);

signals:
    void cancel();
    void gotScreenCut(QImage);

private slots:
    void completeCut();
    void saveCutPic();
    void cancelCut();

private:
    struct Point
    {
        int x, y;
    };
    enum State{beforeCut = 0, cutting, hadCut,
               adjustLeftUp, adjustRightUp, adjustLeftDown, adjustRightDown, adjustLeft,
               adjustUp, adjustRight, adjustDown, adjustPosition, Normal};

    Ui::ScreenCut *ui;
    Point startPoint, endPoint, leftUpPoint, rightDownPoint, adjustStartPoint, adjustDelta;
    QMenu *menu;
    QPixmap *screenPic;
    QPixmap *showScreenPic;
    QWidget *toolBox;
    int state;

    int getCursorState(int x, int y);
};

#endif // SCREENCUT_H
