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

        //implementação das funções virtuais puras TRANSFORM,ROTATE,SCALE
        //OBS : Não há multiple transforms!
        virtual void translate(const Point &xy) = 0;
        virtual void rotate(Point &origin, int &trans_scalar)  = 0;
        virtual void scale(int &trans_scalar)  = 0;
    };

    // Declaration of namespace functions
    // readSVG -> implement it in readSVG.cpp
    // convert -> already given (DO NOT CHANGE) in convert.cpp

    void readSVG(const std::string &svg_file,
                 Point &dimensions,
                 std::vector<SVGElement *> &svg_elements);
    void convert(const std::string &svg_file,
                 const std::string &png_file);


    //
    // ELLIPSE
    //
    class Ellipse : public SVGElement
    {
    public:
        Ellipse(const Color &fill, Point &center, const Point &radius);
        Ellipse(const Color &fill);
        Color get_color() const;
        Point get_center() const;
        void draw(PNGImage &img) const override;

        //OBS: TRANSLATE NÃO PODE SER CONSTANTE, PORQUE ASSIM "PROMETE" QUE
        //NÃO ALTERA NENHUM MEMBRO DA CLASSE, QUANDO QUEREMOS ALTERAR O CENTER, P.EX
        virtual void translate(const Point &xy) override;
        virtual void rotate(Point &origin, int &trans_scalar)  override;
        virtual void scale(int &trans_scalar) override;

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
        Circle(const Color &fill, Point &center, const int &radius);
        int get_radius() const;
        void draw(PNGImage &img) const override;
        virtual void translate(const Point &xy) override;
        virtual void rotate(Point &origin, int &trans_scalar) override;
        virtual void scale(int &trans_scalar) override;

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
        //type of points? vetor de points? istringstream?
        //implementar logica de leitura de transformação string->vetor no readsvg?

        Polyline(const Color &stroke, const std::vector<Point> &points); 
        Polyline(const Color &stroke);
        Color get_stroke() const;
        void draw(PNGImage &img) const override;
        virtual void translate(const Point &xy) override;
        virtual void rotate(Point &origin, int &trans_scalar) override;
        virtual void scale(int &trans_scalar) override;

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
        //Sub-class que apenas recebe o stroke de Polyline
        //Ao invés de vetor, apresenta um ponto inicial e final

        Line(const Color &stroke, const Point &start, const Point &end);
        void draw(PNGImage &img) const override;
        virtual void translate(const Point &xy) override;
        virtual void rotate(Point &origin, int &trans_scalar) override;
        virtual void scale(int &trans_scalar) override;

    private:
        Point start, end;
    };


    //
    // POLYGON
    //
    class Polygon : public SVGElement
    {
    public:
        Polygon(const Color &fill, const std::vector<Point> &points);
        Polygon(const Color &fill);
        Color get_color() const;
        void draw(PNGImage &img) const override;
        virtual void translate(const Point &xy) override;
        virtual void rotate(Point &origin, int &trans_scalar) override;
        virtual void scale(int &trans_scalar) override;

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

        //OBS : No HPP define-se a estrutura
        //No CPP implementa-se cada elemento da classe
        
        Rect(const Color &fill, const Point &start, const int &width, const int &height);

        void draw(PNGImage &img) const override;
        virtual void translate(const Point &xy) override;
        virtual void rotate(Point &origin, int &trans_scalar) override;
        virtual void scale(int &trans_scalar) override;

    private:
        Point start;
        int width,height;
    };
}   
#endif
