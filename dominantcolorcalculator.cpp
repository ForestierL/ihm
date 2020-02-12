#include "dominantcolorcalculator.h"

#include <QDebug>
#include <QtMath>
#include <QPixmap>

dominantColorCalculator::dominantColorCalculator()
{
}

void calculate(QImage srcImg){
    qDebug() << "TEST";
    QImage scaledImg = srcImg.scaledToWidth(200);
    QPixmap scaledPix = QPixmap::fromImage(scaledImg);

    int profondeur = 2;

    int noir = 0, bleu = 0, vert = 0, cyan = 0, rouge = 0, rose = 0, jaune = 0, blanc = 0, gris = 0, orange = 0, violet = 0;
    for (int row=0; row< scaledImg.height(); row++) {
        for (int col=0; col< scaledImg.width(); col++) {
            QRgb pixColor = scaledImg.pixel(col, row);
            QVector<float> colors;
            colors.append(qRed(pixColor));
            colors.append(qGreen(pixColor));
            colors.append(qBlue(pixColor));
            QVector<int> converted = {0,0,0};
            for(int i = 0; i < colors.size(); i++){
                colors[i] = profondeur*colors[i]/255;
            }
            for(int i = 0; i < 3; i++){
                for(int p = 0; p < profondeur+1; p++){
                    if(colors[i]-p > -0.5 && colors[i]-p < 0.5){
                        converted[i] = p;
                    }
                }
            }
            switch (converted[0]) {
            case 0:
                switch(converted[1]){
                case 0:
                    switch (converted[2]) {
                    case 0:
                        noir++;//000
                    break;
                    default:
                        bleu++;//001    002
                    break;
                    }
                break;
                case 1:
                    switch (converted[2]) {
                    case 0:
                        vert++;//010
                    break;
                    case 1:
                        cyan++;//011
                    break;
                    case 2:
                        bleu++;//012
                    break;
                    }
                break;
                case 2:
                    switch (converted[2]) {
                    case 2:
                        cyan++;//022
                    break;
                    default:
                        vert++;//020    021
                    break;
                    }
                break;
                }
            break;
            case 1:
                switch(converted[1]){
                case 0:
                    switch (converted[2]) {
                    case 0:
                        rouge++;//100
                    break;
                    case 1:
                        rose++;//101
                    break;
                    case 2:
                        violet++;//102
                    break;
                    }
                break;
                case 1:
                    switch (converted[2]) {
                    case 0:
                        vert++;//110
                    break;
                    case 1:
                        gris++;//111
                    break;
                    case 2:
                        violet++;//112
                    break;
                    }
                break;
                case 2:
                    switch (converted[2]) {
                    case 2:
                        cyan++;//122
                    break;
                    default:
                        vert++;//120    121
                    break;
                    }
                break;
                }
            break;
            case 2:
                switch(converted[1]){
                case 0:
                    switch (converted[2]) {
                    case 0:
                        rouge++;//200
                    break;
                    default:
                        rose++;//201    202
                    break;
                    }
                break;
                case 1:
                    switch (converted[2]) {
                    case 2:
                        rose++;//212
                    break;
                    default:
                        orange++;//210  211
                    break;
                    }
                break;
                case 2:
                    switch (converted[2]) {
                    case 2:
                        blanc++;//222
                    break;
                    default:
                        jaune++;//220    221
                    break;
                    }
                break;
                }
            break;
            }
        }
    }

    int best = 0, iBest = 0;
    QString names[11] = {"Noir", "Bleu", "Vert", "Cyan", "Rouge", "Rose", "Jaune", "Blanc", "Gris", "Orange", "Violet"};
    int c[11] = {noir, bleu, vert, cyan, rouge, rose, jaune, blanc, gris, orange, violet};
    for(int i = 0; i < 11; i++){
        //qDebug() << names[i] << " " << c[i];
        if(c[i] > best){
            best = c[i];
            iBest = i;
        }
    }
    qDebug() << names[iBest] << " " << c[iBest];
}
