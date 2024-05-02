
    #include <iostream>
    #include "SVGElements.hpp"
    #include "external/tinyxml2/tinyxml2.h"

    //included for istringstream
    #include <sstream>

    using namespace std;
    using namespace tinyxml2;

    namespace svg
    {
        //@author : hgoncalo
        //função read_elements para ir buscar cada elemento do xml
        //@todo : implementar parse para cores

        //OBS : A função xml_elem->Attribute("nome") permite ir buscar o valor do atributo num determinado elemento
        //A variante ->IntAttribute permite ir buscar o valor inteiro associado a esse atributo

        void read_elements(XMLElement *xml_elem, int indentation,vector<SVGElement *>& svg_elements)
        {
            for (int i = 0; i < indentation; i++){
                std::cout << " ";
            }
            std::cout << xml_elem->Name() << " --> [";

            //atribuir os atributos certos a cada tipo
            if (strcmp(xml_elem->Name(),"ellipse") == 0)
            {
                //std::cout << '\n' << "[TEST] : ELLIPSE ";
                Color fill = parse_color(xml_elem->Attribute("fill"));
                Point center = {xml_elem->IntAttribute("cx"),xml_elem->IntAttribute("cy")};
                Point rad = {xml_elem->IntAttribute("rx"),xml_elem->IntAttribute("ry")};

                //alocar dinamicamente!
                //svg_elements é um vetor que recebe objetos do tipo POINTER para SVGElement
                //OBJETIVO : alocar um pointer do tipo SVGElement no vetor svg_elements

                Ellipse* elp_p = new Ellipse(fill,center,rad); //criar um pointer para um novo objeto
                svg_elements.push_back(elp_p); //alocá-lo no vetor acima mencionado
                //verificar se vetor é corretamente eliminado no fim (no memory leaks)
            
            }
            else if (strcmp(xml_elem->Name(),"circle") == 0)
            {
                //std::cout << '\n' << "[TEST] : CIRCLE "; 

                Color fill = parse_color(xml_elem->Attribute("fill"));
                Point center = {xml_elem->IntAttribute("cx"),xml_elem->IntAttribute("cy")};
                int rad = xml_elem->IntAttribute("r");

                Circle* crc_p = new Circle(fill,center,rad);
                svg_elements.push_back(crc_p);        
            }
            else if (strcmp(xml_elem->Name(),"polyline") == 0)
            {
                //std::cout << '\n' << "[TEST] : POLYLINE ";     
            }
            else if (strcmp(xml_elem->Name(),"line") == 0)
            {
                //std::cout << '\n' << "[TEST] : LINE ";   

                Color stroke = parse_color(xml_elem->Attribute("stroke"));
                Point start = {xml_elem->IntAttribute("x1"),xml_elem->IntAttribute("y1")};
                Point end = {xml_elem->IntAttribute("x2"),xml_elem->IntAttribute("y2")};

                Line* lne_p = new Line(stroke,start,end);
                svg_elements.push_back(lne_p);
            }
            else if (strcmp(xml_elem->Name(),"polygon") == 0)
            {
                //std::cout << '\n' << "[TEST] : POLYGON ";  
                Color fill = parse_color(xml_elem->Attribute("fill"));

                //fazer um vetor de pontos para transformar a string num conjunto válido de pontos
                //com o istringstream fazer slice de cada ponto (separado por virgulas)
                std::vector<Point> pts_vtr;
                std::string pts = xml_elem->Attribute("points");
                istringstream iss_pts(pts);
                string current;

                //LÓGICA : A cada linha (que contem um ponto)
                //Encontrar as coordenadas X e Y (que estão separadas por uma vírgula)
                //O que está para trás da virgula será o X, para a frente será o Y
                //Com substr() encontram-se essas coordenadas, que são transformadas para int usando stoi()
                //Os pontos são depois colocados num vetor que será chamado pelo construtor do Polygon
                while (iss_pts){
                    iss_pts >> current;
                    size_t comma = current.find_first_of(',');
                    Point p = {stoi(current.substr(0,comma-current.length())),stoi(current.substr(comma+1))};
                    pts_vtr.push_back(p);
                }

                Polygon* ply_p = new Polygon(fill,pts_vtr);
                svg_elements.push_back(ply_p);
            }
            else if (strcmp(xml_elem->Name(),"rect") == 0)
            {
                //std::cout << '\n' << "[TEST] : RECT ";

                //ir buscar os 4 pontos
                // (x,y) -> 0
                // (x+width,y) -> 1
                // (x,y+height) -> 2
                // (x+width,y+height) -> 3
                // meter num vetor de pontos
            }

            //filhos do element
            std::cout << " ] " << std::endl;
            for (XMLElement *child = xml_elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
            {
                //std::cout << "[TESTE] ELEMENTO ATUAL : ";
                svg::read_elements(child, indentation + 2,svg_elements);
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

            //implementar DOXYGEN

            //implementação do read_elements
            svg_elements = {};
            svg::read_elements(xml_elem, 0, svg_elements);
            
        }

    }

