
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

        std::vector<std::string> attributes;

        //
        // IMPORTANTE !!!    
        // OBS : Será dump_att totalmente necessário ou podemos usar IntAttribute?
        // 

        void dump_att(XMLElement *xml_elem,std::vector<std::string> &attributes){
            //vetor alocado dinamicamente que devolve os atributos quando chamada (e elimina-lo no fim dos ifs)
            for (const XMLAttribute *attr = xml_elem->FirstAttribute(); attr != nullptr; attr = attr->Next()){
                std::cout << " " << attr->Name() << "=\"" << attr->Value() << "\""; //dar pushback ao valor
                attributes.push_back(attr->Value());
            }
        }

        void read_elements(XMLElement *xml_elem, int indentation)
        {
            for (int i = 0; i < indentation; i++){
                std::cout << " ";
            }
            std::cout << xml_elem->Name() << " --> [";

            //cada atributo do node/element
            dump_att(xml_elem,attributes);
            //std::cout << "Atributos: " << '\n';
            //for (auto &i: attributes){
            //    std::cout << '\n' << i << '\n';
            //}

            //atribuir os atributos certos a cada tipo
            if (strcmp(xml_elem->Name(),"ellipse") == 0)
            {
                //std::cout << '\n' << "[TEST] : ELLIPSE ";
                Color fill = parse_color(attributes[4]);
                Point center = {stoi(attributes[0]),stoi(attributes[1])};
                Point rad = {stoi(attributes[2]),stoi(attributes[3])};
                Ellipse elp(fill,center,rad);

                //alocar dinamicamente no vetor
                // [ERROR HERE] : std::vector<SVGElement *>& svg_elements = new std::vector<SVGElement *>[svg_elements.size()+1];

            }
            else if (strcmp(xml_elem->Name(),"circle") == 0)
            {
                //std::cout << '\n' << "[TEST] : CIRCLE ";           
            }
            else if (strcmp(xml_elem->Name(),"polyline") == 0)
            {
                //std::cout << '\n' << "[TEST] : POLYLINE ";     
            }
            else if (strcmp(xml_elem->Name(),"line") == 0)
            {
                //std::cout << '\n' << "[TEST] : LINE ";   
            }
            else if (strcmp(xml_elem->Name(),"polygon") == 0)
            {
                //std::cout << '\n' << "[TEST] : POLYGON ";  
            }
            else if (strcmp(xml_elem->Name(),"rect") == 0)
            {
                //std::cout << '\n' << "[TEST] : RECT ";
            }
            else {
                // ignore
            } 

            //empty vector before reading next element
            attributes = {};


            //filhos do element
            std::cout << " ] " << std::endl;
            for (XMLElement *child = xml_elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
            {
                //std::cout << "[TESTE] ELEMENTO ATUAL : ";
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
            svg_elements = {};
            svg::read_elements(xml_elem, 0);
            
        }

    }

