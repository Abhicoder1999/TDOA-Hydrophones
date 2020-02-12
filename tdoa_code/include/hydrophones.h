#ifndef HYDROPHONES_H
#define HYDROPHONES_H

#include "ui_hydrophones.h"
#include <QMainWindow>
#include <QPixmap>
#include <QTimer>
#include <qcustomplot.h>

namespace Ui {
class Hydrophones;
}

class Hydrophones : public QMainWindow {
  Q_OBJECT

public:
  explicit Hydrophones(QWidget *parent = 0);
  ~Hydrophones();
  void plotFdata1(double arr[], int time[], int length);
  void plotFdata2(double arr[], int time[], int length);
  void plotFildata1(double arr[], int time[], int length);
  void plotFildata2(double arr[], int time[], int length);
  void plotTdata1(double arr[], int time[], int length);
  void plotTdata2(double arr[], int time[], int length);

public slots:
  void resetGraphFdata1();
  void resetGraphFdata2();
  void resetGraphFildata1();
  void resetGraphFildata2();
  void resetGraphTdata1();
  void resetGraphTdata2();
  void resetGraphAll();

private:
  Ui::Hydrophones *ui;
  int Range_tdata, Range_fdata, Range_segment, Range_correlation;
};

#endif // HYDROPHONES_H
