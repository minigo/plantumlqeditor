#include "src/previewwidget.h"

#include <QDebug>
#include <QPainter>
#include <QSvgRenderer>
#include <QScrollBar>
#include <QPalette>

PreviewWidget::PreviewWidget(QWidget *parent)
    : QWidget(parent)
    , m_mode(NoMode)
    , m_zoomScale(ZOOM_ORIGINAL_SCALE)
{
}

void PreviewWidget::zoomOriginal()
{
    setZoomScale(ZOOM_ORIGINAL_SCALE);
}

void PreviewWidget::zoomIn()
{
    int new_scale = m_zoomScale + ((m_zoomScale >= ZOOM_ORIGINAL_SCALE) ? ZOOM_BIG_INCREMENT : ZOOM_SMALL_INCREMENT);
    if (new_scale > MAX_ZOOM_SCALE)
        new_scale = MAX_ZOOM_SCALE;
    setZoomScale(new_scale);
}

void PreviewWidget::zoomOut()
{
    int new_scale = m_zoomScale - ((m_zoomScale <= ZOOM_ORIGINAL_SCALE) ? ZOOM_SMALL_INCREMENT : ZOOM_BIG_INCREMENT);
    if (new_scale < MIN_ZOOM_SCALE)
        new_scale = MIN_ZOOM_SCALE;
    setZoomScale(new_scale);
}
