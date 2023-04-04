#include <iostream>
#include <string>
#include <cstdio>

int main() {
    std::string cmd = "php -r 'if(isset($_FILES[\"./index/video.mp4\"]) && $_FILES[\"./index/video.mp4\"][\"error\"] == UPLOAD_ERR_OK) {echo file_get_contents($_FILES[\"./index/video.mp4\"][\"tmp_name\"]);}'";

    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        std::cerr << "Failed to execute command: " << cmd << std::endl;
        return 1;
    }
    std::cout << pipe->_extra << std::endl;
    std::string video_data;
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe)) {
        std::cout << buffer << std::endl;
        video_data += buffer;
    }


    pclose(pipe);

    // process the video_data as needed

    return 0;
}
