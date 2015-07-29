// Copyright (c) 2015 Byungkuk Choi.

#ifndef SRC_PLOTTER_PLOTVIEWGAUSSIAN_H_
#define SRC_PLOTTER_PLOTVIEWGAUSSIAN_H_

#include <memory>
#include "plotter/observer.h"
#include "plotter/plotter.h"

class PlotModelInterface;
class PlotControlInterface;

class PlotViewGaussian : public Observer, public Plotter {
 public:
  PlotViewGaussian(PlotModelInterface* model, PlotControlInterface* ctrl);

  ~PlotViewGaussian();

  void update() final;

  void changeWeight(const int& w);

  void toggleBoundary(const bool& b);

  void toggleNoBoundary(const bool& b);

  void showPlotter();

 protected:
  void resizeEvent(QResizeEvent* event);

 private:
  class Imple;
  std::unique_ptr<Imple> _p;
};

#endif  // SRC_PLOTTER_PLOTVIEWGAUSSIAN_H_
