#pragma once
namespace Regards::Viewer
{
    // Accumulates progress counters and sends a single UI event per batch interval.
    // Replaces one AddPendingEvent per file with one event per N items or per timer tick.
    class CUIEventBatcher
    {
    public:
        explicit CUIEventBatcher(wxWindow* target, wxEventType progressEvent,
                                  int batchSize = 50)
            : m_target(target)
            , m_progressEvent(progressEvent)
            , m_batchSize(batchSize)
            , m_processed(0)
            , m_total(0)
        {}

        void SetTotal(int total)            { m_total = total; }

        // Call for every processed item. Fires an event only every m_batchSize items
        // or on the last item — no per-item sleep needed.
        void Tick(int currentIndex)
        {
            ++m_processed;
            bool isLast  = (currentIndex + 1) >= m_total;
            bool isDue   = (m_processed % m_batchSize == 0);

            if (isDue || isLast)
                Flush(currentIndex + 1);
        }

        // Force an immediate event (e.g. on early exit).
        void Flush(int processed = -1)
        {
            if (!m_target) return;
            wxCommandEvent evt(m_progressEvent);
            evt.SetInt(processed >= 0 ? processed : static_cast<int>(m_processed.load()));
            evt.SetString(wxString::Format("%d", m_total));
            m_target->GetEventHandler()->AddPendingEvent(evt);
        }

        // Send a one-shot status event with a custom string.
        void SendStatus(wxEventType type, const wxString& msg) const
        {
            if (!m_target) return;
            wxCommandEvent evt(type);
            evt.SetString(msg);
            m_target->GetEventHandler()->AddPendingEvent(evt);
        }

        // Send a one-shot event with no payload.
        void SendSignal(wxEventType type) const
        {
            if (!m_target) return;
            wxCommandEvent evt(type);
            m_target->GetEventHandler()->AddPendingEvent(evt);
        }

    private:
        wxWindow*    m_target;
        wxEventType  m_progressEvent;
        int          m_batchSize;
        std::atomic<int> m_processed;
        int          m_total;
    };
}
