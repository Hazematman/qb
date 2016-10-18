#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <glm/glm.hpp>

class Camera {
    public:
        glm::vec3 position;
        glm::vec3 lookat;

        Camera();

        void move(float amount);
        void strafe(float amount);
        void turn(float pitch, float yaw);
        void rawTurn(float pitch, float yaw);

        glm::mat4 view();
    private:
        glm::vec3 right;
        glm::vec3 up;
        float pitch, yaw, speed;
        void updateLookat();
};

#endif
