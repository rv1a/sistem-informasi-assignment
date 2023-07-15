#include "TugasBaru.hpp"
#include "Datetime.hpp"

namespace Tugas {
void TugasList::tambahTugas(std::shared_ptr<DataTugas> tugas) {
  list_.emplace_back(std::move(tugas));
}

void TugasList::hapusById(int id) {
  for (auto it = list_.begin(); it != list_.end(); ++it) {
    if (it->get()->id == id) {
      list_.erase(it);
      return;
    }
  }
}

std::vector<DataTugas> TugasList::getSemuaTugas() {
  std::vector<DataTugas> result;
  for (const auto tugasptr : list_) {
    result.push_back(*tugasptr.get());
  }
  return result;
}

void MapIDTugas::tambahTugas(std::shared_ptr<DataTugas> tugas) {
  auto id = tugas->id;
  map_.insert({id, tugas});
}

void MapIDTugas::hapusById(int id) { map_.erase(id); }

bool MapIDTugas::adaTugasDenganId(int id) { return map_.contains(id); }

std::optional<std::shared_ptr<DataTugas>> MapIDTugas::getTugasById(int id) {
  auto iterator = map_.find(id);
  if (iterator == map_.end()) {
    // Tidak ditemukan
    return std::nullopt;
  }

  // Iterator->second berisi value map yaitu ptr ke tugasnya
  return iterator->second;
}

void TugasManager::tambahTugas(DataTugas &&tugas, bool skip_assign_id) {
  if (!skip_assign_id) {
    idAssigner_.assignId(tugas);
  }
  auto wrapped_tugas = std::make_shared<DataTugas>(tugas);
  graphTugas_.tambahTugas(tugas.id);
  tugasList_.tambahTugas(wrapped_tugas);
  mapIdTugas_.tambahTugas(wrapped_tugas);
}

bool TugasManager::hapusTugas(int id) {
  if (!mapIdTugas_.adaTugasDenganId(id)) {
    return false;
  }

  graphTugas_.hapusTugas(id);
  tugasList_.hapusById(id);
  mapIdTugas_.hapusById(id);
  return true;
}

std::optional<std::shared_ptr<DataTugas>> TugasManager::getTugasById(int id) {
  return mapIdTugas_.getTugasById(id);
}

void TugasManager::updateNama(std::shared_ptr<DataTugas> tugas,
                              std::string nama_baru) {
  tugas->nama = nama_baru;
};

void TugasManager::updateMataKuliah(std::shared_ptr<DataTugas> tugas,
                                    std::string mata_kuliah_baru) {
  tugas->mata_kuliah = mata_kuliah_baru;
};

void TugasManager::updateDeadline(
    std::shared_ptr<DataTugas> tugas,
    std::chrono::system_clock::time_point deadline_baru) {
  tugas->deadline = deadline_baru;
};

void TugasManager::updateStatus(std::shared_ptr<DataTugas> tugas,
                                bool status_baru) {
  tugas->sudah_dikerjakan = status_baru;
};

Queue<DataTugas>
TugasManager::getTugasBerdasarkanFilter(FilterGetTugas filter) {
  Queue<DataTugas> filtered_tugas;
  for (auto tugas : tugasList_.getSemuaTugas()) {
    if (filter == FilterGetTugas::BELUM_DIKERJAKAN) {
      if (!tugas.sudah_dikerjakan) {
        filtered_tugas.enqueue(tugas);
      }
    } else if (filter == FilterGetTugas::SUDAH_DIKERJAKAN) {
      if (tugas.sudah_dikerjakan) {
        filtered_tugas.enqueue(tugas);
      }
    } else if (filter == FilterGetTugas::SEMUA) {
      filtered_tugas.enqueue(tugas);
    }
  }

  return filtered_tugas;
}

TugasManager TugasManager::clone() {
  TugasManager copy;
  auto semua_tugas = tugasList_.getSemuaTugas();
  for (auto tugas : semua_tugas) {
    if (tugas.is_sub_tugas) {
      auto *node = graphTugas_.getNodeById(tugas.id);
      copy.tambahSubtugas(node->parent->id_tugas, tugas.nama, tugas.id);
    } else {
      copy.tambahTugas(std::move(tugas), true);
    }
  }
  return copy;
};

std::vector<DataTugas> TugasManager::getAllSubtugas(int tugas_id) {
  std::vector<DataTugas> result;
  auto *node = graphTugas_.getNodeById(tugas_id);
  for (auto *child : node->children) {
    auto subtugas = mapIdTugas_.getTugasById(child->id_tugas);
    if (subtugas.has_value()) {
      result.push_back(*subtugas.value().get());
    }
  }
  return result;
}

bool TugasManager::tambahSubtugas(int id_tugas_parent,
                                  std::string nama_subtugas,
                                  int __internal_copy_id) {
  auto parent = this->getTugasById(id_tugas_parent);
  if (!parent.has_value()) {
    return false;
  }

  DataTugas clone = *parent.value().get();
  if (__internal_copy_id != -1) {
    clone.id = __internal_copy_id;
  } else {
    idAssigner_.assignId(clone);
  }
  clone.is_sub_tugas = true;
  clone.nama = nama_subtugas;
  auto shared_clone = std::make_shared<DataTugas>(clone);
  tugasList_.tambahTugas(shared_clone);
  mapIdTugas_.tambahTugas(shared_clone);
  graphTugas_.tambahTugas(parent.value()->id, clone.id);
  return true;
}
void GraphTugas::hapusTugas(int id) {
  auto *node = getNodeById(id);
  for (auto it = nodes.begin(); it != nodes.end(); ++it) {
    GraphTugasNode *node = *it;
    if (node->id_tugas == id) {
      nodes.erase(it);
      break;
    }
  }

  auto node_children = node->children;
  if (node_children.empty()) {
    for (auto it = node_children.begin(); it != node_children.end();) {
      GraphTugasNode *child = *it;
      delete child;
      it = node_children.erase(it);
    }
  }

  delete node;
};
void GraphTugas::tambahTugas(int id) {
  auto *node = buatNodeGraph(id);
  nodes.push_back(node);
}
GraphTugasNode *GraphTugas::getNodeById(int id) {
  for (auto *node : nodes) {
    if (node->id_tugas == id) {
      return node;
    }
  }

  return nullptr;
}

void GraphTugas::tambahTugas(int parent_id, int id) {
  auto *parent_node = getNodeById(parent_id);
  if (parent_node != nullptr) {
    auto *node = buatNodeGraph(id);
    node->parent = parent_node;
    parent_node->children.push_back(node);
    nodes.push_back(node);
  }
}

void AddDummyData(TugasManager &manager) {
  manager.tambahTugas(
      {.nama = "Quiz 2",
       .mata_kuliah = "Kalkulus",
       .deadline = Datetime::ParseDateTimeString("3 agustus 2023 20:30"),
       .sudah_dikerjakan = false});

  manager.tambahTugas(
      {.nama = "Essay Data Struktur",
       .mata_kuliah = "Algoritma Pemrograman",
       .deadline = Datetime::ParseDateTimeString("20 juli 2023 08:30"),
       .sudah_dikerjakan = false});

  manager.tambahTugas(
      {.nama = "Audio Recoding",
       .mata_kuliah = "Multimedia",
       .deadline = Datetime::ParseDateTimeString("5 juli 2023 22:00"),
       .sudah_dikerjakan = true});
}
} // namespace Tugas
