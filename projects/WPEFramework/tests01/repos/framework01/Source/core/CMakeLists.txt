add_library(FrameworkCore SHARED
   add01.cpp
   vector01.cpp
   bool01.cpp
   string01.cpp
)

set(PUBLIC_HEADERS
   add01.h
   bool01.h
   string01.h
   vector01.h
)

set_target_properties(FrameworkCore PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

install(TARGETS FrameworkCore 
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include/framework01/core
)

