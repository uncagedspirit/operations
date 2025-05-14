#include "mainwindow.h"
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();
}

void MainWindow::setupUI() {
    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);

    listWidget = new QListWidget;
    glWidget = new GLWidget;

    QVBoxLayout *sideLayout = new QVBoxLayout;
    addButton = new QPushButton("Add Rectangle");
    unionButton = new QPushButton("Union");
    intersectionButton = new QPushButton("Intersection");
    subtractButton = new QPushButton("Subtraction");
    subtractBAButton = new QPushButton("Subtraction (B - A)");
    deleteButton = new QPushButton("Delete Rectangle"); 

    sideLayout->addWidget(addButton);
    sideLayout->addWidget(unionButton);
    sideLayout->addWidget(intersectionButton);
    sideLayout->addWidget(subtractButton);
    sideLayout->addWidget(subtractBAButton);
    sideLayout->addWidget(deleteButton);
    sideLayout->addWidget(listWidget);
    sideLayout->addStretch();

    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddRectangle);
    connect(unionButton, &QPushButton::clicked, this, &MainWindow::onUnion);
    connect(intersectionButton, &QPushButton::clicked, this, &MainWindow::onIntersection);
    connect(subtractButton, &QPushButton::clicked, this, &MainWindow::onSubtraction);
    connect(subtractBAButton, &QPushButton::clicked, this, &MainWindow::onSubtractionBA);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteRectangle); 

    mainLayout->addLayout(sideLayout);
    mainLayout->addWidget(glWidget, 1);
    setCentralWidget(central);
}

void MainWindow::onDeleteRectangle() {
    QListWidgetItem *selectedItem = listWidget->currentItem();
    if (!selectedItem) {
        return; 
    }

    int index = listWidget->row(selectedItem);
    glWidget->deleteRectangle(index);
    delete listWidget->takeItem(index); 
}

void MainWindow::onAddRectangle() {
    bool ok;
    float x = QInputDialog::getDouble(this, "Input", "Lower-left X:", 0, -1000, 1000, 2, &ok);
    if (!ok) return;
    float y = QInputDialog::getDouble(this, "Input", "Lower-left Y:", 0, -1000, 1000, 2, &ok);
    if (!ok) return;
    float w = QInputDialog::getDouble(this, "Input", "Width:", 1, 0, 1000, 2, &ok);
    if (!ok) return;
    float h = QInputDialog::getDouble(this, "Input", "Height:", 1, 0, 1000, 2, &ok);
    if (!ok) return;

    QRectF rect(x, y, w, h);
    glWidget->addRectangle(rect);
    listWidget->addItem(QString("Rect (%1, %2, %3, %4)").arg(x).arg(y).arg(w).arg(h));
}

void MainWindow::onUnion() {
    glWidget->performOperation("union");
}

void MainWindow::onIntersection() {
    glWidget->performOperation("intersection");
}

void MainWindow::onSubtraction() {
    glWidget->performOperation("subtraction");
}

void MainWindow::onSubtractionBA() {
    glWidget->performOperation("subtractionBA"); 
}
