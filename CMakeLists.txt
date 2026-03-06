
cmake_minimum_required(VERSION 3.10)
project(ContactDirectory CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Para autocompletado del lsp de neovim
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(nlohmann_json QUIET REQUIRED)


add_executable(${PROJECT_NAME} main.cpp ./src/model/ContactList.cpp ./src/data/GestorJSON.cpp  ./src/model/ArbolB.cpp ./src/model/Contact.cpp ./src/data/GestorCSV.cpp ./src/view/Visualizador.cpp)


#Incluimos los headers para no usar paths relativos al importar
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Linking libraries
target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json )

