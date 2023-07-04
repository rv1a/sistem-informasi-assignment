#include "Datetime.hpp"
#include <chrono>
#include <ctime>
#include <exception>
#include <regex>
#include <unordered_map>

namespace Datetime {
system_clock::time_point ParseDateTimeString(std::string datetime_string) {
  // Ubah menjadi lowercase
  for (char &karakter : datetime_string) {
    // TODO: calling std::tolower on char is undefined behavior.
    // we should cast it to unsigned char first.
    karakter = std::tolower(karakter);
  }

  // Parse tanggal menggunakan regex
  // Regex untuk matching datetime string dengan bentuk 3 Juli 2003 12:00
  std::regex form1(
      "^(\\d{1,2})[ -]([^\\s\\d-]+)[ -](\\d{4})[ ]+(\\d{2}:\\d{2})$",
      std::regex_constants::ECMAScript);
  // Regex untuk matching datetime string dengan bentuk 03-07-2003 12:00
  std::regex form2("^(\\d{1,2})[ -](\\d{1,2})[ -](\\d{4})[ ]+(\\d{2}:\\d{2})$",
                   std::regex_constants::ECMAScript);

  std::smatch matches;
  regex_search(datetime_string, matches, form1);
  if (matches.size() == 5) {
    std::unordered_map<std::string, int> nama_bulan_list = {
        {"januari", 1},  {"january", 1},   {"jan", 1},       {"februari", 2},
        {"february", 2}, {"feb", 2},       {"maret", 3},     {"march", 3},
        {"mar", 3},      {"april", 4},     {"apr", 4},       {"mei", 5},
        {"may", 5},      {"juni", 6},      {"june", 6},      {"jun", 6},
        {"juli", 7},     {"july", 7},      {"jul", 7},       {"agustus", 8},
        {"august", 8},   {"aug", 8},       {"september", 9}, {"sep", 9},
        {"oktober", 10}, {"october", 10},  {"oct", 10},      {"november", 11},
        {"nov", 11},     {"desember", 12}, {"december", 12}, {"dec", 12},
    };

    try {
      // TODO: Refactor this.
      // Dapatkan nomor bulan berdasarkan nama bulan dengan menggunakan map
      // nama_bulan_list. nama_bulan_list will throw an exception if month name
      // isn't valid so we catch it below.
      int nomor_bulan = nama_bulan_list.at(matches[2]);

      // Konversikan semua ke integer
      int tanggal = atoi(matches[1].str().c_str());
      int tahun = atoi(matches[3].str().c_str());
      int jam = atoi(matches[4].str().substr(0, 2).c_str());
      int menit = atoi(matches[4].str().substr(2, 2).c_str());

      // https://en.cppreference.com/w/cpp/chrono/c/tm
      // TODO: Validasikan tanggal, tahun (kabisat, etc...)
      std::tm tm = {.tm_min = menit,
                    .tm_hour = jam,
                    .tm_mday = tanggal,
                    .tm_mon = (nomor_bulan - 1),
                    .tm_year = (tahun - 1900),
                    .tm_isdst = -1};

      return system_clock::from_time_t(std::mktime(&tm));
    } catch (std::out_of_range &exception) {
      // TODO: do something with the error. maybe report to the user?
    }
  }

  regex_search(datetime_string, matches, form2);
  if (matches.size() == 5) {
    // TODO: Refactor this.
    // Konversikan semua ke integer
    int nomor_bulan = atoi(matches[2].str().c_str());
    int tanggal = atoi(matches[1].str().c_str());
    int tahun = atoi(matches[3].str().c_str());
    int jam = atoi(matches[4].str().substr(0, 2).c_str());
    int menit = atoi(matches[4].str().substr(2, 2).c_str());

    // https://en.cppreference.com/w/cpp/chrono/c/tm
    // TODO: Validasikan tanggal, tahun (kabisat, etc...)
    std::tm tm = {.tm_min = menit,
                  .tm_hour = jam,
                  .tm_mday = tanggal,
                  .tm_mon = (nomor_bulan - 1),
                  .tm_year = (tahun - 1900),
                  .tm_isdst = -1};

    std::time_t tt = std::mktime(&tm);
    // Validasikan time_t
    if (tt != -1) {
      return system_clock::from_time_t(tt);
    }
  }

  // TODO: Better error reporting
  throw std::runtime_error{"ERROR"};
}

std::string FormatTimepoint(system_clock::time_point time_point) {
  time_t tt = system_clock::to_time_t(time_point);
  char buf[60];
  strftime(buf, 40, "%a, %d-%m-%G %H:%M", localtime(&tt));
  return std::string(buf);
}

SisaWaktu WaktuSampaiTimePoint(system_clock::time_point tp_future) {
  system_clock::time_point tp_now = system_clock::now();
  std::chrono::seconds delta =
      std::chrono::duration_cast<std::chrono::seconds>(tp_future - tp_now);
  const int SATU_HARI_DALAM_DETIK = 86400;
  const int SATU_JAM_DALAM_DETIK = 3600;
  const int SATU_MENIT_DALAM_DETIK = 60;
  long long i_delta = delta.count();
  int hari = i_delta / SATU_HARI_DALAM_DETIK;
  i_delta = i_delta - (hari * SATU_HARI_DALAM_DETIK);
  int jam = i_delta / SATU_JAM_DALAM_DETIK;
  i_delta = i_delta - (jam * SATU_JAM_DALAM_DETIK);
  int menit = i_delta / SATU_MENIT_DALAM_DETIK;
  return SisaWaktu{hari, jam, menit};
}

} // namespace Datetime
