#ifndef SRC_LIB_TUGAS
#define SRC_LIB_TUGAS
#include "DoublyLinkedList.hpp"
#include "Queue.hpp"
#include <chrono>
#include <string>

namespace Tugas {
struct TugasData {
  int id;
  std::string name;
  std::string mata_kuliah;
  std::chrono::system_clock::time_point deadline;
  bool sudah_dikerjakan;
};

enum class FilterGetTugas {
  BELUM_DIKERJAKAN,
  SUDAH_DIKERJAKAN,
  SEMUA,
};

class TugasStorage {
private:
  // Semua TugasData akan disimpan di linkedlist ini.
  DoublyLinkedList<TugasData> _list;

  // Setiap TugasData akan diberikan id unik dari ini.
  int _last_unused_id = 0;

  // Map dengan key id dan value Node.
  std::unordered_map<int, Node<TugasData> *> _map_id_node;

public:
  void tambahTugas(TugasData data);
  bool hapusTugas(int id);
  bool updateNama(int id, std::string nama_baru);
  bool updateMataKuliah(int id, std::string mata_kuliah_baru);
  bool updateDeadline(int id, std::string deadline_baru);
  bool updateStatus(int id, bool status_baru);
  Queue<TugasData> dapatkanTugasBerdasarkanFilter(FilterGetTugas filter) const;
  TugasStorage clone();
};

void AddDummyData(TugasStorage &storage);
} // namespace Tugas

#endif /* SRC_LIB_TUGAS */
