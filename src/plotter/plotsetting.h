// Copyright 2015 Byungkuk Choi

#ifndef SRC_PLOTTER_PLOTSETTING_H_
#define SRC_PLOTTER_PLOTSETTING_H_

#include <QtCore/QScopedPointer>

class PlotSetting {
 public:
  enum { MinTicks = 4 };

  PlotSetting();

  PlotSetting(const PlotSetting& other);

  ~PlotSetting();

  void scroll(const int& dx, const int& dy);

  void adjust();

  float spanX() const;

  float spanY() const;

  const int& numberOfXTicks() const;

  const int& numberOfYTicks() const;

  const float& minX() const;

  const float& maxX() const;

  const float& minY() const;

  const float& maxY() const;

  void setMinX(const float& min_x);

  void setMaxX(const float& max_x);

  void setMinY(const float& min_y);

  void setMaxY(const float& max_y);

  PlotSetting& operator=(const PlotSetting& other);

 private:
  class Imple;
  QScopedPointer<Imple> _p;
};

#endif  // SRC_PLOTTER_PLOTSETTING_H_
