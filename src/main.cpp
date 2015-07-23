// Copyright 2015 Byungkuk

#include <QtWidgets/QApplication>
#include <algorithm>
#include <iostream>
#include <vector>
#include <MLGaussian/gaussianinterpolation.h>

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

  ML::MatNxN mu, sigma;
  g_interp.solve(0.1f, &mu /*, &sigma*/);

  ML::MatNxN C(100, 2);
  //  ML::MatNxN R(100, 3);
  for (int i = 0; i < 100; ++i) C.row(i) = mu.row(i);

  w.setCurveData(0, C);
  w.show();

  return a.exec();
}
