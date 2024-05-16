
# Programming project

## Group elements

Identify all group elements (numbers and names).

- up202306531 Gonçalo Copek Braga Rodrigues
- up202304187 Helder Gonçalo Ferreira de Almeida e Oliveira 
- up202306852 Martim Rocha Ribas 


## Accomplished tasks

#### In the SVGElements.hpp/SVGElements.cpp files we've defined the following:
- Created (and updated) classes and subclasses for all elements (Ellipse, Circle, Polyline, Line, Polygon, Rect, Group)
- Defined functions that allows the caller to draw, translate, rotate, scale and copy an element
- Implemented all the logic behind those functions and wrote all the commentary and documentation behind our thinking

#### In the readSVG.cpp we've done the following:
- Implemented a sub-routine that fetches the current SVG file and reads through its elements (read_elements), while also working as one of the 'main' functions for our code
- Correctly parsed all elements and all their belonging attribute, including the usage of groups and the 'use' attribute, which required the addition of new pieces to our code, such as creating a map (defined in the 'read_svg' function) and fetching the 'href' and 'id' attributes
- Recursively implemented a sub-routine that allows the reading of the next element ('next_element' in the 'read_elements' function), which was useful for 'groups'
- Wrote auxiliary functions that successfully comprehend and realize the 'transform' and 'transform-origin' attributes (child_transform & get_origin)
- Simplified and removed some repeated parts of the code, specially the elements that use the logic behind the 'get_points' function
- Correctly used, allocated and deleted all the dynamic memory used along the program, avoiding memory leaks
- Wrote all the complementary documentation and reasoning behind our thinking