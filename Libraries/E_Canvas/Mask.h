#ifndef E_CANVAS

    #define E_CANVAS : 1
    #include <vector>
    #include <boost/geometry.hpp>
    #include <FL/fl_draw.H>

    namespace BG = boost::geometry;

    typedef BG::model::d2::point_xy<int> BGPoint;
    typedef BG::model::linestring<BGPoint> BGPolyLine;
    typedef BG::model::polygon<BGPoint> BGPolygon;
    typedef BG::model::box<BGPoint> BGRectangle;

    #include "Definition.cpp"

#endif
