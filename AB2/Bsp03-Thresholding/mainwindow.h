#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

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
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_verticalSlider_valueChanged(int value);

    void on_verticalSliderBlockSize_valueChanged(int value);

    void on_checkBoxGausssian_toggled(bool checked);

private:
    Ui::MainWindow* m_ui;
    cv::Mat m_image, m_grayImage, m_thImage;
    uchar m_threshold;
    int m_blockSize;
    bool m_gaussian;

    void myTH();

};

#endif // MAINWINDOW_H
