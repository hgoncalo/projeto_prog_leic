#include "SVGElements.hpp"

namespace svg
{
    // These must be defined!
    SVGElement::SVGElement() {}
    SVGElement::~SVGElement() {}

    // Ellipse (initial code provided)
    Ellipse::Ellipse(const Color &fill,
                     const Point &center,
                     const Point &radius)
        : fill(fill), center(center), radius(radius)
    {
    }
    void Ellipse::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, radius, fill);
    }

    //POLYLINE dale
    Polyline::Polyline(const Color &stroke,
                     const std::vector<Point> &points)
        : stroke(stroke), points(points)
    {
    }
    void Polyline::draw(PNGImage &img) const
    {
        //img.draw_ellipse(center, radius, fill);
        // DRAW multiple lines??
    }

    //POLYGON
    Polygon::Polygon(const Color &fill,
                    const std::vector<Point> &points)
        : fill(fill), points(points)
    {
    }
    void Polygon::draw(PNGImage &img) const
    {
        img.draw_polygon(points, fill);
    }

    //SUBCLASSES

}   
