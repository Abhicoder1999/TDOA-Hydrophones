#include "hydrophones.h"
#include "ui_hydrophones.h"

Hydrophones::Hydrophones(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Hydrophones) {
  ui->setupUi(this);

  ui->fdata1->addGraph()->setName("fdata1");
  ui->fdata2->addGraph()->setName("fdata2");
  ui->fildata1->addGraph()->setName("fildata1");
  ui->fildata2->addGraph()->setName("fildata2");
  ui->tdata1->addGraph()->setName("tdata1");
  ui->tdata2->addGraph()->setName("tdata2");

  ui->fdata1->graph()->setPen(QPen(Qt::red));
  ui->fdata2->graph()->setPen(QPen(Qt::green));
  ui->fildata1->graph()->setPen(QPen(Qt::blue));
  ui->fildata2->graph()->setPen(QPen(Qt::magenta));
  ui->tdata1->graph()->setPen(QPen(Qt::cyan));
  ui->tdata2->graph()->setPen(QPen(Qt::darkGreen));

  ui->fdata1->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  ui->fdata1->axisRect()->setRangeZoom(Qt::Horizontal);
  ui->fdata1->axisRect()->setRangeZoomFactor(1.5);
  // ui->fdata1->xAxis->setRange(0, Range_fdata1);

  ui->fdata2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  ui->fdata2->axisRect()->setRangeZoom(Qt::Horizontal);
  ui->fdata2->axisRect()->setRangeZoomFactor(1.5);
  // ui->fdata2->xAxis->setRange(0, Range_fdata);

  ui->fildata1->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  ui->fildata1->axisRect()->setRangeZoom(Qt::Horizontal);
  ui->fildata1->axisRect()->setRangeZoomFactor(1.5);
  // ui->fildata1->xAxis->setRange(0, Range_fildata1);

  ui->fildata2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  ui->fildata2->axisRect()->setRangeZoom(Qt::Horizontal);
  ui->fildata2->axisRect()->setRangeZoomFactor(1.5);
  // ui->fildata2->xAxis->setRange(0, Range_fildata2);

  ui->tdata1->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  ui->tdata1->axisRect()->setRangeZoom(Qt::Horizontal);
  ui->tdata1->axisRect()->setRangeZoomFactor(1.5);
  // ui->tdata1->xAxis->setRange(0, Range_fdata1);

  ui->tdata2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  ui->tdata2->axisRect()->setRangeZoom(Qt::Horizontal);
  ui->tdata2->axisRect()->setRangeZoomFactor(1.5);
  // ui->tdata2->xAxis->setRange(0, Range_fdata);

  // connect(ui->reset_fdata1, SIGNAL(pressed()), this, SLOT(resetGraphTdata()));
  // connect(ui->reset_fdata, SIGNAL(pressed()), this, SLOT(resetGraphFdata()));
  // connect(ui->reset_fildata1, SIGNAL(pressed()), this,
  //         SLOT(resetGraphSegment()));
  // connect(ui->reset_fildata2, SIGNAL(pressed()), this,
  //         SLOT(resetGraphCorrelation()));
  // connect(ui->reset_all, SIGNAL(pressed()), this, SLOT(resetGraphAll()));
  this->show();
}

Hydrophones::~Hydrophones() { delete ui; }

void Hydrophones::plotFdata1(double arr[], int time[], int length) {

  for (int i = 0; i < length; i++) {
    ui->fdata1->graph()->addData(time[i], arr[i]);
  }
  ui->fdata1->replot();
  ui->fdata1->yAxis->rescale();
  ui->fdata1->xAxis->rescale();
  ui->fdata1->replot();
}
void Hydrophones::plotFdata2(double arr[], int time[], int length) {
  for (int i = 0; i < length; i++) {
    ui->fdata2->graph()->addData(time[i], arr[i]);
  }
  ui->fdata2->replot();
  ui->fdata2->yAxis->rescale();
  ui->fdata2->xAxis->rescale();
  ui->fdata2->replot();
}
void Hydrophones::plotFildata1(double arr[], int time[], int length) {
  for (int i = 0; i < length; i++) {
    ui->fildata1->graph()->addData(time[i], arr[i]);
  }
  ui->fildata1->replot();
  ui->fildata1->yAxis->rescale();
  ui->fildata1->xAxis->rescale();
  ui->fildata1->replot();
}
void Hydrophones::plotFildata2(double arr[], int time[], int length) {
  for (int i = 0; i < length; i++) {
    ui->fildata2->graph()->addData(time[i], arr[i]);
  }
  ui->fildata2->replot();
  ui->fildata2->yAxis->rescale();
  ui->fildata2->xAxis->rescale();
  ui->fildata2->replot();
}
void Hydrophones::plotTdata1(double arr[], int time[], int length) {

  for (int i = 0; i < length; i++) {
    ui->tdata1->graph()->addData(time[i], arr[i]);
  }
  ui->tdata1->replot();
  ui->tdata1->yAxis->rescale();
  ui->tdata1->xAxis->rescale();
  ui->tdata1->replot();
}
void Hydrophones::plotTdata2(double arr[], int time[], int length) {
  for (int i = 0; i < length; i++) {
    ui->tdata2->graph()->addData(time[i], arr[i]);
  }
  ui->tdata2->replot();
  ui->tdata2->yAxis->rescale();
  ui->tdata2->xAxis->rescale();
  ui->tdata2->replot();
}
void Hydrophones::resetGraphAll() {
  resetGraphFdata1();
  resetGraphFdata2();
  resetGraphFildata1();
  resetGraphFildata2();
  resetGraphTdata1();
  resetGraphTdata2();
}

void Hydrophones::resetGraphFdata1() {
  ui->fdata1->graph()->data()->clear();
  ui->fdata1->replot();
  ui->fdata1->yAxis->rescale();
  ui->fdata1->xAxis->rescale();
  ui->fdata1->replot();
}
void Hydrophones::resetGraphFdata2() {
  ui->fdata2->graph()->data()->clear();
  ui->fdata2->replot();
  ui->fdata2->yAxis->rescale();
  ui->fdata2->xAxis->rescale();
  ui->fdata2->replot();
}
void Hydrophones::resetGraphFildata1() {
  ui->fildata1->graph()->data()->clear();
  ui->fildata1->replot();
  ui->fildata1->yAxis->rescale();
  ui->fildata1->xAxis->rescale();
  ui->fildata1->replot();
}
void Hydrophones::resetGraphFildata2() {
  ui->fildata2->graph()->data()->clear();
  ui->fildata2->replot();
  ui->fildata2->yAxis->rescale();
  ui->fildata2->xAxis->rescale();
  ui->fildata2->replot();
}

void Hydrophones::resetGraphTdata1() {
  ui->tdata1->graph()->data()->clear();
  ui->tdata1->replot();
  ui->tdata1->yAxis->rescale();
  ui->tdata1->xAxis->rescale();
  ui->tdata1->replot();
}
void Hydrophones::resetGraphTdata2() {
  ui->tdata2->graph()->data()->clear();
  ui->tdata2->replot();
  ui->tdata2->yAxis->rescale();
  ui->tdata2->xAxis->rescale();
  ui->tdata2->replot();
}
