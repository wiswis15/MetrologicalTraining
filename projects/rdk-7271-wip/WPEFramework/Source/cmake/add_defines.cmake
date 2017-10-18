# Adds the definitions in defines to the compiler options in result, by prepending them with -D or /D depending on the platform
# defines. Input list variable containing definitions to be added, referenced by name (so X, not S{X} or "S{X}")
# result  Output list variable contining compiler options, to which the definitions are added, referenced by name (so X, not S{X} or "S{X}")
function(add_defines result defines)
    set(tmp)
    foreach(VAL ${${result}})
        list(APPEND tmp ${VAL})
    endforeach()

    foreach(VAL ${${defines}})
        if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG)
            list(APPEND tmp -D${VAL})
        else()
            list(APPEND tmp /D${VAL})
        endif()
    endforeach()
    set(${result} ${tmp} PARENT_SCOPE)
endfunction()

