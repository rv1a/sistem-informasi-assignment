#include "MenuHandler.hpp"
#include "Datetime.hpp"
#include "Queue.hpp"
#include "Tugas.hpp"
#include "Utils.hpp"
#include <exception>
#include <iomanip>
#include <iostream>
#include <sstream>

// Kita import std karena cout, cin, dll banyak digunakan
using namespace std;

void UndoManager::saveState(Tugas::TugasStorage storage_state) {
  _prev_state.push(storage_state.clone());
}
std::optional<Tugas::TugasStorage> UndoManager::popState() {
  if (_prev_state.isEmpty()) {
    return std::nullopt;
  }
  auto prev = _prev_state.pop();
  return std::make_optional(prev);
}

void MenuHandler::tampilkanTugas(Tugas::FilterGetTugas filter) {

  cout << "List Tugas" << endl;
  cout << setw(5) << left << "ID" << setw(25) << left << "Nama" << setw(25)
       << left << "Mata Kuliah" << setw(30) << left << "Deadline" << setw(20)
       << left << "Status" << left << "Sisa Waktu" << endl;

  Queue<Tugas::TugasData> filtered =
      _storage.dapatkanTugasBerdasarkanFilter(filter);
  while (!filtered.isEmpty()) {
    Tugas::TugasData &tugas = filtered.front();
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

    cout << setw(5) << left << tugas.id << setw(25) << left << tugas.name
         << setw(25) << left << tugas.mata_kuliah << setw(30) << left
         << formatted_deadline << setw(20) << left << status << left
         << formatted_sisa_waktu.str() << endl;

    filtered.dequeue();
  }
}

void MenuHandler::tambahTugas() {
  _undo_manager.saveState(_storage);
  try {
    Tugas::TugasData tugas_baru;
    tugas_baru.name = Utils::prompt<string>("Masukkan nama tugas: ");
    tugas_baru.mata_kuliah =
        Utils::prompt<string>("Masukkan mata kuliah tugas: ");
    string deadline_raw = Utils::prompt<string>("Masukkan deadline tugas: ");
    tugas_baru.deadline = Datetime::ParseDateTimeString(deadline_raw);
    tugas_baru.sudah_dikerjakan = false;
    _storage.tambahTugas(tugas_baru);
  } catch (std::exception) {
    _undo_manager.popState();
  }
}

void MenuHandler::updateNama() {
  _undo_manager.saveState(_storage);
  int id = Utils::prompt<int>("Masukkan id tugas untuk diupdate: ");
  string nama_baru = Utils::prompt<string>("Masukkan nama baru: ");
  bool success = _storage.updateNama(id, nama_baru);
  if (success) {
    cout << "Tugas dengan id " << id << " berhasil diupdate" << endl;
    return;
  }

  _undo_manager.popState();
  cout << "Tugas dengan id " << id << " tidak ditemukan" << endl;
}

void MenuHandler::updateMataKuliah() {
  int id = Utils::prompt<int>("Masukkan id tugas untuk diupdate: ");
  Utils::reset_cin_state();
  string mk_baru = Utils::prompt<string>("Masukkan mata kuliah baru: ");
  bool success = _storage.updateMataKuliah(id, mk_baru);
  if (success) {
    cout << "Tugas dengan id " << id << " berhasil diupdate" << endl;
    return;
  }

  cout << "Tugas dengan id " << id << " tidak ditemukan" << endl;
}

void MenuHandler::updateDeadline() {
  _undo_manager.saveState(_storage);
  int id = Utils::prompt<int>("Masukkan id tugas untuk diupdate: ");
  Utils::reset_cin_state();
  string deadline_baru = Utils::prompt<string>("Masukkan deadline baru: ");
  bool success = _storage.updateDeadline(id, deadline_baru);
  if (success) {
    cout << "Tugas dengan id " << id << " berhasil diupdate" << endl;
    return;
  }

  _undo_manager.popState();
  cout << "Tugas dengan id " << id << " tidak ditemukan" << endl;
}

void MenuHandler::updateStatus() {
  _undo_manager.saveState(_storage);
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

  bool success = _storage.updateStatus(id, pilihan_in_bool);
  if (success) {
    cout << "Tugas dengan id " << id << " berhasil diupdate" << endl;
    return;
  }

  _undo_manager.popState();
  cout << "Tugas dengan id " << id << " tidak ditemukan" << endl;
}

void MenuHandler::hapusTugas() {
  _undo_manager.saveState(_storage);
  int id = Utils::prompt<int>("Masukkan id tugas untuk dihapus: ");
  bool success = _storage.hapusTugas(id);
  if (success) {
    cout << "Tugas dengan id " << id << " berhasil dihapus" << endl;
    return;
  }

  _undo_manager.popState();
  cout << "Tugas dengan id " << id << " tidak ditemukan" << endl;
}

void MenuHandler::undo() {
  auto prev_state = _undo_manager.popState();
  if (!prev_state.has_value()) {
    cout << "Tidak dapat melakukan undo." << endl;
    return;
  }

  _storage = prev_state.value();
  cout << "Undo berhasil." << endl;
}

void MenuHandler::addDummyData() {
  _storage.tambahTugas(
      {.name = "Quiz 2",
       .mata_kuliah = "Kalkulus",
       .deadline = Datetime::ParseDateTimeString("3 agustus 2023 20:30"),
       .sudah_dikerjakan = false});

  _storage.tambahTugas(
      {.name = "Essay Data Struktur",
       .mata_kuliah = "Algoritma Pemrograman",
       .deadline = Datetime::ParseDateTimeString("20 juli 2023 08:30"),
       .sudah_dikerjakan = false});

  _storage.tambahTugas(
      {.name = "Audio Recoding",
       .mata_kuliah = "Multimedia",
       .deadline = Datetime::ParseDateTimeString("5 juli 2023 22:00"),
       .sudah_dikerjakan = true});
}
