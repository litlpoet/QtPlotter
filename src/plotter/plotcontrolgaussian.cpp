// Copyright (c) 2015 Byungkuk Choi

#include "plotter/plotcontrolgaussian.h"

#include <iostream>
#include "plotter/plotmodelinterface.h"
#include "plotter/plotviewgaussian.h"

class PlotControlGaussian::Imple {
 public:
  PlotModelInterface* _model;
  PlotViewGaussian* _view;

  explicit Imple(PlotModelInterface* model) : _model(model), _view(nullptr) {}

  ~Imple() {}
};

PlotControlGaussian::PlotControlGaussian(PlotModelInterface* model)
    : _p(new PlotControlGaussian::Imple(model)) {
  _p->_view = new PlotViewGaussian(model, this);
}

PlotControlGaussian::~PlotControlGaussian() {}

void PlotControlGaussian::showPlotter() {
  for (int i = 0, n = _p->_model->getDataDimension(); i < n; ++i) {
    ML::MatNxN P;
    _p->_model->getSample(i, 10.0f, &P);
    _p->_view->setPointData(i, P);
  }
  _p->_view->showPlotter();
}

void PlotControlGaussian::setBoundary(const bool& b) {
  _p->_model->setBoundary(b);
}

void PlotControlGaussian::setPrecision(const float& w) { _p->_model->solve(w); }
