add_subdirectory(src)

# Use this directory as the PX4 module
if(COMMAND px4_add_module)
	px4_add_module(
		MODULE lib__DriverFramework__framework
		SRCS
			px4/df_dummy.cpp
		DEPENDS
			platforms__common
		)

	target_link_libraries(lib__DriverFramework__framework df_driver_framework)
endif()

# vim: set noet fenc=utf-8 ff=unix ft=cmake :
