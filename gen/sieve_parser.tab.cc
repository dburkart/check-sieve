// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

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


// First part of user declarations.

#line 37 "../gen/sieve_parser.tab.cc" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "sieve_parser.tab.hh"

// User implementation prologue.

#line 51 "../gen/sieve_parser.tab.cc" // lalr1.cc:412
// Unqualified %code blocks.
#line 35 "../src/sieve_parser.yy" // lalr1.cc:413

#include "sieve_driver.hh"
#include "sieve_scanner.hh"

#line 58 "../gen/sieve_parser.tab.cc" // lalr1.cc:413


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
    while (/*CONSTCOND*/ false)
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
      *yycdebug_ << std::endl;                  \
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
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace yy {
#line 144 "../gen/sieve_parser.tab.cc" // lalr1.cc:479

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
        std::string yyr = "";
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
              // Fall through.
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


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  inline
  sieve_parser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  sieve_parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  sieve_parser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  sieve_parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  sieve_parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  sieve_parser::symbol_number_type
  sieve_parser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  sieve_parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  sieve_parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
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

    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  sieve_parser::stack_symbol_type&
  sieve_parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 22: // "true"
      case 23: // "false"
        value.copy< bool > (that.value);
        break;

      case 21: // "number"
        value.copy< int > (that.value);
        break;

      case 27: // command
      case 28: // block
      case 29: // if_flow
        value.copy< sieve::ASTNode * > (that.value);
        break;

      case 37: // numeric
        value.copy< sieve::ASTNumeric * > (that.value);
        break;

      case 25: // sieve
        value.copy< sieve::ASTSieve * > (that.value);
        break;

      case 17: // "identifier"
      case 18: // "foreverypart"
      case 19: // ":tag"
      case 20: // STRING_LITERAL
        value.copy< std::string > (that.value);
        break;

      case 26: // command_list
      case 30: // arguments
      case 31: // argument
      case 32: // test_list
      case 33: // tests
      case 34: // test
      case 35: // string_list
      case 36: // strings
        value.copy< std::vector<sieve::ASTNode *> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
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
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  sieve_parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  sieve_parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  sieve_parser::yypop_ (unsigned int n)
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

  inline sieve_parser::state_type
  sieve_parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  sieve_parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  sieve_parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
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

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    // User initialization code.
    #line 28 "../src/sieve_parser.yy" // lalr1.cc:745
{
    yyla.location.begin.filename = yyla.location.end.filename = &driver.filepath;
}

#line 500 "../gen/sieve_parser.tab.cc" // lalr1.cc:745

    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            symbol_type yylookahead (yylex (scanner, driver));
            yyla.move (yylookahead);
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
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
    yypush_ ("Shifting", yyn, yyla);
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
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
        switch (yyr1_[yyn])
    {
      case 22: // "true"
      case 23: // "false"
        yylhs.value.build< bool > ();
        break;

      case 21: // "number"
        yylhs.value.build< int > ();
        break;

      case 27: // command
      case 28: // block
      case 29: // if_flow
        yylhs.value.build< sieve::ASTNode * > ();
        break;

      case 37: // numeric
        yylhs.value.build< sieve::ASTNumeric * > ();
        break;

      case 25: // sieve
        yylhs.value.build< sieve::ASTSieve * > ();
        break;

      case 17: // "identifier"
      case 18: // "foreverypart"
      case 19: // ":tag"
      case 20: // STRING_LITERAL
        yylhs.value.build< std::string > ();
        break;

      case 26: // command_list
      case 30: // arguments
      case 31: // argument
      case 32: // test_list
      case 33: // tests
      case 34: // test
      case 35: // string_list
      case 36: // strings
        yylhs.value.build< std::vector<sieve::ASTNode *> > ();
        break;

      default:
        break;
    }


      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 89 "../src/sieve_parser.yy" // lalr1.cc:859
    {
            sieve::ASTSieve *sieve = new sieve::ASTSieve(yystack_[0].location);
            driver.syntax_tree(sieve);
            yylhs.value.as< sieve::ASTSieve * > () = sieve;
        }
#line 655 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 3:
#line 95 "../src/sieve_parser.yy" // lalr1.cc:859
    {
            sieve::ASTSieve *sieve = new sieve::ASTSieve(yystack_[0].location);
            sieve->push(yystack_[0].value.as< std::vector<sieve::ASTNode *> > ());
            driver.syntax_tree(sieve);
            yylhs.value.as< sieve::ASTSieve * > () = sieve;
        }
#line 666 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 4:
#line 103 "../src/sieve_parser.yy" // lalr1.cc:859
    { yylhs.value.as< std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>(1, yystack_[0].value.as< sieve::ASTNode * > ()); }
#line 672 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 5:
#line 104 "../src/sieve_parser.yy" // lalr1.cc:859
    { yystack_[1].value.as< std::vector<sieve::ASTNode *> > ().push_back(yystack_[0].value.as< sieve::ASTNode * > ()); yylhs.value.as< std::vector<sieve::ASTNode *> > () = yystack_[1].value.as< std::vector<sieve::ASTNode *> > (); }
#line 678 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 6:
#line 109 "../src/sieve_parser.yy" // lalr1.cc:859
    {
            sieve::ASTRequire *require = new sieve::ASTRequire(yystack_[2].location);
            if (yystack_[1].value.as< std::vector<sieve::ASTNode *> > ().size() > 1) {
                sieve::ASTStringList *stringList = new sieve::ASTStringList(yystack_[1].location);
                stringList->push(yystack_[1].value.as< std::vector<sieve::ASTNode *> > ());
                require->push(stringList);
            } else {
                require->push(yystack_[1].value.as< std::vector<sieve::ASTNode *> > ());
            }
            yylhs.value.as< sieve::ASTNode * > () = dynamic_cast<sieve::ASTNode *>(require);
        }
#line 694 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 7:
#line 121 "../src/sieve_parser.yy" // lalr1.cc:859
    {
            sieve::ASTCommand *command = new sieve::ASTCommand(yystack_[2].location, yystack_[2].value.as< std::string > ());
            command->push(yystack_[1].value.as< std::vector<sieve::ASTNode *> > ());
            yylhs.value.as< sieve::ASTNode * > () = dynamic_cast<sieve::ASTNode *>(command);
        }
#line 704 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 8:
#line 127 "../src/sieve_parser.yy" // lalr1.cc:859
    {
            sieve::ASTCommand *command = new sieve::ASTCommand(yystack_[1].location, yystack_[1].value.as< std::string > ());
            yylhs.value.as< sieve::ASTNode * > () = dynamic_cast<sieve::ASTNode *>(command);
        }
#line 713 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 9:
#line 132 "../src/sieve_parser.yy" // lalr1.cc:859
    {
            sieve::ASTCommand *command = new sieve::ASTCommand(yystack_[1].location, yystack_[1].value.as< std::string > ());
            command->push(yystack_[0].value.as< sieve::ASTNode * > ());
            yylhs.value.as< sieve::ASTNode * > () = dynamic_cast<sieve::ASTNode *>(command);
        }
#line 723 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 10:
#line 138 "../src/sieve_parser.yy" // lalr1.cc:859
    {
            sieve::ASTCommand *command = new sieve::ASTCommand(yystack_[3].location, yystack_[3].value.as< std::string > ());
            command->push(new sieve::ASTTag(yystack_[2].location, yystack_[2].value.as< std::string > ()));
            command->push(new sieve::ASTString(yystack_[1].location, yystack_[1].value.as< std::string > ()));
            command->push(yystack_[0].value.as< sieve::ASTNode * > ());
            yylhs.value.as< sieve::ASTNode * > () = dynamic_cast<sieve::ASTNode *>(command);
        }
#line 735 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 11:
#line 145 "../src/sieve_parser.yy" // lalr1.cc:859
    { yylhs.value.as< sieve::ASTNode * > () = yystack_[0].value.as< sieve::ASTNode * > (); }
#line 741 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 12:
#line 147 "../src/sieve_parser.yy" // lalr1.cc:859
    {
            yystack_[2].value.as< sieve::ASTNode * > ()->push(yystack_[0].value.as< sieve::ASTNode * > ());
            yylhs.value.as< sieve::ASTNode * > () = yystack_[2].value.as< sieve::ASTNode * > ();
        }
#line 750 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 13:
#line 152 "../src/sieve_parser.yy" // lalr1.cc:859
    {
            sieve::ASTNoOp *noop = new sieve::ASTNoOp(yystack_[0].location);
            yylhs.value.as< sieve::ASTNode * > () = noop;
        }
#line 759 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 14:
#line 159 "../src/sieve_parser.yy" // lalr1.cc:859
    {
            sieve::ASTBlock *block = new sieve::ASTBlock( yystack_[2].location );
            block->push(yystack_[1].value.as< std::vector<sieve::ASTNode *> > ());
            yylhs.value.as< sieve::ASTNode * > () = dynamic_cast<sieve::ASTNode *>( block );
        }
#line 769 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 15:
#line 165 "../src/sieve_parser.yy" // lalr1.cc:859
    {
            sieve::ASTNoOp *noop = new sieve::ASTNoOp( yystack_[1].location );
            yylhs.value.as< sieve::ASTNode * > () = noop;
        }
#line 778 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 16:
#line 172 "../src/sieve_parser.yy" // lalr1.cc:859
    {
            sieve::ASTBranch *branch = new sieve::ASTBranch( yystack_[2].location );
            sieve::ASTCondition *condition = new sieve::ASTCondition( yystack_[1].location );
            condition->push(yystack_[1].value.as< std::vector<sieve::ASTNode *> > ());
            branch->push(condition);
            branch->push(yystack_[0].value.as< sieve::ASTNode * > ());
            yylhs.value.as< sieve::ASTNode * > () = dynamic_cast<sieve::ASTNode *>(branch);
            
        }
#line 792 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 17:
#line 182 "../src/sieve_parser.yy" // lalr1.cc:859
    {
            sieve::ASTCondition *condition = new sieve::ASTCondition( yystack_[1].location );
            condition->push(yystack_[1].value.as< std::vector<sieve::ASTNode *> > ());
            yystack_[3].value.as< sieve::ASTNode * > ()->push(condition);
            yystack_[3].value.as< sieve::ASTNode * > ()->push(yystack_[0].value.as< sieve::ASTNode * > ());
            yylhs.value.as< sieve::ASTNode * > () = yystack_[3].value.as< sieve::ASTNode * > ();
        }
#line 804 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 18:
#line 191 "../src/sieve_parser.yy" // lalr1.cc:859
    { yylhs.value.as< std::vector<sieve::ASTNode *> > () = yystack_[0].value.as< std::vector<sieve::ASTNode *> > (); }
#line 810 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 19:
#line 192 "../src/sieve_parser.yy" // lalr1.cc:859
    { yystack_[1].value.as< std::vector<sieve::ASTNode *> > ().insert(yystack_[1].value.as< std::vector<sieve::ASTNode *> > ().end(), yystack_[0].value.as< std::vector<sieve::ASTNode *> > ().begin(), yystack_[0].value.as< std::vector<sieve::ASTNode *> > ().end()); yylhs.value.as< std::vector<sieve::ASTNode *> > () = yystack_[1].value.as< std::vector<sieve::ASTNode *> > (); }
#line 816 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 20:
#line 193 "../src/sieve_parser.yy" // lalr1.cc:859
    { yystack_[1].value.as< std::vector<sieve::ASTNode *> > ().insert(yystack_[1].value.as< std::vector<sieve::ASTNode *> > ().end(), yystack_[0].value.as< std::vector<sieve::ASTNode *> > ().begin(), yystack_[0].value.as< std::vector<sieve::ASTNode *> > ().end()); yylhs.value.as< std::vector<sieve::ASTNode *> > () = yystack_[1].value.as< std::vector<sieve::ASTNode *> > (); }
#line 822 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 21:
#line 194 "../src/sieve_parser.yy" // lalr1.cc:859
    { yystack_[1].value.as< std::vector<sieve::ASTNode *> > ().insert(yystack_[1].value.as< std::vector<sieve::ASTNode *> > ().end(), yystack_[0].value.as< std::vector<sieve::ASTNode *> > ().begin(), yystack_[0].value.as< std::vector<sieve::ASTNode *> > ().end()); yylhs.value.as< std::vector<sieve::ASTNode *> > () = yystack_[1].value.as< std::vector<sieve::ASTNode *> > (); }
#line 828 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 22:
#line 195 "../src/sieve_parser.yy" // lalr1.cc:859
    { yylhs.value.as< std::vector<sieve::ASTNode *> > () = yystack_[0].value.as< std::vector<sieve::ASTNode *> > (); }
#line 834 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 23:
#line 196 "../src/sieve_parser.yy" // lalr1.cc:859
    { yylhs.value.as< std::vector<sieve::ASTNode *> > () = yystack_[0].value.as< std::vector<sieve::ASTNode *> > (); }
#line 840 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 24:
#line 200 "../src/sieve_parser.yy" // lalr1.cc:859
    {
            if (yystack_[0].value.as< std::vector<sieve::ASTNode *> > ().size() > 1) {
                sieve::ASTStringList *stringList = new sieve::ASTStringList(yystack_[0].location);
                stringList->push(yystack_[0].value.as< std::vector<sieve::ASTNode *> > ());
                yylhs.value.as< std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>( 1, dynamic_cast<sieve::ASTNode *>(stringList));
            } else {
                yylhs.value.as< std::vector<sieve::ASTNode *> > () = yystack_[0].value.as< std::vector<sieve::ASTNode *> > ();
            }
        }
#line 854 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 25:
#line 210 "../src/sieve_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>( 1, dynamic_cast<sieve::ASTNode *>(yystack_[0].value.as< sieve::ASTNumeric * > ()));
        }
#line 862 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 26:
#line 214 "../src/sieve_parser.yy" // lalr1.cc:859
    {
            sieve::ASTTag *tag = new sieve::ASTTag(yystack_[0].location, yystack_[0].value.as< std::string > ());
            yylhs.value.as< std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>( 1, dynamic_cast<sieve::ASTNode *>(tag));
        }
#line 871 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 27:
#line 220 "../src/sieve_parser.yy" // lalr1.cc:859
    { yylhs.value.as< std::vector<sieve::ASTNode *> > () = yystack_[1].value.as< std::vector<sieve::ASTNode *> > (); }
#line 877 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 28:
#line 223 "../src/sieve_parser.yy" // lalr1.cc:859
    { yylhs.value.as< std::vector<sieve::ASTNode *> > () = yystack_[0].value.as< std::vector<sieve::ASTNode *> > (); }
#line 883 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 29:
#line 224 "../src/sieve_parser.yy" // lalr1.cc:859
    { yystack_[2].value.as< std::vector<sieve::ASTNode *> > ().insert(yystack_[2].value.as< std::vector<sieve::ASTNode *> > ().end(), yystack_[0].value.as< std::vector<sieve::ASTNode *> > ().begin(), yystack_[0].value.as< std::vector<sieve::ASTNode *> > ().end()); yylhs.value.as< std::vector<sieve::ASTNode *> > () = yystack_[2].value.as< std::vector<sieve::ASTNode *> > (); }
#line 889 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 30:
#line 229 "../src/sieve_parser.yy" // lalr1.cc:859
    {
            sieve::ASTTest *test = new sieve::ASTTest(yystack_[1].location, yystack_[1].value.as< std::string > ());
            test->push(yystack_[0].value.as< std::vector<sieve::ASTNode *> > ());
            yylhs.value.as< std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>(1, test);
        }
#line 899 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 31:
#line 234 "../src/sieve_parser.yy" // lalr1.cc:859
    { yylhs.value.as< std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>(1, new sieve::ASTBoolean(yystack_[0].location, yystack_[0].value.as< bool > ())); }
#line 905 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 32:
#line 235 "../src/sieve_parser.yy" // lalr1.cc:859
    { yylhs.value.as< std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>(1, new sieve::ASTBoolean(yystack_[0].location, yystack_[0].value.as< bool > ())); }
#line 911 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 33:
#line 238 "../src/sieve_parser.yy" // lalr1.cc:859
    {yylhs.value.as< std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>(1,  new sieve::ASTString(yystack_[0].location, yystack_[0].value.as< std::string > ())); }
#line 917 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 34:
#line 239 "../src/sieve_parser.yy" // lalr1.cc:859
    { yylhs.value.as< std::vector<sieve::ASTNode *> > () = yystack_[1].value.as< std::vector<sieve::ASTNode *> > (); }
#line 923 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 35:
#line 242 "../src/sieve_parser.yy" // lalr1.cc:859
    {yylhs.value.as< std::vector<sieve::ASTNode *> > () = std::vector<sieve::ASTNode *>(1, new sieve::ASTString(yystack_[0].location, yystack_[0].value.as< std::string > ())); }
#line 929 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 36:
#line 243 "../src/sieve_parser.yy" // lalr1.cc:859
    { yystack_[2].value.as< std::vector<sieve::ASTNode *> > ().push_back( new sieve::ASTString(yystack_[0].location, yystack_[0].value.as< std::string > ())); yylhs.value.as< std::vector<sieve::ASTNode *> > () = yystack_[2].value.as< std::vector<sieve::ASTNode *> > (); }
#line 935 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 37:
#line 248 "../src/sieve_parser.yy" // lalr1.cc:859
    {
            yylhs.value.as< sieve::ASTNumeric * > () = new sieve::ASTNumeric(yystack_[0].location, yystack_[0].value.as< int > ());
        }
#line 943 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;

  case 38:
#line 252 "../src/sieve_parser.yy" // lalr1.cc:859
    {
            // TODO: Somehow incorporate the quantifier in here
            yylhs.value.as< sieve::ASTNumeric * > () = new sieve::ASTNumeric(yystack_[1].location, yystack_[1].value.as< int > ());
        }
#line 952 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
    break;


#line 956 "../gen/sieve_parser.tab.cc" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
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

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
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
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

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
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  sieve_parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
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
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
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


  const signed char sieve_parser::yypact_ninf_ = -16;

  const signed char sieve_parser::yytable_ninf_ = -1;

  const signed char
  sieve_parser::yypact_[] =
  {
       4,   -16,    27,   -12,   -16,     6,    24,    32,    52,   -16,
      33,    29,   -16,    36,    55,   -16,   -16,    38,   -16,   -12,
     -16,    40,    72,    62,   -16,   -16,   -16,   -16,    28,    34,
     -16,   -16,   -16,   -12,    38,   -16,    -9,   -16,    75,   -16,
       3,   -16,   -16,   -16,   -16,   -16,   -16,   -16,    45,    38,
      38,   -16,   -16,    41,   -16,   -12,   -16,   -16,   -16,   -16,
     -16
  };

  const unsigned char
  sieve_parser::yydefact_[] =
  {
       0,     2,     0,     0,    13,     0,     0,     0,     3,     4,
      11,     0,    33,     0,     0,    31,    32,     0,     8,     0,
      26,    37,     0,    18,    23,    22,    24,    25,     0,     0,
       9,     1,     5,     0,     0,    35,     0,     6,    30,    16,
       0,    28,    38,     7,    21,    20,    19,    15,     0,     0,
       0,    12,    34,     0,    27,     0,    14,    10,    17,    36,
      29
  };

  const signed char
  sieve_parser::yypgoto_[] =
  {
     -16,   -16,    30,    -7,   -15,   -16,    51,   -14,    44,   -16,
      -3,    66,   -16,   -16
  };

  const signed char
  sieve_parser::yydefgoto_[] =
  {
      -1,     7,     8,     9,    30,    10,    22,    23,    24,    40,
      25,    26,    36,    27
  };

  const unsigned char
  sieve_parser::yytable_[] =
  {
      17,    32,    39,    52,     1,    14,    53,     2,    44,     3,
      15,    16,     4,    54,    18,    19,    41,    11,    55,    51,
      46,     5,     6,    14,    44,    20,    12,    21,    15,    16,
      50,     2,    31,     3,    57,    58,     4,    28,    11,    33,
      34,    32,    47,    29,    37,     5,     6,    12,     2,    35,
       3,    28,    60,     4,    49,     2,    42,     3,    48,    56,
       4,    59,     5,     6,    19,    38,    11,    45,    13,     5,
       6,    19,    14,     0,    20,    12,    21,    15,    16,    14,
      43,     0,     0,    11,    15,    16,    11,     0,     0,     0,
       0,    20,    12,    21,    20,    12,    21
  };

  const signed char
  sieve_parser::yycheck_[] =
  {
       3,     8,    17,    12,     0,    17,    15,     3,    22,     5,
      22,    23,     8,    10,     8,     9,    19,    11,    15,    34,
      23,    17,    18,    17,    38,    19,    20,    21,    22,    23,
      33,     3,     0,     5,    49,    50,     8,    13,    11,     6,
       7,    48,    14,    19,     8,    17,    18,    20,     3,    20,
       5,    13,    55,     8,    20,     3,    16,     5,    28,    14,
       8,    20,    17,    18,     9,    14,    11,    23,     2,    17,
      18,     9,    17,    -1,    19,    20,    21,    22,    23,    17,
       8,    -1,    -1,    11,    22,    23,    11,    -1,    -1,    -1,
      -1,    19,    20,    21,    19,    20,    21
  };

  const unsigned char
  sieve_parser::yystos_[] =
  {
       0,     0,     3,     5,     8,    17,    18,    25,    26,    27,
      29,    11,    20,    35,    17,    22,    23,    34,     8,     9,
      19,    21,    30,    31,    32,    34,    35,    37,    13,    19,
      28,     0,    27,     6,     7,    20,    36,     8,    30,    28,
      33,    34,    16,     8,    31,    32,    34,    14,    26,    20,
      34,    28,    12,    15,    10,    15,    14,    28,    28,    20,
      34
  };

  const unsigned char
  sieve_parser::yyr1_[] =
  {
       0,    24,    25,    25,    26,    26,    27,    27,    27,    27,
      27,    27,    27,    27,    28,    28,    29,    29,    30,    30,
      30,    30,    30,    30,    31,    31,    31,    32,    33,    33,
      34,    34,    34,    35,    35,    36,    36,    37,    37
  };

  const unsigned char
  sieve_parser::yyr2_[] =
  {
       0,     2,     1,     1,     1,     2,     3,     3,     2,     2,
       4,     1,     3,     1,     3,     2,     3,     4,     1,     2,
       2,     2,     1,     1,     1,     1,     1,     3,     1,     3,
       2,     1,     1,     1,     3,     1,     3,     1,     2
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
     137,   145,   146,   151,   158,   164,   171,   181,   191,   192,
     193,   194,   195,   196,   199,   209,   213,   220,   223,   224,
     228,   234,   235,   238,   239,   242,   243,   247,   251
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
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  sieve_parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG



} // yy
#line 1371 "../gen/sieve_parser.tab.cc" // lalr1.cc:1167
#line 258 "../src/sieve_parser.yy" // lalr1.cc:1168


void yy::sieve_parser::error( const location_type &l, const std::string &m ) {
    driver.push_error(l, m);
}
