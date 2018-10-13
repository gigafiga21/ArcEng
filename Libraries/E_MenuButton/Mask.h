#ifndef E_MENUBUTTON

    #define E_MENUBUTTON : 1
    #include <FL/fl_draw.H>

    class E_MenuButton : public Fl_Button
    {
        private:


            int iWidth, iHeight, iLeft, iTop;
            int iFont, iFontSize, iFontCorrectionSize, iNormalFontColor, iActiveFontColor, iCurrentFontColor;
            int iNormalBackgroundColor, iHoverBackgroundColor, iActiveBackgroundColor, iCurrentBackgroundColor;
            const char* sLabel;

        public:

            E_MenuButton(int iGivenLeft, int iGivenTop, int iGivenWidth, int iGivenHeight, const char* sGivenLabel = 0);

        protected:

            virtual void draw();
            int handle(int iEvent);
    };

    #include "Definition.cpp"

#endif
