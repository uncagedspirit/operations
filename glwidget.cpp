#include "glwidget.h"
#include <QPainter>

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(1.0, 1.0, 1.0, 1.0);
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.translate(width() / 2.0, height() / 2.0);
    painter.scale(1, -1);
    const double scale = 10.0;

    painter.setPen(Qt::black);

    if (resultPath.isEmpty()) {
        for (const QRectF &r : rectangles) {
            QRectF scaled(r.left() * scale, r.top() * scale,
                          r.width() * scale, r.height() * scale);
            painter.drawRect(scaled);
        }
    }

    if (!resultPath.isEmpty()) {
        QPainterPath scaledPath;
        QTransform t;
        t.scale(scale, scale);
        t.translate(0, 0);
        scaledPath = t.map(resultPath);
        painter.drawPath(scaledPath);
    }

    painter.setPen(Qt::red);
    for (const QRectF &r : resultRects) {
        QRectF scaled(r.left() * scale, r.top() * scale,
                      r.width() * scale, r.height() * scale);
        painter.drawRect(scaled);
    }
}

void GLWidget::addRectangle(const QRectF &rect) {
    rectangles.push_back(rect);
    resultRects.clear();
    update();
}

void GLWidget::deleteRectangle(int index) {
    if (index < 0 || index >= rectangles.size()) return;
    rectangles.erase(rectangles.begin() + index);
    resultRects.clear();
    update();
}

void GLWidget::performOperation(const QString &op) {
    if (rectangles.size() < 2) return;

    QRectF a = rectangles[rectangles.size() - 2];
    QRectF b = rectangles[rectangles.size() - 1];
    resultRects.clear();

    if (op == "union") {
        QPainterPath path1, path2;
        path1.addRect(a);
        path2.addRect(b);
        resultPath = path1.united(path2);
    } else if (op == "intersection") {
        QRectF r = intersectRects(a, b);
        if (!r.isEmpty()) resultRects.push_back(r);
    } else if (op == "subtraction") {
        resultRects = subtractRects(a, b); 
    } else if (op == "subtractionBA") {
        resultRects = subtractRects(b, a); 
    }

    update();
}

QRectF GLWidget::intersectRects(const QRectF &a, const QRectF &b) {
    return a.intersected(b);
}

QRectF GLWidget::unionRects(const QRectF &a, const QRectF &b) {
    return a.united(b);
}

std::vector<QRectF> GLWidget::subtractRects(const QRectF &a, const QRectF &b) {
    QRectF intersection = a.intersected(b);
    std::vector<QRectF> result;


    if (intersection.isEmpty()) {
        result.push_back(a);
        return result;
    }

    if (intersection.left() > a.left()) {
        result.push_back(QRectF(a.left(), a.top(), intersection.left() - a.left(), a.height())); 
    }
    if (intersection.right() < a.right()) {
        result.push_back(QRectF(intersection.right(), a.top(), a.right() - intersection.right(), a.height())); 
    }
    if (intersection.top() > a.top()) {
        result.push_back(QRectF(intersection.left(), a.top(), intersection.width(), intersection.top() - a.top())); 
    }
    if (intersection.bottom() < a.bottom()) {
        result.push_back(QRectF(intersection.left(), intersection.bottom(), intersection.width(), a.bottom() - intersection.bottom())); 
    }
    return result;
}

