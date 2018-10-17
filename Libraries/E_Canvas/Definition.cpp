#include <iostream>

class E_Canvas : public Fl_Widget
{
    private:

        int iStrokeStep, iLineWeight, iWebLineWeight, iWallWeight, iWebStep,
            iLeft, iTop, iWidth, iHeight,
            iCanvasLeft, iCanvasTop, iCanvasWidth, iCanvasHeight;

        BGPolygon plgFlat;

        void drawWebLines(BGPolygon* plgFlatInner, BGPolyLine* lnWebLine, int iHorisontal)
        {
            std::vector<BGPoint> vptClippedWebLine;
            BG::intersection(*lnWebLine, *plgFlatInner, vptClippedWebLine);

            for (int iCounterClipped = 0; iCounterClipped < vptClippedWebLine.size(); iCounterClipped += 2)
            {
                BGPoint ptA = vptClippedWebLine[iCounterClipped],
                        ptB = vptClippedWebLine[iCounterClipped + 1];
                int iDelta = iLineWeight / 2;

                if (iHorisontal)
                {
                    fl_line(ptA.x() + iCanvasLeft - iDelta, ptA.y() + iCanvasTop, ptB.x() + iCanvasLeft + iDelta, ptB.y() + iCanvasTop);
                }
                else
                {
                    fl_line(ptA.x() + iCanvasLeft, ptA.y() + iCanvasTop + iDelta, ptB.x() + iCanvasLeft, ptB.y() + iCanvasTop - iDelta);
                }
            }
        }

        void drawWeb()
        {
            for (int iInnerIndex = 0; iInnerIndex < plgFlat.inners().size(); iInnerIndex++)
            {
                BGPolygon plgFlatInner;
                BG::assign_points(plgFlatInner, plgFlat.inners()[iInnerIndex]);

                for (int iCounter = iWebStep; iCounter < iCanvasWidth; iCounter += iWebStep)
                {
                    BGPolyLine lnWebLine{BGPoint(iCounter, 0), BGPoint(iCounter, iCanvasHeight)};
                    drawWebLines(&plgFlatInner, &lnWebLine, 0);
                }

                for (int iCounter = iWebStep; iCounter < iCanvasHeight; iCounter += iWebStep)
                {
                    BGPolyLine lnWebLine{BGPoint(0, iCounter), BGPoint(iCanvasWidth, iCounter)};
                    drawWebLines(&plgFlatInner, &lnWebLine, 1);
                }
            }
        }

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

        void drawFlatInners(bool bFilled)
        {
            if (bFilled)
            {
                fl_begin_complex_polygon();
            }

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

            if (bFilled)
            {
                fl_end_complex_polygon();
            }
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
            iWebStep = 4;
            iWebLineWeight = 1;
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
            fl_color(E_COLOR3);
            fl_line_style(FL_SOLID, iLineWeight, NULL);
            strokeFlat();

            fl_color(E_COLOR1);
            drawFlatInners(true);

            fl_color(E_COLOR2);
            fl_line_style(FL_SOLID, iWebLineWeight, NULL);
            drawWeb();

            fl_color(E_COLOR3);
            fl_line_style(FL_SOLID, iLineWeight, NULL);
            drawFlatInners(false);

            fl_color(E_COLOR1);
            drawFlatOuter();
        }
};
