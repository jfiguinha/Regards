#pragma once
namespace Regards
{
    namespace Sqlite
    {
        class CSqlExecuteRequest;

        class CSqlTransaction
        {
        public:



            CSqlTransaction()
            {
                BeginTransaction();
            }

            ~CSqlTransaction()
            {
                if (!committed)
                    RollbackTransaction();
            }
            void commit()
            {
                CommitTransaction();
                committed = true;
            }

        private:


            void BeginTransaction();   // non-static, cohérent avec l'état membre
            void CommitTransaction();  // faute corrigée
            void RollbackTransaction();

            bool committed = false;
        };
    }
}