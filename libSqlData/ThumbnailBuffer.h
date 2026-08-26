#pragma once
#include "Photos.h"
#include <shared_mutex>
#include <unordered_set>

class CThumbnailBuffer
{
public:
    static cv::Mat    GetPicture(const wxString& filename);
    static void       RemovePicture(const wxString& filename);
    static void       InitVectorList(PhotosVector* newVector);
    static CPhotos    GetVectorValue(int i);
    static int        GetVectorSize();
    static bool       FindValidFile(const wxString& localFilename);
    static wxString   FindPhotoById(int id);
    static wxString   FindPhotoByPath(const wxString& path);
    static PhotosVector* GetVectorList();

private:
    // ── LRU cache ──────────────────────────────────────────────────────────
    // list conserve l'ordre LRU (front = le plus ancien)
    // unordered_map donne accès O(1) à l'itérateur dans la list
    struct LruCache
    {
        using ListIt = std::list<wxString>::iterator;

        std::unordered_map<wxString, std::pair<cv::Mat, ListIt>> map;
        std::list<wxString>                                       order;
        mutable std::shared_mutex                                 mutex;
        int                                                       maxSize = 100;

        // Retourne l'image encodée (raw) si présente, sinon Mat vide
        // Promotionne l'entrée en "most recently used"
        cv::Mat get(const wxString& key);

        // Insère ou met à jour ; évince le plus ancien si dépassement
        void put(const wxString& key, cv::Mat data);

        void remove(const wxString& key);
    };

    // ── PhotosVector ───────────────────────────────────────────────────────
    struct VectorStore
    {
        std::unique_ptr<PhotosVector> data;
        mutable std::shared_mutex     mutex;
        std::atomic<int>              size{ 0 };
        std::unordered_set<wxString> pathIndex; // nouveau : lookup O(1)
    };

    static LruCache    s_cache;
    static VectorStore s_store;
};