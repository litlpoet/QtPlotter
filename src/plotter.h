// Copyright 2015 Byungkuk Choi

#ifndef PLOTTER_HPP
#define PLOTTER_HPP

#include <QtWidgets/QWidget>
#include <Eigen/Core>

using Eigen::MatrixXf;

class PlotSetting;

class Plotter : public QWidget {
  Q_OBJECT

 public:
  enum { Margin = 50 };

  static const QColor colors[6];

  explicit Plotter(QWidget* parent = 0);

  ~Plotter();

  void setPlotSetting(const PlotSetting& settings);

  void setRegionData(const int& id, const MatrixXf& data);

  void setCurveData(const int& id, const MatrixXf& data);

  void setPointData(const int& id, const MatrixXf& data);

  void crearCurve(const int& id);

  QSize minimumSizeHint() const;

  QSize sizeHint() const;

 public slots:
  void zoomIn();

  void zoomOut();

 protected:
  void refreshPixmap();

  void paintEvent(QPaintEvent* event);

  void resizeEvent(QResizeEvent* event);

  void mousePressEvent(QMouseEvent* event);

  void mouseMoveEvent(QMouseEvent* event);

  void mouseReleaseEvent(QMouseEvent* event);

  void keyPressEvent(QKeyEvent* event);

  void wheelEvent(QWheelEvent* event);

 private:
  class Imple;
  QScopedPointer<Imple> _p;
};

#endif  // PLOTTER_HPP
