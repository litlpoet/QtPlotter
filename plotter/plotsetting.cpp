// Copyright (c) 2015 Byungkuk Choi

#include "plotter/plotsetting.h"

class PlotSetting::Imple {
 public:
  int _n_x_ticks{5};
  int _n_y_ticks{5};
  float _min_x{0.0f};
  float _max_x{10.0f};
  float _min_y{0.0f};
  float _max_y{10.0f};

  Imple() {}

  ~Imple() {}

  void clone(PlotSetting const& other) {
    _n_x_ticks = other.numberOfXTicks();
    _n_y_ticks = other.numberOfYTicks();
    _min_x = other.minX();
    _max_x = other.maxX();
    _min_y = other.minY();
    _max_y = other.maxY();
  }

  void adjustAxis(float* min, float* max, int* ticks) {
    float gross_step = (*max - *min) / MinTicks;
    float step = powf(10.0, floorf(log10f(gross_step)));

    if (5 * step < gross_step)
      step *= 5;
    else if (2 * step < gross_step)
      step *= 2;

    *ticks = static_cast<int>(ceilf(*max / step) - floorf(*min / step));
    if (*ticks < MinTicks) *ticks = MinTicks;

    *min = floorf(*min / step) * step;
    *max = ceilf(*max / step) * step;
  }
};

PlotSetting::PlotSetting() : _p(new PlotSetting::Imple) {}

PlotSetting::PlotSetting(PlotSetting const& other)
    : _p(new PlotSetting::Imple) {
  _p->clone(other);
}

PlotSetting::~PlotSetting() {}

PlotSetting& PlotSetting::operator=(PlotSetting const& other) {
  if (this != &other) _p->clone(other);
  return *this;
}

void PlotSetting::scroll(int const& dx, int const& dy) {
  float step_x = spanX() / _p->_n_x_ticks;
  _p->_min_x += dx * step_x;
  _p->_max_x += dx * step_x;

  float step_y = spanY() / _p->_n_y_ticks;
  _p->_min_y += dy * step_y;
  _p->_max_y += dy * step_y;
}

void PlotSetting::adjust() {
  _p->adjustAxis(&_p->_min_x, &_p->_max_x, &_p->_n_x_ticks);
  _p->adjustAxis(&_p->_min_y, &_p->_max_y, &_p->_n_y_ticks);
}

float PlotSetting::spanX() const { return _p->_max_x - _p->_min_x; }

float PlotSetting::spanY() const { return _p->_max_y - _p->_min_y; }

int const& PlotSetting::numberOfXTicks() const { return _p->_n_x_ticks; }

int const& PlotSetting::numberOfYTicks() const { return _p->_n_y_ticks; }

float const& PlotSetting::minX() const { return _p->_min_x; }

float const& PlotSetting::maxX() const { return _p->_max_x; }

float const& PlotSetting::minY() const { return _p->_min_y; }

float const& PlotSetting::maxY() const { return _p->_max_y; }

void PlotSetting::setMinX(float const& min_x) { _p->_min_x = min_x; }

void PlotSetting::setMaxX(float const& max_x) { _p->_max_x = max_x; }

void PlotSetting::setMinY(float const& min_y) { _p->_min_y = min_y; }

void PlotSetting::setMaxY(float const& max_y) { _p->_max_y = max_y; }
