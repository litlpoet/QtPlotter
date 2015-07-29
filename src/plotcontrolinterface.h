#ifndef PLOTCONTROLINTERFACE_H_
#define PLOTCONTROLINTERFACE_H_

class PlotControlInterface {
 public:
  PlotControlInterface() {}

  virtual ~PlotControlInterface() {}

  virtual void showPlotter() = 0;

  virtual void setBoundary(const bool& b) = 0;

  virtual void setPrecision(const float& w) = 0;
};

#endif  // PLOTCONTROLINTERFACE_H_
