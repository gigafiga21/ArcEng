typedef struct
{
    int x, y;
} Point;

class E_Canvas : public Fl_Widget
{
    private:

        int iStrokeStep, iLineWeight, iBorder,
            iLeft, iTop, iWidth, iHeight,
            iCanvasLeft, iCanvasTop, iCanvasWidth, iCanvasHeight;
        vector<Point> aLines;

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
            iBorder = 10;

            iLeft = iGivenLeft + iLineWeight;
            iTop = iGivenTop + iLineWeight;
            iWidth = iGivenWidth;
            iHeight = iGivenHeight;

            iCanvasLeft = iLeft + 30;
            iCanvasTop = (iHeight - 265) / 2;
            iCanvasWidth = iWidth - 60;
            iCanvasHeight = 265;
        }

        void draw()
        {
            fl_color(E_COLOR3);
            fl_line_style(FL_SOLID, iLineWeight, NULL);

            fl_rect(iCanvasLeft, iCanvasTop, iCanvasWidth, iCanvasHeight);
            strokeRect(iCanvasLeft, iCanvasTop, iCanvasWidth, iCanvasHeight);
            fl_rectf(iCanvasLeft + iBorder, iCanvasTop + iBorder, iCanvasWidth - 2 * iBorder, iCanvasHeight - 2 * iBorder, E_COLOR1);

            fl_color(E_COLOR3);
            fl_rect(iCanvasLeft + iBorder, iCanvasTop + iBorder, iCanvasWidth - 2 * iBorder, iCanvasHeight - 2 * iBorder);
        }
};
