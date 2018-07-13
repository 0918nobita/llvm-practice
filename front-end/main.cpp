#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// 文字列を、指定した文字をセパレータとして分割し vector で返す
std::vector<std::string> split(const std::string &str, char sep) {
  std::vector<std::string> v;
  std::stringstream ss(str);
  std::string buffer;
  while ( std::getline(ss, buffer, sep) ) v.push_back(buffer);
  return v;
}

// vector<string> とデリミタを引数に取り、各要素を結合した文字列を返す
std::string join(const std::vector<std::string> &v, const char &delim = 0) {
  std::string s;
  if ( !v.empty() ) {
    s += v[0];
    for (decltype(v.size()) i = 1, c = v.size(); i < c; i++) {
      if (delim) s += delim;
      s += v[i];
    }
  }
  return s;
}

int main() {
  std::vector<std::string> tokens = split("PRINT HELLO", ' ');
  std::cout << join(tokens, ',') << std::endl;
  /*std::ifstream ifs("sample.txt");
  if (ifs.fail()) {
    std::cout << "読み込みに失敗しました" << std::endl;
    return 1;
  }

  std::string str;
  std::string llvm_ir = "@.str = constant [13 x i8] c\"Hello, world\\00\", align 1\ndefine i32 @main() {\n";

  while (getline(ifs, str)) {
    std::vector<std::string> tokens = split(&str, ' ');
    
    std::cout << join(&tokens, ',') << std::endl;

    if (str == "HELLO") llvm_ir += "  call i32 @puts(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @.str, i64 0, i64 0))\n" ;
  }
  
  llvm_ir += "  ret i32 0\n}\ndeclare i32 @puts(i8*)\n";

  std::ofstream outputfile("sample.ll");
  outputfile << llvm_ir;
  outputfile.close();*/
  return 0;
}
