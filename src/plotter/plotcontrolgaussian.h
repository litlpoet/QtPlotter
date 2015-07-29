// Copyright (c) 2015 Byungkuk Choi

#ifndef SRC_PLOTTER_PLOTCONTROLGAUSSIAN_H_
#define SRC_PLOTTER_PLOTCONTROLGAUSSIAN_H_

#include <memory>
#include "plotter/plotcontrolinterface.h"

class PlotModelInterface;

class PlotControlGaussian : public PlotControlInterface {
 public:
  explicit PlotControlGaussian(PlotModelInterface* model);

  ~PlotControlGaussian();

  void showPlotter() final;

  void setBoundary(const bool& b) final;

  void setPrecision(const float& w) final;

 private:
  class Imple;
  std::unique_ptr<Imple> _p;
};

#endif  // SRC_PLOTTER_PLOTCONTROLGAUSSIAN_H_
