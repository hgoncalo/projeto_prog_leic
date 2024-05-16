
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
        ///@brief Function that checks whether the transformation of the current element has a different origin than the default ({0,0})
        ///@param xml_elem A pointer of type 'XMLElement' pointing to the line of the current element in the XML file
        ///@return The origin as Point
        Point get_origin(XMLElement *xml_elem){

            if (xml_elem->Attribute("transform-origin") != nullptr)
            {
                std::vector<int> points;
                std::string transform_origin = xml_elem->Attribute("transform-origin");

                //  Usage of an 'istringstream' object to slice the 'transform-origin' string
                istringstream origin_iss(transform_origin);
                std::string current_point;
                while (origin_iss >> current_point)
                {
                    //  Adds the current integer value to the points vector
                    points.push_back(stoi(current_point));                 
                }
                //  As we only have two integer values in the vector, the origin shall be the first 2 elements
                return {points[0],points[1]};
            }
            else 
            {
                return {0,0};
            }
        }

        ///@brief Function that fetches any transform that the current element may have
        ///@param svg_ptr A pointer of type 'SVGElement' pointing to the constructor of the current element
        ///@param xml_elem A pointer of type 'XMLElement' pointing to the line of the current element in the XML file
        void child_transform(SVGElement* svg_ptr, XMLElement *xml_elem){

            std::string transform_str = xml_elem->Attribute("transform"); 

            //  Replaces any extising comma in the transform string with a blank space, as it is easier to fetch the values of the attributes using 'istringstream'
            std::replace(transform_str.begin(),transform_str.end(),',',' ');

            //  Obtains the boundaries of the class of the transform attribute  
            int first_par = transform_str.find_first_of("(");
            int last_par = transform_str.find_first_of(")");

            //  Obtains the class of the transform attribute
            std::string transform_class = transform_str.substr(0,first_par);
            std::cout << transform_class << '\n';

            // Fetches any value inside the transform attribute
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
                //  In an attempt to not be very verbose, the variable 'scalar' is also used as the angle for the 'rotate' transform
                int scalar = 0;
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
        
        ///@brief Function that facilitates the process of fetching Points
        ///@param pts_vtr Vector that will recieve all the Points of an element (passed by reference)
        ///@param pts String that contains the attribute "points" (separated by commas or blank spaces) of a given element
        ///@return An updated version of 'pts_vtr' with all the Points now in the vector, instead of being in the string
        std::vector<Point> get_points(std::vector<Point> &pts_vtr,std::string pts)
        {

            //  Used an 'istringstream' object to help fetch all the points in the string (either separated by commas or blank spaces)
            istringstream iss_pts(pts);
            string current;
            string id;
            while (iss_pts){
                iss_pts >> current;
                size_t comma = current.find_first_of(',');
                Point p = {stoi(current.substr(0,comma-current.length())),stoi(current.substr(comma+1))};
                pts_vtr.push_back(p);
            }
            return pts_vtr;
        }
        
        ///@brief Main function that parses through the SVG file, responsible for getting all the attributes and initializing the constructors of all elements 
        ///@param xml_elem A pointer of type 'XMLElement' pointing to the line of the current element in the XML file
        ///@param svg_elements A vector of type 'pointer of SVGElement' that holds the constructors of all the elements in the SVG file
        ///@param elements_map A map whose keys are of type 'string' and values of type 'pointer of SVGElement', holding the ID and the constructor of a given element, mainly used for the 'use' section
        void read_elements(XMLElement *xml_elem, vector<SVGElement *>& svg_elements, map<string, SVGElement*> &elements_map )
        {

            // 'if' statements that get the type of the current element

            if (strcmp(xml_elem->Name(),"ellipse") == 0)
            {
                //  Fetches the attributes of the current element (according to the attributes used by the constructor of such element)
                Color fill = parse_color(xml_elem->Attribute("fill"));
                Point center = {xml_elem->IntAttribute("cx"),xml_elem->IntAttribute("cy")};
                Point rad = {xml_elem->IntAttribute("rx"),xml_elem->IntAttribute("ry")};
                std::string id;

                //  Dynamically create a pointer of type 'SVGElement', later added to the 'svg_elements' vector
                Ellipse* elp_p = new Ellipse(fill,center,rad); 

                //  If the 'transform' attribute is not null, there must be something to transform
                if (xml_elem->Attribute("transform") != nullptr)
                {
                    child_transform(elp_p,xml_elem);
                }
                
                //  If the 'id' attribute is not null, there must be an 'id' (usually to be used in the 'use' section)
                if (xml_elem->Attribute("id") != nullptr)
                {
                    id = xml_elem->Attribute("id");
                    elements_map.insert({id,elp_p});
                }

                svg_elements.push_back(elp_p); 
            
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

                svg_elements.push_back(crc_p); 
            }

            else if (strcmp(xml_elem->Name(),"polyline") == 0)
            {
                Color stroke = parse_color(xml_elem->Attribute("stroke"));

                //  Defined a vector of Points to join all the points in the 'Polyline' element
                std::vector<Point> pts_vtr;
                std::string pts = xml_elem->Attribute("points");
                string id;

                get_points(pts_vtr,pts);
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

                svg_elements.push_back(pln_p); 
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

                svg_elements.push_back(lne_p);  
            }

            else if (strcmp(xml_elem->Name(),"polygon") == 0)
            {
                Color fill = parse_color(xml_elem->Attribute("fill"));
                std::vector<Point> pts_vtr;
                std::string pts = xml_elem->Attribute("points");
                string id;

                get_points(pts_vtr,pts);
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

                svg_elements.push_back(ply_p); 
            }

            else if (strcmp(xml_elem->Name(),"rect") == 0)
            {
                Color fill = parse_color(xml_elem->Attribute("fill"));
                int width = xml_elem->IntAttribute("width");
                int height = xml_elem->IntAttribute("height");
                Point start = {xml_elem->IntAttribute("x"),xml_elem->IntAttribute("y")};
                string id;
                std::vector<Point> rect_pts;
                
                //  Using the 'start' Point, we shall calculate all the remaining points of the rectangle

                //  (x,y) -> Start (Upper-left corner)
                //  (x+width-1,y) -> X1 (Upper-right corner)
                //  (x+width-1,y+height-1) -> X2 (Lower-right corner)
                //  (x,y+height-1) -> X3 (Lower-left corner)

                //  Then, push all these points into the 'rect_pts' vector 

                //  Observation : We used 'width-1' and 'height-1' because they are the limits of the image, as mentioned in the project descripton
                //  Basically, index 0 is counted as 1 pixel, therefore the subtraction mentioned above

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

                svg_elements.push_back(rec_p); 
            }

            else if (strcmp(xml_elem->Name(),"use") == 0)
            {
                std::string href = xml_elem->Attribute("href");
                std::string id;

                //  The sub-string is used to get the 'href' attribute with the "#"
                href = href.substr(1); 

                //  Defined an iterator to iterator over the map, trying to find the key that corresponds with the 'href' string
                auto iterator = elements_map.find(href);

                //  If the 'if' statement is true, than the iterator has found the right key
                if (iterator != elements_map.end()) 
                {
                    //  Call the copy method in order to duplicate the current element
                    SVGElement* new_element = (iterator->second)->copy();

                    if (xml_elem->Attribute("transform") != nullptr)
                    {
                        child_transform(new_element,xml_elem);
                    }
    
                    if (xml_elem->Attribute("id") != nullptr)
                    {
                        id = xml_elem->Attribute("id");
                        elements_map.insert({id,new_element});
                    }

                    svg_elements.push_back(new_element);
                }
            }
            
            //  Initialized the 'next_element' function, for further documentation seek the next definition of this function
            void next_element(XMLElement *xml_elem, vector<SVGElement *>& svg_elements, map<string, SVGElement*> &elements_map);

            next_element(xml_elem, svg_elements, elements_map);
        }

        ///@brief Function that fetches the next element in the SVG file
        ///@param xml_elem A pointer of type 'XMLElement' pointing to the line of the current element in the XML file
        ///@param svg_elements A vector of type 'pointer of SVGElement' that holds the constructors of all the elements in the SVG file
        ///@param elements_map A map whose keys are of type 'string' and values of type 'pointer of SVGElement', holding the ID and the constructor of a given element, mainly used for the 'use' section
        void next_element(XMLElement *xml_elem, vector<SVGElement *>& svg_elements, map<string, SVGElement*> &elements_map)
        {

            //  Get any children of the current 'xml_elem' (element)
            for (XMLElement *child = xml_elem->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
            {
                //  If any of these children is a group
                if (strcmp(child->Name(),"g") == 0){

                    //  'elements' is a vector of points of type 'SVGElement', it will be used to substitue 'svg_elements' during any recursive calls regarding groups
                    std::vector<SVGElement*> elements;
                    std::string id;

                    //  Recursively call the 'read_elements' function and check if the group has any children
                    //  If the child of the group is a group again, it will recursively fetch the group (as the child of the first group will be a group) and, thus, its elements (children of the child group)
                    //  Else, the call of the 'read_elements' function yields into an element, meaning that there are no children in the group, only elements exist!
                    //  If the call yields into an element, it will be placed in the 'elements' vector to then apply any pending transformations

                    //  Concrete example of this logic : 

                    //  read_ele(g1,ele,map) -> Call of the function
                    //  xml_ele = g1, child = g2, ... -> it is a group (recursive call again)

                    //  read_ele(g2,ele,map) -> Call of the function
                    //  xml_ele = g2, child = polyline, ... -> it is an element (stops and gets back to the main function, pushing the element to the vector)

                    svg::read_elements(child, elements, elements_map);
                    Group* grp_p = new Group(elements);

                    if (child->Attribute("transform") != nullptr){
                        child_transform(grp_p,child); 
                    }

                    if (child->Attribute("id") != nullptr)
                    {
                        id = child->Attribute("id");
                        elements_map.insert({id,grp_p});
                    }

                    svg_elements.push_back(grp_p); 
                }

                else {
                    svg::read_elements(child,svg_elements, elements_map);
                }
            }
        }
        
        ///@brief Main function that fetches the SVG/XML document in order to start reading and constructing its elements
        ///@param svg_file Name of the SVG file
        ///@param dimensions Width and height of the PNGImage that will be drawn
        ///@param svg_elements A vector of type 'pointer of SVGElement' that holds the constructors of all the elements in the SVG file
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

            //  First definition of the 'elements_map' vector, mentioned above
            map<string, SVGElement*> elements_map;

            //  Start reading the SVG file
            svg::read_elements(xml_elem, svg_elements, elements_map);
            
        }

    }