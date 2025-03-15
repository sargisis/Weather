#include <QApplication>
#include "HomePage.h"

int main(int argc, char *argv[])
{
    QApplication ss(argc, argv);
    HomePage h;
    h.show();
    return ss.exec();
}
