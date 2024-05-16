/// @file shape.hpp
#ifndef __svg_SVGElements_hpp__
#define __svg_SVGElements_hpp__

#include "Color.hpp"
#include "Point.hpp"
#include "PNGImage.hpp"

namespace svg
{
    class SVGElement
    {

    public:
        ///@brief Constructor that initiates the element with standard values
        SVGElement();

        ///@brief Destructor that deletes every element
        virtual ~SVGElement();

        ///@brief Performs the actual draw
        ///@param img Reference to the PNG Image in which the elements will be drawn
        virtual void draw(PNGImage &img) const = 0;

        /// @brief Peforms the given translation to the 'xy' Point
        /// @param xy A Point of an element
        virtual void translate(const Point &xy) = 0;

        /// @brief Performs the given rotation of the figure in question
        /// @param origin Origin of the rotation
        /// @param trans_scalar Rate of the rotation in degrees
        virtual void rotate(Point &origin, int &trans_scalar) = 0;

        /// @brief Performs the scale of the figure in question
        /// @param origin Origin of the scale
        /// @param trans_scalar Factor of the scale
        virtual void scale(Point &origin, int &trans_scalar) = 0;

        /// @brief Copy the elements of the 'SVGElements' pointer, allowing us to use them later, without changing the original elements  
        virtual SVGElement* copy() = 0;
    };

    ///@brief Main function that fetches the SVG/XML document in order to start reading and constructing its elements
    ///@param svg_file Name of the SVG file
    ///@param dimensions Width and height of the PNGImage that will be drawn
    ///@param svg_elements A vector of type 'pointer of SVGElement' that holds the constructors of all the elements in the SVG file
    void readSVG(const std::string &svg_file,
                 Point &dimensions,
                 std::vector<SVGElement *> &svg_elements);

    /// @brief Function that converts the SVG file into a PNG image
    /// @param svg_file Name of the SVG File
    /// @param png_file Name of the PNG Image
    void convert(const std::string &svg_file,
                 const std::string &png_file);


    //
    // ELLIPSE
    //
    class Ellipse : public SVGElement
    {
    public:
        /// @brief Constructor that creates the Ellipse
        /// @param fill Color of the Ellipse
        /// @param center Point of the center of the Ellipse
        /// @param radius Point of the radius of the Ellipse
        Ellipse(const Color &fill, Point &center, const Point &radius);

        /// @brief Specific constructor for the Circle, using only the color
        /// @param fill Color of the Ellipse
        Ellipse(const Color &fill);

        /// @brief Fetches the Ellipse color
        /// @return The Ellipse color
        Color get_color() const;

        /// @brief Fetches the Ellipse center
        /// @return The Ellipse center 
        Point get_center() const;

        /// @brief Fetches the Ellipse radius
        /// @return The Ellipse radius
        Point get_radius() const;

        /// @brief Virtual function of 'draw' overridden for the Ellipse object
        /// @param img The PNG Image in which the element will be drawn
        void draw(PNGImage &img) const override;

        /// @brief Virtual function of 'translate' overridden for the Ellipse object
        /// @param xy A Point of an element
        virtual void translate(const Point &xy) override;

        /// @brief Virtual function of 'rotate' overridden for the Ellipse object
        /// @param origin Origin of the rotation
        /// @param trans_scalar Rate of the rotation in degrees
        virtual void rotate(Point &origin, int &trans_scalar)  override;

        /// @brief Virtual function of 'scale' overridden for the Ellipse object
        /// @param origin Origin of the scale
        /// @param trans_scalar Factor of the scale
        virtual void scale(Point &origin, int &trans_scalar) override;

        /// @brief Virtual function of 'copy' overridden for the Ellipse object 
        virtual SVGElement* copy() override;

    private:
        Color fill;
        Point center;
        Point radius;
    };


    //
    // CIRCLE
    //
    class Circle : public Ellipse 
    {
    public:
        /// Constructor that creates the Circle
        /// @param fill Color of the Circle
        /// @param center Point of the center of the Circle
        /// @param radius Integer value of the radius of the Circle
        Circle(const Color &fill, Point &center, const int &radius);

        /// @brief Fetches the Circle radius
        /// @return The Circle radius
        int get_radius() const;

        /// @brief Virtual function of 'draw' overridden for the Circle object
        /// @param img The PNG Image in which the element will be drawn
        void draw(PNGImage &img) const override;

        /// @brief Virtual function of 'translate' overridden for the Circle object
        /// @param xy A Point of an element
        virtual void translate(const Point &xy) override;

        /// @brief Virtual function of 'rotate' overridden for the Circle object
        /// @param origin Origin of the rotation
        /// @param trans_scalar Rate of the rotation in degrees
        virtual void rotate(Point &origin, int &trans_scalar)  override;

        /// @brief Virtual function of 'scale' overridden for the Circle object
        /// @param origin Origin of the scale
        /// @param trans_scalar Factor of the scale
        virtual void scale(Point &origin, int &trans_scalar) override;

        /// @brief Virtual function of 'copy' overridden for the Circle object 
        virtual SVGElement* copy() override;

    private:
        Point center;
        int radius;
    };


    //
    // POLYLINE
    //
    class Polyline : public SVGElement
    {
    public:
        /// Constructor that creates the Polyline
        /// @param stroke Color of the stroke of the Polyline
        /// @param points Vector of Points that belong to the Polyline
        Polyline(const Color &stroke, const std::vector<Point> &points); 

        /// Specific constructor for the Line, using only the Stroke
        /// @param stroke Polyline stroke
        Polyline(const Color &stroke);

        /// Fetches Polyline stroke
        /// @return The Polyline stroke
        Color get_stroke() const;

        /// @brief Virtual function of 'draw' overridden for the Polyline object
        /// @param img The PNG Image in which the element will be drawn
        void draw(PNGImage &img) const override;

        /// @brief Virtual function of 'translate' overridden for the Polyline object
        /// @param xy A Point of an element
        virtual void translate(const Point &xy) override;

        /// @brief Virtual function of 'rotate' overridden for the Polyline object
        /// @param origin Origin of the rotation
        /// @param trans_scalar Rate of the rotation in degrees
        virtual void rotate(Point &origin, int &trans_scalar)  override;

        /// @brief Virtual function of 'scale' overridden for the Polyline object
        /// @param origin Origin of the scale
        /// @param trans_scalar Factor of the scale
        virtual void scale(Point &origin, int &trans_scalar) override;

        /// @brief Virtual function of 'copy' overridden for the Polyline object 
        virtual SVGElement* copy() override;

    private:
        Color stroke;
        std::vector<Point> points;   
    };


    //
    // LINE
    //
    class Line : public Polyline
    {
    public:
        /// Constructor that creates the Line
        /// @param stroke Color of the stroke of the line
        /// @param start Point that indicates the beginning of the line
        /// @param end Point that indicates the end of the line
        Line(const Color &stroke, const Point &start, const Point &end);
        
        /// @brief Virtual function of 'draw' overridden for the Line object
        /// @param img The PNG Image in which the element will be drawn
        void draw(PNGImage &img) const override;

        /// @brief Virtual function of 'translate' overridden for the Line object
        /// @param xy A Point of an element
        virtual void translate(const Point &xy) override;

        /// @brief Virtual function of 'rotate' overridden for the Line object
        /// @param origin Origin of the rotation
        /// @param trans_scalar Rate of the rotation in degrees
        virtual void rotate(Point &origin, int &trans_scalar)  override;

        /// @brief Virtual function of 'scale' overridden for the Line object
        /// @param origin Origin of the scale
        /// @param trans_scalar Factor of the scale
        virtual void scale(Point &origin, int &trans_scalar) override;

        /// @brief Virtual function of 'copy' overridden for the Line object 
        virtual SVGElement* copy() override;

    private:
        Point start, end;
    };


    //
    // POLYGON
    //
    class Polygon : public SVGElement
    {
    public:
        /// Constructor that creates the Polygon
        /// @param fill Fill of the Polygon
        /// @param points Vector of Points that belong to the Polygon
        Polygon(const Color &fill, const std::vector<Point> &points);

        /// Specific constructor for the Polygon, using only the fill
        /// @param fill Polygon fill
        Polygon(const Color &fill);

        /// Fetch Polygon fill
        /// @return The Polygon fill
        Color get_color() const;

        /// @brief Virtual function of 'draw' overridden for the Polygon object
        /// @param img The PNG Image in which the element will be drawn
        void draw(PNGImage &img) const override;

        /// @brief Virtual function of 'translate' overridden for the Polygon object
        /// @param xy A Point of an element
        virtual void translate(const Point &xy) override;

        /// @brief Virtual function of 'rotate' overridden for the Polygon object
        /// @param origin Origin of the rotation
        /// @param trans_scalar Rate of the rotation in degrees
        virtual void rotate(Point &origin, int &trans_scalar)  override;

        /// @brief Virtual function of 'scale' overridden for the Polygon object
        /// @param origin Origin of the scale
        /// @param trans_scalar Factor of the scale
        virtual void scale(Point &origin, int &trans_scalar) override;

        /// @brief Virtual function of 'copy' overridden for the Polygon object 
        virtual SVGElement* copy() override;

    private:
        Color fill;
        std::vector<Point> points;
    };


    //
    // RECTANGLE
    //
    class Rect : public Polygon
    {
    public:
        /// Constructor that creates the Rectangle
        /// @param fill Color of the Rectangle
        /// @param points Vector of Points that belong to the Rectangle
        /// @param width Integer value of the width of the Rectangle
        /// @param heigth Integer value of the height of the Rectangle
        Rect(const Color &fill, const std::vector<Point> &points, const int &width, const int &height);

        /// @brief Virtual function of 'draw' overridden for the Rectangle object
        /// @param img The PNG Image in which the element will be drawn
        void draw(PNGImage &img) const override;

        /// @brief Virtual function of 'translate' overridden for the Rectangle object
        /// @param xy A Point of an element
        virtual void translate(const Point &xy) override;

        /// @brief Virtual function of 'rotate' overridden for the Rectangle object
        /// @param origin Origin of the rotation
        /// @param trans_scalar Rate of the rotation in degrees
        virtual void rotate(Point &origin, int &trans_scalar)  override;

        /// @brief Virtual function of 'scale' overridden for the Rectangle object
        /// @param origin Origin of the scale
        /// @param trans_scalar Factor of the scale
        virtual void scale(Point &origin, int &trans_scalar) override;

        /// @brief Virtual function of 'copy' overridden for the Rectangle object 
        virtual SVGElement* copy() override;

    private:
        std::vector<Point> points;
        int width,height;
    };


    //
    // GROUP
    //
    class Group: public SVGElement
    {
    public:
        /// @brief Constructor of the Group
        /// @param elements Vector that stores pointers of type 'SVGElement' containing all of the elements in a given group
        Group(const std::vector<SVGElement*> &elements);

        /// Fetches the vector of the group 
        /// @return The group vector
        std::vector<SVGElement*> get_vector();

        /// @brief Virtual function of 'draw' overridden for the Group object
        /// @param img The PNG Image in which the element will be drawn
        void draw(PNGImage &img) const override;

        /// @brief Virtual function of 'translate' overridden for the Group object
        /// @param xy A Point of an element
        virtual void translate(const Point &xy) override;

        /// @brief Virtual function of 'rotate' overridden for the Group object
        /// @param origin Origin of the rotation
        /// @param trans_scalar Rate of the rotation in degrees
        virtual void rotate(Point &origin, int &trans_scalar)  override;

        /// @brief Virtual function of 'scale' overridden for the Group object
        /// @param origin Origin of the scale
        /// @param trans_scalar Factor of the scale
        virtual void scale(Point &origin, int &trans_scalar) override;

        /// @brief Virtual function of 'copy' overridden for the Group object 
        virtual SVGElement* copy() override;

        /// @brief Defines the destructor of the Group object 
        ~Group();

    private:
        std::vector<SVGElement*> elements;
    };
}   
#endif