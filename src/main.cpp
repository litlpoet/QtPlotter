// Copyright 2015 Byungkuk

#include <MLGaussian/gaussianinterpolation.h>
#include <MLGaussian/gaussianinterpolationnoisy.h>

#include <QtWidgets/QApplication>
#include <algorithm>
#include <iostream>
#include <vector>

#include "plotter.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);

  Plotter w;

  ML::TimeSeriesMap time_series_map;
  ML::VecN s(2);

  time_series_map.insert(ML::MakeTimeSample(1, 2, 0.0f, 0.0f));
  time_series_map.insert(ML::MakeTimeSample(36, 2, 1.0f, 2.0f));
  time_series_map.insert(ML::MakeTimeSample(40, 2, 3.0f, 3.0f));
  time_series_map.insert(ML::MakeTimeSample(70, 2, 1.0f, 2.0f));
  time_series_map.insert(ML::MakeTimeSample(95, 2, 4.0f, 0.0f));
  time_series_map.insert(ML::MakeTimeSample(99, 2, 5.0f, 3.0f));

  ML::GaussianInterpolation g_interp(100, time_series_map);
  ML::GaussianInterpolationNoisy g_interp_noisy(100, time_series_map);

  ML::MatNxN mu, sigma;
  g_interp.solve(0.1f, &mu /*, &sigma*/);

  ML::MatNxN mu2, sigma2;
  g_interp_noisy.solve(30.0f, &mu2, &sigma2);

  ML::MatNxN C(100, 2);
  ML::MatNxN C2(100, 2);
  //  ML::MatNxN R(100, 3);
  for (int i = 0; i < 100; ++i) {
    C.row(i) = mu.row(i);
    C2.row(i) = mu2.row(i);
  }

  int i = 0;
  ML::MatNxN P(time_series_map.size(), 2);
  for (auto it : time_series_map) {
    P.row(i++) = it.second.transpose();
  }
  w.setCurveData(0, C);
  w.setCurveData(1, C2);
  w.setPointData(2, P);
  w.show();

  return a.exec();
}
