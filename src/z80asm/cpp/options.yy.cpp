// cpp/options.yy.cpp generated by reflex 2.0.1 from cpp/options.l

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  OPTIONS USED                                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#define REFLEX_OPTION_dotall              true
#define REFLEX_OPTION_fast                true
#define REFLEX_OPTION_freespace           true
#define REFLEX_OPTION_header_file         "cpp/options.yy.h"
#define REFLEX_OPTION_lex                 lex
#define REFLEX_OPTION_lexer               OptionsLexer
#define REFLEX_OPTION_noline              true
#define REFLEX_OPTION_outfile             "cpp/options.yy.cpp"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 1: %top user code                                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


// silence warnings from RE-flex
#ifdef _MSC_VER
#pragma warning(disable:4102)
#pragma warning(disable:4800)
#else
#ifdef __GNUC__
//#pragma GCC   diagnostic ignored "-Wignored-qualifiers"
#else
#ifdef __clang__
//#pragma clang diagnostic ignored "-Wignored-qualifiers"
#endif
#endif
#endif


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  REGEX MATCHER                                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/matcher.h>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  ABSTRACT LEXER CLASS                                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/abslexer.h>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  LEXER CLASS                                                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

class OptionsLexer : public reflex::AbstractLexer<reflex::Matcher> {
 public:
  typedef reflex::AbstractLexer<reflex::Matcher> AbstractBaseLexer;
  OptionsLexer(
      const reflex::Input& input = reflex::Input(),
      std::ostream&        os    = std::cout)
    :
      AbstractBaseLexer(input, os)
  {
  }
  static const int INITIAL = 0;
  virtual int lex();
  int lex(
      const reflex::Input& input,
      std::ostream        *os = NULL)
  {
    in(input);
    if (os)
      out(*os);
    return lex();
  }
};

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 1: %{ user code %}                                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------


#include "App.h"

#include <string>
#include <utility>
#include <vector>

#include <cassert>
#include <climits>
#include <cstring>
#include <cstdlib>

static int parseNum(const std::string& text);
static void optionDefine(const std::string& text);
static const char* skip_equal(const char* text);


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 2: rules                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

extern void reflex_code_INITIAL(reflex::Matcher&);

int OptionsLexer::lex()
{
  static const reflex::Pattern PATTERN_INITIAL(reflex_code_INITIAL);
  if (!has_matcher())
  {
    matcher(new Matcher(PATTERN_INITIAL, stdinit(), this));
  }
  while (true)
  {
        switch (matcher().scan())
        {
          case 0:
            if (matcher().at_end())
            {
              return int();
            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule at line 51: -\?\z|-h\z :
{ App::ExitManual(); }
            break;
          case 2: // rule at line 52: -v\z :
{ app.options.verbose = true; return true; }
            break;
          case 3: // rule at line 53: -m\z :
{ app.options.mapfile = true; return true; }
            break;
          case 4: // rule at line 54: -s\z :
{ app.options.symtable = true; return true; }
            break;
          case 5: // rule at line 55: -l\z :
{ app.options.listfile = true; return true; }
            break;
          case 6: // rule at line 56: -g\z :
{ app.options.globaldef = true; return true; }
            break;
          case 7: // rule at line 57: -b\z :
{ app.options.makeBinary = true; return true; }
            break;
          case 8: // rule at line 58: -d\z :
{ app.options.update = true; return true; }
            break;
          case 9: // rule at line 59: -R\z :
{ app.options.relocatable = true; return true; }
            break;
          case 10: // rule at line 60: -reloc-info\z :
{ app.options.relocInfo = true; return true; }
            break;
          case 11: // rule at line 61: -split-bin\z :
{ app.options.splitBinary = true; return true; }

            break;
          case 12: // rule at line 63: -m=?z80\z :
{ app.options.cpu.Init(Cpu::Type::Z80); return true; }
            break;
          case 13: // rule at line 64: -m=?z80n\z :
{ app.options.cpu.Init(Cpu::Type::Z80N); return true; }
            break;
          case 14: // rule at line 65: -m=?z180\z :
{ app.options.cpu.Init(Cpu::Type::Z180); return true; }
            break;
          case 15: // rule at line 66: -m=?r2k\z :
{ app.options.cpu.Init(Cpu::Type::R2K); return true; }
            break;
          case 16: // rule at line 67: -m=?r3k\z :
{ app.options.cpu.Init(Cpu::Type::R3K); return true; }
            break;
          case 17: // rule at line 68: -m=?8080\z :
{ app.options.cpu.Init(Cpu::Type::I8080); return true; }
            break;
          case 18: // rule at line 69: -m=?8085\z :
{ app.options.cpu.Init(Cpu::Type::I8085); return true; }
            break;
          case 19: // rule at line 70: -m=?gbz80\z :
{ app.options.cpu.Init(Cpu::Type::GBZ80); return true; }
            break;
          case 20: // rule at line 71: -m=?ti83\z :
{ app.options.cpu.Init(Cpu::Type::Z80);
				  app.options.arch.Init(Arch::Type::TI83);
				  return true; }
            break;
          case 21: // rule at line 74: -m=?ti83plus\z :
{
				  app.options.cpu.Init(Cpu::Type::Z80);
				  app.options.arch.Init(Arch::Type::TI83PLUS);
				  return true; }
            break;
          case 22: // rule at line 78: -IXIY\z :
{ app.options.swapIxIy = true; return true; }
            break;
          case 23: // rule at line 79: -opt-speed\z :
{ app.options.optimizeSpeed = true; return true; }
            break;
          case 24: // rule at line 80: -debug\z :
{ app.options.debugInfo = true;
				  app.options.mapfile = true;
				  return true; }
            break;
          case 25: // rule at line 83: -I=?[\x00-\xff]+\z :
{ app.options.includePath.push_back(skip_equal(text() + 2));  return true; }

            break;
          case 26: // rule at line 85: -L=?[\x00-\xff]+\z :
{ app.options.libraryPath.push_back(skip_equal(text() + 2));  return true; }
            break;
          case 27: // rule at line 86: -x=?[\x00-\xff]+\z :
{ app.options.outputLibrary = skip_equal(text() + 2); return true; }
            break;
          case 28: // rule at line 87: -l=?[\x00-\xff]+\z :
{ app.options.libraries.push_back(skip_equal(text() + 2)); return true; }

            break;
          case 29: // rule at line 89: -O=?[\x00-\xff]+\z :
{ app.options.outputDirectory = skip_equal(text() + 2); return true; }
            break;
          case 30: // rule at line 90: -o=?[\x00-\xff]+\z :
{ app.options.outputFile = skip_equal(text() + 2); return true; }

            break;
          case 31: // rule at line 92: -D=?(?:[A-Z_a-z][0-9A-Z_a-z]*)(?:=(?:(?:0[Xx][0-9A-Fa-f]+|\$[0-9A-Fa-f]+|[0-9][0-9A-Fa-f]*[Hh])|(?:[0-9]+)))?\z :
{
				  optionDefine(skip_equal(text() + 2)); return true; }

            break;
          case 32: // rule at line 95: [\x00-\xff] :
{ return false; }

            break;
        }
  }
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 3: user code                                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


// parse NUM
static int parseNum(const std::string& text) {
	char *end = nullptr;
	const char *p = text.c_str();
	long lval;
	int radix;
	char suffix = '\0';

	if (p[0] == '\0') {		// empty
		return -1;
	}
	else if (p[0] == '$') {
		p++;
		radix = 16;
	}
	else if (p[0] == '0' && tolower(p[1]) == 'x') {
		p += 2;
		radix = 16;
	}
	else if (isdigit(p[0]) && tolower(p[strlen(p)-1]) == 'h') {
		suffix = p[strlen(p) - 1];
		radix = 16;
	}
	else {
		radix = 10;
	}

	lval = strtol(p, &end, radix);
	if (end == nullptr || *end != suffix || errno == ERANGE || lval < 0 || lval > INT_MAX)
		return -1;
	else
		return static_cast<int>(lval);
}

static void optionDefine(const std::string& text) {
	using namespace std;

	auto p = text.find('=');
	if (p == string::npos) {		// -Dvar
		app.options.defines.push_back({ text, 1});
	}
	else {							// -Dvar=value
		string name = text.substr(0, p);
		int num = parseNum(text.c_str() + p + 1);
		assert(num >= 0);

		app.options.defines.push_back({ name, num});
	}
}

static const char* skip_equal(const char* text) {
	if (*text == '=')
		text++;
	return text;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  TABLES                                                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/matcher.h>

#if defined(OS_WIN)
#pragma warning(disable:4101 4102)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-label"
#elif defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-label"
#endif

void reflex_code_INITIAL(reflex::Matcher& m)
{
  int c0 = 0, c1 = 0;
  m.FSM_INIT(c1);

S0:
  m.FSM_FIND();
  c1 = m.FSM_CHAR();
  if (c1 == '-') goto S2;
  if (0 <= c1) goto S21;
  return m.FSM_HALT(c1);

S2:
  m.FSM_TAKE(32);
  c1 = m.FSM_CHAR();
  if (c1 == 'x') goto S58;
  if (c1 == 'v') goto S27;
  if (c1 == 's') goto S37;
  if (c1 == 'r') goto S51;
  if (c1 == 'o') goto S55;
  if (c1 == 'm') goto S29;
  if (c1 == 'l') goto S40;
  if (c1 == 'h') goto S25;
  if (c1 == 'g') goto S42;
  if (c1 == 'd') goto S46;
  if (c1 == 'b') goto S44;
  if (c1 == 'R') goto S49;
  if (c1 == 'O') goto S59;
  if (c1 == 'L') goto S57;
  if (c1 == 'I') goto S53;
  if (c1 == 'D') goto S60;
  if (c1 == '?') goto S23;
  return m.FSM_HALT(c1);

S21:
  m.FSM_TAKE(32);
  return m.FSM_HALT();

S23:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(1, c1);
  }
  return m.FSM_HALT(c1);

S25:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(1, c1);
  }
  return m.FSM_HALT(c1);

S27:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(2, c1);
  }
  return m.FSM_HALT(c1);

S29:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(3, c1);
  }
  if (c1 == 'z') goto S71;
  if (c1 == 't') goto S81;
  if (c1 == 'r') goto S74;
  if (c1 == 'g') goto S79;
  if (c1 == '=') goto S83;
  if (c1 == '8') goto S77;
  return m.FSM_HALT(c1);

S37:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(4, c1);
  }
  if (c1 == 'p') goto S91;
  return m.FSM_HALT(c1);

S40:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(5, c1);
  }
  if (0 <= c1) goto S95;
  return m.FSM_HALT(c1);

S42:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(6, c1);
  }
  return m.FSM_HALT(c1);

S44:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(7, c1);
  }
  return m.FSM_HALT(c1);

S46:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(8, c1);
  }
  if (c1 == 'e') goto S103;
  return m.FSM_HALT(c1);

S49:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(9, c1);
  }
  return m.FSM_HALT(c1);

S51:
  c1 = m.FSM_CHAR();
  if (c1 == 'e') goto S107;
  return m.FSM_HALT(c1);

S53:
  c1 = m.FSM_CHAR();
  if (c1 == 'X') goto S109;
  if (0 <= c1) goto S112;
  return m.FSM_HALT(c1);

S55:
  c1 = m.FSM_CHAR();
  if (c1 == 'p') goto S114;
  if (0 <= c1) goto S117;
  return m.FSM_HALT(c1);

S57:
  c1 = m.FSM_CHAR();
  if (0 <= c1) goto S119;
  return m.FSM_HALT(c1);

S58:
  c1 = m.FSM_CHAR();
  if (0 <= c1) goto S121;
  return m.FSM_HALT(c1);

S59:
  c1 = m.FSM_CHAR();
  if (0 <= c1) goto S123;
  return m.FSM_HALT(c1);

S60:
  c1 = m.FSM_CHAR();
  if ('a' <= c1 && c1 <= 'z') goto S125;
  if (c1 == '_') goto S125;
  if ('A' <= c1 && c1 <= 'Z') goto S125;
  if (c1 == '=') goto S132;
  return m.FSM_HALT(c1);

S65:
  m.FSM_TAKE(1);
  return m.FSM_HALT();

S67:
  m.FSM_TAKE(2);
  return m.FSM_HALT();

S69:
  m.FSM_TAKE(3);
  return m.FSM_HALT();

S71:
  c1 = m.FSM_CHAR();
  if (c1 == '8') goto S136;
  if (c1 == '1') goto S138;
  return m.FSM_HALT(c1);

S74:
  c1 = m.FSM_CHAR();
  if (c1 == '3') goto S142;
  if (c1 == '2') goto S140;
  return m.FSM_HALT(c1);

S77:
  c1 = m.FSM_CHAR();
  if (c1 == '0') goto S144;
  return m.FSM_HALT(c1);

S79:
  c1 = m.FSM_CHAR();
  if (c1 == 'b') goto S146;
  return m.FSM_HALT(c1);

S81:
  c1 = m.FSM_CHAR();
  if (c1 == 'i') goto S148;
  return m.FSM_HALT(c1);

S83:
  c1 = m.FSM_CHAR();
  if (c1 == 'z') goto S71;
  if (c1 == 't') goto S81;
  if (c1 == 'r') goto S74;
  if (c1 == 'g') goto S79;
  if (c1 == '8') goto S77;
  return m.FSM_HALT(c1);

S89:
  m.FSM_TAKE(4);
  return m.FSM_HALT();

S91:
  c1 = m.FSM_CHAR();
  if (c1 == 'l') goto S150;
  return m.FSM_HALT(c1);

S93:
  m.FSM_TAKE(5);
  return m.FSM_HALT();

S95:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(28, c1);
  }
  if (0 <= c1) goto S95;
  return m.FSM_HALT(c1);

S97:
  m.FSM_TAKE(6);
  return m.FSM_HALT();

S99:
  m.FSM_TAKE(7);
  return m.FSM_HALT();

S101:
  m.FSM_TAKE(8);
  return m.FSM_HALT();

S103:
  c1 = m.FSM_CHAR();
  if (c1 == 'b') goto S154;
  return m.FSM_HALT(c1);

S105:
  m.FSM_TAKE(9);
  return m.FSM_HALT();

S107:
  c1 = m.FSM_CHAR();
  if (c1 == 'l') goto S156;
  return m.FSM_HALT(c1);

S109:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(25, c1);
  }
  if (c1 == 'I') goto S158;
  if (0 <= c1) goto S112;
  return m.FSM_HALT(c1);

S112:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(25, c1);
  }
  if (0 <= c1) goto S112;
  return m.FSM_HALT(c1);

S114:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(30, c1);
  }
  if (c1 == 't') goto S163;
  if (0 <= c1) goto S117;
  return m.FSM_HALT(c1);

S117:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(30, c1);
  }
  if (0 <= c1) goto S117;
  return m.FSM_HALT(c1);

S119:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(26, c1);
  }
  if (0 <= c1) goto S119;
  return m.FSM_HALT(c1);

S121:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(27, c1);
  }
  if (0 <= c1) goto S121;
  return m.FSM_HALT(c1);

S123:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(29, c1);
  }
  if (0 <= c1) goto S123;
  return m.FSM_HALT(c1);

S125:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(31, c1);
  }
  if ('a' <= c1 && c1 <= 'z') goto S125;
  if (c1 == '_') goto S125;
  if ('A' <= c1 && c1 <= 'Z') goto S125;
  if (c1 == '=') goto S176;
  if ('0' <= c1 && c1 <= '9') goto S125;
  return m.FSM_HALT(c1);

S132:
  c1 = m.FSM_CHAR();
  if ('a' <= c1 && c1 <= 'z') goto S125;
  if (c1 == '_') goto S125;
  if ('A' <= c1 && c1 <= 'Z') goto S125;
  return m.FSM_HALT(c1);

S136:
  c1 = m.FSM_CHAR();
  if (c1 == '0') goto S180;
  return m.FSM_HALT(c1);

S138:
  c1 = m.FSM_CHAR();
  if (c1 == '8') goto S183;
  return m.FSM_HALT(c1);

S140:
  c1 = m.FSM_CHAR();
  if (c1 == 'k') goto S185;
  return m.FSM_HALT(c1);

S142:
  c1 = m.FSM_CHAR();
  if (c1 == 'k') goto S187;
  return m.FSM_HALT(c1);

S144:
  c1 = m.FSM_CHAR();
  if (c1 == '8') goto S189;
  return m.FSM_HALT(c1);

S146:
  c1 = m.FSM_CHAR();
  if (c1 == 'z') goto S192;
  return m.FSM_HALT(c1);

S148:
  c1 = m.FSM_CHAR();
  if (c1 == '8') goto S194;
  return m.FSM_HALT(c1);

S150:
  c1 = m.FSM_CHAR();
  if (c1 == 'i') goto S196;
  return m.FSM_HALT(c1);

S152:
  m.FSM_TAKE(28);
  return m.FSM_HALT();

S154:
  c1 = m.FSM_CHAR();
  if (c1 == 'u') goto S198;
  return m.FSM_HALT(c1);

S156:
  c1 = m.FSM_CHAR();
  if (c1 == 'o') goto S200;
  return m.FSM_HALT(c1);

S158:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(25, c1);
  }
  if (c1 == 'Y') goto S202;
  if (0 <= c1) goto S112;
  return m.FSM_HALT(c1);

S161:
  m.FSM_TAKE(25);
  return m.FSM_HALT();

S163:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(30, c1);
  }
  if (c1 == '-') goto S204;
  if (0 <= c1) goto S117;
  return m.FSM_HALT(c1);

S166:
  m.FSM_TAKE(30);
  return m.FSM_HALT();

S168:
  m.FSM_TAKE(26);
  return m.FSM_HALT();

S170:
  m.FSM_TAKE(27);
  return m.FSM_HALT();

S172:
  m.FSM_TAKE(29);
  return m.FSM_HALT();

S174:
  m.FSM_TAKE(31);
  return m.FSM_HALT();

S176:
  c1 = m.FSM_CHAR();
  if ('1' <= c1 && c1 <= '9') goto S220;
  if (c1 == '0') goto S207;
  if (c1 == '$') goto S216;
  return m.FSM_HALT(c1);

S180:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(12, c1);
  }
  if (c1 == 'n') goto S229;
  return m.FSM_HALT(c1);

S183:
  c1 = m.FSM_CHAR();
  if (c1 == '0') goto S231;
  return m.FSM_HALT(c1);

S185:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(15, c1);
  }
  return m.FSM_HALT(c1);

S187:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(16, c1);
  }
  return m.FSM_HALT(c1);

S189:
  c1 = m.FSM_CHAR();
  if (c1 == '5') goto S239;
  if (c1 == '0') goto S237;
  return m.FSM_HALT(c1);

S192:
  c1 = m.FSM_CHAR();
  if (c1 == '8') goto S241;
  return m.FSM_HALT(c1);

S194:
  c1 = m.FSM_CHAR();
  if (c1 == '3') goto S243;
  return m.FSM_HALT(c1);

S196:
  c1 = m.FSM_CHAR();
  if (c1 == 't') goto S246;
  return m.FSM_HALT(c1);

S198:
  c1 = m.FSM_CHAR();
  if (c1 == 'g') goto S248;
  return m.FSM_HALT(c1);

S200:
  c1 = m.FSM_CHAR();
  if (c1 == 'c') goto S250;
  return m.FSM_HALT(c1);

S202:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(22, c1);
  }
  if (0 <= c1) goto S112;
  return m.FSM_HALT(c1);

S204:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(30, c1);
  }
  if (c1 == 's') goto S254;
  if (0 <= c1) goto S117;
  return m.FSM_HALT(c1);

S207:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(31, c1);
  }
  if (c1 == 'x') goto S257;
  if (c1 == 'h') goto S267;
  if ('a' <= c1 && c1 <= 'f') goto S261;
  if (c1 == 'X') goto S257;
  if (c1 == 'H') goto S267;
  if ('A' <= c1 && c1 <= 'F') goto S261;
  if ('0' <= c1 && c1 <= '9') goto S220;
  return m.FSM_HALT(c1);

S216:
  c1 = m.FSM_CHAR();
  if ('a' <= c1 && c1 <= 'f') goto S270;
  if ('A' <= c1 && c1 <= 'F') goto S270;
  if ('0' <= c1 && c1 <= '9') goto S270;
  return m.FSM_HALT(c1);

S220:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(31, c1);
  }
  if (c1 == 'h') goto S267;
  if ('a' <= c1 && c1 <= 'f') goto S261;
  if (c1 == 'H') goto S267;
  if ('A' <= c1 && c1 <= 'F') goto S261;
  if ('0' <= c1 && c1 <= '9') goto S220;
  return m.FSM_HALT(c1);

S227:
  m.FSM_TAKE(12);
  return m.FSM_HALT();

S229:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(13, c1);
  }
  return m.FSM_HALT(c1);

S231:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(14, c1);
  }
  return m.FSM_HALT(c1);

S233:
  m.FSM_TAKE(15);
  return m.FSM_HALT();

S235:
  m.FSM_TAKE(16);
  return m.FSM_HALT();

S237:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(17, c1);
  }
  return m.FSM_HALT(c1);

S239:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(18, c1);
  }
  return m.FSM_HALT(c1);

S241:
  c1 = m.FSM_CHAR();
  if (c1 == '0') goto S283;
  return m.FSM_HALT(c1);

S243:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(20, c1);
  }
  if (c1 == 'p') goto S287;
  return m.FSM_HALT(c1);

S246:
  c1 = m.FSM_CHAR();
  if (c1 == '-') goto S289;
  return m.FSM_HALT(c1);

S248:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(24, c1);
  }
  return m.FSM_HALT(c1);

S250:
  c1 = m.FSM_CHAR();
  if (c1 == '-') goto S293;
  return m.FSM_HALT(c1);

S252:
  m.FSM_TAKE(22);
  return m.FSM_HALT();

S254:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(30, c1);
  }
  if (c1 == 'p') goto S295;
  if (0 <= c1) goto S117;
  return m.FSM_HALT(c1);

S257:
  c1 = m.FSM_CHAR();
  if ('a' <= c1 && c1 <= 'f') goto S298;
  if ('A' <= c1 && c1 <= 'F') goto S298;
  if ('0' <= c1 && c1 <= '9') goto S298;
  return m.FSM_HALT(c1);

S261:
  c1 = m.FSM_CHAR();
  if (c1 == 'h') goto S303;
  if ('a' <= c1 && c1 <= 'f') goto S261;
  if (c1 == 'H') goto S303;
  if ('A' <= c1 && c1 <= 'F') goto S261;
  if ('0' <= c1 && c1 <= '9') goto S261;
  return m.FSM_HALT(c1);

S267:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(31, c1);
  }
  if ('0' <= c1 && c1 <= '9') goto S267;
  return m.FSM_HALT(c1);

S270:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(31, c1);
  }
  if ('a' <= c1 && c1 <= 'f') goto S270;
  if ('A' <= c1 && c1 <= 'F') goto S270;
  if ('0' <= c1 && c1 <= '9') goto S270;
  return m.FSM_HALT(c1);

S275:
  m.FSM_TAKE(13);
  return m.FSM_HALT();

S277:
  m.FSM_TAKE(14);
  return m.FSM_HALT();

S279:
  m.FSM_TAKE(17);
  return m.FSM_HALT();

S281:
  m.FSM_TAKE(18);
  return m.FSM_HALT();

S283:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(19, c1);
  }
  return m.FSM_HALT(c1);

S285:
  m.FSM_TAKE(20);
  return m.FSM_HALT();

S287:
  c1 = m.FSM_CHAR();
  if (c1 == 'l') goto S307;
  return m.FSM_HALT(c1);

S289:
  c1 = m.FSM_CHAR();
  if (c1 == 'b') goto S309;
  return m.FSM_HALT(c1);

S291:
  m.FSM_TAKE(24);
  return m.FSM_HALT();

S293:
  c1 = m.FSM_CHAR();
  if (c1 == 'i') goto S311;
  return m.FSM_HALT(c1);

S295:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(30, c1);
  }
  if (c1 == 'e') goto S313;
  if (0 <= c1) goto S117;
  return m.FSM_HALT(c1);

S298:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(31, c1);
  }
  if ('a' <= c1 && c1 <= 'f') goto S298;
  if ('A' <= c1 && c1 <= 'F') goto S298;
  if ('0' <= c1 && c1 <= '9') goto S298;
  return m.FSM_HALT(c1);

S303:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(31, c1);
  }
  return m.FSM_HALT(c1);

S305:
  m.FSM_TAKE(19);
  return m.FSM_HALT();

S307:
  c1 = m.FSM_CHAR();
  if (c1 == 'u') goto S316;
  return m.FSM_HALT(c1);

S309:
  c1 = m.FSM_CHAR();
  if (c1 == 'i') goto S318;
  return m.FSM_HALT(c1);

S311:
  c1 = m.FSM_CHAR();
  if (c1 == 'n') goto S320;
  return m.FSM_HALT(c1);

S313:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(30, c1);
  }
  if (c1 == 'e') goto S322;
  if (0 <= c1) goto S117;
  return m.FSM_HALT(c1);

S316:
  c1 = m.FSM_CHAR();
  if (c1 == 's') goto S325;
  return m.FSM_HALT(c1);

S318:
  c1 = m.FSM_CHAR();
  if (c1 == 'n') goto S327;
  return m.FSM_HALT(c1);

S320:
  c1 = m.FSM_CHAR();
  if (c1 == 'f') goto S329;
  return m.FSM_HALT(c1);

S322:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(30, c1);
  }
  if (c1 == 'd') goto S331;
  if (0 <= c1) goto S117;
  return m.FSM_HALT(c1);

S325:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(21, c1);
  }
  return m.FSM_HALT(c1);

S327:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(11, c1);
  }
  return m.FSM_HALT(c1);

S329:
  c1 = m.FSM_CHAR();
  if (c1 == 'o') goto S337;
  return m.FSM_HALT(c1);

S331:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(23, c1);
  }
  if (0 <= c1) goto S117;
  return m.FSM_HALT(c1);

S333:
  m.FSM_TAKE(21);
  return m.FSM_HALT();

S335:
  m.FSM_TAKE(11);
  return m.FSM_HALT();

S337:
  c1 = m.FSM_CHAR();
  if (m.FSM_META_EOB(c1)) {
    m.FSM_TAKE(10, c1);
  }
  return m.FSM_HALT(c1);

S339:
  m.FSM_TAKE(23);
  return m.FSM_HALT();

S341:
  m.FSM_TAKE(10);
  return m.FSM_HALT();
}

