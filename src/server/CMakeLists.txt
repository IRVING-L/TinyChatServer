
aux_source_directory(. SRC_LIST)

aux_source_directory(./db DB_LIST)

aux_source_directory(./model MODEL_LIST)

aux_source_directory(./redis REDIS_LIST)

add_executable(server ${SRC_LIST} ${DB_LIST} ${MODEL_LIST} ${REDIS_LIST})

target_link_libraries(server muduo_net muduo_base mysqlclient hiredis pthread)