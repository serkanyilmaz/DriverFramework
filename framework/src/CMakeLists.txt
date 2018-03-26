include(../../cmake/df_common.cmake)

if("${DF_TARGET}" STREQUAL "nuttx")
	add_library(df_driver_framework
		DriverFramework_NuttX.cpp
		DevMgr_Nuttx.cpp
		DFList.cpp
		SyncObj.cpp
		Time.cpp
		)
	add_dependencies(df_driver_framework platforms__nuttx)
else()
	df_add_library(df_driver_framework
		DriverFramework.cpp
		WorkMgr.cpp
		WorkItems.cpp
		DevMgr.cpp
		DevObj.cpp
		SyncObj.cpp
		DFList.cpp
		Time.cpp
		DFDiag.cpp
		I2CDevObj.cpp
		SPIDevObj.cpp
		ImuSensor.cpp
		BaroSensor.cpp
		MagSensor.cpp
		)
endif()

# vim: set noet fenc=utf-8 ff=unix ft=cmake :
