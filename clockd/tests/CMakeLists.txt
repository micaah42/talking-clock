cmake_minimum_required(VERSION 3.14)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

add_test(
	NAME clockd-pytest
	COMMAND ${Python3_EXECUTABLE} -m pytest ${CMAKE_CURRENT_SOURCE_DIR}/test_clockd.py
)
set_tests_properties(clockd-pytest PROPERTIES
	ENVIRONMENT "CLOCKD_TEST_TARGET=$<TARGET_FILE:clockd>"
)
