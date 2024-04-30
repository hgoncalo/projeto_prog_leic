#include "SVGElements.hpp"

namespace svg
{
    // These must be defined!
    SVGElement::SVGElement() {}
    SVGElement::~SVGElement() {}

    //
    // ELLIPSE
    //
    Ellipse::Ellipse(const Color &fill, const Point &center, const Point &radius): fill(fill), center(center), radius(radius){};
    Ellipse::Ellipse(const Color &fill, const Point &center) :fill(fill), center(center){};
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


    //
    //CIRCLE
    //
    Circle::Circle(const Color &fill,const Point &center, const int &radius): Ellipse(fill,center), radius_(radius){};
    int Circle::get_radius() const 
    {
        return radius_;
    }
    void Circle::draw(PNGImage &img) const 
    {
        Point rad = {get_radius(), get_radius()}; //radius x = radius y in a circle
        img.draw_ellipse(get_center(), rad, get_color());
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
        //img.draw_line(());
        // os pontos ficam "start,start , end,end , depois o end,end é o start,start do ponto seguinte"
        //for (SVGElement* e: points)
    }


    //
    // LINE
    //
    Line::Line(const Color &stroke, const Point &start, const Point &end) : Polyline(stroke) , start(start), end(end){};
    void Line::draw(PNGImage &img) const 
    { 
        img.draw_line(start, end, get_stroke());
    }

    
    //
    // POLYGON
    //
    Polygon::Polygon(const Color &fill, const std::vector<Point> &points): fill(fill), points(points){};
    void Polygon::draw(PNGImage &img) const 
    {
        img.draw_polygon(points, fill);
    }


    //
    // RECT
    //
    Rect::Rect(const Color &fill, const std::vector<Point> &start, const int &width, const int &height) : Polygon(fill,start), width(width), height(height){};
    void Rect::draw(PNGImage &img) const
    {
        // por implementar
        // ler .hpp para +
    }

}   
