#include "janelaimg.h"
#include "ui_janelaimg.h"


JanelaImg::JanelaImg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JanelaImg)
{
    ui->setupUi(this);
}

JanelaImg::~JanelaImg()
{
    delete ui;
}

void JanelaImg::change_pixmap(QPixmap pixmap_novo){
    ui->imagem->setPixmap(pixmap_novo);
    ui->imagem->adjustSize();

}

QPixmap JanelaImg::retorna_pixmap(){
    return ui->imagem->pixmap();
}


