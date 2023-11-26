#ifndef HISTOGRAMA_H
#define HISTOGRAMA_H

#include <QDialog>

namespace Ui {
class Histograma;
}

class Histograma : public QDialog
{
    Q_OBJECT

public:
    QPixmap equalize(QPixmap img,QPixmap luminancia);
    void constroi_hist(QPixmap img, Histograma* hist);
    explicit Histograma(QWidget *parent = nullptr);
    QVector<int>histograma_y(QPixmap img);
    QVector<int>histograma_cum(QVector<int> histograma_y);
    int encontra_shade_mais_prox(int shade_level, const QVector<int> &hist_src_cum, const QVector<int> &hist_target_cum);
    ~Histograma();

private:
    Ui::Histograma *ui;

};

#endif // HISTOGRAMA_H
