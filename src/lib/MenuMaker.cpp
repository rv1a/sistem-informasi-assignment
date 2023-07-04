#include "MenuMaker.hpp"
#include "Utils.hpp"
#include <functional>
#include <iostream>

bool MenuMaker::isPilihanValid(int input_pilihan) const {
  return input_pilihan >= 1 && input_pilihan <= _jumlah_menu_item;
}

void MenuMaker::printJudulDanMenu() {
  std::cout << "====== " << _judul << " ======" << std::endl;
  for (int i = 0; i < _jumlah_menu_item; i++) {
    std::cout << (i + 1) << ". " << _menu_items[i] << std::endl;
  }
}

int MenuMaker::mintaInputPilihan() {
  int pilihan;
  std::cout << "Masukkan pilihan anda: ";
  std::cin >> pilihan;
  return pilihan;
}

void MenuMaker::setJudul(std::string judul) { _judul = judul; }

void MenuMaker::addMenu(std::string menu,
                        tipe_fungsi_callback fungsi_callback) {
  if (_jumlah_menu_item >= MAX_MENU_ITEM) {
    std::cout << "Error: max menu item reached" << std::endl;
    return;
  }

  _menu_items[_jumlah_menu_item] = menu;
  _callbacks[_jumlah_menu_item] = fungsi_callback;
  _jumlah_menu_item = _jumlah_menu_item + 1;
}

void MenuMaker::tampilkan() {
  printJudulDanMenu();
  int pilihan = mintaInputPilihan();

  if (!isPilihanValid(pilihan)) {
    std::cout << "Error: pilihan tidak valid!" << std::endl;
    return;
  }

  // Input is valid.
  _callbacks[pilihan - 1]();
}

void MenuMaker::loop(bool clear_console_after_every_loop) {
  while (true) {
    tampilkan();
    std::cout << "Press enter to continue...";
    // Utils::reset_cin_state();
    std::cin.get();

    if (clear_console_after_every_loop) {
      system("cls");
    }
  }
}
