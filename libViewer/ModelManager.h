#pragma once
#include <wx/wx.h>
#include <wx/progdlg.h>

namespace Regards::Viewer
{
    // Vérifie la présence et la version du modèle IA, et le télécharge si nécessaire.
    class CModelManager
    {
    public:
        explicit CModelManager(wxWindow* parent);

        // Retourne true si le modèle est valide et utilisable.
        bool VerifyAndUpdate();

    private:
        void NewModelsAvailable();
  
        wxWindow* parent_ = nullptr;
    };
}
