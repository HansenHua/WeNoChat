#include "UI/login.h"
#include "UI/ui_windows.h"
#include <QApplication>
using namespace mUi;
int main(int argc, char *argv[])
{
    if(QT_VERSION>=QT_VERSION_CHECK(5,6,0))
            QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);//for avoiding the different DPI of equipments
    QApplication a(argc, argv);
    load_windows();
    login->show();
    return a.exec();
}
