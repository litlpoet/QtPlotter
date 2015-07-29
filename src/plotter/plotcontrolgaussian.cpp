// Copyright (c) 2015 Byungkuk Choi

#include "plotter/plotcontrolgaussian.h"

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

void PlotControlGaussian::showPlotter() { _p->_view->showPlotter(); }

void PlotControlGaussian::setBoundary(const bool& b) {
  _p->_model->setBoundary(b);
}

void PlotControlGaussian::setPrecision(const float& w) { _p->_model->solve(w); }
