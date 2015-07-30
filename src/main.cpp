// Copyright 2015 Byungkuk

#include <MLGaussian/gaussianinterpolation.h>
#include <MLGaussian/gaussianinterpolationnoisy.h>

#include <QtWidgets/QApplication>
#include <algorithm>
#include <iostream>
#include <vector>

#include "plotter/plotmodelgaussian.h"
#include "plotter/plotcontrolgaussian.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);

  ML::TimeSeriesMap time_series_map;

  time_series_map.insert(ML::MakeTimeSample(1, 2, 0.1f, 0.0f));
  time_series_map.insert(ML::MakeTimeSample(10, 2, 0.5f, 1.0f));
  time_series_map.insert(ML::MakeTimeSample(11, 2, 1.1f, 0.5f));
  time_series_map.insert(ML::MakeTimeSample(13, 2, 1.3f, 1.5f));
  time_series_map.insert(ML::MakeTimeSample(20, 2, -1.0f, 2.0f));
  time_series_map.insert(ML::MakeTimeSample(22, 2, -1.7f, 2.0f));
  time_series_map.insert(ML::MakeTimeSample(40, 2, 1.0f, 0.0f));
  time_series_map.insert(ML::MakeTimeSample(50, 2, 0.8f, -1.5f));
  time_series_map.insert(ML::MakeTimeSample(51, 2, 1.0f, -1.0f));
  time_series_map.insert(ML::MakeTimeSample(70, 2, 1.4f, 2.0f));
  time_series_map.insert(ML::MakeTimeSample(80, 2, 2.9f, 1.0f));
  time_series_map.insert(ML::MakeTimeSample(85, 2, 3.0f, 0.0f));

  ML::GaussianInterpolation g_interp(100, time_series_map);
  int i = 0;
  ML::MatNxN P(time_series_map.size(), 2);
  for (auto it : time_series_map) {
    P.row(i++) = it.second.transpose();
  }

  ML::GaussianInterpolationNoisy* g_interp_noisy =
      new ML::GaussianInterpolationNoisy(100, time_series_map);

  std::cout << "Make model" << std::endl;
  PlotModelInterface* model = new PlotModelGaussian();
  model->initializeGaussianModel(g_interp_noisy);
  std::cout << "Noisy interpolation initialized" << std::endl;

  PlotControlInterface* ctrl = new PlotControlGaussian(model);
  ctrl->showPlotter();
  std::cout << "Show Plot view" << std::endl;

  return a.exec();
}
