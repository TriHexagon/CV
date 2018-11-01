#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <qmath.h>


using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    m_ui->pushButton_2->setEnabled(false);

    m_ui->verticalSlider->setRange(0, 255);
    m_threshold = 127;
    m_ui->verticalSlider->setValue(m_threshold);
    m_blockSize = 3;
    m_ui->verticalSliderBlockSize->setValue(3);
    m_ui->timeLabel->setText("0 ms");
    m_gaussian = false;
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.bmp)"));
    m_image = imread(fileName.toUtf8().data());
    if(m_image.data) {
        m_ui->pushButton_2->setEnabled(true);

        if( m_image.channels() == 1 && m_image.type() == CV_8U) {
            m_grayImage = m_image.clone();
        }
        if( m_image.channels() == 3 && m_image.type() == CV_8UC3) {
            // Graustufenbild fuer Verarbeitung anlegen
            cv::cvtColor(m_image, m_grayImage, CV_RGB2GRAY);
        }
        if( m_image.channels() == 4 && m_image.type() == CV_8UC4) {
            // Graustufenbild fuer Verarbeitung anlegen
            cv::cvtColor(m_image, m_grayImage, CV_RGBA2GRAY);
        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    double duration;
    duration = static_cast<double> (cv::getTickCount());

    m_thImage = cv::Mat(m_grayImage.rows, m_grayImage.cols, CV_8U, cv::Scalar(127));

    if(m_ui->useMyTH->isChecked()) {
       //myTH();
        int adaptiveMethod = (m_gaussian ? ADAPTIVE_THRESH_GAUSSIAN_C : ADAPTIVE_THRESH_MEAN_C);
        cv::adaptiveThreshold(m_grayImage, m_thImage, 255, adaptiveMethod, THRESH_BINARY, m_blockSize, 0.0);
    }
    else {
        cv::threshold(m_grayImage, m_thImage, m_threshold, 255, CV_THRESH_BINARY);
    }

    duration = static_cast<double>(cv::getTickCount())-duration;
    duration /= cv::getTickFrequency(); // die verstrichene Zeit in s
    m_ui->timeLabel->setText(QString::number(duration*1000.0, 'f', 2) + " ms");
    imshow("Threshold Image", m_thImage);
}


void MainWindow::myTH() {

    int nl = m_grayImage.rows; // number of lines

    // total number of elements per line
    int nc = m_grayImage.cols;

    if(m_grayImage.isContinuous()) {
        nc = nc*nl;
        nl = 1;
    }

    for (int j=0; j<nl; j++) {
        // get the address of row j
        uchar* data = m_grayImage.ptr<uchar> (j);
        uchar* res  = m_thImage.ptr<uchar> (j);
        for (int i=0; i< nc; i++) {
            if(data[i]>m_threshold)
                res[i] = 255;
            else
                res[i] = 0;
        }
    }
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    m_threshold = value;
    m_ui->label_2->setText(QString::number(m_threshold));
}

void MainWindow::on_verticalSliderBlockSize_valueChanged(int value)
{
    m_blockSize = value;
    m_ui->labelBlockSize->setText(QString::number(value));
}

void MainWindow::on_checkBoxGausssian_toggled(bool checked)
{
    m_gaussian = checked;
}
