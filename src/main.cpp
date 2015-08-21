// Copyright 2015 Byungkuk

#include <QtWidgets/QApplication>

#include "plotter/plotter.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  Q_INIT_RESOURCE(plotter);
  Plotter* plotter = new Plotter();
  plotter->show();
  return a.exec();
}
