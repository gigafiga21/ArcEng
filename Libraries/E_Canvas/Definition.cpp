#include <iostream>
#include <vector>

class E_Canvas : public Fl_Widget
{
    private:

        int iStrokeStep, iLineWeight, iWebLineWeight, iWallWeight, iWebStep,
            iLeft, iTop, iWidth, iHeight,
            iCanvasLeft, iCanvasTop, iCanvasWidth, iCanvasHeight;

        BGPolygon plgFlat;
        BGPoint pntBinder;

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
            for (int iCounter = 0; iCounter < iCanvasWidth + iCanvasHeight; iCounter += iStrokeStep)
            {
                BGPolyLine plnLine;
                std::vector<BGPolyLine> aplnLines;

                BG::append(plnLine, BGPoint(iCounter, -1));
                BG::append(plnLine, BGPoint(-1, iCounter));
                BG::intersection(plgFlat, plnLine, aplnLines);

                for (int iLine = 0; iLine < aplnLines.size(); iLine++)
                {
                    fl_line(
                        iCanvasLeft + aplnLines[iLine][0].x(),
                        iCanvasTop + aplnLines[iLine][0].y(),
                        iCanvasLeft + aplnLines[iLine][1].x(),
                        iCanvasTop + aplnLines[iLine][1].y());
                }
            }
        }

        void drawFlat()
        {
            std::vector<BGPoint> apntOuter = plgFlat.outer();

            fl_begin_loop();
            for (int iVertex = 0; iVertex < apntOuter.size(); iVertex++)
            {
                fl_vertex(iCanvasLeft + apntOuter[iVertex].x(), iCanvasTop + apntOuter[iVertex].y());
            }
            fl_end_loop();

            for (int iInner = 0; iInner < plgFlat.inners().size(); iInner++)
            {
                std::vector<BGPoint> apntInner = plgFlat.inners()[iInner];

                fl_begin_loop();
                for (int iVertex = 0; iVertex < apntOuter.size(); iVertex++)
                {
                    fl_vertex(iCanvasLeft + apntInner[iVertex].x(), iCanvasTop + apntInner[iVertex].y());
                }
                fl_end_loop();
            }
        }

        bool updateBinder(int x, int y)
        {
            BGPolygon plgOuter;
            BGPoint pntMouse(x, y);

            for (int iCounter = 0; iCounter < plgFlat.outer().size(); iCounter++)
            {
                BG::append(plgOuter.outer(), plgFlat.outer()[iCounter]);
            }

            if (!BG::within(pntMouse, plgOuter))
            {
                return false;
            }

            bool changed = false;
            pntMouse.x(pntMouse.x() - pntMouse.x() % iWebStep);
            pntMouse.y(pntMouse.y() - pntMouse.y() % iWebStep);

            if (pntMouse.x() != pntBinder.x())
            {
                changed = true;
                pntBinder.x(pntMouse.x());
            }

            if (pntMouse.y() != pntBinder.y())
            {
                changed = true;
                pntBinder.y(pntMouse.y());
            }

            return changed;
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
            iWebStep = 16;
            iWebLineWeight = 1;
            iLineWeight = 2;
            iWallWeight = 8;

            iLeft = iGivenLeft + iLineWeight;
            iTop = iGivenTop + iLineWeight;
            iWidth = iGivenWidth;
            iHeight = iGivenHeight;

            iCanvasWidth = 384;
            iCanvasHeight = 280;
            iCanvasLeft = iLeft + (iWidth - iCanvasWidth) / 2;
            iCanvasTop = iTop + (iHeight - iCanvasHeight) / 2;

            pntBinder.x(-1);
            pntBinder.y(-1);

            BG::append(plgFlat.outer(), BGPoint(0, 0));
            BG::append(plgFlat.outer(), BGPoint(0, iCanvasHeight));
            BG::append(plgFlat.outer(), BGPoint(iCanvasWidth, iCanvasHeight));
            BG::append(plgFlat.outer(), BGPoint(iCanvasWidth, 0));
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
            drawFlat();

            /*fl_color(E_COLOR1);
            drawFlatInners(true);

            fl_color(E_COLOR2);
            fl_line_style(FL_SOLID, iWebLineWeight, NULL);
            drawWeb();

            fl_color(E_COLOR3);
            fl_line_style(FL_SOLID, iLineWeight, NULL);
            drawFlatInners(false);

            fl_color(E_COLOR1);
            drawFlatOuter();*/
        }

    protected:

        int handle(int iEvent)
        {
            switch(iEvent)
            {
                case FL_ENTER:
                    return 1;

                case FL_MOVE:
                    if (updateBinder(Fl::event_x(), Fl::event_y()))
                    {
                        redraw();
                    }
                    return 1;
            }
        }
};
