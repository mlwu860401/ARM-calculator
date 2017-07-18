#include "lengend.h"
#include <gl/GL.h>
#include <QFont>



lengend::lengend(QWidget *parent):QGLWidget(parent){}
lengend::~lengend(){}

void lengend::initializeGL()
{
    glClearColor(1.0,1.0,1.0,0.2);
}

void lengend::draw_rect(double x_origin, double y_origin, double x_length, double y_length, int red, int green, int blue)
{
    glColor3ub(red, green, blue);
    glBegin(GL_QUADS);
    glVertex2f(x_origin,y_origin);
    glVertex2f(x_origin + x_length, y_origin);
    glVertex2f(x_origin + x_length, y_origin + y_length);
    glVertex2f(x_origin, y_origin + y_length);
    glEnd();
}

void lengend::draw_box(double x_origin, double y_origin, double x_length, double y_length, int red, int green, int blue)
{
    draw_rect(x_origin, y_origin, x_length, y_length, red, green, blue);
    glColor3ub(0,0,0);
    glBegin(GL_LINE_STRIP);
    glVertex2f(x_origin,y_origin);
    glVertex2f(x_origin + x_length, y_origin);
    glVertex2f(x_origin + x_length, y_origin + y_length);
    glVertex2f(x_origin, y_origin + y_length);
    glVertex2f(x_origin,y_origin);
    glEnd();
}

void lengend::paintGL()
{
    int last = 0;
    if(last != legend_switch)
    {
        update();
    }
    last = legend_switch;
    double widget_height = double(height());
    double rect_length = double(width())/4.0*3.0;
    double x_origin = double(width())/2.0;
    QFont fig_title("times", 20);
    QFont fig_label("times", 12);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(legend_switch == 0)
    {
        renderText(x_origin - 75.0, 75.0, 0.0, "Amortization", fig_title);
        draw_rect(x_origin - rect_length/2.0, widget_height/4.0, rect_length, 3.0, 32,178,170);//balance
        glColor3ub(0,0,0);
        renderText(x_origin - 65.0, widget_height/4.0*1.0 + 25.0, 0.0, "Remained Balance", fig_label);
        draw_rect(x_origin - rect_length/2.0, widget_height/4.0*2.0, rect_length, 3.0, 220,20,60);//interest
        glColor3ub(0,0,0);
        renderText(x_origin - 45.0, widget_height/4.0*2.0 + 25.0, 0.0, "Paid Interest", fig_label);
        draw_rect(x_origin - rect_length/2.0, widget_height/4.0*3.0, rect_length, 3.0, 30,144,255);//principal
        glColor3ub(0,0,0);
        renderText(x_origin - 45.0, widget_height/4.0*3.0 + 25.0, 0.0, "Paid Principal", fig_label);
        glColor3ub(0,0,0);
    }
    if(legend_switch == 1)
    {
        renderText(x_origin - 50, 75.0, 0.0, "Ratio Chart", fig_title);
        draw_box(x_origin - rect_length/2.0, widget_height/2.0, rect_length/2.0, 10.0, 30,144,255);
        draw_box(x_origin, widget_height/2.0, rect_length/2.0, 10.0, 220,20,60);
        glColor3ub(0,0,0);
        renderText(x_origin - 75.0, widget_height/2.0 + 35.0, 0.0, "Principal / Interest", fig_label);
    }
}

void lengend::resizeGL(int width, int height)
{

    glViewport(0,0,width,height);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    glOrtho (0,width, height, 0, 0, 1) ;
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
}
