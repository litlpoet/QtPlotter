#include "plotviewgaussian.h"

#include <iostream>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>

#include "plotmodelinterface.h"
#include "plotcontrolinterface.h"

class PlotViewGaussian::Imple {
 public:
  QRadioButton* _b1_rbtn;
  QRadioButton* _b2_rbtn;
  QLabel* _w_label;
  QSlider* _w_slider;
  PlotModelInterface* _model;
  PlotControlInterface* _ctrl;

  Imple(PlotModelInterface* model, PlotControlInterface* ctrl)
      : _w_label(nullptr), _w_slider(nullptr), _model(model), _ctrl(ctrl) {}

  ~Imple() {}

  void createUi(PlotViewGaussian* plot_view) {
    _b1_rbtn = new QRadioButton("boundary", plot_view);
    _b2_rbtn = new QRadioButton("no boundary", plot_view);
    _b1_rbtn->setDown(true);

    _w_label = new QLabel("lambda", plot_view);
    _w_label->setAlignment(Qt::AlignRight);
    _w_label->adjustSize();

    _w_slider = new QSlider(Qt::Horizontal, plot_view);
    _w_slider->setMinimum(1);
    _w_slider->setMaximum(1000);
    _w_slider->setTickInterval(1);
    _w_slider->setValue(20);
    _w_slider->adjustSize();

    QObject::connect(_b1_rbtn, &QRadioButton::toggled, plot_view,
                     &PlotViewGaussian::toggleBoundary);
    QObject::connect(_b2_rbtn, &QRadioButton::toggled, plot_view,
                     &PlotViewGaussian::toggleNoBoundary);
    QObject::connect(_w_slider, &QSlider::valueChanged, plot_view,
                     &PlotViewGaussian::changeWeight);
  }
};

PlotViewGaussian::PlotViewGaussian(PlotModelInterface* model,
                                   PlotControlInterface* ctrl)
    : Plotter(), _p(new PlotViewGaussian::Imple(model, ctrl)) {
  _p->_model->registerObserver(this);
  _p->createUi(this);
}

PlotViewGaussian::~PlotViewGaussian() {}

void PlotViewGaussian::update() {
  ML::MatNxN C;
  for (int i = 0, n = _p->_model->getDataDimension(); i < n; ++i) {
    _p->_model->get1dCurve(i, 10.0f, C);
    clearCurve(i);
    setCurveData(i, C);
  }
}

void PlotViewGaussian::changeWeight(const int& w) {
  float real_w = static_cast<float>(w) / 20;
  std::cout << "current weight: " << real_w << std::endl;
  _p->_ctrl->setPrecision(real_w);
}

void PlotViewGaussian::toggleBoundary(const bool& b) {
  if (b) _p->_ctrl->setBoundary(true);
}

void PlotViewGaussian::toggleNoBoundary(const bool& b) {
  if (b) _p->_ctrl->setBoundary(false);
}

void PlotViewGaussian::showPlotter() { show(); }

void PlotViewGaussian::resizeEvent(QResizeEvent* event) {
  int mid_x = width() / 2;
  _p->_b1_rbtn->move(5, height() - 25);
  _p->_b2_rbtn->move(90, height() - 25);
  _p->_w_label->move(mid_x - _p->_w_label->width() - 10, height() - 25);
  _p->_w_slider->move(mid_x, height() - 30);
  Plotter::resizeEvent(event);
}
