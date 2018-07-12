@.str = constant [13 x i8] c"Hello, world\00", align 1

define i32 @main() {
  call i32 @puts(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @.str, i64 0, i64 0))
  ret i32 0
}

declare i32 @puts(i8*)
