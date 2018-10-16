#ifndef E_CANVAS

    #define E_CANVAS : 1
    #include <vector>
    #include <boost/geometry.hpp>
    #include <FL/fl_draw.H>

    namespace BG = boost::geometry;

    typedef BG::model::polygon<BG::model::d2::point_xy<int>> BGPolygon;
    typedef BG::model::d2::point_xy<int> BGPoint;


    #include "Definition.cpp"

#endif
