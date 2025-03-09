#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <lib/libc/stdlibx.h>

enum class SPrintfExpect
{
  NormalChar,
  Filler,
  FillerCounter,
  Format
};

char* xvsnprintf(char* str, size_t n, char* fmt, va_list args)
{

  SPrintfExpect expect = SPrintfExpect::NormalChar;
  char filler = ' ';
  uint32_t filler_counter = 0;

  constexpr char formats[] = { 'd', 'i', 'u', 'o', 'x', 'X', 'c', 's', 'p' , 'n', 'q', 'y', 't' };

  auto is_format_char = [formats](char c) {
    for (int i = 0; i < ARRAY_LENGTH(formats); i++) {
      if (c == formats[i]) return true;
    }
    return false;
  };

  auto format_base_get = [](char c) -> uint8_t {
    switch (c) {
    case 'b': return BINARY;
    case 'o': return OCTAL;
    case 'd': return DECIMAL;
    case 'x':
    case 'X': return HEXADECIMAL;
    default: return DECIMAL;
    }};

  auto toupper_if_needed = [](char c, char* str) -> char* {if (c >= 'A' && c <= 'Z') { toupper(str); } return str;};

  n--; // last character cant be overriden

  for (int si = 0, di = 0; fmt[si] != '\0' && di <= n; )
  {
    switch (expect)
    {

    case SPrintfExpect::NormalChar: {

      if (fmt[si + 1] == '%' && fmt[si] == '%') {

        if (fmt[si + 1] == '%') {
          str[di++] = '%';
          si++;
        }
      }

      else if (fmt[si] == '%') {
        expect = SPrintfExpect::Filler;
      }

      else {
        str[di++] = fmt[si];
      }

      si++;
      break;
    }

    case SPrintfExpect::Filler:
    {
      // dbg_info(DEBUG_LABEL_LIBC, "Expecting filler");
      if (is_format_char(fmt[si])) {
        expect = SPrintfExpect::Format;
        break;
      }

      if (!(fmt[si] >= '1' && fmt[si] <= '9')) {
        filler = fmt[si];
        si++;
      }
      expect = SPrintfExpect::FillerCounter;
      break;
    }

    case SPrintfExpect::FillerCounter:
    {
      char counter_str[64] = { 0 };
      for (int j = 0; fmt[si + j] != '\0'; j++)
      {
        if (is_format_char(fmt[si + j])) {
          memcpy(counter_str, &fmt[si], j);
          filler_counter = atoi(counter_str);
          si = si + j;
          expect = SPrintfExpect::Format;
          break;
        }
      }
      break;
    }

    case SPrintfExpect::Format:
    {
      char st[64] = { 0 };
      memset(&str[di], filler, filler_counter); //fill with filler 

      switch (fmt[si]) {

      case '\0': {
        break;
      }

      case 'c': {
        str[di + (filler_counter > 1 ? filler_counter - 1 : 0)] = (char)va_arg(args, uint32_t);
        di++;
        break;
      }

      case 's': {
        char* sa = va_arg(args, char*); // sa = string_argument
        uint32_t sa_length = strlen(sa);
        strncpy(&str[di + (sa_length >= filler_counter ? 0 : (filler_counter - sa_length))], sa, n - di);

        di = di + (sa_length > filler_counter ? sa_length : filler_counter);
        break;
      }

      case 'q': {
        //%h is used to print BCD digits
        constexpr uint32_t bcd_length = 2;
        bcd_to_string((uint8_t)va_arg(args, int), st);

        strncpy(&str[di + (bcd_length >= filler_counter ? 0 : (filler_counter - bcd_length))], st, n - di);

        di = di + (bcd_length > filler_counter ? bcd_length : filler_counter);
        break;
      }

      case 'u': {
        int_to_decimal_string(STRING_UNSIGNED, va_arg(args, int), st);
        uint32_t st_length = strlen(st);
        strncpy(&str[di + (st_length >= filler_counter ? 0 : (filler_counter - st_length))], st, n - di);
        di = di + (st_length > filler_counter ? st_length : filler_counter);
        break;
      }

      case 'n': {
        //weird character counter
        *(va_arg(args, uint32_t*)) = di;
        break;
      }

      case 'y': {
        //date
        constexpr uint32_t date_length = 10;
        date_to_string(va_arg(args, bcd_date_t), st);

        strncpy(&str[di + (date_length >= filler_counter ? 0 : (filler_counter - date_length))], st, n - di);

        di = di + (date_length > filler_counter ? date_length : filler_counter);
        break;
      }

      case 't': {
        //time
        constexpr uint32_t time_length = 5;
        time_to_string((bcd_time_t)va_arg(args, int), st);

        strncpy(&str[di + (time_length >= filler_counter ? 0 : (filler_counter - time_length))], st, n - di);

        di = di + (time_length > filler_counter ? time_length : filler_counter);
        break;
      }

      default: {
        int_to_string(va_arg(args, int), st, format_base_get(fmt[si]));
        uint32_t st_length = strlen(st);
        toupper_if_needed(fmt[si], st);

        strncpy(&str[di + (st_length >= filler_counter ? 0 : (filler_counter - st_length))], st, n - di);
        di = di + (st_length > filler_counter ? st_length : filler_counter);
      }
      }

      si++;
      expect = SPrintfExpect::NormalChar;
      filler = ' ';
      filler_counter = 0;

      break;
    }
    }
  }
  return str;
}

char* xsnprintf(char* str, size_t n, char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);

  return xvsnprintf(str, n, fmt, args);
}


char* xsprintf(char* str, char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);

  return xvsnprintf(str, UINT32_MAX, fmt, args);
}

int vsnprintf(char* str, size_t n, char* fmt, va_list args) {
  return strlen(xvsnprintf(str, n, fmt, args));
}

int snprintf(char* str, size_t n, char* fmt, ...) {
  va_list args;
  va_start(args, fmt);

  return strlen(xvsnprintf(str, n, fmt, args));
}

int sprintf(char* str, char* fmt, ...) {
  va_list args;
  va_start(args, fmt);

  return strlen(xvsnprintf(str, UINT32_MAX, fmt, args));
}

} //extern "C"

///////////////////FUNCTIONS BELOW ARE USING C++ FUNCTION NAME MANGLING //////////////////

enum class FmtParseExpect
{
  NormalChar,
  Filler,
  FillerCounter,
  Format
};

std::vector <FmtParserInfo> fmt_parse(char* fmt)
{
  FmtParseExpect expect = FmtParseExpect::NormalChar;

  std::vector <FmtParserInfo> entires_parsed;

  constexpr char formats[] = { 'd', 'i', 'u', 'o', 'x', 'X', 'c', 's', 'p' , 'n', 'q', 'y', 't' };

  auto is_format_char = [formats](char c) {
    for (int i = 0; i < ARRAY_LENGTH(formats); i++) {
      if (c == formats[i]) return true;
    }
    return false;
  };

  FmtParserInfo entry;
  for (int si = 0; fmt[si] != '\0';)
  {
    switch (expect)
    {

    case FmtParseExpect::NormalChar: {
      entry = { .alignment_char = ' ', .alignment_count = 0, .fmt = '\0', .entry_lenght = 0 };

      if (fmt[si + 1] == '%' && fmt[si] == '%');
      else if (fmt[si] == '%') {
        expect = FmtParseExpect::Filler;
      }

      si++;
      break;
    }

    case FmtParseExpect::Filler:
    {
      // dbg_info(DEBUG_LABEL_LIBC, "Expecting filler");
      if (is_format_char(fmt[si])) {
        expect = FmtParseExpect::Format;
        break;
      }

      if (!(fmt[si] >= '1' && fmt[si] <= '9')) {
        entry.alignment_char = fmt[si];
        entry.entry_lenght++;
        si++;
      }

      dbg_info("update", xsprintf("     ", "%d", entry.entry_lenght));
      expect = FmtParseExpect::FillerCounter;
      break;
    }

    case FmtParseExpect::FillerCounter:
    {
      char counter_str[64] = { 0 };
      for (int j = 0; fmt[si + j] != '\0'; j++, entry.entry_lenght++)
      {
        if (is_format_char(fmt[si + j])) {
          memcpy(counter_str, &fmt[si], j);
          entry.alignment_count = atoi(counter_str);
          si = si + j;
          expect = FmtParseExpect::Format;
          break;
        }
      }
      dbg_info("update", xsprintf("     ", "%d", entry.entry_lenght));
      break;
    }

    case FmtParseExpect::Format:
    {
      entry.fmt = fmt[si++];
      entry.entry_lenght++;
      dbg_info("update", xsprintf("     ", "%d", entry.entry_lenght));
      expect = FmtParseExpect::NormalChar;

      // if (entry.entry_lenght > 1) {
      //     entry.entry_lenght = entry.entry_lenght - 1;
      // }

      entires_parsed.push_back(entry);
      break;
    }
    }
  }
  return entires_parsed;
}

char* new_xvsnprintf(char* str, size_t n, char* fmt, va_list args)
{
  SPrintfExpect expect = SPrintfExpect::NormalChar;

  constexpr char formats[] = { 'd', 'i', 'u', 'o', 'x', 'X', 'c', 's', 'p' , 'n', 'q', 'y', 't' };

  auto is_format_char = [formats](char c) {
    for (int i = 0; i < ARRAY_LENGTH(formats); i++) {
      if (c == formats[i]) return true;
    }
    return false;
  };

  auto format_base_get = [](char c) -> uint8_t {
    switch (c) {
    case 'b': return BINARY;
    case 'o': return OCTAL;
    case 'd': return DECIMAL;
    case 'x':
    case 'X': return HEXADECIMAL;
    default: return DECIMAL;
    }};

  auto toupper_if_needed = [](char c, char* str) -> char* {if (c >= 'A' && c <= 'Z') { toupper(str); } return str;};

  // n--; // last character cant be overriden

  uint32_t fmt_index = 0;
  auto fmts = fmt_parse(fmt);

  int di = 0;
  for (int si = 0; fmt[si] != '\0' && di <= n; )
  {
    switch (expect)
    {

    case SPrintfExpect::NormalChar: {

      if (fmt[si + 1] == '%' && fmt[si] == '%') {
        str[di++] = '%';
        si += 2;
      }

      else if (fmt[si] == '%') {
        expect = SPrintfExpect::Format;
        si++;
      }

      else {
        str[di++] = fmt[si++];
      }

      break;
    }

    case SPrintfExpect::Format:
    {
      dbg_info("fmt.alignment_count", xsprintf("       ", "%d", fmts[fmt_index].alignment_count));
      dbg_info("fmt.alignment_char", xsprintf("       ", "'%c'", fmts[fmt_index].alignment_char));
      dbg_info("fmt.entry_length", xsprintf("       ", "%d", fmts[fmt_index].entry_lenght));
      dbg_info("fmt.fmt", xsprintf("       ", "'%c'", fmts[fmt_index].fmt));

      char st[64] = { 0 };
      memset(&str[di], fmts[fmt_index].alignment_char, fmts[fmt_index].alignment_count); //fill with filler 

      switch (fmts[fmt_index].fmt)
      {

      case '\0': {
        dbg_error("ugh", "ugh");
        break;
      }

      case 'c': {
        str[di + (fmts[fmt_index].alignment_count > 1 ? fmts[fmt_index].alignment_count - 1 : 0)] = (char)va_arg(args, uint32_t);
        di++;
        break;
      }

      case 's': {
        char* sa = va_arg(args, char*); // sa = string_argument
        uint32_t sa_length = strlen(sa);
        strncpy(&str[di + (sa_length >= fmts[fmt_index].alignment_count ? 0 : (fmts[fmt_index].alignment_count - sa_length))], sa, n - di);

        di = di + (sa_length > fmts[fmt_index].alignment_count ? sa_length : fmts[fmt_index].alignment_count);
        break;
      }

      case 'q': {
        //%h is used to print BCD digits
        constexpr uint32_t bcd_length = 2;
        bcd_to_string((uint8_t)va_arg(args, int), st);

        strncpy(&str[di + (bcd_length >= fmts[fmt_index].alignment_count ? 0 : (fmts[fmt_index].alignment_count - bcd_length))], st, n - di);

        di = di + (bcd_length > fmts[fmt_index].alignment_count ? bcd_length : fmts[fmt_index].alignment_count);
        break;
      }

      case 'u': {
        int_to_decimal_string(STRING_UNSIGNED, va_arg(args, int), st);
        uint32_t st_length = strlen(st);
        strncpy(&str[di + (st_length >= fmts[fmt_index].alignment_count ? 0 : (fmts[fmt_index].alignment_count - st_length))], st, n - di);
        di = di + (st_length > fmts[fmt_index].alignment_count ? st_length : fmts[fmt_index].alignment_count);
        break;
      }

      case 'n': {
        //weird character counter
        *(va_arg(args, uint32_t*)) = di;
        break;
      }

      case 'y': {
        //date
        constexpr uint32_t date_length = 10;
        date_to_string(va_arg(args, bcd_date_t), st);

        strncpy(&str[di + (date_length >= fmts[fmt_index].alignment_count ? 0 : (fmts[fmt_index].alignment_count - date_length))], st, n - di);

        di = di + (date_length > fmts[fmt_index].alignment_count ? date_length : fmts[fmt_index].alignment_count);
        break;
      }

      case 't': {
        //time
        constexpr uint32_t time_length = 5;
        time_to_string((bcd_time_t)va_arg(args, int), st);

        strncpy(&str[di + (time_length >= fmts[fmt_index].alignment_count ? 0 : (fmts[fmt_index].alignment_count - time_length))], st, n - di);

        di = di + (time_length > fmts[fmt_index].alignment_count ? time_length : fmts[fmt_index].alignment_count);
        break;
      }

      default: {
        int_to_string(va_arg(args, int), st, format_base_get(fmts[fmt_index].fmt));
        uint32_t st_length = strlen(st);
        toupper_if_needed(fmts[fmt_index].fmt, st);

        strncpy(&str[di + (st_length >= fmts[fmt_index].alignment_count ? 0 : (fmts[fmt_index].alignment_count - st_length))], st, n - di);
        di = di + (st_length > fmts[fmt_index].alignment_count ? st_length : fmts[fmt_index].alignment_count);
        // break;
        return str;
      }
      }

      si = si + fmts[fmt_index].entry_lenght;
      fmt_index++;
      expect = SPrintfExpect::NormalChar;

      break;
    }
    }
  }

  dbg_warning("index", xsprintf("    ", "%d", di));

  str[di] = '\0';
  return str;
}

char* new_xsprintf(char* str, char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);

  return new_xvsnprintf(str, UINT32_MAX, fmt, args);
}