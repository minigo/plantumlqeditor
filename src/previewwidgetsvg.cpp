#include "src/previewwidgetsvg.h"

#include "src/previewwidgetweb.h"
#include "src/mainwindow.h"

#include <QDebug>
#include <QPainter>
#include <QSvgRenderer>
#include <QScrollBar>
#include <QPalette>

struct screenSizeT {
    float height;
    float width;
};

/**
 * Old SVG preview widget with limited SVG support (no drop shadows).
 *
 * @brief PreviewWidgetSvg::PreviewWidgetSvg
 * @param parent
 */
PreviewWidgetSvg::PreviewWidgetSvg(QWidget *parent)
    : PreviewWidget(parent)
{   
    m_svgRenderer = new QSvgRenderer(this);
}

void PreviewWidgetSvg::load(const QByteArray &data)
{
    if (m_mode == PngMode) {
        m_image.loadFromData(data);
        setMinimumSize(m_image.rect().size());
    } else if (m_mode == SvgMode) {
        m_svgRenderer->load(data);
    }
    zoomImage();
    update();
}

void PreviewWidgetSvg::setZoomScale(int zoom_scale)
{
    if (m_zoomScale != zoom_scale) {
        m_zoomScale = zoom_scale;
        zoomImage();
        update();
    }
}

int PreviewWidgetSvg::adjustZoomScale(int new_scale)
{
    // prevent/minimize scrollbars
    return ( 1 - ( (new_scale / 50) * 0.004 ) ) * new_scale;
}

//a1e
void PreviewWidgetSvg::zoomFitBest()
{
    setZoomAutoFitMode(FitBest);

    screenSizeT screenSize;
    screenSize.height = m_scrollArea->visibleRegion().boundingRect().height(),
    screenSize.width  = m_scrollArea->visibleRegion().boundingRect().width();

    zoomOriginal();

    QSize imageSize;
    if (m_mode == PngMode) {
        imageSize = m_zoomedImage.size();
    } else if (m_mode == SvgMode) {
        imageSize = m_svgRenderer->defaultSize();
        if (m_zoomScale != ZOOM_ORIGINAL_SCALE) {
            float zoom = float(m_zoomScale) / ZOOM_ORIGINAL_SCALE;
            imageSize.scale(imageSize.width() * zoom, imageSize.height() * zoom, Qt::IgnoreAspectRatio);
        }
    }

    float zW = screenSize.width / imageSize.width();
    float zH = screenSize.height / imageSize.height();

    int zoomScale = ( zW < zH ? zW : zH ) * 100;
    zoomScale = adjustZoomScale(zoomScale);
    setZoomScale(zoomScale);
}

//a1e
void PreviewWidgetSvg::zoomFitWidth()
{
    setZoomAutoFitMode(FitWidth);

    screenSizeT screenSize;
    screenSize.height = m_scrollArea->visibleRegion().boundingRect().height(),
    screenSize.width  = m_scrollArea->visibleRegion().boundingRect().width();

    zoomOriginal();

    QSize imageSize;
    if (m_mode == PngMode) {
        imageSize = m_zoomedImage.size();
    } else if (m_mode == SvgMode) {
        imageSize = m_svgRenderer->defaultSize();
        if (m_zoomScale != ZOOM_ORIGINAL_SCALE) {
            float zoom = float(m_zoomScale) / ZOOM_ORIGINAL_SCALE;
            imageSize.scale(imageSize.width() * zoom, imageSize.height() * zoom, Qt::IgnoreAspectRatio);
        }
    }

    float zW = screenSize.width / imageSize.width();

    int zoomScale = zW * 100;
    zoomScale = adjustZoomScale(zoomScale);
    setZoomScale(zoomScale);
}

//a1e
void PreviewWidgetSvg::zoomFitHeight()
{
    setZoomAutoFitMode(FitHeight);

    screenSizeT screenSize;
    screenSize.height = m_scrollArea->visibleRegion().boundingRect().height(),
    screenSize.width  = m_scrollArea->visibleRegion().boundingRect().width();

    zoomOriginal();

    QSize imageSize;
    if (m_mode == PngMode) {
        imageSize = m_zoomedImage.size();
    } else if (m_mode == SvgMode) {
        imageSize = m_svgRenderer->defaultSize();
        if (m_zoomScale != ZOOM_ORIGINAL_SCALE) {
            float zoom = float(m_zoomScale) / ZOOM_ORIGINAL_SCALE;
            imageSize.scale(imageSize.width() * zoom, imageSize.height() * zoom, Qt::IgnoreAspectRatio);
        }
    }

    float zH = screenSize.height / imageSize.height();

    int zoomScale = zH * 100;
    zoomScale = adjustZoomScale(zoomScale);
    setZoomScale(zoomScale);
}

//a1e
void PreviewWidgetSvg::wheelEvent(QWheelEvent *e)
{
  if( e->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier) ) {
    onWheelZoom(e, 8);
  }
  else if( e->modifiers() == (Qt::ControlModifier | Qt::AltModifier) ) {
    onWheelZoom(e, 1);
  }
  else if( e->modifiers() == Qt::ControlModifier) {
    onWheelZoom(e, 4);
  }
  else {
    onWheelScroll(e);
  }
}

void PreviewWidgetSvg::onWheelZoom(QWheelEvent *e, const int steps) {
   if(e->delta() > 0) {
    for(int i = steps; i>0; --i) zoomIn();
  }
  else {
    for(int i = steps; i>0; --i) zoomOut();
  }
  e->accept();
}

void PreviewWidgetSvg::onWheelScroll(QWheelEvent *e) {

  if( e->modifiers() == Qt::NoModifier) {
    // y
    int dy = e->delta() < 0 ? SCROLL_STEP : -SCROLL_STEP;
    const int cy = m_scrollArea->verticalScrollBar()->value();
    const int my = m_scrollArea->verticalScrollBar()->maximum();
    int y = cy + dy; 

    if(dy < 0 && y <  0)  { dy = cy;    y = 0; }
    if(dy > 0 && y >= my) { dy = my-cy; y = my; }

    m_scrollArea->viewport()->scroll(0, dy); 
    m_scrollArea->verticalScrollBar()->setValue(y);

    e->accept();
  }
  else if( e->modifiers() == Qt::AltModifier) { //a1e ShiftModifier -> AltModifier because same beh. as Web
    // x
    int dx = e->delta() < 0 ? SCROLL_STEP : -SCROLL_STEP;
    const int cx = m_scrollArea->horizontalScrollBar()->value();
    const int mx = m_scrollArea->horizontalScrollBar()->maximum();
    int x = cx + dx; 

    if(dx < 0 && x <  0)  { dx = cx;    x = 0; }
    if(dx > 0 && x >= mx) { dx = mx-cx; x = mx; }

    m_scrollArea->viewport()->scroll(dx, 0); 
    m_scrollArea->horizontalScrollBar()->setValue(x);

    e->accept();
  }
}

void PreviewWidgetSvg::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QSize output_size;

    // a1e: white background
    painter.fillRect(0, 0, this->width(), this->height(), Qt::white);

    if (m_mode == PngMode) {
        output_size = m_zoomedImage.size();
        QRect output_rect(QPoint(), output_size);
        output_rect.translate(rect().center() - output_rect.center());
        painter.drawImage(output_rect.topLeft(), m_zoomedImage);
    } else if (m_mode == SvgMode) {
        output_size = m_svgRenderer->defaultSize();
        if (m_zoomScale != ZOOM_ORIGINAL_SCALE) {
            float zoom = float(m_zoomScale) / ZOOM_ORIGINAL_SCALE;
            output_size.scale(output_size.width() * zoom, output_size.height() * zoom, Qt::IgnoreAspectRatio);
        }
        QRect output_rect(QPoint(), output_size);
        output_rect.translate(rect().center() - output_rect.center());
        m_svgRenderer->render(&painter, output_rect);        
    }
    if (output_size.width() == -1 || output_size.height() == -1) {
        qDebug() << "switch preview mode!?";
    } else {
        setMinimumSize(output_size);
    }
}

void PreviewWidgetSvg::zoomImage()
{
    if (m_mode == PngMode) {
        if (m_zoomScale == ZOOM_ORIGINAL_SCALE) {
            m_zoomedImage = m_image;
        } else {
            float zoom = float(m_zoomScale) / ZOOM_ORIGINAL_SCALE;
            m_zoomedImage = m_image.scaled(m_image.width() * zoom, m_image.height() * zoom,
                                           Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        }
    }
}

// a1e
void PreviewWidgetSvg::setScrollArea (QScrollArea *scrollArea)
{
  m_scrollArea = scrollArea;
}
