#include <iostream>
#include <fstream>

int main() {
  std::ifstream ifs("sample.txt");
  if (ifs.fail()) {
    std::cout << "読み込みに失敗しました" << std::endl;
    return 1;
  }

  std::string str;
  std::string llvm_ir = "@.str = constant [13 x i8] c\"Hello, world\\00\", align 1\ndefine i32 @main() {\n";
    
  while (getline(ifs, str))
    if (str == "HELLO") llvm_ir += "  call i32 @puts(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @.str, i64 0, i64 0))\n" ;

  llvm_ir += "  ret i32 0\n}\ndeclare i32 @puts(i8*)\n";

  std::ofstream outputfile("sample.ll");
  outputfile << llvm_ir;
  outputfile.close();
  return 0;
}
