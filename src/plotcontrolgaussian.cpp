#include "plotcontrolgaussian.h"

#include "plotmodelinterface.h"
#include "plotviewgaussian.h"

class PlotControlGaussian::Imple {
 public:
  PlotModelInterface* _model;
  PlotViewGaussian* _view;

  Imple(PlotModelInterface* model) : _model(model), _view(nullptr) {}

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
