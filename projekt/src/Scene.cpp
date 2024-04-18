#include "Scene.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "LightingUtils.h"
#include <GLFW/glfw3.h>

Scene::Scene(GLFWwindow* window, float width, float height) :
    m_width(width),
    m_height(height),
    m_renderer(),
    m_proj(glm::perspective(glm::radians(m_fov), 960.0f / 540.0f, m_near, m_far)),
    m_view(glm::translate(glm::mat4(1.0f), m_cam_pos)),
    m_pin(),
    m_cube1(),
    m_cube2(),
    m_sphere(),
    m_fish(),
    m_camera(m_cam_pos)
{
    this->window = window;
    InitializeModels();
    m_day_lights = new Cube[10];
    for (int i = 0; i < 10; i++)
    {
        m_day_light_pos[i].z *= 3;
        m_day_light_pos[i].x *= 2;
    }
}

void Scene::SetWindowSize(int width, int height)
{
    m_width = (float)width;
    m_height = (float)height;
}

void Scene::OnUpdate(float deltaTime)
{
    if (m_spot_speed > 0 && m_spot_pos.x >  3.5f)
        m_spot_speed = -m_spot_speed;

    else if (m_spot_speed < 0 && m_spot_pos.x < -2.0f)
        m_spot_speed = -m_spot_speed;
    m_spot_pos.x += deltaTime * m_spot_speed;


    if (!m_move_y && m_day_light_speed > 0 && m_day_light_pos[0].x > 6.5f)
    {
        m_move_y = true;
        m_day_light_speed = -m_day_light_speed;
    }

    if (m_move_y && m_day_light_pos[0].y < -0.5f)
    {
        m_day_light_speed = -m_day_light_speed;
        for (int i = 0; i < 10; i++)
        {
            m_day_light_pos[i].x -= 20.0f;
        }
    }

    if (m_move_y && m_day_light_speed > 0 && m_day_light_pos[0].y > 5.0f)
    {
        m_move_y = false;
    }

    for (int i = 0; i < 10; i++)
    {
        if (m_move_y)
        {
            m_day_light_pos[i].y += deltaTime * m_day_light_speed;
        }
        else
        {
            m_day_light_pos[i].x += deltaTime * m_day_light_speed;
        }   
    }
    
}

Scene::~Scene()
{
    delete[] m_day_lights;
}

void Scene::OnRender()
{
    m_renderer.Clear();
    float skyBrightness = 0.0f;
    // dodac 3.5
    if (m_day_light_pos[0].y > 0.1f)
    {
        skyBrightness = glm::sin(3.14 * (m_day_light_pos[0].x + 13.5) / 20.0f) / 2 +
            glm::sin(3.14 * m_day_light_pos[0].y / 10.0f) / 2;
    }
    GLCall(glClearColor(skyBrightness, skyBrightness, skyBrightness, 1.0f))

    m_proj = glm::perspective(glm::radians(m_fov), m_width / m_height, m_near, m_far);
    if (m_camera_type == CameraType::STATIONARY)
    {
        m_view = m_camera.GetViewMatrix(m_cam_pos);
    }
    else
    {
        if (m_camera_type == CameraType::FOLLOW)
        {
            m_cam_pos = glm::vec3(m_spot_pos.x - 1.0f, m_spot_pos.y + 1.2f, m_spot_pos.z - 0.05f);
        }
        m_view = m_camera.GetViewMatrix(m_cam_pos, m_spot_pos);
    }
    //m_view = m_camera.GetOscillatingViewMatrix(glfwGetTime(), m_cam_pos);

    // pins
    DrawPins();
    
    // cube
    DrawCube(m_cube1, m_light_pos1);
    DrawCube(m_cube2, m_light_pos2);
    for (int i = 0; i < 10; i++)
    {
        DrawCube(m_day_lights[i], m_day_light_pos[i]);
    }

    // sphere
    {
        Shader& shader = m_sphere.GetShader();
        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_sphere_pos);
        model = glm::scale(model, glm::vec3(0.2f));
        glm::mat4 mvp = m_proj * m_view * model;
        shader.Bind();
        SetCommonUniforms(shader);
        shader.SetUniformMat4f("u_MVP", mvp);
        shader.SetUniformMat4f("model", model);

        m_renderer.Draw(m_sphere.GetVertexArray(), m_sphere.GetIndexBuffer(), shader);
    }

    // spotlight - fish
    {
        Shader& shader = m_fish.GetShader();
        Texture& texture = m_fish.GetTexture();
        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_spot_pos);
        model = glm::scale(model, glm::vec3(m_spot_scale));
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        glm::mat4 mvp = m_proj * m_view * model;
        shader.Bind();
        texture.Bind();
        SetCommonUniforms(shader);
        shader.SetUniformMat4f("u_MVP", mvp);
        shader.SetUniformMat4f("model", model);

        m_renderer.Draw(m_fish.GetVertexArray(), m_fish.GetIndexBuffer(), shader);
    }

    // plane
    {
        Shader& shader = m_plane.GetShader();
        Texture& texture = m_plane.GetTexture();
        glm::mat4 mvp = m_proj * m_view /* model - jednostkowa*/;
        shader.Bind();
        texture.Bind();
        shader.SetUniformMat4f("u_MVP", mvp);                
        SetCommonUniforms(shader);
        
        m_renderer.Draw(m_plane.GetVertexArray(), m_plane.GetIndexBuffer(), shader);
    }
}

void Scene::DrawCube(Cube& cube, const glm::vec3& light_pos)
{
    Shader& shader = cube.GetShader();
    glm::mat4 model = glm::translate(glm::mat4(1.0f), light_pos);
    model = glm::scale(model, glm::vec3(0.1f));
    glm::mat4 mvp = m_proj * m_view * model;
    shader.Bind();
    shader.SetUniformMat4f("u_MVP", mvp);

    m_renderer.Draw(cube.GetVertexArray(), cube.GetIndexBuffer(), shader);
}

void Scene::InitializeModels()
{
    for (int i = 0; i < 10; i++)
    {
        // pin
        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_pin_start_pos[i]);
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        m_pin_models[i] = model;
    }
}

// zakladamy ze zbindowany juz
void Scene::SetCommonUniforms(Shader& shader)
{
    shader.SetUniform3f("viewPos", m_cam_pos.x, m_cam_pos.y, m_cam_pos.z);
    shader.SetUniform1i("usePhong", m_use_phong);
    shader.SetUniform3f("firstPointLight.position", m_light_pos1.x, m_light_pos1.y, m_light_pos1.z);
    shader.SetUniform3f("secondPointLight.position", m_light_pos2.x, m_light_pos2.y, m_light_pos2.z);
    shader.SetUniform3f("spotLight.position", m_spot_pos.x, m_spot_pos.y, m_spot_pos.z);
    shader.SetUniform3f("spotLight.direction", m_spot_direction.x, m_spot_direction.y, m_spot_direction.z);
    shader.SetUniform1i("useFog", m_use_fog);
    shader.SetUniform1f("fog.density", m_fog_density);
    shader.SetUniform3f("fog.color", m_fog_color.r, m_fog_color.g, m_fog_color.b);
    shader.SetUniform1i("useGourand", m_shading == ShadingType::GOURAND);
    shader.SetUniform1i("useFlat", m_shading == ShadingType::FLAT);
    for (int i = 0; i < 10; i++)
    {
        shader.SetUniform3f("dayLights[" + std::to_string(i) + "].position",
            m_day_light_pos[i].x, m_day_light_pos[i].y, m_day_light_pos[i].z);
    }
}

void Scene::DrawPins()
{
    Shader& shader = m_pin.GetShader();
    Texture& texture = m_pin.GetTexture();
    shader.Bind();
    texture.Bind();
    SetCommonUniforms(shader);
    for (int i = 0; i < 10; i++)
    {
        glm::mat4 model = glm::scale(m_pin_models[i], glm::vec3(m_pin_scale));

        glm::mat4 mvp = m_proj * m_view * model;
                
        shader.SetUniformMat4f("u_MVP", mvp);
        shader.SetUniformMat4f("model", model);
        
        m_renderer.Draw(m_pin.GetVertexArray(), m_pin.GetIndexBuffer(), shader);
    }
}

void Scene::OnImGuiRender()
{
    ImGui::SliderFloat("FOV", &m_fov, 0.0f, 90.0f);
    //ImGui::SliderFloat("Near", &m_near, 0.1f, 20.0f);
    //ImGui::SliderFloat("Far", &m_far, 25.0f, 300.0f);
    ImGui::SliderFloat("Pin scale", &m_pin_scale, 0.005f, 0.2f);
    ImGui::SliderFloat("Fish scale", &m_spot_scale, 0.005f, 0.1f);
    ImGui::SliderFloat3("Cam position", &m_cam_pos[0], -15.0f, 15.0f);
    ImGui::SliderFloat3("SpotPos", &m_spot_pos[0], -10.0f, 10.0f);
    ImGui::SliderFloat3("SpotDirection", &m_spot_direction[0], -10.0f, 10.0f);
    ImGui::SliderFloat3("LightPos1", &m_light_pos1[0], -10.0f, 10.0f);
    ImGui::SliderFloat3("LightPos2", &m_light_pos2[0], -10.0f, 10.0f);

    ImGui::Separator();
    int current = (int)m_shading;
    ImGui::RadioButton("Phong shading", &current, (int)ShadingType::PHONG); ImGui::SameLine();
    ImGui::RadioButton("Gourand", &current, (int)ShadingType::GOURAND); ImGui::SameLine();
    ImGui::RadioButton("Flat", &current, (int)ShadingType::FLAT);
    m_shading = (ShadingType)current;

    ImGui::Separator();
    current = (int)m_use_phong;
    ImGui::RadioButton("Phong lighting", &current, 1); ImGui::SameLine();
    ImGui::RadioButton("Blinn", &current, 0); 
    m_use_phong = (bool)current;

    ImGui::Separator();
    current = (int)m_camera_type;
    ImGui::RadioButton("Stationary", &current, (int)CameraType::STATIONARY); ImGui::SameLine();
    ImGui::RadioButton("Look at fish",&current, (int)CameraType::LOOK_AT); ImGui::SameLine();
    ImGui::RadioButton("Follow fish", &current, (int)CameraType::FOLLOW);
    m_camera_type = (CameraType)current;

    ImGui::Separator();
    ImGui::Checkbox("Use fog", &m_use_fog);
    ImGui::SliderFloat("Fog density", &m_fog_density, 0.001f, 0.2f);
    ImGui::SliderFloat3("Fog color", &m_fog_color[0], 0.0f, 1.0f);

    ImGui::Separator();
    ImGui::Checkbox("DayNight lights", &m_day_lights_enabled);
    if (ImGui::IsItemDeactivatedAfterEdit())
    {
        if (m_day_lights_enabled)
        {
            LightingUtils::EnableDayLights(m_pin.GetShader());
            LightingUtils::EnableDayLights(m_sphere.GetShader());
            LightingUtils::EnableDayLights(m_plane.GetShader());
            LightingUtils::EnableDayLights(m_fish.GetShader());
        }
        else
        {
            LightingUtils::DisableDayLights(m_pin.GetShader());
            LightingUtils::DisableDayLights(m_sphere.GetShader());
            LightingUtils::DisableDayLights(m_plane.GetShader());
            LightingUtils::DisableDayLights(m_fish.GetShader());
        }
    }
    //ImGui::SliderFloat3("Light -direction", &m_directional_light[0], -1.0f, 1.0f);
}