
#include <iostream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

namespace svg
{
    void readSVG(const string& svg_file, Point& dimensions, vector<SVGElement *>& svg_elements)
    {
        XMLDocument doc;
        XMLError r = doc.LoadFile(svg_file.c_str());
        if (r != XML_SUCCESS)
        {
            throw runtime_error("Unable to load " + svg_file);
        }
        XMLElement *xml_elem = doc.RootElement();

        dimensions.x = xml_elem->IntAttribute("width");
        dimensions.y = xml_elem->IntAttribute("height");
        
        // TODO complete code -->
        
        //implementar DOXYGEN
        
        //tal como está no xmldump
        //procurar nodes/siblings do svg file

        //transformar CX,CY em point? ao ler elipse
        //etc

        //definir tipos de SVGElement (elipse,linha,circulo,etc)
        //alocar nodes com a keyword NEW e as suas caracteristicas -> center,radius,fill,etc
        //NEW é usado para alocar dinamicamente no vetor SVG_ELEMENTS

        //vetor svg_elements é uma referência para um vetor do tipo SVGElement pointer
        //ou seja, para um vetor que aponta para objetos do tipo SVGElement
        //criar algum vetor temporário e alocar dinamicamente memoria?
        
    }
}