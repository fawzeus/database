#ifndef PARSER_HPP
#define PARSER_HPP

// STATUS: placeholder — implement across Milestones 2-7.
// Responsibility: turn a token stream into a structured statement (AST):
//   CREATE TABLE ... -> TableSchema
//   INSERT INTO ...  -> target table + values
//   SELECT ...       -> columns, table, WHERE / ORDER BY / LIMIT
//
// Planned API (deliberately not declared yet so the AST can grow naturally):
//   struct Statement;
//   Statement parse(const std::vector<std::string>& tokens);

#endif /* PARSER_HPP */
