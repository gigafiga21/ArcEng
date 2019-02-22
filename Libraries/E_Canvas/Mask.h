#ifndef E_CANVAS

    #define E_CANVAS : 1
    #include <iostream>
    #include <vector>
    #include <cmath>
    #include <boost/geometry.hpp>
    #include <FL/fl_draw.H>

    namespace BG = boost::geometry;

    typedef BG::model::d2::point_xy<int> BGPoint;
    typedef BG::model::linestring<BGPoint> BGPolyLine;
    typedef BG::model::polygon<BGPoint> BGPolygon;
    typedef BG::model::multi_polygon<BGPolygon> BGMultiPolygon;
    typedef BG::model::box<BGPoint> BGRectangle;

    void print(BGMultiPolygon plgOut)
    {
        for (int iPolygon = 0; iPolygon < plgOut.size(); iPolygon++)
        {
            std::cout << std::endl << "Polygon " << iPolygon << std::endl;
            std::cout << "outer:" << std::endl;
            for (int iVertex = 0; iVertex < plgOut[iPolygon].outer().size(); iVertex++)
            {
                std::cout << plgOut[iPolygon].outer()[iVertex].x() << ' ' << plgOut[iPolygon].outer()[iVertex].y() << std::endl;
            }

            for (int iInner = 0; iInner < plgOut[iPolygon].inners().size(); iInner++)
            {
                std::cout << "inner area " << iInner << " :" << std::endl;
                for (int iVertex = 0; iVertex < plgOut[iPolygon].inners()[iInner].size(); iVertex++)
                {
                    std::cout << plgOut[iPolygon].inners()[iInner][iVertex].x() << ' ' << plgOut[iPolygon].inners()[iInner][iVertex].y() << std::endl;
                }
            }
        }
    }

    #include "Definition.cpp"

#endif
