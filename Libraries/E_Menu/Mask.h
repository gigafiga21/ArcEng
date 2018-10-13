#ifndef E_MENU

    #define E_MENU : 1
    #include <vector>

    class E_Menu
    {
        private:

            int iLeft, iTop, iWidth, iHeight, iListTop, iButtonHeight, iDividerHeight, iDividersAmount;
            std::vector <E_MenuButton*> apCButtons;

        public:

            E_Menu(int iGivenLeft, int iGivenTop, int iGivenWidth, int iGivenHeight);
            void createButton(const char* sLabel);
            void createDivider();
    };

    #include "Definition.cpp"

#endif
