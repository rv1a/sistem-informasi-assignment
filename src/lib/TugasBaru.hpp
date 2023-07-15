#ifndef SRC_LIB_TUGASBARU
#define SRC_LIB_TUGASBARU

#include "Queue.hpp"
#include <chrono>
#include <concepts>
#include <list>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

namespace Tugas {

template <typename T>
concept HasIDField = requires(const T &val) { val.id; };

// Class yang berfungsi untuk memberikan id ke struct DataTugas
template <HasIDField T> class IDAssigner {
private:
  static inline int last_assigned_id_ = 0;

public:
  void assignId(T &cls) { cls.id = ++last_assigned_id_; }
};

struct DataTugas {
  int id;
  std::string nama;
  std::string mata_kuliah;
  std::chrono::system_clock::time_point deadline;
  bool sudah_dikerjakan;
  bool is_sub_tugas = false;
};

struct GraphTugasNode {
  int id_tugas;
  GraphTugasNode *parent;
  std::vector<GraphTugasNode *> children;
};

class GraphTugas {
private:
  static GraphTugasNode *buatNodeGraph(int id_tugas) {
    GraphTugasNode *newNode = new GraphTugasNode();
    newNode->id_tugas = id_tugas;
    return newNode;
  }

  std::vector<GraphTugasNode *> nodes;

public:
  void tambahTugas(int id);
  GraphTugasNode *getNodeById(int id);
  void tambahTugas(int parent_id, int id);
  void hapusTugas(int id);
};

class TugasList {
private:
  std::list<std::shared_ptr<DataTugas>> list_;

public:
  void tambahTugas(std::shared_ptr<DataTugas> tugas);
  void hapusById(int id);
  // Return copies of all tugas
  std::vector<DataTugas> getSemuaTugas();
};

class MapIDTugas {
private:
  std::unordered_map<int, std::shared_ptr<DataTugas>> map_;

public:
  void tambahTugas(std::shared_ptr<DataTugas> tugas);
  void hapusById(int id);
  bool adaTugasDenganId(int id);
  std::optional<std::shared_ptr<DataTugas>> getTugasById(int id);
};

enum class FilterGetTugas {
  BELUM_DIKERJAKAN,
  SUDAH_DIKERJAKAN,
  SEMUA,
};

class TugasManager {
private:
  TugasList tugasList_;
  MapIDTugas mapIdTugas_;
  GraphTugas graphTugas_;
  IDAssigner<DataTugas> idAssigner_;

public:
  void tambahTugas(DataTugas &&tugas, bool skip_assign_id = false);
  bool tambahSubtugas(int id_tugas_parent, std::string nama_subtugas,
                      int __internal_copy_id = -1);
  std::vector<DataTugas> getAllSubtugas(int tugas_id);
  bool allSubtugasDone(std::shared_ptr<DataTugas> tugas) {
    auto* node = graphTugas_.getNodeById(tugas->id);
    bool all_done = true;
    if (node != nullptr) {
      for (auto* child : node->children) {
        auto tugas = mapIdTugas_.getTugasById(child->id_tugas);
        if (!tugas->get()->sudah_dikerjakan) {
          all_done = false;
        }
      }
    }
    return all_done;
  }
  bool hapusTugas(int id);
  // Wrapper function
  std::optional<std::shared_ptr<DataTugas>> getTugasById(int id);
  static void updateNama(std::shared_ptr<DataTugas> tugas,
                         std::string nama_baru);
  static void updateMataKuliah(std::shared_ptr<DataTugas> tugas,
                               std::string mata_kuliah_baru);
  static void
  updateDeadline(std::shared_ptr<DataTugas> tugas,
                 std::chrono::system_clock::time_point deadline_baru);

  static void updateStatus(std::shared_ptr<DataTugas> tugas, bool status_baru);
  Queue<DataTugas> getTugasBerdasarkanFilter(FilterGetTugas filter);
  TugasManager clone();
};

void AddDummyData(TugasManager &manager);
} // namespace Tugas

#endif /* SRC_LIB_TUGASBARU */
