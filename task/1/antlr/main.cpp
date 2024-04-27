#include "SYsULexer.h" // 确保这里的头文件名与您生成的词法分析器匹配
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <regex>

// 映射定义，将ANTLR的tokenTypeName映射到clang的格式
std::unordered_map<std::string, std::string> tokenTypeMapping = {
  { "Int", "int" },
  { "Identifier", "identifier" },
  { "LeftParen", "l_paren" },
  { "RightParen", "r_paren" },
  { "RightBrace", "r_brace" },
  { "LeftBrace", "l_brace" },
  { "LeftBracket", "l_square" },
  { "RightBracket", "r_square" },
  { "Constant", "numeric_constant" },
  { "Return", "return" },
  { "Semi", "semi" },
  { "EOF", "eof" },
  { "Equal", "equal" },
  { "Plus", "plus" },
  { "Comma", "comma" },
  // 在这里继续添加其他映射
  { "Const", "const" },
  { "Void", "void" },
  { "If", "if" },
  { "Else", "else" },
  { "While", "while" },
  { "Break", "break" },
  { "Continue", "continue" },
  { "Greater", "greater" },
  { "Less", "less" },
  { "Ampamp", "ampamp" }, 
  { "Pipepipe", "pipepipe" }, 
  { "Exclaim", "exclaim" }, 
  { "Equalequal", "equalequal" },
  { "Exclaimequal", "exclaimequal" },
  { "Greaterequal", "greaterequal" },
  { "Lessequal", "lessequal" },
  { "Minus", "minus" },
  { "Star", "star" },
  { "Div", "div" },
  { "Percent", "percent" },
};

// 记录最新行数
int roll_count = 0;
// 记录最新文件地址
std::string cfile_name = "";
// 记录前置空格是否存在
bool if_space = false;
// 记录换行操作是否存在
bool if_newline = false;

void
print_token(const antlr4::Token* token,
            const antlr4::CommonTokenStream& tokens,
            std::ofstream& outFile,
            const antlr4::Lexer& lexer)
{
  auto& vocabulary = lexer.getVocabulary();

  auto tokenTypeName =
    std::string(vocabulary.getSymbolicName(token->getType()));

  if (tokenTypeName.empty())
    tokenTypeName = "<UNKNOWN>"; // 处理可能的空字符串情况

  if (tokenTypeMapping.find(tokenTypeName) != tokenTypeMapping.end()) {
    tokenTypeName = tokenTypeMapping[tokenTypeName];
  }

  // 考虑出现预处理信息的情况
  if (tokenTypeName=="LineAfterPreprocessing")
  {
    int num_catch = 2;
    std::string roll_count_ = "";
    std::string cfile_name_ = "";
    for (auto ch : token->getText())
    {
      if (ch >= '0' && ch <= '9')
      {
        if (num_catch >= 1)
        {
          num_catch = 1;
          roll_count_ += ch;
        }
      }
      else
      {
        if (num_catch == 1)
        num_catch = 0;
      }
    }
    roll_count = std::stoi(roll_count_)-1;

    bool addr_catch = false;
    for (auto ch : token->getText())
    {
      if (ch == '\"')
      {
        addr_catch = addr_catch ? false : true;
        continue;
      }
      if (addr_catch)
      cfile_name_ += ch;
    }
    cfile_name = cfile_name_;
    if_newline = true;
    return;
  }
  // 考虑出现前置空格的情况
  if (tokenTypeName=="Whitespace")
  {
    if_space = true;
    return;
  }
  // 考虑出现换行的情况
  if (tokenTypeName=="Newline")
  {
    roll_count ++;
    if_newline = true;
    return;
  }

  // 接下来考虑的都是出现有效token的情况
  std::string locInfo = " Loc=<";
  locInfo += cfile_name;
  locInfo += ":";
  //locInfo += std::to_string(token->getLine()-preroll_count);
  locInfo += std::to_string(roll_count);
  locInfo += ":";
  if (token->getText() != "<EOF>")
  locInfo += std::to_string(token->getCharPositionInLine()+1);
  else locInfo += std::to_string(token->getCharPositionInLine());
  locInfo += ">";

  bool startOfLine = false;
  bool leadingSpace = false;
  if (if_space)
  {
    if_space = false;
    leadingSpace = true;
  }
  if (if_newline)
  {
    if_newline = false;
    startOfLine = true;
  }

  if (token->getText() != "<EOF>")
    outFile << tokenTypeName << " '" << token->getText() << "'";
  else
    outFile << tokenTypeName << " '"
            << "'" ;
  if (startOfLine)
    outFile << "\t [StartOfLine]";
  if (leadingSpace)
    outFile << " [LeadingSpace]";
  outFile << locInfo << std::endl;
}

int
main(int argc, char* argv[])
{
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " <input> <output>\n";
    return -1;
  }

  std::ifstream inFile(argv[1]);
  if (!inFile) {
    std::cout << "Error: unable to open input file: " << argv[1] << '\n';
    return -2;
  }

  std::ofstream outFile(argv[2]);
  if (!outFile) {
    std::cout << "Error: unable to open output file: " << argv[2] << '\n';
    return -3;
  }

  std::cout << "程序 '" << argv[0] << std::endl;
  std::cout << "输入 '" << argv[1] << std::endl;
  std::cout << "输出 '" << argv[2] << std::endl;

  antlr4::ANTLRInputStream input(inFile);
  SYsULexer lexer(&input);

  antlr4::CommonTokenStream tokens(&lexer);
  tokens.fill();

  
  for (auto&& token : tokens.getTokens()) {
    print_token(token, tokens, outFile, lexer);
  }
  //outFile << lexer.getGrammarFileName();
}
