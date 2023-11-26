#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "janelaimg.h"
#include "histograma.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QPixmap transforma_cinza(QPixmap img);


private slots:
    void on_quantizar_clicked();
    void on_saida_clicked();

    void on_inserir_imagem_clicked();

    void on_flip_vertical_clicked();

    void on_flip_horizontal_clicked();

    void on_tons_de_cinza_clicked();

    void on_salvar_clicked();



    void on_geraHistograma_clicked();

    void on_brilho_clicked();

    void on_contraste_botao_clicked();

    void on_negativo_botao_clicked();

    void on_equalize_botao_clicked();


    void on_Z_out_clicked();

    void on_Z_in_clicked();

    void on_rotate_dir_clicked();

    void on_rotate_esq_clicked();

    void on_conv_gaus_clicked();

    void on_conv_lapl_clicked();

    void on_conv_passa_altas_clicked();

    void on_conv_Px_clicked();

    void on_conv_Py_clicked();

    void on_conv_sx_clicked();

    void on_conv_sy_clicked();

    void on_conv_personal_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    JanelaImg *imagem2;
    Histograma *histograma;
    void conv2(QVector<QVector<double>> kernel,bool soma_127,bool precisa_cinza);


};
#endif // MAINWINDOW_H
