#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QObject>
#include <QWidget>
#include <QImage>
#include <QWheelEvent>
#include <QScrollArea>

class QSvgRenderer;

class PreviewWidget : public QWidget
{
    Q_OBJECT
public:
    enum Mode { NoMode, PngMode, SvgMode };
    enum AutoFitMode { FitOff, FitBest, FitWidth, FitHeight };

    explicit PreviewWidget(QWidget *parent = 0);

    Mode mode() const { return m_mode; }
    void setMode(Mode new_mode) { m_mode = new_mode; }

    virtual void load(const QByteArray &data) = 0;
    virtual void setScrollArea(QScrollArea *scrollArea) = 0;

    void setZoomAutoFitMode(AutoFitMode mode);
    AutoFitMode getZoomAutoFitMode();

public slots:
    virtual void zoomOriginal();
    virtual void zoomIn();
    virtual void zoomOut();
    virtual void zoomFitBest() = 0;
    virtual void zoomFitWidth() = 0;
    virtual void zoomFitHeight() = 0;
    virtual void zoomAutoFit(bool state);
    virtual void wheelEvent(QWheelEvent *e) = 0;

protected:    

    Mode m_mode;
    int m_zoomScale;
    QScrollArea *m_scrollArea;

    static const int ZOOM_ORIGINAL_SCALE  = 100;
    static const int ZOOM_BIG_INCREMENT   = 10;  // used when m_zoomScale > ZOOM_ORIGINAL_SCALE
    static const int ZOOM_SMALL_INCREMENT = 10;  // used when m_zoomScale < ZOOM_ORIGINAL_SCALE
    static const int MIN_ZOOM_SCALE       = 25;
    static const int MAX_ZOOM_SCALE       = 900;
    static const int SCROLL_STEP          = 50;

private:

    virtual void paintEvent(QPaintEvent *) = 0;
    virtual void zoomImage() = 0;
    virtual void setZoomScale(int new_scale) = 0;

    virtual void onWheelZoom(QWheelEvent *e, const int steps) = 0;
    virtual void onWheelScroll(QWheelEvent *e) = 0;

    bool m_zoomAutoFit;
    AutoFitMode m_zoomAutoFitMode;
};

#endif // PREVIEWWIDGET_H
