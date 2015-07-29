#ifndef PLOTVIEWGAUSSIAN_H_
#define PLOTVIEWGAUSSIAN_H_

#include "observer.h"
#include "plotter.h"

#include <memory>

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

#endif  // PLOTVIEWGAUSSIAN_H_
