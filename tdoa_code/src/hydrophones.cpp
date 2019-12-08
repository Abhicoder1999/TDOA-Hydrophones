#include "hydrophones.h"
#include "ui_hydrophones.h"

Hydrophones::Hydrophones(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Hydrophones) {
  ui->setupUi(this);

  ui->tdata->addGraph()->setName("tdata");
  ui->fdata->addGraph()->setName("fdata");
  ui->segment->addGraph()->setName("segment");
  ui->correlation->addGraph()->setName("correlation");

  ui->tdata->graph()->setPen(QPen(Qt::red));
  ui->fdata->graph()->setPen(QPen(Qt::green));
  ui->segment->graph()->setPen(QPen(Qt::blue));
  ui->correlation->graph()->setPen(QPen(Qt::yellow));

  ui->tdata->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  ui->tdata->axisRect()->setRangeZoom(Qt::Horizontal);
  ui->tdata->axisRect()->setRangeZoomFactor(1.5);
  ui->tdata->xAxis->setRange(0, Range_tdata);

  ui->fdata->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  ui->fdata->axisRect()->setRangeZoom(Qt::Horizontal);
  ui->fdata->axisRect()->setRangeZoomFactor(1.5);
  ui->fdata->xAxis->setRange(0, Range_fdata);

  ui->segment->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  ui->segment->axisRect()->setRangeZoom(Qt::Horizontal);
  ui->segment->axisRect()->setRangeZoomFactor(1.5);
  ui->segment->xAxis->setRange(0, Range_segment);

  ui->correlation->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  ui->correlation->axisRect()->setRangeZoom(Qt::Horizontal);
  ui->correlation->axisRect()->setRangeZoomFactor(1.5);
  ui->correlation->xAxis->setRange(0, Range_correlation);

  connect(ui->reset_tdata, SIGNAL(pressed()), this, SLOT(resetGraphTdata()));
  connect(ui->reset_fdata, SIGNAL(pressed()), this, SLOT(resetGraphFdata()));
  connect(ui->reset_segment, SIGNAL(pressed()), this,
          SLOT(resetGraphSegment()));
  connect(ui->reset_correlation, SIGNAL(pressed()), this,
          SLOT(resetGraphCorrelation()));
  connect(ui->reset_all, SIGNAL(pressed()), this, SLOT(resetGraphAll()));
  this->show();
}

Hydrophones::~Hydrophones() { delete ui; }

void Hydrophones::plotTdata(double arr[], int time[], int length) {

  for (int i = 0; i < length; i++) {
    ui->tdata->graph()->addData(time[i], arr[i]);
  }
  ui->tdata->replot();
  ui->tdata->yAxis->rescale();
  ui->tdata->replot();
}
void Hydrophones::plotFdata(double arr[], int time[], int length) {
  for (int i = 0; i < length; i++) {
    ui->fdata->graph()->addData(time[i], arr[i]);
  }
  ui->fdata->replot();
  ui->fdata->yAxis->rescale();
  ui->fdata->replot();
}
void Hydrophones::plotSegment(double arr[], int time[], int length) {
  for (int i = 0; i < length; i++) {
    ui->segment->graph()->addData(time[i], arr[i]);
  }
  ui->segment->replot();
  ui->segment->yAxis->rescale();
  ui->segment->replot();
}
void Hydrophones::plotCorrelation(double arr[], int time[], int length) {
  for (int i = 0; i < length; i++) {
    ui->correlation->graph()->addData(time[i], arr[i]);
  }
  ui->correlation->replot();
  ui->correlation->yAxis->rescale();
  ui->correlation->replot();
}
void Hydrophones::resetGraphAll() {
  resetGraphTdata();
  resetGraphFdata();
  resetGraphSegment();
  resetGraphCorrelation();
}

void Hydrophones::resetGraphTdata() {
  ui->tdata->graph()->data()->clear();
  ui->tdata->replot();
  ui->tdata->yAxis->rescale();
  ui->tdata->replot();
}
void Hydrophones::resetGraphFdata() {
  ui->fdata->graph()->data()->clear();
  ui->fdata->replot();
  ui->fdata->yAxis->rescale();
  ui->fdata->replot();
}
void Hydrophones::resetGraphSegment() {
  ui->segment->graph()->data()->clear();
  ui->segment->replot();
  ui->segment->yAxis->rescale();
  ui->segment->replot();
}
void Hydrophones::resetGraphCorrelation() {
  ui->correlation->graph()->data()->clear();
  ui->correlation->replot();
  ui->correlation->yAxis->rescale();
  ui->correlation->replot();
}
