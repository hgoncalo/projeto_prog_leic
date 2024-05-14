
    #include <iostream>
    #include "SVGElements.hpp"
    #include "external/tinyxml2/tinyxml2.h"

    //included for istringstream
    #include <sstream>

    //included for map
    #include <map>

    //included for replace
    #include <algorithm>
    #include <string>

    using namespace std;
    using namespace tinyxml2;

    namespace svg
    {
        //função read_elements para ir buscar cada elemento do xml

        //OBS : A função xml_elem->Attribute("nome") permite ir buscar o valor do atributo num determinado elemento
        //A variante ->IntAttribute permite ir buscar o valor inteiro associado a esse atributo

        // função auxiliar que permite ir buscar a origem do transform
        Point get_origin(XMLElement *xml_elem){
            //é (0,0) no caso do transform, caso haja transform-origin é o input do svg
            if (xml_elem->Attribute("transform-origin") != nullptr)
            {
                std::vector<int> points;
                std::string transform_origin = xml_elem->Attribute("transform-origin");
                istringstream origin_iss(transform_origin);
                std::string current_point;
                while (origin_iss >> current_point)
                {
                    points.push_back(stoi(current_point));                 
                }
                return {points[0],points[1]};
            }
            else 
            {
                return {0,0};
            }
        }

        // função auxiliar que permite fazer transform de um child element
        void child_transform(SVGElement* svg_ptr, XMLElement *xml_elem){
            //std::cout << '\n' << xml_elem->Name() << '\n';
            std::string transform_str = "";
            if (xml_elem->Attribute("transform") != nullptr){
                transform_str = xml_elem->Attribute("transform"); 
                //tirar as virgulas
                std::replace(transform_str.begin(),transform_str.end(),',',' ');
            }

            //obter os limites entre o transform e os pontos do transform     
            int first_par = transform_str.find_first_of("(");
            int last_par = transform_str.find_first_of(")");

            //obter transform class (translate,rotate,scale)
            std::string transform_class = transform_str.substr(0,first_par);
            std::cout << transform_class << '\n';

            //obter os atributos/pontos
            istringstream iss(transform_str.substr(first_par+1,last_par-(first_par+1)));
            std::string current;
            if (transform_class == "translate")
            {
                std::vector<int> current_pts;
                while(iss >> current)
                {
                    std::cout << '\n' << current << '\n';
                    current_pts.push_back(stoi(current));
                }
                Point iss_point = {current_pts[0],current_pts[1]};
                svg_ptr->translate(iss_point);
            }
            else if (transform_class == "rotate" || transform_class == "scale")
            {
                int scalar = 0;
                // chamada à função auxiliar para ir buscar a origem do transform
                Point origin = get_origin(xml_elem);
                while (iss >> current)
                {
                    scalar = stoi(current);
                }
               if (transform_class == "rotate")
               {
                    svg_ptr->rotate(origin,scalar);
               } 
               else {
                    svg_ptr->scale(origin,scalar);
               }

            }
        }

        void read_elements(XMLElement *xml_elem, int indentation,vector<SVGElement *>& svg_elements,map<string, SVGElement*> &elements_map )
        {
            for (int i = 0; i < indentation; i++){
                std::cout << " ";
            }
            std::cout << xml_elem->Name() << " --> [";

            //mapa para guardar ocorrencias dos objetos definido no parametro

            // Dar fetch ao objeto com o qual vamos trabalhar
            if (strcmp(xml_elem->Name(),"ellipse") == 0)
            {
                Color fill = parse_color(xml_elem->Attribute("fill"));
                Point center = {xml_elem->IntAttribute("cx"),xml_elem->IntAttribute("cy")};
                Point rad = {xml_elem->IntAttribute("rx"),xml_elem->IntAttribute("ry")};
                std::string id;

                //alocar dinamicamente!
                //svg_elements é um vetor que recebe objetos do tipo POINTER para SVGElement
                //OBJETIVO : alocar um pointer do tipo SVGElement no vetor svg_elements

                Ellipse* elp_p = new Ellipse(fill,center,rad); //criar um pointer para um novo objeto

                //se o transform não for nulo, há algo para dar transform
                if (xml_elem->Attribute("transform") != nullptr)
                {
                    child_transform(elp_p,xml_elem);
                }
                
                if (xml_elem->Attribute("id") != nullptr)
                {
                    id = xml_elem->Attribute("id");
                    elements_map.insert({id,elp_p});
                }
                svg_elements.push_back(elp_p); //alocá-lo no vetor acima mencionado
                //verificar se vetor é corretamente eliminado no fim (no memory leaks)
            
            }
            else if (strcmp(xml_elem->Name(),"circle") == 0)
            {
                Color fill = parse_color(xml_elem->Attribute("fill"));
                Point center = {xml_elem->IntAttribute("cx"),xml_elem->IntAttribute("cy")};
                int rad = xml_elem->IntAttribute("r");
                string id;

                Circle* crc_p = new Circle(fill,center,rad);
                if (xml_elem->Attribute("transform") != nullptr)
                {
                    child_transform(crc_p,xml_elem);
                }

                if (xml_elem->Attribute("id") != nullptr)
                {
                    id = xml_elem->Attribute("id");
                    elements_map.insert({id,crc_p});
                }
                svg_elements.push_back(crc_p); //alocá-lo no vetor acima mencionado   
            }
            else if (strcmp(xml_elem->Name(),"polyline") == 0)
            {
                Color stroke = parse_color(xml_elem->Attribute("stroke"));

                //fazer um vetor de pontos para transformar a string num conjunto válido de pontos
                //com o istringstream fazer slice de cada ponto (separado por virgulas)
                std::vector<Point> pts_vtr;
                std::string pts = xml_elem->Attribute("points");
                istringstream iss_pts(pts);
                string current;
                string id;

                //LÓGICA : A cada linha (que contem um ponto)
                //Encontrar as coordenadas X e Y (que estão separadas por uma vírgula)
                //O que está para trás da virgula será o X, para a frente será o Y
                //Com substr() encontram-se essas coordenadas, que são transformadas para int usando stoi()
                //Os pontos são depois colocados num vetor que será chamado pelo construtor do Polygon/Polyline
                while (iss_pts){
                    iss_pts >> current;
                    size_t comma = current.find_first_of(',');
                    Point p = {stoi(current.substr(0,comma-current.length())),stoi(current.substr(comma+1))};
                    pts_vtr.push_back(p);
                }

                Polyline* pln_p = new Polyline(stroke,pts_vtr);
                if (xml_elem->Attribute("transform") != nullptr)
                {
                    child_transform(pln_p,xml_elem);
                }
                                
                if (xml_elem->Attribute("id") != nullptr)
                {
                    id = xml_elem->Attribute("id");
                    elements_map.insert({id,pln_p});
                }
                svg_elements.push_back(pln_p); //alocá-lo no vetor acima mencionado
            }
            else if (strcmp(xml_elem->Name(),"line") == 0)
            {
                Color stroke = parse_color(xml_elem->Attribute("stroke"));
                Point start = {xml_elem->IntAttribute("x1"),xml_elem->IntAttribute("y1")};
                Point end = {xml_elem->IntAttribute("x2"),xml_elem->IntAttribute("y2")};
                string id;

                Line* lne_p = new Line(stroke,start,end);
                if (xml_elem->Attribute("transform") != nullptr)
                {
                    child_transform(lne_p,xml_elem);
                }
                                
                if (xml_elem->Attribute("id") != nullptr)
                {
                    id = xml_elem->Attribute("id");
                    elements_map.insert({id,lne_p});
                }
                svg_elements.push_back(lne_p); //alocá-lo no vetor acima mencionado
            }
            else if (strcmp(xml_elem->Name(),"polygon") == 0)
            {
                Color fill = parse_color(xml_elem->Attribute("fill"));
                std::vector<Point> pts_vtr;
                std::string pts = xml_elem->Attribute("points");
                istringstream iss_pts(pts);
                string current;
                string id;

                while (iss_pts){
                    iss_pts >> current;
                    size_t comma = current.find_first_of(',');
                    Point p = {stoi(current.substr(0,comma-current.length())),stoi(current.substr(comma+1))};
                    pts_vtr.push_back(p);
                }

                Polygon* ply_p = new Polygon(fill,pts_vtr);
                if (xml_elem->Attribute("transform") != nullptr)
                {
                    child_transform(ply_p,xml_elem);
                }
                                
                if (xml_elem->Attribute("id") != nullptr)
                {
                    id = xml_elem->Attribute("id");
                    elements_map.insert({id,ply_p});
                }
                svg_elements.push_back(ply_p); //alocá-lo no vetor acima mencionado
            }
            else if (strcmp(xml_elem->Name(),"rect") == 0)
            {
                Color fill = parse_color(xml_elem->Attribute("fill"));
                int width = xml_elem->IntAttribute("width");
                int height = xml_elem->IntAttribute("height");
                Point start = {xml_elem->IntAttribute("x"),xml_elem->IntAttribute("y")};
                string id;
                
                //obter todos os pontos do retângulo
                // (x,y) -> 0
                // (x+width,y) -> 1
                // (x,y+height) -> 3
                // (x+width,y+height) -> 2
                // meter num vetor de pontos
                std::vector<Point> rect_pts;

                //OBS : width-1 e height-1 são importantes, porque são os limites da figura!
                // precisamos de fazer -1 porque começamos no índice 0! (e não no 1)
                Point x1 = {start.x+width-1,start.y};
                Point x2 = {start.x+width-1,start.y+height-1};
                Point x3 = {start.x,start.y+height-1};
                rect_pts.push_back(start);
                rect_pts.push_back(x1);
                rect_pts.push_back(x2);
                rect_pts.push_back(x3);

                Rect* rec_p = new Rect(fill,rect_pts,width,height);
                if (xml_elem->Attribute("transform") != nullptr)
                {
                    child_transform(rec_p,xml_elem);
                }
                                
                if (xml_elem->Attribute("id") != nullptr)
                {
                    id = xml_elem->Attribute("id");
                    elements_map.insert({id,rec_p});
                }
                svg_elements.push_back(rec_p); //alocá-lo no vetor acima mencionado
            }
            else if (strcmp(xml_elem->Name(),"use") == 0)
            {
                std::string href = xml_elem->Attribute("href");
                std::string id;
                href = href.substr(1); //sem o #

                //se houver no mapa...             
                auto iterator = elements_map.find(href);
                if (iterator != elements_map.end()) //encontrou a chave
                {
                    //fazer copia
                    std::cout << '\n' << "ITERATOR FIRST: " << iterator->first << '\n';
                    SVGElement* new_element = (iterator->second)->copy();
                    child_transform(new_element,xml_elem);
                    svg_elements.push_back(new_element);
    
                    // use também pode ter id
                    if (xml_elem->Attribute("id") != nullptr)
                    {
                    id = xml_elem->Attribute("id");
                    elements_map.insert({id,new_element});
                    }
                }
            }

            void next_element(XMLElement *xml_elem, int indentation, vector<SVGElement *>& svg_elements, map<string, SVGElement*> &elements_map);
            next_element(xml_elem, indentation, svg_elements, elements_map);

            

            //
            // FETCH ATTRIBUTES (just for testing)
            //

            for (const XMLAttribute *attr = xml_elem->FirstAttribute(); attr != nullptr; attr = attr->Next())
            {
                std::cout << " " << attr->Name() << "=\"" << attr->Value() << "\"";
            }

            //filhos do element
            std::cout << " ] " << std::endl;
        
            //
            // END OF FETCH
            //
        }

        // função auxiliar que permite ver o próximo elemento
        void next_element(XMLElement *xml_elem, int indentation, vector<SVGElement *>& svg_elements, map<string, SVGElement*> &elements_map)
        {
            // os filhos do xml_elem
            for (XMLElement *child = xml_elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
            {
                // se algum for grupo
                if (strcmp(child->Name(),"g") == 0){
                    //std::cout << '\n' << child->Name() << '\n';
                    std::vector<SVGElement*> elements;
                    std::string id;

                    // chamar a função principal novamente (recursão)

                    //ex 6
                    // read_ele(g1,id+2,ele)
                    // xml_ele = g1, child = g2, ... é um grupo

                    // read_ele(g2,id+2,ele)
                    // xml_ele = g2, child = poly, já é um type

                    // se voltar a existir algum grupo, volta a chamar o next_element e faz as resp. transforms.
                    // caso contrário, calcula todos os elementos e faz a transform final
                    svg::read_elements(child, indentation + 2,elements, elements_map);

                    //vetor de pointers de svgelement
                    Group* grp_p = new Group(elements);
                    if (child->Attribute("transform") != nullptr){
                        child_transform(grp_p,child); //se houver transform, fazer transform/transform-origin
                    }

                    std::cout << '\n';
                    svg_elements.push_back(grp_p); //dar pushback a todos os elementos do grupo

                    if (child->Attribute("id") != nullptr)
                    {
                        id = child->Attribute("id");
                        elements_map.insert({id,grp_p});
                    }
                }
                else {
                    svg::read_elements(child, indentation + 2,svg_elements, elements_map);
                }
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
            //chamada de função principal implementada
            map<string, SVGElement*> elements_map;
            svg::read_elements(xml_elem, 0, svg_elements,elements_map);
            
        }

    }

