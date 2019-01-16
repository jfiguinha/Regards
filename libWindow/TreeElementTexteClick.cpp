#include "TreeElementTexteClick.h"
#include <FileUtility.h>

CTreeElementTexteClick::CTreeElementTexteClick()
{

}


CTreeElementTexteClick::~CTreeElementTexteClick()
{

}

int CTreeElementTexteClick::GetTypeLink()
{
    return typeElement;
}

wxString CTreeElementTexteClick::GetLinkElement()
{
    return linkElement;
}

void CTreeElementTexteClick::SetTypeLink(const int &typeElement)
{
    this->typeElement = typeElement;
}

void CTreeElementTexteClick::SetLinkElement(const wxString &linkElement)
{
    this->linkElement = linkElement;
}

void CTreeElementTexteClick::ClickElement(wxWindow * window, const int &x, const int &y)
{
    switch (typeElement)
    {
            //Ouverture d'un fichier dans les ressources
        case 1:
        {
            wxString helpFile = CFileUtility::GetResourcesFolderPath();
            helpFile.Append("//" + linkElement);
            wxLaunchDefaultApplication(helpFile);
        }
            break;
            
            //Ouverture d'un lien URL
        case 2:
        {
            wxLaunchDefaultApplication(linkElement);
        }
            break;
            
        default:
            break;
    }
        
}
