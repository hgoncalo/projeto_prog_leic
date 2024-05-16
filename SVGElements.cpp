#include "SVGElements.hpp"

//test
#include <iostream>

namespace svg
{
    // These must be defined!

    /// @brief Constructor that initiates the 'SVGElement' with standard values
    SVGElement::SVGElement() {}

    /// @brief Destructor that deletes every element of type 'SVGElement'
    SVGElement::~SVGElement() {}


    //
    // ELLIPSE
    //

    /// @brief Constructor that creates the Ellipse
    /// @param fill Color of the Ellipse
    /// @param center Point of the center of the Ellipse
    /// @param radius Point of the radius of the Ellipse
    Ellipse::Ellipse(const Color &fill, Point &center, const Point &radius): fill(fill), center(center), radius(radius) {};

    /// @brief Specific constructor for the Circle, using only the color
    /// @param fill Color of the Ellipse
    Ellipse::Ellipse(const Color &fill) :fill(fill) {};

    /// @brief Fetches the Ellipse color
    /// @return The Ellipse color
    Color Ellipse::get_color() const {
        return fill;
    }

    /// @brief Fetches the Ellipse center
    /// @return The Ellipse center 
    Point Ellipse::get_center() const {
        return center;
    }

    /// @brief Fetches the Ellipse radius
    /// @return The Ellipse radius
    Point Ellipse::get_radius() const {
        return radius;
    }

    /// @brief Virtual function of 'draw' overridden for the Ellipse object that calls the 'draw_ellipse' method in 'PNGImage.cpp'
    /// @param img The PNG Image in which the element will be drawn
    void Ellipse::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, radius, fill);
    }

    /// @brief Virtual function of 'translate' overridden for the Ellipse object that calls the 'translate' method in 'Point.cpp'
    /// @param xy A Point of an element
    void Ellipse::translate(const Point &xy)
    {
        this->center = center.translate(xy);
    }

    /// @brief Virtual function of 'rotate' overridden for the Ellipse object that calls the 'rotate' method in 'Point.cpp'
    /// @param origin Origin of the rotation
    /// @param trans_scalar Rate of the rotation in degrees
    void Ellipse::rotate(Point &origin, int &trans_scalar) 
    {
        this->center = center.rotate(origin,trans_scalar);
    }

    /// @brief Virtual function of 'scale' overridden for the Ellipse object that changes the values of the radius and calls the 'scale' method in 'Point.cpp'
    /// @param origin Origin of the scale
    /// @param trans_scalar Factor of the scale
    void Ellipse::scale(Point &origin, int &trans_scalar) 
    {
        this->center = this->center.scale(origin,trans_scalar);

        //  As we scale the ellipse, the need to multiply the radius (not only the center) by the 'trans_scalar'
        this->radius.x = this->radius.x * trans_scalar;
        this->radius.y = this->radius.y * trans_scalar;
    }

    /// @brief Virtual function of 'copy' overridden for the Ellipse object that creates a new object with the same attributes as the original caller
    /// @return New pointer to an Ellipse object with the same attributes as the original
    SVGElement* Ellipse::copy() 
    {
        //  '*`this' copies the attributes from the caller
        return new Ellipse(*this);
    }


    //
    //CIRCLE
    //

    /// @brief Constructor that creates the Circle
    /// @param fill Color of the Circle
    /// @param center Point of the center of the Circle
    /// @param radius Integer value of the radius of the Circle
    Circle::Circle(const Color &fill, Point &center, const int &radius): Ellipse(fill), center(center), radius(radius) {};

    /// @brief Fetches the Circle radius
    /// @return The Circle radius
    int Circle::get_radius() const 
    {
        return radius;
    }

    /// @brief Virtual function of 'draw' overridden for the Circle object that calls the 'draw_ellipse' (since a circle is a ellipse) method in 'PNGImage.cpp'
    /// @param img The PNG Image in which the element will be drawn
    void Circle::draw(PNGImage &img) const 
    {

        //  Keep in mind that radius x = radius y in a circle
        Point rad = {radius, radius};
        img.draw_ellipse(center, rad, get_color());
    }

    /// @brief Virtual function of 'translate' overridden for the Circle object that calls the 'translate' method in 'Point.cpp'
    /// @param xy A Point of an element
    void Circle::translate(const Point &xy)
    {
        this->center = this->center.translate(xy);
    }

    /// @brief Virtual function of 'rotate' overridden for the Circle object that calls the 'rotate' method in 'Point.cpp'
    /// @param origin Origin of the rotation
    /// @param trans_scalar Rate of the rotation in degrees
    void Circle::rotate(Point &origin, int &trans_scalar) 
    {
        this->center = this->center.rotate(origin,trans_scalar);
    }

    /// @brief Virtual function of 'scale' overridden for the Circle object that changes the values of the radius and calls the 'scale' method in 'Point.cpp'
    /// @param origin Origin of the scale
    /// @param trans_scalar Factor of the scale
    void Circle::scale(Point &origin, int &trans_scalar) 
    {
        this->center = this->center.scale(origin,trans_scalar);
        this->radius = radius * trans_scalar;
    }

    /// @brief Virtual function of 'copy' overridden for the Circle object that creates a new object with the same attributes as the original caller
    /// @return New pointer to an Circle object with the same attributes as the original
    SVGElement* Circle::copy() 
    {
        return new Circle(*this);
    }


    //
    // POLYLINE 
    //

    /// @brief Constructor that creates the Polyline
    /// @param stroke Color of the stroke of the Polyline
    /// @param points Vector of Points that belong to the Polyline
    Polyline::Polyline(const Color &stroke, const std::vector<Point> &points) : stroke(stroke), points(points) {};

    /// @brief Specific constructor for the Line, using only the Stroke
    /// @param stroke Polyline strokes
    Polyline::Polyline(const Color &stroke) : stroke(stroke) {}; 

    /// @brief Fetches Polyline stroke
    /// @return The Polyline stroke
    Color Polyline::get_stroke() const
    {
        return stroke;
    }

    /// @brief Virtual function of 'draw' overridden for the Polyline object that calls the 'draw_line' (since a polyline is a line) method in 'PNGImage.cpp'
    /// @param img The PNG Image in which the element will be drawn
    void Polyline::draw(PNGImage &img) const
    {
        //  Assuming that the vector of points used in the constructor is not empty, there will always be an initial element
        Point current = points[0]; 
        
        //  In polyline, the way the points work is that the second point used in a given iteration will be the first point to be used in the next
        for (size_t i = 1; i < points.size(); i++){
            Point next = points[i]; 
            img.draw_line(current,next,get_stroke()); 
            current = next; 
        }
    }

    /// @brief Virtual function of 'translate' overridden for the Polyline object that calls the 'translate' method in 'Point.cpp'
    /// @param xy A Point of an element
    void Polyline::translate(const Point &xy) 
    {
        //  Create a new temporary vector and then updates the current 'points' vector, in order to avoid conflicts
        std::vector<Point> new_vec;
        
        //  Do the operation for every point in the Polyline
        for(size_t i = 0; i< points.size(); i++){
            Point new_p = points[i].translate(xy);
            new_vec.push_back(new_p);
        }
        this->points = new_vec;
    }

    /// @brief Virtual function of 'rotate' overridden for the Polyline object that calls the 'rotate' method in 'Point.cpp'
    /// @param origin Origin of the rotation
    /// @param trans_scalar Rate of the rotation in degrees
    void Polyline::rotate(Point &origin, int &trans_scalar) 
    {
        std::vector<Point> new_vec;

        for(size_t i = 0; i< points.size(); i++){
            Point new_p = points[i].rotate(origin,trans_scalar);
            new_vec.push_back(new_p);
        }
        this->points = new_vec;
    }

    /// @brief Virtual function of 'scale' overridden for the Polyline object that calls the 'scale' method in 'Point.cpp'
    /// @param origin Origin of the scale
    /// @param trans_scalar Factor of the scale
    void Polyline::scale(Point &origin, int &trans_scalar) 
    {
        std::vector<Point> new_vec;

        for(size_t i = 0; i< points.size(); i++){
            Point new_p = points[i].scale(origin,trans_scalar);
            new_vec.push_back(new_p);
        }
        this->points = new_vec;
    }

    /// @brief Virtual function of 'copy' overridden for the Polyline object that creates a new object with the same attributes as the original caller
    /// @return New pointer to an Polyline object with the same attributes as the original
    SVGElement* Polyline::copy() 
    {
        return new Polyline(*this);
    }


    //
    // LINE
    //

    /// @brief Constructor that creates the Line
    /// @param stroke Color of the stroke of the line
    /// @param start Point that indicates the beginning of the line
    /// @param end Point that indicates the end of the line
    Line::Line(const Color &stroke, const Point &start, const Point &end) : Polyline(stroke) , start(start), end(end) {};

    /// @brief Virtual function of 'draw' overridden for the Line object that calls the 'draw_line' method in 'PNGImage.cpp'
    /// @param img The PNG Image in which the element will be drawn
    void Line::draw(PNGImage &img) const 
    { 
        img.draw_line(start, end, get_stroke());
    }

    /// @brief Virtual function of 'translate' overridden for the Line object that calls the 'translate' method in 'Point.cpp'
    /// @param xy A Point of an element
    void Line::translate(const Point &xy) 
    {
        start = start.translate(xy);
        end = end.translate(xy);
    }

    /// @brief Virtual function of 'rotate' overridden for the Line object that calls the 'rotate' method in 'Point.cpp'
    /// @param origin Origin of the rotation
    /// @param trans_scalar Rate of the rotation in degrees
    void Line::rotate(Point &origin, int &trans_scalar) 
    {
        start = start.rotate(origin,trans_scalar);
        end = end.rotate(origin,trans_scalar);
    }

    /// @brief Virtual function of 'scale' overridden for the Line object that calls the 'scale' method in 'Point.cpp'
    /// @param origin Origin of the scale
    /// @param trans_scalar Factor of the scale
    void Line::scale(Point &origin, int &trans_scalar) 
    {
        start = start.scale(origin,trans_scalar);
        end = end.scale(origin,trans_scalar);
    }

    /// @brief Virtual function of 'copy' overridden for the Line object that creates a new object with the same attributes as the original caller
    /// @return New pointer to an Line object with the same attributes as the original
    SVGElement* Line::copy() 
    {
        return new Line(*this);
    }

    
    //
    // POLYGON
    //

    /// @brief Constructor that creates the Polygon
    /// @param fill Fill of the Polygon
    /// @param points Vector of Points that belong to the Polygon
    Polygon::Polygon(const Color &fill, const std::vector<Point> &points): fill(fill), points(points) {};

    /// @brief Specific constructor for the Polygon, using only the fill
    /// @param fill Polygon fill
    Polygon::Polygon(const Color &fill): fill(fill) {};

    /// @brief Fetch Polygon fill
    /// @return The Polygon fill
    Color Polygon::get_color() const
    {
        return fill;
    }

    /// @brief Virtual function of 'draw' overridden for the Polygon object that calls the 'draw_polygon' method in 'PNGImage.cpp'
    /// @param img The PNG Image in which the element will be drawn
    void Polygon::draw(PNGImage &img) const 
    {
        img.draw_polygon(points, fill);
    }

    /// @brief Virtual function of 'translate' overridden for the Polygon object that calls the 'translate' method in 'Point.cpp'
    /// @param xy A Point of an element
    void Polygon::translate(const Point &xy) 
    {
        std::vector<Point> new_vec;

        for(size_t i = 0; i< points.size(); i++){
            Point new_p = points[i].translate(xy);
            new_vec.push_back(new_p);
        }
        this->points = new_vec;
    }

    /// @brief Virtual function of 'rotate' overridden for the Polygon object that calls the 'rotate' method in 'Point.cpp'
    /// @param origin Origin of the rotation
    /// @param trans_scalar Rate of the rotation in degrees
    void Polygon::rotate(Point &origin, int &trans_scalar) 
    {
        std::vector<Point> new_vec;

        for(size_t i = 0; i< points.size(); i++){
            Point new_p = points[i].rotate(origin,trans_scalar);
            new_vec.push_back(new_p);
        }
        this->points = new_vec;
    }

    /// @brief Virtual function of 'scale' overridden for the Polygon object that calls the 'scale' method in 'Point.cpp'
    /// @param origin Origin of the scale
    /// @param trans_scalar Factor of the scale
    void Polygon::scale(Point &origin, int &trans_scalar) 
    {
        std::vector<Point> new_vec;

        for(size_t i = 0; i< points.size(); i++){
            Point new_p = points[i].scale(origin,trans_scalar);
            new_vec.push_back(new_p);
        }
        this->points = new_vec;
    }

    /// @brief Virtual function of 'copy' overridden for the Polygon object that creates a new object with the same attributes as the original caller
    /// @return New pointer to an Polygon object with the same attributes as the original
    SVGElement* Polygon::copy() 
    {
        return new Polygon(*this);
    }


    //
    // RECT
    //

    /// @brief Constructor that creates the Rectangle
    /// @param fill Color of the Rectangle
    /// @param points Vector of Points that belong to the Rectangle
    /// @param width Integer value of the width of the Rectangle
    /// @param heigth Integer value of the height of the Rectangle
    Rect::Rect(const Color &fill, const std::vector<Point> &points, const int &width, const int &height) : Polygon(fill), points(points), width(width), height(height) {};

    /// @brief Virtual function of 'draw' overridden for the Rect object that calls the 'draw_polygon' (since a rectangle is a polygon) method in 'PNGImage.cpp'
    /// @param img The PNG Image in which the element will be drawn
    void Rect::draw(PNGImage &img) const
    {
        img.draw_polygon(points,get_color());
    }

    /// @brief Virtual function of 'translate' overridden for the Rect object that calls the 'translate' method in 'Point.cpp'
    /// @param xy A Point of an element
    void Rect::translate(const Point &xy) 
    {
        std::vector<Point> new_vec;

        for(size_t i = 0; i< points.size(); i++){
            Point new_p = points[i].translate(xy);
            new_vec.push_back(new_p);
        }
        this->points = new_vec;
    }

    /// @brief Virtual function of 'rotate' overridden for the Rect object that calls the 'rotate' method in 'Point.cpp'
    /// @param origin Origin of the rotation
    /// @param trans_scalar Rate of the rotation in degrees
    void Rect::rotate(Point &origin, int &trans_scalar) 
    {   
        std::vector<Point> new_vec;

        for(size_t i = 0; i< points.size(); i++){
            Point new_p = points[i].rotate(origin,trans_scalar);
            new_vec.push_back(new_p);
        }
        this->points = new_vec;
    }

    /// @brief Virtual function of 'scale' overridden for the Rect object that calls the 'scale' method in 'Point.cpp'
    /// @param origin Origin of the scale
    /// @param trans_scalar Factor of the scale
    void Rect::scale(Point &origin, int &trans_scalar) 
    {
        std::vector<Point> new_vec;

        for(size_t i = 0; i< points.size(); i++){
            Point new_p = points[i].scale(origin,trans_scalar);
            new_vec.push_back(new_p);
        }
        this->points = new_vec;
    }

    /// @brief Virtual function of 'copy' overridden for the Rect object that creates a new object with the same attributes as the original caller
    /// @return New pointer to an Rect object with the same attributes as the original
    SVGElement* Rect::copy() 
    {
        return new Rect(*this);
    }


    //
    // GROUP
    //

    /// @brief Constructor of the Group
    /// @param elements Vector that stores pointers of type 'SVGElement' containing all of the elements in a given group
    Group::Group(const std::vector<SVGElement*> &elements): elements(elements) {};

    /// @brief Virtual function of 'draw' overridden for the Group object that calls the 'draw' method in 'PNGImage.cpp' for each element of type 'pointer of SVGElement' belonging to the 'elements' vector
    /// @param img The PNG Image in which the element will be drawn
    void Group::draw(PNGImage &img) const
    {
        for (SVGElement* element: this->elements)
        {
            element->draw(img);
        } 
    }

    /// @brief Virtual function of 'translate' overridden for the Group object that calls the 'translate' method in 'Point.cpp' for each element of type 'pointer of SVGElement' belonging to the 'elements' vector
    /// @param xy A Point of an element
    void Group::translate(const Point &xy) 
    {
        for (SVGElement* element: this->elements)
        {
            element->translate(xy);
        } 
    }

    /// @brief Virtual function of 'rotate' overridden for the Group object that calls the 'rotate' method in 'Point.cpp' for each element of type 'pointer of SVGElement' belonging to the 'elements' vector
    /// @param origin Origin of the rotation
    /// @param trans_scalar Rate of the rotation in degrees
    void Group::rotate(Point &origin, int &trans_scalar) 
    {   
        for (SVGElement* element: this->elements)
        {
            element->rotate(origin,trans_scalar);
        } 
    }

    /// @brief Virtual function of 'scale' overridden for the Group object that calls the 'scale' method in 'Point.cpp' for each element of type 'pointer of SVGElement' belonging to the 'elements' vector
    /// @param origin Origin of the scale
    /// @param trans_scalar Factor of the scale
    void Group::scale(Point &origin, int &trans_scalar) 
    {
        for (SVGElement* element: this->elements)
        {
            element->scale(origin,trans_scalar);
        } 
    }

    /// @brief Fetches the vector of the group
    /// @return The group vector
    std::vector<SVGElement*> Group::get_vector(){
        return elements;
    }   

    /// @brief Defines the destructor of the Group object 
    Group::~Group(){
        for (SVGElement* element: elements){
            delete element;
        }
        elements.clear();
    }

    /// @brief Virtual function of 'copy' overridden for the Group object that creates a new object with the same attributes as the original caller
    /// @return New pointer to an Group object with the same attributes as the original
    SVGElement* Group::copy() 
    {

        //  Create a new temporary vector and then updates the current 'elements' vector, in order to avoid conflicts
        std::vector<SVGElement*> new_elements;
        for (SVGElement* element: elements)
        {
            new_elements.push_back(element->copy());
        }
        return new Group(new_elements);
    }
}   