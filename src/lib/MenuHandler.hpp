#ifndef SRC_LIB_MENUHANDLER
#define SRC_LIB_MENUHANDLER

#include "Stack2.hpp"
#include "TugasBaru.hpp"
#include "Utils.hpp"
#include <optional>

class UndoManager {
private:
  Stack2<Tugas::TugasManager> prev_state_;

public:
  void saveState(Tugas::TugasManager storage_state);

  std::optional<Tugas::TugasManager> popState();
};

// Wrapper class around TugasManager that deals with user input.
class MenuHandler {
private:
  Tugas::TugasManager tugasManager_;
  UndoManager undoManager_;

public:
  void tampilkanTugas(Tugas::FilterGetTugas filter);
  void tambahTugas();
  void tambahSubtugas();
  void updateNama();
  void updateMataKuliah();
  void updateDeadline();
  void updateStatus();
  void hapusTugas();
  void addDummyData();
  void undo();
};

#endif /* SRC_LIB_MENUHANDLER */
