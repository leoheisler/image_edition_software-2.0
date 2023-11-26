#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <qmessagebox.h>
#include <QPixmap>
#include <QFileDialog>
#include <QColor>
#include <QVector>
bool first = true;
bool cinza_clicado = false;
int num_janela_hist = 1;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_inserir_imagem_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Selecione uma imagem", "", "Imagens (*.png *.jpg *.jpeg *.bmp *.gif *.tif)");
    if (!filePath.isEmpty()) {
        QPixmap imagem(filePath);
        ui->Photo_Original->setPixmap(imagem);

        imagem2 = new JanelaImg(this);
        imagem2->setParent(NULL);
        imagem2->show();

        imagem2->change_pixmap(imagem);

        imagem2->resize(imagem.width() + 20,imagem.height() + 20);

    }
    cinza_clicado = false;
}

void MainWindow::on_salvar_clicked()
{
    QImage imagem = imagem2->retorna_pixmap().toImage();
    QString caminhoDoArquivo = QFileDialog::getSaveFileName(nullptr, "Salvar imagem", "", "Imagens (*.png *.jpg *.bmp);;Todos os arquivos (*.*)");
    imagem.save(caminhoDoArquivo);
}

void MainWindow::on_saida_clicked()
{
    QMessageBox::StandardButton resposta = QMessageBox::question(this,"sair","Deseja mesmo Sair?",QMessageBox::Yes | QMessageBox::No);
    if(resposta == QMessageBox::Yes){
        close();
        imagem2->close();
    }
}

void MainWindow::on_flip_vertical_clicked()
{
    //pega a imagem original e reserva um espaço na memória para ela
    QImage alterar = imagem2->retorna_pixmap().toImage();
    QImage virada = alterar;

    //pega a altura e largura do pixmap da imagem original
    int largura = alterar.width();
    int altura = alterar.height();

    //percorre o pixmap da imagem original e substitui na alterada pelo valor do pixel encontrado na coordenada "oposta"
    for(int y =0; y < altura; y++){
        for(int x = 0; x<largura;x++){
            virada.setPixelColor(x,altura - y - 1,alterar.pixelColor(x,y));

        }
    }

    //da o displey da imagem alterada
    QPixmap alterada = QPixmap::fromImage(virada);
    imagem2->change_pixmap(alterada);
}


void MainWindow::on_flip_horizontal_clicked()
{
    //pega a imagem original e reserva um espaço na memória para ela
    QImage alterar = imagem2->retorna_pixmap().toImage();
    QImage virada = alterar;

    //pega a altura e largura do pixmap da imagem original
    int largura = alterar.width();
    int altura = alterar.height();

    //percorre o pixmap da imagem original e substitui na alterada pelo valor do pixel encontrado na coordenada "oposta"
    for(int y =0; y < altura; y++){
        for(int x = 0; x<largura;x++){
            virada.setPixelColor(largura - x - 1,y,alterar.pixelColor(x,y));

        }
    }

    //da o displey da imagem alterada
    QPixmap alterada = QPixmap::fromImage(virada);
    imagem2->change_pixmap(alterada);
}


void MainWindow::on_tons_de_cinza_clicked()
{
    if(!cinza_clicado){
        QPixmap alterada = MainWindow::transforma_cinza(imagem2->retorna_pixmap());
        imagem2->change_pixmap(alterada);
        cinza_clicado = true;
    }

}
QPixmap MainWindow::transforma_cinza(QPixmap img){
    QImage torna_cinza = img.toImage();
    int largura = torna_cinza.width();
    int altura = torna_cinza.height();

    //variaveis de trabalho
    QColor cor;
    int cor2;

    //percorre o pixmap da imagem nova e aplica a formula em cada pixel
    for(int y = 0; y< altura;y++){
        for(int x = 0; x < largura; x++){
            cor = torna_cinza.pixelColor(x,y);
            cor2 = cor.red()*0.299 + cor.green()*0.587 + cor.blue()*0.114;
            cor = QColor(cor2,cor2,cor2);
            torna_cinza.setPixelColor(x,y,cor);
        }
    }
    return QPixmap::fromImage(torna_cinza);
}

void MainWindow::on_quantizar_clicked()
{
    //se a imagem não esta em tons de cinza não realiza a quantização
    if(cinza_clicado == false){
        QMessageBox::warning(this,"cuidado","A imagem não está em tons de cinza");

    }else{
        //senão faz uma cópia da imagem e salva a quantidade de tons desejada
        QImage alterada = imagem2->retorna_pixmap().toImage();
        int canais = ui->spinBox->value();

        //variaveis de trabalho
        QColor menor_valor = QColor(255,255,255), maior_valor = QColor(0,0,0), cor_atual;
        int largura = alterada.width();
        int altura = alterada.height();

        //percorre o pixmap da imagem para pegar o maior e o menor valor
        for(int y = 0; y < altura; y++){
            for(int x = 0; x < largura; x++ ){
                cor_atual = alterada.pixelColor(x,y);
                if(cor_atual.red() > maior_valor.red()){
                    maior_valor = QColor(cor_atual.red(),cor_atual.green(),cor_atual.blue());
                }
                if(cor_atual.red() < menor_valor.red()){
                    menor_valor = QColor(cor_atual.red(),cor_atual.green(),cor_atual.blue());
                }
            }
        }
        // faz o calculo do intervalo
        int tamanho_intervalor = maior_valor.red() - menor_valor.red() + 1;

        //se o numero de tons for maior ou igual ao intervalo nao quantiza
        if(tamanho_intervalor <= canais){
            QMessageBox::information(this,"aviso","A imagem já esta quantizada para esse valor");

        //se for para 0 tons, não quantiza
        }else if(canais == 0){
            QMessageBox::information(this,"aviso","Não é possivel quantizar para 0 tons");
        }
        else{
            //faz o calculo dos intervalos:
            int tb = tamanho_intervalor/canais,value;
            std::vector<double> intervalos;
            for (float inter = menor_valor.red() - 0.5; inter < maior_valor.red() + tb; inter = inter + tb) {
                if(inter > 255){
                    intervalos.push_back(255);
                }else{
                    intervalos.push_back(inter);
                }

            }
            //coloca os intervalos no console para debug
            qDebug() << menor_valor.red() <<" [] "<<maior_valor.red() <<"\n"<<intervalos;

            //muda o valor dos pixeis da imagem pelo valor medio do intervalo que eles se encontram
            for(int y = 0; y < altura; y++){
                for(int x = 0; x < largura; x++ ){
                    cor_atual = alterada.pixelColor(x,y);

                    for(int i = 0; i<intervalos.size();i++){
                        if(cor_atual.red() < intervalos[i]){
                            value = int((intervalos[i] + intervalos[i-1])/2);
                            cor_atual = QColor(value,value,value);
                            alterada.setPixelColor(x,y,cor_atual);
                            break;
                        }
                    }
                }
            }

            //faz o display do pixmap resultante
            QPixmap altered = QPixmap::fromImage(alterada);
            imagem2->change_pixmap(altered);
        }
    }


}


void MainWindow::on_geraHistograma_clicked()
{
    QString titulo = "Histograma " + QString::number(num_janela_hist);
    if(cinza_clicado){
        histograma = new Histograma();
        histograma->setWindowTitle(titulo);
        histograma->constroi_hist(imagem2->retorna_pixmap(),histograma);
        histograma->show();
    }else{
        num_janela_hist++;
        histograma = new Histograma();
        histograma->setWindowTitle(titulo);
        histograma->show();
        histograma->constroi_hist(MainWindow::transforma_cinza(imagem2->retorna_pixmap()),histograma);
    }

}

void MainWindow::on_brilho_clicked()
{
    int valor_deslocado = ui->numBrilho->value();
    QImage nova_imagem = imagem2->retorna_pixmap().toImage();
    QColor cor;
    int vermelho,azul,verde;
    for(int x = 0; x < nova_imagem.width();x++){
        for(int y = 0; y <nova_imagem.height();y++){

            //seta vermelho
            if(nova_imagem.pixelColor(x,y).red() + valor_deslocado > 255){
                vermelho = 255;
            }else if(nova_imagem.pixelColor(x,y).red() + valor_deslocado < 0){
                vermelho = 0;
            }else{
                vermelho = nova_imagem.pixelColor(x,y).red() + valor_deslocado;
            }

            //seta azul
            if(nova_imagem.pixelColor(x,y).blue() + valor_deslocado > 255){
                azul = 255;
            }else if(nova_imagem.pixelColor(x,y).blue() + valor_deslocado < 0){
                azul = 0;
            }else{
                azul = nova_imagem.pixelColor(x,y).blue() + valor_deslocado;
            }

            //seta verde
            if(nova_imagem.pixelColor(x,y).green() + valor_deslocado > 255){
                verde = 255;
            }else if(nova_imagem.pixelColor(x,y).green()+ valor_deslocado < 0){
                verde = 0;
            }else{
                verde = nova_imagem.pixelColor(x,y).green() + valor_deslocado;
            }

            cor = QColor(vermelho,verde,azul);

            nova_imagem.setPixelColor(x,y,cor);
        }
    }

    imagem2->change_pixmap(QPixmap::fromImage(nova_imagem));
}

void MainWindow::on_contraste_botao_clicked()
{
    QColor cor_final;
    QImage imagem = imagem2->retorna_pixmap().toImage();
    int vermelho,verde,azul;
    for(int x = 0; x<imagem.width(); x++){
        for(int y = 0; y <imagem.height(); y++){
            vermelho = ui->valor_contraste->value()*imagem.pixelColor(x,y).red();
            verde = ui->valor_contraste->value()*imagem.pixelColor(x,y).green();
            azul = ui->valor_contraste->value()*imagem.pixelColor(x,y).blue();
            if(vermelho > 255){ vermelho = 255;}
            if(verde > 255){ verde = 255;}
            if(azul > 255) {azul = 255;}
            cor_final = QColor(vermelho,verde,azul);
            imagem.setPixelColor(x,y,cor_final);
        }
    }
    imagem2->change_pixmap(QPixmap::fromImage(imagem));
}


void MainWindow::on_negativo_botao_clicked()
{
    QColor cor_final;
    QImage imagem = imagem2->retorna_pixmap().toImage();
    int vermelho,verde,azul;
    for(int x = 0; x<imagem.width(); x++){
        for(int y = 0; y <imagem.height(); y++){
            vermelho = 255 - imagem.pixelColor(x,y).red();
            verde = 255 - imagem.pixelColor(x,y).green();
            azul = 255 - imagem.pixelColor(x,y).blue();

            cor_final = QColor(vermelho,verde,azul);
            imagem.setPixelColor(x,y,cor_final);
        }
    }
    imagem2->change_pixmap(QPixmap::fromImage(imagem));
}


void MainWindow::on_equalize_botao_clicked()
{

    if(cinza_clicado){
        MainWindow::on_geraHistograma_clicked();
        QPixmap equalizado = histograma->equalize(imagem2->retorna_pixmap(),imagem2->retorna_pixmap());
        imagem2->change_pixmap(equalizado);
        MainWindow::on_geraHistograma_clicked();
    }else{
        MainWindow::on_geraHistograma_clicked();
        QPixmap cinza = MainWindow::transforma_cinza(imagem2->retorna_pixmap());
        QPixmap equalizado = histograma->equalize(imagem2->retorna_pixmap(),cinza);
        imagem2->change_pixmap(equalizado);
        MainWindow::on_geraHistograma_clicked();
    }


}



void MainWindow::on_Z_out_clicked()
{
    QImage imagem = imagem2->retorna_pixmap().toImage();
    int altura = ui->z_height->value();
    int largura = ui->z_width->value();

    int altura_new = qCeil(imagem.height()/altura);
    int largura_new = qCeil(imagem.width()/largura);
    int media_vermelho = 0, media_azul = 0, media_verde = 0;
    QColor cor;
    QImage imagem_nova = QImage(largura_new,altura_new,QImage::Format_RGB888);

    int x =0, y = 0;

    for(int i = 0; i < imagem.height();i = i + altura){
        for(int j = 0; j < imagem.width(); j = j + largura){

            for(int h = 0; h < altura && i + h < imagem.height(); h++){
                for(int w = 0; w < largura && w + j < imagem.width(); w++){
                    cor = imagem.pixelColor(j+w,i +h);
                    media_vermelho = media_vermelho + cor.red();
                    media_verde = media_verde + cor.green();
                    media_azul = media_azul + cor.blue();
                }
            }
            media_vermelho = media_vermelho/(altura*largura);
            media_verde = media_verde/(altura*largura);
            media_azul = media_azul/(altura*largura);

            cor = QColor(media_vermelho,media_verde,media_azul);
            imagem_nova.setPixelColor(x,y,cor);
            x++;

            media_vermelho = 0;
            media_verde = 0;
            media_azul = 0;
            if(x == imagem_nova.width()){
                break;
            }
        }
        x = 0;
        y++;
        if(y == imagem_nova.height()){
            break;
        }
    }

    imagem2->change_pixmap(QPixmap::fromImage(imagem_nova));
    imagem2->resize(imagem_nova.width() + 20,imagem_nova.height() + 20);

}





void MainWindow::on_Z_in_clicked()
{
    QImage original = imagem2->retorna_pixmap().toImage();
    int altura_new = original.height()*2 - 1, largura_new = original.width()*2 - 1;
    QImage imagem_nova = QImage(largura_new,altura_new, QImage::Format_RGB888);
    QColor cor;
    int vermelho,verde,azul;

    //coloca os pixeis da imagem original na imagem_nova
    for(int y = 0; y <original.height();y++){
        for(int x = 0; x < original.width(); x++ ){
            cor = original.pixelColor(x,y);
            imagem_nova.setPixelColor(x*2,y*2,cor);
        }
    }

    //faz os pixeis intermediarios em x
    for(int x = 1; x <= largura_new - 2; x = x + 2){
        for(int y = 0; y < altura_new; y = y + 2){
            vermelho = (imagem_nova.pixelColor(x-1,y).red() + imagem_nova.pixelColor(x+1,y).red())/2;
            verde = (imagem_nova.pixelColor(x-1,y).green() + imagem_nova.pixelColor(x+1,y).green())/2;
            azul = (imagem_nova.pixelColor(x-1,y).blue() + imagem_nova.pixelColor(x+1,y).blue())/2;
            cor = QColor(vermelho,verde,azul);
            imagem_nova.setPixelColor(x,y,cor);
        }
    }

    //faz os pixeis das linhas restantes
    for(int y = 1; y<=altura_new - 2;y = y +2){
        for(int x = 0; x<largura_new;x++){

            vermelho = (imagem_nova.pixelColor(x,y-1).red() + imagem_nova.pixelColor(x,y+1).red())/2;
            verde = (imagem_nova.pixelColor(x,y-1).green() + imagem_nova.pixelColor(x,y+1).green())/2;
            azul = (imagem_nova.pixelColor(x,y-1).blue() + imagem_nova.pixelColor(x,y+1).blue())/2;
            cor = QColor(vermelho,verde,azul);
            imagem_nova.setPixelColor(x,y,cor);
        }
    }

    imagem2->change_pixmap(QPixmap::fromImage(imagem_nova));
    imagem2->resize(imagem_nova.width() + 20,imagem_nova.height() + 20);

}


void MainWindow::on_rotate_dir_clicked()
{
    QImage imagem_r = imagem2->retorna_pixmap().toImage();
    QImage imagem_nova = QImage(imagem_r.height()  ,imagem_r.width() ,QImage::Format_RGB888);
    QColor cor;
    for(int x = 0; x<imagem_r.width();x++){
        for(int y = 0; y <imagem_r.height();y++){
            cor = imagem_r.pixelColor(x,y);
            imagem_nova.setPixelColor(imagem_r.height() - y - 1,x,cor);
        }
    }
    imagem2->change_pixmap(QPixmap::fromImage(imagem_nova));
    imagem2->resize(imagem_nova.width() + 20,imagem_nova.height() + 20);
}


void MainWindow::on_rotate_esq_clicked()
{
    QImage imagem_r = imagem2->retorna_pixmap().toImage();
    QImage imagem_nova = QImage(imagem_r.height()  ,imagem_r.width() ,QImage::Format_RGB888);
    QColor cor;
    for(int x = 0; x<imagem_r.width();x++){
        for(int y = 0; y <imagem_r.height();y++){
            cor = imagem_r.pixelColor(x,y);
            imagem_nova.setPixelColor(y,imagem_r.width() - 1 -x,cor);
        }
    }
    imagem2->change_pixmap(QPixmap::fromImage(imagem_nova));
    imagem2->resize(imagem_nova.width() + 20,imagem_nova.height() + 20);
}

void MainWindow::conv2(QVector<QVector<double>> kernel,bool soma_127, bool precisa_cinza){
    QImage imagem_velha = imagem2->retorna_pixmap().toImage();
    QImage imagem_nova = imagem2->retorna_pixmap().toImage();
    QColor cor;
    QVector<QVector<double>> k = kernel;
    float vermelho = 0,verde = 0,azul = 0;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j <3; j++){
            kernel[i][j] = k[2 - i][2 - j];
        }
    }
    qDebug() <<kernel;
    if(precisa_cinza && !cinza_clicado){
        QMessageBox::warning(this,"Atenção","Para essa convolução a imagem precisa estar em tons de cinza");
    }else{
        for(int x = 1; x < imagem_velha.width()-1; x++){
            for(int y = 1; y < imagem_velha.height()-1; y++){

                for(int i = 0; i < 3; i++){
                    for(int j = 0; j < 3; j++){
                        vermelho = vermelho + imagem_velha.pixelColor(x - 1 + j,y - 1 + i).red()*kernel[i][j];
                        verde = verde + imagem_velha.pixelColor(x - 1 + j,y - 1 + i).green()*kernel[i][j];
                        azul = azul + imagem_velha.pixelColor(x - 1 + j,y - 1 + i).blue()*kernel[i][j];
                    }
                }
                if(soma_127){
                    vermelho = vermelho + 127;
                    verde = verde + 127;
                    azul = azul + 127;
                }

                if(vermelho < 0) vermelho = 0;
                if(vermelho >255) vermelho = 255;
                if(azul < 0) azul = 0;
                if(azul > 255) azul = 255;
                if(verde < 0) verde = 0;
                if(verde > 255) verde = 255;

                cor = QColor(int(vermelho),int(verde),int(azul));
                imagem_nova.setPixelColor(x,y,cor);

                vermelho = 0;
                verde = 0;
                azul = 0;
            }
        }
    }

    imagem2->change_pixmap(QPixmap::fromImage(imagem_nova));
}


void MainWindow::on_conv_gaus_clicked()
{
    QVector<QVector<double>> kernel = {{0.0625,0.125,0.0625},{0.125,0.25,0.125},{0.0625,0.125,0.0625}};
    conv2(kernel,false,false);
}


void MainWindow::on_conv_lapl_clicked()
{
    QVector<QVector<double>> kernel = {{0,-1,0},{-1,4,-1},{0,-1,0}};
    conv2(kernel,false,true);
}


void MainWindow::on_conv_passa_altas_clicked()
{
    QVector<QVector<double>> kernel = {{-1,-1,-1},{-1,8,-1},{-1,-1,-1}};
    conv2(kernel,false,true);
}


void MainWindow::on_conv_Px_clicked()
{
    QVector<QVector<double>> kernel = {{-1,0,1},{-1,0,1},{-1,0,1}};
    conv2(kernel,true,true);
}


void MainWindow::on_conv_Py_clicked()
{
    QVector<QVector<double>> kernel = {{-1,-1,-1},{0,0,0},{1,1,1}};
    conv2(kernel,true,true);
}


void MainWindow::on_conv_sx_clicked()
{
    QVector<QVector<double>> kernel = {{-1,0,1},{-2,0,2},{-1,0,1}};
    conv2(kernel,true,true);
}


void MainWindow::on_conv_sy_clicked()
{
    QVector<QVector<double>> kernel = {{-1,-2,-1},{0,0,0},{1,2,1}};
    conv2(kernel,true,true);
}


void MainWindow::on_conv_personal_clicked()
{
    QVector<QVector<double>> kernel = {{ui->personal_00->value(),ui->personal_01->value(),ui->personal_02->value()},
                                       {ui->personal_10->value(),ui->personal_11->value(),ui->personal_12->value()},
                                       {ui->personal_20->value(),ui->personal_21->value(),ui->personal_22->value()}};
    conv2(kernel,false,true);
}


void MainWindow::on_pushButton_clicked()
{
    if(cinza_clicado){
        QImage hm_image = imagem2->retorna_pixmap().toImage();
        QImage src_image = imagem2->retorna_pixmap().toImage();

        QString filePath = QFileDialog::getOpenFileName(this, "Selecione uma imagem", "", "Imagens (*.png *.jpg *.jpeg *.bmp *.gif *.tif)");
        QPixmap source = imagem2->retorna_pixmap();
        QPixmap target(filePath);
        target = MainWindow::transforma_cinza(target);

        QVector<int>hst_src(256,0),hst_trg(256,0),hst_src_cum(256,0),hst_trg_cum(256,0);
        hst_src = histograma->histograma_y(source);
        hst_trg = histograma->histograma_y(target);

        hst_src_cum = histograma->histograma_cum(hst_src);
        hst_trg_cum = histograma->histograma_cum(hst_trg);


        for(int x = 0; x< hm_image.width();x++){
            for(int y = 0; y<hm_image.height();y++){
                //encontra o tom mais proximo
                int closest_shade = histograma->encontra_shade_mais_prox(src_image.pixelColor(x, y).red(), hst_src_cum, hst_trg_cum);

                // Aplicar a correspondência de tons na imagem de origem
                hm_image.setPixelColor(x, y, QColor(closest_shade, closest_shade, closest_shade));
            }
        }

        imagem2->change_pixmap(QPixmap::fromImage(hm_image));



    }else{
        QMessageBox::warning(this,"cuidado","A imagem não está em tons de cinza");
    }

}

