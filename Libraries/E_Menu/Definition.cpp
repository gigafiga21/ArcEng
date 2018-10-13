E_Menu::E_Menu(int iGivenLeft, int iGivenTop, int iGivenWidth, int iGivenHeight)
{
    iLeft = iGivenLeft;
    iTop = iGivenTop;
    iWidth = iGivenWidth;
    iHeight = iGivenHeight;

    iListTop = 107;
    iButtonHeight = 35;
    iDividerHeight = 10;

    iDividersAmount = 0;
}

void E_Menu::createButton(const char* sLabel)
{
    int iButtonTop = iTop + iListTop + iButtonHeight * apCButtons.size() + iDividerHeight * iDividersAmount;

    apCButtons.push_back(new E_MenuButton(iLeft, iButtonTop, iWidth, iButtonHeight, sLabel));
}

void E_Menu::createDivider()
{
    iDividersAmount++;
}
