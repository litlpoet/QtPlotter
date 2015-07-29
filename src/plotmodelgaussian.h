#ifndef PLOTMODELGAUSSIAN_H_
#define PLOTMODELGAUSSIAN_H_

#include "plotmodelinterface.h"

#include <memory>

class PlotModelGaussian : public PlotModelInterface {
 public:
  PlotModelGaussian();

  ~PlotModelGaussian();

  void initializeGaussianModel(
      ML::GaussianInterpolationNoisy* g_interp_noisy) final;

  void setBoundary(const bool& b) final;

  void solve(const float& lambda) final;

  int getDataDimension() final;

  void get1dCurve(const int& d, const float& end_time, ML::MatNxN& C) final;

  void getMean(ML::MatNxN& Mu) final;

  void getVariance(ML::MatNxN& Sigma) final;

  void registerObserver(Observer* observer) final;

  void removeObserver(Observer* observer) final;

 protected:
  void notifyObservers();

 private:
  class Imple;
  std::unique_ptr<Imple> _p;
};

#endif  // PLOTMODELGAUSSIAN_H_
