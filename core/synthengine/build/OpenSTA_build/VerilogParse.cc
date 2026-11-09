// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

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

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.



// First part of user prologue.
#line 25 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"

#include <cstdlib>
#include <string>

#include "Report.hh"
#include "PortDirection.hh"
#include "VerilogReader.hh"
#include "verilog/VerilogReaderPvt.hh"
#include "verilog/VerilogScanner.hh"

#undef yylex
#define yylex scanner->lex

// warning: variable 'yynerrs_' set but not used
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

#define loc_line(loc) loc.begin.line

void
sta::VerilogParse::error(const location_type &loc,
                         const std::string &msg)
{
  reader->report()->fileError(171,reader->filename(),loc.begin.line, "{}",msg);
}


#line 68 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"


#include "VerilogParse.hh"




#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 80 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
namespace sta {
#line 166 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"

  /// Build a parser object.
  VerilogParse::VerilogParse (VerilogScanner *scanner_yyarg, VerilogReader *reader_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      scanner (scanner_yyarg),
      reader (reader_yyarg)
  {}

  VerilogParse::~VerilogParse ()
  {}

  VerilogParse::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  VerilogParse::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_parameter_expr: // parameter_expr
      case symbol_kind::S_parameter_exprs: // parameter_exprs
        value.copy< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_port_dcl_type: // port_dcl_type
      case symbol_kind::S_dcl_type: // dcl_type
        value.copy< sta::PortDirection * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_net_assignment: // net_assignment
        value.copy< sta::VerilogAssign * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_attr_spec: // attr_spec
        value.copy< sta::VerilogAttrEntry * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_attr_specs: // attr_specs
        value.copy< sta::VerilogAttrEntrySeq * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_attr_instance: // attr_instance
        value.copy< sta::VerilogAttrStmt * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_attr_instance_seq: // attr_instance_seq
        value.copy< sta::VerilogAttrStmtSeq * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_dcl_arg: // dcl_arg
        value.copy< sta::VerilogDclArg * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_dcl_args: // dcl_args
        value.copy< sta::VerilogDclArgSeq * > (YY_MOVE (that.value));
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
        value.copy< sta::VerilogNet * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_port_list: // port_list
      case symbol_kind::S_port_refs: // port_refs
      case symbol_kind::S_inst_pins: // inst_pins
      case symbol_kind::S_inst_ordered_pins: // inst_ordered_pins
      case symbol_kind::S_inst_named_pins: // inst_named_pins
      case symbol_kind::S_net_exprs: // net_exprs
        value.copy< sta::VerilogNetSeq * > (YY_MOVE (that.value));
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
        value.copy< sta::VerilogStmt * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_port_dcls: // port_dcls
      case symbol_kind::S_stmts: // stmts
      case symbol_kind::S_stmt_seq: // stmt_seq
      case symbol_kind::S_specify_stmts: // specify_stmts
      case symbol_kind::S_continuous_assign: // continuous_assign
      case symbol_kind::S_net_assignments: // net_assignments
        value.copy< sta::VerilogStmtSeq * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ID: // ID
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_CONSTANT: // CONSTANT
      case symbol_kind::S_attr_spec_value: // attr_spec_value
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }




  template <typename Base>
  VerilogParse::symbol_kind_type
  VerilogParse::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  VerilogParse::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  VerilogParse::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_parameter_expr: // parameter_expr
      case symbol_kind::S_parameter_exprs: // parameter_exprs
        value.move< int > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_port_dcl_type: // port_dcl_type
      case symbol_kind::S_dcl_type: // dcl_type
        value.move< sta::PortDirection * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_net_assignment: // net_assignment
        value.move< sta::VerilogAssign * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_attr_spec: // attr_spec
        value.move< sta::VerilogAttrEntry * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_attr_specs: // attr_specs
        value.move< sta::VerilogAttrEntrySeq * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_attr_instance: // attr_instance
        value.move< sta::VerilogAttrStmt * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_attr_instance_seq: // attr_instance_seq
        value.move< sta::VerilogAttrStmtSeq * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_dcl_arg: // dcl_arg
        value.move< sta::VerilogDclArg * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_dcl_args: // dcl_args
        value.move< sta::VerilogDclArgSeq * > (YY_MOVE (s.value));
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
        value.move< sta::VerilogNet * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_port_list: // port_list
      case symbol_kind::S_port_refs: // port_refs
      case symbol_kind::S_inst_pins: // inst_pins
      case symbol_kind::S_inst_ordered_pins: // inst_ordered_pins
      case symbol_kind::S_inst_named_pins: // inst_named_pins
      case symbol_kind::S_net_exprs: // net_exprs
        value.move< sta::VerilogNetSeq * > (YY_MOVE (s.value));
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
        value.move< sta::VerilogStmt * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_port_dcls: // port_dcls
      case symbol_kind::S_stmts: // stmts
      case symbol_kind::S_stmt_seq: // stmt_seq
      case symbol_kind::S_specify_stmts: // specify_stmts
      case symbol_kind::S_continuous_assign: // continuous_assign
      case symbol_kind::S_net_assignments: // net_assignments
        value.move< sta::VerilogStmtSeq * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_ID: // ID
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_CONSTANT: // CONSTANT
      case symbol_kind::S_attr_spec_value: // attr_spec_value
        value.move< std::string > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  VerilogParse::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  VerilogParse::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  VerilogParse::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  VerilogParse::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  VerilogParse::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  VerilogParse::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  VerilogParse::symbol_kind_type
  VerilogParse::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  VerilogParse::symbol_kind_type
  VerilogParse::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  VerilogParse::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  VerilogParse::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  VerilogParse::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  VerilogParse::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  VerilogParse::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  VerilogParse::symbol_kind_type
  VerilogParse::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  VerilogParse::stack_symbol_type::stack_symbol_type ()
  {}

  VerilogParse::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_parameter_expr: // parameter_expr
      case symbol_kind::S_parameter_exprs: // parameter_exprs
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_port_dcl_type: // port_dcl_type
      case symbol_kind::S_dcl_type: // dcl_type
        value.YY_MOVE_OR_COPY< sta::PortDirection * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_net_assignment: // net_assignment
        value.YY_MOVE_OR_COPY< sta::VerilogAssign * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_attr_spec: // attr_spec
        value.YY_MOVE_OR_COPY< sta::VerilogAttrEntry * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_attr_specs: // attr_specs
        value.YY_MOVE_OR_COPY< sta::VerilogAttrEntrySeq * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_attr_instance: // attr_instance
        value.YY_MOVE_OR_COPY< sta::VerilogAttrStmt * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_attr_instance_seq: // attr_instance_seq
        value.YY_MOVE_OR_COPY< sta::VerilogAttrStmtSeq * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_dcl_arg: // dcl_arg
        value.YY_MOVE_OR_COPY< sta::VerilogDclArg * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_dcl_args: // dcl_args
        value.YY_MOVE_OR_COPY< sta::VerilogDclArgSeq * > (YY_MOVE (that.value));
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
        value.YY_MOVE_OR_COPY< sta::VerilogNet * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_port_list: // port_list
      case symbol_kind::S_port_refs: // port_refs
      case symbol_kind::S_inst_pins: // inst_pins
      case symbol_kind::S_inst_ordered_pins: // inst_ordered_pins
      case symbol_kind::S_inst_named_pins: // inst_named_pins
      case symbol_kind::S_net_exprs: // net_exprs
        value.YY_MOVE_OR_COPY< sta::VerilogNetSeq * > (YY_MOVE (that.value));
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
        value.YY_MOVE_OR_COPY< sta::VerilogStmt * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_port_dcls: // port_dcls
      case symbol_kind::S_stmts: // stmts
      case symbol_kind::S_stmt_seq: // stmt_seq
      case symbol_kind::S_specify_stmts: // specify_stmts
      case symbol_kind::S_continuous_assign: // continuous_assign
      case symbol_kind::S_net_assignments: // net_assignments
        value.YY_MOVE_OR_COPY< sta::VerilogStmtSeq * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ID: // ID
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_CONSTANT: // CONSTANT
      case symbol_kind::S_attr_spec_value: // attr_spec_value
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  VerilogParse::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_parameter_expr: // parameter_expr
      case symbol_kind::S_parameter_exprs: // parameter_exprs
        value.move< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_port_dcl_type: // port_dcl_type
      case symbol_kind::S_dcl_type: // dcl_type
        value.move< sta::PortDirection * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_net_assignment: // net_assignment
        value.move< sta::VerilogAssign * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_attr_spec: // attr_spec
        value.move< sta::VerilogAttrEntry * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_attr_specs: // attr_specs
        value.move< sta::VerilogAttrEntrySeq * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_attr_instance: // attr_instance
        value.move< sta::VerilogAttrStmt * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_attr_instance_seq: // attr_instance_seq
        value.move< sta::VerilogAttrStmtSeq * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_dcl_arg: // dcl_arg
        value.move< sta::VerilogDclArg * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_dcl_args: // dcl_args
        value.move< sta::VerilogDclArgSeq * > (YY_MOVE (that.value));
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
        value.move< sta::VerilogNet * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_port_list: // port_list
      case symbol_kind::S_port_refs: // port_refs
      case symbol_kind::S_inst_pins: // inst_pins
      case symbol_kind::S_inst_ordered_pins: // inst_ordered_pins
      case symbol_kind::S_inst_named_pins: // inst_named_pins
      case symbol_kind::S_net_exprs: // net_exprs
        value.move< sta::VerilogNetSeq * > (YY_MOVE (that.value));
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
        value.move< sta::VerilogStmt * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_port_dcls: // port_dcls
      case symbol_kind::S_stmts: // stmts
      case symbol_kind::S_stmt_seq: // stmt_seq
      case symbol_kind::S_specify_stmts: // specify_stmts
      case symbol_kind::S_continuous_assign: // continuous_assign
      case symbol_kind::S_net_assignments: // net_assignments
        value.move< sta::VerilogStmtSeq * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ID: // ID
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_CONSTANT: // CONSTANT
      case symbol_kind::S_attr_spec_value: // attr_spec_value
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  VerilogParse::stack_symbol_type&
  VerilogParse::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_parameter_expr: // parameter_expr
      case symbol_kind::S_parameter_exprs: // parameter_exprs
        value.copy< int > (that.value);
        break;

      case symbol_kind::S_port_dcl_type: // port_dcl_type
      case symbol_kind::S_dcl_type: // dcl_type
        value.copy< sta::PortDirection * > (that.value);
        break;

      case symbol_kind::S_net_assignment: // net_assignment
        value.copy< sta::VerilogAssign * > (that.value);
        break;

      case symbol_kind::S_attr_spec: // attr_spec
        value.copy< sta::VerilogAttrEntry * > (that.value);
        break;

      case symbol_kind::S_attr_specs: // attr_specs
        value.copy< sta::VerilogAttrEntrySeq * > (that.value);
        break;

      case symbol_kind::S_attr_instance: // attr_instance
        value.copy< sta::VerilogAttrStmt * > (that.value);
        break;

      case symbol_kind::S_attr_instance_seq: // attr_instance_seq
        value.copy< sta::VerilogAttrStmtSeq * > (that.value);
        break;

      case symbol_kind::S_dcl_arg: // dcl_arg
        value.copy< sta::VerilogDclArg * > (that.value);
        break;

      case symbol_kind::S_dcl_args: // dcl_args
        value.copy< sta::VerilogDclArgSeq * > (that.value);
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
        value.copy< sta::VerilogNet * > (that.value);
        break;

      case symbol_kind::S_port_list: // port_list
      case symbol_kind::S_port_refs: // port_refs
      case symbol_kind::S_inst_pins: // inst_pins
      case symbol_kind::S_inst_ordered_pins: // inst_ordered_pins
      case symbol_kind::S_inst_named_pins: // inst_named_pins
      case symbol_kind::S_net_exprs: // net_exprs
        value.copy< sta::VerilogNetSeq * > (that.value);
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
        value.copy< sta::VerilogStmt * > (that.value);
        break;

      case symbol_kind::S_port_dcls: // port_dcls
      case symbol_kind::S_stmts: // stmts
      case symbol_kind::S_stmt_seq: // stmt_seq
      case symbol_kind::S_specify_stmts: // specify_stmts
      case symbol_kind::S_continuous_assign: // continuous_assign
      case symbol_kind::S_net_assignments: // net_assignments
        value.copy< sta::VerilogStmtSeq * > (that.value);
        break;

      case symbol_kind::S_ID: // ID
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_CONSTANT: // CONSTANT
      case symbol_kind::S_attr_spec_value: // attr_spec_value
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  VerilogParse::stack_symbol_type&
  VerilogParse::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_parameter_expr: // parameter_expr
      case symbol_kind::S_parameter_exprs: // parameter_exprs
        value.move< int > (that.value);
        break;

      case symbol_kind::S_port_dcl_type: // port_dcl_type
      case symbol_kind::S_dcl_type: // dcl_type
        value.move< sta::PortDirection * > (that.value);
        break;

      case symbol_kind::S_net_assignment: // net_assignment
        value.move< sta::VerilogAssign * > (that.value);
        break;

      case symbol_kind::S_attr_spec: // attr_spec
        value.move< sta::VerilogAttrEntry * > (that.value);
        break;

      case symbol_kind::S_attr_specs: // attr_specs
        value.move< sta::VerilogAttrEntrySeq * > (that.value);
        break;

      case symbol_kind::S_attr_instance: // attr_instance
        value.move< sta::VerilogAttrStmt * > (that.value);
        break;

      case symbol_kind::S_attr_instance_seq: // attr_instance_seq
        value.move< sta::VerilogAttrStmtSeq * > (that.value);
        break;

      case symbol_kind::S_dcl_arg: // dcl_arg
        value.move< sta::VerilogDclArg * > (that.value);
        break;

      case symbol_kind::S_dcl_args: // dcl_args
        value.move< sta::VerilogDclArgSeq * > (that.value);
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
        value.move< sta::VerilogNet * > (that.value);
        break;

      case symbol_kind::S_port_list: // port_list
      case symbol_kind::S_port_refs: // port_refs
      case symbol_kind::S_inst_pins: // inst_pins
      case symbol_kind::S_inst_ordered_pins: // inst_ordered_pins
      case symbol_kind::S_inst_named_pins: // inst_named_pins
      case symbol_kind::S_net_exprs: // net_exprs
        value.move< sta::VerilogNetSeq * > (that.value);
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
        value.move< sta::VerilogStmt * > (that.value);
        break;

      case symbol_kind::S_port_dcls: // port_dcls
      case symbol_kind::S_stmts: // stmts
      case symbol_kind::S_stmt_seq: // stmt_seq
      case symbol_kind::S_specify_stmts: // specify_stmts
      case symbol_kind::S_continuous_assign: // continuous_assign
      case symbol_kind::S_net_assignments: // net_assignments
        value.move< sta::VerilogStmtSeq * > (that.value);
        break;

      case symbol_kind::S_ID: // ID
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_CONSTANT: // CONSTANT
      case symbol_kind::S_attr_spec_value: // attr_spec_value
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  VerilogParse::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  VerilogParse::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  VerilogParse::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  VerilogParse::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  VerilogParse::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  VerilogParse::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  VerilogParse::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  VerilogParse::debug_level_type
  VerilogParse::debug_level () const
  {
    return yydebug_;
  }

  void
  VerilogParse::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  VerilogParse::state_type
  VerilogParse::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  VerilogParse::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  VerilogParse::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  VerilogParse::operator() ()
  {
    return parse ();
  }

  int
  VerilogParse::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, &yyla.location));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_parameter_expr: // parameter_expr
      case symbol_kind::S_parameter_exprs: // parameter_exprs
        yylhs.value.emplace< int > ();
        break;

      case symbol_kind::S_port_dcl_type: // port_dcl_type
      case symbol_kind::S_dcl_type: // dcl_type
        yylhs.value.emplace< sta::PortDirection * > ();
        break;

      case symbol_kind::S_net_assignment: // net_assignment
        yylhs.value.emplace< sta::VerilogAssign * > ();
        break;

      case symbol_kind::S_attr_spec: // attr_spec
        yylhs.value.emplace< sta::VerilogAttrEntry * > ();
        break;

      case symbol_kind::S_attr_specs: // attr_specs
        yylhs.value.emplace< sta::VerilogAttrEntrySeq * > ();
        break;

      case symbol_kind::S_attr_instance: // attr_instance
        yylhs.value.emplace< sta::VerilogAttrStmt * > ();
        break;

      case symbol_kind::S_attr_instance_seq: // attr_instance_seq
        yylhs.value.emplace< sta::VerilogAttrStmtSeq * > ();
        break;

      case symbol_kind::S_dcl_arg: // dcl_arg
        yylhs.value.emplace< sta::VerilogDclArg * > ();
        break;

      case symbol_kind::S_dcl_args: // dcl_args
        yylhs.value.emplace< sta::VerilogDclArgSeq * > ();
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
        yylhs.value.emplace< sta::VerilogNet * > ();
        break;

      case symbol_kind::S_port_list: // port_list
      case symbol_kind::S_port_refs: // port_refs
      case symbol_kind::S_inst_pins: // inst_pins
      case symbol_kind::S_inst_ordered_pins: // inst_ordered_pins
      case symbol_kind::S_inst_named_pins: // inst_named_pins
      case symbol_kind::S_net_exprs: // net_exprs
        yylhs.value.emplace< sta::VerilogNetSeq * > ();
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
        yylhs.value.emplace< sta::VerilogStmt * > ();
        break;

      case symbol_kind::S_port_dcls: // port_dcls
      case symbol_kind::S_stmts: // stmts
      case symbol_kind::S_stmt_seq: // stmt_seq
      case symbol_kind::S_specify_stmts: // specify_stmts
      case symbol_kind::S_continuous_assign: // continuous_assign
      case symbol_kind::S_net_assignments: // net_assignments
        yylhs.value.emplace< sta::VerilogStmtSeq * > ();
        break;

      case symbol_kind::S_ID: // ID
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_CONSTANT: // CONSTANT
      case symbol_kind::S_attr_spec_value: // attr_spec_value
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 5: // module: attr_instance_seq MODULE ID ';' stmts ENDMODULE
#line 136 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { reader->makeModule(yystack_[3].value.as < std::string > (), new sta::VerilogNetSeq,yystack_[1].value.as < sta::VerilogStmtSeq * > (), yystack_[5].value.as < sta::VerilogAttrStmtSeq * > (), loc_line(yystack_[4].location));}
#line 1314 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 6: // module: attr_instance_seq MODULE ID '(' ')' ';' stmts ENDMODULE
#line 138 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { reader->makeModule(yystack_[5].value.as < std::string > (), new sta::VerilogNetSeq,yystack_[1].value.as < sta::VerilogStmtSeq * > (), yystack_[7].value.as < sta::VerilogAttrStmtSeq * > (), loc_line(yystack_[6].location));}
#line 1320 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 7: // module: attr_instance_seq MODULE ID '(' port_list ')' ';' stmts ENDMODULE
#line 140 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { reader->makeModule(yystack_[6].value.as < std::string > (), yystack_[4].value.as < sta::VerilogNetSeq * > (), yystack_[1].value.as < sta::VerilogStmtSeq * > (), yystack_[8].value.as < sta::VerilogAttrStmtSeq * > (), loc_line(yystack_[7].location)); }
#line 1326 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 8: // module: attr_instance_seq MODULE ID '(' port_dcls ')' ';' stmts ENDMODULE
#line 142 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { reader->makeModule(yystack_[6].value.as < std::string > (), yystack_[4].value.as < sta::VerilogStmtSeq * > (), yystack_[1].value.as < sta::VerilogStmtSeq * > (), yystack_[8].value.as < sta::VerilogAttrStmtSeq * > (), loc_line(yystack_[7].location)); }
#line 1332 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 9: // port_list: port
#line 147 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNetSeq * > () = new sta::VerilogNetSeq;
          yylhs.value.as < sta::VerilogNetSeq * > ()->push_back(yystack_[0].value.as < sta::VerilogNet * > ());
	}
#line 1340 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 10: // port_list: port_list ',' port
#line 151 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yystack_[2].value.as < sta::VerilogNetSeq * > ()->push_back(yystack_[0].value.as < sta::VerilogNet * > ()); yylhs.value.as < sta::VerilogNetSeq * > () = yystack_[2].value.as < sta::VerilogNetSeq * > (); }
#line 1346 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 11: // port: port_expr
#line 155 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = yystack_[0].value.as < sta::VerilogNet * > (); }
#line 1352 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 12: // port: '.' ID '(' ')'
#line 157 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = reader->makeNetNamedPortRefScalar(yystack_[2].value.as < std::string > (), nullptr);}
#line 1358 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 13: // port: '.' ID '(' port_expr ')'
#line 159 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = reader->makeNetNamedPortRefScalar(yystack_[3].value.as < std::string > (), yystack_[1].value.as < sta::VerilogNet * > ());}
#line 1364 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 14: // port_expr: port_ref
#line 163 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = yystack_[0].value.as < sta::VerilogNet * > (); }
#line 1370 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 15: // port_expr: '{' port_refs '}'
#line 165 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = reader->makeNetConcat(yystack_[1].value.as < sta::VerilogNetSeq * > ()); }
#line 1376 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 16: // port_refs: port_ref
#line 169 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNetSeq * > () = new sta::VerilogNetSeq;
	  yylhs.value.as < sta::VerilogNetSeq * > ()->push_back(yystack_[0].value.as < sta::VerilogNet * > ());
	}
#line 1384 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 17: // port_refs: port_refs ',' port_ref
#line 173 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yystack_[2].value.as < sta::VerilogNetSeq * > ()->push_back(yystack_[0].value.as < sta::VerilogNet * > ()); yylhs.value.as < sta::VerilogNetSeq * > () = yystack_[2].value.as < sta::VerilogNetSeq * > (); }
#line 1390 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 18: // port_ref: net_scalar
#line 177 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = yystack_[0].value.as < sta::VerilogNet * > (); }
#line 1396 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 19: // port_ref: net_bit_select
#line 178 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = yystack_[0].value.as < sta::VerilogNet * > (); }
#line 1402 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 20: // port_ref: net_part_select
#line 179 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = yystack_[0].value.as < sta::VerilogNet * > (); }
#line 1408 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 21: // port_dcls: port_dcl
#line 184 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmtSeq * > () = new sta::VerilogStmtSeq;
	  yylhs.value.as < sta::VerilogStmtSeq * > ()->push_back(yystack_[0].value.as < sta::VerilogStmt * > ());
	}
#line 1416 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 22: // port_dcls: port_dcls ',' port_dcl
#line 188 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmtSeq * > () = yystack_[2].value.as < sta::VerilogStmtSeq * > ();
	  yystack_[2].value.as < sta::VerilogStmtSeq * > ()->push_back(yystack_[0].value.as < sta::VerilogStmt * > ());
	}
#line 1424 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 23: // port_dcls: port_dcls ',' dcl_arg
#line 192 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        {
	  sta::VerilogDcl *dcl = dynamic_cast<sta::VerilogDcl*>(yystack_[2].value.as < sta::VerilogStmtSeq * > ()->back());
	  dcl->appendArg(yystack_[0].value.as < sta::VerilogDclArg * > ());
	  yylhs.value.as < sta::VerilogStmtSeq * > () = yystack_[2].value.as < sta::VerilogStmtSeq * > ();
	}
#line 1434 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 24: // port_dcl: attr_instance_seq port_dcl_type dcl_arg
#line 201 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmt * > () = reader->makeDcl(yystack_[1].value.as < sta::PortDirection * > (), yystack_[0].value.as < sta::VerilogDclArg * > (), yystack_[2].value.as < sta::VerilogAttrStmtSeq * > (), loc_line(yystack_[1].location)); }
#line 1440 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 25: // port_dcl: attr_instance_seq port_dcl_type '[' INT ':' INT ']' dcl_arg
#line 203 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmt * > () = reader->makeDclBus(yystack_[6].value.as < sta::PortDirection * > (), yystack_[4].value.as < int > (), yystack_[2].value.as < int > (), yystack_[0].value.as < sta::VerilogDclArg * > (), yystack_[7].value.as < sta::VerilogAttrStmtSeq * > (), loc_line(yystack_[6].location)); }
#line 1446 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 26: // port_dcl_type: INPUT
#line 207 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
              { yylhs.value.as < sta::PortDirection * > () = sta::PortDirection::input(); }
#line 1452 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 27: // port_dcl_type: INPUT WIRE
#line 208 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
                   { yylhs.value.as < sta::PortDirection * > () = sta::PortDirection::input(); }
#line 1458 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 28: // port_dcl_type: INOUT
#line 209 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
              { yylhs.value.as < sta::PortDirection * > () = sta::PortDirection::bidirect(); }
#line 1464 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 29: // port_dcl_type: INOUT REG
#line 210 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
                  { yylhs.value.as < sta::PortDirection * > () = sta::PortDirection::bidirect(); }
#line 1470 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 30: // port_dcl_type: INOUT WIRE
#line 211 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
                   { yylhs.value.as < sta::PortDirection * > () = sta::PortDirection::bidirect(); }
#line 1476 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 31: // port_dcl_type: OUTPUT
#line 212 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
               { yylhs.value.as < sta::PortDirection * > () = sta::PortDirection::output(); }
#line 1482 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 32: // port_dcl_type: OUTPUT WIRE
#line 213 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
                    { yylhs.value.as < sta::PortDirection * > () = sta::PortDirection::output(); }
#line 1488 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 33: // port_dcl_type: OUTPUT REG
#line 214 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
                   { yylhs.value.as < sta::PortDirection * > () = sta::PortDirection::output(); }
#line 1494 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 34: // stmts: %empty
#line 219 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmtSeq * > () = new sta::VerilogStmtSeq; }
#line 1500 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 35: // stmts: stmts stmt
#line 221 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { if (yystack_[0].value.as < sta::VerilogStmt * > ()) yystack_[1].value.as < sta::VerilogStmtSeq * > ()->push_back(yystack_[0].value.as < sta::VerilogStmt * > ()); yylhs.value.as < sta::VerilogStmtSeq * > () = yystack_[1].value.as < sta::VerilogStmtSeq * > (); }
#line 1506 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 36: // stmts: stmts stmt_seq
#line 224 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { for (sta::VerilogStmt *stmt : *yystack_[0].value.as < sta::VerilogStmtSeq * > ())
	    yystack_[1].value.as < sta::VerilogStmtSeq * > ()->push_back(stmt);
	  delete yystack_[0].value.as < sta::VerilogStmtSeq * > ();
	  yylhs.value.as < sta::VerilogStmtSeq * > () = yystack_[1].value.as < sta::VerilogStmtSeq * > ();
	}
#line 1516 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 37: // stmt: parameter
#line 232 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmt * > () = yystack_[0].value.as < sta::VerilogStmt * > (); }
#line 1522 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 38: // stmt: defparam
#line 233 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmt * > () = yystack_[0].value.as < sta::VerilogStmt * > (); }
#line 1528 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 39: // stmt: declaration
#line 234 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmt * > () = yystack_[0].value.as < sta::VerilogStmt * > (); }
#line 1534 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 40: // stmt: instance
#line 235 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmt * > () = yystack_[0].value.as < sta::VerilogStmt * > (); }
#line 1540 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 41: // stmt: specify_block
#line 236 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmt * > () = yystack_[0].value.as < sta::VerilogStmt * > (); }
#line 1546 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 42: // stmt: error ';'
#line 238 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yyerrok; yylhs.value.as < sta::VerilogStmt * > () = nullptr; }
#line 1552 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 43: // stmt_seq: continuous_assign
#line 242 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmtSeq * > () = yystack_[0].value.as < sta::VerilogStmtSeq * > (); }
#line 1558 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 44: // specify_block: SPECIFY specify_stmts ENDSPECIFY
#line 249 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmt * > () = nullptr; }
#line 1564 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 45: // specify_stmts: SPECPARAM parameter_dcl ';'
#line 254 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmtSeq * > () = nullptr; }
#line 1570 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 46: // specify_stmts: specify_stmts SPECPARAM parameter_dcl ';'
#line 256 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmtSeq * > () = nullptr; }
#line 1576 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 47: // parameter: PARAMETER parameter_dcls ';'
#line 262 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmt * > () = nullptr; }
#line 1582 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 48: // parameter: PARAMETER '[' INT ':' INT ']' parameter_dcls ';'
#line 264 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmt * > () = nullptr; }
#line 1588 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 49: // parameter_dcls: parameter_dcl
#line 269 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmt * > () = nullptr; }
#line 1594 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 50: // parameter_dcls: parameter_dcls ',' parameter_dcl
#line 271 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmt * > () = nullptr; }
#line 1600 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 51: // parameter_dcl: ID '=' parameter_expr
#line 276 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmt * > () = nullptr; }
#line 1606 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 52: // parameter_dcl: ID '=' STRING
#line 278 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmt * > () = nullptr; }
#line 1612 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 53: // parameter_expr: ID
#line 283 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < int > () = 0; }
#line 1618 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 54: // parameter_expr: '`' ID
#line 285 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < int > () = 0; }
#line 1624 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 55: // parameter_expr: CONSTANT
#line 287 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < int > () = 0; }
#line 1630 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 56: // parameter_expr: INT
#line 288 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < int > () = yystack_[0].value.as < int > (); }
#line 1636 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 57: // parameter_expr: '-' parameter_expr
#line 290 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < int > () = - yystack_[0].value.as < int > (); }
#line 1642 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 58: // parameter_expr: parameter_expr '+' parameter_expr
#line 292 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < int > () = yystack_[2].value.as < int > () + yystack_[0].value.as < int > (); }
#line 1648 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 59: // parameter_expr: parameter_expr '-' parameter_expr
#line 294 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < int > () = yystack_[2].value.as < int > () - yystack_[0].value.as < int > (); }
#line 1654 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 60: // parameter_expr: parameter_expr '*' parameter_expr
#line 296 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < int > () = yystack_[2].value.as < int > () * yystack_[0].value.as < int > (); }
#line 1660 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 61: // parameter_expr: parameter_expr '/' parameter_expr
#line 298 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < int > () = yystack_[2].value.as < int > () / yystack_[0].value.as < int > (); }
#line 1666 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 62: // parameter_expr: '(' parameter_expr ')'
#line 300 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < int > () = yystack_[1].value.as < int > (); }
#line 1672 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 63: // defparam: DEFPARAM param_values ';'
#line 305 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmt * > () = nullptr; }
#line 1678 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 64: // param_values: param_value
#line 310 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmt * > () = nullptr; }
#line 1684 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 65: // param_values: param_values ',' param_value
#line 312 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmt * > () = nullptr; }
#line 1690 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 66: // param_value: ID '=' parameter_expr
#line 317 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmt * > () = nullptr; }
#line 1696 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 67: // param_value: ID '=' STRING
#line 319 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmt * > () = nullptr; }
#line 1702 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 68: // declaration: attr_instance_seq dcl_type dcl_args ';'
#line 324 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmt * > () = reader->makeDcl(yystack_[2].value.as < sta::PortDirection * > (), yystack_[1].value.as < sta::VerilogDclArgSeq * > (), yystack_[3].value.as < sta::VerilogAttrStmtSeq * > (), loc_line(yystack_[2].location)); }
#line 1708 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 69: // declaration: attr_instance_seq dcl_type '[' INT ':' INT ']' dcl_args ';'
#line 326 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmt * > () = reader->makeDclBus(yystack_[7].value.as < sta::PortDirection * > (), yystack_[5].value.as < int > (), yystack_[3].value.as < int > (), yystack_[1].value.as < sta::VerilogDclArgSeq * > (), yystack_[8].value.as < sta::VerilogAttrStmtSeq * > (),loc_line(yystack_[7].location)); }
#line 1714 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 70: // dcl_type: INPUT
#line 330 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
              { yylhs.value.as < sta::PortDirection * > () = sta::PortDirection::input(); }
#line 1720 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 71: // dcl_type: INOUT
#line 331 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
              { yylhs.value.as < sta::PortDirection * > () = sta::PortDirection::bidirect(); }
#line 1726 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 72: // dcl_type: OUTPUT
#line 332 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
               { yylhs.value.as < sta::PortDirection * > () = sta::PortDirection::output(); }
#line 1732 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 73: // dcl_type: SUPPLY0
#line 333 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
                { yylhs.value.as < sta::PortDirection * > () = sta::PortDirection::ground(); }
#line 1738 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 74: // dcl_type: SUPPLY1
#line 334 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
                { yylhs.value.as < sta::PortDirection * > () = sta::PortDirection::power(); }
#line 1744 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 75: // dcl_type: TRI
#line 335 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
            { yylhs.value.as < sta::PortDirection * > () = sta::PortDirection::tristate(); }
#line 1750 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 76: // dcl_type: WAND
#line 336 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
             { yylhs.value.as < sta::PortDirection * > () = sta::PortDirection::internal(); }
#line 1756 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 77: // dcl_type: WIRE
#line 337 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
             { yylhs.value.as < sta::PortDirection * > () = sta::PortDirection::internal(); }
#line 1762 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 78: // dcl_type: WOR
#line 338 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
            { yylhs.value.as < sta::PortDirection * > () = sta::PortDirection::internal(); }
#line 1768 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 79: // dcl_args: dcl_arg
#line 343 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogDclArgSeq * > () = new sta::VerilogDclArgSeq;
	  yylhs.value.as < sta::VerilogDclArgSeq * > ()->push_back(yystack_[0].value.as < sta::VerilogDclArg * > ());
	}
#line 1776 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 80: // dcl_args: dcl_args ',' dcl_arg
#line 347 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yystack_[2].value.as < sta::VerilogDclArgSeq * > ()->push_back(yystack_[0].value.as < sta::VerilogDclArg * > ());
	  yylhs.value.as < sta::VerilogDclArgSeq * > () = yystack_[2].value.as < sta::VerilogDclArgSeq * > ();
	}
#line 1784 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 81: // dcl_arg: ID
#line 354 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogDclArg * > () = reader->makeDclArg(yystack_[0].value.as < std::string > ()); }
#line 1790 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 82: // dcl_arg: net_assignment
#line 356 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogDclArg * > () = reader->makeDclArg(yystack_[0].value.as < sta::VerilogAssign * > ()); }
#line 1796 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 83: // continuous_assign: ASSIGN net_assignments ';'
#line 361 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmtSeq * > () = yystack_[1].value.as < sta::VerilogStmtSeq * > (); }
#line 1802 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 84: // net_assignments: net_assignment
#line 366 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmtSeq * > () = new sta::VerilogStmtSeq();
	  yylhs.value.as < sta::VerilogStmtSeq * > ()->push_back(yystack_[0].value.as < sta::VerilogAssign * > ());
	}
#line 1810 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 85: // net_assignments: net_assignments ',' net_assignment
#line 370 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yystack_[2].value.as < sta::VerilogStmtSeq * > ()->push_back(yystack_[0].value.as < sta::VerilogAssign * > ()); yylhs.value.as < sta::VerilogStmtSeq * > () = yystack_[2].value.as < sta::VerilogStmtSeq * > (); }
#line 1816 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 86: // net_assignment: net_assign_lhs '=' net_expr
#line 375 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogAssign * > () = reader->makeAssign(yystack_[2].value.as < sta::VerilogNet * > (), yystack_[0].value.as < sta::VerilogNet * > (), loc_line(yystack_[2].location)); }
#line 1822 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 87: // net_assign_lhs: net_named
#line 379 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = yystack_[0].value.as < sta::VerilogNet * > (); }
#line 1828 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 88: // net_assign_lhs: net_expr_concat
#line 380 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
          { yylhs.value.as < sta::VerilogNet * > () = yystack_[0].value.as < sta::VerilogNet * > (); }
#line 1834 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 89: // instance: attr_instance_seq ID ID '(' inst_pins ')' ';'
#line 385 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmt * > () = reader->makeModuleInst(yystack_[5].value.as < std::string > (), yystack_[4].value.as < std::string > (), yystack_[2].value.as < sta::VerilogNetSeq * > (), yystack_[6].value.as < sta::VerilogAttrStmtSeq * > (), loc_line(yystack_[5].location)); }
#line 1840 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 90: // instance: attr_instance_seq ID parameter_values ID '(' inst_pins ')' ';'
#line 387 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogStmt * > () = reader->makeModuleInst(yystack_[6].value.as < std::string > (), yystack_[4].value.as < std::string > (), yystack_[2].value.as < sta::VerilogNetSeq * > (), yystack_[7].value.as < sta::VerilogAttrStmtSeq * > (), loc_line(yystack_[6].location)); }
#line 1846 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 92: // parameter_exprs: parameter_expr
#line 395 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < int > () = yystack_[0].value.as < int > (); }
#line 1852 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 93: // parameter_exprs: '{' parameter_exprs '}'
#line 397 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < int > () = yystack_[1].value.as < int > (); }
#line 1858 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 94: // parameter_exprs: parameter_exprs ',' parameter_expr
#line 398 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < int > () = yystack_[2].value.as < int > (); }
#line 1864 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 95: // inst_pins: %empty
#line 403 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNetSeq * > () = nullptr; }
#line 1870 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 96: // inst_pins: inst_ordered_pins
#line 404 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNetSeq * > () = yystack_[0].value.as < sta::VerilogNetSeq * > (); }
#line 1876 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 97: // inst_pins: inst_named_pins
#line 405 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNetSeq * > () = yystack_[0].value.as < sta::VerilogNetSeq * > (); }
#line 1882 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 98: // inst_ordered_pins: net_expr
#line 411 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNetSeq * > () = new sta::VerilogNetSeq;
	  yylhs.value.as < sta::VerilogNetSeq * > ()->push_back(yystack_[0].value.as < sta::VerilogNet * > ());
	}
#line 1890 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 99: // inst_ordered_pins: inst_ordered_pins ',' net_expr
#line 415 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yystack_[2].value.as < sta::VerilogNetSeq * > ()->push_back(yystack_[0].value.as < sta::VerilogNet * > ()); yylhs.value.as < sta::VerilogNetSeq * > () = yystack_[2].value.as < sta::VerilogNetSeq * > (); }
#line 1896 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 100: // inst_named_pins: inst_named_pin
#line 421 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNetSeq * > () = new sta::VerilogNetSeq;
	  yylhs.value.as < sta::VerilogNetSeq * > ()->push_back(yystack_[0].value.as < sta::VerilogNet * > ());
	}
#line 1904 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 101: // inst_named_pins: inst_named_pins ',' inst_named_pin
#line 425 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yystack_[2].value.as < sta::VerilogNetSeq * > ()->push_back(yystack_[0].value.as < sta::VerilogNet * > ()); yylhs.value.as < sta::VerilogNetSeq * > () = yystack_[2].value.as < sta::VerilogNetSeq * > (); }
#line 1910 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 102: // inst_named_pin: '.' ID '(' ')'
#line 433 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = reader->makeNetNamedPortRefScalarNet(yystack_[2].value.as < std::string > ()); }
#line 1916 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 103: // inst_named_pin: '.' ID '(' ID ')'
#line 435 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = reader->makeNetNamedPortRefScalarNet(yystack_[3].value.as < std::string > (), yystack_[1].value.as < std::string > ()); }
#line 1922 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 104: // inst_named_pin: '.' ID '(' ID '[' INT ']' ')'
#line 437 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = reader->makeNetNamedPortRefBitSelect(yystack_[6].value.as < std::string > (), yystack_[4].value.as < std::string > (), yystack_[2].value.as < int > ()); }
#line 1928 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 105: // inst_named_pin: '.' ID '(' named_pin_net_expr ')'
#line 439 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = reader->makeNetNamedPortRefScalar(yystack_[3].value.as < std::string > (), yystack_[1].value.as < sta::VerilogNet * > ()); }
#line 1934 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 106: // inst_named_pin: '.' ID '[' INT ']' '(' ')'
#line 442 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = reader->makeNetNamedPortRefBit(yystack_[5].value.as < std::string > (), yystack_[3].value.as < int > (), nullptr); }
#line 1940 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 107: // inst_named_pin: '.' ID '[' INT ']' '(' net_expr ')'
#line 444 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = reader->makeNetNamedPortRefBit(yystack_[6].value.as < std::string > (), yystack_[4].value.as < int > (), yystack_[1].value.as < sta::VerilogNet * > ()); }
#line 1946 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 108: // inst_named_pin: '.' ID '[' INT ':' INT ']' '(' ')'
#line 447 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = reader->makeNetNamedPortRefPart(yystack_[7].value.as < std::string > (), yystack_[5].value.as < int > (), yystack_[3].value.as < int > (), nullptr); }
#line 1952 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 109: // inst_named_pin: '.' ID '[' INT ':' INT ']' '(' net_expr ')'
#line 449 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = reader->makeNetNamedPortRefPart(yystack_[8].value.as < std::string > (), yystack_[6].value.as < int > (), yystack_[4].value.as < int > (), yystack_[1].value.as < sta::VerilogNet * > ()); }
#line 1958 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 110: // named_pin_net_expr: net_part_select
#line 453 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = yystack_[0].value.as < sta::VerilogNet * > (); }
#line 1964 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 111: // named_pin_net_expr: net_constant
#line 454 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = yystack_[0].value.as < sta::VerilogNet * > (); }
#line 1970 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 112: // named_pin_net_expr: net_expr_concat
#line 455 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = yystack_[0].value.as < sta::VerilogNet * > (); }
#line 1976 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 113: // net_named: net_scalar
#line 459 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = yystack_[0].value.as < sta::VerilogNet * > (); }
#line 1982 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 114: // net_named: net_bit_select
#line 460 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = yystack_[0].value.as < sta::VerilogNet * > (); }
#line 1988 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 115: // net_named: net_part_select
#line 461 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = yystack_[0].value.as < sta::VerilogNet * > (); }
#line 1994 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 116: // net_scalar: ID
#line 466 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = reader->makeNetScalar(yystack_[0].value.as < std::string > ()); }
#line 2000 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 117: // net_bit_select: ID '[' INT ']'
#line 471 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = reader->makeNetBitSelect(yystack_[3].value.as < std::string > (), yystack_[1].value.as < int > ()); }
#line 2006 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 118: // net_part_select: ID '[' INT ':' INT ']'
#line 476 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = reader->makeNetPartSelect(yystack_[5].value.as < std::string > (), yystack_[3].value.as < int > (), yystack_[1].value.as < int > ()); }
#line 2012 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 119: // net_constant: CONSTANT
#line 481 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = reader->makeNetConstant(yystack_[0].value.as < std::string > (), loc_line(yystack_[0].location)); }
#line 2018 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 120: // net_expr_concat: '{' net_exprs '}'
#line 486 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = reader->makeNetConcat(yystack_[1].value.as < sta::VerilogNetSeq * > ()); }
#line 2024 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 121: // net_exprs: net_expr
#line 491 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNetSeq * > () = new sta::VerilogNetSeq;
	  yylhs.value.as < sta::VerilogNetSeq * > ()->push_back(yystack_[0].value.as < sta::VerilogNet * > ());
	}
#line 2032 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 122: // net_exprs: net_exprs ',' net_expr
#line 495 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yystack_[2].value.as < sta::VerilogNetSeq * > ()->push_back(yystack_[0].value.as < sta::VerilogNet * > ()); yylhs.value.as < sta::VerilogNetSeq * > () = yystack_[2].value.as < sta::VerilogNetSeq * > (); }
#line 2038 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 123: // net_expr: net_scalar
#line 499 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = yystack_[0].value.as < sta::VerilogNet * > (); }
#line 2044 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 124: // net_expr: net_bit_select
#line 500 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = yystack_[0].value.as < sta::VerilogNet * > (); }
#line 2050 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 125: // net_expr: net_part_select
#line 501 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = yystack_[0].value.as < sta::VerilogNet * > (); }
#line 2056 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 126: // net_expr: net_constant
#line 502 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = yystack_[0].value.as < sta::VerilogNet * > (); }
#line 2062 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 127: // net_expr: net_expr_concat
#line 503 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogNet * > () = yystack_[0].value.as < sta::VerilogNet * > (); }
#line 2068 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 128: // attr_instance_seq: %empty
#line 508 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogAttrStmtSeq * > () = new sta::VerilogAttrStmtSeq; }
#line 2074 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 129: // attr_instance_seq: attr_instance_seq attr_instance
#line 510 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { if (yystack_[0].value.as < sta::VerilogAttrStmt * > ()) yystack_[1].value.as < sta::VerilogAttrStmtSeq * > ()->push_back(yystack_[0].value.as < sta::VerilogAttrStmt * > ()); yylhs.value.as < sta::VerilogAttrStmtSeq * > () = yystack_[1].value.as < sta::VerilogAttrStmtSeq * > (); }
#line 2080 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 130: // attr_instance: ATTR_OPEN attr_specs ATTR_CLOSED
#line 515 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogAttrStmt * > () = new sta::VerilogAttrStmt(yystack_[1].value.as < sta::VerilogAttrEntrySeq * > ()); }
#line 2086 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 131: // attr_specs: attr_spec
#line 520 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogAttrEntrySeq * > () = new sta::VerilogAttrEntrySeq;
	  yylhs.value.as < sta::VerilogAttrEntrySeq * > ()->push_back(yystack_[0].value.as < sta::VerilogAttrEntry * > ());
	}
#line 2094 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 132: // attr_specs: attr_specs ',' attr_spec
#line 524 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yystack_[2].value.as < sta::VerilogAttrEntrySeq * > ()->push_back(yystack_[0].value.as < sta::VerilogAttrEntry * > ()); yylhs.value.as < sta::VerilogAttrEntrySeq * > () = yystack_[2].value.as < sta::VerilogAttrEntrySeq * > (); }
#line 2100 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 133: // attr_spec: ID
#line 529 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogAttrEntry * > () = new sta::VerilogAttrEntry(yystack_[0].value.as < std::string > (), "1"); }
#line 2106 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 134: // attr_spec: ID '=' attr_spec_value
#line 531 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < sta::VerilogAttrEntry * > () = new sta::VerilogAttrEntry(yystack_[2].value.as < std::string > (), yystack_[0].value.as < std::string > ()); }
#line 2112 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 135: // attr_spec_value: CONSTANT
#line 536 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2118 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 136: // attr_spec_value: STRING
#line 538 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 2124 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;

  case 137: // attr_spec_value: INT
#line 540 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
        { yylhs.value.as < std::string > () = std::to_string(yystack_[0].value.as < int > ()); }
#line 2130 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"
    break;


#line 2134 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        std::string msg = YY_("syntax error");
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  VerilogParse::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

#if YYDEBUG || 0
  const char *
  VerilogParse::symbol_name (symbol_kind_type yysymbol)
  {
    return yytname_[yysymbol];
  }
#endif // #if YYDEBUG || 0









  const signed char VerilogParse::yypact_ninf_ = -126;

  const short VerilogParse::yytable_ninf_ = -129;

  const short
  VerilogParse::yypact_[] =
  {
    -126,    16,    28,  -126,  -126,    15,    40,    47,  -126,    10,
      36,    -6,  -126,  -126,    91,   104,  -126,    47,   201,    43,
      85,   127,   129,   145,  -126,  -126,  -126,   180,  -126,  -126,
    -126,  -126,   151,  -126,  -126,  -126,  -126,  -126,   115,  -126,
      35,    13,   157,   149,  -126,  -126,  -126,  -126,  -126,  -126,
    -126,  -126,   310,   164,  -126,   142,    -8,  -126,   141,   114,
     174,    79,   210,     9,    17,    11,  -126,    19,    59,  -126,
     191,  -126,  -126,  -126,  -126,  -126,   193,   184,    64,  -126,
     195,   122,  -126,   235,   218,    14,  -126,  -126,  -126,  -126,
    -126,  -126,  -126,  -126,  -126,    42,   217,   244,   144,   129,
    -126,  -126,  -126,  -126,    84,  -126,  -126,  -126,  -126,  -126,
    -126,  -126,  -126,   240,  -126,  -126,  -126,  -126,  -126,  -126,
    -126,   156,  -126,  -126,    35,    19,    71,   209,  -126,   235,
      97,  -126,   157,   219,  -126,   235,   208,   237,   270,   294,
     161,  -126,   313,  -126,  -126,  -126,   259,  -126,   267,   290,
     277,    19,  -126,  -126,  -126,  -126,  -126,  -126,  -126,   102,
     102,   314,   155,   316,  -126,  -126,   155,  -126,  -126,   288,
     132,   116,   289,   281,  -126,    79,   282,  -126,  -126,  -126,
     321,  -126,  -126,   200,  -126,   102,   102,   102,   102,   293,
    -126,   332,   303,   304,   305,  -126,  -126,   116,   155,   238,
     132,   335,  -126,  -126,   300,  -126,   251,   251,  -126,  -126,
     235,    -2,   311,    19,   306,   162,  -126,   102,   312,   307,
      79,   172,    32,   341,  -126,  -126,  -126,  -126,   155,   315,
      79,  -126,  -126,    20,  -126,   317,  -126,  -126,  -126,   255,
    -126,   215,  -126,   342,  -126,   346,   319,  -126,   263,   309,
     128,   320,   322,  -126,   323,  -126,   140,  -126,  -126,   324,
    -126
  };

  const unsigned char
  VerilogParse::yydefact_[] =
  {
       3,     0,   128,     1,     4,     0,     0,     0,   129,     0,
     133,     0,   131,    34,   128,     0,   130,     0,     0,   116,
       0,     0,     0,     0,     9,    11,    14,     0,    21,    18,
      19,    20,     0,   137,   136,   135,   134,   132,     0,     5,
       0,     0,     0,     0,    35,    36,    41,    37,    38,    39,
      43,    40,     0,     0,    34,     0,     0,    16,     0,     0,
       0,   128,    26,    31,    28,     0,    42,     0,     0,    84,
       0,    87,   113,   114,   115,    88,     0,     0,     0,    49,
       0,     0,    64,     0,     0,     0,    77,    76,    78,    75,
      70,    72,    71,    74,    73,     0,     0,     0,     0,     0,
      15,    34,    10,    34,    81,    22,    23,    82,    27,    32,
      33,    30,    29,     0,    24,   119,   123,   124,   125,   126,
     127,     0,   121,    83,     0,     0,     0,     0,    47,     0,
       0,    63,     0,     0,    44,     0,     0,     0,     0,     0,
       0,    79,     0,   117,     6,    12,     0,    17,     0,     0,
       0,     0,   120,    85,    86,    56,    53,    52,    55,     0,
       0,     0,    51,     0,    50,    67,    66,    65,    45,     0,
      95,     0,     0,     0,    68,     0,     0,    13,     7,     8,
       0,   122,    57,     0,    54,     0,     0,     0,     0,     0,
      46,     0,     0,    96,    97,   100,    98,     0,    92,     0,
      95,     0,    80,   118,     0,    62,    59,    58,    60,    61,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,    89,    99,   101,    93,    94,     0,
       0,    25,    48,     0,   102,     0,   110,   111,   112,     0,
      90,     0,   103,     0,   105,     0,     0,    69,     0,     0,
       0,     0,     0,   106,     0,   104,     0,   107,   108,     0,
     109
  };

  const short
  VerilogParse::yypgoto_[] =
  {
    -126,  -126,  -126,  -126,  -126,   297,   261,  -126,   -19,  -126,
     292,  -126,   -33,  -126,  -126,  -126,  -126,  -126,   150,   -64,
    -125,  -126,  -126,   229,  -126,  -126,   133,   -54,  -126,  -126,
     -27,  -126,  -126,  -126,   165,   166,  -126,  -126,   153,  -126,
    -126,   -12,   -10,   -14,   143,   -58,  -126,   -61,    12,  -126,
    -126,   347,  -126
  };

  const unsigned char
  VerilogParse::yydefgoto_[] =
  {
       0,     1,     2,     4,    23,    24,    25,    56,    26,    27,
      28,    65,    18,    44,    45,    46,    84,    47,    78,    79,
     198,    48,    81,    82,    49,    95,   140,   141,    50,    68,
     107,    70,    51,   138,   199,   192,   193,   194,   195,   235,
      71,    72,    73,    74,   119,    75,   121,   196,    52,     8,
      11,    12,    36
  };

  const short
  VerilogParse::yytable_[] =
  {
      31,   162,    29,    57,    30,   166,   122,   106,    31,   120,
      29,   114,    30,    69,     5,   104,     3,    76,   136,   133,
      16,    97,     6,    19,   109,   115,    32,    99,    -2,    17,
     100,   222,   111,   110,   182,   183,   233,   223,   115,    19,
       7,   112,    13,    14,     9,    31,   104,    29,    67,    30,
     113,    10,    77,   118,   242,   116,    67,   117,   137,   243,
     206,   207,   208,   209,   154,   164,   234,   120,   148,    67,
     149,   169,    67,    32,   155,   156,   157,   158,    15,    67,
     147,   139,    53,   104,    31,    31,    29,    29,    30,    30,
     181,   123,   228,   120,   124,    19,   128,   153,   159,   129,
     155,   156,   165,   158,   160,   155,   156,    33,   158,    34,
      35,   118,   120,   116,   161,   117,    67,    54,    19,   155,
     156,   202,   158,    53,   159,    20,  -116,    21,    22,   159,
     160,    55,    19,    19,   115,   160,    19,   118,   115,   116,
     161,   117,   120,   159,    19,   161,   115,    66,    19,   160,
      21,    22,   225,   197,   131,   120,   118,   132,   116,   161,
     117,    80,   253,    83,   238,    67,   231,    96,   191,    67,
      62,    63,    64,   101,   258,    98,     7,    67,   145,    58,
      59,    22,   185,   186,   187,   188,   118,   127,   116,   254,
     117,   151,   120,   174,   152,   259,   175,   217,   120,   118,
     227,   116,    38,   117,   232,  -128,   103,   129,   236,    39,
      40,    41,    42,    43,    60,    61,  -128,  -128,  -128,  -128,
    -128,  -128,  -128,  -128,  -128,   108,  -128,   185,   186,   187,
     188,   134,   135,   125,   205,   126,   118,   130,   116,    76,
     117,   170,   118,   150,   116,    38,   117,   247,  -128,   163,
     175,   168,   144,    40,    41,    42,    43,   142,   143,  -128,
    -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,    38,  -128,
     171,  -128,   216,   217,   172,   178,    40,    41,    42,    43,
     187,   188,  -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,
    -128,    38,  -128,   177,  -128,   245,   246,   173,   179,    40,
      41,    42,    43,   142,   251,  -128,  -128,  -128,  -128,  -128,
    -128,  -128,  -128,  -128,    85,  -128,   176,   180,   184,   189,
     190,   201,   200,   203,   204,    86,    87,    88,    89,    90,
      91,    92,    93,    94,   210,     7,   211,   212,   219,   213,
     214,   220,   191,   224,   239,   248,   229,   240,   230,   249,
     252,   244,   250,   105,   255,   256,   102,   257,   260,   146,
     221,   167,   215,   241,    37,   237,   218,   226
  };

  const short
  VerilogParse::yycheck_[] =
  {
      14,   126,    14,    22,    14,   130,    67,    61,    22,    67,
      22,    65,    22,    40,     2,     4,     0,     4,     4,    83,
      26,    54,     7,     4,    15,     6,    14,    35,     0,    35,
      38,    33,    15,    24,   159,   160,     4,    39,     6,     4,
      25,    24,    32,    33,     4,    59,     4,    59,    37,    59,
      39,     4,    39,    67,    34,    67,    37,    67,    44,    39,
     185,   186,   187,   188,   125,   129,    34,   125,   101,    37,
     103,   135,    37,    61,     3,     4,     5,     6,    42,    37,
      99,    39,    39,     4,    98,    99,    98,    99,    98,    99,
     151,    32,   217,   151,    35,     4,    32,   124,    27,    35,
       3,     4,     5,     6,    33,     3,     4,     3,     6,     5,
       6,   125,   170,   125,    43,   125,    37,    32,     4,     3,
       4,   175,     6,    39,    27,    34,    42,    36,    37,    27,
      33,     4,     4,     4,     6,    33,     4,   151,     6,   151,
      43,   151,   200,    27,     4,    43,     6,    32,     4,    33,
      36,    37,   213,    37,    32,   213,   170,    35,   170,    43,
     170,     4,    34,    14,   222,    37,   220,     3,    36,    37,
      19,    20,    21,    32,    34,    33,    25,    37,    34,    34,
      35,    37,    27,    28,    29,    30,   200,     3,   200,   250,
     200,    35,   250,    32,    38,   256,    35,    35,   256,   213,
      38,   213,     1,   213,    32,     4,    32,    35,   222,     8,
       9,    10,    11,    12,    34,    35,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    15,    25,    27,    28,    29,
      30,    13,    14,    42,    34,    42,   250,    42,   250,     4,
     250,    33,   256,     3,   256,     1,   256,    32,     4,    40,
      35,    32,     8,     9,    10,    11,    12,    40,    41,    15,
      16,    17,    18,    19,    20,    21,    22,    23,     1,    25,
      33,     4,    34,    35,     4,     8,     9,    10,    11,    12,
      29,    30,    15,    16,    17,    18,    19,    20,    21,    22,
      23,     1,    25,    34,     4,    40,    41,     3,     8,     9,
      10,    11,    12,    40,    41,    15,    16,    17,    18,    19,
      20,    21,    22,    23,     4,    25,     3,    40,     4,     3,
      32,    40,    33,    41,     3,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    41,    25,     4,    34,     3,    35,
      35,    41,    36,    32,     3,     3,    34,    32,    41,     3,
      41,    34,    33,    61,    34,    33,    59,    34,    34,    98,
     210,   132,   197,   230,    17,   222,   200,   214
  };

  const signed char
  VerilogParse::yystos_[] =
  {
       0,    46,    47,     0,    48,    93,     7,    25,    94,     4,
       4,    95,    96,    32,    33,    42,    26,    35,    57,     4,
      34,    36,    37,    49,    50,    51,    53,    54,    55,    86,
      87,    88,    93,     3,     5,     6,    97,    96,     1,     8,
       9,    10,    11,    12,    58,    59,    60,    62,    66,    69,
      73,    77,    93,    39,    32,     4,    52,    53,    34,    35,
      34,    35,    19,    20,    21,    56,    32,    37,    74,    75,
      76,    85,    86,    87,    88,    90,     4,    39,    63,    64,
       4,    67,    68,    14,    61,     4,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    70,     3,    57,    33,    35,
      38,    32,    50,    32,     4,    55,    72,    75,    15,    15,
      24,    15,    24,    39,    72,     6,    86,    87,    88,    89,
      90,    91,    92,    32,    35,    42,    42,     3,    32,    35,
      42,    32,    35,    64,    13,    14,     4,    44,    78,    39,
      71,    72,    40,    41,     8,    34,    51,    53,    57,    57,
       3,    35,    38,    75,    92,     3,     4,     5,     6,    27,
      33,    43,    65,    40,    64,     5,    65,    68,    32,    64,
      33,    33,     4,     3,    32,    35,     3,    34,     8,     8,
      40,    92,    65,    65,     4,    27,    28,    29,    30,     3,
      32,    36,    80,    81,    82,    83,    92,    37,    65,    79,
      33,    40,    72,    41,     3,    34,    65,    65,    65,    65,
      41,     4,    34,    35,    35,    79,    34,    35,    80,     3,
      41,    63,    33,    39,    32,    92,    83,    38,    65,    34,
      41,    72,    32,     4,    34,    84,    88,    89,    90,     3,
      32,    71,    34,    39,    34,    40,    41,    32,     3,     3,
      33,    41,    41,    34,    92,    34,    33,    34,    34,    92,
      34
  };

  const signed char
  VerilogParse::yyr1_[] =
  {
       0,    45,    46,    47,    47,    48,    48,    48,    48,    49,
      49,    50,    50,    50,    51,    51,    52,    52,    53,    53,
      53,    54,    54,    54,    55,    55,    56,    56,    56,    56,
      56,    56,    56,    56,    57,    57,    57,    58,    58,    58,
      58,    58,    58,    59,    60,    61,    61,    62,    62,    63,
      63,    64,    64,    65,    65,    65,    65,    65,    65,    65,
      65,    65,    65,    66,    67,    67,    68,    68,    69,    69,
      70,    70,    70,    70,    70,    70,    70,    70,    70,    71,
      71,    72,    72,    73,    74,    74,    75,    76,    76,    77,
      77,    78,    79,    79,    79,    80,    80,    80,    81,    81,
      82,    82,    83,    83,    83,    83,    83,    83,    83,    83,
      84,    84,    84,    85,    85,    85,    86,    87,    88,    89,
      90,    91,    91,    92,    92,    92,    92,    92,    93,    93,
      94,    95,    95,    96,    96,    97,    97,    97
  };

  const signed char
  VerilogParse::yyr2_[] =
  {
       0,     2,     1,     0,     2,     6,     8,     9,     9,     1,
       3,     1,     4,     5,     1,     3,     1,     3,     1,     1,
       1,     1,     3,     3,     3,     8,     1,     2,     1,     2,
       2,     1,     2,     2,     0,     2,     2,     1,     1,     1,
       1,     1,     2,     1,     3,     3,     4,     3,     8,     1,
       3,     3,     3,     1,     2,     1,     1,     2,     3,     3,
       3,     3,     3,     3,     1,     3,     3,     3,     4,     9,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     1,     3,     1,     3,     3,     1,     1,     7,
       8,     4,     1,     3,     3,     0,     1,     1,     1,     3,
       1,     3,     4,     5,     8,     5,     7,     8,     9,    10,
       1,     1,     1,     1,     1,     1,     1,     4,     6,     1,
       3,     1,     3,     1,     1,     1,     1,     1,     0,     2,
       3,     1,     3,     1,     3,     1,     1,     1
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const VerilogParse::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "INT", "ID", "STRING",
  "CONSTANT", "MODULE", "ENDMODULE", "ASSIGN", "PARAMETER", "DEFPARAM",
  "SPECIFY", "ENDSPECIFY", "SPECPARAM", "WIRE", "WAND", "WOR", "TRI",
  "INPUT", "OUTPUT", "INOUT", "SUPPLY1", "SUPPLY0", "REG", "ATTR_OPEN",
  "ATTR_CLOSED", "'-'", "'+'", "'*'", "'/'", "NEG", "';'", "'('", "')'",
  "','", "'.'", "'{'", "'}'", "'['", "':'", "']'", "'='", "'`'", "'#'",
  "$accept", "file", "modules", "module", "port_list", "port", "port_expr",
  "port_refs", "port_ref", "port_dcls", "port_dcl", "port_dcl_type",
  "stmts", "stmt", "stmt_seq", "specify_block", "specify_stmts",
  "parameter", "parameter_dcls", "parameter_dcl", "parameter_expr",
  "defparam", "param_values", "param_value", "declaration", "dcl_type",
  "dcl_args", "dcl_arg", "continuous_assign", "net_assignments",
  "net_assignment", "net_assign_lhs", "instance", "parameter_values",
  "parameter_exprs", "inst_pins", "inst_ordered_pins", "inst_named_pins",
  "inst_named_pin", "named_pin_net_expr", "net_named", "net_scalar",
  "net_bit_select", "net_part_select", "net_constant", "net_expr_concat",
  "net_exprs", "net_expr", "attr_instance_seq", "attr_instance",
  "attr_specs", "attr_spec", "attr_spec_value", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  VerilogParse::yyrline_[] =
  {
       0,   126,   126,   129,   131,   135,   137,   139,   141,   146,
     150,   155,   156,   158,   163,   164,   168,   172,   177,   178,
     179,   183,   187,   191,   200,   202,   207,   208,   209,   210,
     211,   212,   213,   214,   219,   220,   222,   232,   233,   234,
     235,   236,   237,   242,   248,   253,   255,   261,   263,   268,
     270,   275,   277,   282,   284,   286,   288,   289,   291,   293,
     295,   297,   299,   304,   309,   311,   316,   318,   323,   325,
     330,   331,   332,   333,   334,   335,   336,   337,   338,   342,
     346,   353,   355,   360,   365,   369,   374,   379,   380,   384,
     386,   391,   395,   396,   398,   403,   404,   405,   410,   414,
     420,   424,   432,   434,   436,   438,   441,   443,   446,   448,
     453,   454,   455,   459,   460,   461,   465,   470,   475,   480,
     485,   490,   494,   499,   500,   501,   502,   503,   508,   509,
     514,   519,   523,   528,   530,   535,   537,   539
  };

  void
  VerilogParse::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  VerilogParse::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  VerilogParse::symbol_kind_type
  VerilogParse::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    44,     2,     2,     2,     2,
      33,    34,    29,    28,    35,    27,    36,    30,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    40,    32,
       2,    42,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    39,     2,    41,     2,     2,    43,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    37,     2,    38,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    31
    };
    // Last valid token kind.
    const int code_max = 282;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 80 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"
} // sta
#line 2699 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/VerilogParse.cc"

#line 543 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/verilog/VerilogParse.yy"

