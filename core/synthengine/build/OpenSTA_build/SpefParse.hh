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
 ** \file /home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.hh
 ** Define the sta::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

#ifndef YY_YY_HOME_EATHESWAR_SILISFOUNDATION_SILIS_SYNTHENGINE_BUILD_OPENSTA_BUILD_SPEFPARSE_HH_INCLUDED
# define YY_YY_HOME_EATHESWAR_SILISFOUNDATION_SILIS_SYNTHENGINE_BUILD_OPENSTA_BUILD_SPEFPARSE_HH_INCLUDED
// "%code requires" blocks.
#line 48 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"

#include <string>
#include "StringUtil.hh"

namespace sta {
// Bison's C++ variant skeleton cannot use void as a semantic type.
struct SpefParseVoid {};
class SpefReader;
class SpefScanner;
class Net;
class Pin;
class PortDirection;
class SpefRspfPi;
class SpefTriple;
}

#line 66 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.hh"

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
# include "SpefLocation.hh"
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

#line 68 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
namespace sta {
#line 207 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.hh"




  /// A Bison parser.
  class SpefParse
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
      // prefix_bus_delim
      // suffix_bus_delim
      // hchar
      char dummy1[sizeof (char)];

      // FLOAT
      // threshold
      // poles
      // pole
      // complex_par_value
      // cnumber
      // real_component
      // imaginary_component
      // residues
      // residue
      // number
      // pos_number
      char dummy2[sizeof (float)];

      // INTEGER
      // conf
      // cap_elems
      // cap_elem
      // cap_id
      // res_elems
      // res_elem
      // res_id
      // induc_elems
      // induc_elem
      // induc_id
      // pos_integer
      char dummy3[sizeof (int)];

      // net
      char dummy4[sizeof (sta::Net *)];

      // internal_connection
      // pin_name
      // driver_pair
      char dummy5[sizeof (sta::Pin *)];

      // direction
      char dummy6[sizeof (sta::PortDirection *)];

      // name_map_entries
      // name_map_entry
      // net_names
      // net_name
      // port_entries
      // port_entry
      // pport_entries
      // pport_entry
      // pport_name
      // pexternal_connection
      char dummy7[sizeof (sta::SpefParseVoid)];

      // pi_model
      char dummy8[sizeof (sta::SpefRspfPi *)];

      // par_value
      // total_cap
      char dummy9[sizeof (sta::SpefTriple *)];

      // qstrings
      char dummy10[sizeof (sta::StringSeq *)];

      // QSTRING
      // INDEX
      // IDENT
      // NAME
      // mapped_item
      // port_name
      // inst_name
      // pport
      // physical_inst
      // cell_type
      // entity
      // external_connection
      // internal_parasitic_node
      // parasitic_node
      // driver_cell
      // pnet_ref
      // internal_pdspf_node
      // name_or_index
      char dummy11[sizeof (std::string)];
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
    INTEGER = 258,                 // INTEGER
    FLOAT = 259,                   // FLOAT
    QSTRING = 260,                 // QSTRING
    INDEX = 261,                   // INDEX
    IDENT = 262,                   // IDENT
    NAME = 263,                    // NAME
    SPEF = 264,                    // SPEF
    DESIGN = 265,                  // DESIGN
    DATE = 266,                    // DATE
    VENDOR = 267,                  // VENDOR
    PROGRAM = 268,                 // PROGRAM
    DESIGN_FLOW = 269,             // DESIGN_FLOW
    PVERSION = 270,                // PVERSION
    DIVIDER = 271,                 // DIVIDER
    DELIMITER = 272,               // DELIMITER
    BUS_DELIMITER = 273,           // BUS_DELIMITER
    T_UNIT = 274,                  // T_UNIT
    C_UNIT = 275,                  // C_UNIT
    R_UNIT = 276,                  // R_UNIT
    L_UNIT = 277,                  // L_UNIT
    NAME_MAP = 278,                // NAME_MAP
    POWER_NETS = 279,              // POWER_NETS
    GROUND_NETS = 280,             // GROUND_NETS
    KW_C = 281,                    // KW_C
    KW_L = 282,                    // KW_L
    KW_S = 283,                    // KW_S
    KW_D = 284,                    // KW_D
    KW_V = 285,                    // KW_V
    PORTS = 286,                   // PORTS
    PHYSICAL_PORTS = 287,          // PHYSICAL_PORTS
    DEFINE = 288,                  // DEFINE
    PDEFINE = 289,                 // PDEFINE
    D_NET = 290,                   // D_NET
    D_PNET = 291,                  // D_PNET
    R_NET = 292,                   // R_NET
    R_PNET = 293,                  // R_PNET
    END = 294,                     // END
    CONN = 295,                    // CONN
    CAP = 296,                     // CAP
    RES = 297,                     // RES
    INDUC = 298,                   // INDUC
    KW_P = 299,                    // KW_P
    KW_I = 300,                    // KW_I
    KW_N = 301,                    // KW_N
    DRIVER = 302,                  // DRIVER
    CELL = 303,                    // CELL
    C2_R1_C1 = 304,                // C2_R1_C1
    LOADS = 305,                   // LOADS
    RC = 306,                      // RC
    KW_Q = 307,                    // KW_Q
    KW_K = 308                     // KW_K
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
        YYNTOKENS = 66, ///< Number of tokens.
        S_YYEMPTY = -2,
        S_YYEOF = 0,                             // "end of file"
        S_YYerror = 1,                           // error
        S_YYUNDEF = 2,                           // "invalid token"
        S_INTEGER = 3,                           // INTEGER
        S_FLOAT = 4,                             // FLOAT
        S_QSTRING = 5,                           // QSTRING
        S_INDEX = 6,                             // INDEX
        S_IDENT = 7,                             // IDENT
        S_NAME = 8,                              // NAME
        S_SPEF = 9,                              // SPEF
        S_DESIGN = 10,                           // DESIGN
        S_DATE = 11,                             // DATE
        S_VENDOR = 12,                           // VENDOR
        S_PROGRAM = 13,                          // PROGRAM
        S_DESIGN_FLOW = 14,                      // DESIGN_FLOW
        S_PVERSION = 15,                         // PVERSION
        S_DIVIDER = 16,                          // DIVIDER
        S_DELIMITER = 17,                        // DELIMITER
        S_BUS_DELIMITER = 18,                    // BUS_DELIMITER
        S_T_UNIT = 19,                           // T_UNIT
        S_C_UNIT = 20,                           // C_UNIT
        S_R_UNIT = 21,                           // R_UNIT
        S_L_UNIT = 22,                           // L_UNIT
        S_NAME_MAP = 23,                         // NAME_MAP
        S_POWER_NETS = 24,                       // POWER_NETS
        S_GROUND_NETS = 25,                      // GROUND_NETS
        S_KW_C = 26,                             // KW_C
        S_KW_L = 27,                             // KW_L
        S_KW_S = 28,                             // KW_S
        S_KW_D = 29,                             // KW_D
        S_KW_V = 30,                             // KW_V
        S_PORTS = 31,                            // PORTS
        S_PHYSICAL_PORTS = 32,                   // PHYSICAL_PORTS
        S_DEFINE = 33,                           // DEFINE
        S_PDEFINE = 34,                          // PDEFINE
        S_D_NET = 35,                            // D_NET
        S_D_PNET = 36,                           // D_PNET
        S_R_NET = 37,                            // R_NET
        S_R_PNET = 38,                           // R_PNET
        S_END = 39,                              // END
        S_CONN = 40,                             // CONN
        S_CAP = 41,                              // CAP
        S_RES = 42,                              // RES
        S_INDUC = 43,                            // INDUC
        S_KW_P = 44,                             // KW_P
        S_KW_I = 45,                             // KW_I
        S_KW_N = 46,                             // KW_N
        S_DRIVER = 47,                           // DRIVER
        S_CELL = 48,                             // CELL
        S_C2_R1_C1 = 49,                         // C2_R1_C1
        S_LOADS = 50,                            // LOADS
        S_RC = 51,                               // RC
        S_KW_Q = 52,                             // KW_Q
        S_KW_K = 53,                             // KW_K
        S_54_ = 54,                              // '['
        S_55_ = 55,                              // '{'
        S_56_ = 56,                              // '('
        S_57_ = 57,                              // '<'
        S_58_ = 58,                              // ']'
        S_59_ = 59,                              // '}'
        S_60_ = 60,                              // ')'
        S_61_ = 61,                              // '>'
        S_62_ = 62,                              // '.'
        S_63_ = 63,                              // '/'
        S_64_ = 64,                              // '|'
        S_65_ = 65,                              // ':'
        S_YYACCEPT = 66,                         // $accept
        S_file = 67,                             // file
        S_prefix_bus_delim = 68,                 // prefix_bus_delim
        S_suffix_bus_delim = 69,                 // suffix_bus_delim
        S_hchar = 70,                            // hchar
        S_header_def = 71,                       // header_def
        S_spef_version = 72,                     // spef_version
        S_design_name = 73,                      // design_name
        S_date = 74,                             // date
        S_program_name = 75,                     // program_name
        S_program_version = 76,                  // program_version
        S_vendor = 77,                           // vendor
        S_design_flow = 78,                      // design_flow
        S_qstrings = 79,                         // qstrings
        S_hierarchy_div_def = 80,                // hierarchy_div_def
        S_pin_delim_def = 81,                    // pin_delim_def
        S_bus_delim_def = 82,                    // bus_delim_def
        S_unit_def = 83,                         // unit_def
        S_time_scale = 84,                       // time_scale
        S_cap_scale = 85,                        // cap_scale
        S_res_scale = 86,                        // res_scale
        S_induc_scale = 87,                      // induc_scale
        S_name_map = 88,                         // name_map
        S_name_map_entries = 89,                 // name_map_entries
        S_name_map_entry = 90,                   // name_map_entry
        S_mapped_item = 91,                      // mapped_item
        S_power_def = 92,                        // power_def
        S_power_net_def = 93,                    // power_net_def
        S_ground_net_def = 94,                   // ground_net_def
        S_net_names = 95,                        // net_names
        S_net_name = 96,                         // net_name
        S_external_def = 97,                     // external_def
        S_port_def = 98,                         // port_def
        S_port_entries = 99,                     // port_entries
        S_port_entry = 100,                      // port_entry
        S_direction = 101,                       // direction
        S_port_name = 102,                       // port_name
        S_inst_name = 103,                       // inst_name
        S_physical_port_def = 104,               // physical_port_def
        S_pport_entries = 105,                   // pport_entries
        S_pport_entry = 106,                     // pport_entry
        S_pport_name = 107,                      // pport_name
        S_pport = 108,                           // pport
        S_physical_inst = 109,                   // physical_inst
        S_conn_attrs = 110,                      // conn_attrs
        S_conn_attr = 111,                       // conn_attr
        S_coordinates = 112,                     // coordinates
        S_cap_load = 113,                        // cap_load
        S_par_value = 114,                       // par_value
        S_slews = 115,                           // slews
        S_threshold = 116,                       // threshold
        S_driving_cell = 117,                    // driving_cell
        S_cell_type = 118,                       // cell_type
        S_define_def = 119,                      // define_def
        S_define_entry = 120,                    // define_entry
        S_entity = 121,                          // entity
        S_internal_def = 122,                    // internal_def
        S_nets = 123,                            // nets
        S_d_net = 124,                           // d_net
        S_125_1 = 125,                           // $@1
        S_net = 126,                             // net
        S_total_cap = 127,                       // total_cap
        S_routing_conf = 128,                    // routing_conf
        S_conf = 129,                            // conf
        S_conn_sec = 130,                        // conn_sec
        S_conn_defs = 131,                       // conn_defs
        S_conn_def = 132,                        // conn_def
        S_external_connection = 133,             // external_connection
        S_internal_connection = 134,             // internal_connection
        S_pin_name = 135,                        // pin_name
        S_internal_node_coords = 136,            // internal_node_coords
        S_internal_node_coord = 137,             // internal_node_coord
        S_internal_parasitic_node = 138,         // internal_parasitic_node
        S_cap_sec = 139,                         // cap_sec
        S_cap_elems = 140,                       // cap_elems
        S_cap_elem = 141,                        // cap_elem
        S_cap_id = 142,                          // cap_id
        S_parasitic_node = 143,                  // parasitic_node
        S_res_sec = 144,                         // res_sec
        S_res_elems = 145,                       // res_elems
        S_res_elem = 146,                        // res_elem
        S_res_id = 147,                          // res_id
        S_induc_sec = 148,                       // induc_sec
        S_induc_elems = 149,                     // induc_elems
        S_induc_elem = 150,                      // induc_elem
        S_induc_id = 151,                        // induc_id
        S_r_net = 152,                           // r_net
        S_153_2 = 153,                           // $@2
        S_driver_reducs = 154,                   // driver_reducs
        S_driver_reduc = 155,                    // driver_reduc
        S_156_3 = 156,                           // $@3
        S_driver_pair = 157,                     // driver_pair
        S_driver_cell = 158,                     // driver_cell
        S_pi_model = 159,                        // pi_model
        S_load_desc = 160,                       // load_desc
        S_rc_descs = 161,                        // rc_descs
        S_rc_desc = 162,                         // rc_desc
        S_pole_residue_desc = 163,               // pole_residue_desc
        S_pole_desc = 164,                       // pole_desc
        S_poles = 165,                           // poles
        S_pole = 166,                            // pole
        S_complex_par_value = 167,               // complex_par_value
        S_cnumber = 168,                         // cnumber
        S_real_component = 169,                  // real_component
        S_imaginary_component = 170,             // imaginary_component
        S_residue_desc = 171,                    // residue_desc
        S_residues = 172,                        // residues
        S_residue = 173,                         // residue
        S_d_pnet = 174,                          // d_pnet
        S_pnet_ref = 175,                        // pnet_ref
        S_pconn_sec = 176,                       // pconn_sec
        S_pconn_defs = 177,                      // pconn_defs
        S_pconn_def = 178,                       // pconn_def
        S_pexternal_connection = 179,            // pexternal_connection
        S_internal_pnode_coords = 180,           // internal_pnode_coords
        S_internal_pnode_coord = 181,            // internal_pnode_coord
        S_internal_pdspf_node = 182,             // internal_pdspf_node
        S_name_or_index = 183,                   // name_or_index
        S_r_pnet = 184,                          // r_pnet
        S_pdriver_reduc = 185,                   // pdriver_reduc
        S_pdriver_pair = 186,                    // pdriver_pair
        S_number = 187,                          // number
        S_pos_integer = 188,                     // pos_integer
        S_pos_number = 189                       // pos_number
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
      case symbol_kind::S_prefix_bus_delim: // prefix_bus_delim
      case symbol_kind::S_suffix_bus_delim: // suffix_bus_delim
      case symbol_kind::S_hchar: // hchar
        value.move< char > (std::move (that.value));
        break;

      case symbol_kind::S_FLOAT: // FLOAT
      case symbol_kind::S_threshold: // threshold
      case symbol_kind::S_poles: // poles
      case symbol_kind::S_pole: // pole
      case symbol_kind::S_complex_par_value: // complex_par_value
      case symbol_kind::S_cnumber: // cnumber
      case symbol_kind::S_real_component: // real_component
      case symbol_kind::S_imaginary_component: // imaginary_component
      case symbol_kind::S_residues: // residues
      case symbol_kind::S_residue: // residue
      case symbol_kind::S_number: // number
      case symbol_kind::S_pos_number: // pos_number
        value.move< float > (std::move (that.value));
        break;

      case symbol_kind::S_INTEGER: // INTEGER
      case symbol_kind::S_conf: // conf
      case symbol_kind::S_cap_elems: // cap_elems
      case symbol_kind::S_cap_elem: // cap_elem
      case symbol_kind::S_cap_id: // cap_id
      case symbol_kind::S_res_elems: // res_elems
      case symbol_kind::S_res_elem: // res_elem
      case symbol_kind::S_res_id: // res_id
      case symbol_kind::S_induc_elems: // induc_elems
      case symbol_kind::S_induc_elem: // induc_elem
      case symbol_kind::S_induc_id: // induc_id
      case symbol_kind::S_pos_integer: // pos_integer
        value.move< int > (std::move (that.value));
        break;

      case symbol_kind::S_net: // net
        value.move< sta::Net * > (std::move (that.value));
        break;

      case symbol_kind::S_internal_connection: // internal_connection
      case symbol_kind::S_pin_name: // pin_name
      case symbol_kind::S_driver_pair: // driver_pair
        value.move< sta::Pin * > (std::move (that.value));
        break;

      case symbol_kind::S_direction: // direction
        value.move< sta::PortDirection * > (std::move (that.value));
        break;

      case symbol_kind::S_name_map_entries: // name_map_entries
      case symbol_kind::S_name_map_entry: // name_map_entry
      case symbol_kind::S_net_names: // net_names
      case symbol_kind::S_net_name: // net_name
      case symbol_kind::S_port_entries: // port_entries
      case symbol_kind::S_port_entry: // port_entry
      case symbol_kind::S_pport_entries: // pport_entries
      case symbol_kind::S_pport_entry: // pport_entry
      case symbol_kind::S_pport_name: // pport_name
      case symbol_kind::S_pexternal_connection: // pexternal_connection
        value.move< sta::SpefParseVoid > (std::move (that.value));
        break;

      case symbol_kind::S_pi_model: // pi_model
        value.move< sta::SpefRspfPi * > (std::move (that.value));
        break;

      case symbol_kind::S_par_value: // par_value
      case symbol_kind::S_total_cap: // total_cap
        value.move< sta::SpefTriple * > (std::move (that.value));
        break;

      case symbol_kind::S_qstrings: // qstrings
        value.move< sta::StringSeq * > (std::move (that.value));
        break;

      case symbol_kind::S_QSTRING: // QSTRING
      case symbol_kind::S_INDEX: // INDEX
      case symbol_kind::S_IDENT: // IDENT
      case symbol_kind::S_NAME: // NAME
      case symbol_kind::S_mapped_item: // mapped_item
      case symbol_kind::S_port_name: // port_name
      case symbol_kind::S_inst_name: // inst_name
      case symbol_kind::S_pport: // pport
      case symbol_kind::S_physical_inst: // physical_inst
      case symbol_kind::S_cell_type: // cell_type
      case symbol_kind::S_entity: // entity
      case symbol_kind::S_external_connection: // external_connection
      case symbol_kind::S_internal_parasitic_node: // internal_parasitic_node
      case symbol_kind::S_parasitic_node: // parasitic_node
      case symbol_kind::S_driver_cell: // driver_cell
      case symbol_kind::S_pnet_ref: // pnet_ref
      case symbol_kind::S_internal_pdspf_node: // internal_pdspf_node
      case symbol_kind::S_name_or_index: // name_or_index
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
      basic_symbol (typename Base::kind_type t, sta::Net *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sta::Net *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sta::Pin *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sta::Pin *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sta::PortDirection *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sta::PortDirection *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sta::SpefParseVoid&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sta::SpefParseVoid& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sta::SpefRspfPi *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sta::SpefRspfPi *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sta::SpefTriple *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sta::SpefTriple *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sta::StringSeq *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sta::StringSeq *& v, const location_type& l)
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
      case symbol_kind::S_prefix_bus_delim: // prefix_bus_delim
      case symbol_kind::S_suffix_bus_delim: // suffix_bus_delim
      case symbol_kind::S_hchar: // hchar
        value.template destroy< char > ();
        break;

      case symbol_kind::S_FLOAT: // FLOAT
      case symbol_kind::S_threshold: // threshold
      case symbol_kind::S_poles: // poles
      case symbol_kind::S_pole: // pole
      case symbol_kind::S_complex_par_value: // complex_par_value
      case symbol_kind::S_cnumber: // cnumber
      case symbol_kind::S_real_component: // real_component
      case symbol_kind::S_imaginary_component: // imaginary_component
      case symbol_kind::S_residues: // residues
      case symbol_kind::S_residue: // residue
      case symbol_kind::S_number: // number
      case symbol_kind::S_pos_number: // pos_number
        value.template destroy< float > ();
        break;

      case symbol_kind::S_INTEGER: // INTEGER
      case symbol_kind::S_conf: // conf
      case symbol_kind::S_cap_elems: // cap_elems
      case symbol_kind::S_cap_elem: // cap_elem
      case symbol_kind::S_cap_id: // cap_id
      case symbol_kind::S_res_elems: // res_elems
      case symbol_kind::S_res_elem: // res_elem
      case symbol_kind::S_res_id: // res_id
      case symbol_kind::S_induc_elems: // induc_elems
      case symbol_kind::S_induc_elem: // induc_elem
      case symbol_kind::S_induc_id: // induc_id
      case symbol_kind::S_pos_integer: // pos_integer
        value.template destroy< int > ();
        break;

      case symbol_kind::S_net: // net
        value.template destroy< sta::Net * > ();
        break;

      case symbol_kind::S_internal_connection: // internal_connection
      case symbol_kind::S_pin_name: // pin_name
      case symbol_kind::S_driver_pair: // driver_pair
        value.template destroy< sta::Pin * > ();
        break;

      case symbol_kind::S_direction: // direction
        value.template destroy< sta::PortDirection * > ();
        break;

      case symbol_kind::S_name_map_entries: // name_map_entries
      case symbol_kind::S_name_map_entry: // name_map_entry
      case symbol_kind::S_net_names: // net_names
      case symbol_kind::S_net_name: // net_name
      case symbol_kind::S_port_entries: // port_entries
      case symbol_kind::S_port_entry: // port_entry
      case symbol_kind::S_pport_entries: // pport_entries
      case symbol_kind::S_pport_entry: // pport_entry
      case symbol_kind::S_pport_name: // pport_name
      case symbol_kind::S_pexternal_connection: // pexternal_connection
        value.template destroy< sta::SpefParseVoid > ();
        break;

      case symbol_kind::S_pi_model: // pi_model
        value.template destroy< sta::SpefRspfPi * > ();
        break;

      case symbol_kind::S_par_value: // par_value
      case symbol_kind::S_total_cap: // total_cap
        value.template destroy< sta::SpefTriple * > ();
        break;

      case symbol_kind::S_qstrings: // qstrings
        value.template destroy< sta::StringSeq * > ();
        break;

      case symbol_kind::S_QSTRING: // QSTRING
      case symbol_kind::S_INDEX: // INDEX
      case symbol_kind::S_IDENT: // IDENT
      case symbol_kind::S_NAME: // NAME
      case symbol_kind::S_mapped_item: // mapped_item
      case symbol_kind::S_port_name: // port_name
      case symbol_kind::S_inst_name: // inst_name
      case symbol_kind::S_pport: // pport
      case symbol_kind::S_physical_inst: // physical_inst
      case symbol_kind::S_cell_type: // cell_type
      case symbol_kind::S_entity: // entity
      case symbol_kind::S_external_connection: // external_connection
      case symbol_kind::S_internal_parasitic_node: // internal_parasitic_node
      case symbol_kind::S_parasitic_node: // parasitic_node
      case symbol_kind::S_driver_cell: // driver_cell
      case symbol_kind::S_pnet_ref: // pnet_ref
      case symbol_kind::S_internal_pdspf_node: // internal_pdspf_node
      case symbol_kind::S_name_or_index: // name_or_index
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
        return SpefParse::symbol_name (this->kind ());
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
                   || (token::SPEF <= tok && tok <= token::KW_K)
                   || tok == 91
                   || tok == 123
                   || tok == 40
                   || tok == 60
                   || tok == 93
                   || tok == 125
                   || tok == 41
                   || tok == 62
                   || (46 <= tok && tok <= 47)
                   || tok == 124
                   || tok == 58);
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
        YY_ASSERT (tok == token::FLOAT);
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
        YY_ASSERT (tok == token::INTEGER);
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
        YY_ASSERT ((token::QSTRING <= tok && tok <= token::NAME));
#endif
      }
    };

    /// Build a parser object.
    SpefParse (SpefScanner *scanner_yyarg, SpefReader *reader_yyarg);
    virtual ~SpefParse ();

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    SpefParse (const SpefParse&) = delete;
    /// Non copyable.
    SpefParse& operator= (const SpefParse&) = delete;
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
      make_INTEGER (int v, location_type l)
      {
        return symbol_type (token::INTEGER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_INTEGER (const int& v, const location_type& l)
      {
        return symbol_type (token::INTEGER, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FLOAT (float v, location_type l)
      {
        return symbol_type (token::FLOAT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FLOAT (const float& v, const location_type& l)
      {
        return symbol_type (token::FLOAT, v, l);
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
      make_INDEX (std::string v, location_type l)
      {
        return symbol_type (token::INDEX, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_INDEX (const std::string& v, const location_type& l)
      {
        return symbol_type (token::INDEX, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IDENT (std::string v, location_type l)
      {
        return symbol_type (token::IDENT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_IDENT (const std::string& v, const location_type& l)
      {
        return symbol_type (token::IDENT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NAME (std::string v, location_type l)
      {
        return symbol_type (token::NAME, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_NAME (const std::string& v, const location_type& l)
      {
        return symbol_type (token::NAME, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SPEF (location_type l)
      {
        return symbol_type (token::SPEF, std::move (l));
      }
#else
      static
      symbol_type
      make_SPEF (const location_type& l)
      {
        return symbol_type (token::SPEF, l);
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
      make_DESIGN_FLOW (location_type l)
      {
        return symbol_type (token::DESIGN_FLOW, std::move (l));
      }
#else
      static
      symbol_type
      make_DESIGN_FLOW (const location_type& l)
      {
        return symbol_type (token::DESIGN_FLOW, l);
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
      make_DELIMITER (location_type l)
      {
        return symbol_type (token::DELIMITER, std::move (l));
      }
#else
      static
      symbol_type
      make_DELIMITER (const location_type& l)
      {
        return symbol_type (token::DELIMITER, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BUS_DELIMITER (location_type l)
      {
        return symbol_type (token::BUS_DELIMITER, std::move (l));
      }
#else
      static
      symbol_type
      make_BUS_DELIMITER (const location_type& l)
      {
        return symbol_type (token::BUS_DELIMITER, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_T_UNIT (location_type l)
      {
        return symbol_type (token::T_UNIT, std::move (l));
      }
#else
      static
      symbol_type
      make_T_UNIT (const location_type& l)
      {
        return symbol_type (token::T_UNIT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_C_UNIT (location_type l)
      {
        return symbol_type (token::C_UNIT, std::move (l));
      }
#else
      static
      symbol_type
      make_C_UNIT (const location_type& l)
      {
        return symbol_type (token::C_UNIT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_R_UNIT (location_type l)
      {
        return symbol_type (token::R_UNIT, std::move (l));
      }
#else
      static
      symbol_type
      make_R_UNIT (const location_type& l)
      {
        return symbol_type (token::R_UNIT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_L_UNIT (location_type l)
      {
        return symbol_type (token::L_UNIT, std::move (l));
      }
#else
      static
      symbol_type
      make_L_UNIT (const location_type& l)
      {
        return symbol_type (token::L_UNIT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NAME_MAP (location_type l)
      {
        return symbol_type (token::NAME_MAP, std::move (l));
      }
#else
      static
      symbol_type
      make_NAME_MAP (const location_type& l)
      {
        return symbol_type (token::NAME_MAP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_POWER_NETS (location_type l)
      {
        return symbol_type (token::POWER_NETS, std::move (l));
      }
#else
      static
      symbol_type
      make_POWER_NETS (const location_type& l)
      {
        return symbol_type (token::POWER_NETS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_GROUND_NETS (location_type l)
      {
        return symbol_type (token::GROUND_NETS, std::move (l));
      }
#else
      static
      symbol_type
      make_GROUND_NETS (const location_type& l)
      {
        return symbol_type (token::GROUND_NETS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_C (location_type l)
      {
        return symbol_type (token::KW_C, std::move (l));
      }
#else
      static
      symbol_type
      make_KW_C (const location_type& l)
      {
        return symbol_type (token::KW_C, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_L (location_type l)
      {
        return symbol_type (token::KW_L, std::move (l));
      }
#else
      static
      symbol_type
      make_KW_L (const location_type& l)
      {
        return symbol_type (token::KW_L, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_S (location_type l)
      {
        return symbol_type (token::KW_S, std::move (l));
      }
#else
      static
      symbol_type
      make_KW_S (const location_type& l)
      {
        return symbol_type (token::KW_S, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_D (location_type l)
      {
        return symbol_type (token::KW_D, std::move (l));
      }
#else
      static
      symbol_type
      make_KW_D (const location_type& l)
      {
        return symbol_type (token::KW_D, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_V (location_type l)
      {
        return symbol_type (token::KW_V, std::move (l));
      }
#else
      static
      symbol_type
      make_KW_V (const location_type& l)
      {
        return symbol_type (token::KW_V, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PORTS (location_type l)
      {
        return symbol_type (token::PORTS, std::move (l));
      }
#else
      static
      symbol_type
      make_PORTS (const location_type& l)
      {
        return symbol_type (token::PORTS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PHYSICAL_PORTS (location_type l)
      {
        return symbol_type (token::PHYSICAL_PORTS, std::move (l));
      }
#else
      static
      symbol_type
      make_PHYSICAL_PORTS (const location_type& l)
      {
        return symbol_type (token::PHYSICAL_PORTS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DEFINE (location_type l)
      {
        return symbol_type (token::DEFINE, std::move (l));
      }
#else
      static
      symbol_type
      make_DEFINE (const location_type& l)
      {
        return symbol_type (token::DEFINE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PDEFINE (location_type l)
      {
        return symbol_type (token::PDEFINE, std::move (l));
      }
#else
      static
      symbol_type
      make_PDEFINE (const location_type& l)
      {
        return symbol_type (token::PDEFINE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_D_NET (location_type l)
      {
        return symbol_type (token::D_NET, std::move (l));
      }
#else
      static
      symbol_type
      make_D_NET (const location_type& l)
      {
        return symbol_type (token::D_NET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_D_PNET (location_type l)
      {
        return symbol_type (token::D_PNET, std::move (l));
      }
#else
      static
      symbol_type
      make_D_PNET (const location_type& l)
      {
        return symbol_type (token::D_PNET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_R_NET (location_type l)
      {
        return symbol_type (token::R_NET, std::move (l));
      }
#else
      static
      symbol_type
      make_R_NET (const location_type& l)
      {
        return symbol_type (token::R_NET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_R_PNET (location_type l)
      {
        return symbol_type (token::R_PNET, std::move (l));
      }
#else
      static
      symbol_type
      make_R_PNET (const location_type& l)
      {
        return symbol_type (token::R_PNET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_END (location_type l)
      {
        return symbol_type (token::END, std::move (l));
      }
#else
      static
      symbol_type
      make_END (const location_type& l)
      {
        return symbol_type (token::END, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CONN (location_type l)
      {
        return symbol_type (token::CONN, std::move (l));
      }
#else
      static
      symbol_type
      make_CONN (const location_type& l)
      {
        return symbol_type (token::CONN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CAP (location_type l)
      {
        return symbol_type (token::CAP, std::move (l));
      }
#else
      static
      symbol_type
      make_CAP (const location_type& l)
      {
        return symbol_type (token::CAP, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RES (location_type l)
      {
        return symbol_type (token::RES, std::move (l));
      }
#else
      static
      symbol_type
      make_RES (const location_type& l)
      {
        return symbol_type (token::RES, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INDUC (location_type l)
      {
        return symbol_type (token::INDUC, std::move (l));
      }
#else
      static
      symbol_type
      make_INDUC (const location_type& l)
      {
        return symbol_type (token::INDUC, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_P (location_type l)
      {
        return symbol_type (token::KW_P, std::move (l));
      }
#else
      static
      symbol_type
      make_KW_P (const location_type& l)
      {
        return symbol_type (token::KW_P, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_I (location_type l)
      {
        return symbol_type (token::KW_I, std::move (l));
      }
#else
      static
      symbol_type
      make_KW_I (const location_type& l)
      {
        return symbol_type (token::KW_I, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_N (location_type l)
      {
        return symbol_type (token::KW_N, std::move (l));
      }
#else
      static
      symbol_type
      make_KW_N (const location_type& l)
      {
        return symbol_type (token::KW_N, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DRIVER (location_type l)
      {
        return symbol_type (token::DRIVER, std::move (l));
      }
#else
      static
      symbol_type
      make_DRIVER (const location_type& l)
      {
        return symbol_type (token::DRIVER, l);
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
      make_C2_R1_C1 (location_type l)
      {
        return symbol_type (token::C2_R1_C1, std::move (l));
      }
#else
      static
      symbol_type
      make_C2_R1_C1 (const location_type& l)
      {
        return symbol_type (token::C2_R1_C1, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LOADS (location_type l)
      {
        return symbol_type (token::LOADS, std::move (l));
      }
#else
      static
      symbol_type
      make_LOADS (const location_type& l)
      {
        return symbol_type (token::LOADS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RC (location_type l)
      {
        return symbol_type (token::RC, std::move (l));
      }
#else
      static
      symbol_type
      make_RC (const location_type& l)
      {
        return symbol_type (token::RC, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_Q (location_type l)
      {
        return symbol_type (token::KW_Q, std::move (l));
      }
#else
      static
      symbol_type
      make_KW_Q (const location_type& l)
      {
        return symbol_type (token::KW_Q, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_K (location_type l)
      {
        return symbol_type (token::KW_K, std::move (l));
      }
#else
      static
      symbol_type
      make_KW_K (const location_type& l)
      {
        return symbol_type (token::KW_K, l);
      }
#endif


  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    SpefParse (const SpefParse&);
    /// Non copyable.
    SpefParse& operator= (const SpefParse&);
#endif


    /// Stored state numbers (used for stacks).
    typedef short state_type;

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
    static const unsigned char yydefact_[];

    // YYPGOTO[NTERM-NUM].
    static const short yypgoto_[];

    // YYDEFGOTO[NTERM-NUM].
    static const short yydefgoto_[];

    // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
    // positive, shift that token.  If negative, reduce the rule whose
    // number is the opposite.  If YYTABLE_NINF, syntax error.
    static const short yytable_[];

    static const short yycheck_[];

    // YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
    // state STATE-NUM.
    static const unsigned char yystos_[];

    // YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.
    static const unsigned char yyr1_[];

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
      yylast_ = 253,     ///< Last index in yytable_.
      yynnts_ = 124,  ///< Number of nonterminal symbols.
      yyfinal_ = 6 ///< Termination state number.
    };


    // User arguments.
    SpefScanner *scanner;
    SpefReader *reader;

  };


#line 68 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
} // sta
#line 2585 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.hh"




#endif // !YY_YY_HOME_EATHESWAR_SILISFOUNDATION_SILIS_SYNTHENGINE_BUILD_OPENSTA_BUILD_SPEFPARSE_HH_INCLUDED
