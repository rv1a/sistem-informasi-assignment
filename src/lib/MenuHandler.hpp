#ifndef SRC_LIB_MENUHANDLER
#define SRC_LIB_MENUHANDLER

#include "Stack2.hpp"
#include "Tugas.hpp"
#include "Utils.hpp"
#include <optional>

class UndoManager {
private:
  Stack2<Tugas::TugasStorage> _prev_state;

public:
  void saveState(Tugas::TugasStorage storage_state);

  std::optional<Tugas::TugasStorage> popState();
};

// Wrapper class around TugasStorage that deals with user input.
class MenuHandler {
private:
  Tugas::TugasStorage _storage;
  UndoManager _undo_manager;

public:
  void tampilkanTugas(Tugas::FilterGetTugas filter);
  void tambahTugas();
  void updateNama();
  void updateMataKuliah();
  void updateDeadline();
  void updateStatus();
  void hapusTugas();
  void addDummyData();
  void undo();
};

#endif /* SRC_LIB_MENUHANDLER */
