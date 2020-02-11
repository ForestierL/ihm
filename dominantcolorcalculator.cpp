#include "dominantcolorcalculator.h"

#include <QDebug>
#include <QtMath>

dominantColorCalculator::dominantColorCalculator()
{
}

void calculate(QImage scaledImg){
    int precision = 10;
    int profondeur = 2;

    int RGB[4][precision];
    float couleurs[4][precision];
    int finalC[4][precision];
    for(int x = 0; x < 3; x++){
        for(int y = 0; y < precision; y++){
            RGB[x][y] = 0;
            couleurs[x][y] = 0;
            finalC[x][y] = 0;
        }
    }
    for (int row=0; row< scaledImg.height(); row++) {
        for (int col=0; col< scaledImg.width(); col++) {
            QRgb pixColor = scaledImg.pixel(row, col);
            QVector<int> colors;
            colors.append(qRed(pixColor));
            colors.append(qGreen(pixColor));
            colors.append(qBlue(pixColor));
            for(int i = 0; i < colors.size(); i++){
                RGB[i][qCeil(colors.at(i)/(255/precision))] = RGB[i][qCeil(colors.at(i)/(255/precision))]+1;
            }
        }
    }
    for(int i = 0; i < precision; i++){
        RGB[3][i] = RGB[0][i]+RGB[1][i]+RGB[2][i];
        //qDebug() << RGB[0][i] << RGB[1][i] << RGB[2][i];
    }
    for(int j = 0; j < precision; j++){
        int bestR = 0, bestG = 0, bestB = 0;
        int iR = 0, iG = 0, iB = 0;
        for(int i = 0; i < precision; i++){
            if(RGB[0][i] > bestR){
                bestR = RGB[0][i];
                iR = i;
            }
            if(RGB[1][i] > bestG){
                bestG = RGB[1][i];
                iG = i;
            }
            if(RGB[2][i] > bestB){
                bestB = RGB[2][i];
                iB = i;
            }
        }
        couleurs[0][j] = iR*(255/precision)+(255/precision);
        couleurs[1][j] = iG*(255/precision)+(255/precision);
        couleurs[2][j] = iB*(255/precision)+(255/precision);
        couleurs[3][j] = RGB[3][j];
        RGB[0][iR] = 0;
        RGB[1][iG] = 0;
        RGB[2][iB] = 0;
        RGB[3][j] = 0;
    }
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 10; j++){
            //qDebug() << couleurs[i][j];
            couleurs[i][j] = profondeur*couleurs[i][j]/255;
        }
    }
    int noir = 0, bleu = 0, vert = 0, cyan = 0, rouge = 0, rose = 0, jaune = 0, blanc = 0, gris = 0, orange = 0, violet = 0;
    for(int p = 0; p < profondeur+1; p++){
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 10; j++){
                //qDebug() << p << "couleurs[" << i << "][" << j <<"]" << couleurs[i][j] << (couleurs[i][j]-p) << (couleurs[i][j]-p > -0.5) << (couleurs[i][j]-p < 0.5);
                if(couleurs[i][j]-p > -0.5 && couleurs[i][j]-p < 0.5){
                    finalC[i][j] = p;
                }
                //qDebug() << p << "finalC[" << i << "][" << j <<"]" << finalC[i][j];
            }
        }
    }
    for(int i = 0; i < 10; i++){
        if(finalC[0][i] == 0 && finalC[1][i] == 0 && finalC[2][i] == 0){
            noir += couleurs[3][i];
        } else if((finalC[0][i] == 0 && finalC[1][i] == 0 && finalC[2][i] == 2) ||
                  (finalC[0][i] == 0 && finalC[1][i] == 0 && finalC[2][i] == 1) ||
                  (finalC[0][i] == 0 && finalC[1][i] == 1 && finalC[2][i] == 2)){
            bleu += couleurs[3][i];
        } else if((finalC[0][i] == 0 && finalC[1][i] == 2 && finalC[2][i] == 0) ||
                  (finalC[0][i] == 0 && finalC[1][i] == 1 && finalC[2][i] == 0) ||
                  (finalC[0][i] == 1 && finalC[1][i] == 1 && finalC[2][i] == 0) ||
                  (finalC[0][i] == 0 && finalC[1][i] == 2 && finalC[2][i] == 1) ||
                  (finalC[0][i] == 1 && finalC[1][i] == 2 && finalC[2][i] == 0) ||
                  (finalC[0][i] == 1 && finalC[1][i] == 2 && finalC[2][i] == 1)){
            vert += couleurs[3][i];
        } else if((finalC[0][i] == 0 && finalC[1][i] == 2 && finalC[2][i] == 2) ||
                  (finalC[0][i] == 0 && finalC[1][i] == 1 && finalC[2][i] == 1) ||
                  (finalC[0][i] == 1 && finalC[1][i] == 2 && finalC[2][i] == 2)){
            cyan += couleurs[3][i];
        } else if((finalC[0][i] == 2 && finalC[1][i] == 0 && finalC[2][i] == 0) ||
                  (finalC[0][i] == 1 && finalC[1][i] == 0 && finalC[2][i] == 0)){
            rouge += couleurs[3][i];
        } else if((finalC[0][i] == 2 && finalC[1][i] == 0 && finalC[2][i] == 2) ||
                  (finalC[0][i] == 1 && finalC[1][i] == 0 && finalC[2][i] == 1) ||
                  (finalC[0][i] == 2 && finalC[1][i] == 0 && finalC[2][i] == 1) ||
                  (finalC[0][i] == 2 && finalC[1][i] == 1 && finalC[2][i] == 2)){
            rose += couleurs[3][i];
        } else if((finalC[0][i] == 2 && finalC[1][i] == 2 && finalC[2][i] == 0) ||
                  (finalC[0][i] == 2 && finalC[1][i] == 2 && finalC[2][i] == 1)){
            jaune += couleurs[3][i];
        } else if((finalC[0][i] == 2 && finalC[1][i] == 2 && finalC[2][i] == 2)){
            blanc += couleurs[3][i];
        } else if((finalC[0][i] == 1 && finalC[1][i] == 1 && finalC[2][i] == 1)){
            gris += couleurs[3][i];
        } else if((finalC[0][i] == 2 && finalC[1][i] == 1 && finalC[2][i] == 0) ||
                  (finalC[0][i] == 2 && finalC[1][i] == 1 && finalC[2][i] == 1)){
            orange += couleurs[3][i];
        } else if((finalC[0][i] == 1 && finalC[1][i] == 0 && finalC[2][i] == 2) ||
                  (finalC[0][i] == 1 && finalC[1][i] == 1 && finalC[2][i] == 2)){
            violet += couleurs[3][i];
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
