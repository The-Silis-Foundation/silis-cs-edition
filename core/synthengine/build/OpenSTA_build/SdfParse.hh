// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton interface for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


/**
 ** \file /home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.hh
 ** Define the sta::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

#ifndef YY_YY_HOME_EATHESWAR_SILISFOUNDATION_SILIS_SYNTHENGINE_BUILD_OPENSTA_BUILD_SDFPARSE_HH_INCLUDED
# define YY_YY_HOME_EATHESWAR_SILISFOUNDATION_SILIS_SYNTHENGINE_BUILD_OPENSTA_BUILD_SDFPARSE_HH_INCLUDED
// "%code requires" blocks.
#line 47 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"

#include <string>
#include <vector>
#include "StringUtil.hh"

namespace sta {
class SdfScanner;
class SdfReader;
class Transition;
class SdfPortSpec;
class SdfTriple;

using SdfTripleSeq = std::vector<SdfTriple*>;
}

#line 65 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.hh"

# include <cassert>
# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

#if defined __cplusplus
# define YY_CPLUSPLUS __cplusplus
#else
# define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_NOEXCEPT noexcept
# define YY_NOTHROW
#else
# define YY_NOEXCEPT
# define YY_NOTHROW throw ()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
# define YY_CONSTEXPR constexpr
#else
# define YY_CONSTEXPR
#endif
# include "SdfLocation.hh"
#include <typeinfo>
#ifndef YY_ASSERT
# include <cassert>
# define YY_ASSERT assert
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

#line 66 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
namespace sta {
#line 206 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.hh"




  /// A Bison parser.
  class SdfParse
  {
  public:
#ifdef YYSTYPE
# ifdef __GNUC__
#  pragma GCC message "bison: do not #define YYSTYPE in C++, use %define api.value.type"
# endif
    typedef YYSTYPE value_type;
#else
  /// A buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current parser state.
  class value_type
  {
  public:
    /// Type of *this.
    typedef value_type self_type;

    /// Empty construction.
    value_type () YY_NOEXCEPT
      : yyraw_ ()
      , yytypeid_ (YY_NULLPTR)
    {}

    /// Construct and fill.
    template <typename T>
    value_type (YY_RVREF (T) t)
      : yytypeid_ (&typeid (T))
    {
      YY_ASSERT (sizeof (T) <= size);
      new (yyas_<T> ()) T (YY_MOVE (t));
    }

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    value_type (const self_type&) = delete;
    /// Non copyable.
    self_type& operator= (const self_type&) = delete;
#endif

    /// Destruction, allowed only if empty.
    ~value_type () YY_NOEXCEPT
    {
      YY_ASSERT (!yytypeid_);
    }

# if 201103L <= YY_CPLUSPLUS
    /// Instantiate a \a T in here from \a t.
    template <typename T, typename... U>
    T&
    emplace (U&&... u)
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (std::forward <U>(u)...);
    }
# else
    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    emplace ()
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T ();
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    emplace (const T& t)
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (t);
    }
# endif

    /// Instantiate an empty \a T in here.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build ()
    {
      return emplace<T> ();
    }

    /// Instantiate a \a T in here from \a t.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build (const T& t)
    {
      return emplace<T> (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as () YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == typeid (T));
      YY_ASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == typeid (T));
      YY_ASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Swap the content with \a that, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsibility.
    /// Swapping between built and (possibly) non-built is done with
    /// self_type::move ().
    template <typename T>
    void
    swap (self_type& that) YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == *that.yytypeid_);
      std::swap (as<T> (), that.as<T> ());
    }

    /// Move the content of \a that to this.
    ///
    /// Destroys \a that.
    template <typename T>
    void
    move (self_type& that)
    {
# if 201103L <= YY_CPLUSPLUS
      emplace<T> (std::move (that.as<T> ()));
# else
      emplace<T> ();
      swap<T> (that);
# endif
      that.destroy<T> ();
    }

# if 201103L <= YY_CPLUSPLUS
    /// Move the content of \a that to this.
    template <typename T>
    void
    move (self_type&& that)
    {
      emplace<T> (std::move (that.as<T> ()));
      that.destroy<T> ();
    }
#endif

    /// Copy the content of \a that to this.
    template <typename T>
    void
    copy (const self_type& that)
    {
      emplace<T> (that.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
      yytypeid_ = YY_NULLPTR;
    }

  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    value_type (const self_type&);
    /// Non copyable.
    self_type& operator= (const self_type&);
#endif

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ () YY_NOEXCEPT
    {
      void *yyp = yyraw_;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const YY_NOEXCEPT
    {
      const void *yyp = yyraw_;
      return static_cast<const T*> (yyp);
     }

    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // hchar
      char dummy1[sizeof (char)];

      // port_transition
      char dummy2[sizeof (const sta::Transition *)];

      // FNUMBER
      // NUMBER
      char dummy3[sizeof (float)];

      // number_opt
      char dummy4[sizeof (float*)];

      // DNUMBER
      char dummy5[sizeof (int)];

      // port_spec
      // port_tchk
      char dummy6[sizeof (sta::SdfPortSpec *)];

      // value
      // triple
      char dummy7[sizeof (sta::SdfTriple *)];

      // delval_list
      char dummy8[sizeof (sta::SdfTripleSeq *)];

      // QSTRING
      // ID
      // EXPR_OPEN_IOPATH
      // EXPR_OPEN
      // EXPR_ID_CLOSE
      // path
      // port
      // port_instance
      char dummy9[sizeof (std::string)];
    };

    /// The size of the largest semantic type.
    enum { size = sizeof (union_type) };

    /// A buffer to store semantic values.
    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me_;
      /// A buffer large enough to store any of the semantic values.
      char yyraw_[size];
    };

    /// Whether the content is built: if defined, the name of the stored type.
    const std::type_info *yytypeid_;
  };

#endif
    /// Backward compatibility (Bison 3.8).
    typedef value_type semantic_type;

    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m)
        : std::runtime_error (m)
        , location (l)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
        , location (s.location)
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;

      location_type location;
    };

    /// Token kinds.
    struct token
    {
      enum token_kind_type
      {
        YYEMPTY = -2,
    YYEOF = 0,                     // "end of file"
    YYerror = 256,                 // error
    YYUNDEF = 257,                 // "invalid token"
    QSTRING = 258,                 // QSTRING
    ID = 259,                      // ID
    EXPR_OPEN_IOPATH = 260,        // EXPR_OPEN_IOPATH
    EXPR_OPEN = 261,               // EXPR_OPEN
    EXPR_ID_CLOSE = 262,           // EXPR_ID_CLOSE
    FNUMBER = 263,                 // FNUMBER
    DNUMBER = 264,                 // DNUMBER
    DELAYFILE = 265,               // DELAYFILE
    SDFVERSION = 266,              // SDFVERSION
    DESIGN = 267,                  // DESIGN
    DATE = 268,                    // DATE
    VENDOR = 269,                  // VENDOR
    PROGRAM = 270,                 // PROGRAM
    PVERSION = 271,                // PVERSION
    DIVIDER = 272,                 // DIVIDER
    VOLTAGE = 273,                 // VOLTAGE
    PROCESS = 274,                 // PROCESS
    TEMPERATURE = 275,             // TEMPERATURE
    TIMESCALE = 276,               // TIMESCALE
    CELL = 277,                    // CELL
    CELLTYPE = 278,                // CELLTYPE
    INSTANCE = 279,                // INSTANCE
    DELAY = 280,                   // DELAY
    ABSOLUTE = 281,                // ABSOLUTE
    INCREMENTAL = 282,             // INCREMENTAL
    INTERCONNECT = 283,            // INTERCONNECT
    PORT = 284,                    // PORT
    DEVICE = 285,                  // DEVICE
    RETAIN = 286,                  // RETAIN
    IOPATH = 287,                  // IOPATH
    TIMINGCHECK = 288,             // TIMINGCHECK
    SETUP = 289,                   // SETUP
    HOLD = 290,                    // HOLD
    SETUPHOLD = 291,               // SETUPHOLD
    RECOVERY = 292,                // RECOVERY
    REMOVAL = 293,                 // REMOVAL
    RECREM = 294,                  // RECREM
    WIDTH = 295,                   // WIDTH
    PERIOD = 296,                  // PERIOD
    SKEW = 297,                    // SKEW
    NOCHANGE = 298,                // NOCHANGE
    POSEDGE = 299,                 // POSEDGE
    NEGEDGE = 300,                 // NEGEDGE
    COND = 301,                    // COND
    CONDELSE = 302                 // CONDELSE
      };
      /// Backward compatibility alias (Bison 3.6).
      typedef token_kind_type yytokentype;
    };

    /// Token kind, as returned by yylex.
    typedef token::token_kind_type token_kind_type;

    /// Backward compatibility alias (Bison 3.6).
    typedef token_kind_type token_type;

    /// Symbol kinds.
    struct symbol_kind
    {
      enum symbol_kind_type
      {
        YYNTOKENS = 57, ///< Number of tokens.
        S_YYEMPTY = -2,
        S_YYEOF = 0,                             // "end of file"
        S_YYerror = 1,                           // error
        S_YYUNDEF = 2,                           // "invalid token"
        S_QSTRING = 3,                           // QSTRING
        S_ID = 4,                                // ID
        S_EXPR_OPEN_IOPATH = 5,                  // EXPR_OPEN_IOPATH
        S_EXPR_OPEN = 6,                         // EXPR_OPEN
        S_EXPR_ID_CLOSE = 7,                     // EXPR_ID_CLOSE
        S_FNUMBER = 8,                           // FNUMBER
        S_DNUMBER = 9,                           // DNUMBER
        S_DELAYFILE = 10,                        // DELAYFILE
        S_SDFVERSION = 11,                       // SDFVERSION
        S_DESIGN = 12,                           // DESIGN
        S_DATE = 13,                             // DATE
        S_VENDOR = 14,                           // VENDOR
        S_PROGRAM = 15,                          // PROGRAM
        S_PVERSION = 16,                         // PVERSION
        S_DIVIDER = 17,                          // DIVIDER
        S_VOLTAGE = 18,                          // VOLTAGE
        S_PROCESS = 19,                          // PROCESS
        S_TEMPERATURE = 20,                      // TEMPERATURE
        S_TIMESCALE = 21,                        // TIMESCALE
        S_CELL = 22,                             // CELL
        S_CELLTYPE = 23,                         // CELLTYPE
        S_INSTANCE = 24,                         // INSTANCE
        S_DELAY = 25,                            // DELAY
        S_ABSOLUTE = 26,                         // ABSOLUTE
        S_INCREMENTAL = 27,                      // INCREMENTAL
        S_INTERCONNECT = 28,                     // INTERCONNECT
        S_PORT = 29,                             // PORT
        S_DEVICE = 30,                           // DEVICE
        S_RETAIN = 31,                           // RETAIN
        S_IOPATH = 32,                           // IOPATH
        S_TIMINGCHECK = 33,                      // TIMINGCHECK
        S_SETUP = 34,                            // SETUP
        S_HOLD = 35,                             // HOLD
        S_SETUPHOLD = 36,                        // SETUPHOLD
        S_RECOVERY = 37,                         // RECOVERY
        S_REMOVAL = 38,                          // REMOVAL
        S_RECREM = 39,                           // RECREM
        S_WIDTH = 40,                            // WIDTH
        S_PERIOD = 41,                           // PERIOD
        S_SKEW = 42,                             // SKEW
        S_NOCHANGE = 43,                         // NOCHANGE
        S_POSEDGE = 44,                          // POSEDGE
        S_NEGEDGE = 45,                          // NEGEDGE
        S_COND = 46,                             // COND
        S_CONDELSE = 47,                         // CONDELSE
        S_48_ = 48,                              // '('
        S_49_ = 49,                              // ')'
        S_50_ = 50,                              // '/'
        S_51_ = 51,                              // '.'
        S_52_ = 52,                              // '*'
        S_53_ = 53,                              // '['
        S_54_ = 54,                              // ']'
        S_55_ = 55,                              // ':'
        S_56_ = 56,                              // '-'
        S_YYACCEPT = 57,                         // $accept
        S_file = 58,                             // file
        S_header = 59,                           // header
        S_header_stmt = 60,                      // header_stmt
        S_hchar = 61,                            // hchar
        S_number_opt = 62,                       // number_opt
        S_cells = 63,                            // cells
        S_cell = 64,                             // cell
        S_celltype = 65,                         // celltype
        S_cell_instance = 66,                    // cell_instance
        S_timing_specs = 67,                     // timing_specs
        S_timing_spec = 68,                      // timing_spec
        S_deltypes = 69,                         // deltypes
        S_deltype = 70,                          // deltype
        S_71_1 = 71,                             // $@1
        S_72_2 = 72,                             // $@2
        S_del_defs = 73,                         // del_defs
        S_path = 74,                             // path
        S_del_def = 75,                          // del_def
        S_retains = 76,                          // retains
        S_retain = 77,                           // retain
        S_delval_list = 78,                      // delval_list
        S_tchk_defs = 79,                        // tchk_defs
        S_tchk_def = 80,                         // tchk_def
        S_81_3 = 81,                             // $@3
        S_82_4 = 82,                             // $@4
        S_83_5 = 83,                             // $@5
        S_84_6 = 84,                             // $@6
        S_85_7 = 85,                             // $@7
        S_86_8 = 86,                             // $@8
        S_87_9 = 87,                             // $@9
        S_88_10 = 88,                            // $@10
        S_89_11 = 89,                            // $@11
        S_90_12 = 90,                            // $@12
        S_port = 91,                             // port
        S_port_instance = 92,                    // port_instance
        S_port_spec = 93,                        // port_spec
        S_port_transition = 94,                  // port_transition
        S_port_tchk = 95,                        // port_tchk
        S_value = 96,                            // value
        S_triple = 97,                           // triple
        S_NUMBER = 98                            // NUMBER
      };
    };

    /// (Internal) symbol kind.
    typedef symbol_kind::symbol_kind_type symbol_kind_type;

    /// The number of tokens.
    static const symbol_kind_type YYNTOKENS = symbol_kind::YYNTOKENS;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol kind
    /// via kind ().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol () YY_NOEXCEPT
        : value ()
        , location ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that)
        : Base (std::move (that))
        , value ()
        , location (std::move (that.location))
      {
        switch (this->kind ())
    {
      case symbol_kind::S_hchar: // hchar
        value.move< char > (std::move (that.value));
        break;

      case symbol_kind::S_port_transition: // port_transition
        value.move< const sta::Transition * > (std::move (that.value));
        break;

      case symbol_kind::S_FNUMBER: // FNUMBER
      case symbol_kind::S_NUMBER: // NUMBER
        value.move< float > (std::move (that.value));
        break;

      case symbol_kind::S_number_opt: // number_opt
        value.move< float* > (std::move (that.value));
        break;

      case symbol_kind::S_DNUMBER: // DNUMBER
        value.move< int > (std::move (that.value));
        break;

      case symbol_kind::S_port_spec: // port_spec
      case symbol_kind::S_port_tchk: // port_tchk
        value.move< sta::SdfPortSpec * > (std::move (that.value));
        break;

      case symbol_kind::S_value: // value
      case symbol_kind::S_triple: // triple
        value.move< sta::SdfTriple * > (std::move (that.value));
        break;

      case symbol_kind::S_delval_list: // delval_list
        value.move< sta::SdfTripleSeq * > (std::move (that.value));
        break;

      case symbol_kind::S_QSTRING: // QSTRING
      case symbol_kind::S_ID: // ID
      case symbol_kind::S_EXPR_OPEN_IOPATH: // EXPR_OPEN_IOPATH
      case symbol_kind::S_EXPR_OPEN: // EXPR_OPEN
      case symbol_kind::S_EXPR_ID_CLOSE: // EXPR_ID_CLOSE
      case symbol_kind::S_path: // path
      case symbol_kind::S_port: // port
      case symbol_kind::S_port_instance: // port_instance
        value.move< std::string > (std::move (that.value));
        break;

      default:
        break;
    }

      }
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);

      /// Constructors for typed symbols.
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, location_type&& l)
        : Base (t)
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const location_type& l)
        : Base (t)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, char&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const char& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, const sta::Transition *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const const sta::Transition *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, float&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const float& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, float*&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const float*& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, int&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const int& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sta::SdfPortSpec *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sta::SdfPortSpec *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sta::SdfTriple *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sta::SdfTriple *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sta::SdfTripleSeq *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sta::SdfTripleSeq *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::string&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::string& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }



      /// Destroy contents, and record that is empty.
      void clear () YY_NOEXCEPT
      {
        // User destructor.
        symbol_kind_type yykind = this->kind ();
        basic_symbol<Base>& yysym = *this;
        (void) yysym;
        switch (yykind)
        {
       default:
          break;
        }

        // Value type destructor.
switch (yykind)
    {
      case symbol_kind::S_hchar: // hchar
        value.template destroy< char > ();
        break;

      case symbol_kind::S_port_transition: // port_transition
        value.template destroy< const sta::Transition * > ();
        break;

      case symbol_kind::S_FNUMBER: // FNUMBER
      case symbol_kind::S_NUMBER: // NUMBER
        value.template destroy< float > ();
        break;

      case symbol_kind::S_number_opt: // number_opt
        value.template destroy< float* > ();
        break;

      case symbol_kind::S_DNUMBER: // DNUMBER
        value.template destroy< int > ();
        break;

      case symbol_kind::S_port_spec: // port_spec
      case symbol_kind::S_port_tchk: // port_tchk
        value.template destroy< sta::SdfPortSpec * > ();
        break;

      case symbol_kind::S_value: // value
      case symbol_kind::S_triple: // triple
        value.template destroy< sta::SdfTriple * > ();
        break;

      case symbol_kind::S_delval_list: // delval_list
        value.template destroy< sta::SdfTripleSeq * > ();
        break;

      case symbol_kind::S_QSTRING: // QSTRING
      case symbol_kind::S_ID: // ID
      case symbol_kind::S_EXPR_OPEN_IOPATH: // EXPR_OPEN_IOPATH
      case symbol_kind::S_EXPR_OPEN: // EXPR_OPEN
      case symbol_kind::S_EXPR_ID_CLOSE: // EXPR_ID_CLOSE
      case symbol_kind::S_path: // path
      case symbol_kind::S_port: // port
      case symbol_kind::S_port_instance: // port_instance
        value.template destroy< std::string > ();
        break;

      default:
        break;
    }

        Base::clear ();
      }

#if YYDEBUG || 0
      /// The user-facing name of this symbol.
      const char *name () const YY_NOEXCEPT
      {
        return SdfParse::symbol_name (this->kind ());
      }
#endif // #if YYDEBUG || 0


      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// Whether empty.
      bool empty () const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      value_type value;

      /// The location.
      location_type location;

    private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& that);
#endif
    };

    /// Type access provider for token (enum) based symbols.
    struct by_kind
    {
      /// The symbol kind as needed by the constructor.
      typedef token_kind_type kind_type;

      /// Default constructor.
      by_kind () YY_NOEXCEPT;

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_kind (by_kind&& that) YY_NOEXCEPT;
#endif

      /// Copy constructor.
      by_kind (const by_kind& that) YY_NOEXCEPT;

      /// Constructor from (external) token numbers.
      by_kind (kind_type t) YY_NOEXCEPT;



      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_kind& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// The symbol kind.
      /// \a S_YYEMPTY when empty.
      symbol_kind_type kind_;
    };

    /// Backward compatibility for a private implementation detail (Bison 3.6).
    typedef by_kind by_type;

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_kind>
    {
      /// Superclass.
      typedef basic_symbol<by_kind> super_type;

      /// Empty symbol.
      symbol_type () YY_NOEXCEPT {}

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, location_type l)
        : super_type (token_kind_type (tok), std::move (l))
#else
      symbol_type (int tok, const location_type& l)
        : super_type (token_kind_type (tok), l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT (tok == token::YYEOF
                   || (token::YYerror <= tok && tok <= token::YYUNDEF)
                   || (token::DELAYFILE <= tok && tok <= token::CONDELSE)
                   || (40 <= tok && tok <= 41)
                   || tok == 47
                   || tok == 46
                   || tok == 42
                   || tok == 91
                   || tok == 93
                   || tok == 58
                   || tok == 45);
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, float v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const float& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT (tok == token::FNUMBER);
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, int v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const int& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT (tok == token::DNUMBER);
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, std::string v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const std::string& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT ((token::QSTRING <= tok && tok <= token::EXPR_ID_CLOSE));
#endif
      }
    };

    /// Build a parser object.
    SdfParse (SdfScanner *scanner_yyarg, SdfReader *reader_yyarg);
    virtual ~SdfParse ();

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    SdfParse (const SdfParse&) = delete;
    /// Non copyable.
    SdfParse& operator= (const SdfParse&) = delete;
#endif

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

#if YYDEBUG || 0
    /// The user-facing name of the symbol whose (internal) number is
    /// YYSYMBOL.  No bounds checking.
    static const char *symbol_name (symbol_kind_type yysymbol);
#endif // #if YYDEBUG || 0


    // Implementation of make_symbol for each token kind.
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYEOF (location_type l)
      {
        return symbol_type (token::YYEOF, std::move (l));
      }
#else
      static
      symbol_type
      make_YYEOF (const location_type& l)
      {
        return symbol_type (token::YYEOF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYerror (location_type l)
      {
        return symbol_type (token::YYerror, std::move (l));
      }
#else
      static
      symbol_type
      make_YYerror (const location_type& l)
      {
        return symbol_type (token::YYerror, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYUNDEF (location_type l)
      {
        return symbol_type (token::YYUNDEF, std::move (l));
      }
#else
      static
      symbol_type
      make_YYUNDEF (const location_type& l)
      {
        return symbol_type (token::YYUNDEF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_QSTRING (std::string v, location_type l)
      {
        return symbol_type (token::QSTRING, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_QSTRING (const std::string& v, const location_type& l)
      {
        return symbol_type (token::QSTRING, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ID (std::string v, location_type l)
      {
        return symbol_type (token::ID, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_ID (const std::string& v, const location_type& l)
      {
        return symbol_type (token::ID, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EXPR_OPEN_IOPATH (std::string v, location_type l)
      {
        return symbol_type (token::EXPR_OPEN_IOPATH, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_EXPR_OPEN_IOPATH (const std::string& v, const location_type& l)
      {
        return symbol_type (token::EXPR_OPEN_IOPATH, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EXPR_OPEN (std::string v, location_type l)
      {
        return symbol_type (token::EXPR_OPEN, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_EXPR_OPEN (const std::string& v, const location_type& l)
      {
        return symbol_type (token::EXPR_OPEN, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EXPR_ID_CLOSE (std::string v, location_type l)
      {
        return symbol_type (token::EXPR_ID_CLOSE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_EXPR_ID_CLOSE (const std::string& v, const location_type& l)
      {
        return symbol_type (token::EXPR_ID_CLOSE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FNUMBER (float v, location_type l)
      {
        return symbol_type (token::FNUMBER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FNUMBER (const float& v, const location_type& l)
      {
        return symbol_type (token::FNUMBER, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DNUMBER (int v, location_type l)
      {
        return symbol_type (token::DNUMBER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_DNUMBER (const int& v, const location_type& l)
      {
        return symbol_type (token::DNUMBER, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DELAYFILE (location_type l)
      {
        return symbol_type (token::DELAYFILE, std::move (l));
      }
#else
      static
      symbol_type
      make_DELAYFILE (const location_type& l)
      {
        return symbol_type (token::DELAYFILE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SDFVERSION (location_type l)
      {
        return symbol_type (token::SDFVERSION, std::move (l));
      }
#else
      static
      symbol_type
      make_SDFVERSION (const location_type& l)
      {
        return symbol_type (token::SDFVERSION, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DESIGN (location_type l)
      {
        return symbol_type (token::DESIGN, std::move (l));
      }
#else
      static
      symbol_type
      make_DESIGN (const location_type& l)
      {
        return symbol_type (token::DESIGN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DATE (location_type l)
      {
        return symbol_type (token::DATE, std::move (l));
      }
#else
      static
      symbol_type
      make_DATE (const location_type& l)
      {
        return symbol_type (token::DATE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_VENDOR (location_type l)
      {
        return symbol_type (token::VENDOR, std::move (l));
      }
#else
      static
      symbol_type
      make_VENDOR (const location_type& l)
      {
        return symbol_type (token::VENDOR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PROGRAM (location_type l)
      {
        return symbol_type (token::PROGRAM, std::move (l));
      }
#else
      static
      symbol_type
      make_PROGRAM (const location_type& l)
      {
        return symbol_type (token::PROGRAM, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PVERSION (location_type l)
      {
        return symbol_type (token::PVERSION, std::move (l));
      }
#else
      static
      symbol_type
      make_PVERSION (const location_type& l)
      {
        return symbol_type (token::PVERSION, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIVIDER (location_type l)
      {
        return symbol_type (token::DIVIDER, std::move (l));
      }
#else
      static
      symbol_type
      make_DIVIDER (const location_type& l)
      {
        return symbol_type (token::DIVIDER, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_VOLTAGE (location_type l)
      {
        return symbol_type (token::VOLTAGE, std::move (l));
      }
#else
      static
      symbol_type
      make_VOLTAGE (const location_type& l)
      {
        return symbol_type (token::VOLTAGE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PROCESS (location_type l)
      {
        return symbol_type (token::PROCESS, std::move (l));
      }
#else
      static
      symbol_type
      make_PROCESS (const location_type& l)
      {
        return symbol_type (token::PROCESS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TEMPERATURE (location_type l)
      {
        return symbol_type (token::TEMPERATURE, std::move (l));
      }
#else
      static
      symbol_type
      make_TEMPERATURE (const location_type& l)
      {
        return symbol_type (token::TEMPERATURE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TIMESCALE (location_type l)
      {
        return symbol_type (token::TIMESCALE, std::move (l));
      }
#else
      static
      symbol_type
      make_TIMESCALE (const location_type& l)
      {
        return symbol_type (token::TIMESCALE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CELL (location_type l)
      {
        return symbol_type (token::CELL, std::move (l));
      }
#else
      static
      symbol_type
      make_CELL (const location_type& l)
      {
        return symbol_type (token::CELL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CELLTYPE (location_type l)
      {
        return symbol_type (token::CELLTYPE, std::move (l));
      }
#else
      static
      symbol_type
      make_CELLTYPE (const location_type& l)
      {
        return symbol_type (token::CELLTYPE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INSTANCE (location_type l)
      {
        return symbol_type (token::INSTANCE, std::move (l));
      }
#else
      static
      symbol_type
      make_INSTANCE (const location_type& l)
      {
        return symbol_type (token::INSTANCE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DELAY (location_type l)
      {
        return symbol_type (token::DELAY, std::move (l));
      }
#else
      static
      symbol_type
      make_DELAY (const location_type& l)
      {
        return symbol_type (token::DELAY, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ABSOLUTE (location_type l)
      {
        return symbol_type (token::ABSOLUTE, std::move (l));
      }
#else
      static
      symbol_type
      make_ABSOLUTE (const location_type& l)
      {
        return symbol_type (token::ABSOLUTE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INCREMENTAL (location_type l)
      {
        return symbol_type (token::INCREMENTAL, std::move (l));
      }
#else
      static
      symbol_type
      make_INCREMENTAL (const location_type& l)
      {
        return symbol_type (token::INCREMENTAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INTERCONNECT (location_type l)
      {
        return symbol_type (token::INTERCONNECT, std::move (l));
      }
#else
      static
      symbol_type
      make_INTERCONNECT (const location_type& l)
      {
        return symbol_type (token::INTERCONNECT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PORT (location_type l)
      {
        return symbol_type (token::PORT, std::move (l));
      }
#else
      static
      symbol_type
      make_PORT (const location_type& l)
      {
        return symbol_type (token::PORT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DEVICE (location_type l)
      {
        return symbol_type (token::DEVICE, std::move (l));
      }
#else
      static
      symbol_type
      make_DEVICE (const location_type& l)
      {
        return symbol_type (token::DEVICE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RETAIN (location_type l)
      {
        return symbol_type (token::RETAIN, std::move (l));
      }
#else
      static
      symbol_type
      make_RETAIN (const location_type& l)
      {
        return symbol_type (token::RETAIN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IOPATH (location_type l)
      {
        return symbol_type (token::IOPATH, std::move (l));
      }
#else
      static
      symbol_type
      make_IOPATH (const location_type& l)
      {
        return symbol_type (token::IOPATH, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TIMINGCHECK (location_type l)
      {
        return symbol_type (token::TIMINGCHECK, std::move (l));
      }
#else
      static
      symbol_type
      make_TIMINGCHECK (const location_type& l)
      {
        return symbol_type (token::TIMINGCHECK, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SETUP (location_type l)
      {
        return symbol_type (token::SETUP, std::move (l));
      }
#else
      static
      symbol_type
      make_SETUP (const location_type& l)
      {
        return symbol_type (token::SETUP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_HOLD (location_type l)
      {
        return symbol_type (token::HOLD, std::move (l));
      }
#else
      static
      symbol_type
      make_HOLD (const location_type& l)
      {
        return symbol_type (token::HOLD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SETUPHOLD (location_type l)
      {
        return symbol_type (token::SETUPHOLD, std::move (l));
      }
#else
      static
      symbol_type
      make_SETUPHOLD (const location_type& l)
      {
        return symbol_type (token::SETUPHOLD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RECOVERY (location_type l)
      {
        return symbol_type (token::RECOVERY, std::move (l));
      }
#else
      static
      symbol_type
      make_RECOVERY (const location_type& l)
      {
        return symbol_type (token::RECOVERY, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_REMOVAL (location_type l)
      {
        return symbol_type (token::REMOVAL, std::move (l));
      }
#else
      static
      symbol_type
      make_REMOVAL (const location_type& l)
      {
        return symbol_type (token::REMOVAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RECREM (location_type l)
      {
        return symbol_type (token::RECREM, std::move (l));
      }
#else
      static
      symbol_type
      make_RECREM (const location_type& l)
      {
        return symbol_type (token::RECREM, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_WIDTH (location_type l)
      {
        return symbol_type (token::WIDTH, std::move (l));
      }
#else
      static
      symbol_type
      make_WIDTH (const location_type& l)
      {
        return symbol_type (token::WIDTH, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PERIOD (location_type l)
      {
        return symbol_type (token::PERIOD, std::move (l));
      }
#else
      static
      symbol_type
      make_PERIOD (const location_type& l)
      {
        return symbol_type (token::PERIOD, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SKEW (location_type l)
      {
        return symbol_type (token::SKEW, std::move (l));
      }
#else
      static
      symbol_type
      make_SKEW (const location_type& l)
      {
        return symbol_type (token::SKEW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NOCHANGE (location_type l)
      {
        return symbol_type (token::NOCHANGE, std::move (l));
      }
#else
      static
      symbol_type
      make_NOCHANGE (const location_type& l)
      {
        return symbol_type (token::NOCHANGE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_POSEDGE (location_type l)
      {
        return symbol_type (token::POSEDGE, std::move (l));
      }
#else
      static
      symbol_type
      make_POSEDGE (const location_type& l)
      {
        return symbol_type (token::POSEDGE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NEGEDGE (location_type l)
      {
        return symbol_type (token::NEGEDGE, std::move (l));
      }
#else
      static
      symbol_type
      make_NEGEDGE (const location_type& l)
      {
        return symbol_type (token::NEGEDGE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COND (location_type l)
      {
        return symbol_type (token::COND, std::move (l));
      }
#else
      static
      symbol_type
      make_COND (const location_type& l)
      {
        return symbol_type (token::COND, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CONDELSE (location_type l)
      {
        return symbol_type (token::CONDELSE, std::move (l));
      }
#else
      static
      symbol_type
      make_CONDELSE (const location_type& l)
      {
        return symbol_type (token::CONDELSE, l);
      }
#endif


  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    SdfParse (const SdfParse&);
    /// Non copyable.
    SdfParse& operator= (const SdfParse&);
#endif


    /// Stored state numbers (used for stacks).
    typedef unsigned char state_type;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    static state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT;

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT;

    static const short yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token kind \a t to a symbol kind.
    /// In theory \a t should be a token_kind_type, but character literals
    /// are valid, yet not members of the token_kind_type enum.
    static symbol_kind_type yytranslate_ (int t) YY_NOEXCEPT;

#if YYDEBUG || 0
    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#endif // #if YYDEBUG || 0


    // Tables.
    // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
    // STATE-NUM.
    static const short yypact_[];

    // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
    // Performed when YYTABLE does not specify something else to do.  Zero
    // means the default is an error.
    static const signed char yydefact_[];

    // YYPGOTO[NTERM-NUM].
    static const short yypgoto_[];

    // YYDEFGOTO[NTERM-NUM].
    static const unsigned char yydefgoto_[];

    // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
    // positive, shift that token.  If negative, reduce the rule whose
    // number is the opposite.  If YYTABLE_NINF, syntax error.
    static const short yytable_[];

    static const short yycheck_[];

    // YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
    // state STATE-NUM.
    static const signed char yystos_[];

    // YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.
    static const signed char yyr1_[];

    // YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.
    static const signed char yyr2_[];


#if YYDEBUG
    // YYRLINE[YYN] -- Source line where rule number YYN was defined.
    static const short yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r) const;
    /// Print the state stack on the debug stream.
    virtual void yy_stack_print_ () const;

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
    std::ostream* yycdebug_;

    /// \brief Display a symbol kind, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state () YY_NOEXCEPT;

      /// The symbol kind as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_state& that);

      /// The symbol kind (corresponding to \a state).
      /// \a symbol_kind::S_YYEMPTY when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      /// We use the initial state, as it does not have a value.
      enum { empty_state = 0 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Move or copy construction.
      stack_symbol_type (YY_RVREF (stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator= (stack_symbol_type& that);

      /// Assignment, needed by push_back by other implementations.
      /// Needed by some other old implementations.
      stack_symbol_type& operator= (const stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::iterator iterator;
      typedef typename S::const_iterator const_iterator;
      typedef typename S::size_type size_type;
      typedef typename std::ptrdiff_t index_type;

      stack (size_type n = 200) YY_NOEXCEPT
        : seq_ (n)
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Non copyable.
      stack (const stack&) = delete;
      /// Non copyable.
      stack& operator= (const stack&) = delete;
#endif

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (index_type i) const
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (index_type i)
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push (YY_MOVE_REF (T) t)
      {
        seq_.push_back (T ());
        operator[] (0).move (t);
      }

      /// Pop elements from the stack.
      void
      pop (std::ptrdiff_t n = 1) YY_NOEXCEPT
      {
        for (; 0 < n; --n)
          seq_.pop_back ();
      }

      /// Pop all elements from the stack.
      void
      clear () YY_NOEXCEPT
      {
        seq_.clear ();
      }

      /// Number of elements on the stack.
      index_type
      size () const YY_NOEXCEPT
      {
        return index_type (seq_.size ());
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.begin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.end ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, index_type range) YY_NOEXCEPT
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (index_type i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        index_type range_;
      };

    private:
#if YY_CPLUSPLUS < 201103L
      /// Non copyable.
      stack (const stack&);
      /// Non copyable.
      stack& operator= (const stack&);
#endif
      /// The wrapped container.
      S seq_;
    };


    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param sym  the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a sym.value is stolen.
    void yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym);

    /// Pop \a n symbols from the stack.
    void yypop_ (int n = 1) YY_NOEXCEPT;

    /// Constants.
    enum
    {
      yylast_ = 268,     ///< Last index in yytable_.
      yynnts_ = 42,  ///< Number of nonterminal symbols.
      yyfinal_ = 4 ///< Termination state number.
    };


    // User arguments.
    SdfScanner *scanner;
    SdfReader *reader;

  };


#line 66 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
} // sta
#line 2215 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.hh"




#endif // !YY_YY_HOME_EATHESWAR_SILISFOUNDATION_SILIS_SYNTHENGINE_BUILD_OPENSTA_BUILD_SDFPARSE_HH_INCLUDED
