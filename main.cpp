#include <wx/wx.h>
#include <ctime>

class Minesweeper : public wxFrame {
 public:
  Minesweeper(const wxString& title)
      : wxFrame(NULL, wxID_ANY, title, wxPoint(30, 30), wxSize(800, 600)) {
    // Create an nxn grid matching the screen proportions
    wxGridSizer* grid = new wxGridSizer(n, n, 0, 0);

    wxFont font(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD,
                false);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        board[i][j] = new wxButton(this, 10000 + idx(i, j), " ");
        grid->Add(board[i][j], 1, wxEXPAND | wxALL);
        board[i][j]->SetLabel("#");
        board[i][j]->SetFont(font);
        board[i][j]->SetBackgroundColour(wxColour(49, 11, 154, 50));
        board[i][j]->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
                          &Minesweeper::on_left_click, this);
        board[i][j]->Bind(wxEVT_RIGHT_DOWN, &Minesweeper::on_right_click, this);
      }
    }

    // 30 mines placed in random positions
    std::srand(std::time(0));
    for (int i = 0, nMines = 30; i < nMines;) {
      int y = rand() % n, x = rand() % n;
      if (!isMine(y, x)) {
        mines.push_back(idx(y, x));
        i++;
      }
    }

    this->SetSizer(grid);
    grid->Layout();
    Centre();
    this->connectKeyDownEvent(this);
  }

  bool valid_index(int i, int j) { return 0 <= i && i < n && 0 <= j && j < n; }
  // 2d index -> 1d index
  int idx(int y, int x) const { return y * n + x; }

  // is a mine located in the array of mines
  bool isMine(int y, int x) const {
    for (int m : mines) {
      if (m == idx(y, x))
        return true;
    }
    return false;
  }

 private:
  static constexpr int n = 10;
  // index for each cell neighbor
  int dx[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
  int dy[8] = {1, 1, 1, 0, 0, -1, -1, -1};
  bool gameOver = false;
  wxButton* board[n][n];
  std::vector<int> mines;
  
  void connectKeyDownEvent(wxWindow* pclComponent) {
    if (pclComponent) {
      pclComponent->Bind(wxEVT_KEY_DOWN, &Minesweeper::onKeyDown, this);

      wxWindowListNode* pclNode = pclComponent->GetChildren().GetFirst();
      while (pclNode) {
        wxWindow* pclChild = pclNode->GetData();
        this->connectKeyDownEvent(pclChild);
        pclNode = pclNode->GetNext();
      }
    }
  }

  void on_left_click(wxCommandEvent& evt) {
    int y = (evt.GetId() - 10000) / n;
    int x = (evt.GetId() - 10000) % n;

    if (gameOver || isMine(y, x)) {
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          if (isMine(i, j)) {
            board[i][j]->SetBackgroundColour(wxColour(207, 21, 33, 70));
            board[i][j]->SetLabel("*");
          }
        }
      }
      wxMessageBox("GAMEOVER");
      gameOver = true;
      return;
    }

    // disable button
    board[y][x]->Enable(false);
    // clears all nearby cells which have 0 mines as neighbors or sets value at
    // the cell to the
    // number of mine neighbors
    board[y][x]->SetLabel(dfs(y, x));
    // do not propogate event
    evt.Skip();
  }

  void on_right_click(wxMouseEvent& evt) {
    int y = (evt.GetId() - 10000) / n;
    int x = (evt.GetId() - 10000) % n;
    if (board[y][x]->GetLabel() == "?")
      board[y][x]->SetLabel("M");
    else if (board[y][x]->GetLabel() == "M")
      board[y][x]->SetLabel("#");
    else if (board[y][x]->GetLabel() == "#")
      board[y][x]->SetLabel("?");
  }

  std::string dfs(bool(&visited)[n][n], int y, int x) {
    // Invariant: board[y][x] != '*' and y,x in [0,n-1]
    visited[y][x] = true;
    int count = 0;
    std::vector<std::pair<int, int>> neighbors;
    for (int i = 0; i < 8; i++) {
      if (!valid_index(y + dy[i], x + dx[i]) || visited[y + dy[i]][x + dx[i]])
        continue;
      // count all neighbors that are not mines
      if (isMine(y + dy[i], x + dx[i]))
        count++;
      else
        neighbors.push_back({y + dy[i], x + dx[i]});
    }

    // if there are zero neighbors that are mines, visit each neighbor
    if (count == 0) {
      for (auto p : neighbors) {
        board[p.first][p.second]->SetLabel(dfs(visited, p.first, p.second));
      }
      // blank cell
      board[y][x]->SetBackgroundColour(wxColour(*wxWHITE));
      return " ";
    }
    return std::to_string(count);
  }

  std::string dfs(int y, int x) {
    bool visited[n][n] = {};
    return dfs(visited, y, x);
  }

  void onKeyDown(wxKeyEvent& event) {
      std::cout<<"keydown\n";
    if (event.GetKeyCode() == WXK_ESCAPE) {
      std::exit(0);
    }
  }
};

class MyApp : public wxApp {
 public:
  bool OnInit() {
    Minesweeper* game = new Minesweeper(wxT("Minesweeper"));
    game->Show(true);
    return true;
  }
};

wxIMPLEMENT_APP(MyApp);