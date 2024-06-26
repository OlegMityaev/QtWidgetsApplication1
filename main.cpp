#include <QApplication>
#include "QtWidgetsApplication1.h"


int main(int argc, char* argv[])
{
    std::cout << "ujk" << std::endl;
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QApplication a(argc, argv);
    QtWidgetsApplication1 w;
    w.show();
    return a.exec();
}
