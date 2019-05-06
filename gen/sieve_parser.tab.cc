// A Bison parser, made by GNU Bison 3.3.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2019 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.





#include "sieve_parser.tab.hh"


// Unqualified %code blocks.
#line 35 "../src/sieve_parser.yy" // lalr1.cc:435

#include "sieve_driver.hh"
#include "sieve_scanner.hh"

#line 50 "../gen/sieve_parser.tab.cc" // lalr1.cc:435


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


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

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
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
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
#line 145 "../gen/sieve_parser.tab.cc" // lalr1.cc:510

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


  /// Build a parser object.
  sieve_parser::sieve_parser (yyscan_t scanner_yyarg, sieve::driver &driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      scanner (scanner_yyarg),
      driver (driver_yyarg)
  {}

  sieve_parser::~sieve_parser ()
  {}

  sieve_parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/



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

  sieve_parser::symbol_number_type
  sieve_parser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  sieve_parser::stack_symbol_type::stack_symbol_type ()
  {}

  sieve_parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 22: // "true"
      case 23: // "false"
        value.YY_MOVE_OR_COPY< bool > (YY_MOVE (that.value));
        break;

      case 21: // "number"
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case 27: // command
      case 28: // block
      case 29: // if_flow
        value.YY_MOVE_OR_COPY< sieve::ASTNode * > (YY_MOVE (that.value));
        break;

      case 37: // numeric
        value.YY_MOVE_OR_COPY< sieve::ASTNumeric * > (YY_MOVE (that.value));
        break;

      case 25: // sieve
        value.YY_MOVE_OR_COPY< sieve::ASTSieve * > (YY_MOVE (that.value));
        break;

      case 17: // "identifier"
      case 18: // "foreverypart"
      case 19: // ":tag"
      case 20: // STRING_LITERAL
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case 26: // command_list
      case 30: // arguments
      case 31: // argument
      case 32: // test_list
      case 33: // tests
      case 34: // test
      case 35: // string_list
      case 36: // strings
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
    switch (that.type_get ())
    {
      case 22: // "true"
      case 23: // "false"
        value.move< bool > (YY_MOVE (that.value));
        break;

      case 21: // "number"
        value.move< int > (YY_MOVE (that.value));
        break;

      case 27: // command
      case 28: // block
      case 29: // if_flow
        value.move< sieve::ASTNode * > (YY_MOVE (that.value));
        break;

      case 37: // numeric
        value.move< sieve::ASTNumeric * > (YY_MOVE (that.value));
        break;

      case 25: // sieve
        value.move< sieve::ASTSieve * > (YY_MOVE (that.value));
        break;

      case 17: // "identifier"
      case 18: // "foreverypart"
      case 19: // ":tag"
      case 20: // STRING_LITERAL
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case 26: // command_list
      case 30: // arguments
      case 31: // argument
      case 32: // test_list
      case 33: // tests
      case 34: // test
      case 35: // string_list
      case 36: // strings
        value.move< std::vector<sieve::ASTNode *> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  sieve_parser::stack_symbol_type&
  sieve_parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 22: // "true"
      case 23: // "false"
        value.move< bool > (that.value);
        break;

      case 21: // "number"
        value.move< int > (that.value);
        break;

      case 27: // command
      case 28: // block
      case 29: // if_flow
        value.move< sieve::ASTNode * > (that.value);
        break;

      case 37: // numeric
        value.move< sieve::ASTNumeric * > (that.value);
        break;

      case 25: // sieve
        value.move< sieve::ASTSieve * > (that.value);
        break;

      case 17: // "identifier"
      case 18: // "foreverypart"
      case 19: // ":tag"
      case 20: // STRING_LITERAL
        value.move< std::string > (that.value);
        break;

      case 26: // command_list
      case 30: // arguments
      case 31: // argument
      case 32: // test_list
      case 33: // tests
      case 34: // test
      case 35: // string_list
      case 36: // strings
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
  sieve_parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
#if defined __GNUC__ && ! defined __clang__ && ! defined __ICC && __GNUC__ * 100 + __GNUC_MINOR__ <= 408
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
#endif
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
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
  sieve_parser::yypop_ (int n)
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
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  sieve_parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  sieve_parser::yy_table_value_is_error_ (int yyvalue)
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
    // State.
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
#line 28 "../src/sieve_parser.yy" // lalr1.cc:791
{
    yyla.location.begin.filename = yyla.location.end.filename = &driver.filepath;
}

#line 563 "../gen/sieve_parser.tab.cc" // lalr1.cc:791

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
    YYCDEBUG << "Entering state " << yystack_[0].state << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
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

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

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
    yypush_ ("Shifting", yyn, YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
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
      case 22: // "true"
      case 23: // "false"
        yylhs.value.emplace< bool > ();
        break;

      case 21: // "number"
        yylhs.value.emplace< int > ();
        break;

      case 27: // command
      case 28: // block
      case 29: // if_flow
        yylhs.value.emplace< sieve::ASTNode * > ();
        break;

      case 37: // numeric
        yylhs.value.emplace< sieve::ASTNumeric * > ();
        break;

      case 25: // sieve
        yylhs.value.emplace< sieve::ASTSieve * > ();
        break;

      case 17: // "identifier"
      case 18: // "foreverypart"
      case 19: // ":tag"
      case 20: // STRING_LITERAL
        yylhs.value.emplace< std::string > ();
        break;

      case 26: // command_list
      case 30: // arguments
      case 31: // argument
      case 32: // test_list
      case 33: // tests
      case 34: // test
      case 35: // string_list
      case 36: // strings
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
  case 2:
#line 89 "../src/sieve_parser.yy" // lalr1.cc:919
    {
            sieve::ASTSieve *sieve = new sieve::ASTSieve(yystack_[0].location);
            driver.syntax_tree(sieve);
            yylhs.value.as < sieve::ASTSieve * > () = sieve;
        }
#line 732 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 3:
#line 95 "../src/sieve_parser.yy" // lalr1.cc:919
    {
            sieve::ASTSieve *sieve = new sieve::ASTSieve(yystack_[0].location);
            sieve->push(yystack_[0].value.as < std::vector<sieve::ASTNode *> > ());
            driver.syntax_tree(sieve);
            yylhs.value.as < sieve::ASTSieve * > () = sieve;
        }
#line 743 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 4:
#line 103 "../src/sieve_parser.yy" // lalr1.cc:919
    { yylhs.value.as < std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>(1, yystack_[0].value.as < sieve::ASTNode * > ()); }
#line 749 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 5:
#line 104 "../src/sieve_parser.yy" // lalr1.cc:919
    { yystack_[1].value.as < std::vector<sieve::ASTNode *> > ().push_back(yystack_[0].value.as < sieve::ASTNode * > ()); yylhs.value.as < std::vector<sieve::ASTNode *> > () = yystack_[1].value.as < std::vector<sieve::ASTNode *> > (); }
#line 755 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 6:
#line 109 "../src/sieve_parser.yy" // lalr1.cc:919
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
#line 771 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 7:
#line 121 "../src/sieve_parser.yy" // lalr1.cc:919
    {
            sieve::ASTCommand *command = new sieve::ASTCommand(yystack_[2].location, yystack_[2].value.as < std::string > ());
            command->push(yystack_[1].value.as < std::vector<sieve::ASTNode *> > ());
            yylhs.value.as < sieve::ASTNode * > () = dynamic_cast<sieve::ASTNode *>(command);
        }
#line 781 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 8:
#line 127 "../src/sieve_parser.yy" // lalr1.cc:919
    {
            sieve::ASTCommand *command = new sieve::ASTCommand(yystack_[1].location, yystack_[1].value.as < std::string > ());
            yylhs.value.as < sieve::ASTNode * > () = dynamic_cast<sieve::ASTNode *>(command);
        }
#line 790 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 9:
#line 132 "../src/sieve_parser.yy" // lalr1.cc:919
    {
            sieve::ASTCommand *command = new sieve::ASTCommand(yystack_[1].location, yystack_[1].value.as < std::string > ());
            command->push(yystack_[0].value.as < sieve::ASTNode * > ());
            yylhs.value.as < sieve::ASTNode * > () = dynamic_cast<sieve::ASTNode *>(command);
        }
#line 800 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 10:
#line 138 "../src/sieve_parser.yy" // lalr1.cc:919
    {
            sieve::ASTCommand *command = new sieve::ASTCommand(yystack_[3].location, yystack_[3].value.as < std::string > ());
            command->push(new sieve::ASTTag(yystack_[2].location, yystack_[2].value.as < std::string > ()));
            command->push(new sieve::ASTString(yystack_[1].location, yystack_[1].value.as < std::string > ()));
            command->push(yystack_[0].value.as < sieve::ASTNode * > ());
            yylhs.value.as < sieve::ASTNode * > () = dynamic_cast<sieve::ASTNode *>(command);
        }
#line 812 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 11:
#line 145 "../src/sieve_parser.yy" // lalr1.cc:919
    { yylhs.value.as < sieve::ASTNode * > () = yystack_[0].value.as < sieve::ASTNode * > (); }
#line 818 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 12:
#line 147 "../src/sieve_parser.yy" // lalr1.cc:919
    {
            yystack_[2].value.as < sieve::ASTNode * > ()->push(yystack_[0].value.as < sieve::ASTNode * > ());
            yylhs.value.as < sieve::ASTNode * > () = yystack_[2].value.as < sieve::ASTNode * > ();
        }
#line 827 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 13:
#line 154 "../src/sieve_parser.yy" // lalr1.cc:919
    {
            sieve::ASTBlock *block = new sieve::ASTBlock( yystack_[2].location );
            block->push(yystack_[1].value.as < std::vector<sieve::ASTNode *> > ());
            yylhs.value.as < sieve::ASTNode * > () = dynamic_cast<sieve::ASTNode *>( block );
        }
#line 837 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 14:
#line 160 "../src/sieve_parser.yy" // lalr1.cc:919
    {
            sieve::ASTNoOp *noop = new sieve::ASTNoOp( yystack_[1].location );
            yylhs.value.as < sieve::ASTNode * > () = noop;
        }
#line 846 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 15:
#line 167 "../src/sieve_parser.yy" // lalr1.cc:919
    {
            sieve::ASTBranch *branch = new sieve::ASTBranch( yystack_[2].location );
            sieve::ASTCondition *condition = new sieve::ASTCondition( yystack_[1].location );
            condition->push(yystack_[1].value.as < std::vector<sieve::ASTNode *> > ());
            branch->push(condition);
            branch->push(yystack_[0].value.as < sieve::ASTNode * > ());
            yylhs.value.as < sieve::ASTNode * > () = dynamic_cast<sieve::ASTNode *>(branch);
            
        }
#line 860 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 16:
#line 177 "../src/sieve_parser.yy" // lalr1.cc:919
    {
            sieve::ASTCondition *condition = new sieve::ASTCondition( yystack_[1].location );
            condition->push(yystack_[1].value.as < std::vector<sieve::ASTNode *> > ());
            yystack_[3].value.as < sieve::ASTNode * > ()->push(condition);
            yystack_[3].value.as < sieve::ASTNode * > ()->push(yystack_[0].value.as < sieve::ASTNode * > ());
            yylhs.value.as < sieve::ASTNode * > () = yystack_[3].value.as < sieve::ASTNode * > ();
        }
#line 872 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 17:
#line 186 "../src/sieve_parser.yy" // lalr1.cc:919
    { yylhs.value.as < std::vector<sieve::ASTNode *> > () = yystack_[0].value.as < std::vector<sieve::ASTNode *> > (); }
#line 878 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 18:
#line 187 "../src/sieve_parser.yy" // lalr1.cc:919
    { yystack_[1].value.as < std::vector<sieve::ASTNode *> > ().insert(yystack_[1].value.as < std::vector<sieve::ASTNode *> > ().end(), yystack_[0].value.as < std::vector<sieve::ASTNode *> > ().begin(), yystack_[0].value.as < std::vector<sieve::ASTNode *> > ().end()); yylhs.value.as < std::vector<sieve::ASTNode *> > () = yystack_[1].value.as < std::vector<sieve::ASTNode *> > (); }
#line 884 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 19:
#line 188 "../src/sieve_parser.yy" // lalr1.cc:919
    { yystack_[1].value.as < std::vector<sieve::ASTNode *> > ().insert(yystack_[1].value.as < std::vector<sieve::ASTNode *> > ().end(), yystack_[0].value.as < std::vector<sieve::ASTNode *> > ().begin(), yystack_[0].value.as < std::vector<sieve::ASTNode *> > ().end()); yylhs.value.as < std::vector<sieve::ASTNode *> > () = yystack_[1].value.as < std::vector<sieve::ASTNode *> > (); }
#line 890 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 20:
#line 189 "../src/sieve_parser.yy" // lalr1.cc:919
    { yystack_[1].value.as < std::vector<sieve::ASTNode *> > ().insert(yystack_[1].value.as < std::vector<sieve::ASTNode *> > ().end(), yystack_[0].value.as < std::vector<sieve::ASTNode *> > ().begin(), yystack_[0].value.as < std::vector<sieve::ASTNode *> > ().end()); yylhs.value.as < std::vector<sieve::ASTNode *> > () = yystack_[1].value.as < std::vector<sieve::ASTNode *> > (); }
#line 896 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 21:
#line 190 "../src/sieve_parser.yy" // lalr1.cc:919
    { yylhs.value.as < std::vector<sieve::ASTNode *> > () = yystack_[0].value.as < std::vector<sieve::ASTNode *> > (); }
#line 902 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 22:
#line 191 "../src/sieve_parser.yy" // lalr1.cc:919
    { yylhs.value.as < std::vector<sieve::ASTNode *> > () = yystack_[0].value.as < std::vector<sieve::ASTNode *> > (); }
#line 908 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 23:
#line 195 "../src/sieve_parser.yy" // lalr1.cc:919
    {
            if (yystack_[0].value.as < std::vector<sieve::ASTNode *> > ().size() > 1) {
                sieve::ASTStringList *stringList = new sieve::ASTStringList(yystack_[0].location);
                stringList->push(yystack_[0].value.as < std::vector<sieve::ASTNode *> > ());
                yylhs.value.as < std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>( 1, dynamic_cast<sieve::ASTNode *>(stringList));
            } else {
                yylhs.value.as < std::vector<sieve::ASTNode *> > () = yystack_[0].value.as < std::vector<sieve::ASTNode *> > ();
            }
        }
#line 922 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 24:
#line 205 "../src/sieve_parser.yy" // lalr1.cc:919
    {
            yylhs.value.as < std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>( 1, dynamic_cast<sieve::ASTNode *>(yystack_[0].value.as < sieve::ASTNumeric * > ()));
        }
#line 930 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 25:
#line 209 "../src/sieve_parser.yy" // lalr1.cc:919
    {
            sieve::ASTTag *tag = new sieve::ASTTag(yystack_[0].location, yystack_[0].value.as < std::string > ());
            yylhs.value.as < std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>( 1, dynamic_cast<sieve::ASTNode *>(tag));
        }
#line 939 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 26:
#line 215 "../src/sieve_parser.yy" // lalr1.cc:919
    { yylhs.value.as < std::vector<sieve::ASTNode *> > () = yystack_[1].value.as < std::vector<sieve::ASTNode *> > (); }
#line 945 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 27:
#line 218 "../src/sieve_parser.yy" // lalr1.cc:919
    { yylhs.value.as < std::vector<sieve::ASTNode *> > () = yystack_[0].value.as < std::vector<sieve::ASTNode *> > (); }
#line 951 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 28:
#line 219 "../src/sieve_parser.yy" // lalr1.cc:919
    { yystack_[2].value.as < std::vector<sieve::ASTNode *> > ().insert(yystack_[2].value.as < std::vector<sieve::ASTNode *> > ().end(), yystack_[0].value.as < std::vector<sieve::ASTNode *> > ().begin(), yystack_[0].value.as < std::vector<sieve::ASTNode *> > ().end()); yylhs.value.as < std::vector<sieve::ASTNode *> > () = yystack_[2].value.as < std::vector<sieve::ASTNode *> > (); }
#line 957 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 29:
#line 224 "../src/sieve_parser.yy" // lalr1.cc:919
    {
            sieve::ASTTest *test = new sieve::ASTTest(yystack_[1].location, yystack_[1].value.as < std::string > ());
            test->push(yystack_[0].value.as < std::vector<sieve::ASTNode *> > ());
            yylhs.value.as < std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>(1, test);
        }
#line 967 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 30:
#line 229 "../src/sieve_parser.yy" // lalr1.cc:919
    { yylhs.value.as < std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>(1, new sieve::ASTBoolean(yystack_[0].location, yystack_[0].value.as < bool > ())); }
#line 973 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 31:
#line 230 "../src/sieve_parser.yy" // lalr1.cc:919
    { yylhs.value.as < std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>(1, new sieve::ASTBoolean(yystack_[0].location, yystack_[0].value.as < bool > ())); }
#line 979 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 32:
#line 233 "../src/sieve_parser.yy" // lalr1.cc:919
    {yylhs.value.as < std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>(1,  new sieve::ASTString(yystack_[0].location, yystack_[0].value.as < std::string > ())); }
#line 985 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 33:
#line 234 "../src/sieve_parser.yy" // lalr1.cc:919
    { yylhs.value.as < std::vector<sieve::ASTNode *> > () = yystack_[1].value.as < std::vector<sieve::ASTNode *> > (); }
#line 991 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 34:
#line 237 "../src/sieve_parser.yy" // lalr1.cc:919
    {yylhs.value.as < std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>(1, new sieve::ASTString(yystack_[0].location, yystack_[0].value.as < std::string > ())); }
#line 997 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 35:
#line 238 "../src/sieve_parser.yy" // lalr1.cc:919
    { yystack_[2].value.as < std::vector<sieve::ASTNode *> > ().push_back( new sieve::ASTString(yystack_[0].location, yystack_[0].value.as < std::string > ())); yylhs.value.as < std::vector<sieve::ASTNode *> > () = yystack_[2].value.as < std::vector<sieve::ASTNode *> > (); }
#line 1003 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 36:
#line 243 "../src/sieve_parser.yy" // lalr1.cc:919
    {
            yylhs.value.as < sieve::ASTNumeric * > () = new sieve::ASTNumeric(yystack_[0].location, yystack_[0].value.as < int > ());
        }
#line 1011 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;

  case 37:
#line 247 "../src/sieve_parser.yy" // lalr1.cc:919
    {
            // TODO: Somehow incorporate the quantifier in here
            yylhs.value.as < sieve::ASTNumeric * > () = new sieve::ASTNumeric(yystack_[1].location, yystack_[1].value.as < int > ());
        }
#line 1020 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
    break;


#line 1024 "../gen/sieve_parser.tab.cc" // lalr1.cc:919
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
      YY_STACK_PRINT ();

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
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
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
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
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

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
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

  // Generate an error message.
  std::string
  sieve_parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

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
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

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
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char sieve_parser::yypact_ninf_ = -20;

  const signed char sieve_parser::yytable_ninf_ = -1;

  const signed char
  sieve_parser::yypact_[] =
  {
       4,   -20,    17,   -12,    23,    11,     6,    47,   -20,    10,
      28,   -20,    46,    57,   -20,   -20,    42,   -20,   -12,   -20,
      43,    52,    16,   -20,   -20,   -20,   -20,     9,    37,   -20,
     -20,   -20,   -12,    42,   -20,    41,   -20,    64,   -20,    -2,
     -20,   -20,   -20,   -20,   -20,   -20,   -20,    44,    42,    42,
     -20,   -20,    49,   -20,   -12,   -20,   -20,   -20,   -20,   -20
  };

  const unsigned char
  sieve_parser::yydefact_[] =
  {
       0,     2,     0,     0,     0,     0,     0,     3,     4,    11,
       0,    32,     0,     0,    30,    31,     0,     8,     0,    25,
      36,     0,    17,    22,    21,    23,    24,     0,     0,     9,
       1,     5,     0,     0,    34,     0,     6,    29,    15,     0,
      27,    37,     7,    20,    19,    18,    14,     0,     0,     0,
      12,    33,     0,    26,     0,    13,    10,    16,    35,    28
  };

  const signed char
  sieve_parser::yypgoto_[] =
  {
     -20,   -20,    40,    -6,   -13,   -20,    68,   -19,    48,   -20,
      -3,    80,   -20,   -20
  };

  const signed char
  sieve_parser::yydefgoto_[] =
  {
      -1,     6,     7,     8,    29,     9,    21,    22,    23,    39,
      24,    25,    35,    26
  };

  const unsigned char
  sieve_parser::yytable_[] =
  {
      16,    31,    43,    38,     1,    13,    30,     2,    53,     3,
      14,    15,     2,    54,     3,    40,    32,    33,    43,    45,
      50,     4,     5,    46,    27,    18,     4,     5,    10,    49,
      28,    17,    18,    13,    10,    56,    57,    11,    14,    15,
      13,    31,    19,    11,    20,    14,    15,     2,    34,     3,
       2,    59,     3,    51,    36,    27,    52,    48,    55,    41,
      42,     4,     5,    10,     4,     5,    18,    47,    10,    58,
      44,    19,    11,    20,    13,    10,    19,    11,    20,    14,
      15,    37,    12,    19,    11,    20
  };

  const unsigned char
  sieve_parser::yycheck_[] =
  {
       3,     7,    21,    16,     0,    17,     0,     3,    10,     5,
      22,    23,     3,    15,     5,    18,     6,     7,    37,    22,
      33,    17,    18,    14,    13,     9,    17,    18,    11,    32,
      19,     8,     9,    17,    11,    48,    49,    20,    22,    23,
      17,    47,    19,    20,    21,    22,    23,     3,    20,     5,
       3,    54,     5,    12,     8,    13,    15,    20,    14,    16,
       8,    17,    18,    11,    17,    18,     9,    27,    11,    20,
      22,    19,    20,    21,    17,    11,    19,    20,    21,    22,
      23,    13,     2,    19,    20,    21
  };

  const unsigned char
  sieve_parser::yystos_[] =
  {
       0,     0,     3,     5,    17,    18,    25,    26,    27,    29,
      11,    20,    35,    17,    22,    23,    34,     8,     9,    19,
      21,    30,    31,    32,    34,    35,    37,    13,    19,    28,
       0,    27,     6,     7,    20,    36,     8,    30,    28,    33,
      34,    16,     8,    31,    32,    34,    14,    26,    20,    34,
      28,    12,    15,    10,    15,    14,    28,    28,    20,    34
  };

  const unsigned char
  sieve_parser::yyr1_[] =
  {
       0,    24,    25,    25,    26,    26,    27,    27,    27,    27,
      27,    27,    27,    28,    28,    29,    29,    30,    30,    30,
      30,    30,    30,    31,    31,    31,    32,    33,    33,    34,
      34,    34,    35,    35,    36,    36,    37,    37
  };

  const unsigned char
  sieve_parser::yyr2_[] =
  {
       0,     2,     1,     1,     1,     2,     3,     3,     2,     2,
       4,     1,     3,     3,     2,     3,     4,     1,     2,     2,
       2,     1,     1,     1,     1,     1,     3,     1,     3,     2,
       1,     1,     1,     3,     1,     3,     1,     2
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const sieve_parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\"require\"", "\"string\"",
  "\"if\"", "\"elsif\"", "\"else\"", "\";\"", "\"(\"", "\")\"", "\"[\"",
  "\"]\"", "\"{\"", "\"}\"", "\",\"", "\"quantifier\"", "\"identifier\"",
  "\"foreverypart\"", "\":tag\"", "STRING_LITERAL", "\"number\"",
  "\"true\"", "\"false\"", "$accept", "sieve", "command_list", "command",
  "block", "if_flow", "arguments", "argument", "test_list", "tests",
  "test", "string_list", "strings", "numeric", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned char
  sieve_parser::yyrline_[] =
  {
       0,    88,    88,    94,   103,   104,   108,   120,   126,   131,
     137,   145,   146,   153,   159,   166,   176,   186,   187,   188,
     189,   190,   191,   194,   204,   208,   215,   218,   219,   223,
     229,   230,   233,   234,   237,   238,   242,   246
  };

  // Print the state stack on the debug stream.
  void
  sieve_parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  sieve_parser::yy_reduce_print_ (int yyrule)
  {
    unsigned yylno = yyrline_[yyrule];
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
#line 1449 "../gen/sieve_parser.tab.cc" // lalr1.cc:1242
#line 253 "../src/sieve_parser.yy" // lalr1.cc:1243


void yy::sieve_parser::error( const location_type &l, const std::string &m ) {
    driver.push_error(l, m);
}
