#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QRectF>
#include <vector>
#include <QPainterPath>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core {
    Q_OBJECT

public:
    GLWidget(QWidget *parent = nullptr);
    void addRectangle(const QRectF &rect);
    void performOperation(const QString &op);
    void deleteRectangle(int index);
    std::vector<QRectF> rectangles;
    

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    std::vector<QRectF> resultRects;

    QRectF intersectRects(const QRectF &a, const QRectF &b);
    QRectF unionRects(const QRectF &a, const QRectF &b);
    QPainterPath resultPath;
    std::vector<QRectF> subtractRects(const QRectF &a, const QRectF &b);
};
