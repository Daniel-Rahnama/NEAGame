#ifndef MAIN_HPP
#define MAIN_HPP

const unsigned int width = 800;
const unsigned int height = 800;
const unsigned int target_fps = 60;
const unsigned int target_frame_duration = 1000/target_fps;

const char* resources;

int main(int, char**);

void findResources();

#endif /* MAIN_HPP */