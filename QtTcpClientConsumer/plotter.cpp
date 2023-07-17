#include "plotter.h"
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <algorithm>

Plotter::Plotter(QWidget *parent)
    : QWidget{parent}
{

}

void Plotter::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QBrush brush;
    QPen pen;
    std::vector<double> y, t;
    int ymax, ymin;
    long tmax, tmin;
    int len = data.size();

    // Pinta o fundo de amarelo
    brush.setColor(QColor(255,255,100));
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    painter.drawRect(0,0,width(),height());

    // Seta a caneta azul
    pen.setColor(Qt::darkCyan);
    pen.setWidth(2);
    painter.setPen(pen);

    if (data.size() < 2)
    {
        return;
    }

    ymax = *std::max_element(data.begin(), data.end());
    ymin = *std::min_element(data.begin(), data.end());
    tmax = *std::max_element(time.begin(), time.end());
    tmin = *std::min_element(time.begin(), time.end());

    for (int i = 0; i < len; i++)
    {
        t.push_back((time[i] - tmin)* width() / (tmax - tmin));
        y.push_back((data[i] - ymin) * height() / (ymax - ymin));
    }

    for (int i = 0; i < len - 1; i++)
    {
        painter.drawLine(t[i], y[i], t[i+1], y[i+1]);
    }
}

void Plotter::setData(std::vector<long> time, std::vector<int> data)
{
    this->time = time;
    this->data = data;
    repaint();
}
