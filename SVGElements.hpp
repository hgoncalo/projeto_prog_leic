//! @file shape.hpp
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
        SVGElement();
        virtual ~SVGElement();
        virtual void draw(PNGImage &img) const = 0;
    };

    // Declaration of namespace functions
    // readSVG -> implement it in readSVG.cpp
    // convert -> already given (DO NOT CHANGE) in convert.cpp

    void readSVG(const std::string &svg_file,
                 Point &dimensions,
                 std::vector<SVGElement *> &svg_elements);
    void convert(const std::string &svg_file,
                 const std::string &png_file);

    //ellipse class
    class Ellipse : public SVGElement
    {
    public:
        Ellipse(const Color &fill, const Point &center, const Point &radius);
        void draw(PNGImage &img) const override;

    private:
        Color fill;
        Point center;
        Point radius;
    };

    //
    // Implementação de novas classes (Polyline/Polygon) e de novas subclasses (Circle/Line/Rectangle)
    //

    //polyline class
    class Polyline : public SVGElement
    {
    public:
        //type of points? vetor de points? istringstream?
        //implememas ntar logica de leitura de transformação string->vetor no readsvg?

        Polyline(const Color &stroke, const std::vector<Point> &points);
        void draw(PNGImage &img) const override;

    private:
        Color stroke;
        std::vector<Point> points;   
    };
    //line subclass, extends stroke but not points

    //POLYGON CLASS
    class Polygon : public SVGElement
    {
    public:
        Polygon(const Color &fill, const std::vector<Point> &points);
        void draw(PNGImage &img) const override;

    private:
        Color fill;
        std::vector<Point> points;
    };

    //rectangle SUB-class
    class Rect : public Polygon
    {
    public:
        Rect(const Color &fill, const std::vector<Point> &start, const int &width, const int &height) : Polygon(fill,start), width(width), height(height){};
        //atualizar o vetor do polygon???
        void draw(PNGImage &img) const override;

    private:
        int width,height;
    };
}   
#endif
