set(GeoIP_TESTS
    Init
    Invalid
    PrivateRangeIPv4
    PrivateRangeIPv6
    RealIPv4
    RealIPv6
)

set(Packets_TESTS
    CountUpdate
    MajorUpdate17
    MajorUpdate31
)

set(TEST_SCOPES
    GeoIP
    Packets
)

foreach(SCOPE_NAME ${TEST_SCOPES})
    foreach (TEST_NAME ${${SCOPE_NAME}_TESTS})
        set(EXE_NAME ${SCOPE_NAME}${TEST_NAME})
        add_executable(${EXE_NAME} ${SCOPE_NAME}/Test${TEST_NAME}.c)
        add_test(
            NAME ${SCOPE_NAME}/${TEST_NAME}
            COMMAND ${PROJECT_BINARY_DIR}/Source/Tests/${EXE_NAME}
        )

        target_link_libraries(${EXE_NAME}
            PRIVATE
                TrackerCommon
                Server
        )
    endforeach()
endforeach()
