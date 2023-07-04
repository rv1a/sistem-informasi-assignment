#include "Tugas.hpp"
#include "Datetime.hpp"
#include "DoublyLinkedList.hpp"
#include "Queue.hpp"
#include <stdexcept>
#include <unordered_map>


namespace Tugas {
void TugasStorage::tambahTugas(TugasData data) {
  data.id = _last_unused_id;
  auto *node = _list.insertBelakang(data);
  _map_id_node.insert({_last_unused_id, node});
  _last_unused_id++;
}

bool TugasStorage::hapusTugas(int id) {
  if (!_map_id_node.contains(id)) {
    return false;
  }

  auto *node = _map_id_node.at(id);
  _map_id_node.erase(id);
  _list.hapusNode(node);
  return true;
}

bool TugasStorage::updateNama(int id, std::string nama_baru) {
  if (!_map_id_node.contains(id)) {
    return false;
  }

  auto *node = _map_id_node.at(id);
  node->data.name = nama_baru;
  return true;
}

bool TugasStorage::updateMataKuliah(int id, std::string mata_kuliah_baru) {
  if (!_map_id_node.contains(id)) {
    return false;
  }

  auto *node = _map_id_node.at(id);
  node->data.mata_kuliah = mata_kuliah_baru;
  return true;
}

bool TugasStorage::updateDeadline(int id, std::string deadline_baru) {
  if (!_map_id_node.contains(id)) {
    return false;
  }

  auto *node = _map_id_node.at(id);
  auto parsed = Datetime::ParseDateTimeString(deadline_baru);
  node->data.deadline = parsed;
  return true;
}

bool TugasStorage::updateStatus(int id, bool status_baru) {
  if (!_map_id_node.contains(id)) {
    return false;
  }

  auto *node = _map_id_node.at(id);
  node->data.sudah_dikerjakan = status_baru;
  return true;
}

Queue<TugasData>
TugasStorage::dapatkanTugasBerdasarkanFilter(FilterGetTugas filter) const {
  auto *current = _list.head;
  Queue<TugasData> filtered_tugas;
  while (current != nullptr) {
    auto tugas = current->data;
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

    current = current->next;
  }
  return filtered_tugas;
}

TugasStorage TugasStorage::clone() {
  TugasStorage new_storage;
  
  // Traverse over list
  auto* temp = _list.head;
  while (temp != nullptr) {
    new_storage.tambahTugas(temp->data);
    temp = temp->next;
  }

  return new_storage;
}


} // namespace Tugas
