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
 ** \file /home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.hh
 ** Define the sta::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

#ifndef YY_YY_HOME_EATHESWAR_SILISFOUNDATION_SILIS_SYNTHENGINE_BUILD_OPENSTA_BUILD_VERILOGPARSE_HH_INCLUDED
# define YY_YY_HOME_EATHESWAR_SILISFOUNDATION_SILIS_SYNTHENGINE_BUILD_OPENSTA_BUILD_VERILOGPARSE_HH_INCLUDED
// "%code requires" blocks.
#line 52 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"

#include <string>
#include <vector>

namespace sta {
class PortDirection;
class VerilogAssign;
class VerilogAttr;
class VerilogAttrEntry;
class VerilogAttrStmt;
class VerilogDclArg;
class VerilogNet;
class VerilogReader;
class VerilogScanner;
class VerilogStmt;

using VerilogAttrEntrySeq = std::vector<VerilogAttrEntry*>;
using VerilogAttrSeq = std::vector<VerilogAttr*>;
using VerilogAttrStmtSeq = std::vector<VerilogAttrStmt*>;
using VerilogDclArgSeq = std::vector<VerilogDclArg*>;
using VerilogNetSeq = std::vector<VerilogNet*>;
using VerilogStmtSeq = std::vector<VerilogStmt*>;
}

#line 74 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.hh"

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
# include "VerilogLocation.hh"
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

#line 80 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
namespace sta {
#line 215 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.hh"




  /// A Bison parser.
  class VerilogParse
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
      // INT
      // parameter_expr
      // parameter_exprs
      char dummy1[sizeof (int)];

      // port_dcl_type
      // dcl_type
      char dummy2[sizeof (sta::PortDirection *)];

      // net_assignment
      char dummy3[sizeof (sta::VerilogAssign *)];

      // attr_spec
      char dummy4[sizeof (sta::VerilogAttrEntry *)];

      // attr_specs
      char dummy5[sizeof (sta::VerilogAttrEntrySeq *)];

      // attr_instance
      char dummy6[sizeof (sta::VerilogAttrStmt *)];

      // attr_instance_seq
      char dummy7[sizeof (sta::VerilogAttrStmtSeq *)];

      // dcl_arg
      char dummy8[sizeof (sta::VerilogDclArg *)];

      // dcl_args
      char dummy9[sizeof (sta::VerilogDclArgSeq *)];

      // port
      // port_expr
      // port_ref
      // net_assign_lhs
      // inst_named_pin
      // named_pin_net_expr
      // net_named
      // net_scalar
      // net_bit_select
      // net_part_select
      // net_constant
      // net_expr_concat
      // net_expr
      char dummy10[sizeof (sta::VerilogNet *)];

      // port_list
      // port_refs
      // inst_pins
      // inst_ordered_pins
      // inst_named_pins
      // net_exprs
      char dummy11[sizeof (sta::VerilogNetSeq *)];

      // port_dcl
      // stmt
      // specify_block
      // parameter
      // parameter_dcls
      // parameter_dcl
      // defparam
      // param_values
      // param_value
      // declaration
      // instance
      char dummy12[sizeof (sta::VerilogStmt *)];

      // port_dcls
      // stmts
      // stmt_seq
      // specify_stmts
      // continuous_assign
      // net_assignments
      char dummy13[sizeof (sta::VerilogStmtSeq *)];

      // ID
      // STRING
      // CONSTANT
      // attr_spec_value
      char dummy14[sizeof (std::string)];
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
    INT = 258,                     // INT
    ID = 259,                      // ID
    STRING = 260,                  // STRING
    CONSTANT = 261,                // CONSTANT
    MODULE = 262,                  // MODULE
    ENDMODULE = 263,               // ENDMODULE
    ASSIGN = 264,                  // ASSIGN
    PARAMETER = 265,               // PARAMETER
    DEFPARAM = 266,                // DEFPARAM
    SPECIFY = 267,                 // SPECIFY
    ENDSPECIFY = 268,              // ENDSPECIFY
    SPECPARAM = 269,               // SPECPARAM
    WIRE = 270,                    // WIRE
    WAND = 271,                    // WAND
    WOR = 272,                     // WOR
    TRI = 273,                     // TRI
    INPUT = 274,                   // INPUT
    OUTPUT = 275,                  // OUTPUT
    INOUT = 276,                   // INOUT
    SUPPLY1 = 277,                 // SUPPLY1
    SUPPLY0 = 278,                 // SUPPLY0
    REG = 279,                     // REG
    ATTR_OPEN = 280,               // ATTR_OPEN
    ATTR_CLOSED = 281,             // ATTR_CLOSED
    NEG = 282                      // NEG
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
        YYNTOKENS = 45, ///< Number of tokens.
        S_YYEMPTY = -2,
        S_YYEOF = 0,                             // "end of file"
        S_YYerror = 1,                           // error
        S_YYUNDEF = 2,                           // "invalid token"
        S_INT = 3,                               // INT
        S_ID = 4,                                // ID
        S_STRING = 5,                            // STRING
        S_CONSTANT = 6,                          // CONSTANT
        S_MODULE = 7,                            // MODULE
        S_ENDMODULE = 8,                         // ENDMODULE
        S_ASSIGN = 9,                            // ASSIGN
        S_PARAMETER = 10,                        // PARAMETER
        S_DEFPARAM = 11,                         // DEFPARAM
        S_SPECIFY = 12,                          // SPECIFY
        S_ENDSPECIFY = 13,                       // ENDSPECIFY
        S_SPECPARAM = 14,                        // SPECPARAM
        S_WIRE = 15,                             // WIRE
        S_WAND = 16,                             // WAND
        S_WOR = 17,                              // WOR
        S_TRI = 18,                              // TRI
        S_INPUT = 19,                            // INPUT
        S_OUTPUT = 20,                           // OUTPUT
        S_INOUT = 21,                            // INOUT
        S_SUPPLY1 = 22,                          // SUPPLY1
        S_SUPPLY0 = 23,                          // SUPPLY0
        S_REG = 24,                              // REG
        S_ATTR_OPEN = 25,                        // ATTR_OPEN
        S_ATTR_CLOSED = 26,                      // ATTR_CLOSED
        S_27_ = 27,                              // '-'
        S_28_ = 28,                              // '+'
        S_29_ = 29,                              // '*'
        S_30_ = 30,                              // '/'
        S_NEG = 31,                              // NEG
        S_32_ = 32,                              // ';'
        S_33_ = 33,                              // '('
        S_34_ = 34,                              // ')'
        S_35_ = 35,                              // ','
        S_36_ = 36,                              // '.'
        S_37_ = 37,                              // '{'
        S_38_ = 38,                              // '}'
        S_39_ = 39,                              // '['
        S_40_ = 40,                              // ':'
        S_41_ = 41,                              // ']'
        S_42_ = 42,                              // '='
        S_43_ = 43,                              // '`'
        S_44_ = 44,                              // '#'
        S_YYACCEPT = 45,                         // $accept
        S_file = 46,                             // file
        S_modules = 47,                          // modules
        S_module = 48,                           // module
        S_port_list = 49,                        // port_list
        S_port = 50,                             // port
        S_port_expr = 51,                        // port_expr
        S_port_refs = 52,                        // port_refs
        S_port_ref = 53,                         // port_ref
        S_port_dcls = 54,                        // port_dcls
        S_port_dcl = 55,                         // port_dcl
        S_port_dcl_type = 56,                    // port_dcl_type
        S_stmts = 57,                            // stmts
        S_stmt = 58,                             // stmt
        S_stmt_seq = 59,                         // stmt_seq
        S_specify_block = 60,                    // specify_block
        S_specify_stmts = 61,                    // specify_stmts
        S_parameter = 62,                        // parameter
        S_parameter_dcls = 63,                   // parameter_dcls
        S_parameter_dcl = 64,                    // parameter_dcl
        S_parameter_expr = 65,                   // parameter_expr
        S_defparam = 66,                         // defparam
        S_param_values = 67,                     // param_values
        S_param_value = 68,                      // param_value
        S_declaration = 69,                      // declaration
        S_dcl_type = 70,                         // dcl_type
        S_dcl_args = 71,                         // dcl_args
        S_dcl_arg = 72,                          // dcl_arg
        S_continuous_assign = 73,                // continuous_assign
        S_net_assignments = 74,                  // net_assignments
        S_net_assignment = 75,                   // net_assignment
        S_net_assign_lhs = 76,                   // net_assign_lhs
        S_instance = 77,                         // instance
        S_parameter_values = 78,                 // parameter_values
        S_parameter_exprs = 79,                  // parameter_exprs
        S_inst_pins = 80,                        // inst_pins
        S_inst_ordered_pins = 81,                // inst_ordered_pins
        S_inst_named_pins = 82,                  // inst_named_pins
        S_inst_named_pin = 83,                   // inst_named_pin
        S_named_pin_net_expr = 84,               // named_pin_net_expr
        S_net_named = 85,                        // net_named
        S_net_scalar = 86,                       // net_scalar
        S_net_bit_select = 87,                   // net_bit_select
        S_net_part_select = 88,                  // net_part_select
        S_net_constant = 89,                     // net_constant
        S_net_expr_concat = 90,                  // net_expr_concat
        S_net_exprs = 91,                        // net_exprs
        S_net_expr = 92,                         // net_expr
        S_attr_instance_seq = 93,                // attr_instance_seq
        S_attr_instance = 94,                    // attr_instance
        S_attr_specs = 95,                       // attr_specs
        S_attr_spec = 96,                        // attr_spec
        S_attr_spec_value = 97                   // attr_spec_value
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
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_parameter_expr: // parameter_expr
      case symbol_kind::S_parameter_exprs: // parameter_exprs
        value.move< int > (std::move (that.value));
        break;

      case symbol_kind::S_port_dcl_type: // port_dcl_type
      case symbol_kind::S_dcl_type: // dcl_type
        value.move< sta::PortDirection * > (std::move (that.value));
        break;

      case symbol_kind::S_net_assignment: // net_assignment
        value.move< sta::VerilogAssign * > (std::move (that.value));
        break;

      case symbol_kind::S_attr_spec: // attr_spec
        value.move< sta::VerilogAttrEntry * > (std::move (that.value));
        break;

      case symbol_kind::S_attr_specs: // attr_specs
        value.move< sta::VerilogAttrEntrySeq * > (std::move (that.value));
        break;

      case symbol_kind::S_attr_instance: // attr_instance
        value.move< sta::VerilogAttrStmt * > (std::move (that.value));
        break;

      case symbol_kind::S_attr_instance_seq: // attr_instance_seq
        value.move< sta::VerilogAttrStmtSeq * > (std::move (that.value));
        break;

      case symbol_kind::S_dcl_arg: // dcl_arg
        value.move< sta::VerilogDclArg * > (std::move (that.value));
        break;

      case symbol_kind::S_dcl_args: // dcl_args
        value.move< sta::VerilogDclArgSeq * > (std::move (that.value));
        break;

      case symbol_kind::S_port: // port
      case symbol_kind::S_port_expr: // port_expr
      case symbol_kind::S_port_ref: // port_ref
      case symbol_kind::S_net_assign_lhs: // net_assign_lhs
      case symbol_kind::S_inst_named_pin: // inst_named_pin
      case symbol_kind::S_named_pin_net_expr: // named_pin_net_expr
      case symbol_kind::S_net_named: // net_named
      case symbol_kind::S_net_scalar: // net_scalar
      case symbol_kind::S_net_bit_select: // net_bit_select
      case symbol_kind::S_net_part_select: // net_part_select
      case symbol_kind::S_net_constant: // net_constant
      case symbol_kind::S_net_expr_concat: // net_expr_concat
      case symbol_kind::S_net_expr: // net_expr
        value.move< sta::VerilogNet * > (std::move (that.value));
        break;

      case symbol_kind::S_port_list: // port_list
      case symbol_kind::S_port_refs: // port_refs
      case symbol_kind::S_inst_pins: // inst_pins
      case symbol_kind::S_inst_ordered_pins: // inst_ordered_pins
      case symbol_kind::S_inst_named_pins: // inst_named_pins
      case symbol_kind::S_net_exprs: // net_exprs
        value.move< sta::VerilogNetSeq * > (std::move (that.value));
        break;

      case symbol_kind::S_port_dcl: // port_dcl
      case symbol_kind::S_stmt: // stmt
      case symbol_kind::S_specify_block: // specify_block
      case symbol_kind::S_parameter: // parameter
      case symbol_kind::S_parameter_dcls: // parameter_dcls
      case symbol_kind::S_parameter_dcl: // parameter_dcl
      case symbol_kind::S_defparam: // defparam
      case symbol_kind::S_param_values: // param_values
      case symbol_kind::S_param_value: // param_value
      case symbol_kind::S_declaration: // declaration
      case symbol_kind::S_instance: // instance
        value.move< sta::VerilogStmt * > (std::move (that.value));
        break;

      case symbol_kind::S_port_dcls: // port_dcls
      case symbol_kind::S_stmts: // stmts
      case symbol_kind::S_stmt_seq: // stmt_seq
      case symbol_kind::S_specify_stmts: // specify_stmts
      case symbol_kind::S_continuous_assign: // continuous_assign
      case symbol_kind::S_net_assignments: // net_assignments
        value.move< sta::VerilogStmtSeq * > (std::move (that.value));
        break;

      case symbol_kind::S_ID: // ID
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_CONSTANT: // CONSTANT
      case symbol_kind::S_attr_spec_value: // attr_spec_value
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
      basic_symbol (typename Base::kind_type t, sta::VerilogAssign *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sta::VerilogAssign *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sta::VerilogAttrEntry *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sta::VerilogAttrEntry *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sta::VerilogAttrEntrySeq *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sta::VerilogAttrEntrySeq *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sta::VerilogAttrStmt *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sta::VerilogAttrStmt *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sta::VerilogAttrStmtSeq *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sta::VerilogAttrStmtSeq *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sta::VerilogDclArg *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sta::VerilogDclArg *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sta::VerilogDclArgSeq *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sta::VerilogDclArgSeq *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sta::VerilogNet *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sta::VerilogNet *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sta::VerilogNetSeq *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sta::VerilogNetSeq *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sta::VerilogStmt *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sta::VerilogStmt *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, sta::VerilogStmtSeq *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const sta::VerilogStmtSeq *& v, const location_type& l)
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
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_parameter_expr: // parameter_expr
      case symbol_kind::S_parameter_exprs: // parameter_exprs
        value.template destroy< int > ();
        break;

      case symbol_kind::S_port_dcl_type: // port_dcl_type
      case symbol_kind::S_dcl_type: // dcl_type
        value.template destroy< sta::PortDirection * > ();
        break;

      case symbol_kind::S_net_assignment: // net_assignment
        value.template destroy< sta::VerilogAssign * > ();
        break;

      case symbol_kind::S_attr_spec: // attr_spec
        value.template destroy< sta::VerilogAttrEntry * > ();
        break;

      case symbol_kind::S_attr_specs: // attr_specs
        value.template destroy< sta::VerilogAttrEntrySeq * > ();
        break;

      case symbol_kind::S_attr_instance: // attr_instance
        value.template destroy< sta::VerilogAttrStmt * > ();
        break;

      case symbol_kind::S_attr_instance_seq: // attr_instance_seq
        value.template destroy< sta::VerilogAttrStmtSeq * > ();
        break;

      case symbol_kind::S_dcl_arg: // dcl_arg
        value.template destroy< sta::VerilogDclArg * > ();
        break;

      case symbol_kind::S_dcl_args: // dcl_args
        value.template destroy< sta::VerilogDclArgSeq * > ();
        break;

      case symbol_kind::S_port: // port
      case symbol_kind::S_port_expr: // port_expr
      case symbol_kind::S_port_ref: // port_ref
      case symbol_kind::S_net_assign_lhs: // net_assign_lhs
      case symbol_kind::S_inst_named_pin: // inst_named_pin
      case symbol_kind::S_named_pin_net_expr: // named_pin_net_expr
      case symbol_kind::S_net_named: // net_named
      case symbol_kind::S_net_scalar: // net_scalar
      case symbol_kind::S_net_bit_select: // net_bit_select
      case symbol_kind::S_net_part_select: // net_part_select
      case symbol_kind::S_net_constant: // net_constant
      case symbol_kind::S_net_expr_concat: // net_expr_concat
      case symbol_kind::S_net_expr: // net_expr
        value.template destroy< sta::VerilogNet * > ();
        break;

      case symbol_kind::S_port_list: // port_list
      case symbol_kind::S_port_refs: // port_refs
      case symbol_kind::S_inst_pins: // inst_pins
      case symbol_kind::S_inst_ordered_pins: // inst_ordered_pins
      case symbol_kind::S_inst_named_pins: // inst_named_pins
      case symbol_kind::S_net_exprs: // net_exprs
        value.template destroy< sta::VerilogNetSeq * > ();
        break;

      case symbol_kind::S_port_dcl: // port_dcl
      case symbol_kind::S_stmt: // stmt
      case symbol_kind::S_specify_block: // specify_block
      case symbol_kind::S_parameter: // parameter
      case symbol_kind::S_parameter_dcls: // parameter_dcls
      case symbol_kind::S_parameter_dcl: // parameter_dcl
      case symbol_kind::S_defparam: // defparam
      case symbol_kind::S_param_values: // param_values
      case symbol_kind::S_param_value: // param_value
      case symbol_kind::S_declaration: // declaration
      case symbol_kind::S_instance: // instance
        value.template destroy< sta::VerilogStmt * > ();
        break;

      case symbol_kind::S_port_dcls: // port_dcls
      case symbol_kind::S_stmts: // stmts
      case symbol_kind::S_stmt_seq: // stmt_seq
      case symbol_kind::S_specify_stmts: // specify_stmts
      case symbol_kind::S_continuous_assign: // continuous_assign
      case symbol_kind::S_net_assignments: // net_assignments
        value.template destroy< sta::VerilogStmtSeq * > ();
        break;

      case symbol_kind::S_ID: // ID
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_CONSTANT: // CONSTANT
      case symbol_kind::S_attr_spec_value: // attr_spec_value
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
        return VerilogParse::symbol_name (this->kind ());
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
                   || (token::MODULE <= tok && tok <= token::ATTR_CLOSED)
                   || tok == 45
                   || tok == 43
                   || tok == 42
                   || tok == 47
                   || tok == token::NEG
                   || tok == 59
                   || (40 <= tok && tok <= 41)
                   || tok == 44
                   || tok == 46
                   || tok == 123
                   || tok == 125
                   || tok == 91
                   || tok == 58
                   || tok == 93
                   || tok == 61
                   || tok == 96
                   || tok == 35);
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
        YY_ASSERT (tok == token::INT);
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
        YY_ASSERT ((token::ID <= tok && tok <= token::CONSTANT));
#endif
      }
    };

    /// Build a parser object.
    VerilogParse (VerilogScanner *scanner_yyarg, VerilogReader *reader_yyarg);
    virtual ~VerilogParse ();

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    VerilogParse (const VerilogParse&) = delete;
    /// Non copyable.
    VerilogParse& operator= (const VerilogParse&) = delete;
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
      make_INT (int v, location_type l)
      {
        return symbol_type (token::INT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_INT (const int& v, const location_type& l)
      {
        return symbol_type (token::INT, v, l);
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
      make_STRING (std::string v, location_type l)
      {
        return symbol_type (token::STRING, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_STRING (const std::string& v, const location_type& l)
      {
        return symbol_type (token::STRING, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CONSTANT (std::string v, location_type l)
      {
        return symbol_type (token::CONSTANT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_CONSTANT (const std::string& v, const location_type& l)
      {
        return symbol_type (token::CONSTANT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MODULE (location_type l)
      {
        return symbol_type (token::MODULE, std::move (l));
      }
#else
      static
      symbol_type
      make_MODULE (const location_type& l)
      {
        return symbol_type (token::MODULE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ENDMODULE (location_type l)
      {
        return symbol_type (token::ENDMODULE, std::move (l));
      }
#else
      static
      symbol_type
      make_ENDMODULE (const location_type& l)
      {
        return symbol_type (token::ENDMODULE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ASSIGN (location_type l)
      {
        return symbol_type (token::ASSIGN, std::move (l));
      }
#else
      static
      symbol_type
      make_ASSIGN (const location_type& l)
      {
        return symbol_type (token::ASSIGN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PARAMETER (location_type l)
      {
        return symbol_type (token::PARAMETER, std::move (l));
      }
#else
      static
      symbol_type
      make_PARAMETER (const location_type& l)
      {
        return symbol_type (token::PARAMETER, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DEFPARAM (location_type l)
      {
        return symbol_type (token::DEFPARAM, std::move (l));
      }
#else
      static
      symbol_type
      make_DEFPARAM (const location_type& l)
      {
        return symbol_type (token::DEFPARAM, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SPECIFY (location_type l)
      {
        return symbol_type (token::SPECIFY, std::move (l));
      }
#else
      static
      symbol_type
      make_SPECIFY (const location_type& l)
      {
        return symbol_type (token::SPECIFY, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ENDSPECIFY (location_type l)
      {
        return symbol_type (token::ENDSPECIFY, std::move (l));
      }
#else
      static
      symbol_type
      make_ENDSPECIFY (const location_type& l)
      {
        return symbol_type (token::ENDSPECIFY, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SPECPARAM (location_type l)
      {
        return symbol_type (token::SPECPARAM, std::move (l));
      }
#else
      static
      symbol_type
      make_SPECPARAM (const location_type& l)
      {
        return symbol_type (token::SPECPARAM, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_WIRE (location_type l)
      {
        return symbol_type (token::WIRE, std::move (l));
      }
#else
      static
      symbol_type
      make_WIRE (const location_type& l)
      {
        return symbol_type (token::WIRE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_WAND (location_type l)
      {
        return symbol_type (token::WAND, std::move (l));
      }
#else
      static
      symbol_type
      make_WAND (const location_type& l)
      {
        return symbol_type (token::WAND, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_WOR (location_type l)
      {
        return symbol_type (token::WOR, std::move (l));
      }
#else
      static
      symbol_type
      make_WOR (const location_type& l)
      {
        return symbol_type (token::WOR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TRI (location_type l)
      {
        return symbol_type (token::TRI, std::move (l));
      }
#else
      static
      symbol_type
      make_TRI (const location_type& l)
      {
        return symbol_type (token::TRI, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INPUT (location_type l)
      {
        return symbol_type (token::INPUT, std::move (l));
      }
#else
      static
      symbol_type
      make_INPUT (const location_type& l)
      {
        return symbol_type (token::INPUT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OUTPUT (location_type l)
      {
        return symbol_type (token::OUTPUT, std::move (l));
      }
#else
      static
      symbol_type
      make_OUTPUT (const location_type& l)
      {
        return symbol_type (token::OUTPUT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INOUT (location_type l)
      {
        return symbol_type (token::INOUT, std::move (l));
      }
#else
      static
      symbol_type
      make_INOUT (const location_type& l)
      {
        return symbol_type (token::INOUT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SUPPLY1 (location_type l)
      {
        return symbol_type (token::SUPPLY1, std::move (l));
      }
#else
      static
      symbol_type
      make_SUPPLY1 (const location_type& l)
      {
        return symbol_type (token::SUPPLY1, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SUPPLY0 (location_type l)
      {
        return symbol_type (token::SUPPLY0, std::move (l));
      }
#else
      static
      symbol_type
      make_SUPPLY0 (const location_type& l)
      {
        return symbol_type (token::SUPPLY0, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_REG (location_type l)
      {
        return symbol_type (token::REG, std::move (l));
      }
#else
      static
      symbol_type
      make_REG (const location_type& l)
      {
        return symbol_type (token::REG, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ATTR_OPEN (location_type l)
      {
        return symbol_type (token::ATTR_OPEN, std::move (l));
      }
#else
      static
      symbol_type
      make_ATTR_OPEN (const location_type& l)
      {
        return symbol_type (token::ATTR_OPEN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ATTR_CLOSED (location_type l)
      {
        return symbol_type (token::ATTR_CLOSED, std::move (l));
      }
#else
      static
      symbol_type
      make_ATTR_CLOSED (const location_type& l)
      {
        return symbol_type (token::ATTR_CLOSED, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NEG (location_type l)
      {
        return symbol_type (token::NEG, std::move (l));
      }
#else
      static
      symbol_type
      make_NEG (const location_type& l)
      {
        return symbol_type (token::NEG, l);
      }
#endif


  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    VerilogParse (const VerilogParse&);
    /// Non copyable.
    VerilogParse& operator= (const VerilogParse&);
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

    static const signed char yypact_ninf_;
    static const short yytable_ninf_;

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
      yylast_ = 367,     ///< Last index in yytable_.
      yynnts_ = 53,  ///< Number of nonterminal symbols.
      yyfinal_ = 3 ///< Termination state number.
    };


    // User arguments.
    VerilogScanner *scanner;
    VerilogReader *reader;

  };


#line 80 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
} // sta
#line 2109 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.hh"




#endif // !YY_YY_HOME_EATHESWAR_SILISFOUNDATION_SILIS_SYNTHENGINE_BUILD_OPENSTA_BUILD_VERILOGPARSE_HH_INCLUDED
