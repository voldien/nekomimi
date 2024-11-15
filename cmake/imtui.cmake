
INCLUDE(FetchContent)

FetchContent_Declare(imtui_source
	GIT_REPOSITORY https://github.com/ggerganov/imtui.git
	GIT_TAG ec0d62d06175b369c31381e69f615cb5d337361d
) # or whatever tag you want

FetchContent_GetProperties(imtui_source)

IF(NOT imtui_source_POPULATED)
	FetchContent_Populate(imtui_source)
ELSE()
	#MESSAGE( WARNING "Could not find imtui_source source code")
ENDIF()

IF(imtui_source_POPULATED)
	ADD_SUBDIRECTORY(${imtui_source_SOURCE_DIR} ${imtui_source_BINARY_DIR} EXCLUDE_FROM_ALL)
ENDIF()
