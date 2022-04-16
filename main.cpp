#include <wx/wx.h>

class Simple : public wxFrame
{
public:
    Simple(const wxString& title) 
        : wxFrame(NULL, wxID_ANY, title, wxPoint(30, 30), wxSize(800, 600))
	{
        btns = new wxButton*[nFieldWidth * nFieldHeight];
        wxGridSizer* grid = new wxGridSizer(nFieldWidth, nFieldHeight, 0, 0);
        for (int i = 0; i < nFieldWidth*nFieldHeight; i++) {
            btns[i] = new wxButton(this, 1000 + i, " ");
            grid->Add(btns[i], 1, wxEXPAND | wxALL);
            btns[i]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Simple::on_button_clicked, this);
        }
        this->SetSizer(grid);
        grid->Layout();
        Centre();
	}

    void on_button_clicked(xwCommandEvent& evt) {
        int x = (evt.GetId() - 10000) % nFieldWidth;
        int y = (evt.GetId() - 10000) / nFieldWidth;
        
        evt.Skip();
    }

    ~Simple() {
        delete[] btns;
    }
    
private:
    int nFieldHeight = 10;
    int nFieldWidth = 10;
    wxButton** btns = nullptr;


    wxDECLARE_EVENT_TABLE();
};

/*wxBEGIN_EVENT_TABLE(Simple, wxFrame)
    EVT_BUTTON(10001, Simple::onButtonClicked)
    EVT_LISTBOX(10003, Simple::onKeyPress)
    EVT_BUTTON(10004, Simple::deleteClick)
wxEND_EVENT_TABLE()*/

class MyApp : public wxApp
{
public:
	bool OnInit()
	{
		Simple *simple = new Simple(wxT("Minesweeper"));
		simple->Show(true);
		return true;
	}
};

wxIMPLEMENT_APP(MyApp);