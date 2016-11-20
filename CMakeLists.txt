cmake_minimum_required(VERSION 2.6)
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(Ensithreadsvideo)
enable_testing()

set(CMAKE_BUILD_TYPE Debug)

#########
# Gestion des variantes
#########
# Vous devez editer les lignes suivantes pour y mettre vos logins
#########
set(LOGINS filippan grozir)

# Vérification du renseignement des login
list(SORT LOGINS)
if (LOGINS MATCHES "login[123]")
  message(FATAL_ERROR "** ERREUR **: Vous devez modifier CMakeLists.txt pour y mettre vos logins")
endif(LOGINS MATCHES "login[123]")

# Trouver la SDL2, ogg, theora et vorbis avec pkgconfig
execute_process(COMMAND pkg-config --cflags ogg vorbis theora theoradec sdl2 OUTPUT_VARIABLE ALL_PKG_CFLAGS)
execute_process(COMMAND pkg-config --libs ogg vorbis theora theoradec sdl2 OUTPUT_VARIABLE ALL_PKG_LDFLAGS)
string(STRIP ${ALL_PKG_LDFLAGS} ALLSTRIP_PKG_CFLAGS)
string(STRIP ${ALL_PKG_LDFLAGS} ALLSTRIP_PKG_LDFLAGS)


# Passer le compilateur C à C11 (gnu11) et donner les CFLAGS pour la sdl
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${ALLSTRIP_PKG_CFLAGS} -Wall -Werror -std=gnu11")


##
# Si vous utilisez d'autres fichiers pour votre lecteur video il faut
# les ajouter ici
##

add_executable(ensivideo src/ensivideo.c src/ensitheora.c src/ensivorbis.c src/oggstream.c src/stream_common.c src/synchro.c)

target_link_libraries(ensivideo pthread ${ALLSTRIP_PKG_LDFLAGS})

##
# Construction de l'archive
##
string(REPLACE ";" "-" LOGINS_SANS_POINTVIRGULE "${VARIANTE_LOGINS}")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH ${LOGINS_SANS_POINTVIRGULE})
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_IGNORE_FILES
  "~$"
  "\\\\.o$"
  "^${PROJECT_SOURCE_DIR}/build/"
  "^${PROJECT_SOURCE_DIR}/.git/"
)
include(CPack)
