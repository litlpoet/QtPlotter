// Copyright 2015 Byungkuk Choi

#ifndef PLOTTER_PLOTSETTING_H_
#define PLOTTER_PLOTSETTING_H_

#include <QtCore/QScopedPointer>

class PlotSetting {
 public:
  enum { MinTicks = 4 };

  PlotSetting();

  PlotSetting(PlotSetting const& other);

  ~PlotSetting();

  PlotSetting& operator=(PlotSetting const& other);

  void scroll(int const& dx, int const& dy);

  void adjust();

  float spanX() const;

  float spanY() const;

  int const& numberOfXTicks() const;

  int const& numberOfYTicks() const;

  float const& minX() const;

  float const& maxX() const;

  float const& minY() const;

  float const& maxY() const;

  void setMinX(float const& min_x);

  void setMaxX(float const& max_x);

  void setMinY(float const& min_y);

  void setMaxY(float const& max_y);

 protected:
  void test();

 private:
  class Imple;
  QScopedPointer<Imple> _p;
};

#endif  // PLOTTER_PLOTSETTING_H_
