#ifndef SRC_LIB_DATETIME
#define SRC_LIB_DATETIME

#include <chrono>

namespace Datetime {
using system_clock = std::chrono::system_clock;
// Fungsi untuk parsing date time string.
// Date time string dapat berbentuk:
// 1. 03-07-2003 14:00
// 2. 3 Juli 2003 14:00
// 3. 3 July 2003 14:00
// 4. 3 Jul 2003 14:00 (3 character month name abbrev.)
system_clock::time_point ParseDateTimeString(std::string datetime_string);
std::string FormatTimepoint(system_clock::time_point time_point);

struct SisaWaktu {
  int hari;
  int jam;
  int menit;
};

// Fungsi untuk menghitung sisa waktu dari waktu sekarang sampai tp_future.
SisaWaktu WaktuSampaiTimePoint(system_clock::time_point tp_future);
} // namespace Datetime

#endif /* SRC_LIB_DATETIME */
