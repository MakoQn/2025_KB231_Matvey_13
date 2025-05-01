#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow),
    sheet(1600, 900), chosenTool(DrawTools::DRAW_TOOL),
    chosenColor(Qt::black), thickness(1),
    isClicked(false), isChanged(false)
{
    ui->setupUi(this);

    createFileMenuBar();
    connect(ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::setThickness);
    sheet.fill(Qt::white);

    connect(ui->DrawTool, &QPushButton::clicked, this, &MainWindow::setDrawTool);
    connect(ui->Line, &QPushButton::clicked, this, &MainWindow::setLine);
    connect(ui->Ellipse, &QPushButton::clicked, this, &MainWindow::setEllipse);
    connect(ui->Rectangle, &QPushButton::clicked, this, &MainWindow::setRectangle);
    connect(ui->Color, &QPushButton::clicked, this, &MainWindow::setColor);

    this->setWindowTitle("Рисовашкес");
    this->resize(1600, 900);
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (isClicked && chosenTool == DrawTools::DRAW_TOOL)
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

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    isClicked = false;

    if (chosenTool != DrawTools::DRAW_TOOL)
    {
        QPainter painter(&sheet);
        painter.setPen(QPen(chosenColor, thickness));

        switch ((int)chosenTool)
        {
        case (int)DrawTools::LINE:
        {
            painter.drawLine(lastPosition, event->pos());
            break;
        }
        case (int)DrawTools::ELLIPSE:
        {
            painter.drawEllipse(QRect(lastPosition, event->pos()));
            break;
        }
        case (int)DrawTools::RECTANGLE:
        {
            painter.drawRect(QRect(lastPosition, event->pos()));
            break;
        }
        default:
        {
            break;
        }
        }

        isChanged = true;
        update();
    }
}

void MainWindow::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, sheet);

    if (isClicked && chosenTool != DrawTools::DRAW_TOOL)
    {
        painter.setPen(QPen(chosenColor, thickness, Qt::DashLine));

        switch ((int)chosenTool)
        {
        case (int)DrawTools::LINE:
        {
            painter.drawLine(lastPosition, currentPosition);
            break;
        }
        case (int)DrawTools::ELLIPSE:
        {
            painter.drawEllipse(QRect(lastPosition, currentPosition));
            break;
        }
        case (int)DrawTools::RECTANGLE:
        {
            painter.drawRect(QRect(lastPosition, currentPosition));
            break;
        }
        default:
        {
            break;
        }
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
    chosenTool = DrawTools::DRAW_TOOL;
}

void MainWindow::setLine()
{
    chosenTool = DrawTools::LINE;
}

void MainWindow::setEllipse()
{
    chosenTool = DrawTools::ELLIPSE;
}

void MainWindow::setRectangle()
{
    chosenTool = DrawTools::RECTANGLE;
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
