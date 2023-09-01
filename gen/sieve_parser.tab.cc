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





#include "sieve_parser.tab.hh"


// Unqualified %code blocks.
#line 35 "../src/sieve_parser.yy"

#include "sieve_driver.hh"
#include "sieve_scanner.hh"

#line 51 "../gen/sieve_parser.tab.cc"


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

namespace yy {
#line 143 "../gen/sieve_parser.tab.cc"

  /// Build a parser object.
  sieve_parser::sieve_parser (yyscan_t scanner_yyarg, sieve::driver &driver_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      scanner (scanner_yyarg),
      driver (driver_yyarg)
  {}

  sieve_parser::~sieve_parser ()
  {}

  sieve_parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/



  // by_state.
  sieve_parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  sieve_parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  sieve_parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  sieve_parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  sieve_parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  sieve_parser::symbol_kind_type
  sieve_parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  sieve_parser::stack_symbol_type::stack_symbol_type ()
  {}

  sieve_parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
        value.YY_MOVE_OR_COPY< bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUMBER: // "number"
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_command: // command
      case symbol_kind::S_block: // block
      case symbol_kind::S_if_flow: // if_flow
        value.YY_MOVE_OR_COPY< sieve::ASTNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_numeric: // numeric
        value.YY_MOVE_OR_COPY< sieve::ASTNumeric * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_sieve: // sieve
        value.YY_MOVE_OR_COPY< sieve::ASTSieve * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDENTIFIER: // "identifier"
      case symbol_kind::S_FOREVERYPART: // "foreverypart"
      case symbol_kind::S_TAG: // ":tag"
      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_command_list: // command_list
      case symbol_kind::S_arguments: // arguments
      case symbol_kind::S_argument: // argument
      case symbol_kind::S_test_list: // test_list
      case symbol_kind::S_tests: // tests
      case symbol_kind::S_test: // test
      case symbol_kind::S_string_list: // string_list
      case symbol_kind::S_strings: // strings
        value.YY_MOVE_OR_COPY< std::vector<sieve::ASTNode *> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  sieve_parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
        value.move< bool > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUMBER: // "number"
        value.move< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_command: // command
      case symbol_kind::S_block: // block
      case symbol_kind::S_if_flow: // if_flow
        value.move< sieve::ASTNode * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_numeric: // numeric
        value.move< sieve::ASTNumeric * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_sieve: // sieve
        value.move< sieve::ASTSieve * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDENTIFIER: // "identifier"
      case symbol_kind::S_FOREVERYPART: // "foreverypart"
      case symbol_kind::S_TAG: // ":tag"
      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_command_list: // command_list
      case symbol_kind::S_arguments: // arguments
      case symbol_kind::S_argument: // argument
      case symbol_kind::S_test_list: // test_list
      case symbol_kind::S_tests: // tests
      case symbol_kind::S_test: // test
      case symbol_kind::S_string_list: // string_list
      case symbol_kind::S_strings: // strings
        value.move< std::vector<sieve::ASTNode *> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  sieve_parser::stack_symbol_type&
  sieve_parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
        value.copy< bool > (that.value);
        break;

      case symbol_kind::S_NUMBER: // "number"
        value.copy< int > (that.value);
        break;

      case symbol_kind::S_command: // command
      case symbol_kind::S_block: // block
      case symbol_kind::S_if_flow: // if_flow
        value.copy< sieve::ASTNode * > (that.value);
        break;

      case symbol_kind::S_numeric: // numeric
        value.copy< sieve::ASTNumeric * > (that.value);
        break;

      case symbol_kind::S_sieve: // sieve
        value.copy< sieve::ASTSieve * > (that.value);
        break;

      case symbol_kind::S_IDENTIFIER: // "identifier"
      case symbol_kind::S_FOREVERYPART: // "foreverypart"
      case symbol_kind::S_TAG: // ":tag"
      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
        value.copy< std::string > (that.value);
        break;

      case symbol_kind::S_command_list: // command_list
      case symbol_kind::S_arguments: // arguments
      case symbol_kind::S_argument: // argument
      case symbol_kind::S_test_list: // test_list
      case symbol_kind::S_tests: // tests
      case symbol_kind::S_test: // test
      case symbol_kind::S_string_list: // string_list
      case symbol_kind::S_strings: // strings
        value.copy< std::vector<sieve::ASTNode *> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  sieve_parser::stack_symbol_type&
  sieve_parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
        value.move< bool > (that.value);
        break;

      case symbol_kind::S_NUMBER: // "number"
        value.move< int > (that.value);
        break;

      case symbol_kind::S_command: // command
      case symbol_kind::S_block: // block
      case symbol_kind::S_if_flow: // if_flow
        value.move< sieve::ASTNode * > (that.value);
        break;

      case symbol_kind::S_numeric: // numeric
        value.move< sieve::ASTNumeric * > (that.value);
        break;

      case symbol_kind::S_sieve: // sieve
        value.move< sieve::ASTSieve * > (that.value);
        break;

      case symbol_kind::S_IDENTIFIER: // "identifier"
      case symbol_kind::S_FOREVERYPART: // "foreverypart"
      case symbol_kind::S_TAG: // ":tag"
      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
        value.move< std::string > (that.value);
        break;

      case symbol_kind::S_command_list: // command_list
      case symbol_kind::S_arguments: // arguments
      case symbol_kind::S_argument: // argument
      case symbol_kind::S_test_list: // test_list
      case symbol_kind::S_tests: // tests
      case symbol_kind::S_test: // test
      case symbol_kind::S_string_list: // string_list
      case symbol_kind::S_strings: // strings
        value.move< std::vector<sieve::ASTNode *> > (that.value);
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
  sieve_parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  sieve_parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
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
  sieve_parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  sieve_parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  sieve_parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  sieve_parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  sieve_parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  sieve_parser::debug_level_type
  sieve_parser::debug_level () const
  {
    return yydebug_;
  }

  void
  sieve_parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  sieve_parser::state_type
  sieve_parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  sieve_parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  sieve_parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  sieve_parser::operator() ()
  {
    return parse ();
  }

  int
  sieve_parser::parse ()
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


    // User initialization code.
#line 28 "../src/sieve_parser.yy"
{
    yyla.location.begin.filename = yyla.location.end.filename = &driver.filepath;
}

#line 573 "../gen/sieve_parser.tab.cc"


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
            symbol_type yylookahead (yylex (scanner, driver));
            yyla.move (yylookahead);
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
      case symbol_kind::S_TRUE: // "true"
      case symbol_kind::S_FALSE: // "false"
        yylhs.value.emplace< bool > ();
        break;

      case symbol_kind::S_NUMBER: // "number"
        yylhs.value.emplace< int > ();
        break;

      case symbol_kind::S_command: // command
      case symbol_kind::S_block: // block
      case symbol_kind::S_if_flow: // if_flow
        yylhs.value.emplace< sieve::ASTNode * > ();
        break;

      case symbol_kind::S_numeric: // numeric
        yylhs.value.emplace< sieve::ASTNumeric * > ();
        break;

      case symbol_kind::S_sieve: // sieve
        yylhs.value.emplace< sieve::ASTSieve * > ();
        break;

      case symbol_kind::S_IDENTIFIER: // "identifier"
      case symbol_kind::S_FOREVERYPART: // "foreverypart"
      case symbol_kind::S_TAG: // ":tag"
      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
        yylhs.value.emplace< std::string > ();
        break;

      case symbol_kind::S_command_list: // command_list
      case symbol_kind::S_arguments: // arguments
      case symbol_kind::S_argument: // argument
      case symbol_kind::S_test_list: // test_list
      case symbol_kind::S_tests: // tests
      case symbol_kind::S_test: // test
      case symbol_kind::S_string_list: // string_list
      case symbol_kind::S_strings: // strings
        yylhs.value.emplace< std::vector<sieve::ASTNode *> > ();
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
  case 2: // sieve: "end of file"
#line 89 "../src/sieve_parser.yy"
        {
            sieve::ASTSieve *sieve = new sieve::ASTSieve(yystack_[0].location);
            driver.syntax_tree(sieve);
            yylhs.value.as < sieve::ASTSieve * > () = sieve;
        }
#line 756 "../gen/sieve_parser.tab.cc"
    break;

  case 3: // sieve: command_list
#line 95 "../src/sieve_parser.yy"
        {
            sieve::ASTSieve *sieve = new sieve::ASTSieve(yystack_[0].location);
            sieve->push(yystack_[0].value.as < std::vector<sieve::ASTNode *> > ());
            driver.syntax_tree(sieve);
            yylhs.value.as < sieve::ASTSieve * > () = sieve;
        }
#line 767 "../gen/sieve_parser.tab.cc"
    break;

  case 4: // command_list: command
#line 103 "../src/sieve_parser.yy"
                       { yylhs.value.as < std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>(1, yystack_[0].value.as < sieve::ASTNode * > ()); }
#line 773 "../gen/sieve_parser.tab.cc"
    break;

  case 5: // command_list: command_list command
#line 104 "../src/sieve_parser.yy"
                           { yystack_[1].value.as < std::vector<sieve::ASTNode *> > ().push_back(yystack_[0].value.as < sieve::ASTNode * > ()); yylhs.value.as < std::vector<sieve::ASTNode *> > () = yystack_[1].value.as < std::vector<sieve::ASTNode *> > (); }
#line 779 "../gen/sieve_parser.tab.cc"
    break;

  case 6: // command: "require" string_list ";"
#line 109 "../src/sieve_parser.yy"
        {
            sieve::ASTRequire *require = new sieve::ASTRequire(yystack_[2].location);
            if (yystack_[1].value.as < std::vector<sieve::ASTNode *> > ().size() > 1) {
                sieve::ASTStringList *stringList = new sieve::ASTStringList(yystack_[1].location);
                stringList->push(yystack_[1].value.as < std::vector<sieve::ASTNode *> > ());
                require->push(stringList);
            } else {
                require->push(yystack_[1].value.as < std::vector<sieve::ASTNode *> > ());
            }
            yylhs.value.as < sieve::ASTNode * > () = dynamic_cast<sieve::ASTNode *>(require);
        }
#line 795 "../gen/sieve_parser.tab.cc"
    break;

  case 7: // command: "identifier" arguments ";"
#line 121 "../src/sieve_parser.yy"
        {
            sieve::ASTCommand *command = new sieve::ASTCommand(yystack_[2].location, yystack_[2].value.as < std::string > ());
            command->push(yystack_[1].value.as < std::vector<sieve::ASTNode *> > ());
            yylhs.value.as < sieve::ASTNode * > () = dynamic_cast<sieve::ASTNode *>(command);
        }
#line 805 "../gen/sieve_parser.tab.cc"
    break;

  case 8: // command: "identifier" ";"
#line 127 "../src/sieve_parser.yy"
        {
            sieve::ASTCommand *command = new sieve::ASTCommand(yystack_[1].location, yystack_[1].value.as < std::string > ());
            yylhs.value.as < sieve::ASTNode * > () = dynamic_cast<sieve::ASTNode *>(command);
        }
#line 814 "../gen/sieve_parser.tab.cc"
    break;

  case 9: // command: "foreverypart" block
#line 132 "../src/sieve_parser.yy"
        {
            sieve::ASTCommand *command = new sieve::ASTCommand(yystack_[1].location, yystack_[1].value.as < std::string > ());
            command->push(yystack_[0].value.as < sieve::ASTNode * > ());
            yylhs.value.as < sieve::ASTNode * > () = dynamic_cast<sieve::ASTNode *>(command);
        }
#line 824 "../gen/sieve_parser.tab.cc"
    break;

  case 10: // command: "foreverypart" ":tag" STRING_LITERAL block
#line 138 "../src/sieve_parser.yy"
        {
            sieve::ASTCommand *command = new sieve::ASTCommand(yystack_[3].location, yystack_[3].value.as < std::string > ());
            command->push(new sieve::ASTTag(yystack_[2].location, yystack_[2].value.as < std::string > ()));
            command->push(new sieve::ASTString(yystack_[1].location, yystack_[1].value.as < std::string > ()));
            command->push(yystack_[0].value.as < sieve::ASTNode * > ());
            yylhs.value.as < sieve::ASTNode * > () = dynamic_cast<sieve::ASTNode *>(command);
        }
#line 836 "../gen/sieve_parser.tab.cc"
    break;

  case 11: // command: if_flow
#line 145 "../src/sieve_parser.yy"
              { yylhs.value.as < sieve::ASTNode * > () = yystack_[0].value.as < sieve::ASTNode * > (); }
#line 842 "../gen/sieve_parser.tab.cc"
    break;

  case 12: // command: if_flow "else" block
#line 147 "../src/sieve_parser.yy"
        {
            yystack_[2].value.as < sieve::ASTNode * > ()->push(yystack_[0].value.as < sieve::ASTNode * > ());
            yylhs.value.as < sieve::ASTNode * > () = yystack_[2].value.as < sieve::ASTNode * > ();
        }
#line 851 "../gen/sieve_parser.tab.cc"
    break;

  case 13: // block: "{" command_list "}"
#line 154 "../src/sieve_parser.yy"
        {
            sieve::ASTBlock *block = new sieve::ASTBlock( yystack_[2].location );
            block->push(yystack_[1].value.as < std::vector<sieve::ASTNode *> > ());
            yylhs.value.as < sieve::ASTNode * > () = dynamic_cast<sieve::ASTNode *>( block );
        }
#line 861 "../gen/sieve_parser.tab.cc"
    break;

  case 14: // block: "{" "}"
#line 160 "../src/sieve_parser.yy"
        {
            sieve::ASTNoOp *noop = new sieve::ASTNoOp( yystack_[1].location );
            yylhs.value.as < sieve::ASTNode * > () = noop;
        }
#line 870 "../gen/sieve_parser.tab.cc"
    break;

  case 15: // if_flow: "if" test block
#line 167 "../src/sieve_parser.yy"
        {
            sieve::ASTBranch *branch = new sieve::ASTBranch( yystack_[2].location );
            sieve::ASTCondition *condition = new sieve::ASTCondition( yystack_[1].location );
            condition->push(yystack_[1].value.as < std::vector<sieve::ASTNode *> > ());
            branch->push(condition);
            branch->push(yystack_[0].value.as < sieve::ASTNode * > ());
            yylhs.value.as < sieve::ASTNode * > () = dynamic_cast<sieve::ASTNode *>(branch);

        }
#line 884 "../gen/sieve_parser.tab.cc"
    break;

  case 16: // if_flow: "if" "(" test ")" block
#line 177 "../src/sieve_parser.yy"
        {
            sieve::ASTBranch *branch = new sieve::ASTBranch( yystack_[4].location );
            sieve::ASTCondition *condition = new sieve::ASTCondition( yystack_[2].location );
            condition->push(yystack_[2].value.as < std::vector<sieve::ASTNode *> > ());
            branch->push(condition);
            branch->push(yystack_[0].value.as < sieve::ASTNode * > ());
            yylhs.value.as < sieve::ASTNode * > () = dynamic_cast<sieve::ASTNode *>(branch);

        }
#line 898 "../gen/sieve_parser.tab.cc"
    break;

  case 17: // if_flow: if_flow "elsif" test block
#line 187 "../src/sieve_parser.yy"
        {
            sieve::ASTCondition *condition = new sieve::ASTCondition( yystack_[1].location );
            condition->push(yystack_[1].value.as < std::vector<sieve::ASTNode *> > ());
            yystack_[3].value.as < sieve::ASTNode * > ()->push(condition);
            yystack_[3].value.as < sieve::ASTNode * > ()->push(yystack_[0].value.as < sieve::ASTNode * > ());
            yylhs.value.as < sieve::ASTNode * > () = yystack_[3].value.as < sieve::ASTNode * > ();
        }
#line 910 "../gen/sieve_parser.tab.cc"
    break;

  case 18: // arguments: argument
#line 196 "../src/sieve_parser.yy"
                     { yylhs.value.as < std::vector<sieve::ASTNode *> > () = yystack_[0].value.as < std::vector<sieve::ASTNode *> > (); }
#line 916 "../gen/sieve_parser.tab.cc"
    break;

  case 19: // arguments: argument test
#line 197 "../src/sieve_parser.yy"
                    { yystack_[1].value.as < std::vector<sieve::ASTNode *> > ().insert(yystack_[1].value.as < std::vector<sieve::ASTNode *> > ().end(), yystack_[0].value.as < std::vector<sieve::ASTNode *> > ().begin(), yystack_[0].value.as < std::vector<sieve::ASTNode *> > ().end()); yylhs.value.as < std::vector<sieve::ASTNode *> > () = yystack_[1].value.as < std::vector<sieve::ASTNode *> > (); }
#line 922 "../gen/sieve_parser.tab.cc"
    break;

  case 20: // arguments: argument test_list
#line 198 "../src/sieve_parser.yy"
                         { yystack_[1].value.as < std::vector<sieve::ASTNode *> > ().insert(yystack_[1].value.as < std::vector<sieve::ASTNode *> > ().end(), yystack_[0].value.as < std::vector<sieve::ASTNode *> > ().begin(), yystack_[0].value.as < std::vector<sieve::ASTNode *> > ().end()); yylhs.value.as < std::vector<sieve::ASTNode *> > () = yystack_[1].value.as < std::vector<sieve::ASTNode *> > (); }
#line 928 "../gen/sieve_parser.tab.cc"
    break;

  case 21: // arguments: arguments argument
#line 199 "../src/sieve_parser.yy"
                         { yystack_[1].value.as < std::vector<sieve::ASTNode *> > ().insert(yystack_[1].value.as < std::vector<sieve::ASTNode *> > ().end(), yystack_[0].value.as < std::vector<sieve::ASTNode *> > ().begin(), yystack_[0].value.as < std::vector<sieve::ASTNode *> > ().end()); yylhs.value.as < std::vector<sieve::ASTNode *> > () = yystack_[1].value.as < std::vector<sieve::ASTNode *> > (); }
#line 934 "../gen/sieve_parser.tab.cc"
    break;

  case 22: // arguments: test
#line 200 "../src/sieve_parser.yy"
           { yylhs.value.as < std::vector<sieve::ASTNode *> > () = yystack_[0].value.as < std::vector<sieve::ASTNode *> > (); }
#line 940 "../gen/sieve_parser.tab.cc"
    break;

  case 23: // arguments: test_list
#line 201 "../src/sieve_parser.yy"
                { yylhs.value.as < std::vector<sieve::ASTNode *> > () = yystack_[0].value.as < std::vector<sieve::ASTNode *> > (); }
#line 946 "../gen/sieve_parser.tab.cc"
    break;

  case 24: // argument: string_list
#line 205 "../src/sieve_parser.yy"
        {
            if (yystack_[0].value.as < std::vector<sieve::ASTNode *> > ().size() > 1) {
                sieve::ASTStringList *stringList = new sieve::ASTStringList(yystack_[0].location);
                stringList->push(yystack_[0].value.as < std::vector<sieve::ASTNode *> > ());
                yylhs.value.as < std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>( 1, dynamic_cast<sieve::ASTNode *>(stringList));
            } else {
                yylhs.value.as < std::vector<sieve::ASTNode *> > () = yystack_[0].value.as < std::vector<sieve::ASTNode *> > ();
            }
        }
#line 960 "../gen/sieve_parser.tab.cc"
    break;

  case 25: // argument: numeric
#line 215 "../src/sieve_parser.yy"
        {
            yylhs.value.as < std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>( 1, dynamic_cast<sieve::ASTNode *>(yystack_[0].value.as < sieve::ASTNumeric * > ()));
        }
#line 968 "../gen/sieve_parser.tab.cc"
    break;

  case 26: // argument: ":tag"
#line 219 "../src/sieve_parser.yy"
        {
            sieve::ASTTag *tag = new sieve::ASTTag(yystack_[0].location, yystack_[0].value.as < std::string > ());
            yylhs.value.as < std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>( 1, dynamic_cast<sieve::ASTNode *>(tag));
        }
#line 977 "../gen/sieve_parser.tab.cc"
    break;

  case 27: // test_list: "(" tests ")"
#line 226 "../src/sieve_parser.yy"
    {
        sieve::ASTTestList *testList = new sieve::ASTTestList( yystack_[2].location );
        testList->push(yystack_[1].value.as < std::vector<sieve::ASTNode *> > ());
        yylhs.value.as < std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>(1, testList);
    }
#line 987 "../gen/sieve_parser.tab.cc"
    break;

  case 28: // tests: test
#line 233 "../src/sieve_parser.yy"
             { yylhs.value.as < std::vector<sieve::ASTNode *> > () = yystack_[0].value.as < std::vector<sieve::ASTNode *> > (); }
#line 993 "../gen/sieve_parser.tab.cc"
    break;

  case 29: // tests: tests "," test
#line 234 "../src/sieve_parser.yy"
                     { yystack_[2].value.as < std::vector<sieve::ASTNode *> > ().insert(yystack_[2].value.as < std::vector<sieve::ASTNode *> > ().end(), yystack_[0].value.as < std::vector<sieve::ASTNode *> > ().begin(), yystack_[0].value.as < std::vector<sieve::ASTNode *> > ().end()); yylhs.value.as < std::vector<sieve::ASTNode *> > () = yystack_[2].value.as < std::vector<sieve::ASTNode *> > (); }
#line 999 "../gen/sieve_parser.tab.cc"
    break;

  case 30: // test: "identifier" arguments
#line 239 "../src/sieve_parser.yy"
        {
            sieve::ASTTest *test = new sieve::ASTTest(yystack_[1].location, yystack_[1].value.as < std::string > ());
            test->push(yystack_[0].value.as < std::vector<sieve::ASTNode *> > ());
            yylhs.value.as < std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>(1, test);
        }
#line 1009 "../gen/sieve_parser.tab.cc"
    break;

  case 31: // test: "identifier"
#line 244 "../src/sieve_parser.yy"
                 { yylhs.value.as < std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>(1, new sieve::ASTTest(yystack_[0].location, yystack_[0].value.as < std::string > ())); }
#line 1015 "../gen/sieve_parser.tab.cc"
    break;

  case 32: // test: "true"
#line 245 "../src/sieve_parser.yy"
           { yylhs.value.as < std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>(1, new sieve::ASTBoolean(yystack_[0].location, yystack_[0].value.as < bool > ())); }
#line 1021 "../gen/sieve_parser.tab.cc"
    break;

  case 33: // test: "false"
#line 246 "../src/sieve_parser.yy"
            { yylhs.value.as < std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>(1, new sieve::ASTBoolean(yystack_[0].location, yystack_[0].value.as < bool > ())); }
#line 1027 "../gen/sieve_parser.tab.cc"
    break;

  case 34: // string_list: STRING_LITERAL
#line 249 "../src/sieve_parser.yy"
                             {yylhs.value.as < std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>(1,  new sieve::ASTString(yystack_[0].location, yystack_[0].value.as < std::string > ())); }
#line 1033 "../gen/sieve_parser.tab.cc"
    break;

  case 35: // string_list: "[" strings "]"
#line 250 "../src/sieve_parser.yy"
                      { yylhs.value.as < std::vector<sieve::ASTNode *> > () = yystack_[1].value.as < std::vector<sieve::ASTNode *> > (); }
#line 1039 "../gen/sieve_parser.tab.cc"
    break;

  case 36: // strings: STRING_LITERAL
#line 253 "../src/sieve_parser.yy"
                         {yylhs.value.as < std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>(1, new sieve::ASTString(yystack_[0].location, yystack_[0].value.as < std::string > ())); }
#line 1045 "../gen/sieve_parser.tab.cc"
    break;

  case 37: // strings: strings "," STRING_LITERAL
#line 254 "../src/sieve_parser.yy"
                                 { yystack_[2].value.as < std::vector<sieve::ASTNode *> > ().push_back( new sieve::ASTString(yystack_[0].location, yystack_[0].value.as < std::string > ())); yylhs.value.as < std::vector<sieve::ASTNode *> > () = yystack_[2].value.as < std::vector<sieve::ASTNode *> > (); }
#line 1051 "../gen/sieve_parser.tab.cc"
    break;

  case 38: // numeric: "number"
#line 259 "../src/sieve_parser.yy"
        {
            yylhs.value.as < sieve::ASTNumeric * > () = new sieve::ASTNumeric(yystack_[0].location, yystack_[0].value.as < int > ());
        }
#line 1059 "../gen/sieve_parser.tab.cc"
    break;

  case 39: // numeric: "number" "quantifier"
#line 263 "../src/sieve_parser.yy"
        {
            // TODO: Somehow incorporate the quantifier in here
            yylhs.value.as < sieve::ASTNumeric * > () = new sieve::ASTNumeric(yystack_[1].location, yystack_[1].value.as < int > ());
        }
#line 1068 "../gen/sieve_parser.tab.cc"
    break;


#line 1072 "../gen/sieve_parser.tab.cc"

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
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
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
  sieve_parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  sieve_parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

  std::string
  sieve_parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // sieve_parser::context.
  sieve_parser::context::context (const sieve_parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  sieve_parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  sieve_parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  sieve_parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char sieve_parser::yypact_ninf_ = -17;

  const signed char sieve_parser::yytable_ninf_ = -1;

  const signed char
  sieve_parser::yypact_[] =
  {
       4,   -17,    14,    39,    30,    13,    12,    28,   -17,    52,
       9,   -17,    36,    57,    46,   -17,   -17,    47,   -17,    57,
     -17,    59,    62,    55,   -17,   -17,   -17,   -17,     0,    51,
     -17,   -17,   -17,    57,    47,   -17,    -4,   -17,    77,    65,
     -17,    27,   -17,   -17,   -17,   -17,   -17,   -17,   -17,    10,
      47,    47,   -17,   -17,    68,    47,   -17,    57,   -17,   -17,
     -17,   -17,   -17,   -17
  };

  const signed char
  sieve_parser::yydefact_[] =
  {
       0,     2,     0,     0,     0,     0,     0,     3,     4,    11,
       0,    34,     0,     0,    31,    32,    33,     0,     8,     0,
      26,    38,     0,    18,    23,    22,    24,    25,     0,     0,
       9,     1,     5,     0,     0,    36,     0,     6,     0,    30,
      15,     0,    28,    39,     7,    21,    20,    19,    14,     0,
       0,     0,    12,    35,     0,     0,    27,     0,    13,    10,
      17,    37,    16,    29
  };

  const signed char
  sieve_parser::yypgoto_[] =
  {
     -17,   -17,    61,    -6,   -15,   -17,    76,   -16,    69,   -17,
      -3,    89,   -17,   -17
  };

  const signed char
  sieve_parser::yydefgoto_[] =
  {
       0,     6,     7,     8,    30,     9,    22,    23,    24,    41,
      25,    26,    36,    27
  };

  const signed char
  sieve_parser::yytable_[] =
  {
      17,    32,    40,     2,     1,     3,    45,     2,    53,     3,
      38,    54,    31,     2,    48,     3,    42,     4,     5,    52,
      47,     4,     5,    45,    58,    10,    28,     4,     5,    35,
      51,     2,    29,     3,    11,    59,    60,    56,    18,    19,
      62,    10,    57,    32,    37,     4,     5,    14,    13,    20,
      11,    21,    15,    16,    63,    19,    14,    10,    33,    34,
      28,    15,    16,    14,    19,    20,    11,    21,    15,    16,
      44,    50,    14,    10,    14,    43,    10,    15,    16,    15,
      16,    20,    11,    21,    20,    11,    21,    55,    61,    49,
      39,    12,    46
  };

  const signed char
  sieve_parser::yycheck_[] =
  {
       3,     7,    17,     3,     0,     5,    22,     3,    12,     5,
      13,    15,     0,     3,    14,     5,    19,    17,    18,    34,
      23,    17,    18,    39,    14,    11,    13,    17,    18,    20,
      33,     3,    19,     5,    20,    50,    51,    10,     8,     9,
      55,    11,    15,    49,     8,    17,    18,    17,     9,    19,
      20,    21,    22,    23,    57,     9,    17,    11,     6,     7,
      13,    22,    23,    17,     9,    19,    20,    21,    22,    23,
       8,    20,    17,    11,    17,    16,    11,    22,    23,    22,
      23,    19,    20,    21,    19,    20,    21,    10,    20,    28,
      14,     2,    23
  };

  const signed char
  sieve_parser::yystos_[] =
  {
       0,     0,     3,     5,    17,    18,    25,    26,    27,    29,
      11,    20,    35,     9,    17,    22,    23,    34,     8,     9,
      19,    21,    30,    31,    32,    34,    35,    37,    13,    19,
      28,     0,    27,     6,     7,    20,    36,     8,    34,    30,
      28,    33,    34,    16,     8,    31,    32,    34,    14,    26,
      20,    34,    28,    12,    15,    10,    10,    15,    14,    28,
      28,    20,    28,    34
  };

  const signed char
  sieve_parser::yyr1_[] =
  {
       0,    24,    25,    25,    26,    26,    27,    27,    27,    27,
      27,    27,    27,    28,    28,    29,    29,    29,    30,    30,
      30,    30,    30,    30,    31,    31,    31,    32,    33,    33,
      34,    34,    34,    34,    35,    35,    36,    36,    37,    37
  };

  const signed char
  sieve_parser::yyr2_[] =
  {
       0,     2,     1,     1,     1,     2,     3,     3,     2,     2,
       4,     1,     3,     3,     2,     3,     5,     4,     1,     2,
       2,     2,     1,     1,     1,     1,     1,     3,     1,     3,
       2,     1,     1,     1,     1,     3,     1,     3,     1,     2
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const sieve_parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "\"require\"",
  "\"string\"", "\"if\"", "\"elsif\"", "\"else\"", "\";\"", "\"(\"",
  "\")\"", "\"[\"", "\"]\"", "\"{\"", "\"}\"", "\",\"", "\"quantifier\"",
  "\"identifier\"", "\"foreverypart\"", "\":tag\"", "STRING_LITERAL",
  "\"number\"", "\"true\"", "\"false\"", "$accept", "sieve",
  "command_list", "command", "block", "if_flow", "arguments", "argument",
  "test_list", "tests", "test", "string_list", "strings", "numeric", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  sieve_parser::yyrline_[] =
  {
       0,    88,    88,    94,   103,   104,   108,   120,   126,   131,
     137,   145,   146,   153,   159,   166,   176,   186,   196,   197,
     198,   199,   200,   201,   204,   214,   218,   225,   233,   234,
     238,   244,   245,   246,   249,   250,   253,   254,   258,   262
  };

  void
  sieve_parser::yy_stack_print_ () const
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
  sieve_parser::yy_reduce_print_ (int yyrule) const
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


} // yy
#line 1583 "../gen/sieve_parser.tab.cc"

#line 269 "../src/sieve_parser.yy"


void yy::sieve_parser::error( const location_type &l, const std::string &m ) {
    driver.push_error(l, m);
}
