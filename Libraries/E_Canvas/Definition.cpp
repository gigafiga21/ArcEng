typedef struct
{
    int x, y;
} Point;

class E_Canvas : public Fl_Widget
{
    private:

        int iStrokeStep, iLineWeight, iWallWeight,
            iLeft, iTop, iWidth, iHeight,
            iCanvasLeft, iCanvasTop, iCanvasWidth, iCanvasHeight;
        std::vector<Point> aBoundaryLines;

        void strokeRect(int iLeft, int iTop, int iWidth, int iHeight)
        {
            fl_push_clip(iLeft, iTop, iWidth, iHeight);

            for (int iCounter = 0; iCounter < iWidth + iHeight; iCounter += iStrokeStep)
            {
                fl_line(iLeft + iCounter, iTop, iLeft, iTop + iCounter);
            }

            fl_pop_clip();
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
            iWallWeight = 5;

            iLeft = iGivenLeft + iLineWeight;
            iTop = iGivenTop + iLineWeight;
            iWidth = iGivenWidth;
            iHeight = iGivenHeight;

            iCanvasLeft = iLeft + 30;
            iCanvasTop = (iHeight - 265) / 2;
            iCanvasWidth = iWidth - 60;
            iCanvasHeight = 265;

            aBoundaryLines.push_back({0, 0});
            aBoundaryLines.push_back({iCanvasWidth, 0});
            aBoundaryLines.push_back({iCanvasWidth, iCanvasHeight});
            aBoundaryLines.push_back({0, iCanvasHeight});

            aBoundaryLines.push_back({-1, -1});

            aBoundaryLines.push_back({iWallWeight, iWallWeight});
            aBoundaryLines.push_back({iCanvasWidth - iWallWeight, iWallWeight});
            aBoundaryLines.push_back({iCanvasWidth - iWallWeight, iCanvasHeight - iWallWeight});
            aBoundaryLines.push_back({iWallWeight, iCanvasHeight - iWallWeight});
        }

        void draw()
        {
            fl_color(E_COLOR3);
            fl_line_style(FL_SOLID, iWallWeight, NULL);

        }
};
