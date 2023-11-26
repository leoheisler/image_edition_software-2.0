#ifndef JANELAIMG_H
#define JANELAIMG_H

#include <QDialog>
namespace Ui {
class JanelaImg;
}

class JanelaImg : public QDialog
{
    Q_OBJECT

public:
    explicit JanelaImg(QWidget *parent = nullptr);
    ~JanelaImg();
    void change_pixmap(QPixmap pixmap_novo);
    QPixmap retorna_pixmap();


private:
    Ui::JanelaImg *ui;

};

#endif // JANELAIMG_H
