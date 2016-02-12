// Copyright 2015 Byungkuk Choi

#include "plotter/plotter.h"

#include <QtCore/QDebug>
#include <QtCore/QMap>
#include <QtCore/QVector>
#include <QtGui/QMouseEvent>
#include <QtGui/QPixmap>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStyleOptionFocusRect>
#include <QtWidgets/QStylePainter>
#include <QtWidgets/QToolButton>

#include "plotter/plotsetting.h"

QColor const Plotter::colors[6] = {Qt::red,  Qt::green,   Qt::blue,
                                   Qt::cyan, Qt::magenta, Qt::yellow};

class Plotter::Imple {
 public:
  bool _is_rubburband_shown{false};
  int _cur_zoom_idx{0};
  QToolButton* _btn_zoomin{nullptr};
  QToolButton* _btn_zoomout{nullptr};
  QVector<PlotSetting> _zoom_stack;
  QMap<int, MatNxN> _region_map;
  QMap<int, MatNxN> _curve_map;
  QMap<int, MatNxN> _point_map;
  QMap<int, int> _id_2_colid;
  QRect _rubberband_rect;
  QPixmap _pixmap;

  Imple() {}

  ~Imple() {}

  void createButtons(Plotter* plotter) {
    _btn_zoomin = new QToolButton(plotter);
    _btn_zoomin->setIcon(QIcon(":/imgs/zoomin.png"));
    _btn_zoomin->adjustSize();
    _btn_zoomout = new QToolButton(plotter);
    _btn_zoomout->setIcon(QIcon(":/imgs/zoomout.png"));
    _btn_zoomout->adjustSize();

    QObject::connect(_btn_zoomin, &QAbstractButton::clicked, plotter,
                     &Plotter::zoomIn);
    QObject::connect(_btn_zoomout, &QAbstractButton::clicked, plotter,
                     &Plotter::zoomOut);
  }

  void updateRubberbandRegion(Plotter* plotter) {
    QRect R = _rubberband_rect.normalized();
    plotter->update(R.left(), R.top(), R.width(), 1);
    plotter->update(R.left(), R.top(), 1, R.height());
    plotter->update(R.left(), R.bottom(), R.width(), 1);
    plotter->update(R.right(), R.top(), 1, R.height());
  }

  void applyZoom(Plotter* plotter, QRect* R) {
    R->translate(-Margin, -Margin);

    PlotSetting const& prev_setting = _zoom_stack.at(_cur_zoom_idx);
    PlotSetting new_setting;
    float dx = prev_setting.spanX() / (plotter->width() - 2 * Margin);
    float dy = prev_setting.spanY() / (plotter->height() - 2 * Margin);
    new_setting.setMinX(prev_setting.minX() + dx * R->left());
    new_setting.setMaxX(prev_setting.minX() + dx * R->right());
    new_setting.setMinY(prev_setting.maxY() - dy * R->bottom());
    new_setting.setMaxY(prev_setting.maxY() - dy * R->top());
    new_setting.adjust();

    _zoom_stack.resize(_cur_zoom_idx + 1);
    _zoom_stack.append(new_setting);
    plotter->zoomIn();
  }

  void drawGrid(Plotter* plotter, QPainter* painter) {
    QRect R(Margin, Margin, plotter->width() - 2 * Margin,
            plotter->height() - 2 * Margin);

    if (!R.isValid()) return;

    PlotSetting const& settings = _zoom_stack.at(_cur_zoom_idx);
    QPen quite_dark = plotter->palette().dark().color().light();
    QPen light = plotter->palette().light().color();
    quite_dark.setStyle(Qt::DotLine);

    float dx = static_cast<float>(R.width() - 1) / settings.numberOfXTicks();
    float dx_t = settings.spanX() / settings.numberOfXTicks();

    for (int i = 0; i <= settings.numberOfXTicks(); ++i) {
      int x = R.left() + i * dx;
      painter->setPen(quite_dark);
      painter->drawLine(x, R.top(), x, R.bottom());
      painter->setPen(light);
      painter->drawLine(x, R.bottom(), x, R.bottom() + 5);
      painter->drawText(x - 50, R.bottom() + 8, 100, 20,
                        Qt::AlignHCenter | Qt::AlignTop,
                        QString::number(settings.minX() + i * dx_t));
    }

    float dy = static_cast<float>(R.height() - 1) / settings.numberOfYTicks();
    float dy_t = settings.spanY() / settings.numberOfYTicks();

    for (int i = 0; i <= settings.numberOfYTicks(); ++i) {
      int y = R.bottom() - i * dy;
      painter->setPen(quite_dark);
      painter->drawLine(R.left(), y, R.right(), y);
      painter->setPen(light);
      painter->drawLine(R.left() - 5, y, R.left(), y);
      painter->drawText(R.left() - Margin, y - 10, Margin - 7, 20,
                        Qt::AlignVCenter | Qt::AlignRight,
                        QString::number(settings.minY() + i * dy_t));
    }

    painter->drawRect(R.adjusted(0, 0, -1, -1));
  }

  void drawRegion(Plotter* plotter, QPainter* painter) {
    QRect R(Margin, Margin, plotter->width() - 2 * Margin,
            plotter->height() - 2 * Margin);

    if (!R.isValid() || (_region_map.size() == 0)) return;

    painter->setClipRect(R.adjusted(+1, +1, -1, -1));
    PlotSetting const& settings = _zoom_stack.at(_cur_zoom_idx);

    QMap<int, MatNxN>::iterator iter = _region_map.begin();
    for (; iter != _region_map.end(); ++iter) {
      int const& id = iter.key();
      MatNxN const& data = iter.value();
      QPolygonF polygon(data.rows() * 2);

      for (int i = 0; i < data.rows(); ++i) {
        float dx = data(i, 0) - settings.minX();
        float dy = data(i, 1) - settings.minY();
        float x = R.left() + (dx * (R.width() - 1) / settings.spanX());
        float y = R.bottom() - (dy * (R.height() - 1) / settings.spanY());
        polygon[i] = QPointF(x, y);
      }

      for (int i = data.rows(), j = data.rows() - 1; i < data.rows() * 2;
           ++i, --j) {
        float dx = data(j, 0) - settings.minX();
        float dy = data(j, 2) - settings.minY();
        float x = R.left() + (dx * (R.width() - 1) / settings.spanX());
        float y = R.bottom() - (dy * (R.height() - 1) / settings.spanY());
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

  void drawCurves(Plotter* plotter, QPainter* painter) {
    QRect R(Margin, Margin, plotter->width() - 2 * Margin,
            plotter->height() - 2 * Margin);

    if (!R.isValid() || (_curve_map.size() == 0)) return;

    painter->setClipRect(R.adjusted(+1, +1, -1, -1));
    PlotSetting const& settings = _zoom_stack.at(_cur_zoom_idx);

    QMap<int, MatNxN>::iterator iter = _curve_map.begin();
    for (; iter != _curve_map.end(); ++iter) {
      int const& id = iter.key();
      int const& col_id = _id_2_colid.value(id);
      MatNxN const& data = iter.value();
      QPolygonF poly_line(data.rows());

      for (int i = 0; i < data.rows(); ++i) {
        float dx = data(i, 0) - settings.minX();
        float dy = data(i, 1) - settings.minY();
        float x = R.left() + (dx * (R.width() - 1) / settings.spanX());
        float y = R.bottom() - (dy * (R.height() - 1) / settings.spanY());
        poly_line[i] = QPointF(x, y);
      }

      QPen curve_pen(colors[col_id % 6]);
      curve_pen.setWidth(2);
      painter->setPen(curve_pen);
      painter->drawPolyline(poly_line);
    }
  }

  void drawPoints(Plotter* plotter, QPainter* painter) {
    QRect R(Margin, Margin, plotter->width() - 2 * Margin,
            plotter->height() - 2 * Margin);

    if (!R.isValid() || (_point_map.size() == 0)) return;

    painter->setClipRect(R.adjusted(+1, +1, -1, -1));
    PlotSetting const& settings = _zoom_stack.at(_cur_zoom_idx);

    QMap<int, MatNxN>::iterator iter = _point_map.begin();

    for (; iter != _point_map.end(); ++iter) {
      int const& id = iter.key();
      MatNxN const& data = iter.value();

      QPen point_pen(colors[id % 6].dark());
      point_pen.setWidth(1);
      painter->setPen(point_pen);

      for (int i = 0; i < data.rows(); ++i) {
        float dx = data(i, 0) - settings.minX();
        float dy = data(i, 1) - settings.minY();
        float x = R.left() + (dx * (R.width() - 1) / settings.spanX());
        float y = R.bottom() - (dy * (R.height() - 1) / settings.spanY());
        painter->drawEllipse(QPointF(x, y), 5, 5);
      }
    }
  }
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

void Plotter::setPlotSetting(PlotSetting const& settings) {
  _p->_zoom_stack.clear();
  _p->_zoom_stack.append(settings);
  _p->_cur_zoom_idx = 0;
  _p->_btn_zoomin->hide();
  _p->_btn_zoomout->hide();
  refreshPixmap();
}

void Plotter::setRegionData(int const& id, MatNxN const& data) {
  _p->_region_map.insert(id, data);
  refreshPixmap();
}

void Plotter::setCurveData(int const& id, MatNxN const& data,
                           int const& col_id) {
  _p->_curve_map.insert(id, data);
  _p->_id_2_colid.insert(id, col_id);
  refreshPixmap();
}

void Plotter::setPointData(int const& id, MatNxN const& data) {
  _p->_point_map.insert(id, data);
  refreshPixmap();
}

void Plotter::clearCurve(int const& id) {
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
  QRect R(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);

  if (event->button() == Qt::LeftButton) {
    if (!R.contains(event->pos())) return;
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

    QRect R = _p->_rubberband_rect.normalized();
    if ((R.width() < 4) || (R.height() < 4)) return;

    _p->applyZoom(this, &R);
  }
}

void Plotter::keyPressEvent(QKeyEvent* event) {
  PlotSetting& cur_plot_setting = _p->_zoom_stack[_p->_cur_zoom_idx];
  auto applyScroll = [&cur_plot_setting](int const& dx, int const& dy) -> void {
    cur_plot_setting.scroll(dx, dy);
  };

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
  auto applyScroll = [&cur_plot_setting](int const& dx, int const& dy) -> void {
    cur_plot_setting.scroll(dx, dy);
  };

  int num_ticks = event->delta() / 120;
  if (event->orientation() == Qt::Horizontal)
    applyScroll(num_ticks, 0);
  else
    applyScroll(0, num_ticks);

  refreshPixmap();
}
