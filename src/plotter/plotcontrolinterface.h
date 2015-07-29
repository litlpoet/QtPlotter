// Copyright (c) 2015 Byungkuk Choi

#ifndef SRC_PLOTTER_PLOTCONTROLINTERFACE_H_
#define SRC_PLOTTER_PLOTCONTROLINTERFACE_H_

class PlotControlInterface {
 public:
  PlotControlInterface() {}

  virtual ~PlotControlInterface() {}

  virtual void showPlotter() = 0;

  virtual void setBoundary(const bool& b) = 0;

  virtual void setPrecision(const float& w) = 0;
};

#endif  // SRC_PLOTTER_PLOTCONTROLINTERFACE_H_
