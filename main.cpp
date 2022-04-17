#include <wx/wx.h>
#include <ctime>
using namespace std;

class Minesweeper : public wxFrame {
 public:
  Minesweeper(const wxString& title)
      : wxFrame(NULL, wxID_ANY, title, wxPoint(30, 30), wxSize(800, 600)) {
    wxGridSizer* grid = new wxGridSizer(n, n, 0, 0);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        board[i][j] = new wxButton(this, 10000 + i, " ");
        grid->Add(board[i][j], 1, wxEXPAND | wxALL);
        board[i][j]->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
                          &Minesweeper::on_button_clicked, this);
      }
    }

    for (int i = 0, nMines = 10; i < nMines;) {
      int y = rand() % n, x = rand() % n;
      if (!isMine(y, x)) {
        mines.push_back(idx(y, x));
        i++;
      }
    }

    this->SetSizer(grid);
    grid->Layout();
    Centre();
  }

  bool valid_index(int i, int j) { return 0 <= i && i < n && 0 <= j && j < n; }
  int idx(int y, int x) const { return y * n + x; }

  bool isMine(int y, int x) const {
    for (int m : mines) {
      if (m == idx(y, x))
        return true;
    }
    return false;
  }

 private:
  static constexpr int n = 10;
  int dx[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
  int dy[8] = {1, 1, 1, 0, 0, -1, -1, -1};
  bool gameOver = false;
  wxButton* board[n][n];
  std::vector<int> mines;

  void on_button_clicked(wxCommandEvent& evt) {
    int y = (evt.GetId() - 10000) / n;
    int x = (evt.GetId() - 10000) % n;

    gameOver = isMine(y, x);

    if (gameOver) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (isMine(i, j)) {
                    board[i][j]->SetLabel("*");
                }
            }
        }
        return;
    }
    board[y][x]->SetLabel(dfs(y, x));
    evt.Skip();
  }

  std::string dfs(bool(&visited)[n][n], int y, int x) {
    // Invariant: board[y][x] != '*' and y,x in [0,n-1]
    visited[y][x] = true;
    int count = 0;
    std::vector<std::pair<int, int>> neighbors;
    for (int i = 0; i < 8; i++) {
      if (!valid_index(y + dy[i], x + dx[i]) || visited[y + dy[i]][x + dx[i]])
        continue;
      if (isMine(y + dy[i], x + dx[i]))
        count++;
      else
        neighbors.push_back({y + dy[i], x + dx[i]});
    }
    if (count == 0) {
      for (auto p : neighbors) {
        board[p.first][p.second]->SetLabel(
            dfs(visited, p.first, p.second));
      }
      return "#";
    }
    return std::to_string(count);
  }

  std::string dfs(int y, int x) {
    bool visited[n][n] = {};
    return dfs(visited, y, x);
  }

  // wxDECLARE_EVENT_TABLE();
};

/*wxBEGIN_EVENT_TABLE(Simple, wxFrame)
    EVT_BUTTON(10001, Simple::onButtonClicked)
    EVT_LISTBOX(10003, Simple::onKeyPress)
    EVT_BUTTON(10004, Simple::deleteClick)
wxEND_EVENT_TABLE()*/

class MyApp : public wxApp {
 public:
  bool OnInit() {
    Minesweeper* game = new Minesweeper(wxT("Minesweeper"));
    game->Show(true);
    return true;
  }
};

wxIMPLEMENT_APP(MyApp);