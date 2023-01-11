find_program(LLVMOBJDUMP_EXECUTABLE NAMES llvm-objdump llvm-objdump-10 llvm-objdump-9 llvm-objdump-8 llvm-objdump-7 llvm-objdump-6.0 llvm-objdump-5.0 llvm-objdump-4.0 llvm-objdump-3.9)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LLVMObjDump
  DEFAULT_MSG
  LLVMOBJDUMP_EXECUTABLE)

SET_PACKAGE_PROPERTIES(LLVMObjDump PROPERTIES
  DESCRIPTION "llvm object file dumper"
)
