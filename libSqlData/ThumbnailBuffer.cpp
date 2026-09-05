#include "header.h"
#include "ThumbnailBuffer.h"
#include <ParamInit.h>
#include "RegardsConfigParam.h"
#include <ConvertUtility.h>
#include <iostream>
#include <wx/file.h>

CThumbnailBuffer::LruCache    CThumbnailBuffer::s_cache;
CThumbnailBuffer::VectorStore CThumbnailBuffer::s_store;

// ── LruCache ────────────────────────────────────────────────────────────────

cv::Mat CThumbnailBuffer::LruCache::get(const wxString& key)
{
    // Lecture optimiste en shared
    {
        std::shared_lock read(mutex);
        auto it = map.find(key);
        if (it == map.end())
            return {};
        // CORRECTION CRITIQUE : On retourne une copie de l'en-tête (incrémente le compteur de réf OpenCV)
        // pour figer les données et empêcher l'éviction LRU de détruire le buffer sous nos pieds.
        return it->second.first;
    }

    // Promotion LRU (exclusif)
    std::unique_lock write(mutex);
    auto it = map.find(key);
    if (it == map.end())
        return {};

    order.erase(it->second.second);
    order.push_back(key);
    it->second.second = std::prev(order.end());
    return it->second.first;
}

void CThumbnailBuffer::LruCache::put(const wxString& key, cv::Mat data)
{
    std::unique_lock write(mutex);

    auto it = map.find(key);
    if (it != map.end())
    {
        order.erase(it->second.second);
        order.push_back(key);
        it->second = { data, std::prev(order.end()) };
        return;
    }

    if (static_cast<int>(map.size()) >= maxSize)
    {
        auto oldest = order.front();
        order.pop_front();
        map.erase(oldest);
    }

    order.push_back(key);
    map[key] = { data, std::prev(order.end()) };
}

void CThumbnailBuffer::LruCache::remove(const wxString& key)
{
    std::unique_lock write(mutex);
    auto it = map.find(key);
    if (it == map.end()) return;

    order.erase(it->second.second);
    map.erase(it);
}

// ── GetPicture ───────────────────────────────────────────────────────────────

cv::Mat CThumbnailBuffer::GetPicture(const wxString& filename)
{
    int sizeBuffer = 100;
    if (auto* param = CParamInit::getInstance())
        sizeBuffer = param->GetBufferSize();

    if (sizeBuffer <= 0)
        return cv::imread(CConvertUtility::ConvertToStdString(filename).c_str(), cv::IMREAD_COLOR);

    s_cache.maxSize = sizeBuffer;

    // 1. Cherche dans le cache (Retourne un objet Mat incrémenté de manière atomique)
    cv::Mat raw = s_cache.get(filename);

    if (raw.empty())
    {
        cv::Mat loaded;
        if (wxFile::Exists(filename))
        {
            wxFile file(filename);
            if (file.IsOpened())
            {
                size_t fileSize = file.Length();
                loaded = cv::Mat(1, static_cast<int>(fileSize), CV_8UC1);
                file.Read(loaded.data, fileSize);
                file.Close();
            }
        }

        if (!loaded.empty())
        {
            s_cache.put(filename, loaded);
            raw = loaded;
        }
    }

    if (raw.empty())
        return cv::imread(CConvertUtility::ConvertToStdString(filename).c_str(), cv::IMREAD_COLOR);

    // Décodage 100% sécurisé (Même si l'entrée est évincée du cache, notre instance locale 'raw' possède son propre verrou)
    cv::Mat decoded = cv::imdecode(raw, cv::IMREAD_COLOR);
    if (decoded.empty())
        return cv::imread(CConvertUtility::ConvertToStdString(filename).c_str(), cv::IMREAD_COLOR);

    return decoded;
}

void CThumbnailBuffer::RemovePicture(const wxString& filename)
{
    s_cache.remove(filename);
}

void CThumbnailBuffer::InitVectorList(PhotosVector* newVector)
{
    std::unique_ptr<PhotosVector> incoming(newVector);
    std::unique_lock write(s_store.mutex);

    std::unordered_set<wxString> newIndex;
    newIndex.reserve(incoming->size());
    for (CPhotos& photo : *incoming)
        newIndex.insert(photo.GetPath());

    s_store.data = std::move(incoming);
    s_store.size = static_cast<int>(s_store.data->size());
    s_store.pathIndex = std::move(newIndex);
}

PhotosVector* CThumbnailBuffer::GetVectorList()
{
    std::shared_lock read(s_store.mutex);
    return s_store.data.get();
}

int CThumbnailBuffer::GetVectorSize()
{
    return s_store.size.load();
}

CPhotos CThumbnailBuffer::GetVectorValue(int i)
{
    std::shared_lock read(s_store.mutex);
    if (!s_store.data || i < 0 || i >= static_cast<int>(s_store.data->size()))
        throw std::out_of_range("GetVectorValue: index hors limites");
    return s_store.data->at(i);
}

wxString CThumbnailBuffer::FindPhotoByPath(const wxString& path)
{
    std::shared_lock read(s_store.mutex);
    if (!s_store.data) return {};
    return s_store.pathIndex.find(path) != s_store.pathIndex.end() ? path : wxString{};
}

bool CThumbnailBuffer::FindValidFile(const wxString& localFilename)
{
    std::shared_lock read(s_store.mutex);
    if (!s_store.data) return false;
    return s_store.pathIndex.find(localFilename) != s_store.pathIndex.end();
}

wxString CThumbnailBuffer::FindPhotoById(int id)
{
    std::shared_lock read(s_store.mutex);
    if (!s_store.data) return {};

    auto it = std::find_if(s_store.data->begin(), s_store.data->end(),
        [id](CPhotos& p) { return p.GetId() == id; });

    return (it != s_store.data->end()) ? it->GetPath() : wxString{};
}
