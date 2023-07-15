#include "MenuHandler.hpp"
#include "Datetime.hpp"
#include "Queue.hpp"
#include "TugasBaru.hpp"
#include "Utils.hpp"
#include <exception>
#include <iomanip>
#include <iostream>
#include <sstream>

// Kita import std karena cout, cin, dll banyak digunakan
using namespace std;

void UndoManager::saveState(Tugas::TugasManager storage_state) {
  prev_state_.push(storage_state.clone());
}
std::optional<Tugas::TugasManager> UndoManager::popState() {
  if (prev_state_.isEmpty()) {
    return std::nullopt;
  }
  auto prev = prev_state_.pop();
  return std::make_optional(prev);
}

void MenuHandler::tampilkanTugas(Tugas::FilterGetTugas filter) {

  cout << "List Tugas" << endl;
  cout << setw(5) << left << "ID" << setw(25) << left << "Nama" << setw(25)
       << left << "Mata Kuliah" << setw(30) << left << "Deadline" << setw(20)
       << left << "Status" << left << "Sisa Waktu" << endl;

  Queue<Tugas::DataTugas> filtered =
      tugasManager_.getTugasBerdasarkanFilter(filter);
  while (!filtered.isEmpty()) {
    Tugas::DataTugas &tugas = filtered.front();
    if (!tugas.is_sub_tugas) {
      auto sisa_waktu = Datetime::WaktuSampaiTimePoint(tugas.deadline);
      auto formatted_deadline = Datetime::FormatTimepoint(tugas.deadline);
      ostringstream formatted_sisa_waktu;

      string status;
      if (tugas.sudah_dikerjakan) {
        status = "Sudah dikerjakan";
        formatted_sisa_waktu << formatted_deadline;
      } else {
        status = "Belum dikerjakan";
        formatted_sisa_waktu << to_string(sisa_waktu.hari) << " hari "
                             << to_string(sisa_waktu.jam) << " jam "
                             << to_string(sisa_waktu.menit) << " menit";
      }

      cout << setw(5) << left << tugas.id << setw(25) << left << tugas.nama
           << setw(25) << left << tugas.mata_kuliah << setw(30) << left
           << formatted_deadline << setw(20) << left << status << left
           << formatted_sisa_waktu.str() << endl;

      auto subtugas_list = tugasManager_.getAllSubtugas(tugas.id);
      if (!subtugas_list.empty()) {
        cout << "=== SUBTUGAS DARI TUGAS " << tugas.nama << " ===" << endl;
        for (const auto &subtugas : subtugas_list) {
          string sub_status = subtugas.sudah_dikerjakan ? "Sudah dikerjakan"
                                                        : "Belum dikerjakan";
          cout << "=== ID: " << subtugas.id << " Nama: " << subtugas.nama
               << " Status: " << sub_status << " ===" << endl;
        }
        cout << "\n";
      }
    }
    filtered.dequeue();
  }
}

void MenuHandler::tambahTugas() {
  undoManager_.saveState(tugasManager_);
  try {
    Tugas::DataTugas tugas_baru;
    tugas_baru.nama = Utils::prompt<string>("Masukkan nama tugas: ");
    tugas_baru.mata_kuliah =
        Utils::prompt<string>("Masukkan mata kuliah tugas: ");
    string deadline_raw = Utils::prompt<string>("Masukkan deadline tugas: ");
    tugas_baru.deadline = Datetime::ParseDateTimeString(deadline_raw);
    tugas_baru.sudah_dikerjakan = false;
    tugasManager_.tambahTugas(std::move(tugas_baru));
  } catch (std::exception) {
    undoManager_.popState();
  }
}

void MenuHandler::tambahSubtugas() {
  undoManager_.saveState(tugasManager_);
  try {
    int parent_id = Utils::prompt<int>("Masukkan id tugas: ");
    string nama = Utils::prompt<string>("Masukkan nama sub-tugas: ");
    bool success = tugasManager_.tambahSubtugas(parent_id, nama);
    if (!success) {
      cout << "Gagal menambahkan sub-tugas" << endl;
      undoManager_.popState();
      return;
    }
  } catch (std::exception) {
    undoManager_.popState();
  }
}

void MenuHandler::updateNama() {
  undoManager_.saveState(tugasManager_);
  int id = Utils::prompt<int>("Masukkan id tugas untuk diupdate: ");
  string nama_baru = Utils::prompt<string>("Masukkan nama baru: ");
  auto tugasptr = tugasManager_.getTugasById(id);
  if (tugasptr.has_value()) {
    Tugas::TugasManager::updateNama(tugasptr.value(), nama_baru);
    cout << "Tugas dengan id " << id << " berhasil diupdate" << endl;
    return;
  }
  undoManager_.popState();
  cout << "Tugas dengan id " << id << " tidak ditemukan" << endl;
}

void MenuHandler::updateMataKuliah() {
  int id = Utils::prompt<int>("Masukkan id tugas untuk diupdate: ");
  Utils::reset_cin_state();
  string mk_baru = Utils::prompt<string>("Masukkan mata kuliah baru: ");
  auto tugasptr = tugasManager_.getTugasById(id);
  if (tugasptr.has_value()) {
    Tugas::TugasManager::updateMataKuliah(tugasptr.value(), mk_baru);
    cout << "Tugas dengan id " << id << " berhasil diupdate" << endl;
    return;
  }
  undoManager_.popState();
  cout << "Tugas dengan id " << id << " tidak ditemukan" << endl;
}

void MenuHandler::updateDeadline() {
  undoManager_.saveState(tugasManager_);
  int id = Utils::prompt<int>("Masukkan id tugas untuk diupdate: ");
  Utils::reset_cin_state();
  string deadline_baru = Utils::prompt<string>("Masukkan deadline baru: ");
  auto tugasptr = tugasManager_.getTugasById(id);
  if (tugasptr.has_value()) {
    Tugas::TugasManager::updateDeadline(
        tugasptr.value(), Datetime::ParseDateTimeString(deadline_baru));
    cout << "Tugas dengan id " << id << " berhasil diupdate" << endl;
    return;
  }
  undoManager_.popState();
  cout << "Tugas dengan id " << id << " tidak ditemukan" << endl;
}

void MenuHandler::updateStatus() {
  undoManager_.saveState(tugasManager_);
  int id = Utils::prompt<int>("Masukkan id tugas untuk diupdate: ");
  cout << "Pilih status baru: " << endl;
  cout << "1. Belum dikerjakan" << endl;
  cout << "2. Sudah dikerjakan" << endl;
  int pilihan = Utils::prompt<int>("Masukkan pilihan anda: ");
  bool pilihan_in_bool = false;
  if (pilihan == 1) {
    pilihan_in_bool = false;
  } else if (pilihan == 2) {
    pilihan_in_bool = true;
  } else {
    cout << "Pilihan invalid!" << endl;
    return;
  }

  auto tugas_or_null = tugasManager_.getTugasById(id);
  if (tugas_or_null.has_value()) {
    auto tugas = tugas_or_null.value();
    bool canUpdateStatus = tugasManager_.allSubtugasDone(tugas);
    if (canUpdateStatus) {
      Tugas::TugasManager::updateStatus(tugas, pilihan_in_bool);
      cout << "Tugas dengan id " << id << " berhasil diupdate" << endl;
    } else {
      undoManager_.popState();
      cout << "Tidak bisa mengubah status tugas karena ada subtugas yang belum "
              "dikerjakan"
           << endl;
    }
    return;
  }
  undoManager_.popState();
  cout << "Tugas dengan id " << id << " tidak ditemukan" << endl;
}

void MenuHandler::hapusTugas() {
  undoManager_.saveState(tugasManager_);
  int id = Utils::prompt<int>("Masukkan id tugas untuk dihapus: ");
  bool success = tugasManager_.hapusTugas(id);
  if (success) {
    cout << "Tugas dengan id " << id << " berhasil dihapus" << endl;
    return;
  }

  undoManager_.popState();
  cout << "Tugas dengan id " << id << " tidak ditemukan" << endl;
}

void MenuHandler::undo() {
  auto prev_state = undoManager_.popState();
  if (!prev_state.has_value()) {
    cout << "Tidak dapat melakukan undo." << endl;
    return;
  }

  tugasManager_ = prev_state.value();
  cout << "Undo berhasil." << endl;
}

void MenuHandler::addDummyData() {
  tugasManager_.tambahTugas(
      {.nama = "Quiz 2",
       .mata_kuliah = "Kalkulus",
       .deadline = Datetime::ParseDateTimeString("3 agustus 2023 20:30"),
       .sudah_dikerjakan = false});

  tugasManager_.tambahTugas(
      {.nama = "Essay Data Struktur",
       .mata_kuliah = "Algoritma Pemrograman",
       .deadline = Datetime::ParseDateTimeString("20 juli 2023 08:30"),
       .sudah_dikerjakan = false});

  tugasManager_.tambahTugas(
      {.nama = "Audio Recoding",
       .mata_kuliah = "Multimedia",
       .deadline = Datetime::ParseDateTimeString("5 juli 2023 22:00"),
       .sudah_dikerjakan = true});
}
