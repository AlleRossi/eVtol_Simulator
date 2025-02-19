# CMakeList.txt: progetto CMake per eVtol_Simulator_v_1. Includere l'origine e definire
# qui la logica specifica del progetto.
#
cmake_minimum_required (VERSION 3.8)

# Abilita Ricaricamento rapido per i compilatori MSVC, se supportato.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("eVtol_Simulator_v_1")

# Aggiungere l'origine all'eseguibile di questo progetto.
add_executable (eVtol_Simulator_v_1 "eVtol_Simulator_v_1.cpp" "eVtol_Simulator_v_1.h" "eVtols_vehicles.h" "eVtol_Factory.cpp" "eVtol_Factory.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET eVtol_Simulator_v_1 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Aggiungere i test e, se necessario, installare le destinazioni.
