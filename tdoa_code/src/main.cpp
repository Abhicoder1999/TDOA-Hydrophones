#include "hydrophones.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  Hydrophones w;
  w.show();

  return a.exec();
}
