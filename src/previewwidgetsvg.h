#ifndef PREVIEWWIDGETSVG_H
#define PREVIEWWIDGETSVG_H

#include <src/previewwidget.h>

#include <QObject>
#include <QWidget>
#include <QImage>
#include <QWheelEvent>
#include <QScrollArea>

class QSvgRenderer;

class PreviewWidgetSvg : public PreviewWidget
{
    Q_OBJECT
public:

    explicit PreviewWidgetSvg(QWidget *parent = 0);

    void load(const QByteArray &data);
    void setScrollArea(QScrollArea *scrollArea);

public slots:
    void zoomFitBest();
    void zoomFitWidth();
    void zoomFitHeight();

protected:
    void wheelEvent(QWheelEvent *e); 

private:
    static const int ZOOM_ORIGINAL_SCALE = 100;

    void paintEvent(QPaintEvent *);
    void zoomImage();
    void setZoomScale(int new_scale);
    int adjustZoomScale(int new_scale);

    void onWheelZoom(QWheelEvent *e, const int steps);
    void onWheelScroll(QWheelEvent *e);

    QImage m_image;
    QImage m_zoomedImage;
    QSvgRenderer *m_svgRenderer;
};

#endif // PREVIEWWIDGETSVG_H
