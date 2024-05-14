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
        this->center = center.translate(xy);
    }
    void Ellipse::rotate(Point &origin, int &trans_scalar) 
    {
        this->center = center.rotate(origin,trans_scalar);
    }
    void Ellipse::scale(Point &origin, int &trans_scalar) 
    {
        this->center = this->center.scale(origin,trans_scalar);

        //ao darmos scale à ellipse, também aumentamos o raio da ellipse
        this->radius.x = this->radius.x * trans_scalar;
        this->radius.y = this->radius.y * trans_scalar;
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
    void Circle::scale(Point &origin, int &trans_scalar) 
    {
        this->center = this->center.scale(origin,trans_scalar);
        this->radius = radius * trans_scalar;
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
    void Polyline::scale(Point &origin, int &trans_scalar) 
    {
        std::vector<Point> new_vec;
        //a cada ponto da polyline, deslocar esse ponto
        for(size_t i = 0; i< points.size(); i++){
            Point new_p = points[i].scale(origin,trans_scalar);
            new_vec.push_back(new_p);
        }
        this->points = new_vec;
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
    void Line::scale(Point &origin, int &trans_scalar) 
    {
        start = start.scale(origin,trans_scalar);
        end = end.scale(origin,trans_scalar);
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
    void Polygon::scale(Point &origin, int &trans_scalar) 
    {
        std::vector<Point> new_vec;
        //a cada ponto da polyline, deslocar esse ponto
        for(size_t i = 0; i< points.size(); i++){
            Point new_p = points[i].scale(origin,trans_scalar);
            new_vec.push_back(new_p);
        }
        this->points = new_vec;
    }


    //
    // RECT
    //
    Rect::Rect(const Color &fill, const std::vector<Point> &points, const int &width, const int &height) : Polygon(fill), points(points), width(width), height(height){};
    void Rect::draw(PNGImage &img) const
    {

        img.draw_polygon(points,get_color());
    }
    void Rect::translate(const Point &xy) 
    {
        std::vector<Point> new_vec;
        //a cada ponto da polyline, deslocar esse ponto
        for(size_t i = 0; i< points.size(); i++){
            Point new_p = points[i].translate(xy);
            new_vec.push_back(new_p);
        }
        this->points = new_vec;
    }
    void Rect::rotate(Point &origin, int &trans_scalar) 
    {   
        std::vector<Point> new_vec;
        //a cada ponto da polyline, deslocar esse ponto
        for(size_t i = 0; i< points.size(); i++){
            Point new_p = points[i].rotate(origin,trans_scalar);
            new_vec.push_back(new_p);
        }
        this->points = new_vec;
    }
    void Rect::scale(Point &origin, int &trans_scalar) 
    {
        std::vector<Point> new_vec;
        //a cada ponto da polyline, deslocar esse ponto
        for(size_t i = 0; i< points.size(); i++){
            Point new_p = points[i].scale(origin,trans_scalar);
            new_vec.push_back(new_p);
        }
        this->points = new_vec;
    }

    //
    // GROUP
    //

    Group::Group(const std::vector<SVGElement*> &elements): elements(elements) {};
    void Group::draw(PNGImage &img) const
    {
        for (SVGElement* element: this->elements)
        {
            element->draw(img);
        } 
    }
    void Group::translate(const Point &xy) 
    {
        for (SVGElement* element: this->elements)
        {
            element->translate(xy);
        } 
    }
    void Group::rotate(Point &origin, int &trans_scalar) 
    {   
        for (SVGElement* element: this->elements)
        {
            element->rotate(origin,trans_scalar);
        } 
    }
    void Group::scale(Point &origin, int &trans_scalar) 
    {
        for (SVGElement* element: this->elements)
        {
            element->scale(origin,trans_scalar);
        } 
    }
    std::vector<SVGElement*> Group::get_vector(){
        return elements;
    }    
    Group::~Group(){
        for (SVGElement* element: elements){
            delete element;
        }
        elements.clear();
    }

}   
