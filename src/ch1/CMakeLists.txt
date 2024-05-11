add_library(ch1_lib ch1/ch1.cpp)					# .cpp file path

target_include_directories(ch1_lib PUBLIC .)		# path to catalog that contains
												# .hpp file
												# (not directly to .hpp file!)