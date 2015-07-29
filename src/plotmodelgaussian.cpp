#include "plotmodelgaussian.h"

#include <vector>
#include <algorithm>
#include <MLGaussian/gaussianinterpolationnoisy.h>

#include "observer.h"

class PlotModelGaussian::Imple {
 public:
  float _prev_lambda;
  ML::GaussianInterpolationNoisy* _g_interp_noisy;  // owned by this class
  ML::MatNxN _Mu;
  ML::MatNxN _Sigma;
  std::vector<Observer*> _observers;

  Imple() : _prev_lambda(1.0f), _g_interp_noisy(nullptr) {}

  ~Imple() {
    if (_g_interp_noisy) delete _g_interp_noisy;
  }
};

PlotModelGaussian::PlotModelGaussian() : _p(new PlotModelGaussian::Imple) {}

PlotModelGaussian::~PlotModelGaussian() {}

void PlotModelGaussian::initializeGaussianModel(
    ML::GaussianInterpolationNoisy* g_interp_noisy) {
  _p->_g_interp_noisy = g_interp_noisy;
}

void PlotModelGaussian::setBoundary(const bool& b) {
  if (!_p->_g_interp_noisy) return;
  _p->_g_interp_noisy->setBoundaryConstraint(b);
  solve(_p->_prev_lambda);
}

void PlotModelGaussian::solve(const float& lambda) {
  if (!_p->_g_interp_noisy) return;
  _p->_prev_lambda = lambda;
  _p->_g_interp_noisy->solve(lambda, &_p->_Mu, &_p->_Sigma);
  notifyObservers();
}

int PlotModelGaussian::getDataDimension() { return _p->_Mu.cols(); }

void PlotModelGaussian::get1dCurve(const int& d, const float& end_time,
                                   ML::MatNxN& C) {
  if (_p->_Mu.cols() <= d) return;
  C.resize(_p->_Mu.rows(), 2);
  C << ML::VecN::LinSpaced(_p->_Mu.rows(), 0.0, end_time), _p->_Mu.col(d);
}

void PlotModelGaussian::getMean(ML::MatNxN& Mu) { Mu = _p->_Mu; }

void PlotModelGaussian::getVariance(ML::MatNxN& Sigma) { Sigma = _p->_Sigma; }

void PlotModelGaussian::registerObserver(Observer* observer) {
  _p->_observers.push_back(observer);
}

void PlotModelGaussian::removeObserver(Observer* observer) {
  auto it = std::find(_p->_observers.begin(), _p->_observers.end(), observer);
  if (it != _p->_observers.end()) _p->_observers.erase(it);
}

void PlotModelGaussian::notifyObservers() {
  for (auto it : _p->_observers) it->update();
}
