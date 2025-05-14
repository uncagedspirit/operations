#pragma once

#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "glwidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onAddRectangle();
    void onUnion();
    void onIntersection();
    void onSubtraction();
    void onSubtractionBA(); 
    void onDeleteRectangle();

private:
    QListWidget *listWidget;
    GLWidget *glWidget;

    QPushButton *addButton;
    QPushButton *unionButton;
    QPushButton *intersectionButton;
    QPushButton *subtractButton;
    QPushButton *subtractBAButton;
    QPushButton *deleteButton;

    void setupUI();
};
