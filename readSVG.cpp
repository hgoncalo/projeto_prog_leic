
    #include <iostream>
    #include "SVGElements.hpp"
    #include "external/tinyxml2/tinyxml2.h"

    using namespace std;
    using namespace tinyxml2;

    namespace svg
    {
        //@author : hgoncalo
        //função read_elements para ir buscar cada elemento do xml
        //@todo : implementar parse para cores

        void read_elements(XMLElement *xml_elem, int indentation){
                for (int i = 0; i < indentation; i++){
                    std::cout << " ";
                }

                std::cout << xml_elem->Name() << " --> [";

                //unspecified behavoir
                //if (xml_elem->Name() == "ellipse"){
                //
                //}
                //else if (xml_elem->Name() == "ellipse"){
                //    
                //}

                //cada atributo do node/element
                for (const XMLAttribute *attr = xml_elem->FirstAttribute(); attr != nullptr; attr = attr->Next()){
                    std::cout << " " << attr->Name() << "=\"" << attr->Value() << "\"";
                }

                //filhos do element
                std::cout << " ] " << std::endl;
                for (XMLElement *child = xml_elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
                {
                    std::cout << "DAS IST CHILD : ";
                    svg::read_elements(child, indentation + 2);
                }
        }


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

            //implementação do read_elements
            svg::read_elements(xml_elem, 0);
            
        }

    }

