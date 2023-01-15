#ifndef checktree_H_INCLUDED
#define checktree_H_INCLUDED

#include "wx/treectrl.h"
#include <map>

// dll export macros
#ifdef WXMAKINGDLL_CHECKTREE
    #define WXDLLIMPEXP_CHECKTREE WXEXPORT
#elif defined(WXUSING_CHECKTREE_SOURCE)
    #define WXDLLIMPEXP_CHECKTREE
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_CHECKTREE WXIMPORT
#else // not making nor using DLL
#define WXDLLIMPEXP_CHECKTREE
#endif

class WXDLLIMPEXP_CHECKTREE wxCheckTree : public wxTreeCtrl
{
public:
	wxCheckTree(wxWindow* parent, wxWindowID id,
	            const wxPoint& pos, const wxSize& size,
	            long style);
	wxCheckTree();
	~wxCheckTree() override;

	void Init();

	//methods overriden from base class:
	void SetFocusFromKbd() override;
	void SetItemTextColour(const wxTreeItemId& item, const wxColour& col) override;

	//interaction with the check boxes:
	bool EnableCheckBox(const wxTreeItemId& item, bool enable = true);
	bool DisableCheckBox(const wxTreeItemId& item);
	void Check(const wxTreeItemId& item, bool state = true);
	void Uncheck(const wxTreeItemId& item);
	void MakeCheckable(const wxTreeItemId& item, bool state = false);

	enum
	{
		UNCHECKED,
		UNCHECKED_MOUSE_OVER,
		UNCHECKED_LEFT_DOWN,
		UNCHECKED_DISABLED,
		CHECKED,
		CHECKED_MOUSE_OVER,
		CHECKED_LEFT_DOWN,
		CHECKED_DISABLED
	};

private:
	//event handlers
	void On_Tree_Sel_Changed(wxTreeEvent& event);

	void On_Char(wxKeyEvent& event);
	void On_KeyDown(wxKeyEvent& event);
	void On_KeyUp(wxKeyEvent& event);

	void On_Mouse_Enter_Tree(wxMouseEvent& event);
	void On_Mouse_Leave_Tree(wxMouseEvent& event);
	void On_Left_DClick(wxMouseEvent& event);
	void On_Left_Down(wxMouseEvent& event);
	void On_Left_Up(wxMouseEvent& event);
	void On_Mouse_Motion(wxMouseEvent& event);
	void On_Mouse_Wheel(wxMouseEvent& event);

	void On_Tree_Focus_Set(wxFocusEvent& event);
	void On_Tree_Focus_Lost(wxFocusEvent& event);

	//private data:
	std::map<wxTreeItemId,wxColor> m_colors;

	bool mouse_entered_tree_with_left_down;

	wxTreeItemId last_mo;
	wxTreeItemId last_ld;
	wxTreeItemId last_kf;


	// NB: due to an ugly wxMSW hack you _must_ use DECLARE_DYNAMIC_CLASS()
	//     if you want your overloaded OnCompareItems() to be called.
	//     OTOH, if you don't want it you may omit the next line - this will
	//     make default (alphabetical) sorting much faster under wxMSW.
	DECLARE_DYNAMIC_CLASS(wxCheckTree)
};

wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_CHECKTREE, wxEVT_CHECKTREE_CHOICE, wxTreeEvent);
wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_CHECKTREE, wxEVT_CHECKTREE_FOCUS, wxTreeEvent);


#endif // checktree_H_INCLUDED
