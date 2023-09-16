build_project() {
    local build_type="$1"
    
    rm -rf build
    rm -rf out
    # 创建并进入build目录
    mkdir build
    cd build  
    # 运行CMake
    cmake .. -DCMAKE_BUILD_TYPE=$1
        
    # 返回原始目录
    cd - || return
}
