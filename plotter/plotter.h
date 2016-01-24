// Copyright 2015 Byungkuk Choi
#ifndef PLOTTER_PLOTTER_H_
#define PLOTTER_PLOTTER_H_

#include <QtWidgets/QWidget>

#include <Eigen/Core>

using MatNxN = Eigen::MatrixXf;

class PlotSetting;

class Plotter : public QWidget {
  Q_OBJECT

 public:
  enum { Margin = 50 };

  static const QColor colors[6];

  explicit Plotter(QWidget* parent = 0);

  ~Plotter();

  void setPlotSetting(PlotSetting const& settings);

  void setRegionData(int const& id, MatNxN const& data);

  void setCurveData(int const& id, MatNxN const& data);

  void setPointData(int const& id, MatNxN const& data);

  void clearCurve(int const& id);

  QSize minimumSizeHint() const;

  QSize sizeHint() const;

 public slots:
  void zoomIn();

  void zoomOut();

 protected:
  void refreshPixmap();

  virtual void paintEvent(QPaintEvent* event);

  virtual void resizeEvent(QResizeEvent* event);

  virtual void mousePressEvent(QMouseEvent* event);

  virtual void mouseMoveEvent(QMouseEvent* event);

  virtual void mouseReleaseEvent(QMouseEvent* event);

  virtual void keyPressEvent(QKeyEvent* event);

  virtual void wheelEvent(QWheelEvent* event);

 private:
  class Imple;
  QScopedPointer<Imple> _p;
};

#endif  // PLOTTER_PLOTTER_H_
