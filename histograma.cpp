#include "histograma.h"
#include "qboxlayout.h"
#include "ui_histograma.h"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
Histograma::Histograma(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Histograma)
{
    ui->setupUi(this);
}

Histograma::~Histograma()
{
    delete ui;
}

void Histograma::constroi_hist(QPixmap img, Histograma* hist){
    QVector<int> histograma_y = Histograma::histograma_y(img);
    QVector<QString>  histograma_x(256,"");
    float soma_total = img.height()*img.width();
    for(int i = 0; i<histograma_y.size(); i++){
        histograma_x[i] =QString::number(i+1);
    }


    // Crie uma série de barras e adicione os conjuntos de dados

    QBarSeries *series = new QBarSeries();
    QBarSet *barSet = new QBarSet("% de pixeis");

    for (int i = 0; i < histograma_x.size(); ++i) {
        *barSet <<100.0* float(histograma_y[i])/soma_total;
    }

    series->append(barSet);

    // Crie um gráfico de barras e adicione a série
    QChart *chart = new QChart();
    chart->addSeries(series);

    // Defina as categorias no eixo x
    QStringList categories;
    for (int i = 0; i < histograma_x.size(); ++i) {
        categories << histograma_x[i];
    }

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->setLabelsAngle(90);
    QFont labelsFont = axisX->labelsFont();
    labelsFont.setPixelSize(12);
    axisX->setLabelsFont(labelsFont);
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Defina o intervalo no eixo y
    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Crie uma exibição do gráfico
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(chartView);
    hist->setLayout(layout);
    hist->resize(1000,500);


}

QVector<int> Histograma::histograma_y(QPixmap img){
    QVector<int> histograma_y(256,0);
    QImage imagem = img.toImage();
    QColor cor;

    for(int x = 0; x <imagem.width();x++){
        for(int y = 0; y <imagem.height();y++){
            cor = imagem.pixelColor(x,y);
            histograma_y[cor.red()]++;
        }
    }
    return histograma_y;
}

QPixmap Histograma::equalize(QPixmap img,QPixmap luminancia){
    QImage image= img.toImage();
    QVector<int> histograma_y = Histograma::histograma_y(luminancia);
    QVector<int> cum_hist = Histograma::histograma_cum(histograma_y);
    float valor;
    QColor cor;


    for(int x = 0; x < image.width();x++){
        for(int y = 0; y < image.height(); y++){
            cor = image.pixelColor(x,y);
            cor = QColor(cum_hist[cor.red()],cum_hist[cor.green()],cum_hist[cor.blue()]);
            image.setPixelColor(x,y,cor);

        }
    }
    qDebug() << cum_hist;
    return QPixmap::fromImage(image);
}

QVector<int> Histograma::histograma_cum(QVector<int> histograma_y){
    QVector<int> cum_hist(256,0);
    float soma_total = 0;
    for (int i = 0;  i < 256; i++) {
        soma_total = soma_total + histograma_y[i];
    }
    cum_hist[0] = histograma_y[0];

    for(int i = 1; i < 256; i++){

        cum_hist[i] = cum_hist[i-1] + histograma_y[i];

    }

    for(int i = 0; i <256; i++){
        cum_hist[i] = (cum_hist[i]*255)/soma_total;
    }


    return cum_hist;
}
int Histograma::encontra_shade_mais_prox(int shade_level, const QVector<int> &hist_src_cum, const QVector<int> &hist_target_cum){
    int closest_shade = 0;
    int min_difference = std::numeric_limits<int>::max();
    for (int i = 0; i < 256; i++)
    {
        int difference = qAbs(hist_src_cum[shade_level] - hist_target_cum[i]);

        if (difference <= min_difference)
        {
            min_difference = difference;
            closest_shade = i;
        }
    }

    return closest_shade;
}
