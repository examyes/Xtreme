
SET (CMAKE_CXX_FLAGS "-std=c++11")

SET (XASM_SRC
  main.cpp
  data/FunctionTable.cpp
  data/InstrLookupTable.cpp
  data/InstrStream.cpp
  data/LabelTable.cpp
  data/SourceCodeHolder.cpp
  data/StringTable.cpp
  data/SymbolTable.cpp
  data/TokenStream.cpp
  data/XseData.cpp
  parse/LexicalAnalyzer.cpp
  parse/PreProcessor.cpp
  parse/SourceLoader.cpp
  parse/SyntaxParser.cpp
  utils/ErrorReporter.cpp
  utils/StringUtils.cpp
  )

include_directories(../../3rd/liter)

ADD_EXECUTABLE(xasm ${XASM_SRC})
