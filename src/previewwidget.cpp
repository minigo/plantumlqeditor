#include "src/previewwidget.h"

#include <QDebug>
#include <QPainter>
#include <QSvgRenderer>
#include <QScrollBar>
#include <QPalette>

PreviewWidget::PreviewWidget(QWidget *parent)
    : QWidget (parent)
{}

void PreviewWidget::zoomOriginal ()
{
    setZoomAutoFitMode (FitOff);
    setZoomScale (ZOOM_ORIGINAL_SCALE);
}

void PreviewWidget::zoomIn()
{
    setZoomAutoFitMode(FitOff);

    int new_scale = m_zoomScale + ((m_zoomScale >= ZOOM_ORIGINAL_SCALE) ? ZOOM_BIG_INCREMENT : ZOOM_SMALL_INCREMENT);
    if (new_scale > MAX_ZOOM_SCALE)
        new_scale = MAX_ZOOM_SCALE;
    setZoomScale(new_scale);
}

void PreviewWidget::zoomOut()
{
    setZoomAutoFitMode(FitOff);

    int new_scale = m_zoomScale - ((m_zoomScale <= ZOOM_ORIGINAL_SCALE) ? ZOOM_SMALL_INCREMENT : ZOOM_BIG_INCREMENT);
    if (new_scale < MIN_ZOOM_SCALE)
        new_scale = MIN_ZOOM_SCALE;
    setZoomScale(new_scale);
}

void PreviewWidget::setZoomAutoFitMode(AutoFitMode mode)
{
    m_zoomAutoFitMode = mode;

    if (m_zoomAutoFitMode == FitOff)
        m_zoomAutoFit = false;
}

PreviewWidget::AutoFitMode PreviewWidget::getZoomAutoFitMode () const
{
    if (m_zoomAutoFit)
        return m_zoomAutoFitMode;
    else
        return FitOff;
}

//void PreviewWidget::getScrollAreaPos () const
//{
//    if (!m_scrollArea)
//        return;
//    qDebug () << "vertical slider position:" << m_scrollArea->verticalScrollBar ()->sliderPosition ();
//    qDebug () << "horizontal slider position:" << m_scrollArea->horizontalScrollBar ()->sliderPosition ();
//}

void PreviewWidget::zoomAutoFit(bool state)
{
    m_zoomAutoFit = state;

    if(m_zoomAutoFit == true && m_zoomAutoFitMode == FitOff) {
        m_zoomAutoFitMode = FitBest;
    }
}
