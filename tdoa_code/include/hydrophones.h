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
  void plotTdata(double arr[], int time[], int length);
  void plotFdata(double arr[], int time[], int length);
  void plotSegment(double arr[], int time[], int length);
  void plotCorrelation(double arr[], int time[], int length);
  void resetGraphTdata();
  void resetGraphFdata();
  void resetGraphSegment();
  void resetGraphCorrelation();
  void resetGraphAll();

private:
  Ui::Hydrophones *ui;
  int Range_tdata, Range_fdata, Range_segment, Range_correlation;


};

#endif // HYDROPHONES_H
