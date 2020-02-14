#include "dominantcolorcalculator.h"

#include <QDebug>
#include <QtMath>
#include <QPixmap>

QString DominantColorCalculator::calculate(QImage srcImg){
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
                        noir++;
                    break;
                    default:
                        bleu++;
                    break;
                    }
                break;
                case 1:
                    switch (converted[2]) {
                    case 0:
                        vert++;
                    break;
                    case 1:
                        cyan++;
                    break;
                    case 2:
                        bleu++;
                    break;
                    }
                break;
                case 2:
                    switch (converted[2]) {
                    case 2:
                        cyan++;
                    break;
                    default:
                        vert++;
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
                        rouge++;
                    break;
                    case 1:
                        rose++;
                    break;
                    case 2:
                        violet++;
                    break;
                    }
                break;
                case 1:
                    switch (converted[2]) {
                    case 0:
                        vert++;
                    break;
                    case 1:
                        gris++;
                    break;
                    case 2:
                        violet++;
                    break;
                    }
                break;
                case 2:
                    switch (converted[2]) {
                    case 2:
                        cyan++;
                    break;
                    default:
                        vert++;
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
                        rouge++;
                    break;
                    default:
                        rose++;
                    break;
                    }
                break;
                case 1:
                    switch (converted[2]) {
                    case 2:
                        rose++;
                    break;
                    default:
                        orange++;
                    break;
                    }
                break;
                case 2:
                    switch (converted[2]) {
                    case 2:
                        blanc++;
                    break;
                    default:
                        jaune++;
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
        if(c[i] > best){
            best = c[i];
            iBest = i;
        }
    }
    return names[iBest];
}
