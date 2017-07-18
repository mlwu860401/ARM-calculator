#ifndef LENGEND_H
#define LENGEND_H

#include <QObject>
#include <QGLWidget>
#include <QApplication>


class lengend : public QGLWidget
{
    Q_OBJECT
public:

    explicit lengend(QWidget *parent = 0);
    int legend_switch = 0;
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void draw_rect(double x_origin, double y_origin, double x_length, double y_length, int red, int green, int blue);
    void draw_box(double x_origin, double y_origin, double x_length, double y_length, int red, int green, int blue);
    ~lengend();
};

#endif // LENGEND_H
