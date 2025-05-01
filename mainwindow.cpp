#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow),
    sheet(1600, 900), chosenTool(DrawTools::drawTool),
    chosenColor(Qt::black), thickness(1),
    isClicked(false), isChanged(false)
{
    ui->setupUi(this);

    createFileMenuBar();
    connect(ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::setThickness);
    sheet.fill(Qt::white);
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (isClicked && chosenTool == DrawTools::drawTool)
    {
        QPainter painter(&sheet);
        painter.setPen(QPen(chosenColor, thickness));
        painter.drawLine(lastPosition, event->pos());
        lastPosition = event->pos();
        update();
        isChanged = true;
    }
    else
    {
        currentPosition = event->pos();
        update();
    }
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    isClicked = true;
    lastPosition = event->pos();
    currentPosition = lastPosition;
}

void MainWindow::mouseUnpressEvent(QMouseEvent* event)
{
    isClicked = false;

    if (chosenTool != DrawTools::drawTool)
    {
        QPainter painter(&sheet);
        painter.setPen(QPen(chosenColor, thickness));

        switch ((int)chosenTool)
        {
        case (int)DrawTools::Line:
        {
            painter.drawLine(lastPosition, event->pos());
        }
            break;
        case (int)DrawTools::Ellipse:
        {
            painter.drawEllipse(QRect(lastPosition, event->pos()));
        }
            break;
        case (int)DrawTools::Rectangle:
        {
            painter.drawRect(QRect(lastPosition, event->pos()));
        }
            break;
        default:
            break;
        }

        isChanged = true;
        update();
    }
}

void MainWindow::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, sheet);

    if (isClicked && chosenTool != DrawTools::drawTool)
    {
        painter.setPen(QPen(chosenColor, thickness, Qt::DashLine));

        switch ((int)chosenTool)
        {
        case (int)DrawTools::Line:
        {
            painter.drawLine(lastPosition, currentPosition);
        }
            break;
        case (int)DrawTools::Ellipse:
        {
            painter.drawEllipse(QRect(lastPosition, currentPosition));
        }
            break;
        case (int)DrawTools::Rectangle:
        {
            painter.drawRect(QRect(lastPosition, currentPosition));
        }
            break;
        default:
            break;
        }
    }
}

void MainWindow::createFileMenuBar()
{
    QMenu* fileMenu = menuBar()->addMenu("Файл");

    QAction* createFileAction = fileMenu->addAction("Создать файл");
    connect(createFileAction, &QAction::triggered, this, &MainWindow::createFile);
    QAction* openFile = fileMenu->addAction("Открыть файл");
    connect(openFile, &QAction::triggered, this, &MainWindow::openFile);
    QAction* saveFile = fileMenu->addAction("Сохранить файл");
    connect(saveFile, &QAction::triggered, this, &MainWindow::saveFile);
}

void MainWindow::createFile()
{
    if (isChanged)
    {
        QMessageBox::StandardButton answer = QMessageBox::question(
            this,
            "Сохранение",
            "Сохранить изменения в текущем файле?",
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
            );

        if(answer == QMessageBox::Cancel)
        {
            return;
        }

        if(answer == QMessageBox::Yes)
        {
            saveFile();
        }
    }

    sheet = QPixmap(1600, 900);
    sheet.fill(Qt::white);
    isChanged = false;
    update();
}

void MainWindow::openFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Открыть файл", "", "Images(*.jpg *.jpeg *.png *.bmp)");

    if (!filePath.isEmpty())
    {
        if (sheet.load(filePath))
        {
            update();
            isChanged = false;
        }
        else
        {
            QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл");
        }
    }
}

void MainWindow::saveFile()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Сохранить файл", "", "PNG(*.png);;JPEG(*.jpg *.jpeg);;BMP(*.bmp)");

    if (!filePath.isEmpty())
    {
        if (sheet.save(filePath))
        {
            isChanged = false;
        }
        else
        {
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить файл");
        }
    }
}

void MainWindow::setDrawTool()
{
    chosenTool = DrawTools::drawTool;
}

void MainWindow::setLine()
{
    chosenTool = DrawTools::Line;
}

void MainWindow::setEllipse()
{
    chosenTool = DrawTools::Ellipse;
}

void MainWindow::setRectangle()
{
    chosenTool = DrawTools::Rectangle;
}

void MainWindow::setColor()
{
    QColor color = QColorDialog::getColor(chosenColor, this);

    if (color.isValid())
    {
        chosenColor = color;
    }
}

void MainWindow::setThickness(int thicknessValue)
{
    thickness = thicknessValue;
}
