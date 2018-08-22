
# returns a map containing all properties whose keys were matched by any of the specified regexes
function(map_pick_regex map)
  set(regexes ${ARGN})
  map_keys("${map}")
  ans(keys)
  set(pick_keys)
  foreach(regex ${regexex})
    foreach(key ${keys})
      if("${key}" MATCHES "${regex}")
        list(APPEND pick_keys "${key}")
      endforeach()
    endforeach()
  endforeach()
  list(REMOVE_DUPLICATES pick_keys)
  map_pick("${map}" ${pick_keys})
  return_ans()
endfunction()
