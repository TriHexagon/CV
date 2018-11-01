#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

double kernelBinomial3x3[] = {
    1.0, 2.0, 1.0,
    2.0, 4.0, 2.0,
    1.0, 2.0, 1.0
};

/*
übernommen aus Python Numpy berechnung:
>>> import numpy as np
>>> a = np.matrix('1 4 6 4 1')
>>> print(np.transpose(a) * a)
[[ 1  4  6  4  1]
 [ 4 16 24 16  4]
 [ 6 24 36 24  6]
 [ 4 16 24 16  4]
 [ 1  4  6  4  1]]

alternativ über cv::Mat berechnen
*/
double kernelBinomial5x5[] = {
    1.0, 4.0,  6.0,  4.0,  1.0,
    4.0, 16.0, 24.0, 16.0, 4.0,
    6.0, 24.0, 36.0, 24.0, 6.0,
    4.0, 16.0, 24.0, 16.0, 4.0,
    1.0, 4.0,  6.0,  4.0,  1.0,
};
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    factor = ui->slider->value() * 0.01;
    ui->valueLabel->setText(QString::number(factor));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_slider_valueChanged(int value)
{
    factor = value * 0.01;
    ui->valueLabel->setText(QString::number(factor));
}

void MainWindow::on_loadButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.bmp)"));

    inImage = cv::imread(fileName.toUtf8().data(), CV_LOAD_IMAGE_GRAYSCALE);
}

void MainWindow::on_processButton_clicked()
{
    if (inImage.data)
    {
        int blockSize;
        if (ui->size3RadioButton->isChecked())
            blockSize = 3;
        else
            blockSize = 5;

        //Erstelle Ausgabematrix
        cv::Mat outImage = inImage.clone(); //Ausgabematrix mit den gleichen Eigenschaften wie Eingangsmatrix wird benötigt

        //Lege Filterkern an
        cv::Mat kernelMatrix;
        if (ui->binomialRadioButton->isChecked())
        {
            //binomial
            double* kernelValues;
            if (blockSize == 3)
                kernelValues = kernelBinomial3x3;
            else
                kernelValues = kernelBinomial5x5;

            kernelMatrix = cv::Mat(blockSize, blockSize, CV_64F, kernelValues).clone(); //Matrix kopieren, da die init Arrays sonst bei Multiplikation modifiziert wird
            kernelMatrix *= factor;
        }
        else
        {
            //mean
            kernelMatrix = cv::Mat(blockSize, blockSize, CV_64F, factor);
        }

        //Wende Binomialfilter an
        cv::filter2D(inImage, outImage, -1, kernelMatrix);

        //Zeige Bild an
        //cv::namedWindow("Output", cv::WINDOW_NORMAL);
        cv::imshow("Output", outImage);
    }
}
