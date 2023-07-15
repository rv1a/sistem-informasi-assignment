#include "MenuHandler.hpp"
#include "TugasBaru.hpp"
#include <MenuMaker.hpp>
#include <iostream>

int main() {
  MenuMaker maker;
  MenuHandler handler;

  handler.addDummyData();
  maker.setJudul("Assigment Manager");
  maker.addMenu("Kelola tugasmu", [&handler]() {
    MenuMaker submenu_maker;
    submenu_maker.setJudul("Menu Kelola Tugas");
    submenu_maker.addMenu("Tambah tugas baru", [&handler]() {
      handler.tambahTugas();
    });

    submenu_maker.addMenu("Tambah subtugas", [&handler]() {
      handler.tambahSubtugas();
    });

    submenu_maker.addMenu("Update tugas", [&handler]() {
      MenuMaker subsubmenu_maker;
      subsubmenu_maker.setJudul("Menu pilihan update");
      subsubmenu_maker.addMenu("Update nama tugas",
                               [&handler]() { handler.updateNama(); });
      subsubmenu_maker.addMenu("Update mata kuliah tugas",
                               [&handler]() { handler.updateMataKuliah(); });
      subsubmenu_maker.addMenu("Update deadline tugas",
                               [&handler]() { handler.updateDeadline(); });
      subsubmenu_maker.addMenu("Update status tugas",
                               [&handler]() { handler.updateStatus(); });
      subsubmenu_maker.tampilkan();
    });

    submenu_maker.addMenu("Hapus tugas",
                          [&handler]() { handler.hapusTugas(); });

    submenu_maker.addMenu("Undo operasi sebelumnya", [&handler](){
      handler.undo();
    });

    submenu_maker.tampilkan();
  });

  maker.addMenu("Tampilkan tugasmu", [&handler]() {
    MenuMaker submenu_maker;
    submenu_maker.setJudul("Menu pilihan tampilan");
    submenu_maker.addMenu("Hanya tugas yang belum dikerjakan", [&handler]() {
      handler.tampilkanTugas(Tugas::FilterGetTugas::BELUM_DIKERJAKAN);
    });
    submenu_maker.addMenu("Hanya tugas yang sudah dikerjakan", [&handler]() {
      handler.tampilkanTugas(Tugas::FilterGetTugas::SUDAH_DIKERJAKAN);
    });
    submenu_maker.addMenu("Semua tugas", [&handler]() {
      handler.tampilkanTugas(Tugas::FilterGetTugas::SEMUA);
    });
    submenu_maker.tampilkan();
  });

  maker.addMenu("Exit", []() { ::exit(1); });

  maker.loop();
}