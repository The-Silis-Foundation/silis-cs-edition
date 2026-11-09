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
#line 25 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"

#include <string>
#include <utility>

#include "Report.hh"
#include "StringUtil.hh"
#include "parasitics/SpefReaderPvt.hh"
#include "parasitics/SpefScanner.hh"

#undef yylex
#define yylex scanner->lex

// warning: variable 'yynerrs_' set but not used
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

void
sta::SpefParse::error(const location_type &loc,
                     const std::string &msg)
{
  reader->report()->fileError(1670, reader->filename(), loc.begin.line, "{}", msg);
}

#line 64 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"


#include "SpefParse.hh"




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

#line 68 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
namespace sta {
#line 162 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"

  /// Build a parser object.
  SpefParse::SpefParse (SpefScanner *scanner_yyarg, SpefReader *reader_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      scanner (scanner_yyarg),
      reader (reader_yyarg)
  {}

  SpefParse::~SpefParse ()
  {}

  SpefParse::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  SpefParse::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S_prefix_bus_delim: // prefix_bus_delim
      case symbol_kind::S_suffix_bus_delim: // suffix_bus_delim
      case symbol_kind::S_hchar: // hchar
        value.copy< char > (YY_MOVE (that.value));
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
        value.copy< float > (YY_MOVE (that.value));
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
        value.copy< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_net: // net
        value.copy< sta::Net * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_internal_connection: // internal_connection
      case symbol_kind::S_pin_name: // pin_name
      case symbol_kind::S_driver_pair: // driver_pair
        value.copy< sta::Pin * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_direction: // direction
        value.copy< sta::PortDirection * > (YY_MOVE (that.value));
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
        value.copy< sta::SpefParseVoid > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pi_model: // pi_model
        value.copy< sta::SpefRspfPi * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_par_value: // par_value
      case symbol_kind::S_total_cap: // total_cap
        value.copy< sta::SpefTriple * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qstrings: // qstrings
        value.copy< sta::StringSeq * > (YY_MOVE (that.value));
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
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }




  template <typename Base>
  SpefParse::symbol_kind_type
  SpefParse::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  SpefParse::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  SpefParse::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_prefix_bus_delim: // prefix_bus_delim
      case symbol_kind::S_suffix_bus_delim: // suffix_bus_delim
      case symbol_kind::S_hchar: // hchar
        value.move< char > (YY_MOVE (s.value));
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
        value.move< float > (YY_MOVE (s.value));
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
        value.move< int > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_net: // net
        value.move< sta::Net * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_internal_connection: // internal_connection
      case symbol_kind::S_pin_name: // pin_name
      case symbol_kind::S_driver_pair: // driver_pair
        value.move< sta::Pin * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_direction: // direction
        value.move< sta::PortDirection * > (YY_MOVE (s.value));
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
        value.move< sta::SpefParseVoid > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_pi_model: // pi_model
        value.move< sta::SpefRspfPi * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_par_value: // par_value
      case symbol_kind::S_total_cap: // total_cap
        value.move< sta::SpefTriple * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_qstrings: // qstrings
        value.move< sta::StringSeq * > (YY_MOVE (s.value));
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
        value.move< std::string > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  SpefParse::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  SpefParse::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  SpefParse::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  SpefParse::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  SpefParse::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  SpefParse::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  SpefParse::symbol_kind_type
  SpefParse::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  SpefParse::symbol_kind_type
  SpefParse::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  SpefParse::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  SpefParse::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  SpefParse::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  SpefParse::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  SpefParse::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  SpefParse::symbol_kind_type
  SpefParse::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  SpefParse::stack_symbol_type::stack_symbol_type ()
  {}

  SpefParse::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_prefix_bus_delim: // prefix_bus_delim
      case symbol_kind::S_suffix_bus_delim: // suffix_bus_delim
      case symbol_kind::S_hchar: // hchar
        value.YY_MOVE_OR_COPY< char > (YY_MOVE (that.value));
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
        value.YY_MOVE_OR_COPY< float > (YY_MOVE (that.value));
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
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_net: // net
        value.YY_MOVE_OR_COPY< sta::Net * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_internal_connection: // internal_connection
      case symbol_kind::S_pin_name: // pin_name
      case symbol_kind::S_driver_pair: // driver_pair
        value.YY_MOVE_OR_COPY< sta::Pin * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_direction: // direction
        value.YY_MOVE_OR_COPY< sta::PortDirection * > (YY_MOVE (that.value));
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
        value.YY_MOVE_OR_COPY< sta::SpefParseVoid > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pi_model: // pi_model
        value.YY_MOVE_OR_COPY< sta::SpefRspfPi * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_par_value: // par_value
      case symbol_kind::S_total_cap: // total_cap
        value.YY_MOVE_OR_COPY< sta::SpefTriple * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qstrings: // qstrings
        value.YY_MOVE_OR_COPY< sta::StringSeq * > (YY_MOVE (that.value));
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

  SpefParse::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_prefix_bus_delim: // prefix_bus_delim
      case symbol_kind::S_suffix_bus_delim: // suffix_bus_delim
      case symbol_kind::S_hchar: // hchar
        value.move< char > (YY_MOVE (that.value));
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
        value.move< float > (YY_MOVE (that.value));
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
        value.move< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_net: // net
        value.move< sta::Net * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_internal_connection: // internal_connection
      case symbol_kind::S_pin_name: // pin_name
      case symbol_kind::S_driver_pair: // driver_pair
        value.move< sta::Pin * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_direction: // direction
        value.move< sta::PortDirection * > (YY_MOVE (that.value));
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
        value.move< sta::SpefParseVoid > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pi_model: // pi_model
        value.move< sta::SpefRspfPi * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_par_value: // par_value
      case symbol_kind::S_total_cap: // total_cap
        value.move< sta::SpefTriple * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_qstrings: // qstrings
        value.move< sta::StringSeq * > (YY_MOVE (that.value));
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
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  SpefParse::stack_symbol_type&
  SpefParse::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_prefix_bus_delim: // prefix_bus_delim
      case symbol_kind::S_suffix_bus_delim: // suffix_bus_delim
      case symbol_kind::S_hchar: // hchar
        value.copy< char > (that.value);
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
        value.copy< float > (that.value);
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
        value.copy< int > (that.value);
        break;

      case symbol_kind::S_net: // net
        value.copy< sta::Net * > (that.value);
        break;

      case symbol_kind::S_internal_connection: // internal_connection
      case symbol_kind::S_pin_name: // pin_name
      case symbol_kind::S_driver_pair: // driver_pair
        value.copy< sta::Pin * > (that.value);
        break;

      case symbol_kind::S_direction: // direction
        value.copy< sta::PortDirection * > (that.value);
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
        value.copy< sta::SpefParseVoid > (that.value);
        break;

      case symbol_kind::S_pi_model: // pi_model
        value.copy< sta::SpefRspfPi * > (that.value);
        break;

      case symbol_kind::S_par_value: // par_value
      case symbol_kind::S_total_cap: // total_cap
        value.copy< sta::SpefTriple * > (that.value);
        break;

      case symbol_kind::S_qstrings: // qstrings
        value.copy< sta::StringSeq * > (that.value);
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
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  SpefParse::stack_symbol_type&
  SpefParse::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_prefix_bus_delim: // prefix_bus_delim
      case symbol_kind::S_suffix_bus_delim: // suffix_bus_delim
      case symbol_kind::S_hchar: // hchar
        value.move< char > (that.value);
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
        value.move< float > (that.value);
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
        value.move< int > (that.value);
        break;

      case symbol_kind::S_net: // net
        value.move< sta::Net * > (that.value);
        break;

      case symbol_kind::S_internal_connection: // internal_connection
      case symbol_kind::S_pin_name: // pin_name
      case symbol_kind::S_driver_pair: // driver_pair
        value.move< sta::Pin * > (that.value);
        break;

      case symbol_kind::S_direction: // direction
        value.move< sta::PortDirection * > (that.value);
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
        value.move< sta::SpefParseVoid > (that.value);
        break;

      case symbol_kind::S_pi_model: // pi_model
        value.move< sta::SpefRspfPi * > (that.value);
        break;

      case symbol_kind::S_par_value: // par_value
      case symbol_kind::S_total_cap: // total_cap
        value.move< sta::SpefTriple * > (that.value);
        break;

      case symbol_kind::S_qstrings: // qstrings
        value.move< sta::StringSeq * > (that.value);
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
  SpefParse::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  SpefParse::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
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
  SpefParse::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  SpefParse::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  SpefParse::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  SpefParse::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  SpefParse::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  SpefParse::debug_level_type
  SpefParse::debug_level () const
  {
    return yydebug_;
  }

  void
  SpefParse::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  SpefParse::state_type
  SpefParse::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  SpefParse::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  SpefParse::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  SpefParse::operator() ()
  {
    return parse ();
  }

  int
  SpefParse::parse ()
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
      case symbol_kind::S_prefix_bus_delim: // prefix_bus_delim
      case symbol_kind::S_suffix_bus_delim: // suffix_bus_delim
      case symbol_kind::S_hchar: // hchar
        yylhs.value.emplace< char > ();
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
        yylhs.value.emplace< float > ();
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
        yylhs.value.emplace< int > ();
        break;

      case symbol_kind::S_net: // net
        yylhs.value.emplace< sta::Net * > ();
        break;

      case symbol_kind::S_internal_connection: // internal_connection
      case symbol_kind::S_pin_name: // pin_name
      case symbol_kind::S_driver_pair: // driver_pair
        yylhs.value.emplace< sta::Pin * > ();
        break;

      case symbol_kind::S_direction: // direction
        yylhs.value.emplace< sta::PortDirection * > ();
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
        yylhs.value.emplace< sta::SpefParseVoid > ();
        break;

      case symbol_kind::S_pi_model: // pi_model
        yylhs.value.emplace< sta::SpefRspfPi * > ();
        break;

      case symbol_kind::S_par_value: // par_value
      case symbol_kind::S_total_cap: // total_cap
        yylhs.value.emplace< sta::SpefTriple * > ();
        break;

      case symbol_kind::S_qstrings: // qstrings
        yylhs.value.emplace< sta::StringSeq * > ();
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
  case 3: // prefix_bus_delim: '['
#line 141 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < char > () = '['; }
#line 1331 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 4: // prefix_bus_delim: '{'
#line 143 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < char > () = '}'; }
#line 1337 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 5: // prefix_bus_delim: '('
#line 145 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < char > () = ')'; }
#line 1343 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 6: // prefix_bus_delim: '<'
#line 147 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < char > () = '<'; }
#line 1349 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 7: // suffix_bus_delim: ']'
#line 152 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < char > () = ']'; }
#line 1355 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 8: // suffix_bus_delim: '}'
#line 154 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < char > () = '}'; }
#line 1361 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 9: // suffix_bus_delim: ')'
#line 156 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < char > () = ')'; }
#line 1367 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 10: // suffix_bus_delim: '>'
#line 158 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < char > () = '>'; }
#line 1373 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 11: // hchar: '.'
#line 163 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < char > () = '.'; }
#line 1379 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 12: // hchar: '/'
#line 165 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < char > () = '/'; }
#line 1385 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 13: // hchar: '|'
#line 167 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < char > () = '|'; }
#line 1391 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 14: // hchar: ':'
#line 169 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < char > () = ':'; }
#line 1397 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 22: // design_flow: DESIGN_FLOW qstrings
#line 214 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { reader->setDesignFlow(yystack_[0].value.as < sta::StringSeq * > ()); }
#line 1403 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 23: // qstrings: QSTRING
#line 219 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < sta::StringSeq * > () = new sta::StringSeq;
	  yylhs.value.as < sta::StringSeq * > ()->push_back(std::move(yystack_[0].value.as < std::string > ()));
	}
#line 1411 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 24: // qstrings: qstrings QSTRING
#line 223 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < sta::StringSeq * > () = yystack_[1].value.as < sta::StringSeq * > ();
	  yylhs.value.as < sta::StringSeq * > ()->push_back(std::move(yystack_[0].value.as < std::string > ()));
	}
#line 1419 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 25: // hierarchy_div_def: DIVIDER hchar
#line 230 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { reader->setDivider(yystack_[0].value.as < char > ()); }
#line 1425 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 26: // pin_delim_def: DELIMITER hchar
#line 235 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { reader->setDelimiter(yystack_[0].value.as < char > ()); }
#line 1431 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 27: // bus_delim_def: BUS_DELIMITER prefix_bus_delim
#line 240 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { reader->setBusBrackets(yystack_[0].value.as < char > (), '\0'); }
#line 1437 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 28: // bus_delim_def: BUS_DELIMITER prefix_bus_delim suffix_bus_delim
#line 242 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { reader->setBusBrackets(yystack_[1].value.as < char > (), yystack_[0].value.as < char > ()); }
#line 1443 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 30: // time_scale: T_UNIT pos_number IDENT
#line 256 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { reader->setTimeScale(yystack_[1].value.as < float > (), yystack_[0].value.as < std::string > ()); }
#line 1449 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 31: // cap_scale: C_UNIT pos_number IDENT
#line 261 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { reader->setCapScale(yystack_[1].value.as < float > (), yystack_[0].value.as < std::string > ()); }
#line 1455 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 32: // res_scale: R_UNIT pos_number IDENT
#line 266 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { reader->setResScale(yystack_[1].value.as < float > (), yystack_[0].value.as < std::string > ()); }
#line 1461 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 33: // induc_scale: L_UNIT pos_number IDENT
#line 271 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { reader->setInductScale(yystack_[1].value.as < float > (), yystack_[0].value.as < std::string > ()); }
#line 1467 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 36: // name_map_entries: name_map_entry
#line 282 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < sta::SpefParseVoid > () = yystack_[0].value.as < sta::SpefParseVoid > (); }
#line 1473 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 37: // name_map_entries: name_map_entries name_map_entry
#line 283 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < sta::SpefParseVoid > () = yystack_[1].value.as < sta::SpefParseVoid > (); }
#line 1479 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 38: // name_map_entry: INDEX mapped_item
#line 288 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { reader->makeNameMapEntry(yystack_[1].value.as < std::string > (), yystack_[0].value.as < std::string > ()); }
#line 1485 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 39: // mapped_item: IDENT
#line 292 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1491 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 40: // mapped_item: NAME
#line 293 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1497 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 41: // mapped_item: QSTRING
#line 294 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1503 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 48: // net_names: net_name
#line 315 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < sta::SpefParseVoid > () = yystack_[0].value.as < sta::SpefParseVoid > (); }
#line 1509 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 49: // net_names: net_names net_name
#line 316 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < sta::SpefParseVoid > () = yystack_[1].value.as < sta::SpefParseVoid > (); }
#line 1515 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 50: // net_name: name_or_index
#line 321 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < sta::SpefParseVoid > () = sta::SpefParseVoid{}; }
#line 1521 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 56: // port_entries: port_entry
#line 338 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < sta::SpefParseVoid > () = yystack_[0].value.as < sta::SpefParseVoid > (); }
#line 1527 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 57: // port_entries: port_entries port_entry
#line 339 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < sta::SpefParseVoid > () = yystack_[1].value.as < sta::SpefParseVoid > (); }
#line 1533 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 58: // port_entry: port_name direction conn_attrs
#line 344 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < sta::SpefParseVoid > () = sta::SpefParseVoid{}; }
#line 1539 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 59: // direction: IDENT
#line 349 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < sta::PortDirection * > () = reader->portDirection(yystack_[0].value.as < std::string > ()); }
#line 1545 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 60: // port_name: name_or_index
#line 353 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1551 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 61: // inst_name: name_or_index
#line 357 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1557 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 63: // pport_entries: pport_entry
#line 365 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < sta::SpefParseVoid > () = yystack_[0].value.as < sta::SpefParseVoid > (); }
#line 1563 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 64: // pport_entries: pport_entries pport_entry
#line 366 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < sta::SpefParseVoid > () = yystack_[1].value.as < sta::SpefParseVoid > (); }
#line 1569 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 65: // pport_entry: pport_name IDENT conn_attrs
#line 371 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < sta::SpefParseVoid > () = sta::SpefParseVoid{}; }
#line 1575 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 66: // pport_name: name_or_index
#line 376 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < sta::SpefParseVoid > () = sta::SpefParseVoid{}; }
#line 1581 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 67: // pport_name: physical_inst ':' pport
#line 378 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < sta::SpefParseVoid > () = sta::SpefParseVoid{}; }
#line 1587 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 68: // pport: name_or_index
#line 382 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1593 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 69: // physical_inst: name_or_index
#line 386 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1599 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 77: // cap_load: KW_L par_value
#line 409 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { delete yystack_[0].value.as < sta::SpefTriple * > (); }
#line 1605 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 78: // par_value: number
#line 414 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < sta::SpefTriple * > () = new sta::SpefTriple(yystack_[0].value.as < float > ()); }
#line 1611 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 79: // par_value: number ':' number ':' number
#line 416 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < sta::SpefTriple * > () = new sta::SpefTriple(yystack_[4].value.as < float > (), yystack_[2].value.as < float > (), yystack_[0].value.as < float > ()); }
#line 1617 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 80: // slews: KW_S par_value par_value
#line 421 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { delete yystack_[1].value.as < sta::SpefTriple * > ();
	  delete yystack_[0].value.as < sta::SpefTriple * > ();
	}
#line 1625 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 81: // slews: KW_S par_value par_value threshold threshold
#line 425 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { delete yystack_[3].value.as < sta::SpefTriple * > ();
	  delete yystack_[2].value.as < sta::SpefTriple * > ();
	}
#line 1633 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 82: // threshold: pos_number
#line 431 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < float > () = yystack_[0].value.as < float > (); }
#line 1639 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 83: // threshold: pos_number ':' pos_number ':' pos_number
#line 432 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < float > () = yystack_[4].value.as < float > (); }
#line 1645 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 85: // cell_type: IDENT
#line 440 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1651 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 86: // cell_type: INDEX
#line 441 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1657 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 92: // entity: QSTRING
#line 458 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1663 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 99: // $@1: %empty
#line 479 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { reader->dspfBegin(yystack_[1].value.as < sta::Net * > (), yystack_[0].value.as < sta::SpefTriple * > ()); }
#line 1669 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 100: // d_net: D_NET net total_cap $@1 routing_conf conn_sec cap_sec res_sec induc_sec END
#line 481 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { reader->dspfFinish(); }
#line 1675 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 101: // net: name_or_index
#line 486 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < sta::Net * > () = reader->findNet(yystack_[0].value.as < std::string > ()); }
#line 1681 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 102: // total_cap: par_value
#line 490 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < sta::SpefTriple * > () = yystack_[0].value.as < sta::SpefTriple * > (); }
#line 1687 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 105: // conf: pos_integer
#line 499 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < int > () = yystack_[0].value.as < int > (); }
#line 1693 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 112: // external_connection: name_or_index
#line 521 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < std::string > () = std::string(); }
#line 1699 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 113: // external_connection: physical_inst ':' pport
#line 523 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < std::string > () = std::string(); }
#line 1705 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 114: // internal_connection: pin_name
#line 527 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < sta::Pin * > () = yystack_[0].value.as < sta::Pin * > (); }
#line 1711 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 115: // pin_name: name_or_index
#line 532 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < sta::Pin * > () = reader->findPin(yystack_[0].value.as < std::string > ()); }
#line 1717 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 119: // internal_parasitic_node: name_or_index
#line 546 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < std::string > () = std::string(); }
#line 1723 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 122: // cap_elems: %empty
#line 558 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < int > () = 0; }
#line 1729 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 123: // cap_elems: cap_elems cap_elem
#line 560 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < int > () = yystack_[1].value.as < int > (); }
#line 1735 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 124: // cap_elem: cap_id parasitic_node par_value
#line 565 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { reader->makeCapacitor(yystack_[2].value.as < int > (), yystack_[1].value.as < std::string > (), yystack_[0].value.as < sta::SpefTriple * > ()); }
#line 1741 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 125: // cap_elem: cap_id parasitic_node parasitic_node par_value
#line 567 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { reader->makeCapacitor(yystack_[3].value.as < int > (), yystack_[2].value.as < std::string > (), yystack_[1].value.as < std::string > (), yystack_[0].value.as < sta::SpefTriple * > ()); }
#line 1747 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 126: // cap_id: pos_integer
#line 571 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < int > () = yystack_[0].value.as < int > (); }
#line 1753 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 127: // parasitic_node: name_or_index
#line 575 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1759 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 130: // res_elems: %empty
#line 587 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < int > () = 0; }
#line 1765 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 131: // res_elems: res_elems res_elem
#line 589 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < int > () = yystack_[1].value.as < int > (); }
#line 1771 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 132: // res_elem: res_id parasitic_node parasitic_node par_value
#line 594 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { reader->makeResistor(yystack_[3].value.as < int > (), yystack_[2].value.as < std::string > (), yystack_[1].value.as < std::string > (), yystack_[0].value.as < sta::SpefTriple * > ()); }
#line 1777 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 133: // res_id: pos_integer
#line 598 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < int > () = yystack_[0].value.as < int > (); }
#line 1783 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 136: // induc_elems: %empty
#line 610 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < int > () = 0; }
#line 1789 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 137: // induc_elems: induc_elems induc_elem
#line 612 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < int > () = yystack_[1].value.as < int > (); }
#line 1795 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 138: // induc_elem: induc_id parasitic_node parasitic_node par_value
#line 617 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { delete yystack_[0].value.as < sta::SpefTriple * > (); }
#line 1801 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 139: // induc_id: pos_integer
#line 621 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < int > () = yystack_[0].value.as < int > (); }
#line 1807 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 140: // $@2: %empty
#line 628 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { reader->rspfBegin(yystack_[1].value.as < sta::Net * > (), yystack_[0].value.as < sta::SpefTriple * > ()); }
#line 1813 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 141: // r_net: R_NET net total_cap $@2 routing_conf driver_reducs END
#line 630 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { reader->rspfFinish(); }
#line 1819 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 144: // $@3: %empty
#line 640 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { reader->rspfDrvrBegin(yystack_[2].value.as < sta::Pin * > (), yystack_[0].value.as < sta::SpefRspfPi * > ()); }
#line 1825 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 145: // driver_reduc: driver_pair driver_cell pi_model $@3 load_desc
#line 642 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { reader->rspfDrvrFinish(); }
#line 1831 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 146: // driver_pair: DRIVER pin_name
#line 647 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < sta::Pin * > () = yystack_[0].value.as < sta::Pin * > (); }
#line 1837 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 147: // driver_cell: CELL cell_type
#line 652 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1843 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 148: // pi_model: C2_R1_C1 par_value par_value par_value
#line 657 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < sta::SpefRspfPi * > () = new sta::SpefRspfPi(yystack_[2].value.as < sta::SpefTriple * > (), yystack_[1].value.as < sta::SpefTriple * > (), yystack_[0].value.as < sta::SpefTriple * > ()); }
#line 1849 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 152: // rc_desc: RC pin_name par_value
#line 673 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { reader->rspfLoad(yystack_[1].value.as < sta::Pin * > (), yystack_[0].value.as < sta::SpefTriple * > ()); }
#line 1855 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 153: // rc_desc: RC pin_name par_value pole_residue_desc
#line 675 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { reader->rspfLoad(yystack_[2].value.as < sta::Pin * > (), yystack_[1].value.as < sta::SpefTriple * > ()); }
#line 1861 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 156: // poles: pole
#line 687 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < float > () = yystack_[0].value.as < float > (); }
#line 1867 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 157: // poles: poles pole
#line 689 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < float > () = yystack_[0].value.as < float > (); }
#line 1873 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 158: // pole: complex_par_value
#line 693 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < float > () = yystack_[0].value.as < float > (); }
#line 1879 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 159: // complex_par_value: cnumber
#line 697 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < float > () = yystack_[0].value.as < float > (); }
#line 1885 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 160: // complex_par_value: number
#line 698 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < float > () = yystack_[0].value.as < float > (); }
#line 1891 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 161: // complex_par_value: cnumber ':' cnumber ':' cnumber
#line 699 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < float > () = yystack_[4].value.as < float > (); }
#line 1897 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 162: // complex_par_value: number ':' number ':' number
#line 700 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < float > () = yystack_[4].value.as < float > (); }
#line 1903 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 163: // cnumber: '(' real_component imaginary_component ')'
#line 705 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < float > () = yystack_[2].value.as < float > (); }
#line 1909 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 164: // real_component: number
#line 709 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < float > () = yystack_[0].value.as < float > (); }
#line 1915 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 165: // imaginary_component: number
#line 713 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < float > () = yystack_[0].value.as < float > (); }
#line 1921 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 167: // residues: residue
#line 721 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < float > () = yystack_[0].value.as < float > (); }
#line 1927 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 168: // residues: residues residue
#line 722 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < float > () = yystack_[1].value.as < float > (); }
#line 1933 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 169: // residue: complex_par_value
#line 726 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < float > () = yystack_[0].value.as < float > (); }
#line 1939 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 171: // pnet_ref: name_or_index
#line 737 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1945 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 177: // pexternal_connection: pport_name
#line 755 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < sta::SpefParseVoid > () = yystack_[0].value.as < sta::SpefParseVoid > (); }
#line 1951 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 181: // internal_pdspf_node: name_or_index
#line 769 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < std::string > () = std::string(); }
#line 1957 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 182: // name_or_index: IDENT
#line 773 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1963 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 183: // name_or_index: NAME
#line 774 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1969 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 184: // name_or_index: INDEX
#line 775 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1975 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 189: // number: INTEGER
#line 797 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < float > () = static_cast<float>(yystack_[0].value.as < int > ()); }
#line 1981 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 190: // number: FLOAT
#line 799 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { yylhs.value.as < float > () = yystack_[0].value.as < float > (); }
#line 1987 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 191: // pos_integer: INTEGER
#line 804 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { int value = yystack_[0].value.as < int > ();
	  if (value < 0)
	    reader->warn(1525, "{} is not positive.", value);
	  yylhs.value.as < int > () = value;
	}
#line 1997 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 192: // pos_number: INTEGER
#line 813 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { float value = static_cast<float>(yystack_[0].value.as < int > ());
	  if (value < 0)
	    reader->warn(1526, "{:.4f} is not positive.", value);
	  yylhs.value.as < float > () = value;
	}
#line 2007 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;

  case 193: // pos_number: FLOAT
#line 819 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
        { float value = static_cast<float>(yystack_[0].value.as < float > ());
	  if (value < 0)
	    reader->warn(1527, "{:.4f} is not positive.", value);
	  yylhs.value.as < float > () = value;
	}
#line 2017 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"
    break;


#line 2021 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"

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
  SpefParse::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

#if YYDEBUG || 0
  const char *
  SpefParse::symbol_name (symbol_kind_type yysymbol)
  {
    return yytname_[yysymbol];
  }
#endif // #if YYDEBUG || 0









  const short SpefParse::yypact_ninf_ = -306;

  const signed char SpefParse::yytable_ninf_ = -113;

  const short
  SpefParse::yypact_[] =
  {
      47,    56,    67,    40,    76,  -306,  -306,    92,    35,   117,
     118,   116,    92,  -306,    36,    36,    83,   114,  -306,  -306,
     135,   130,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,
      36,  -306,  -306,    36,    36,    36,  -306,   111,  -306,  -306,
    -306,   139,   132,  -306,    36,  -306,   140,  -306,    36,  -306,
     141,    84,    85,    45,  -306,  -306,   146,   137,  -306,  -306,
    -306,  -306,  -306,    36,    36,    36,    36,    36,    36,    36,
    -306,    55,  -306,  -306,  -306,  -306,  -306,  -306,   148,   142,
      68,    68,  -306,  -306,    95,  -306,   152,  -306,   122,  -306,
     122,  -306,   122,   122,  -306,  -306,   153,   143,   122,   122,
     122,   121,  -306,  -306,  -306,  -306,  -306,  -306,   152,  -306,
    -306,  -306,  -306,  -306,  -306,    96,   133,  -306,   133,  -306,
     155,    42,   145,   122,  -306,   122,  -306,  -306,  -306,  -306,
     133,   122,   162,   126,   133,     0,  -306,  -306,  -306,  -306,
    -306,  -306,    42,   149,  -306,   128,   129,   103,  -306,  -306,
    -306,    89,   134,  -306,  -306,    36,   131,   123,  -306,    63,
     154,  -306,  -306,   128,   109,    91,   134,   122,    36,    36,
      89,  -306,  -306,   138,    18,  -306,  -306,  -306,  -306,   121,
     127,  -306,  -306,  -306,  -306,    51,   128,  -306,   159,  -306,
     128,    36,    36,    91,  -306,   138,  -306,  -306,   140,   140,
    -306,   136,   162,  -306,   144,  -306,    36,  -306,   123,  -306,
     122,   147,  -306,  -306,  -306,  -306,  -306,   174,   128,   163,
     120,   157,   140,   176,   140,  -306,   158,   144,    68,    68,
      36,  -306,  -306,    36,  -306,   162,  -306,   156,  -306,   127,
     122,   150,  -306,  -306,   179,   128,   166,   128,    36,  -306,
    -306,    36,  -306,   160,  -306,  -306,   167,  -306,    81,  -306,
    -306,    36,  -306,   162,  -306,  -306,   122,    36,   150,  -306,
    -306,   187,   128,  -306,  -306,  -306,    68,    68,   167,  -306,
    -306,  -306,  -306,   122,    36,  -306,    36,  -306,   147,  -306,
     122,  -306,  -306,   191,  -306,  -306,   122,    36,  -306,   151,
    -306,  -306,   122,   162,  -306,   164,  -306,     6,   162,  -306,
     122,     6,  -306,  -306,   161,   165,     6,   122,  -306,  -306,
     168,   122,  -306,     6,  -306,   169,  -306,   170,   173,  -306,
    -306,   168,   122,  -306,  -306
  };

  const unsigned char
  SpefParse::yydefact_[] =
  {
       0,     0,     0,    34,     0,    16,     1,     0,    42,     0,
       0,     0,    35,    36,     0,     0,    51,    43,    44,    17,
       0,     0,    41,    39,    40,    38,    37,   184,   182,   183,
      46,    48,    50,    47,     0,     0,    87,    52,    53,    45,
      18,     0,     0,    49,    55,    56,     0,    60,    62,    63,
       0,     0,    66,     0,    54,    21,     0,     0,    57,    59,
      70,    64,    70,     0,     0,     0,     0,     0,     0,     0,
      88,     2,    93,    95,    96,    97,    98,    19,     0,     0,
      58,    65,    67,    68,     0,    61,     0,    69,     0,   101,
       0,   171,     0,     0,    94,    20,     0,     0,     0,     0,
       0,     0,    71,    72,    73,    74,    75,    92,     0,    89,
      91,   189,   190,   102,    99,    78,   103,   140,   103,    23,
      22,     0,     0,     0,    77,     0,    86,    85,    84,    90,
     103,     0,     0,     0,   103,     0,    24,    11,    12,    13,
      14,    25,     0,     0,    76,    80,   106,     0,   191,   104,
     105,     0,   120,   142,   185,     0,     0,     0,    26,     0,
       0,   192,   193,     0,    82,     0,   120,     0,     0,     0,
     178,   173,   122,   128,     0,   188,   114,   115,   186,     0,
       0,     3,     4,     5,     6,    27,     0,    15,     0,    81,
       0,     0,     0,   116,   108,   128,    79,   177,     0,     0,
     174,   172,   121,   130,   134,   141,     0,   143,     0,   147,
       0,     0,     7,     8,     9,    10,    28,     0,     0,     0,
       0,     0,     0,    69,     0,   109,   107,   134,     0,     0,
       0,   179,   123,     0,   126,   129,   136,     0,   146,     0,
       0,     0,   187,    30,     0,     0,     0,     0,     0,    70,
      70,     0,   117,     0,   175,   176,     0,   181,     0,   127,
     131,     0,   133,   135,   170,   144,     0,     0,   149,   150,
      31,     0,     0,    29,    83,   113,   110,   111,     0,   119,
     100,   180,   124,     0,     0,   137,     0,   139,     0,   148,
       0,   151,    32,     0,   118,   125,     0,     0,   145,   152,
      33,   132,     0,     0,   153,     0,   138,     0,     0,   154,
       0,   155,   156,   158,   159,   160,     0,     0,   164,   157,
       0,     0,   169,   166,   167,     0,   165,     0,     0,   168,
     163,     0,     0,   161,   162
  };

  const short
  SpefParse::yypgoto_[] =
  {
    -306,  -306,  -306,  -306,    58,  -306,  -306,  -306,  -306,  -306,
    -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,  -306,
    -306,  -306,  -306,  -306,   193,  -306,  -306,  -306,   189,   192,
      -1,  -306,  -306,  -306,   175,  -176,  -306,   125,   181,  -306,
     180,    44,   -38,   -61,   -59,   -91,  -245,  -306,   -94,  -306,
      50,  -306,    52,  -306,  -306,   -50,  -306,   171,  -306,  -306,
     172,   -17,  -106,  -306,  -306,  -306,    30,  -306,  -155,  -198,
    -306,  -306,  -306,    66,  -306,  -306,  -306,  -220,    38,  -306,
    -306,  -306,    12,  -306,  -306,  -306,  -306,  -306,  -306,  -306,
    -306,  -306,    33,     4,   -44,  -306,   -23,  -306,  -306,  -306,
     -65,  -298,  -305,  -306,  -306,  -306,  -306,   -76,  -306,   182,
    -306,  -306,    78,  -306,  -306,  -306,  -306,   -14,  -306,  -306,
    -306,   -96,  -195,  -173
  };

  const short
  SpefParse::yydefgoto_[] =
  {
       0,     2,   185,   216,   141,     3,     4,    10,    21,    57,
      79,    42,    97,   120,   122,   143,   160,   187,   188,   219,
     246,   273,     8,    12,    13,    25,    16,    17,    18,    30,
      31,    36,    37,    44,    45,    60,    46,    84,    38,    48,
      49,    50,    82,    51,    80,   102,   103,   104,   113,   105,
     163,   106,   128,    53,    70,   109,    71,    72,    73,   130,
      88,   114,   133,   149,   166,   193,   194,   222,   175,   176,
     226,   252,   278,   173,   202,   232,   233,   258,   204,   235,
     260,   261,   237,   263,   285,   286,    74,   134,   174,   207,
     288,   208,   180,   211,   242,   268,   269,   304,   305,   311,
     312,   313,   314,   317,   325,   309,   323,   324,    75,    90,
     152,   170,   171,   198,   201,   231,   256,   259,    76,   156,
     157,   115,   150,   164
  };

  const short
  SpefParse::yytable_[] =
  {
      32,    32,   123,    81,    86,   124,   125,   234,   238,   111,
     112,   281,   135,   217,   199,   327,    32,   220,   322,    32,
      47,    52,   228,   229,   146,   322,   333,   144,   153,    43,
      47,   145,    43,   294,    52,   147,   110,   224,   283,   154,
     262,   284,    27,    28,    29,   244,   249,   155,   250,    83,
      85,    87,    89,    91,    89,    91,     1,   205,   129,    14,
      15,     5,   310,     7,   296,   206,   297,     6,   287,   290,
      85,   196,   271,   116,   274,   117,   118,   302,    64,    65,
      66,    67,    68,    69,   111,   112,     9,    27,    28,    29,
      66,    67,    68,    69,    98,    99,   100,   101,    11,   293,
     107,    27,    28,    29,   137,   138,   139,   140,   307,   212,
     213,   214,   215,   316,    34,    35,   240,   181,   182,   183,
     184,    22,    19,    23,    24,   111,   112,   126,   127,    20,
     221,   161,   162,   168,   169,   191,   192,   254,   255,    15,
      40,   177,    41,    35,    55,    56,   266,    59,    62,    63,
     -69,    77,    78,    95,    52,   177,    96,   107,   119,   121,
     136,   131,   142,   132,   282,   148,   151,   159,   167,   165,
     178,   179,   289,   186,   190,   172,   210,   223,   177,   218,
     203,   243,   230,  -112,   245,   247,   270,   236,   272,   295,
     276,   277,   177,    98,   292,   264,   299,   241,   300,   280,
     158,   267,   301,   303,   251,    26,    39,    33,   306,   108,
     275,   315,   197,   189,   318,   315,   257,   308,    54,    58,
     315,   326,   248,   225,   310,   328,   320,   315,    61,   330,
     321,   209,   195,   227,    83,   331,   334,   279,   332,   253,
      92,   239,    94,   265,   298,   291,   319,   329,   200,     0,
       0,    93,     0,   177
  };

  const short
  SpefParse::yycheck_[] =
  {
      14,    15,    98,    62,    65,    99,   100,   202,   206,     3,
       4,   256,   118,   186,   169,   320,    30,   190,   316,    33,
      34,    35,   198,   199,   130,   323,   331,   123,   134,    30,
      44,   125,    33,   278,    48,   131,    86,   192,   258,    39,
     235,   261,     6,     7,     8,   218,   222,    47,   224,    63,
      64,    65,    66,    67,    68,    69,     9,    39,   108,    24,
      25,     5,    56,    23,   284,    47,   286,     0,   263,   267,
      84,   167,   245,    90,   247,    92,    93,   297,    33,    34,
      35,    36,    37,    38,     3,     4,    10,     6,     7,     8,
      35,    36,    37,    38,    26,    27,    28,    29,     6,   272,
       5,     6,     7,     8,    62,    63,    64,    65,   303,    58,
      59,    60,    61,   308,    31,    32,   210,    54,    55,    56,
      57,     5,     5,     7,     8,     3,     4,     6,     7,    11,
     191,     3,     4,    44,    45,    44,    45,   228,   229,    25,
       5,   155,    12,    32,     5,    13,   240,     7,     7,    65,
      65,     5,    15,     5,   168,   169,    14,     5,     5,    16,
       5,    65,    17,    30,   258,     3,    40,    18,    65,    40,
      39,    48,   266,    19,    65,    41,    49,   191,   192,    20,
      42,     7,    46,     7,    21,    65,     7,    43,    22,   283,
     249,   250,   206,    26,     7,    39,   290,    50,     7,    39,
     142,    51,   296,    52,    46,    12,    17,    15,   302,    84,
     248,   307,   168,   163,   310,   311,   230,    53,    37,    44,
     316,   317,    65,   193,    56,   321,    65,   323,    48,    60,
      65,   179,   166,   195,   248,    65,   332,   251,    65,   227,
      68,   208,    71,   239,   288,   268,   311,   323,   170,    -1,
      -1,    69,    -1,   267
  };

  const unsigned char
  SpefParse::yystos_[] =
  {
       0,     9,    67,    71,    72,     5,     0,    23,    88,    10,
      73,     6,    89,    90,    24,    25,    92,    93,    94,     5,
      11,    74,     5,     7,     8,    91,    90,     6,     7,     8,
      95,    96,   183,    95,    31,    32,    97,    98,   104,    94,
       5,    12,    77,    96,    99,   100,   102,   183,   105,   106,
     107,   109,   183,   119,   104,     5,    13,    75,   100,     7,
     101,   106,     7,    65,    33,    34,    35,    36,    37,    38,
     120,   122,   123,   124,   152,   174,   184,     5,    15,    76,
     110,   110,   108,   183,   103,   183,   109,   183,   126,   183,
     175,   183,   126,   175,   123,     5,    14,    78,    26,    27,
      28,    29,   111,   112,   113,   115,   117,     5,   103,   121,
     121,     3,     4,   114,   127,   187,   127,   127,   127,     5,
      79,    16,    80,   187,   114,   114,     6,     7,   118,   121,
     125,    65,    30,   128,   153,   128,     5,    62,    63,    64,
      65,    70,    17,    81,   187,   114,   128,   187,     3,   129,
     188,    40,   176,   128,    39,    47,   185,   186,    70,    18,
      82,     3,     4,   116,   189,    40,   130,    65,    44,    45,
     177,   178,    41,   139,   154,   134,   135,   183,    39,    48,
     158,    54,    55,    56,    57,    68,    19,    83,    84,   116,
      65,    44,    45,   131,   132,   139,   187,   107,   179,   134,
     178,   180,   140,    42,   144,    39,    47,   155,   157,   118,
      49,   159,    58,    59,    60,    61,    69,   189,    20,    85,
     189,   109,   133,   183,   134,   132,   136,   144,   101,   101,
      46,   181,   141,   142,   188,   145,    43,   148,   135,   158,
     114,    50,   160,     7,   189,    21,    86,    65,    65,   101,
     101,    46,   137,   148,   111,   111,   182,   183,   143,   183,
     146,   147,   188,   149,    39,   159,   114,    51,   161,   162,
       7,   189,    22,    87,   189,   108,   110,   110,   138,   183,
      39,   112,   114,   143,   143,   150,   151,   188,   156,   114,
     135,   162,     7,   189,   112,   114,   143,   143,   160,   114,
       7,   114,   143,    52,   163,   164,   114,   188,    53,   171,
      56,   165,   166,   167,   168,   187,   188,   169,   187,   166,
      65,    65,   167,   172,   173,   170,   187,   168,   187,   173,
      60,    65,    65,   168,   187
  };

  const unsigned char
  SpefParse::yyr1_[] =
  {
       0,    66,    67,    68,    68,    68,    68,    69,    69,    69,
      69,    70,    70,    70,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    79,    80,    81,    82,    82,    83,
      84,    85,    86,    87,    88,    88,    89,    89,    90,    91,
      91,    91,    92,    92,    92,    92,    93,    94,    95,    95,
      96,    97,    97,    97,    97,    98,    99,    99,   100,   101,
     102,   103,   104,   105,   105,   106,   107,   107,   108,   109,
     110,   110,   111,   111,   111,   111,   112,   113,   114,   114,
     115,   115,   116,   116,   117,   118,   118,   119,   119,   120,
     120,   120,   121,   122,   122,   123,   123,   123,   123,   125,
     124,   126,   127,   128,   128,   129,   130,   130,   131,   131,
     132,   132,   133,   133,   134,   135,   136,   136,   137,   138,
     139,   139,   140,   140,   141,   141,   142,   143,   144,   144,
     145,   145,   146,   147,   148,   148,   149,   149,   150,   151,
     153,   152,   154,   154,   156,   155,   157,   158,   159,   160,
     161,   161,   162,   162,   163,   164,   165,   165,   166,   167,
     167,   167,   167,   168,   169,   170,   171,   172,   172,   173,
     174,   175,   176,   177,   177,   178,   178,   179,   180,   180,
     181,   182,   183,   183,   183,   184,   184,   185,   186,   187,
     187,   188,   189,   189
  };

  const signed char
  SpefParse::yyr2_[] =
  {
       0,     2,     6,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,    11,     2,     2,     2,     2,
       2,     2,     2,     1,     2,     2,     2,     2,     3,     4,
       3,     3,     3,     3,     0,     2,     1,     2,     2,     1,
       1,     1,     0,     1,     1,     2,     2,     2,     1,     2,
       1,     0,     1,     1,     2,     2,     1,     2,     3,     1,
       1,     1,     2,     1,     2,     3,     1,     3,     1,     1,
       0,     2,     1,     1,     1,     1,     3,     2,     1,     5,
       3,     5,     1,     5,     2,     1,     1,     0,     2,     3,
       4,     3,     1,     1,     2,     1,     1,     1,     1,     0,
      10,     1,     1,     0,     2,     1,     0,     3,     1,     2,
       4,     4,     1,     3,     1,     1,     0,     2,     3,     1,
       0,     2,     0,     2,     3,     4,     1,     1,     0,     2,
       0,     2,     4,     1,     0,     2,     0,     2,     4,     1,
       0,     7,     0,     2,     0,     5,     2,     2,     4,     2,
       1,     2,     3,     4,     2,     3,     1,     2,     1,     1,
       1,     5,     5,     4,     1,     1,     3,     1,     2,     1,
       9,     1,     3,     1,     2,     4,     4,     1,     0,     2,
       3,     1,     1,     1,     1,     5,     6,     4,     2,     1,
       1,     1,     1,     1
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const SpefParse::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "INTEGER", "FLOAT",
  "QSTRING", "INDEX", "IDENT", "NAME", "SPEF", "DESIGN", "DATE", "VENDOR",
  "PROGRAM", "DESIGN_FLOW", "PVERSION", "DIVIDER", "DELIMITER",
  "BUS_DELIMITER", "T_UNIT", "C_UNIT", "R_UNIT", "L_UNIT", "NAME_MAP",
  "POWER_NETS", "GROUND_NETS", "KW_C", "KW_L", "KW_S", "KW_D", "KW_V",
  "PORTS", "PHYSICAL_PORTS", "DEFINE", "PDEFINE", "D_NET", "D_PNET",
  "R_NET", "R_PNET", "END", "CONN", "CAP", "RES", "INDUC", "KW_P", "KW_I",
  "KW_N", "DRIVER", "CELL", "C2_R1_C1", "LOADS", "RC", "KW_Q", "KW_K",
  "'['", "'{'", "'('", "'<'", "']'", "'}'", "')'", "'>'", "'.'", "'/'",
  "'|'", "':'", "$accept", "file", "prefix_bus_delim", "suffix_bus_delim",
  "hchar", "header_def", "spef_version", "design_name", "date",
  "program_name", "program_version", "vendor", "design_flow", "qstrings",
  "hierarchy_div_def", "pin_delim_def", "bus_delim_def", "unit_def",
  "time_scale", "cap_scale", "res_scale", "induc_scale", "name_map",
  "name_map_entries", "name_map_entry", "mapped_item", "power_def",
  "power_net_def", "ground_net_def", "net_names", "net_name",
  "external_def", "port_def", "port_entries", "port_entry", "direction",
  "port_name", "inst_name", "physical_port_def", "pport_entries",
  "pport_entry", "pport_name", "pport", "physical_inst", "conn_attrs",
  "conn_attr", "coordinates", "cap_load", "par_value", "slews",
  "threshold", "driving_cell", "cell_type", "define_def", "define_entry",
  "entity", "internal_def", "nets", "d_net", "$@1", "net", "total_cap",
  "routing_conf", "conf", "conn_sec", "conn_defs", "conn_def",
  "external_connection", "internal_connection", "pin_name",
  "internal_node_coords", "internal_node_coord", "internal_parasitic_node",
  "cap_sec", "cap_elems", "cap_elem", "cap_id", "parasitic_node",
  "res_sec", "res_elems", "res_elem", "res_id", "induc_sec", "induc_elems",
  "induc_elem", "induc_id", "r_net", "$@2", "driver_reducs",
  "driver_reduc", "$@3", "driver_pair", "driver_cell", "pi_model",
  "load_desc", "rc_descs", "rc_desc", "pole_residue_desc", "pole_desc",
  "poles", "pole", "complex_par_value", "cnumber", "real_component",
  "imaginary_component", "residue_desc", "residues", "residue", "d_pnet",
  "pnet_ref", "pconn_sec", "pconn_defs", "pconn_def",
  "pexternal_connection", "internal_pnode_coords", "internal_pnode_coord",
  "internal_pdspf_node", "name_or_index", "r_pnet", "pdriver_reduc",
  "pdriver_pair", "number", "pos_integer", "pos_number", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  SpefParse::yyrline_[] =
  {
       0,   129,   129,   140,   142,   144,   146,   151,   153,   155,
     157,   162,   164,   166,   168,   175,   189,   193,   197,   201,
     205,   209,   213,   218,   222,   229,   234,   239,   241,   248,
     255,   260,   265,   270,   276,   278,   282,   283,   287,   292,
     293,   294,   299,   301,   302,   303,   307,   311,   315,   316,
     320,   326,   328,   329,   330,   334,   338,   339,   343,   348,
     353,   357,   361,   365,   366,   370,   375,   377,   382,   386,
     391,   393,   397,   398,   399,   400,   404,   408,   413,   415,
     420,   424,   431,   432,   436,   440,   441,   446,   448,   452,
     453,   454,   458,   464,   465,   469,   470,   471,   472,   479,
     478,   485,   490,   493,   495,   499,   504,   506,   510,   511,
     515,   516,   520,   522,   527,   531,   535,   537,   541,   545,
     551,   553,   558,   559,   564,   566,   571,   575,   580,   582,
     587,   588,   593,   598,   603,   605,   610,   611,   616,   621,
     628,   627,   633,   635,   640,   639,   646,   651,   656,   663,
     667,   668,   672,   674,   679,   683,   687,   688,   693,   697,
     698,   699,   700,   704,   709,   713,   717,   721,   722,   726,
     732,   737,   741,   745,   746,   750,   751,   755,   758,   760,
     764,   768,   773,   774,   775,   781,   782,   786,   790,   796,
     798,   803,   812,   818
  };

  void
  SpefParse::yy_stack_print_ () const
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
  SpefParse::yy_reduce_print_ (int yyrule) const
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

  SpefParse::symbol_kind_type
  SpefParse::yytranslate_ (int t) YY_NOEXCEPT
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      56,    60,     2,     2,     2,     2,    62,    63,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    65,     2,
      57,     2,    61,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    54,     2,    58,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    55,    64,    59,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53
    };
    // Last valid token kind.
    const int code_max = 308;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 68 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"
} // sta
#line 2637 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SpefParse.cc"

#line 826 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/parasitics/SpefParse.yy"

