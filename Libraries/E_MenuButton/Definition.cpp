E_MenuButton::E_MenuButton(int iGivenLeft, int iGivenTop, int iGivenWidth, int iGivenHeight, const char* sGivenLabel) : Fl_Button(
    iGivenLeft,
    iGivenTop,
    iGivenWidth,
    iGivenHeight,
    sGivenLabel)
{
    iLeft = iGivenLeft;
    iTop = iGivenTop;
    iWidth = iGivenWidth;
    iHeight = iGivenHeight;
    sLabel = sGivenLabel;

    iFont = FL_HELVETICA;
    iFontSize = 15;
    iFontCorrectionSize = 5;
    iNormalFontColor = E_COLOR3;
    iActiveFontColor = E_COLOR0;

    iNormalBackgroundColor = E_COLOR1;
    iHoverBackgroundColor = E_COLOR2;
    iActiveBackgroundColor = E_COLOR3;

    iCurrentFontColor = iNormalFontColor;
    iCurrentBackgroundColor = iNormalBackgroundColor;
}

void E_MenuButton::draw()
{
    fl_color(iCurrentBackgroundColor);
    fl_rectf(iLeft, iTop, iWidth, iHeight);

    fl_color(iCurrentFontColor);
    fl_font(iFont, iFontSize);
    fl_draw(sLabel, iLeft + 20, iTop + (iHeight + iFontSize - iFontCorrectionSize) / 2);
}

int E_MenuButton::handle(int iEvent)
{
    switch(iEvent)
    {
        case FL_PUSH:

            iCurrentBackgroundColor = iActiveBackgroundColor;
            iCurrentFontColor = iActiveFontColor;
            redraw();
            return 1;

        case FL_RELEASE:

            iCurrentBackgroundColor = iHoverBackgroundColor;
            iCurrentFontColor = iNormalFontColor;
            redraw();
            return 1;

        case FL_ENTER:

            iCurrentBackgroundColor = iHoverBackgroundColor;
            redraw();
            return 1;

        case FL_LEAVE:

            iCurrentBackgroundColor = iNormalBackgroundColor;
            redraw();
            return 1;

    }
}
