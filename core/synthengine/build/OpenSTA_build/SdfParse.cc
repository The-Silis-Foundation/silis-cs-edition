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
#line 25 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"

#include <cctype>
#include <string>
#include <utility>

#include "sdf/SdfReaderPvt.hh"
#include "sdf/SdfScanner.hh"

#undef yylex
#define yylex scanner->lex

// warning: variable 'yynerrs_' set but not used
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

void
sta::SdfParse::error(const location_type &loc,
                     const std::string &msg)
{
  reader->report()->fileError(170, reader->filename(), loc.begin.line,"{}",msg);
}

#line 63 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"


#include "SdfParse.hh"




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

#line 66 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
namespace sta {
#line 161 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"

  /// Build a parser object.
  SdfParse::SdfParse (SdfScanner *scanner_yyarg, SdfReader *reader_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      scanner (scanner_yyarg),
      reader (reader_yyarg)
  {}

  SdfParse::~SdfParse ()
  {}

  SdfParse::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  SdfParse::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S_hchar: // hchar
        value.copy< char > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_port_transition: // port_transition
        value.copy< const sta::Transition * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FNUMBER: // FNUMBER
      case symbol_kind::S_NUMBER: // NUMBER
        value.copy< float > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_number_opt: // number_opt
        value.copy< float* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DNUMBER: // DNUMBER
        value.copy< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_port_spec: // port_spec
      case symbol_kind::S_port_tchk: // port_tchk
        value.copy< sta::SdfPortSpec * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_value: // value
      case symbol_kind::S_triple: // triple
        value.copy< sta::SdfTriple * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_delval_list: // delval_list
        value.copy< sta::SdfTripleSeq * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_QSTRING: // QSTRING
      case symbol_kind::S_ID: // ID
      case symbol_kind::S_EXPR_OPEN_IOPATH: // EXPR_OPEN_IOPATH
      case symbol_kind::S_EXPR_OPEN: // EXPR_OPEN
      case symbol_kind::S_EXPR_ID_CLOSE: // EXPR_ID_CLOSE
      case symbol_kind::S_path: // path
      case symbol_kind::S_port: // port
      case symbol_kind::S_port_instance: // port_instance
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }




  template <typename Base>
  SdfParse::symbol_kind_type
  SdfParse::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  SdfParse::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  SdfParse::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_hchar: // hchar
        value.move< char > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_port_transition: // port_transition
        value.move< const sta::Transition * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_FNUMBER: // FNUMBER
      case symbol_kind::S_NUMBER: // NUMBER
        value.move< float > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_number_opt: // number_opt
        value.move< float* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_DNUMBER: // DNUMBER
        value.move< int > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_port_spec: // port_spec
      case symbol_kind::S_port_tchk: // port_tchk
        value.move< sta::SdfPortSpec * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_value: // value
      case symbol_kind::S_triple: // triple
        value.move< sta::SdfTriple * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_delval_list: // delval_list
        value.move< sta::SdfTripleSeq * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_QSTRING: // QSTRING
      case symbol_kind::S_ID: // ID
      case symbol_kind::S_EXPR_OPEN_IOPATH: // EXPR_OPEN_IOPATH
      case symbol_kind::S_EXPR_OPEN: // EXPR_OPEN
      case symbol_kind::S_EXPR_ID_CLOSE: // EXPR_ID_CLOSE
      case symbol_kind::S_path: // path
      case symbol_kind::S_port: // port
      case symbol_kind::S_port_instance: // port_instance
        value.move< std::string > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  SdfParse::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  SdfParse::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  SdfParse::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  SdfParse::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  SdfParse::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  SdfParse::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  SdfParse::symbol_kind_type
  SdfParse::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  SdfParse::symbol_kind_type
  SdfParse::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  SdfParse::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  SdfParse::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  SdfParse::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  SdfParse::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  SdfParse::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  SdfParse::symbol_kind_type
  SdfParse::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  SdfParse::stack_symbol_type::stack_symbol_type ()
  {}

  SdfParse::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_hchar: // hchar
        value.YY_MOVE_OR_COPY< char > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_port_transition: // port_transition
        value.YY_MOVE_OR_COPY< const sta::Transition * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FNUMBER: // FNUMBER
      case symbol_kind::S_NUMBER: // NUMBER
        value.YY_MOVE_OR_COPY< float > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_number_opt: // number_opt
        value.YY_MOVE_OR_COPY< float* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DNUMBER: // DNUMBER
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_port_spec: // port_spec
      case symbol_kind::S_port_tchk: // port_tchk
        value.YY_MOVE_OR_COPY< sta::SdfPortSpec * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_value: // value
      case symbol_kind::S_triple: // triple
        value.YY_MOVE_OR_COPY< sta::SdfTriple * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_delval_list: // delval_list
        value.YY_MOVE_OR_COPY< sta::SdfTripleSeq * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_QSTRING: // QSTRING
      case symbol_kind::S_ID: // ID
      case symbol_kind::S_EXPR_OPEN_IOPATH: // EXPR_OPEN_IOPATH
      case symbol_kind::S_EXPR_OPEN: // EXPR_OPEN
      case symbol_kind::S_EXPR_ID_CLOSE: // EXPR_ID_CLOSE
      case symbol_kind::S_path: // path
      case symbol_kind::S_port: // port
      case symbol_kind::S_port_instance: // port_instance
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

  SdfParse::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_hchar: // hchar
        value.move< char > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_port_transition: // port_transition
        value.move< const sta::Transition * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FNUMBER: // FNUMBER
      case symbol_kind::S_NUMBER: // NUMBER
        value.move< float > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_number_opt: // number_opt
        value.move< float* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DNUMBER: // DNUMBER
        value.move< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_port_spec: // port_spec
      case symbol_kind::S_port_tchk: // port_tchk
        value.move< sta::SdfPortSpec * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_value: // value
      case symbol_kind::S_triple: // triple
        value.move< sta::SdfTriple * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_delval_list: // delval_list
        value.move< sta::SdfTripleSeq * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_QSTRING: // QSTRING
      case symbol_kind::S_ID: // ID
      case symbol_kind::S_EXPR_OPEN_IOPATH: // EXPR_OPEN_IOPATH
      case symbol_kind::S_EXPR_OPEN: // EXPR_OPEN
      case symbol_kind::S_EXPR_ID_CLOSE: // EXPR_ID_CLOSE
      case symbol_kind::S_path: // path
      case symbol_kind::S_port: // port
      case symbol_kind::S_port_instance: // port_instance
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  SdfParse::stack_symbol_type&
  SdfParse::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_hchar: // hchar
        value.copy< char > (that.value);
        break;

      case symbol_kind::S_port_transition: // port_transition
        value.copy< const sta::Transition * > (that.value);
        break;

      case symbol_kind::S_FNUMBER: // FNUMBER
      case symbol_kind::S_NUMBER: // NUMBER
        value.copy< float > (that.value);
        break;

      case symbol_kind::S_number_opt: // number_opt
        value.copy< float* > (that.value);
        break;

      case symbol_kind::S_DNUMBER: // DNUMBER
        value.copy< int > (that.value);
        break;

      case symbol_kind::S_port_spec: // port_spec
      case symbol_kind::S_port_tchk: // port_tchk
        value.copy< sta::SdfPortSpec * > (that.value);
        break;

      case symbol_kind::S_value: // value
      case symbol_kind::S_triple: // triple
        value.copy< sta::SdfTriple * > (that.value);
        break;

      case symbol_kind::S_delval_list: // delval_list
        value.copy< sta::SdfTripleSeq * > (that.value);
        break;

      case symbol_kind::S_QSTRING: // QSTRING
      case symbol_kind::S_ID: // ID
      case symbol_kind::S_EXPR_OPEN_IOPATH: // EXPR_OPEN_IOPATH
      case symbol_kind::S_EXPR_OPEN: // EXPR_OPEN
      case symbol_kind::S_EXPR_ID_CLOSE: // EXPR_ID_CLOSE
      case symbol_kind::S_path: // path
      case symbol_kind::S_port: // port
      case symbol_kind::S_port_instance: // port_instance
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  SdfParse::stack_symbol_type&
  SdfParse::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_hchar: // hchar
        value.move< char > (that.value);
        break;

      case symbol_kind::S_port_transition: // port_transition
        value.move< const sta::Transition * > (that.value);
        break;

      case symbol_kind::S_FNUMBER: // FNUMBER
      case symbol_kind::S_NUMBER: // NUMBER
        value.move< float > (that.value);
        break;

      case symbol_kind::S_number_opt: // number_opt
        value.move< float* > (that.value);
        break;

      case symbol_kind::S_DNUMBER: // DNUMBER
        value.move< int > (that.value);
        break;

      case symbol_kind::S_port_spec: // port_spec
      case symbol_kind::S_port_tchk: // port_tchk
        value.move< sta::SdfPortSpec * > (that.value);
        break;

      case symbol_kind::S_value: // value
      case symbol_kind::S_triple: // triple
        value.move< sta::SdfTriple * > (that.value);
        break;

      case symbol_kind::S_delval_list: // delval_list
        value.move< sta::SdfTripleSeq * > (that.value);
        break;

      case symbol_kind::S_QSTRING: // QSTRING
      case symbol_kind::S_ID: // ID
      case symbol_kind::S_EXPR_OPEN_IOPATH: // EXPR_OPEN_IOPATH
      case symbol_kind::S_EXPR_OPEN: // EXPR_OPEN
      case symbol_kind::S_EXPR_ID_CLOSE: // EXPR_ID_CLOSE
      case symbol_kind::S_path: // path
      case symbol_kind::S_port: // port
      case symbol_kind::S_port_instance: // port_instance
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
  SdfParse::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  SdfParse::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
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
  SdfParse::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  SdfParse::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  SdfParse::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  SdfParse::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  SdfParse::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  SdfParse::debug_level_type
  SdfParse::debug_level () const
  {
    return yydebug_;
  }

  void
  SdfParse::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  SdfParse::state_type
  SdfParse::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  SdfParse::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  SdfParse::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  SdfParse::operator() ()
  {
    return parse ();
  }

  int
  SdfParse::parse ()
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
      case symbol_kind::S_hchar: // hchar
        yylhs.value.emplace< char > ();
        break;

      case symbol_kind::S_port_transition: // port_transition
        yylhs.value.emplace< const sta::Transition * > ();
        break;

      case symbol_kind::S_FNUMBER: // FNUMBER
      case symbol_kind::S_NUMBER: // NUMBER
        yylhs.value.emplace< float > ();
        break;

      case symbol_kind::S_number_opt: // number_opt
        yylhs.value.emplace< float* > ();
        break;

      case symbol_kind::S_DNUMBER: // DNUMBER
        yylhs.value.emplace< int > ();
        break;

      case symbol_kind::S_port_spec: // port_spec
      case symbol_kind::S_port_tchk: // port_tchk
        yylhs.value.emplace< sta::SdfPortSpec * > ();
        break;

      case symbol_kind::S_value: // value
      case symbol_kind::S_triple: // triple
        yylhs.value.emplace< sta::SdfTriple * > ();
        break;

      case symbol_kind::S_delval_list: // delval_list
        yylhs.value.emplace< sta::SdfTripleSeq * > ();
        break;

      case symbol_kind::S_QSTRING: // QSTRING
      case symbol_kind::S_ID: // ID
      case symbol_kind::S_EXPR_OPEN_IOPATH: // EXPR_OPEN_IOPATH
      case symbol_kind::S_EXPR_OPEN: // EXPR_OPEN
      case symbol_kind::S_EXPR_ID_CLOSE: // EXPR_ID_CLOSE
      case symbol_kind::S_path: // path
      case symbol_kind::S_port: // port
      case symbol_kind::S_port_instance: // port_instance
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
  case 2: // file: '(' DELAYFILE header cells ')'
#line 104 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
                                        {}
#line 973 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 11: // header_stmt: '(' DIVIDER hchar ')'
#line 120 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
                              { reader->setDivider(yystack_[1].value.as < char > ()); }
#line 979 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 12: // header_stmt: '(' VOLTAGE triple ')'
#line 121 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
                               { reader->deleteTriple(yystack_[1].value.as < sta::SdfTriple * > ()); }
#line 985 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 18: // header_stmt: '(' TEMPERATURE triple ')'
#line 127 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
                                   { reader->deleteTriple(yystack_[1].value.as < sta::SdfTriple * > ()); }
#line 991 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 20: // header_stmt: '(' TIMESCALE NUMBER ID ')'
#line 129 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
                                    { reader->setTimescale(yystack_[2].value.as < float > (), yystack_[1].value.as < std::string > ()); }
#line 997 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 21: // hchar: '/'
#line 134 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { yylhs.value.as < char > () = '/'; }
#line 1003 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 22: // hchar: '.'
#line 136 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { yylhs.value.as < char > () = '.'; }
#line 1009 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 23: // number_opt: %empty
#line 139 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
            { yylhs.value.as < float* > () = nullptr; }
#line 1015 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 24: // number_opt: NUMBER
#line 140 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
                { yylhs.value.as < float* > () = new float(yystack_[0].value.as < float > ()); }
#line 1021 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 27: // cell: '(' CELL celltype cell_instance timing_specs ')'
#line 150 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { reader->cellFinish(); }
#line 1027 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 28: // celltype: '(' CELLTYPE QSTRING ')'
#line 155 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { reader->setCell(yystack_[1].value.as < std::string > ()); }
#line 1033 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 29: // cell_instance: '(' INSTANCE ')'
#line 160 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { reader->setInstance(); }
#line 1039 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 30: // cell_instance: '(' INSTANCE '*' ')'
#line 162 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { reader->setInstanceWildcard(); }
#line 1045 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 31: // cell_instance: '(' INSTANCE path ')'
#line 164 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { reader->setInstance(yystack_[1].value.as < std::string > ()); }
#line 1051 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 38: // $@1: %empty
#line 183 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { reader->setInIncremental(false); }
#line 1057 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 40: // $@2: %empty
#line 186 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { reader->setInIncremental(true); }
#line 1063 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 44: // path: ID
#line 196 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { yylhs.value.as < std::string > () = reader->unescaped(yystack_[0].value.as < std::string > ()); }
#line 1069 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 45: // path: path hchar ID
#line 198 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { yylhs.value.as < std::string > () = reader->makePath(yystack_[2].value.as < std::string > (), reader->unescaped(yystack_[0].value.as < std::string > ())); }
#line 1075 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 46: // del_def: '(' IOPATH port_spec port_instance retains delval_list ')'
#line 203 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { reader->iopath(yystack_[4].value.as < sta::SdfPortSpec * > (), yystack_[3].value.as < std::string > (), yystack_[1].value.as < sta::SdfTripleSeq * > (), "", false); }
#line 1081 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 47: // del_def: '(' CONDELSE '(' IOPATH port_spec port_instance retains delval_list ')' ')'
#line 206 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { reader->iopath(yystack_[5].value.as < sta::SdfPortSpec * > (), yystack_[4].value.as < std::string > (), yystack_[2].value.as < sta::SdfTripleSeq * > (), "", true); }
#line 1087 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 48: // del_def: '(' COND EXPR_OPEN_IOPATH port_spec port_instance retains delval_list ')' ')'
#line 209 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { reader->iopath(yystack_[5].value.as < sta::SdfPortSpec * > (), yystack_[4].value.as < std::string > (), yystack_[2].value.as < sta::SdfTripleSeq * > (), yystack_[6].value.as < std::string > (), false); }
#line 1093 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 49: // del_def: '(' INTERCONNECT port_instance port_instance delval_list ')'
#line 211 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { reader->interconnect(yystack_[3].value.as < std::string > (), yystack_[2].value.as < std::string > (), yystack_[1].value.as < sta::SdfTripleSeq * > ()); }
#line 1099 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 50: // del_def: '(' PORT port_instance delval_list ')'
#line 213 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { reader->port(yystack_[2].value.as < std::string > (), yystack_[1].value.as < sta::SdfTripleSeq * > ()); }
#line 1105 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 51: // del_def: '(' DEVICE delval_list ')'
#line 215 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { reader->device(yystack_[1].value.as < sta::SdfTripleSeq * > ()); }
#line 1111 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 52: // del_def: '(' DEVICE port_instance delval_list ')'
#line 217 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { reader->device(yystack_[2].value.as < std::string > (), yystack_[1].value.as < sta::SdfTripleSeq * > ()); }
#line 1117 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 55: // retain: '(' RETAIN delval_list ')'
#line 227 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { reader->deleteTripleSeq(yystack_[1].value.as < sta::SdfTripleSeq * > ()); }
#line 1123 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 56: // delval_list: value
#line 232 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { yylhs.value.as < sta::SdfTripleSeq * > () = reader->makeTripleSeq(); yylhs.value.as < sta::SdfTripleSeq * > ()->push_back(yystack_[0].value.as < sta::SdfTriple * > ()); }
#line 1129 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 57: // delval_list: delval_list value
#line 234 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { yystack_[1].value.as < sta::SdfTripleSeq * > ()->push_back(yystack_[0].value.as < sta::SdfTriple * > ()); yylhs.value.as < sta::SdfTripleSeq * > () = yystack_[1].value.as < sta::SdfTripleSeq * > (); }
#line 1135 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 60: // $@3: %empty
#line 242 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
                  { reader->setInTimingCheck(true); }
#line 1141 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 61: // tchk_def: '(' SETUP $@3 port_tchk port_tchk value ')'
#line 244 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { reader->timingCheck(sta::TimingRole::setup(), yystack_[3].value.as < sta::SdfPortSpec * > (), yystack_[2].value.as < sta::SdfPortSpec * > (), yystack_[1].value.as < sta::SdfTriple * > ());
	  reader->setInTimingCheck(false);
	}
#line 1149 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 62: // $@4: %empty
#line 247 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
                 { reader->setInTimingCheck(true); }
#line 1155 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 63: // tchk_def: '(' HOLD $@4 port_tchk port_tchk value ')'
#line 249 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { reader->timingCheck(sta::TimingRole::hold(), yystack_[3].value.as < sta::SdfPortSpec * > (), yystack_[2].value.as < sta::SdfPortSpec * > (), yystack_[1].value.as < sta::SdfTriple * > ());
	  reader->setInTimingCheck(false);
	}
#line 1163 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 64: // $@5: %empty
#line 252 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
                      { reader->setInTimingCheck(true); }
#line 1169 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 65: // tchk_def: '(' SETUPHOLD $@5 port_tchk port_tchk value value ')'
#line 254 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { reader->timingCheckSetupHold(yystack_[4].value.as < sta::SdfPortSpec * > (), yystack_[3].value.as < sta::SdfPortSpec * > (), yystack_[2].value.as < sta::SdfTriple * > (), yystack_[1].value.as < sta::SdfTriple * > ());
	  reader->setInTimingCheck(false);
	}
#line 1177 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 66: // $@6: %empty
#line 257 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
                     { reader->setInTimingCheck(true); }
#line 1183 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 67: // tchk_def: '(' RECOVERY $@6 port_tchk port_tchk value ')'
#line 259 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { reader->timingCheck(sta::TimingRole::recovery(),yystack_[3].value.as < sta::SdfPortSpec * > (),yystack_[2].value.as < sta::SdfPortSpec * > (),yystack_[1].value.as < sta::SdfTriple * > ());
	  reader->setInTimingCheck(false);
	}
#line 1191 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 68: // $@7: %empty
#line 262 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
                    { reader->setInTimingCheck(true); }
#line 1197 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 69: // tchk_def: '(' REMOVAL $@7 port_tchk port_tchk value ')'
#line 264 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { reader->timingCheck(sta::TimingRole::removal(),yystack_[3].value.as < sta::SdfPortSpec * > (),yystack_[2].value.as < sta::SdfPortSpec * > (),yystack_[1].value.as < sta::SdfTriple * > ());
	  reader->setInTimingCheck(false);
	}
#line 1205 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 70: // $@8: %empty
#line 267 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
                   { reader->setInTimingCheck(true); }
#line 1211 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 71: // tchk_def: '(' RECREM $@8 port_tchk port_tchk value value ')'
#line 269 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { reader->timingCheckRecRem(yystack_[4].value.as < sta::SdfPortSpec * > (), yystack_[3].value.as < sta::SdfPortSpec * > (), yystack_[2].value.as < sta::SdfTriple * > (), yystack_[1].value.as < sta::SdfTriple * > ());
	  reader->setInTimingCheck(false);
	}
#line 1219 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 72: // $@9: %empty
#line 272 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
                 { reader->setInTimingCheck(true); }
#line 1225 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 73: // tchk_def: '(' SKEW $@9 port_tchk port_tchk value ')'
#line 275 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { reader->timingCheck(sta::TimingRole::skew(),yystack_[2].value.as < sta::SdfPortSpec * > (),yystack_[3].value.as < sta::SdfPortSpec * > (),yystack_[1].value.as < sta::SdfTriple * > ());
	  reader->setInTimingCheck(false);
	}
#line 1233 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 74: // $@10: %empty
#line 278 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
                  { reader->setInTimingCheck(true); }
#line 1239 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 75: // tchk_def: '(' WIDTH $@10 port_tchk value ')'
#line 280 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { reader->timingCheckWidth(yystack_[2].value.as < sta::SdfPortSpec * > (), yystack_[1].value.as < sta::SdfTriple * > ());
	  reader->setInTimingCheck(false);
	}
#line 1247 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 76: // $@11: %empty
#line 283 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
                   { reader->setInTimingCheck(true); }
#line 1253 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 77: // tchk_def: '(' PERIOD $@11 port_tchk value ')'
#line 285 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { reader->timingCheckPeriod(yystack_[2].value.as < sta::SdfPortSpec * > (), yystack_[1].value.as < sta::SdfTriple * > ());
	  reader->setInTimingCheck(false);
	}
#line 1261 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 78: // $@12: %empty
#line 288 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
                     { reader->setInTimingCheck(true); }
#line 1267 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 79: // tchk_def: '(' NOCHANGE $@12 port_tchk port_tchk value value ')'
#line 290 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { reader->timingCheckNochange(yystack_[4].value.as < sta::SdfPortSpec * > (), yystack_[3].value.as < sta::SdfPortSpec * > (), yystack_[2].value.as < sta::SdfTriple * > (), yystack_[1].value.as < sta::SdfTriple * > ());
	  reader->setInTimingCheck(false);
	}
#line 1275 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 80: // port: ID
#line 297 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { yylhs.value.as < std::string > () = reader->unescaped(yystack_[0].value.as < std::string > ()); }
#line 1281 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 81: // port: ID '[' DNUMBER ']'
#line 299 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { yylhs.value.as < std::string > () = reader->makeBusName(yystack_[3].value.as < std::string > (), yystack_[1].value.as < int > ()); }
#line 1287 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 82: // port_instance: port
#line 304 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1293 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 83: // port_instance: path hchar port
#line 306 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { yylhs.value.as < std::string > () = reader->makePath(yystack_[2].value.as < std::string > (), yystack_[0].value.as < std::string > ()); }
#line 1299 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 84: // port_spec: port_instance
#line 311 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { yylhs.value.as < sta::SdfPortSpec * > () = reader->makePortSpec(sta::Transition::riseFall(), yystack_[0].value.as < std::string > ()); }
#line 1305 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 85: // port_spec: '(' port_transition port_instance ')'
#line 313 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { yylhs.value.as < sta::SdfPortSpec * > () = reader->makePortSpec(yystack_[2].value.as < const sta::Transition * > (), yystack_[1].value.as < std::string > ()); }
#line 1311 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 86: // port_transition: POSEDGE
#line 317 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
                { yylhs.value.as < const sta::Transition * > () = sta::Transition::rise(); }
#line 1317 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 87: // port_transition: NEGEDGE
#line 318 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
                { yylhs.value.as < const sta::Transition * > () = sta::Transition::fall(); }
#line 1323 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 88: // port_tchk: port_spec
#line 323 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { yylhs.value.as < sta::SdfPortSpec * > () = yystack_[0].value.as < sta::SdfPortSpec * > (); }
#line 1329 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 89: // port_tchk: '(' COND EXPR_ID_CLOSE
#line 325 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { yylhs.value.as < sta::SdfPortSpec * > () = reader->makeCondPortSpec(yystack_[0].value.as < std::string > ()); }
#line 1335 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 90: // port_tchk: '(' COND EXPR_OPEN port_transition port_instance ')' ')'
#line 327 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { yylhs.value.as < sta::SdfPortSpec * > () = reader->makePortSpec(yystack_[3].value.as < const sta::Transition * > (), yystack_[2].value.as < std::string > (), yystack_[4].value.as < std::string > ()); }
#line 1341 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 91: // value: '(' ')'
#line 332 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        {
	  yylhs.value.as < sta::SdfTriple * > () = reader->makeTriple();
	}
#line 1349 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 92: // value: '(' NUMBER ')'
#line 336 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        {
	  yylhs.value.as < sta::SdfTriple * > () = reader->makeTriple(yystack_[1].value.as < float > ());
	}
#line 1357 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 93: // value: '(' triple ')'
#line 339 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
                       { yylhs.value.as < sta::SdfTriple * > () = yystack_[1].value.as < sta::SdfTriple * > (); }
#line 1363 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 94: // triple: NUMBER ':' number_opt ':' number_opt
#line 344 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        {
	  float *fp = new float(yystack_[4].value.as < float > ());
	  yylhs.value.as < sta::SdfTriple * > () = reader->makeTriple(fp, yystack_[2].value.as < float* > (), yystack_[0].value.as < float* > ());
	}
#line 1372 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 95: // triple: number_opt ':' NUMBER ':' number_opt
#line 349 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        {
	  float *fp = new float(yystack_[2].value.as < float > ());
	  yylhs.value.as < sta::SdfTriple * > () = reader->makeTriple(yystack_[4].value.as < float* > (), fp, yystack_[0].value.as < float* > ());
	}
#line 1381 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 96: // triple: number_opt ':' number_opt ':' NUMBER
#line 354 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        {
	  float *fp = new float(yystack_[0].value.as < float > ());
	  yylhs.value.as < sta::SdfTriple * > () = reader->makeTriple(yystack_[4].value.as < float* > (), yystack_[2].value.as < float* > (), fp);
	}
#line 1390 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 97: // NUMBER: FNUMBER
#line 362 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { yylhs.value.as < float > () = yystack_[0].value.as < float > (); }
#line 1396 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 98: // NUMBER: DNUMBER
#line 364 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { yylhs.value.as < float > () = static_cast<float>(yystack_[0].value.as < int > ()); }
#line 1402 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;

  case 99: // NUMBER: '-' DNUMBER
#line 366 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
        { yylhs.value.as < float > () = static_cast<float>(-yystack_[0].value.as < int > ()); }
#line 1408 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"
    break;


#line 1412 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"

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
  SdfParse::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

#if YYDEBUG || 0
  const char *
  SdfParse::symbol_name (symbol_kind_type yysymbol)
  {
    return yytname_[yysymbol];
  }
#endif // #if YYDEBUG || 0









  const short SdfParse::yypact_ninf_ = -188;

  const signed char SdfParse::yytable_ninf_ = -46;

  const short
  SdfParse::yypact_[] =
  {
     -39,    58,    54,   -11,  -188,   151,    23,  -188,    70,    97,
     100,   102,   126,   130,     0,    25,    14,    53,     3,   139,
    -188,    38,  -188,    99,   124,   146,   166,   167,   168,  -188,
    -188,   169,  -188,  -188,  -188,   179,   145,   170,   -42,   171,
    -188,  -188,   173,   -23,   219,   176,   203,  -188,  -188,  -188,
    -188,  -188,  -188,  -188,  -188,  -188,  -188,     3,  -188,  -188,
       3,  -188,  -188,  -188,   177,   204,   180,   174,   175,   178,
    -188,  -188,   228,   208,  -188,     3,     3,     3,   185,    27,
      93,  -188,  -188,  -188,  -188,  -188,  -188,   186,    40,     5,
    -188,  -188,  -188,  -188,   232,  -188,  -188,  -188,   136,   138,
      87,  -188,  -188,   140,  -188,  -188,  -188,  -188,  -188,  -188,
    -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,
      36,    36,    36,    36,    36,    36,    36,    36,    36,    36,
     141,   143,    77,   101,     0,  -188,  -188,  -188,    36,    36,
      36,    36,    36,    36,   189,   189,    36,    36,    65,  -188,
    -188,  -188,   229,  -188,  -188,   187,   235,   236,   189,   189,
     189,   189,   189,   189,    61,   192,   193,   189,   189,   235,
     235,    37,    44,   238,   196,   191,   152,  -188,   197,    81,
    -188,   198,   199,   189,   200,   201,   189,  -188,   202,    52,
    -188,  -188,   205,   189,   235,   189,   150,   189,  -188,   152,
     235,    44,   220,  -188,   235,  -188,  -188,  -188,   206,  -188,
    -188,   207,  -188,  -188,  -188,   209,   189,   153,  -188,  -188,
     155,  -188,   235,    44,   210,  -188,  -188,  -188,   157,  -188,
    -188,   212,  -188,   235,   213,  -188,    16,  -188,   159,   212,
    -188,  -188,   189,  -188,   161,   212,   163,   214,   165,  -188,
    -188,   215,  -188
  };

  const signed char
  SdfParse::yydefact_[] =
  {
       0,     0,     0,     0,     1,     0,     0,     3,     0,     0,
       0,     0,     0,     0,     0,    23,     0,    23,     0,     0,
       4,     0,    25,     0,     0,     0,     0,     0,     0,    21,
      22,     0,    97,    98,    14,     0,     0,     0,     0,     0,
      16,    19,     0,     0,     0,     0,     0,     2,    26,     5,
       6,     7,     8,     9,    10,    11,    99,    23,    12,    13,
      23,    15,    18,    17,     0,     0,     0,     0,     0,     0,
      24,    20,     0,     0,    32,     0,    23,    23,     0,     0,
       0,    96,    95,    94,    28,    44,    29,     0,     0,     0,
      27,    33,    30,    31,     0,    36,    58,    45,     0,     0,
       0,    34,    37,     0,    35,    59,    38,    40,    60,    62,
      64,    66,    68,    70,    74,    76,    72,    78,    42,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    80,     0,     0,    82,    84,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    39,
      43,    41,     0,    86,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    23,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,     0,    80,
      83,     0,     0,     0,     0,     0,     0,    91,     0,     0,
      75,    77,     0,     0,     0,     0,     0,     0,    56,     0,
       0,     0,     0,    81,     0,    85,    61,    63,     0,    67,
      69,     0,    93,    92,    73,     0,     0,     0,    51,    57,
       0,    53,     0,     0,     0,    65,    71,    79,     0,    50,
      52,     0,    53,     0,     0,    49,    23,    54,     0,     0,
      53,    90,     0,    46,     0,     0,     0,     0,     0,    55,
      48,     0,    47
  };

  const short
  SdfParse::yypgoto_[] =
  {
    -188,  -188,  -188,   247,   -52,    39,  -188,   240,  -188,  -188,
    -188,  -188,  -188,  -188,  -188,  -188,   147,   188,  -188,  -157,
    -188,  -187,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,
    -188,  -188,  -188,  -188,   108,  -155,  -166,    92,    -3,  -140,
      49,   -15
  };

  const unsigned char
  SdfParse::yydefgoto_[] =
  {
       0,     2,     6,     7,    31,    36,    21,    22,    66,    74,
      80,    91,    98,   102,   118,   119,   130,   134,   150,   231,
     237,   196,    99,   105,   120,   121,   122,   123,   124,   125,
     128,   126,   127,   129,   135,   136,   137,   156,   138,   198,
     188,    70
  };

  const short
  SdfParse::yytable_[] =
  {
      38,   178,    43,    44,   165,   166,   200,    59,   217,     1,
     220,    32,    33,    60,   194,   195,   197,    39,   181,   182,
     183,   184,   185,   186,    32,    33,    63,   192,   193,   228,
      95,    85,    60,    32,    33,   222,    94,     5,    96,   216,
     132,   132,    68,   208,   238,   221,   211,   242,   132,   224,
      29,    30,   244,   215,     4,   246,   219,   233,   248,    35,
      81,    32,    33,    40,    37,   187,    42,   232,     3,    32,
      33,    19,    35,    23,    34,   239,    86,   219,   240,    87,
     219,    35,   157,   245,   133,   164,    46,    47,   219,    93,
      29,    30,   199,   169,   170,   171,    67,   172,   219,    69,
      24,   213,    41,    25,   219,    26,   219,    60,   219,    35,
     187,   173,   174,   106,   107,    82,    83,    35,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   -44,   -44,    27,
     152,   -45,   -45,    28,   152,   158,   159,   160,   161,   162,
     163,    89,    90,   167,   168,   153,   154,   155,    49,   189,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    45,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    50,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   100,   101,   103,   104,    56,   148,
     149,   148,   151,   176,   177,    51,   153,   154,   164,   218,
      57,   164,   229,   164,   230,   164,   235,   164,   243,   164,
     247,   164,   249,   164,   251,    52,    53,    54,    55,    58,
      61,   189,    62,    64,    65,    45,    71,    72,    73,    75,
      76,    78,    79,    77,    84,    92,    97,   164,   175,   132,
     179,   190,   191,   201,   202,   203,   205,   206,   207,   209,
     210,   212,   223,    20,   214,   225,   226,     0,   227,   234,
     236,    48,   241,   250,   252,   180,   131,    88,   204
  };

  const short
  SdfParse::yycheck_[] =
  {
      15,   156,    17,    18,   144,   145,   172,    49,   195,    48,
     197,     8,     9,    55,   169,   170,   171,     3,   158,   159,
     160,   161,   162,   163,     8,     9,    49,   167,   168,   216,
      25,     4,    55,     8,     9,   201,    88,    48,    33,   194,
       4,     4,    57,   183,   231,   200,   186,    31,     4,   204,
      50,    51,   239,   193,     0,   242,   196,   223,   245,    56,
      75,     8,     9,    49,    15,    49,    17,   222,    10,     8,
       9,    48,    56,     3,    49,   232,    49,   217,   233,    52,
     220,    56,   134,   240,    48,    48,    48,    49,   228,    49,
      50,    51,    48,    28,    29,    30,    57,    32,   238,    60,
       3,    49,    49,     3,   244,     3,   246,    55,   248,    56,
      49,    46,    47,    26,    27,    76,    77,    56,   121,   122,
     123,   124,   125,   126,   127,   128,   129,    50,    51,     3,
      53,    50,    51,     3,    53,   138,   139,   140,   141,   142,
     143,    48,    49,   146,   147,    44,    45,    46,    49,   164,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    49,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    48,    49,    48,    49,     9,    48,
      49,    48,    49,     6,     7,    49,    44,    45,    48,    49,
      55,    48,    49,    48,    49,    48,    49,    48,    49,    48,
      49,    48,    49,    48,    49,    49,    49,    49,    49,    49,
      49,   236,    49,     4,    48,    22,    49,    23,    48,    55,
      55,     3,    24,    55,    49,    49,     4,    48,     9,     4,
       4,    49,    49,     5,    48,    54,    49,    49,    49,    49,
      49,    49,    32,     6,    49,    49,    49,    -1,    49,    49,
      48,    21,    49,    49,    49,   157,   119,    79,   176
  };

  const signed char
  SdfParse::yystos_[] =
  {
       0,    48,    58,    10,     0,    48,    59,    60,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    48,
      60,    63,    64,     3,     3,     3,     3,     3,     3,    50,
      51,    61,     8,     9,    49,    56,    62,    97,    98,     3,
      49,    49,    97,    98,    98,    22,    48,    49,    64,    49,
      49,    49,    49,    49,    49,    49,     9,    55,    49,    49,
      55,    49,    49,    49,     4,    48,    65,    62,    98,    62,
      98,    49,    23,    48,    66,    55,    55,    55,     3,    24,
      67,    98,    62,    62,    49,     4,    49,    52,    74,    48,
      49,    68,    49,    49,    61,    25,    33,     4,    69,    79,
      48,    49,    70,    48,    49,    80,    26,    27,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    71,    72,
      81,    82,    83,    84,    85,    86,    88,    89,    87,    90,
      73,    73,     4,    48,    74,    91,    92,    93,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    48,    49,
      75,    49,    53,    44,    45,    46,    94,    61,    95,    95,
      95,    95,    95,    95,    48,    96,    96,    95,    95,    28,
      29,    30,    32,    46,    47,     9,     6,     7,    92,     4,
      91,    96,    96,    96,    96,    96,    96,    49,    97,    98,
      49,    49,    96,    96,    92,    92,    78,    92,    96,    48,
      93,     5,    48,    54,    94,    49,    49,    49,    96,    49,
      49,    96,    49,    49,    49,    96,    92,    78,    49,    96,
      78,    92,    93,    32,    92,    49,    49,    49,    78,    49,
      49,    76,    92,    93,    49,    49,    48,    77,    78,    76,
      92,    49,    31,    49,    78,    76,    78,    49,    78,    49,
      49,    49,    49
  };

  const signed char
  SdfParse::yyr1_[] =
  {
       0,    57,    58,    59,    59,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    61,    61,    62,    62,    63,    63,    64,    65,    66,
      66,    66,    67,    67,    68,    68,    69,    69,    71,    70,
      72,    70,    73,    73,    74,    74,    75,    75,    75,    75,
      75,    75,    75,    76,    76,    77,    78,    78,    79,    79,
      81,    80,    82,    80,    83,    80,    84,    80,    85,    80,
      86,    80,    87,    80,    88,    80,    89,    80,    90,    80,
      91,    91,    92,    92,    93,    93,    94,    94,    95,    95,
      95,    96,    96,    96,    97,    97,    97,    98,    98,    98
  };

  const signed char
  SdfParse::yyr2_[] =
  {
       0,     2,     5,     1,     2,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     3,     4,     3,     4,     4,     3,
       5,     1,     1,     0,     1,     1,     2,     6,     4,     3,
       4,     4,     0,     2,     4,     4,     0,     2,     0,     5,
       0,     5,     0,     2,     1,     3,     7,    10,     9,     6,
       5,     4,     5,     0,     2,     4,     1,     2,     0,     2,
       0,     7,     0,     7,     0,     8,     0,     7,     0,     7,
       0,     8,     0,     7,     0,     6,     0,     6,     0,     8,
       1,     4,     1,     3,     1,     4,     1,     1,     1,     3,
       7,     2,     3,     3,     5,     5,     5,     1,     1,     2
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const SdfParse::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "QSTRING", "ID",
  "EXPR_OPEN_IOPATH", "EXPR_OPEN", "EXPR_ID_CLOSE", "FNUMBER", "DNUMBER",
  "DELAYFILE", "SDFVERSION", "DESIGN", "DATE", "VENDOR", "PROGRAM",
  "PVERSION", "DIVIDER", "VOLTAGE", "PROCESS", "TEMPERATURE", "TIMESCALE",
  "CELL", "CELLTYPE", "INSTANCE", "DELAY", "ABSOLUTE", "INCREMENTAL",
  "INTERCONNECT", "PORT", "DEVICE", "RETAIN", "IOPATH", "TIMINGCHECK",
  "SETUP", "HOLD", "SETUPHOLD", "RECOVERY", "REMOVAL", "RECREM", "WIDTH",
  "PERIOD", "SKEW", "NOCHANGE", "POSEDGE", "NEGEDGE", "COND", "CONDELSE",
  "'('", "')'", "'/'", "'.'", "'*'", "'['", "']'", "':'", "'-'", "$accept",
  "file", "header", "header_stmt", "hchar", "number_opt", "cells", "cell",
  "celltype", "cell_instance", "timing_specs", "timing_spec", "deltypes",
  "deltype", "$@1", "$@2", "del_defs", "path", "del_def", "retains",
  "retain", "delval_list", "tchk_defs", "tchk_def", "$@3", "$@4", "$@5",
  "$@6", "$@7", "$@8", "$@9", "$@10", "$@11", "$@12", "port",
  "port_instance", "port_spec", "port_transition", "port_tchk", "value",
  "triple", "NUMBER", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  SdfParse::yyrline_[] =
  {
       0,   104,   104,   108,   109,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   133,   135,   139,   140,   144,   145,   149,   154,   159,
     161,   163,   167,   169,   173,   174,   177,   178,   183,   182,
     186,   185,   190,   191,   195,   197,   202,   204,   207,   210,
     212,   214,   216,   220,   222,   226,   231,   233,   237,   238,
     242,   242,   247,   247,   252,   252,   257,   257,   262,   262,
     267,   267,   272,   272,   278,   278,   283,   283,   288,   288,
     296,   298,   303,   305,   310,   312,   317,   318,   322,   324,
     326,   331,   335,   339,   343,   348,   353,   361,   363,   365
  };

  void
  SdfParse::yy_stack_print_ () const
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
  SdfParse::yy_reduce_print_ (int yyrule) const
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

  SdfParse::symbol_kind_type
  SdfParse::yytranslate_ (int t) YY_NOEXCEPT
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
      48,    49,    52,     2,     2,    56,    51,    50,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    55,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    53,     2,    54,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47
    };
    // Last valid token kind.
    const int code_max = 302;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 66 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"
} // sta
#line 1940 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/SdfParse.cc"

#line 369 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/sdf/SdfParse.yy"

