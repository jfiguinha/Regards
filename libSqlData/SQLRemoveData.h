#pragma once
#include <Criteria.h>

namespace Regards
{
	namespace Sqlite
	{
        class CSQLRemoveData
        {
        public:
            // Classe utilitaire : pas d'instanciation
            CSQLRemoveData() = delete;

            // Retournent true si tout s'est bien passé
            static bool DeleteCatalog(int numCatalog);
            static bool DeleteFolder(int numFolder);
            static bool DeleteListPhoto(const std::vector<int>& listPhoto, CriteriaVector* criteriaVector);
            static bool DeletePhoto(int idPhoto);
            static bool DeleteFaceDatabase();

        private:
            // Logique face factorisée, réutilisée par DeleteCatalog et DeleteFaceDatabase
            static bool DeleteFaceData();
        };
	}
}
