#include "SVGElements.hpp"

//test
#include <iostream>

namespace svg
{
    // These must be defined!
    SVGElement::SVGElement() {}
    SVGElement::~SVGElement() {}

    //
    // ELLIPSE
    //
    Ellipse::Ellipse(const Color &fill, Point &center, const Point &radius): fill(fill), center(center), radius(radius){};
    Ellipse::Ellipse(const Color &fill) :fill(fill){};
    Color Ellipse::get_color() const {
        return fill;
    }
    Point Ellipse::get_center() const {
        return center;
    }
    void Ellipse::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, radius, fill);
    }
    void Ellipse::translate(const Point &xy)
    {
        //std::cout << "Center before: " << get_center().x << get_center().y << '\n'; 
        this->center = center.translate(xy);
        //std::cout << "Center after: " << get_center().x << get_center().y << '\n';
    }
    void Ellipse::rotate(Point &origin, int &trans_scalar) 
    {
        this->center = center.rotate(this->center,trans_scalar);
    }
    void Ellipse::scale(int &trans_scalar) 
    {
        //@ todo
    }


    //
    //CIRCLE
    //
    Circle::Circle(const Color &fill, Point &center, const int &radius): Ellipse(fill), center(center), radius(radius){};
    int Circle::get_radius() const 
    {
        return radius;
    }
    void Circle::draw(PNGImage &img) const 
    {
        Point rad = {radius, radius}; //radius x = radius y in a circle
        img.draw_ellipse(center, rad, get_color());
    }
    void Circle::translate(const Point &xy)
    {
        this->center = this->center.translate(xy);
    }
    void Circle::rotate(Point &origin, int &trans_scalar) 
    {
        this->center = this->center.rotate(origin,trans_scalar);
    }
    void Circle::scale(int &trans_scalar) 
    {
        //@ todo
    }


    //
    // POLYLINE 
    //
    Polyline::Polyline(const Color &stroke, const std::vector<Point> &points) : stroke(stroke), points(points){};
    Polyline::Polyline(const Color &stroke) : stroke(stroke){}; 
    Color Polyline::get_stroke() const
    {
        return stroke;
    }
    void Polyline::draw(PNGImage &img) const
    {
        Point current = points[0]; //assumindo que o vetor points não é vazio, há sempre um elemento inicial
        //img.draw_line(());
        // os pontos ficam "start,start , end,end , depois o end,end é o start,start do ponto seguinte"
        for (size_t i = 1; i < points.size(); i++){
            Point next = points[i]; //o ponto seguinte será o 1º end point
            img.draw_line(current,next,get_stroke()); //sendo current = start, next = end, construimos uma line, pois uma polyline são multiplas lines
            current = next; //o ponto next será o start da próxima iteração
        }
    }
    void Polyline::translate(const Point &xy) 
    {
        std::vector<Point> new_vec;
        //a cada ponto da polyline, deslocar esse ponto
        for(size_t i = 0; i< points.size(); i++){
            Point new_p = points[i].translate(xy);
            new_vec.push_back(new_p);
        }
        this->points = new_vec;
    }
    void Polyline::rotate(Point &origin, int &trans_scalar) 
    {
        std::vector<Point> new_vec;
        //a cada ponto da polyline, deslocar esse ponto
        for(size_t i = 0; i< points.size(); i++){
            Point new_p = points[i].rotate(origin,trans_scalar);
            new_vec.push_back(new_p);
        }
        this->points = new_vec;
    }
    void Polyline::scale(int &trans_scalar) 
    {
        //@ todo
    }


    //
    // LINE
    //
    Line::Line(const Color &stroke, const Point &start, const Point &end) : Polyline(stroke) , start(start), end(end){};
    void Line::draw(PNGImage &img) const 
    { 
        img.draw_line(start, end, get_stroke());
    }
    void Line::translate(const Point &xy) 
    {
        start = start.translate(xy);
        end = end.translate(xy);
    }
    void Line::rotate(Point &origin, int &trans_scalar) 
    {
        start = start.rotate(origin,trans_scalar);
        end = end.rotate(origin,trans_scalar);
    }
    void Line::scale(int &trans_scalar) 
    {
        //@ todo
    }

    
    //
    // POLYGON
    //
    Polygon::Polygon(const Color &fill, const std::vector<Point> &points): fill(fill), points(points){};
    Polygon::Polygon(const Color &fill): fill(fill){};
    Color Polygon::get_color() const
    {
        return fill;
    }
    void Polygon::draw(PNGImage &img) const 
    {
        img.draw_polygon(points, fill);
    }
    void Polygon::translate(const Point &xy) 
    {
        std::vector<Point> new_vec;
        //a cada ponto da polyline, deslocar esse ponto
        for(size_t i = 0; i< points.size(); i++){
            Point new_p = points[i].translate(xy);
            new_vec.push_back(new_p);
        }
        this->points = new_vec;
    }
    void Polygon::rotate(Point &origin, int &trans_scalar) 
    {
        std::vector<Point> new_vec;
        //a cada ponto da polyline, deslocar esse ponto
        for(size_t i = 0; i< points.size(); i++){
            Point new_p = points[i].rotate(origin,trans_scalar);
            new_vec.push_back(new_p);
        }
        this->points = new_vec;
    }
    void Polygon::scale(int &trans_scalar) 
    {
        //@ todo
    }


    //
    // RECT
    //
    Rect::Rect(const Color &fill, const Point &start, const int &width, const int &height) : Polygon(fill), start(start), width(width), height(height){};
    void Rect::draw(PNGImage &img) const
    {
        //obter todos os pontos do retângulo
        // (x,y) -> 0
        // (x+width,y) -> 1
        // (x,y+height) -> 3
        // (x+width,y+height) -> 2
        // meter num vetor de pontos
        std::vector<Point> rect_pts;

        //OBS : width-1 e height-1 são importantes, porque são os limites da figura!
        // precisamos de fazer -1 porque começamos no índice 0! (e não no 1)
        Point x1 = {start.x+width-1,start.y};
        Point x2 = {start.x+width-1,start.y+height-1};
        Point x3 = {start.x,start.y+height-1};
        rect_pts.push_back(start);
        rect_pts.push_back(x1);
        rect_pts.push_back(x2);
        rect_pts.push_back(x3);

        img.draw_polygon(rect_pts,get_color());
    }
    void Rect::translate(const Point &xy) 
    {
        //apenas é necessário fazer o start porque no draw ele altera os restantes
        this->start = this->start.translate(xy);
    }
    void Rect::rotate(Point &origin, int &trans_scalar) 
    {   
        //@todo : not working // fazer todos pts??
        this->start = this->start.rotate(origin,trans_scalar);
    }
    void Rect::scale(int &trans_scalar) 
    {
        //@ todo
    }

}   
