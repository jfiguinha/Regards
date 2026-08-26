#pragma once

#include <PenDrawInfo.h>

#include <array>
#include <cstdint>

#include "Draw.h"

namespace Regards::FiltreEffet {
    class CCrop : public CDraw {
    public:
        CCrop();
        ~CCrop() override = default;

        void GetPos(wxRect& rc) override;

        void InitPoint(const long& m_lx, const long& m_ly, const long& m_lHScroll,
            const long& m_lVScroll, const float& ratio) override;

        void MouseMove(const long& xNewSize, const long& yNewSize,
            const long& m_lHScroll, const long& m_lVScroll,
            const float& ratio) override;

        void Dessiner(wxDC* deviceContext, const long& hScroll, const long& vScroll, const float& ratio,
            const wxColour& rgb, const wxColour& rgbFirst, const wxColour& rgbSecond, const int32_t& style) override;

        void Selection(const int32_t& xNewSize, const int32_t& yNewSize,
            const long& m_lHScroll, const long& m_lVScroll,
            const float& ratio) override;

    private:
        enum class SelectionType : int {
            Aucun = 0,
            Left = 1,
            Right = 2,
            Top = 3,
            TopLeft = 4,
            TopRight = 5,
            Bottom = 6,
            BottomLeft = 7,
            BottomRight = 8
        };

        static constexpr int HANDLE_SIZE = 6;
        static constexpr int HANDLE_HALF_SIZE = HANDLE_SIZE / 2;

        using SelectionPoints = std::array<CPenDrawInfo, 4>;

        void UpdateCursor() const;

        void DrawHandle(wxDC* deviceContext, const wxRect& rect, int x, int y,
            const wxColour& colour) const;

        wxRect GetDrawingRect(const long& hScroll, const long& vScroll,
            const float& ratio);

        SelectionPoints ptSelection{};
        SelectionType iSelect = SelectionType::Aucun;
    };
}  // namespace Regards::FiltreEffet