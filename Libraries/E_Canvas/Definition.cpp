#include <iostream>

class E_Canvas : public Fl_Widget
{
    private:

        int iStrokeStep, iLineWeight, iWallWeight,
            iLeft, iTop, iWidth, iHeight,
            iCanvasLeft, iCanvasTop, iCanvasWidth, iCanvasHeight;

        BGPolygon plgFlat;

        void strokeFlat()
        {
            fl_push_clip(iCanvasLeft, iCanvasTop, iCanvasWidth, iCanvasHeight);

            for (int iCounter = 0; iCounter < iCanvasWidth + iCanvasHeight; iCounter += iStrokeStep)
            {
                fl_line(iCanvasLeft + iCounter, iCanvasTop, iCanvasLeft, iCanvasTop + iCounter);
            }

            fl_pop_clip();
        }

        void drawFlatOuter()
        {
            std::vector<BGPoint> vptFlatOuter = plgFlat.outer();

            fl_color(E_COLOR1);
            fl_begin_complex_polygon();
            fl_vertex(iCanvasLeft - 1, iCanvasTop - 1);
            fl_vertex(iCanvasLeft + iCanvasWidth + 1, iCanvasTop - 1);
            fl_vertex(iCanvasLeft + iCanvasWidth + 1, iCanvasTop + iCanvasHeight + 1);
            fl_vertex(iCanvasLeft - 1, iCanvasTop + iCanvasHeight + 1);
            fl_vertex(iCanvasLeft - 1, iCanvasTop - 1);
            fl_gap();

            for (int iCounter = 0; iCounter < vptFlatOuter.size(); iCounter++)
            {
                fl_vertex(vptFlatOuter[iCounter].x() + iCanvasLeft, vptFlatOuter[iCounter].y() + iCanvasTop);
            }

            fl_end_complex_polygon();

            fl_color(E_COLOR3);
            fl_begin_loop();
            fl_vertex(iCanvasLeft, iCanvasTop);
            fl_vertex(iCanvasLeft + iCanvasWidth, iCanvasTop);
            fl_vertex(iCanvasLeft + iCanvasWidth, iCanvasTop + iCanvasHeight);
            fl_vertex(iCanvasLeft, iCanvasTop + iCanvasHeight);
            fl_vertex(iCanvasLeft, iCanvasTop);
            fl_end_loop();
        }

        void pushFlatInnersVerteces(bool bFilled)
        {
            for (int iInnerIndex = 0; iInnerIndex < plgFlat.inners().size(); iInnerIndex++)
            {
                std::vector<BGPoint> vptFlatInner = plgFlat.inners()[iInnerIndex];

                if (!bFilled)
                {
                    fl_begin_loop();
                }

                for (int iCounter = 0; iCounter < vptFlatInner.size(); iCounter++)
                {
                    fl_vertex(vptFlatInner[iCounter].x() + iCanvasLeft, vptFlatInner[iCounter].y() + iCanvasTop);
                }

                if (!bFilled)
                {
                    fl_end_loop();
                }
                else
                {
                    fl_gap();
                }
            }
        }

        void drawFlatInners()
        {
            fl_color(E_COLOR1);
            fl_begin_complex_polygon();
            pushFlatInnersVerteces(true);
            fl_end_complex_polygon();

            fl_color(E_COLOR3);
            pushFlatInnersVerteces(false);
        }

    public:

        E_Canvas(int iGivenLeft, int iGivenTop, int iGivenWidth, int iGivenHeight, const char* sGivenLabel) : Fl_Widget(
            iGivenLeft,
            iGivenTop,
            iGivenWidth,
            iGivenHeight,
            sGivenLabel)
        {
            iStrokeStep = 5;
            iLineWeight = 2;
            iWallWeight = 8;

            iLeft = iGivenLeft + iLineWeight;
            iTop = iGivenTop + iLineWeight;
            iWidth = iGivenWidth;
            iHeight = iGivenHeight;

            iCanvasLeft = iLeft + 30;
            iCanvasTop = (iHeight - 265) / 2;
            iCanvasWidth = iWidth - 60;
            iCanvasHeight = 265;

            BG::append(plgFlat.outer(), BGPoint(0, 0));
            BG::append(plgFlat.outer(), BGPoint(iCanvasWidth, 0));
            BG::append(plgFlat.outer(), BGPoint(iCanvasWidth, iCanvasHeight));
            BG::append(plgFlat.outer(), BGPoint(0, iCanvasHeight));
            BG::append(plgFlat.outer(), BGPoint(0, 0));

            plgFlat.inners().resize(1);
            BG::append(plgFlat.inners()[0], BGPoint(iWallWeight, iWallWeight));
            BG::append(plgFlat.inners()[0], BGPoint(iCanvasWidth - iWallWeight, iWallWeight));
            BG::append(plgFlat.inners()[0], BGPoint(iCanvasWidth - iWallWeight, iCanvasHeight - iWallWeight));
            BG::append(plgFlat.inners()[0], BGPoint(iWallWeight, iCanvasHeight - iWallWeight));
            BG::append(plgFlat.inners()[0], BGPoint(iWallWeight, iWallWeight));
        }

        void draw()
        {
            fl_line_style(FL_SOLID, iLineWeight, NULL);

            fl_color(E_COLOR3);
            strokeFlat();
            drawFlatOuter();
            drawFlatInners();
        }
};
