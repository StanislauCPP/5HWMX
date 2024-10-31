add_library(matrix matrix.hpp)
set_target_properties(matrix PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(matrix PRIVATE cxx_std_17)
target_include_directories(matrix INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

option(TESTLIB "activate test matrix.hpp")

if(TESTLIB)
	cmake_minimum_required(VERSION 3.15)

	project(testmatrix)
	set(projTarg ${PROJECT_NAME}.x)
	add_executable(${projTarg} ${PROJECT_NAME}.cpp)
	target_link_libraries(${projTarg} matrix)

	enable_testing()

	option(UNITTEST "testing single functions")
	if(UNITTEST)
		target_compile_definitions(matrix INTERFACE UNITTEST)

		set(EXCOMMAND "exitStatus=$(echo $?); exit $exitStatus")
		add_test(NAME WithoutPar COMMAND bash -c "./${projTarg} | grep \"number\"; ${EXCOMMAND}")
		set_tests_properties(WithoutPar PROPERTIES DEPENDS ${projTarg} WILL_FAIL FALSE)

		function(do_test test_name test_par reg_val)
			add_test(NAME ${test_name}${test_par} COMMAND bash -c "./${projTarg} ${test_par} | grep \"${reg_val}\"; ${EXCOMMAND}")
			set_tests_properties(${test_name}${test_par} PROPERTIES DEPENDS ${projTarg} WILL_FAIL FALSE)
		endfunction()
	
		do_test(${PROJECT_NAME} 0 "([a-zA-Z]*, [a-zA-Z]*)")
		do_test(${PROJECT_NAME} 1 "[^&]&)")
		do_test(${PROJECT_NAME} 2 "&&)")
		do_test(${PROJECT_NAME} 3 "=(const.*&)")
		do_test(${PROJECT_NAME} 4 "=(.*&&)")
	endif()

	option(DET "testing determinant function")
	if(DET)
		target_compile_definitions(matrix INTERFACE DET)

		set(ANS -23 26 -105 1 1 2.52163 -2.52163 1.02688 7.00014)

		list(LENGTH ANS LENGTHANS)
		math(EXPR LENGTHANS ${LENGTHANS}-1)
		foreach(i RANGE ${LENGTHANS})
			list(GET ANS ${i} ANSOUT)
			add_test(NAME Matrix${i} COMMAND bash -c "./${projTarg} < ../tests/t${i}.test")
			set_tests_properties(Matrix${i} PROPERTIES DEPENDS ${projTarg} PASS_REGULAR_EXPRESSION "${ANSOUT}")
		endforeach()
	endif()


endif()