#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_slider_valueChanged(int value);

    void on_loadButton_clicked();

    void on_processButton_clicked();

private:
    Ui::MainWindow *ui;

    double factor;
    cv::Mat inImage;
};

#endif // MAINWINDOW_H
