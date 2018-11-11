#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDebug>
#include <cmath>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

static float vSobelArray[] = {
    1.0f, 0.0f, -1.0f,
    2.0f, 0.0f, -2.0f,
    1.0f, 0.0f, -1.0f,
};

static float hSobelArray[] = {
    1.0f,  2.0f,  -1.0f,
    0.0f,  0.0f,  0.0f,
    -1.0f, -2.0f, -1.0f,
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.bmp)"));

    cv::Mat image = cv::imread(fileName.toUtf8().data(), CV_LOAD_IMAGE_GRAYSCALE);

    if (image.data)
    {
        inImage = image;
        ui->nameLabel->setText(fileName);
    }
}

void MainWindow::on_processButton_clicked()
{
    if (inImage.data)
    {
        //Arbeite mit einer Kopie
        cv::Mat outImage = inImage.clone();

        bool vertical = ui->vCheckBox->isChecked();
        bool horizontal = ui->hCheckBox->isChecked();

        //Bilde Sobelmasken
        cv::Mat vSobelKernel, hSobelKernel;
        float divisor = ui->scaleSpinBox->value();
        float delta = ui->transformSpinBox->value();
        if (vertical)
        {
            //Vertikal
            vSobelKernel = cv::Mat(3, 3, CV_32F, vSobelArray).clone();
            vSobelKernel /= divisor;
        }

        if (horizontal)
        {
            hSobelKernel = cv::Mat(3, 3, CV_32F, hSobelArray).clone();
            hSobelKernel /= divisor;
        }

        //Führe Faltung aus
        cv::Mat sobelVOut;
        cv::Mat sobelHOut;
        if (vertical)
        {
            cv::filter2D(inImage, sobelVOut, CV_8U, vSobelKernel, cv::Point(-1, -1), delta);
        }

        if (horizontal)
        {
            cv::filter2D(inImage, sobelHOut, CV_8U, hSobelKernel, cv::Point(-1, -1), delta);
        }

        //Magnituden Berechnung und Auswahl der Ausgabe
        if (vertical && horizontal)
        {
            //Berechne Magnitude
            for (int x = 0; x < outImage.cols; x++)
            {
                for (int y = 0; y < outImage.rows; y++)
                {
                    unsigned vValue = sobelVOut.at<uchar>(x, y);
                    unsigned hValue = sobelHOut.at<uchar>(x, y);

                    unsigned magnitude = std::sqrt( vValue * vValue + hValue * hValue );

                    outImage.at<uchar>(x, y) = magnitude;
                }
            }
        }
        else if (vertical)
        {
            outImage = sobelVOut;
        }
        else if (horizontal)
        {
            outImage = sobelHOut;
        }

        //Ausgabebild in Label darstellen
        QImage qimage = QImage(outImage.data, outImage.cols, outImage.rows, outImage.step, QImage::Format_Grayscale8);
        QPixmap pixmap = QPixmap::fromImage(qimage);

        ui->imageViewLabel->setPixmap(pixmap);
    }
}
