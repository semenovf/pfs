set(PFS_RECENT_CXX_STANDARD 98)

if (CMAKE_COMPILER_IS_GNUCXX)
    #--- C++98 ---
    CHECK_CXX_COMPILER_FLAG(-std=c++98 PFS_COMPILER_CAN_CXX98)

    if(PFS_COMPILER_CAN_CXX98)
        set(PFS_COMPILER_FLAG_CXX98 -std=c++98)
        set(PFS_RECENT_CXX_STANDARD 98)
    endif()

    #--- C++11 ---
    CHECK_CXX_COMPILER_FLAG(-std=c++11 PFS_COMPILER_CAN_CXX11)

    if(NOT PFS_COMPILER_CAN_CXX11)
        CHECK_CXX_COMPILER_FLAG(-std=c++0x PFS_COMPILER_CAN_CXX11)

        if(PFS_COMPILER_CAN_CXX11)
            set(PFS_COMPILER_FLAG_CXX11 -std=c++0x)
        endif()
    else()
        set(PFS_COMPILER_FLAG_CXX11 -std=c++11)
        set(PFS_RECENT_CXX_STANDARD 11)
    endif()

    #--- C++14 ---
    CHECK_CXX_COMPILER_FLAG(-std=c++14 PFS_COMPILER_CAN_CXX14)

    if(NOT PFS_COMPILER_CAN_CXX14)
        CHECK_CXX_COMPILER_FLAG(-std=c++1y PFS_COMPILER_CAN_CXX14)

        if(PFS_COMPILER_CAN_CXX14)
            set(PFS_COMPILER_FLAG_CXX14 -std=c++1y)
        endif()
    else()
        set(PFS_COMPILER_FLAG_CXX14 -std=c++14)
        set(PFS_RECENT_CXX_STANDARD 14)
    endif()

    #--- C++14 ---
    CHECK_CXX_COMPILER_FLAG(-std=c++17 PFS_COMPILER_CAN_CXX17)

    if(NOT PFS_COMPILER_CAN_CXX17)
        CHECK_CXX_COMPILER_FLAG(-std=c++1z PFS_COMPILER_CAN_CXX17)

        if(PFS_COMPILER_CAN_CXX17)
            set(PFS_COMPILER_FLAG_CXX17 -std=c++1z)
        endif()
    else()
        set(PFS_COMPILER_FLAG_CXX17 -std=c++17)
        set(PFS_RECENT_CXX_STANDARD 17)
    endif()
endif()

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD ${PFS_RECENT_CXX_STANDARD})
endif()
