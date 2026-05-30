#pragma once
namespace Regards::Viewer
{
    // Lance le processus scanner externe (RegardsViewer -p RegardsPDF).
    class CScannerLauncher
    {
    public:
        CScannerLauncher() = default;

        // Démarre le scanner ; retourne 0 en cas de succès.
        int Launch();
    };
}
