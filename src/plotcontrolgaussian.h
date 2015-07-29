#ifndef PLOTCONTROLGAUSSAIN_H_
#define PLOTCONTROLGAUSSAIN_H_

#include "plotcontrolinterface.h"

#include <memory>

class PlotModelInterface;

class PlotControlGaussian : public PlotControlInterface {
 public:
  PlotControlGaussian(PlotModelInterface* model);

  ~PlotControlGaussian();

  void showPlotter() final;

  void setBoundary(const bool& b) final;

  void setPrecision(const float& w) final;

 private:
  class Imple;
  std::unique_ptr<Imple> _p;
};

#endif  // PLOTCONTROLGAUSSAIN_H_
