#include <wx/wx.h>

class Simple : public wxFrame
{
public:
    Simple(const wxString& title) 
        : wxFrame(NULL, wxID_ANY, title, wxPoint(30, 30), wxSize(800, 600))
	{
		Centre();
        m_button = new wxButton(this, wxID_ANY, "Click Me", wxPoint(10, 10), wxSize(150, 50));
        m_txt1 = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 70), wxSize(100, 30));
        m_list1 = new wxListBox(this, wxID_ANY, wxPoint(10, 110), wxSize(300, 300));
	}
private:
    wxButton* m_button = nullptr;
    wxTextCtrl* m_txt1 = nullptr;
    wxListBox* m_list1 = nullptr;
};

class MyApp : public wxApp
{
public:
    MyApp() {
        
    }

	bool OnInit()
	{
		Simple *simple = new Simple(wxT("Minesweeper"));
		simple->Show(true);
		return true;
	}
private:
};

wxIMPLEMENT_APP(MyApp);