#pragma once
#include "Renderer.h"
#include "Pin.h"
#include "Cube.h"
#include "Camera.h"
#include "Plane.h"
#include "Sphere.h"
#include "Fish.h"
#include <GLFW/glfw3.h>

class Scene
{
public:
    Scene(GLFWwindow* window, float width, float height);
    ~Scene();

    void SetWindowSize(int width, int height);
    void OnUpdate(float deltaTime);
    void OnRender();
    void OnImGuiRender();
private:
    void InitializeModels();
    void SetCommonUniforms(Shader& shader);
    void DrawPins();
    void DrawCube(Cube& cube, const glm::vec3& light_pos);
    GLFWwindow* window;

    float m_width;
    float m_height;
    Renderer m_renderer;

    float m_fov = 45.0f;
    float m_near = 0.1f;
    float m_far = 100.0f;
    const glm::vec3 m_initial_cam_pos = glm::vec3(-7.5f, 6.5f, 5.38f);
    glm::vec3 m_cam_pos = m_initial_cam_pos;
    glm::vec3 m_light_pos1 = glm::vec3(-1.6f, 1.4f, 0.0f);
    glm::vec3 m_light_pos2 = glm::vec3(-1.6f, 1.4f, -3.0f);

    glm::mat4 m_proj;
    glm::mat4 m_view;    

    Pin m_pin;
    glm::mat4 m_pin_models[10];
    float m_pin_scale = 0.05f;    
    glm::vec3 m_pin_start_pos[10] = {
        glm::vec3(0.0f, 0.0f, 0.0f),

        glm::vec3(1.0f, 0.0f, -0.5f),
        glm::vec3(1.0f, 0.0f, 0.5f),

        glm::vec3(2.0f, 0.0f, -1.0f),
        glm::vec3(2.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 0.0f, 1.0f),

        glm::vec3(3.0f, 0.0f, -1.5f),
        glm::vec3(3.0f, 0.0f, -0.5f),
        glm::vec3(3.0f, 0.0f, 0.5f),
        glm::vec3(3.0f, 0.0f, 1.5f),

    };

    Cube m_cube1;
    Cube m_cube2;
    Plane m_plane;

    Sphere m_sphere;
    glm::vec3 m_sphere_pos = glm::vec3(-1.0f, 1.0f, -2.0f);
    //float m_sphere_scale = 0.1f;

    Fish m_fish;
    float m_spot_speed = 0.5f;
    float m_spot_scale = 0.05f;
    glm::vec3 m_spot_pos = glm::vec3(-4.6f, 1.4f, -2.0f);
    glm::vec3 m_spot_direction = glm::vec3(-0.2f, -1.0f, -0.3f);

    Camera m_camera;
    CameraType m_camera_type = CameraType::STATIONARY;
    bool m_look_at_spot = false;

    bool m_use_phong = false;
    enum class ShadingType
    {
        GOURAND = 0, FLAT = 1, PHONG = 2
    };
    ShadingType m_shading = ShadingType::PHONG;
    
    bool m_use_fog = false;
    float m_fog_density = 0.06f;
    glm::vec3 m_fog_color = glm::vec3(1.0f);

    Cube *m_day_lights;
    glm::vec3 m_day_light_pos[10] = {
        glm::vec3(0.0f, 5.0f, 0.0f),

        glm::vec3(1.0f, 5.0f, -0.5f),
        glm::vec3(1.0f, 5.0f, 0.5f),

        glm::vec3(2.0f, 5.0f, -1.0f),
        glm::vec3(2.0f, 5.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, 1.0f),

        glm::vec3(3.0f, 5.0f, -1.5f),
        glm::vec3(3.0f, 5.0f, -0.5f),
        glm::vec3(3.0f, 5.0f, 0.5f),
        glm::vec3(3.0f, 5.0f, 1.5f),
    };
    float m_day_light_speed = 0.5f;
    bool m_day_lights_enabled = true;
    bool m_move_y = false;
};
