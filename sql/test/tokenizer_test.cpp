// Unit tests for the tokenizer in sql/src/tokenizer.cpp.

#include "../src/tokenizer.hpp"

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

namespace {

int failures = 0;

#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)

// Capture the caller's file/line/function when lex()/expectLex() is invoked,
// so a failing helper check points at the test line that caused it, not at the
// generic helper body below.
#define HERE_LOC __FILE__ ":" STRINGIFY(__LINE__)
#define HERE_FN  __func__

#define CHECK_AT(cond, where, fn)                                               \
    do {                                                                        \
        if (!(cond)) {                                                          \
            std::cerr << "tokenizer_test: FAIL " << where << " in " << fn      \
                      << ": " #cond << '\n';                                   \
            ++failures;                                                         \
        }                                                                       \
    } while (false)

#define CHECK(cond) CHECK_AT(cond, HERE_LOC, HERE_FN)

using Expected = std::pair<TokenType, const char*>;

std::vector<Token> lex_at(const std::string& input,
                          const char* where, const char* fn) {
    std::vector<Token> tokens;
    CHECK_AT(tokenize(input, tokens) == errorId_t::ERR_STATUS_OK, where, fn);
    return tokens;
}

#define lex(input) lex_at((input), HERE_LOC, HERE_FN)

// Checks the non-EOF tokens, then requires a trailing EndOfFile token.
void expectLex_at(const std::string& input,
                  std::initializer_list<Expected> expected,
                  const char* where, const char* fn) {
    const std::vector<Token> tokens = lex_at(input, where, fn);
    CHECK_AT(tokens.size() == expected.size() + 1, where, fn);

    const size_t n = std::min(tokens.size(), expected.size());
    for (size_t i = 0; i < n; ++i) {
        CHECK_AT(tokens[i].type == expected.begin()[i].first, where, fn);
        CHECK_AT(tokens[i].text == expected.begin()[i].second, where, fn);
    }
    if (!tokens.empty()) {
        CHECK_AT(tokens.back().type == TokenType::EndOfFile, where, fn);
    }
}

// Variadic so brace-initializer lists with commas stay a single argument.
#define expectLex(input, ...) expectLex_at((input), __VA_ARGS__, HERE_LOC, HERE_FN)

void emptyInputProducesOnlyEofToken() {
    std::vector<Token> tokens;
    CHECK(tokenize("", tokens) == errorId_t::ERR_STATUS_OK);
    CHECK(tokens.size() == 1);
    CHECK(tokens[0].type == TokenType::EndOfFile);
    CHECK(tokens[0].text.empty());
    // Deliberately no assertion on tokens[0].line/col: they are uninitialized.
}

void keywordsMatchCaseInsensitivelyAndKeepOriginalLexeme() {
    for (const auto& [word, type] : KEYWORDS) {
        expectLex(word, {{type, word.c_str()}});
    }
    expectLex("select", {{TokenType::Select, "select"}});
    expectLex("SeLeCt", {{TokenType::Select, "SeLeCt"}});
    expectLex("fRoM", {{TokenType::From, "fRoM"}});
}

void identifiersAllowLeadingUnderscoreDigitsAndUnderscores() {
    expectLex("users", {{TokenType::Identifier, "users"}});
    expectLex("_id", {{TokenType::Identifier, "_id"}});
    expectLex("col_2", {{TokenType::Identifier, "col_2"}});
    expectLex("selected", {{TokenType::Identifier, "selected"}}); // not a keyword
}

void integerAndFloatTokensKeepTheirRawText() {
    expectLex("0 42 007", {{TokenType::Integer, "0"},
                           {TokenType::Integer, "42"},
                           {TokenType::Integer, "007"}});
    expectLex("3.14 0.5", {{TokenType::Float, "3.14"},
                           {TokenType::Float, "0.5"}});
}

void operatorsRecognizeSingleAndTwoCharacterForms() {
    expectLex("= == != <> < <= > >=",
              {{TokenType::Eq, "="},
               {TokenType::Eq, "=="},
               {TokenType::NotEq, "!="},
               {TokenType::NotEq, "<>"},
               {TokenType::Lt, "<"},
               {TokenType::LtEq, "<="},
               {TokenType::Gt, ">"},
               {TokenType::GtEq, ">="}});
    expectLex("|| + - * / %",
              {{TokenType::Concat, "||"},
               {TokenType::Plus, "+"},
               {TokenType::Minus, "-"},
               {TokenType::Star, "*"},
               {TokenType::Slash, "/"},
               {TokenType::Percent, "%"}});
}

void punctuationBecomesSingleCharacterTokens() {
    expectLex("( ) , ; .",
              {{TokenType::LParen, "("},
               {TokenType::RParen, ")"},
               {TokenType::Comma, ","},
               {TokenType::Semicolon, ";"},
               {TokenType::Dot, "."}});
}

void lineAndColumnPointToTokenStartAcrossBlankLines() {
    const std::vector<Token> tokens = lex("a\n  b\n\nc");
    CHECK(tokens.size() == 4);
    if (tokens.size() == 4) {
        CHECK(tokens[0].line == 1 && tokens[0].col == 1);
        CHECK(tokens[1].line == 2 && tokens[1].col == 3);
        CHECK(tokens[2].line == 4 && tokens[2].col == 1);
    }
}

void createTableStatementProducesExpectedTokenSequence() {
    expectLex("CREATE TABLE users (id INTEGER, name TEXT)",
              {{TokenType::Create, "CREATE"},
               {TokenType::Table, "TABLE"},
               {TokenType::Identifier, "users"},
               {TokenType::LParen, "("},
               {TokenType::Identifier, "id"},
               {TokenType::KwInteger, "INTEGER"},
               {TokenType::Comma, ","},
               {TokenType::Identifier, "name"},
               {TokenType::KwText, "TEXT"},
               {TokenType::RParen, ")"}});
}

void invalidCharactersProduceErrorTokenAndStatus() {
    std::vector<Token> tokens;
    CHECK(tokenize("@", tokens) == errorId_t::ERR_TOKINIZER_INVALID_ENTRY);
    CHECK(tokens.size() == 1);
    CHECK(tokens[0].type == TokenType::Error);
    CHECK(tokens[0].text == "unexpected character: @");

    tokens.clear();
    CHECK(tokenize("!", tokens) == errorId_t::ERR_TOKINIZER_INVALID_ENTRY);
    CHECK(tokens.size() == 1);
    CHECK(tokens[0].type == TokenType::Error);
    CHECK(tokens[0].text == "unexpected '!'");

    tokens.clear();
    CHECK(tokenize("|", tokens) == errorId_t::ERR_TOKINIZER_INVALID_ENTRY);
    CHECK(tokens.size() == 1);
    CHECK(tokens[0].type == TokenType::Error);
    CHECK(tokens[0].text == "unexpected '|'");
}

void firstErrorStopsScanningAndOmitsEof() {
    std::vector<Token> tokens;
    CHECK(tokenize("SELECT @ FROM t", tokens) ==
          errorId_t::ERR_TOKINIZER_INVALID_ENTRY);
    CHECK(tokens.size() == 2);
    if (tokens.size() == 2) {
        CHECK(tokens[0].type == TokenType::Select);
        CHECK(tokens[1].type == TokenType::Error);
        CHECK(tokens[1].col == 8);
    }
}

void preexistingTokensCauseNotEmptyErrorWithoutModification() {
    std::vector<Token> tokens{Token{TokenType::Integer, "keep", 1, 1}};
    CHECK(tokenize("SELECT", tokens) == errorId_t::ERR_TOKINIZER_NOT_EMPTY);
    CHECK(tokens.size() == 1);
    CHECK(tokens[0].text == "keep");
}

void leadingDecimalPointProducesFloatToken() {
    expectLex(".5", {{TokenType::Float, ".5"}});
}

void lettersAfterDigitsAreRejectedAsMalformedNumber() {
    std::vector<Token> tokens;
    CHECK(tokenize("123abc", tokens) == errorId_t::ERR_TOKINIZER_INVALID_ENTRY);
    CHECK(tokens.size() == 1);
    CHECK(tokens[0].type == TokenType::Error);
    CHECK(tokens[0].text == "malformed number: 123a");
}

void exponentNotationProducesFloatToken() {
    expectLex("1e3", {{TokenType::Float, "1e3"}});
}

void trailingDecimalPointStillProducesFloat() {
    expectLex("123.", {{TokenType::Float, "123."}});
}

void equalsAndDoubleEqualsBothBecomeEq() {
    expectLex("=", {{TokenType::Eq, "="}});
    expectLex("==", {{TokenType::Eq, "=="}});
}

void quotedStringsBecomeStringTokensWithDecodedText() {
    expectLex("'Fawzi'", {{TokenType::String, "Fawzi"}});
    expectLex("'it''s'", {{TokenType::String, "it's"}});
    expectLex("''", {{TokenType::String, ""}});
}

void blobAndParameterLiteralsHaveDedicatedTokenTypes() {
    expectLex("X'FF00'", {{TokenType::Blob, "FF00"}});
    expectLex("?", {{TokenType::Parameter, "?"}});
    expectLex("NULL", {{TokenType::Null, "NULL"}});
}

void dashDashCommentsAreSkipped() {
    expectLex("-- comment\nSELECT 1",
              {{TokenType::Select, "SELECT"}, {TokenType::Integer, "1"}});
}

void eofTokenCarriesEndPosition() {
    const std::vector<Token> tokens = lex("a\nb");
    CHECK(tokens.size() == 3);
    CHECK(tokens.back().line == 2);
    CHECK(tokens.back().col == 2);
}

} // namespace

int main() {
    emptyInputProducesOnlyEofToken();
    keywordsMatchCaseInsensitivelyAndKeepOriginalLexeme();
    identifiersAllowLeadingUnderscoreDigitsAndUnderscores();
    integerAndFloatTokensKeepTheirRawText();
    operatorsRecognizeSingleAndTwoCharacterForms();
    punctuationBecomesSingleCharacterTokens();
    lineAndColumnPointToTokenStartAcrossBlankLines();
    createTableStatementProducesExpectedTokenSequence();
    invalidCharactersProduceErrorTokenAndStatus();
    firstErrorStopsScanningAndOmitsEof();
    preexistingTokensCauseNotEmptyErrorWithoutModification();
    leadingDecimalPointProducesFloatToken();
    lettersAfterDigitsAreRejectedAsMalformedNumber();
    exponentNotationProducesFloatToken();
    trailingDecimalPointStillProducesFloat();
    equalsAndDoubleEqualsBothBecomeEq();
    quotedStringsBecomeStringTokensWithDecodedText();
    blobAndParameterLiteralsHaveDedicatedTokenTypes();
    dashDashCommentsAreSkipped();
    eofTokenCarriesEndPosition();

    if (failures != 0) {
        std::cerr << "tokenizer_test: " << failures << " check(s) failed\n";
        return 1;
    }
    std::cout << "tokenizer_test: PASSED\n";
    return 0;
}
