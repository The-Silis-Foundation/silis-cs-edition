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
#line 25 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"

#include <cstdlib>
#include <variant>
#include <string>
#include <utility>

#include "Report.hh"
#include "liberty/LibertyParser.hh"
#include "liberty/LibertyScanner.hh"

#undef yylex
#define yylex scanner->lex

// warning: variable 'yynerrs_' set but not used
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

#define loc_line(loc) loc.begin.line

void
sta::LibertyParse::error(const location_type &loc,
                         const std::string &msg)
{
  reader->report()->fileError(164, reader->filename(),
                              loc.begin.line, "{}", msg);
}


#line 69 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"


#include "LibertyParse.hh"




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

#line 56 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
namespace sta {
#line 167 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"

  /// Build a parser object.
  LibertyParse::LibertyParse (LibertyScanner *scanner_yyarg, LibertyParser *reader_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      scanner (scanner_yyarg),
      reader (reader_yyarg)
  {}

  LibertyParse::~LibertyParse ()
  {}

  LibertyParse::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  LibertyParse::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S_expr_op: // expr_op
        value.copy< char > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.copy< float > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_attr_value: // attr_value
        value.copy< sta::LibertyAttrValue * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_attr_values: // attr_values
        value.copy< sta::LibertyAttrValueSeq * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_KEYWORD: // KEYWORD
      case symbol_kind::S_string: // string
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_expr_term: // expr_term
      case symbol_kind::S_expr_term1: // expr_term1
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_file: // file
      case symbol_kind::S_group: // group
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_simple_attr: // simple_attr
      case symbol_kind::S_complex_attr: // complex_attr
      case symbol_kind::S_variable: // variable
        value.copy< void * > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }




  template <typename Base>
  LibertyParse::symbol_kind_type
  LibertyParse::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  LibertyParse::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  LibertyParse::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_expr_op: // expr_op
        value.move< char > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.move< float > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_attr_value: // attr_value
        value.move< sta::LibertyAttrValue * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_attr_values: // attr_values
        value.move< sta::LibertyAttrValueSeq * > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_KEYWORD: // KEYWORD
      case symbol_kind::S_string: // string
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_expr_term: // expr_term
      case symbol_kind::S_expr_term1: // expr_term1
        value.move< std::string > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_file: // file
      case symbol_kind::S_group: // group
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_simple_attr: // simple_attr
      case symbol_kind::S_complex_attr: // complex_attr
      case symbol_kind::S_variable: // variable
        value.move< void * > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  LibertyParse::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  LibertyParse::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  LibertyParse::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  LibertyParse::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  LibertyParse::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  LibertyParse::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  LibertyParse::symbol_kind_type
  LibertyParse::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  LibertyParse::symbol_kind_type
  LibertyParse::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  LibertyParse::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  LibertyParse::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  LibertyParse::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  LibertyParse::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  LibertyParse::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  LibertyParse::symbol_kind_type
  LibertyParse::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  LibertyParse::stack_symbol_type::stack_symbol_type ()
  {}

  LibertyParse::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_expr_op: // expr_op
        value.YY_MOVE_OR_COPY< char > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.YY_MOVE_OR_COPY< float > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_attr_value: // attr_value
        value.YY_MOVE_OR_COPY< sta::LibertyAttrValue * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_attr_values: // attr_values
        value.YY_MOVE_OR_COPY< sta::LibertyAttrValueSeq * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_KEYWORD: // KEYWORD
      case symbol_kind::S_string: // string
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_expr_term: // expr_term
      case symbol_kind::S_expr_term1: // expr_term1
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_file: // file
      case symbol_kind::S_group: // group
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_simple_attr: // simple_attr
      case symbol_kind::S_complex_attr: // complex_attr
      case symbol_kind::S_variable: // variable
        value.YY_MOVE_OR_COPY< void * > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  LibertyParse::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_expr_op: // expr_op
        value.move< char > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.move< float > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_attr_value: // attr_value
        value.move< sta::LibertyAttrValue * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_attr_values: // attr_values
        value.move< sta::LibertyAttrValueSeq * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_KEYWORD: // KEYWORD
      case symbol_kind::S_string: // string
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_expr_term: // expr_term
      case symbol_kind::S_expr_term1: // expr_term1
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_file: // file
      case symbol_kind::S_group: // group
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_simple_attr: // simple_attr
      case symbol_kind::S_complex_attr: // complex_attr
      case symbol_kind::S_variable: // variable
        value.move< void * > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  LibertyParse::stack_symbol_type&
  LibertyParse::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_expr_op: // expr_op
        value.copy< char > (that.value);
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.copy< float > (that.value);
        break;

      case symbol_kind::S_attr_value: // attr_value
        value.copy< sta::LibertyAttrValue * > (that.value);
        break;

      case symbol_kind::S_attr_values: // attr_values
        value.copy< sta::LibertyAttrValueSeq * > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_KEYWORD: // KEYWORD
      case symbol_kind::S_string: // string
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_expr_term: // expr_term
      case symbol_kind::S_expr_term1: // expr_term1
        value.copy< std::string > (that.value);
        break;

      case symbol_kind::S_file: // file
      case symbol_kind::S_group: // group
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_simple_attr: // simple_attr
      case symbol_kind::S_complex_attr: // complex_attr
      case symbol_kind::S_variable: // variable
        value.copy< void * > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  LibertyParse::stack_symbol_type&
  LibertyParse::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_expr_op: // expr_op
        value.move< char > (that.value);
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        value.move< float > (that.value);
        break;

      case symbol_kind::S_attr_value: // attr_value
        value.move< sta::LibertyAttrValue * > (that.value);
        break;

      case symbol_kind::S_attr_values: // attr_values
        value.move< sta::LibertyAttrValueSeq * > (that.value);
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_KEYWORD: // KEYWORD
      case symbol_kind::S_string: // string
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_expr_term: // expr_term
      case symbol_kind::S_expr_term1: // expr_term1
        value.move< std::string > (that.value);
        break;

      case symbol_kind::S_file: // file
      case symbol_kind::S_group: // group
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_simple_attr: // simple_attr
      case symbol_kind::S_complex_attr: // complex_attr
      case symbol_kind::S_variable: // variable
        value.move< void * > (that.value);
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
  LibertyParse::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  LibertyParse::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
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
  LibertyParse::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  LibertyParse::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  LibertyParse::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  LibertyParse::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  LibertyParse::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  LibertyParse::debug_level_type
  LibertyParse::debug_level () const
  {
    return yydebug_;
  }

  void
  LibertyParse::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  LibertyParse::state_type
  LibertyParse::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  LibertyParse::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  LibertyParse::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  LibertyParse::operator() ()
  {
    return parse ();
  }

  int
  LibertyParse::parse ()
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
      case symbol_kind::S_expr_op: // expr_op
        yylhs.value.emplace< char > ();
        break;

      case symbol_kind::S_FLOAT: // FLOAT
        yylhs.value.emplace< float > ();
        break;

      case symbol_kind::S_attr_value: // attr_value
        yylhs.value.emplace< sta::LibertyAttrValue * > ();
        break;

      case symbol_kind::S_attr_values: // attr_values
        yylhs.value.emplace< sta::LibertyAttrValueSeq * > ();
        break;

      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_KEYWORD: // KEYWORD
      case symbol_kind::S_string: // string
      case symbol_kind::S_expr: // expr
      case symbol_kind::S_expr_term: // expr_term
      case symbol_kind::S_expr_term1: // expr_term1
        yylhs.value.emplace< std::string > ();
        break;

      case symbol_kind::S_file: // file
      case symbol_kind::S_group: // group
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_simple_attr: // simple_attr
      case symbol_kind::S_complex_attr: // complex_attr
      case symbol_kind::S_variable: // variable
        yylhs.value.emplace< void * > ();
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
  case 2: // file: group
#line 86 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < void * > () = yystack_[0].value.as < void * > (); }
#line 895 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 3: // $@1: %empty
#line 91 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { reader->groupBegin(std::move(yystack_[3].value.as < std::string > ()), nullptr, loc_line(yystack_[3].location)); }
#line 901 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 4: // group: KEYWORD '(' ')' '{' $@1 '}' semi_opt
#line 93 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < void * > () = reader->groupEnd(); }
#line 907 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 5: // $@2: %empty
#line 95 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { reader->groupBegin(std::move(yystack_[3].value.as < std::string > ()), nullptr, loc_line(yystack_[3].location)); }
#line 913 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 6: // group: KEYWORD '(' ')' '{' $@2 statements '}' semi_opt
#line 97 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < void * > () = reader->groupEnd(); }
#line 919 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 7: // $@3: %empty
#line 99 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { reader->groupBegin(std::move(yystack_[4].value.as < std::string > ()), yystack_[2].value.as < sta::LibertyAttrValueSeq * > (), loc_line(yystack_[4].location)); }
#line 925 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 8: // group: KEYWORD '(' attr_values ')' '{' $@3 '}' semi_opt
#line 101 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < void * > () = reader->groupEnd(); }
#line 931 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 9: // $@4: %empty
#line 103 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { reader->groupBegin(std::move(yystack_[4].value.as < std::string > ()), yystack_[2].value.as < sta::LibertyAttrValueSeq * > (), loc_line(yystack_[4].location)); }
#line 937 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 10: // group: KEYWORD '(' attr_values ')' '{' $@4 statements '}' semi_opt
#line 105 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < void * > () = reader->groupEnd(); }
#line 943 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 13: // statement: simple_attr
#line 114 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < void * > () = yystack_[0].value.as < void * > (); }
#line 949 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 14: // statement: complex_attr
#line 115 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < void * > () = yystack_[0].value.as < void * > (); }
#line 955 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 15: // statement: group
#line 116 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < void * > () = yystack_[0].value.as < void * > (); }
#line 961 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 16: // statement: variable
#line 117 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < void * > () = yystack_[0].value.as < void * > (); }
#line 967 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 17: // simple_attr: KEYWORD ':' attr_value semi_opt
#line 122 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < void * > () = reader->makeSimpleAttr(std::move(yystack_[3].value.as < std::string > ()), yystack_[1].value.as < sta::LibertyAttrValue * > (), loc_line(yystack_[3].location)); }
#line 973 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 18: // complex_attr: KEYWORD '(' ')' semi_opt
#line 127 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < void * > () = reader->makeComplexAttr(std::move(yystack_[3].value.as < std::string > ()), nullptr, loc_line(yystack_[3].location)); }
#line 979 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 19: // complex_attr: KEYWORD '(' attr_values ')' semi_opt
#line 129 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < void * > () = reader->makeComplexAttr(std::move(yystack_[4].value.as < std::string > ()), yystack_[2].value.as < sta::LibertyAttrValueSeq * > (), loc_line(yystack_[4].location)); }
#line 985 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 20: // attr_values: attr_value
#line 134 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < sta::LibertyAttrValueSeq * > () = new sta::LibertyAttrValueSeq;
	  yylhs.value.as < sta::LibertyAttrValueSeq * > ()->push_back(yystack_[0].value.as < sta::LibertyAttrValue * > ());
	}
#line 993 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 21: // attr_values: attr_values ',' attr_value
#line 138 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yystack_[2].value.as < sta::LibertyAttrValueSeq * > ()->push_back(yystack_[0].value.as < sta::LibertyAttrValue * > ());
	  yylhs.value.as < sta::LibertyAttrValueSeq * > () = yystack_[2].value.as < sta::LibertyAttrValueSeq * > ();
	}
#line 1001 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 22: // attr_values: attr_values attr_value
#line 142 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yystack_[1].value.as < sta::LibertyAttrValueSeq * > ()->push_back(yystack_[0].value.as < sta::LibertyAttrValue * > ());
	  yylhs.value.as < sta::LibertyAttrValueSeq * > () = yystack_[1].value.as < sta::LibertyAttrValueSeq * > ();
	}
#line 1009 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 23: // variable: string '=' FLOAT semi_opt
#line 149 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < void * > () = reader->makeVariable(std::move(yystack_[3].value.as < std::string > ()), yystack_[1].value.as < float > (), loc_line(yystack_[3].location)); }
#line 1015 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 24: // string: STRING
#line 154 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1021 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 25: // string: KEYWORD
#line 156 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1027 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 26: // attr_value: expr
#line 161 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < sta::LibertyAttrValue * > () = reader->makeAttrValueString(std::move(yystack_[0].value.as < std::string > ())); }
#line 1033 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 27: // expr: expr_term1
#line 165 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1039 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 28: // expr: expr_term1 expr_op expr
#line 167 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < std::string > () = sta::format("{}{}{}", yystack_[2].value.as < std::string > (), yystack_[1].value.as < char > (), yystack_[0].value.as < std::string > ()); }
#line 1045 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 29: // expr_term: string
#line 171 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1051 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 30: // expr_term: FLOAT
#line 173 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < std::string > () = sta::format("{}", yystack_[0].value.as < float > ()); }
#line 1057 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 31: // expr_term: '0'
#line 175 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < std::string > () = std::string("0"); }
#line 1063 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 32: // expr_term: '1'
#line 177 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < std::string > () = std::string("1"); }
#line 1069 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 33: // expr_term: '(' expr ')'
#line 179 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < std::string > () = "(" + yystack_[1].value.as < std::string > () + ")"; }
#line 1075 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 34: // expr_term1: expr_term
#line 183 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1081 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 35: // expr_term1: '!' expr_term
#line 185 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < std::string > () = "!" + yystack_[0].value.as < std::string > (); }
#line 1087 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 36: // expr_term1: expr_term '\''
#line 187 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + "'"; }
#line 1093 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 37: // expr_op: '+'
#line 192 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < char > () = '+'; }
#line 1099 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 38: // expr_op: '|'
#line 194 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < char > () = '|'; }
#line 1105 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 39: // expr_op: '*'
#line 196 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < char > () = '*'; }
#line 1111 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 40: // expr_op: '&'
#line 198 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < char > () = '&'; }
#line 1117 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 41: // expr_op: '^'
#line 200 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < char > () = '^'; }
#line 1123 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 42: // expr_op: '-'
#line 202 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < char > () = '-'; }
#line 1129 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;

  case 43: // expr_op: '/'
#line 204 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
        { yylhs.value.as < char > () = '/'; }
#line 1135 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"
    break;


#line 1139 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"

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
  LibertyParse::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

#if YYDEBUG || 0
  const char *
  LibertyParse::symbol_name (symbol_kind_type yysymbol)
  {
    return yytname_[yysymbol];
  }
#endif // #if YYDEBUG || 0









  const signed char LibertyParse::yypact_ninf_ = -37;

  const signed char LibertyParse::yytable_ninf_ = -8;

  const signed char
  LibertyParse::yypact_[] =
  {
      26,    18,    35,   -37,    46,   -37,   -37,   -37,   -37,    72,
      52,    25,   -37,   -37,     0,   -37,   -37,   -37,    11,    73,
     -37,    24,    29,    27,    52,   -37,   -37,   -37,   -37,   -37,
     -37,   -37,   -37,   -37,    52,   -37,    30,     8,    41,   -37,
     -37,   -37,    15,   -37,     3,   -37,   -37,   -37,   -37,    58,
      55,     8,    45,    49,    52,   -37,   -37,    82,   -37,    14,
     -37,    25,    23,   -37,    45,   -37,    45,   -37,    45,    27,
      45,    45,    45,    45
  };

  const signed char
  LibertyParse::yydefact_[] =
  {
       0,     0,     0,     2,     0,     1,    24,    25,    30,     0,
       0,     0,    31,    32,     0,    29,    20,    26,    34,    27,
      35,     0,     5,     0,     0,    22,    36,    37,    42,    38,
      39,    43,    40,    41,     0,    33,     0,     0,     9,    21,
      28,    44,    25,    15,     0,    11,    13,    14,    16,     0,
       0,     0,     4,     0,     0,    44,    12,     0,    44,     0,
      45,    44,     0,    44,     6,    44,     8,    44,    18,    44,
      17,    23,    10,    19
  };

  const signed char
  LibertyParse::yypgoto_[] =
  {
     -37,   -37,    88,   -37,   -37,   -37,   -37,    39,   -36,   -37,
     -37,    38,   -37,   -35,   -14,    -9,    87,   -37,   -37,    34
  };

  const signed char
  LibertyParse::yydefgoto_[] =
  {
       0,     2,    43,    36,    37,    50,    51,    44,    45,    46,
      47,    14,    48,    15,    16,    17,    18,    19,    34,    52
  };

  const signed char
  LibertyParse::yytable_[] =
  {
      25,    21,    49,     6,     7,     8,     6,    42,    56,    49,
      39,     6,    42,     9,    10,    23,    49,     6,    42,    24,
      55,    12,    13,    56,    49,    40,     6,     7,     8,    53,
       1,    67,     4,    54,    26,     5,     9,    10,    69,    35,
      63,    22,    24,    38,    12,    13,    -3,    41,    25,     6,
       7,     8,     6,     7,     8,     6,     7,     8,    -7,     9,
      10,    11,     9,    10,    61,     9,    10,    12,    13,    60,
      12,    13,    58,    12,    13,     6,     7,     8,    57,    27,
      28,    29,    30,    31,    32,    33,    10,    65,     3,    64,
      59,    62,    66,    12,    13,    68,    20,    70,     0,    71,
       0,    72,     0,    73
  };

  const signed char
  LibertyParse::yycheck_[] =
  {
      14,    10,    37,     3,     4,     5,     3,     4,    44,    44,
      24,     3,     4,    13,    14,    15,    51,     3,     4,    19,
      17,    21,    22,    59,    59,    34,     3,     4,     5,    14,
       4,    17,    14,    18,    23,     0,    13,    14,    15,    15,
      54,    16,    19,    16,    21,    22,    17,    17,    62,     3,
       4,     5,     3,     4,     5,     3,     4,     5,    17,    13,
      14,    15,    13,    14,    15,    13,    14,    21,    22,    24,
      21,    22,    17,    21,    22,     3,     4,     5,    20,     6,
       7,     8,     9,    10,    11,    12,    14,     5,     0,    55,
      51,    53,    58,    21,    22,    61,     9,    63,    -1,    65,
      -1,    67,    -1,    69
  };

  const signed char
  LibertyParse::yystos_[] =
  {
       0,     4,    26,    27,    14,     0,     3,     4,     5,    13,
      14,    15,    21,    22,    36,    38,    39,    40,    41,    42,
      41,    40,    16,    15,    19,    39,    23,     6,     7,     8,
       9,    10,    11,    12,    43,    15,    28,    29,    16,    39,
      40,    17,     4,    27,    32,    33,    34,    35,    37,    38,
      30,    31,    44,    14,    18,    17,    33,    20,    17,    32,
      24,    15,    36,    39,    44,     5,    44,    17,    44,    15,
      44,    44,    44,    44
  };

  const signed char
  LibertyParse::yyr1_[] =
  {
       0,    25,    26,    28,    27,    29,    27,    30,    27,    31,
      27,    32,    32,    33,    33,    33,    33,    34,    35,    35,
      36,    36,    36,    37,    38,    38,    39,    40,    40,    41,
      41,    41,    41,    41,    42,    42,    42,    43,    43,    43,
      43,    43,    43,    43,    44,    44
  };

  const signed char
  LibertyParse::yyr2_[] =
  {
       0,     2,     1,     0,     7,     0,     8,     0,     8,     0,
       9,     1,     2,     1,     1,     1,     1,     4,     4,     5,
       1,     3,     2,     4,     1,     1,     1,     1,     3,     1,
       1,     1,     1,     3,     1,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     0,     2
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const LibertyParse::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "STRING", "KEYWORD",
  "FLOAT", "'+'", "'-'", "'|'", "'*'", "'/'", "'&'", "'^'", "'!'", "'('",
  "')'", "'{'", "'}'", "':'", "','", "'='", "'0'", "'1'", "'\\''", "';'",
  "$accept", "file", "group", "$@1", "$@2", "$@3", "$@4", "statements",
  "statement", "simple_attr", "complex_attr", "attr_values", "variable",
  "string", "attr_value", "expr", "expr_term", "expr_term1", "expr_op",
  "semi_opt", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const unsigned char
  LibertyParse::yyrline_[] =
  {
       0,    86,    86,    91,    90,    95,    94,    99,    98,   103,
     102,   109,   110,   114,   115,   116,   117,   121,   126,   128,
     133,   137,   141,   148,   153,   155,   160,   165,   166,   171,
     172,   174,   176,   178,   183,   184,   186,   191,   193,   195,
     197,   199,   201,   203,   207,   209
  };

  void
  LibertyParse::yy_stack_print_ () const
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
  LibertyParse::yy_reduce_print_ (int yyrule) const
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

  LibertyParse::symbol_kind_type
  LibertyParse::yytranslate_ (int t) YY_NOEXCEPT
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
       2,     2,     2,    13,     2,     2,     2,     2,    11,    23,
      14,    15,     9,     6,    19,     7,     2,    10,    21,    22,
       2,     2,     2,     2,     2,     2,     2,     2,    18,    24,
       2,    20,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    12,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    16,     8,    17,     2,     2,     2,     2,
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
       5
    };
    // Last valid token kind.
    const int code_max = 260;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 56 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"
} // sta
#line 1547 "/home/eatheswar/silisfoundation/silis/synthengine/build/OpenSTA_build/LibertyParse.cc"

#line 212 "/home/eatheswar/silisfoundation/silis/third-party/OpenSTA/liberty/LibertyParse.yy"

