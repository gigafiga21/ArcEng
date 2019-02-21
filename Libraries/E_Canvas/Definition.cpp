#include <iostream>
#include <vector>
#include <cmath>

class E_Canvas : public Fl_Widget
{
    private:

        int iStrokeStep, iLineWeight, iWebLineWeight, iWallWeight, iWebStep, iBinderRadius,
            iLeft, iTop, iWidth, iHeight,
            iCanvasLeft, iCanvasTop, iCanvasWidth, iCanvasHeight;

        bool bDrawing;

        BGRectangle rctClip;
        BGPolygon plgFlat;
        BGPoint pntBinder, pntDrawingStart;

        void cropLine(BGPolygon plgPolygon, int x1, int y1, int x2, int y2)
        {
            BGPolyLine plnLine;
            std::vector<BGPolyLine> aplnLines;

            BG::append(plnLine, BGPoint(x1, y1));
            BG::append(plnLine, BGPoint(x2, y2));
            BG::intersection(plgPolygon, plnLine, aplnLines);

            for (int iLine = 0; iLine < aplnLines.size(); iLine++)
            {
                fl_line(
                    iCanvasLeft + aplnLines[iLine][0].x(),
                    iCanvasTop + aplnLines[iLine][0].y(),
                    iCanvasLeft + aplnLines[iLine][1].x(),
                    iCanvasTop + aplnLines[iLine][1].y());
            }
        }

        void drawWeb()
        {
            for (int iInner = 0; iInner < plgFlat.inners().size(); iInner++)
            {
                BGPolygon plgInner;
                for (int iCounter = plgFlat.inners()[iInner].size() - 1; iCounter >= 0 ; iCounter--)
                {
                    BG::append(plgInner.outer(), plgFlat.inners()[iInner][iCounter]);
                }

                for (int iCoord = 0; iCoord < iCanvasWidth; iCoord += iWebStep)
                {
                    cropLine(plgInner, iCoord, iLineWeight, iCoord, iCanvasHeight - 2 * iLineWeight);
                }

                for (int iCoord = 0; iCoord < iCanvasHeight - 2 * iLineWeight; iCoord += iWebStep)
                {
                    cropLine(plgInner, iLineWeight, iCoord, iCanvasWidth - iLineWeight, iCoord);
                }
            }
        }

        void strokeFlat()
        {
            for (int iCounter = 0; iCounter < iCanvasWidth + iCanvasHeight; iCounter += iStrokeStep)
            {
                cropLine(plgFlat, iCounter, 0, 0, iCounter);
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

        bool pointerInCanvas(BGPoint pntPointer)
        {
            BGPolygon plgOuter;

            for (int iCounter = 0; iCounter < plgFlat.outer().size(); iCounter++)
            {
                BG::append(plgOuter.outer(), plgFlat.outer()[iCounter]);
            }

            if (BG::within(pntPointer, plgOuter))
            {
                return true;
            }

            return false;
        }

        bool updateBinder(int x, int y)
        {
            BGPoint pntMouse(x - iCanvasLeft, y - iCanvasTop);

            if (!pointerInCanvas(pntMouse))
            {
                return false;
            }

            bool changed = false;
            pntMouse.x(round((float)pntMouse.x() / iWebStep) * iWebStep);
            pntMouse.y(round((float)pntMouse.y() / iWebStep) * iWebStep);

            if (pntMouse.x() != pntBinder.x() || pntMouse.y() != pntBinder.y())
            {
                changed = true;
                rctClip.min_corner().x(pntBinder.x() - iBinderRadius);
                rctClip.min_corner().y(pntBinder.y() - iBinderRadius);
                rctClip.max_corner().x(pntBinder.x() + iBinderRadius + iLineWeight);
                rctClip.max_corner().y(pntBinder.y() + iBinderRadius + iLineWeight);

                if (bDrawing && pntMouse.x() - pntDrawingStart.x() != 0)
                {
                    if (abs((pntMouse.y() - pntDrawingStart.y()) / (pntMouse.x() - pntDrawingStart.x())) > 1)
                    {
                        if (pntMouse.y() == pntBinder.y())
                        {
                            return false;
                        }
                        pntBinder.x(pntDrawingStart.x());
                        pntBinder.y(pntMouse.y());
                    }
                    else
                    {
                        if (pntMouse.x() == pntBinder.x())
                        {
                            return false;
                        }
                        pntBinder.y(pntDrawingStart.y());
                        pntBinder.x(pntMouse.x());
                    }
                }
                else
                {
                    pntBinder.x(pntMouse.x());
                    pntBinder.y(pntMouse.y());
                }
            }

            return changed;
        }

        void drawBinder(BGPoint pntTarget)
        {
            fl_begin_complex_polygon();
            fl_vertex(iCanvasLeft + pntTarget.x() - iBinderRadius, iCanvasTop + pntTarget.y());
            fl_vertex(iCanvasLeft + pntTarget.x(), iCanvasTop + pntTarget.y() - iBinderRadius);
            fl_vertex(iCanvasLeft + pntTarget.x() + iBinderRadius, iCanvasTop + pntTarget.y());
            fl_vertex(iCanvasLeft + pntTarget.x(), iCanvasTop + pntTarget.y() + iBinderRadius);
            fl_end_complex_polygon();
        }

        void freeClip()
        {
            rctClip.min_corner().x(-iLineWeight);
            rctClip.min_corner().y(-iLineWeight);
            rctClip.max_corner().x(iCanvasWidth + iLineWeight);
            rctClip.max_corner().y(iCanvasHeight + iLineWeight);
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
            iBinderRadius = 3;

            iLeft = iGivenLeft + iLineWeight;
            iTop = iGivenTop + iLineWeight;
            iWidth = iGivenWidth;
            iHeight = iGivenHeight;

            iCanvasWidth = 384;
            iCanvasHeight = 280;
            iCanvasLeft = iLeft + (iWidth - iCanvasWidth) / 2;
            iCanvasTop = iTop + (iHeight - iCanvasHeight) / 2;

            bDrawing = false;

            pntBinder.x(-1);
            pntBinder.y(-1);

            freeClip();

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
            fl_push_clip(
                rctClip.min_corner().x() + iCanvasLeft,
                rctClip.min_corner().y() + iCanvasTop,
                rctClip.max_corner().x() - rctClip.min_corner().x(),
                rctClip.max_corner().y() - rctClip.min_corner().y());

            fl_color(E_COLOR1);
            fl_rectf(
                iCanvasLeft - iBinderRadius,
                iCanvasTop - iBinderRadius,
                iCanvasLeft + iCanvasWidth + iBinderRadius,
                iCanvasTop + iCanvasHeight + iBinderRadius);

            fl_color(E_COLOR2);
            fl_line_style(FL_SOLID, iWebLineWeight, NULL);
            drawWeb();

            fl_color(E_COLOR3);
            fl_line_style(FL_SOLID, iLineWeight, NULL);
            strokeFlat();
            drawFlat();

            fl_pop_clip();

            if (bDrawing)
            {
                fl_color(E_COLOR3);
                drawBinder(pntBinder);
                drawBinder(pntDrawingStart);
            }
            else
            {
                fl_color(E_COLOR4);
                drawBinder(pntBinder);
            }
        }

    protected:

        int handle(int iEvent)
        {
            switch(iEvent)
            {
                case FL_ENTER:
                    return 1;

                case FL_PUSH:
                    if (!pointerInCanvas(BGPoint(Fl::event_x() - iCanvasLeft, Fl::event_y() - iCanvasTop)))
                    {
                        return 0;
                    }

                    bDrawing = true;
                    pntDrawingStart.x(pntBinder.x());
                    pntDrawingStart.y(pntBinder.y());
                    redraw();
                    return 1;

                case FL_DRAG:
                    if (updateBinder(Fl::event_x(), Fl::event_y()))
                    {
                        redraw();
                    }
                    return 0;

                case FL_RELEASE:
                    bDrawing = false;
                    freeClip();
                    redraw();

                case FL_MOVE:
                    if (updateBinder(Fl::event_x(), Fl::event_y()))
                    {
                        redraw();
                    }
                    return 1;
            }
        }
};
