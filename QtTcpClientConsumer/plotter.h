#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <vector>

class Plotter : public QWidget
{
    Q_OBJECT
private:
    std::vector<long> time;
    std::vector<int> data;
public:
    explicit Plotter(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void setData(std::vector<long> time, std::vector<int> data);
signals:

};

#endif // PLOTTER_H
