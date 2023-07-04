#ifndef SRC_LIB_MENUMAKER
#define SRC_LIB_MENUMAKER

#include <string>

class MenuMaker {
  // Tipe fungsi dengan return type void dan tanpa parameter.
  using tipe_fungsi_callback = std::function<void()>;

private:
  // Jumlah menu maksimal yang dapat disimpan.
  static const int MAX_MENU_ITEM = 20;

  std::string _judul = "Unnamed Menu";
  std::string _menu_items[MAX_MENU_ITEM] = {};
  tipe_fungsi_callback _callbacks[MAX_MENU_ITEM] = {};
  int _jumlah_menu_item = 0;

  // Fungsi untuk mengecek apakah pilihan yang dimasukkan dari user valid.
  bool isPilihanValid(int input_pilihan) const;

  void printJudulDanMenu();

  static int mintaInputPilihan();

public:
  void setJudul(std::string judul);

  void addMenu(std::string menu, tipe_fungsi_callback fungsi_callback);

  void tampilkan();

  void loop(bool clear_console_after_every_loop = false);
};

#endif /* SRC_LIB_MENUMAKER */
