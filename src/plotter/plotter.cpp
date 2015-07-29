// Copyright 2015 Byungkuk Choi

#include "plotter/plotter.h"

#include <QtCore/QDebug>
#include <QtCore/QMap>
#include <QtCore/QVector>
#include <QtGui/QMouseEvent>
#include <QtGui/QPixmap>
#include <QtWidgets/QStyleOptionFocusRect>
#include <QtWidgets/QStylePainter>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QSlider>

#include "plotter/plotsetting.h"

const QColor Plotter::colors[6] = {Qt::red,  Qt::green,   Qt::blue,
                                   Qt::cyan, Qt::magenta, Qt::yellow};

class Plotter::Imple {
 public:
  bool _is_rubburband_shown;
  int _cur_zoom_idx;
  QToolButton* _btn_zoomin;
  QToolButton* _btn_zoomout;
  QVector<PlotSetting> _zoom_stack;
  QMap<int, MatrixXf> _region_map;
  QMap<int, MatrixXf> _curve_map;
  QMap<int, MatrixXf> _point_map;
  QRect _rubberband_rect;
  QPixmap _pixmap;

  Imple()
      : _is_rubburband_shown(false),
        _cur_zoom_idx(0),
        _btn_zoomin(nullptr),
        _btn_zoomout(nullptr) {}

  ~Imple() {}

  void createButtons(Plotter* plotter);

  void updateRubberbandRegion(Plotter* plotter);

  void applyZoom(Plotter* plotter, QRect* rect);

  void drawGrid(Plotter* plotter, QPainter* painter);

  void drawRegion(Plotter* plotter, QPainter* painter);

  void drawCurves(Plotter* plotter, QPainter* painter);

  void drawPoints(Plotter* plotter, QPainter* painter);
};

Plotter::Plotter(QWidget* parent) : QWidget(parent), _p(new Plotter::Imple()) {
  setBackgroundRole(QPalette::Dark);
  setAutoFillBackground(true);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setFocusPolicy(Qt::StrongFocus);
  _p->createButtons(this);
  setPlotSetting(PlotSetting());
}

Plotter::~Plotter() {}

void Plotter::setPlotSetting(const PlotSetting& settings) {
  _p->_zoom_stack.clear();
  _p->_zoom_stack.append(settings);
  _p->_cur_zoom_idx = 0;
  _p->_btn_zoomin->hide();
  _p->_btn_zoomout->hide();
  refreshPixmap();
}

void Plotter::setRegionData(const int& id, const Eigen::MatrixXf& data) {
  _p->_region_map.insert(id, data);
  refreshPixmap();
}

void Plotter::setCurveData(const int& id, const MatrixXf& data) {
  _p->_curve_map.insert(id, data);
  refreshPixmap();
}

void Plotter::setPointData(const int& id, const Eigen::MatrixXf& data) {
  _p->_point_map.insert(id, data);
  refreshPixmap();
}

void Plotter::clearCurve(const int& id) {
  _p->_curve_map.remove(id);
  refreshPixmap();
}

QSize Plotter::minimumSizeHint() const { return QSize(6 * Margin, 4 * Margin); }

QSize Plotter::sizeHint() const { return QSize(12 * Margin, 8 * Margin); }

void Plotter::zoomIn() {
  if (_p->_cur_zoom_idx >= _p->_zoom_stack.count() - 1) return;

  ++(_p->_cur_zoom_idx);
  _p->_btn_zoomin->setEnabled(_p->_cur_zoom_idx < _p->_zoom_stack.count() - 1);
  _p->_btn_zoomout->setEnabled(true);
  _p->_btn_zoomout->show();
  refreshPixmap();
}

void Plotter::zoomOut() {
  if (_p->_cur_zoom_idx <= 0) return;

  --(_p->_cur_zoom_idx);
  _p->_btn_zoomout->setEnabled(_p->_cur_zoom_idx > 0);
  _p->_btn_zoomin->setEnabled(true);
  _p->_btn_zoomin->show();
  refreshPixmap();
}

void Plotter::refreshPixmap() {
  _p->_pixmap = QPixmap(size());
  _p->_pixmap.fill(palette().dark().color());

  QPainter painter(&_p->_pixmap);
  painter.initFrom(this);
  _p->drawGrid(this, &painter);
  _p->drawRegion(this, &painter);
  _p->drawCurves(this, &painter);
  _p->drawPoints(this, &painter);
  update();
}

void Plotter::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event)
  QStylePainter painter(this);
  painter.drawPixmap(0, 0, _p->_pixmap);

  if (_p->_is_rubburband_shown) {
    painter.setPen(palette().light().color());
    painter.drawRect(_p->_rubberband_rect.normalized().adjusted(0, 0, -1, -1));
  }

  if (hasFocus()) {
    QStyleOptionFocusRect option;
    option.initFrom(this);
    option.backgroundColor = palette().dark().color();
    painter.drawPrimitive(QStyle::PE_FrameFocusRect, option);
  }
}

void Plotter::resizeEvent(QResizeEvent* event) {
  Q_UNUSED(event)
  int x = width() - (_p->_btn_zoomin->width() + _p->_btn_zoomout->width() + 10);
  _p->_btn_zoomin->move(x, 5);
  _p->_btn_zoomout->move(x + _p->_btn_zoomin->width() + 5, 5);
  refreshPixmap();
}

void Plotter::mousePressEvent(QMouseEvent* event) {
  QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);

  if (event->button() == Qt::LeftButton) {
    if (!rect.contains(event->pos())) return;

    _p->_is_rubburband_shown = true;
    _p->_rubberband_rect.setTopLeft(event->pos());
    _p->_rubberband_rect.setBottomRight(event->pos());
    _p->updateRubberbandRegion(this);
    setCursor(Qt::CrossCursor);
  }
}

void Plotter::mouseMoveEvent(QMouseEvent* event) {
  if (_p->_is_rubburband_shown) {
    _p->updateRubberbandRegion(this);
    _p->_rubberband_rect.setBottomRight(event->pos());
    _p->updateRubberbandRegion(this);
  }
}

void Plotter::mouseReleaseEvent(QMouseEvent* event) {
  if ((event->button() == Qt::LeftButton) && _p->_is_rubburband_shown) {
    _p->_is_rubburband_shown = false;
    _p->updateRubberbandRegion(this);
    unsetCursor();

    QRect rect = _p->_rubberband_rect.normalized();
    if ((rect.width() < 4) || (rect.height() < 4)) return;

    _p->applyZoom(this, &rect);
  }
}

void Plotter::keyPressEvent(QKeyEvent* event) {
  PlotSetting& cur_plot_setting = _p->_zoom_stack[_p->_cur_zoom_idx];
  auto applyScroll = [&cur_plot_setting](const int& dx, const int& dy)
                         -> void { cur_plot_setting.scroll(dx, dy); };

  switch (event->key()) {
    case Qt::Key_Plus:
      zoomIn();
      break;
    case Qt::Key_Minus:
      zoomOut();
      break;
    case Qt::Key_Left:
      applyScroll(-1, 0);
      break;
    case Qt::Key_Right:
      applyScroll(+1, 0);
      break;
    case Qt::Key_Up:
      applyScroll(0, +1);
      break;
    case Qt::Key_Down:
      applyScroll(0, -1);
      break;
    default:
      QWidget::keyPressEvent(event);
      return;
  }
  refreshPixmap();
}

void Plotter::wheelEvent(QWheelEvent* event) {
  PlotSetting& cur_plot_setting = _p->_zoom_stack[_p->_cur_zoom_idx];
  auto applyScroll = [&cur_plot_setting](const int& dx, const int& dy)
                         -> void { cur_plot_setting.scroll(dx, dy); };

  const int num_ticks = event->delta() / 120;

  if (event->orientation() == Qt::Horizontal)
    applyScroll(num_ticks, 0);
  else
    applyScroll(0, num_ticks);

  refreshPixmap();
}

void Plotter::Imple::createButtons(Plotter* plotter) {
  _btn_zoomin = new QToolButton(plotter);
  _btn_zoomin->setIcon(QIcon(":/images/zoomin.png"));
  _btn_zoomin->adjustSize();

  _btn_zoomout = new QToolButton(plotter);
  _btn_zoomout->setIcon(QIcon(":/images/zoomout.png"));
  _btn_zoomout->adjustSize();

  QObject::connect(_btn_zoomin, &QAbstractButton::clicked, plotter,
                   &Plotter::zoomIn);
  QObject::connect(_btn_zoomout, &QAbstractButton::clicked, plotter,
                   &Plotter::zoomOut);
}

void Plotter::Imple::updateRubberbandRegion(Plotter* plotter) {
  QRect rect = _rubberband_rect.normalized();
  plotter->update(rect.left(), rect.top(), rect.width(), 1);
  plotter->update(rect.left(), rect.top(), 1, rect.height());
  plotter->update(rect.left(), rect.bottom(), rect.width(), 1);
  plotter->update(rect.right(), rect.top(), 1, rect.height());
}

void Plotter::Imple::applyZoom(Plotter* plotter, QRect* rect) {
  rect->translate(-Margin, -Margin);

  const PlotSetting& prev_setting = _zoom_stack.at(_cur_zoom_idx);
  PlotSetting new_setting;
  float dx = prev_setting.spanX() / (plotter->width() - 2 * Margin);
  float dy = prev_setting.spanY() / (plotter->height() - 2 * Margin);
  new_setting.setMinX(prev_setting.minX() + dx * rect->left());
  new_setting.setMaxX(prev_setting.minX() + dx * rect->right());
  new_setting.setMinY(prev_setting.maxY() - dy * rect->bottom());
  new_setting.setMaxY(prev_setting.maxY() - dy * rect->top());
  new_setting.adjust();

  _zoom_stack.resize(_cur_zoom_idx + 1);
  _zoom_stack.append(new_setting);
  plotter->zoomIn();
}

void Plotter::Imple::drawGrid(Plotter* plotter, QPainter* painter) {
  QRect rect(Margin, Margin, plotter->width() - 2 * Margin,
             plotter->height() - 2 * Margin);

  if (!rect.isValid()) return;

  const PlotSetting& settings = _zoom_stack.at(_cur_zoom_idx);
  QPen quite_dark = plotter->palette().dark().color().light();
  quite_dark.setStyle(Qt::DotLine);
  QPen light = plotter->palette().light().color();

  const float dx =
      static_cast<float>(rect.width() - 1) / settings.numberOfXTicks();
  const float dx_t = settings.spanX() / settings.numberOfXTicks();

  for (int i = 0; i <= settings.numberOfXTicks(); ++i) {
    int x = rect.left() + i * dx;
    painter->setPen(quite_dark);
    painter->drawLine(x, rect.top(), x, rect.bottom());
    painter->setPen(light);
    painter->drawLine(x, rect.bottom(), x, rect.bottom() + 5);
    painter->drawText(x - 50, rect.bottom() + 8, 100, 20,
                      Qt::AlignHCenter | Qt::AlignTop,
                      QString::number(settings.minX() + i * dx_t));
  }

  const float dy =
      static_cast<float>(rect.height() - 1) / settings.numberOfYTicks();
  const float dy_t = settings.spanY() / settings.numberOfYTicks();

  for (int i = 0; i <= settings.numberOfYTicks(); ++i) {
    int y = rect.bottom() - i * dy;
    painter->setPen(quite_dark);
    painter->drawLine(rect.left(), y, rect.right(), y);
    painter->setPen(light);
    painter->drawLine(rect.left() - 5, y, rect.left(), y);
    painter->drawText(rect.left() - Margin, y - 10, Margin - 7, 20,
                      Qt::AlignVCenter | Qt::AlignRight,
                      QString::number(settings.minY() + i * dy_t));
  }

  painter->drawRect(rect.adjusted(0, 0, -1, -1));
}

void Plotter::Imple::drawRegion(Plotter* plotter, QPainter* painter) {
  QRect rect(Margin, Margin, plotter->width() - 2 * Margin,
             plotter->height() - 2 * Margin);

  if (!rect.isValid() || (_region_map.size() == 0)) return;

  painter->setClipRect(rect.adjusted(+1, +1, -1, -1));
  const PlotSetting& settings = _zoom_stack.at(_cur_zoom_idx);

  QMap<int, MatrixXf>::iterator iter = _region_map.begin();

  for (; iter != _region_map.end(); ++iter) {
    const int& id = iter.key();
    const MatrixXf& data = iter.value();

    QPolygonF polygon(data.rows() * 2);

    for (int i = 0; i < data.rows(); ++i) {
      float dx = data(i, 0) - settings.minX();
      float dy = data(i, 1) - settings.minY();
      float x = rect.left() + (dx * (rect.width() - 1) / settings.spanX());
      float y = rect.bottom() - (dy * (rect.height() - 1) / settings.spanY());
      polygon[i] = QPointF(x, y);
    }

    for (int i = data.rows(), j = data.rows() - 1; i < data.rows() * 2;
         ++i, --j) {
      float dx = data(j, 0) - settings.minX();
      float dy = data(j, 2) - settings.minY();
      float x = rect.left() + (dx * (rect.width() - 1) / settings.spanX());
      float y = rect.bottom() - (dy * (rect.height() - 1) / settings.spanY());
      polygon[i] = QPointF(x, y);
    }

    QPen region_pen(colors[id % 6].lighter());
    region_pen.setStyle(Qt::DashLine);
    QBrush region_brush(colors[id % 6].lighter(), Qt::Dense6Pattern);
    painter->setBrush(region_brush);
    painter->setPen(region_pen);
    painter->drawPolygon(polygon);
  }
}

void Plotter::Imple::drawCurves(Plotter* plotter, QPainter* painter) {
  QRect rect(Margin, Margin, plotter->width() - 2 * Margin,
             plotter->height() - 2 * Margin);

  if (!rect.isValid() || (_curve_map.size() == 0)) return;

  painter->setClipRect(rect.adjusted(+1, +1, -1, -1));
  const PlotSetting& settings = _zoom_stack.at(_cur_zoom_idx);

  QMap<int, MatrixXf>::iterator iter = _curve_map.begin();

  for (; iter != _curve_map.end(); ++iter) {
    const int& id = iter.key();
    const MatrixXf& data = iter.value();

    QPolygonF poly_line(data.rows());

    for (int i = 0; i < data.rows(); ++i) {
      float dx = data(i, 0) - settings.minX();
      float dy = data(i, 1) - settings.minY();
      float x = rect.left() + (dx * (rect.width() - 1) / settings.spanX());
      float y = rect.bottom() - (dy * (rect.height() - 1) / settings.spanY());
      poly_line[i] = QPointF(x, y);
    }

    QPen curve_pen(colors[id % 6]);
    curve_pen.setWidth(2);
    painter->setPen(curve_pen);
    painter->drawPolyline(poly_line);
  }
}

void Plotter::Imple::drawPoints(Plotter* plotter, QPainter* painter) {
  QRect rect(Margin, Margin, plotter->width() - 2 * Margin,
             plotter->height() - 2 * Margin);

  if (!rect.isValid() || (_point_map.size() == 0)) return;

  painter->setClipRect(rect.adjusted(+1, +1, -1, -1));
  const PlotSetting& settings = _zoom_stack.at(_cur_zoom_idx);

  QMap<int, MatrixXf>::iterator iter = _point_map.begin();

  for (; iter != _point_map.end(); ++iter) {
    const int& id = iter.key();
    const MatrixXf& data = iter.value();

    QPen point_pen(colors[id % 6].dark());
    point_pen.setWidth(2);
    painter->setPen(point_pen);

    for (int i = 0; i < data.rows(); ++i) {
      float dx = data(i, 0) - settings.minX();
      float dy = data(i, 1) - settings.minY();
      float x = rect.left() + (dx * (rect.width() - 1) / settings.spanX());
      float y = rect.bottom() - (dy * (rect.height() - 1) / settings.spanY());
      painter->drawEllipse(QPointF(x, y), 5, 5);
    }
  }
}
