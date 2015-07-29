// Copyright (c) 2015 Byungkuk Choi

#ifndef SRC_PLOTTER_PLOTMODELINTERFACE_H_
#define SRC_PLOTTER_PLOTMODELINTERFACE_H_

#include <MLCore/mathtypes.h>

namespace ML {
class GaussianInterpolationNoisy;
}

class Observer;

class PlotModelInterface {
 public:
  PlotModelInterface() {}

  virtual ~PlotModelInterface() {}

  virtual void initializeGaussianModel(
      ML::GaussianInterpolationNoisy* g_interp_nosiy) = 0;

  virtual void setBoundary(const bool& b) = 0;

  virtual void solve(const float& lambda) = 0;

  virtual int getDataDimension() = 0;

  virtual void get1dCurve(const int& d, const float& end_time,
                          ML::MatNxN* C) = 0;

  virtual void getMean(ML::MatNxN* Mu) = 0;

  virtual void getVariance(ML::MatNxN* Sigma) = 0;

  virtual void registerObserver(Observer* observer) = 0;

  virtual void removeObserver(Observer* observer) = 0;
};

#endif  // SRC_PLOTTER_PLOTMODELINTERFACE_H_
