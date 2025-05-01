#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "smartptr.h"

QT_BEGIN_NAMESPACE

namespace Ui
{
class MainWindow;
}

QT_END_NAMESPACE

enum class DrawTools
{
    DRAW_TOOL,
    LINE,
    ELLIPSE,
    RECTANGLE
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;
private:
    SmartPtr<Ui::MainWindow> ui;
    QPixmap sheet;
    QPoint currentPosition, lastPosition;
    DrawTools chosenTool;
    QColor chosenColor;
    int thickness;
    bool isClicked;
    bool isChanged;

    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);
    void createFileMenuBar();
private slots:
    void createFile();
    void openFile();
    void saveFile();
    void setDrawTool();
    void setLine();
    void setEllipse();
    void setRectangle();
    void setColor();
    void setThickness(int thicknessValue);
};

#endif
