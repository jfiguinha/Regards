// For compilers that support precompilation, includes "wx.h".
// For compilers that support precompilation, includes "wx.h".
#include "header.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "checktree.h"

#include "checked2.xpm"
#include "checked_d.xpm"
#include "checked_ld.xpm"
#include "checked_mo.xpm"
#include "unchecked2.xpm"
#include "unchecked_d.xpm"
#include "unchecked_ld.xpm"
#include "unchecked_mo.xpm"
#include <wx/icon.h>
#include <wx/imaglist.h>

wxDEFINE_EVENT(wxEVT_CHECKTREE_FOCUS, wxTreeEvent);
wxDEFINE_EVENT(wxEVT_CHECKTREE_CHOICE, wxTreeEvent);

IMPLEMENT_DYNAMIC_CLASS(wxCheckTree, wxTreeCtrl)

bool on_check_or_label(int flags)
{
    return flags&(wxTREE_HITTEST_ONITEMSTATEICON|wxTREE_HITTEST_ONITEMLABEL)?true:false;
}

void unhighlight( wxTreeCtrl* m_treeCtrl1, wxTreeItemId& id )
{
    if(id.IsOk())
    {
        int i=m_treeCtrl1->GetItemState(id);

        if(wxCheckTree::UNCHECKED<=i && i<wxCheckTree::UNCHECKED_DISABLED)
        {
            m_treeCtrl1->SetItemState(id,wxCheckTree::UNCHECKED);
        }
        else if(wxCheckTree::CHECKED<=i && i<wxCheckTree::CHECKED_DISABLED)
        {
            m_treeCtrl1->SetItemState(id,wxCheckTree::CHECKED);
        }
    }
}


void mohighlight( wxTreeCtrl* m_treeCtrl1,wxTreeItemId& id,bool toggle )
{
    if(id.IsOk())
    {    
        int i=m_treeCtrl1->GetItemState (id);

        if(!id.IsOk() || i < 0)
        {
            return;
        }

        bool is_checked;

        if(wxCheckTree::UNCHECKED<=i && i<wxCheckTree::UNCHECKED_DISABLED)
        {
            m_treeCtrl1->SetItemState (id,toggle?wxCheckTree::CHECKED_MOUSE_OVER:wxCheckTree::UNCHECKED_MOUSE_OVER);
            is_checked=true;
        }
        else if(wxCheckTree::CHECKED<=i && i<wxCheckTree::CHECKED_DISABLED)
        {
            m_treeCtrl1->SetItemState (id,toggle?wxCheckTree::UNCHECKED_MOUSE_OVER:wxCheckTree::CHECKED_MOUSE_OVER);
            is_checked=false;
        }

        if(toggle)
        {
            /*
            if (is_checked)
                m_treeCtrl1->SelectItem(id, true);
            else
                m_treeCtrl1->SelectItem(id, false);
            */
            m_treeCtrl1->SelectItem(id, true);
            wxTreeEvent event2(wxEVT_CHECKTREE_CHOICE,m_treeCtrl1,id);
            event2.SetExtraLong(is_checked?1:0);
            m_treeCtrl1->ProcessWindowEvent(event2);


        }
    }
}

void ldhighlight( wxTreeCtrl* m_treeCtrl1, wxTreeItemId& id )
{
    if(id.IsOk())
    {    
        int i=m_treeCtrl1->GetItemState (id);

        if(wxCheckTree::UNCHECKED<=i && i<wxCheckTree::UNCHECKED_DISABLED)
        {
            m_treeCtrl1->SetItemState (id,wxCheckTree::UNCHECKED_LEFT_DOWN);
        }
        else if(wxCheckTree::CHECKED<=i && i<wxCheckTree::CHECKED_DISABLED)
        {
            printf("ldhighlight CHECKED \n");
            m_treeCtrl1->SetItemState (id,wxCheckTree::CHECKED_LEFT_DOWN);
        }
    }
}

wxCheckTree::wxCheckTree():wxTreeCtrl(), last_mo(),last_ld(),last_kf(),mouse_entered_tree_with_left_down(false)
{

}

wxCheckTree::wxCheckTree(wxWindow *parent, const wxWindowID id,const wxPoint& pos, const wxSize& size,long style)
                : wxTreeCtrl(parent, id, pos, size, style)
                , last_mo(),last_ld(),last_kf(),mouse_entered_tree_with_left_down(false)
{
    Init();
}

wxCheckTree::~wxCheckTree()
{
    m_colors.clear();
}

void wxCheckTree::Init()
{
    wxImageList *states;

    wxIcon icons[8];
    icons[0] = wxIcon(unchecked2_xpm);
    icons[1] = wxIcon(unchecked_mo_xpm);
    icons[2] = wxIcon(unchecked_ld_xpm);
    icons[3] = wxIcon(unchecked_d_xpm);
    icons[4] = wxIcon(checked2_xpm);
    icons[5] = wxIcon(checked_mo_xpm);
    icons[6] = wxIcon(checked_ld_xpm);
    icons[7] = wxIcon(checked_d_xpm);

    int width  = icons[0].GetWidth(),
        height = icons[0].GetHeight();

    // Make an state image list containing small icons
    states = new wxImageList(width, height, true);

    for ( size_t i = 0; i < WXSIZEOF(icons); i++ )
        states->Add(icons[i]);

    AssignStateImageList(states);

	
    Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( wxCheckTree::On_Tree_Sel_Changed ), NULL, this );
	Connect( wxEVT_CHAR, wxKeyEventHandler( wxCheckTree::On_Char ), NULL, this );
	Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( wxCheckTree::On_KeyDown ), NULL, this );
	Connect( wxEVT_KEY_UP, wxKeyEventHandler( wxCheckTree::On_KeyUp ), NULL, this );
	Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( wxCheckTree::On_Mouse_Enter_Tree ), NULL, this );
	Connect( wxEVT_LEAVE_WINDOW, wxMouseEventHandler( wxCheckTree::On_Mouse_Leave_Tree ), NULL, this );
	//Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( wxCheckTree::On_Left_DClick ), NULL, this );
	Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCheckTree::On_Left_Down ), NULL, this );
	Connect( wxEVT_LEFT_UP, wxMouseEventHandler( wxCheckTree::On_Left_Up ), NULL, this );
	Connect( wxEVT_MOTION, wxMouseEventHandler( wxCheckTree::On_Mouse_Motion ), NULL, this );
	Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( wxCheckTree::On_Mouse_Wheel ), NULL, this );
    Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCheckTree::On_Tree_Focus_Set ), NULL, this );
	Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCheckTree::On_Tree_Focus_Lost ), NULL, this );
	
}

void wxCheckTree::SetItemTextColour(const wxTreeItemId &item, const wxColour &col)
{
    std::map<wxTreeItemId,wxColor>::iterator it=  m_colors.find(item);

    if( it==m_colors.end() )
    {
        m_colors.insert(std::pair<wxTreeItemId,wxColor>(item, col) );
    }
    else
    {
        m_colors[item]=col;
    }

    wxTreeCtrl::SetItemTextColour(item, col);
}

bool wxCheckTree::EnableCheckBox(const wxTreeItemId &item, bool enable)
{
    if( !item.IsOk() )
    {
        return false;
    }

    int i=GetItemState(item);

    if(i<0 || i>CHECKED_DISABLED)
    {
        return false;
    }
    else if(enable)
    {
        if(i==UNCHECKED_DISABLED)
        {
            SetItemState(item, UNCHECKED);
        }
        else if(i==CHECKED_DISABLED)
        {
            SetItemState(item, CHECKED);
        }

        std::map<wxTreeItemId,wxColor>::iterator it=m_colors.find(item);

        if( it!=m_colors.end() )
        {
            SetItemTextColour(item,it->second);
            m_colors.erase(it);
        }

        return true;
    }
    else
    {
        if( i==UNCHECKED_DISABLED || i==CHECKED_DISABLED )
        {
            //don't disable a second time or we'll lose the
            //text color information.
            return true;
        }

        if( i==UNCHECKED || i==UNCHECKED_MOUSE_OVER || i==UNCHECKED_LEFT_DOWN )
        {
            SetItemState(item, UNCHECKED_DISABLED);
        }
        else if( i==CHECKED || i==CHECKED_MOUSE_OVER || i==CHECKED_LEFT_DOWN )
        {
            SetItemState(item, CHECKED_DISABLED);
        }

        wxColor col=GetItemTextColour(item);

        SetItemTextColour( item, wxColour(161,161,146) );
        //SetItemTextColour( item, wxColour(255,255,255) );
        m_colors[item]=col;
        return true;
    }
}

bool wxCheckTree::DisableCheckBox(const wxTreeItemId &item)
{
    return EnableCheckBox(item,false);
}

void wxCheckTree::MakeCheckable(const wxTreeItemId &item,bool state)
{
    if( item.IsOk() )
    {
        int i=GetItemState(item);

        if(i<0 || i>CHECKED_DISABLED)
        {
            SetItemState(item, state?CHECKED:UNCHECKED);
        }
    }
}

void wxCheckTree::Check(const wxTreeItemId &item,bool state)
{
    if( item.IsOk() )
    {
        int old_state=GetItemState(item);

        if(UNCHECKED<=old_state && old_state<=CHECKED_DISABLED)
        {
            bool enab = (old_state==UNCHECKED_DISABLED || old_state==CHECKED_DISABLED)?false:true;
            int ch= enab?CHECKED:CHECKED_DISABLED;
            int unch= enab?UNCHECKED:UNCHECKED_DISABLED;
            int new_state=state?ch:unch;

            if( new_state!=old_state )
            {
                SetItemState(item, new_state);
            }
        }
    }
}

void wxCheckTree::Uncheck(const wxTreeItemId &item)
{
    Check(item,false);
}

void wxCheckTree::On_Tree_Sel_Changed( wxTreeEvent& event )
{
    wxTreeItemId id=event.GetItem () ;

    unhighlight(this, last_kf);
    mohighlight(this,id,false);
    last_kf=id;

    event.Skip();

}

void wxCheckTree::On_Char( wxKeyEvent& event )
{
    if( !GetSelection().IsOk() )
    {
        //If there is no selection, any keypress should just select the first item
        wxTreeItemIdValue cookie;
        SelectItem(HasFlag(wxTR_HIDE_ROOT)?GetFirstChild(GetRootItem(),cookie ):GetRootItem());

        return;
    }

    event.Skip();
}

void wxCheckTree::On_KeyDown( wxKeyEvent& event )
{
	/*
    if(event.GetKeyCode()==32)
    {
        ldhighlight( this, last_kf );
    }
	*/
    event.Skip();
}

void wxCheckTree::On_KeyUp( wxKeyEvent& event )
{
	/*
    if( event.GetKeyCode()==32 )
    {
        mohighlight(this,last_kf,true);
    }
    else if(event.GetKeyCode()==27)
    {
        unhighlight(this,last_kf);
        last_kf=wxTreeItemId();

        Unselect();
    }
	*/
    event.Skip();
}


void wxCheckTree::On_Mouse_Enter_Tree( wxMouseEvent& event )
{
    if(event.LeftIsDown())
    {
        mouse_entered_tree_with_left_down=true;
    }
}

void wxCheckTree::On_Mouse_Leave_Tree( wxMouseEvent& event )
{
    unhighlight( this, last_mo );
    unhighlight( this, last_ld );
    last_mo=wxTreeItemId();
    last_ld=wxTreeItemId();
}

void wxCheckTree::On_Left_DClick( wxMouseEvent& event )
{
    int flags;

    HitTest(event.GetPosition(),flags);

    //double clicks on buttons can be annoying, so we'll ignore those
    //but all other double clicks will just have 1 more click added to them

    //without this, the check boxes are not as responsive as they should be
    if( !(flags&wxTREE_HITTEST_ONITEMBUTTON) )
    {
        On_Left_Down( event );
        On_Left_Up( event );
    }
}

void wxCheckTree::On_Left_Down( wxMouseEvent& event )
{
    int flags;
    wxTreeItemId id=HitTest( event.GetPosition (), flags );
    int i=GetItemState(id);
    
    printf("wxCheckTree On_Left_Down\n");

    if( id.IsOk () && i>=0 && on_check_or_label(flags) )
    {
        last_ld=id;
        ldhighlight( this, id );
    }

}

void wxCheckTree::On_Left_Up( wxMouseEvent& event )
{
    SetFocus();

    int flags;
    wxTreeItemId id = HitTest( event.GetPosition (), flags );

    int i = GetItemState(id);

    if(mouse_entered_tree_with_left_down)
    {
        mouse_entered_tree_with_left_down=false;

        if( i>=0 && on_check_or_label(flags) )
        {
            mohighlight( this, id, false );
            last_mo=id;
        }
    }
    else if(id.IsOk ())
    {
        if( flags&wxTREE_HITTEST_ONITEMBUTTON && ItemHasChildren(id) )
        {
            if(IsExpanded(id))
            {
                Collapse(id);
            }
            else
            {
                Expand(id);
            }
        }
        else if( i>=0 && on_check_or_label(flags) )
        {
            if(id!=last_ld)
            {
                unhighlight(this,last_ld);
                mohighlight(this,id,false);
            }
            else
            {
                mohighlight(this,id,true);
            }

            last_ld=wxTreeItemId();
            last_mo=id;
        }
        else
        {
            unhighlight(this,last_ld);
            unhighlight(this,last_mo);
            last_ld=wxTreeItemId();
            last_mo=wxTreeItemId();
        }
    }
    else
    {
        //id is not ok
        unhighlight(this,last_ld);
        unhighlight(this,last_mo);
        last_ld=wxTreeItemId();
        last_mo=wxTreeItemId();
    }
}

void wxCheckTree::On_Mouse_Motion( wxMouseEvent& event )
{
    if(mouse_entered_tree_with_left_down)
    {
        //just ignore everything until the left button is released
        return;
    }

    int flags;
    wxTreeItemId id = HitTest( event.GetPosition(), flags );

    if(event.LeftIsDown())
    {
        //to match the behavior of ordinary check boxes,
        //if we've moved to a new item while holding the mouse button down
        //we want to set the item where the left down click occured to have
        //mouse over highlight.  And if we return to the box where the
        //left down occured, we want to return it to the having the left down highlight

        //I don't understand why this is the behavior
        //of ordinary check boxes, but I'm goin to match it anyway.

        if(id==last_ld)
        {
            int i=GetItemState(last_ld);

            if(i!=UNCHECKED_LEFT_DOWN&&i!=CHECKED_LEFT_DOWN)
            {
                ldhighlight(this,last_ld);
            }
        }
        else
        {
            mohighlight(this,last_ld,false);
        }
    }
    else if(!id.IsOk ())
    {
        unhighlight(this,last_mo);
        last_mo=wxTreeItemId();
    }
    else
    {
        //4 cases 1 we're still on the same item, but we've moved off the state icon or label
        //        2 we're still on the same item and on the state icon or label - do nothing
        //        3 we're on a new item but not on its state icon or label (or the new item has no state)
        //        4 we're on a new item, it has a state icon, and we're on the state icon or label

        int i=GetItemState(id);

        if(id==last_mo)
        {
            if( i<0 || !on_check_or_label(flags) )
            {
                unhighlight(this,last_mo);
                last_mo=wxTreeItemId();
            }
            else
            {
                //nothing
            }
        }
        else
        {
            if( i<0 || !on_check_or_label(flags) )
            {
                unhighlight(this,last_mo);
                last_mo=wxTreeItemId();
            }
            else
            {
                unhighlight(this,last_mo);
                mohighlight(this,id,false);
                last_mo=id;
            }
        }
    }
}

void wxCheckTree::On_Mouse_Wheel( wxMouseEvent& event )
{
    event.Skip();
}

void wxCheckTree::On_Tree_Focus_Set( wxFocusEvent& event )
{
    event.Skip();

    //skipping this event will set the last selected item
    //to be highlighted and I want the tree items to only be
    //highlighted by keyboard actions.
}

void wxCheckTree::On_Tree_Focus_Lost( wxFocusEvent& event )
{
    unhighlight(this,last_kf);
    Unselect();
    event.Skip();
}

void wxCheckTree::SetFocusFromKbd()
{
    if(last_kf.IsOk())
    {
        SelectItem(last_kf);
    }

    wxTreeEvent event2( wxEVT_CHECKTREE_FOCUS, this, wxTreeItemId() );
    ProcessWindowEvent(event2);

    wxWindow::SetFocusFromKbd();
}
